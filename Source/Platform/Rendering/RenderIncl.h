#pragma once
#include <type_traits>

#define SDBX_RENDER_DX11 0x00000000

#define SDBX_RENDER SDBX_RENDER_DX11

#if SDBX_RENDER == SDBX_RENDER_DX11
	#include <dxgi.h>
	#pragma comment(lib, "dxgi.lib")
	#include <d3d11.h>
	#pragma comment(lib, "d3d11.lib")
	#include <d3dcompiler.h>
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "dxguid.lib")


	namespace SDBX
	{
		template<typename RESOURCE_TYPE, typename = std::enable_if_t<std::is_convertible_v<RESOURCE_TYPE, IUnknown*>>>
		void SafeRelease(RESOURCE_TYPE& pResource)
		{
			if (pResource)
			{
				pResource->Release();
				pResource = nullptr;
			}
		}
	}
#endif // SDBX_RENDER_DX11