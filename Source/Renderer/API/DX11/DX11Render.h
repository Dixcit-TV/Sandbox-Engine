#pragma once
#include <map>
#include <queue>

#include "Platform/Platform.h"

namespace SDBX
{
	namespace Renderer
	{
		struct DrawCommand;
		struct DispatchCommand;

		using GpuResourceHandle = uint32_t;
	}
	
	namespace DX11
	{
		class DX11Render
		{
		public:
			explicit DX11Render() = default;
			~DX11Render();

			DX11Render(const DX11Render&) = delete;
			DX11Render(DX11Render&&) noexcept = delete;
			DX11Render& operator=(const DX11Render&) = delete;
			DX11Render& operator=(DX11Render&&) noexcept = delete;

			void Init(const Window& window, Viewport* viewports, size_t viewportCount = 1);
			void UpdateViewports(Viewport* viewports, size_t viewportCount = 1);
			
			void Draw(const Renderer::DrawCommand& drawCommand);
			void Dispatch(const Renderer::DispatchCommand& dispatchCommand);

			void Present() const { m_pDxSwapChain->Present(0, 0); }

		private:
			D3D11_VIEWPORT m_Viewports[4];
			HWND m_pWindow;

			//std::map<> m_DrawCommands;
			//std::map<> m_ComputeCommands;

			//std::queue<ICommand*> m_RenderQueue;

			ID3D11Device* m_pDxDevice;
			ID3D11DeviceContext* m_pDxDeviceContext;
			IDXGISwapChain* m_pDxSwapChain;
			std::vector<ID3D11UnorderedAccessView*> m_RenderTargets;
			std::vector<ID3D11UnorderedAccessView*> m_DepthStencils;


			ID3D11UnorderedAccessView* m_pRenderTargetUAV;
			ID3D11UnorderedAccessView* m_pDepthUAV;
		};
	}
}

