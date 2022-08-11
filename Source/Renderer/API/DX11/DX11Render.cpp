#include "pch.h"
#include "DX11Render.h"

#include "Core/Log/Logger.h"

SDBX::DX11::DX11Render::~DX11Render()
{
	SDBX::SafeRelease(m_pDepthUAV);
	SDBX::SafeRelease(m_pRenderTargetUAV);
	SDBX::SafeRelease(m_pDxSwapChain);
	SDBX::SafeRelease(m_pDxDeviceContext);
	SDBX::SafeRelease(m_pDxDevice);
}

void SDBX::DX11::DX11Render::Init(const Window& window, Viewport* viewports, size_t viewportCount)
{
	HRESULT res{ S_OK };

	//APP_LOG_ERROR(L"Window not initialized !", window.IsInitialized());
	UINT crDeviceFlag{ D3D11_CREATE_DEVICE_SINGLETHREADED };

#if defined(DEBUG) | defined(_DEBUG)
	crDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

	D3D_FEATURE_LEVEL featureLevel{ };

	res = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, crDeviceFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDxDevice, &featureLevel, &m_pDxDeviceContext);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Could not create DX11 device !");
	SDBX_W_ASSERT(featureLevel >= D3D_FEATURE_LEVEL_11_0, L"Supported feature level is below D3D_FEATURE_LEVEL_11_0 !");

	IDXGIDevice* dxgiDevice;
	res = m_pDxDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	SDBX_W_ASSERT(SUCCEEDED(res), L"Could not query DXGI device !");

	IDXGIAdapter* dxgiAdapter;
	res = dxgiDevice->GetAdapter(&dxgiAdapter);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Could not query DXGI adapter !");

	IDXGIFactory* dxgiFactory;
	res = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
	SDBX_W_ASSERT(SUCCEEDED(res), L"Could not query DXGI factory !");

	IDXGIOutput* tempOutput{};
	res = dxgiAdapter->EnumOutputs(0, &tempOutput);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to get temp IDXGIOutput from adapter !");

	UINT numOutput{ };
	res = tempOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, NULL, &numOutput, nullptr);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to request the number of Adapter outputs !");

	DXGI_MODE_DESC* bbDescs{ new DXGI_MODE_DESC[numOutput] };
	res = tempOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, NULL, &numOutput, bbDescs);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to request Adapter output list !");

	UINT bbDescIdx{ numOutput - 1 };

	for (UINT idx{}; idx < numOutput; ++idx)
	{
		const DXGI_MODE_DESC& desc{ bbDescs[idx] };
		if (desc.Width == window.GetWidth()
			&& desc.Height == window.GetHeight())
		{
			bbDescIdx = idx;
			break;
		}
	}

	SafeRelease(tempOutput);

	DXGI_SWAP_CHAIN_DESC swapDesc{};
	swapDesc.BufferDesc = bbDescs[bbDescIdx];
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;
	swapDesc.OutputWindow = window.GetHandle();

	delete[] bbDescs;

	UpdateViewports(viewports, viewportCount);

	res = dxgiFactory->CreateSwapChain(m_pDxDevice, &swapDesc, &m_pDxSwapChain);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to create swap chain !");

	ID3D11Texture2D* pbackBuffer;
	res = m_pDxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pbackBuffer));
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to query swap chain's back buffer !");

	D3D11_TEXTURE2D_DESC bbDesc{};
	pbackBuffer->GetDesc(&bbDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC rtUAVDesc{};
	rtUAVDesc.Format = bbDesc.Format;
	rtUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	rtUAVDesc.Texture2D.MipSlice = 0;

	res = m_pDxDevice->CreateUnorderedAccessView(pbackBuffer, &rtUAVDesc, &m_pRenderTargetUAV);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to create render target UAV !");

	D3D11_TEXTURE2D_DESC depthStencilDesc{ bbDesc };
	depthStencilDesc.Format = DXGI_FORMAT_R32_FLOAT;

	ID3D11Texture2D* pdepthBuffer;
	res = m_pDxDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pdepthBuffer);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to create depth buffer texture !");

	D3D11_UNORDERED_ACCESS_VIEW_DESC depthUAVDesc{};
	depthUAVDesc.Format = depthStencilDesc.Format;
	depthUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	depthUAVDesc.Texture2D.MipSlice = 0;

	res = m_pDxDevice->CreateUnorderedAccessView(pdepthBuffer, &depthUAVDesc, &m_pDepthUAV);
	SDBX_W_ASSERT(SUCCEEDED(res), L"Failed to create depth buffer UAV !");

	SafeRelease(pdepthBuffer);
	SafeRelease(pbackBuffer);
	SafeRelease(dxgiDevice);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiFactory);

	SDBX_W_LOG(INFO_LOG, L"Renderer Initialized !");
}

void SDBX::DX11::DX11Render::UpdateViewports(Viewport* viewports, size_t viewportCount)
{
	for (uint32_t vpIdx{ 0 }; vpIdx < viewportCount; ++vpIdx)
	{
		D3D11_VIEWPORT& vp{ m_Viewports[vpIdx] };
		vp.Width = viewports[vpIdx].width;
		vp.Height = viewports[vpIdx].height;
		vp.TopLeftX = viewports[vpIdx].topLeftX;
		vp.TopLeftY = viewports[vpIdx].topLeftY;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
	}
}