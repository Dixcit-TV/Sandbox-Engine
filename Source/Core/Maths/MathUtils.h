#pragma once

namespace SDBX
{
	namespace Maths
	{
		constexpr static double PI = 3.141592653589793238462643383279;
		
		template<typename T>
		inline static T ToDegrees(T rad) { return rad * static_cast<T>(180.0 / PI); };
		template<typename T>
		inline static double ToRadians(T deg) { return deg * static_cast<T>(PI / 180.0); };

		template<typename U>
		using Enable_64_Type = std::enable_if_t<(sizeof(U) > sizeof(float))>;

		template<typename U>
		using Enable_32_Type = std::enable_if_t<(sizeof(U) <= sizeof(float))>;


	}
}