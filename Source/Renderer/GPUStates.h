#pragma once
#include <cstdint>

namespace SDBX
{
	namespace RenderStates
	{
		enum class OpacityState : uint8_t
		{
			Opaque		= 0
			, Alpha		= 1
		};

		enum class BlendState : uint32_t
		{
			AlphaToCoverage		= 0b01
			, Independent		= 0b10
		};

		enum class BlendOp : uint32_t
		{
			Add				= 1
			, Substract		= 2
			, RevSubstract	= 3
			, Min			= 4
			, Max			= 5
		};

		enum class BlendType : uint32_t
		{
			SrcColor			= 1
			, InvSrcColor		= 2
			, SrcAlpha			= 3
			, InvSrcAlpha		= 4
			, DestAlpha			= 5
			, InvDestAlpha		= 6
			, DestColor			= 7
			, InvDestColor		= 8
			, SrcAlphaSat		= 9
			, BlendFactor		= 10
			, InvBlendFactor	= 11
			, Src1Color			= 12
			, InvSrc1Color		= 13
			, Src1Alpha			= 14
			, InvSrc1Alpha		= 15
		};

		enum class RasterState : uint32_t
		{
			Fill				= 0
			, Wireframe			= 1
			, FrontCull			= 2
			, BackCull			= 3	
		};

		enum class DepthStencilState : uint32_t
		{
			DepthEnbaled		= 0b01
			, StencilEnbaled	= 0b10
		};

		enum class DepthStencilComparaison : uint32_t
		{
			Never			= 0
			, Less			= 1
			, Equal			= 2
			, LessEqual		= 3
			, Greater		= 4
			, NotEqual		= 5
			, GreaterEqual	= 6
			, Always		= 7
		};

		enum class StencilOp : uint32_t {
			Keep		= 0
			, Zero		= 1
			, Replace	= 2
			, IncrSat	= 3
			, DecrSat	= 4
			, Invert	= 5
			, Incr		= 6
			, Decr		= 7
		};

		//enum class StateOffset : uint32_t
		//{
		//	Opacity		= 0
		//	, Raster	= 1
		//	, Depth		= 3
		//};
	}
}

