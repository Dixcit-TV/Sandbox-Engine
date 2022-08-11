#pragma once

#include "Platform/Rendering/RenderIncl.h"

#define SDBX_PLATFORM_WINDOWS 0x000000010

#define SDBX_PLATFORM SDBX_PLATFORM_WINDOWS

#if SDBX_PLATFORM == SDBX_PLATFORM_WINDOWS
	#include "Platform/Target/Windows/Window.h"

	namespace SDBX 
	{
		using Viewport = Windows::Viewport;
		using Window = Windows::Window;
	}
#endif
