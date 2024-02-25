#pragma once
#include <algorithm>

#include "Core/Maths/MathUtils.h"

namespace SDBX
{
	namespace Vector
	{
		template<int N>
		using Vec2_3_Enable = std::enable_if_t<N == 2 || N == 3>;

		template<typename TypeName, int N, bool AsVector = true, typename = std::enable_if_t<std::is_arithmetic_v<TypeName>>>
		struct Vec
		{
			TypeName data[N];

			explicit Vec() = default;
			explicit Vec(TypeName values[N]) : data{} { memcpy(data, values, sizeof(TypeName) * N); }
			template<typename... T>
			explicit Vec(T&&... values) : data{ static_cast<TypeName>(values)... } {}

			static void Fill(TypeName val) { std::fill(std::begin(data), std::end(data), val); }

			inline static Vec<TypeName, N, AsVector> Zero() { return Vec<TypeName, N>{static_cast<TypeName>(0)}; }

			TypeName operator [](size_t index) const { return data[index]; }
			TypeName& operator [](size_t index) { return data[index]; }
			template<typename T>
			bool operator ==(const Vec<T, N, AsVector>& rhs) const { return std::equal(std::begin(data), std::end(data), std::begin(rhs.data), &Maths::Equals<TypeName>); }
			template<typename T>
			bool operator !=(const Vec<T, N, AsVector>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, N, AsVector>& rhs)
			{ 
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] += static_cast<TypeName>(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator -=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] -= static_cast<TypeName>(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator *=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] *= static_cast<TypeName>(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator /=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] /= static_cast<TypeName>(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator *=(T scalar)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] *= static_cast<TypeName>(scalar);

				return (*this);
			}
			template<typename T>
			Vec& operator /=(T scalar)
			{
				return (*this) *= 1 / static_cast<TypeName>(scalar);
			}

			Vec& operator <<=(int scalar) 
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] <<= scalar;

				return (*this);
			}
			Vec& operator >>=(int scalar)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] >>= scalar;

				return (*this);
			}
			template<typename T>
			Vec& operator &=(T scalar) 
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] &= scalar;

				return (*this);
			}
			template<typename T>
			Vec& operator |= (T scalar)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] |= scalar;

				return (*this);
			}
		};

		template<typename TypeName, bool AsVector>
		struct Vec<TypeName, 2, AsVector>
		{
#pragma warning( push )
#pragma warning( disable : 4201 )
			union {
				TypeName data[2];
				struct { TypeName x, y; };
			};
#pragma warning( pop )

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y) : x{ x }, y{ y } {}
			explicit Vec(TypeName val) : Vec(val, val) {}
			explicit Vec(TypeName components[2]) : Vec(components[0], components[1]) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 2, AsVec>& v) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 3, AsVec>& v) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 4, AsVec>& v) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y)) {}

			inline static Vec<TypeName, 2> Zero() { return Vec<TypeName, 3>{static_cast<TypeName>(0)}; }

			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 2, AsVec>& v) { x = v.x; y = v.y; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 3, AsVec>& v) { x = v.x; y = v.y; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 4, AsVec>& v) { x = v.x; y = v.y; return *this; }

			TypeName operator [](size_t index) const { return data[index]; }
			TypeName& operator [](size_t index) { return data[index]; }
			template<typename T>
			bool operator ==(const Vec<T, 2, AsVector>& rhs) const { return Maths::Equals<TypeName>(x, static_cast<TypeName>(rhs.x)) && Maths::Equals<TypeName>(y, static_cast<TypeName>(rhs.y)); }
			template<typename T>
			bool operator !=(const Vec<T, 2, AsVector>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 2, AsVector>& rhs) { x += static_cast<TypeName>(rhs.x); y += static_cast<TypeName>(rhs.y); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 2, AsVector>& rhs) { x -= static_cast<TypeName>(rhs.x); y -= static_cast<TypeName>(rhs.y); return *this; }
			template<typename T>
			Vec& operator *=(const Vec<T, 2, AsVector>& rhs) { x *= static_cast<TypeName>(rhs.x); y *= static_cast<TypeName>(rhs.y); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 2, AsVector>& rhs) { x /= static_cast<TypeName>(rhs.x); y /= static_cast<TypeName>(rhs.y); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= static_cast<TypeName>(scalar); y *= static_cast<TypeName>(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / static_cast<TypeName>(scalar)); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; return *this; }

			Vec operator ~() { return Vec(~x, ~y); }
			Vec operator -() { return Vec(-x, -y); }
		};

		template<typename TypeName, bool AsVector>
		struct Vec<TypeName, 3, AsVector>
		{
#pragma warning( push )
#pragma warning( disable : 4201 )
			union {
				TypeName data[3];
				struct { TypeName x, y, z; };
			};
#pragma warning( pop )

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y, TypeName z) : x{ x }, y{ y }, z{ z } {}
			explicit Vec(TypeName val) : Vec(val, val, val) {}
			explicit Vec(TypeName components[3]) : Vec(components[0], components[1], components[2]) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 2, AsVec>& v, TypeName z = static_cast<TypeName>(0)) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y), z) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 3, AsVec>& v) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y), static_cast<TypeName>(v.z)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 4, AsVec>& v) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y), static_cast<TypeName>(v.z)) {}

			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 2, AsVec>& v) { x = v.x; y = v.y; z = 0; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 3, AsVec>& v) { x = v.x; y = v.y; z = v.z; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 4, AsVec>& v) { x = v.x; y = v.y; z = v.z; return *this; }

			inline static Vec<TypeName, 3> Zero() { return Vec<TypeName, 3>{static_cast<TypeName>(0)}; }

			TypeName operator [](size_t index) const { return data[index]; }
			TypeName& operator [](size_t index) { return data[index]; }
			template<typename T>
			bool operator ==(const Vec<T, 3, AsVector>& rhs) const { return Maths::Equals<TypeName>(x, static_cast<TypeName>(rhs.x)) && Maths::Equals<TypeName>(y, static_cast<TypeName>(rhs.y)) && Maths::Equals<TypeName>(z, static_cast<TypeName>(rhs.z)); }
			template<typename T>
			bool operator !=(const Vec<T, 3, AsVector>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 3, AsVector>& rhs) { x += static_cast<TypeName>(rhs.x); y += static_cast<TypeName>(rhs.y); z += static_cast<TypeName>(rhs.z); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 3, AsVector>& rhs) { x -= static_cast<TypeName>(rhs.x); y -= static_cast<TypeName>(rhs.y); z -= static_cast<TypeName>(rhs.z); return *this; }
			template<typename T>
			Vec& operator *=(const Vec<T, 3, AsVector>& rhs) { x *= static_cast<TypeName>(rhs.x); y *= static_cast<TypeName>(rhs.y); z *= static_cast<TypeName>(rhs.z); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 3, AsVector>& rhs) { x /= static_cast<TypeName>(rhs.x); y /= static_cast<TypeName>(rhs.y); z /= static_cast<TypeName>(rhs.z); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= static_cast<TypeName>(scalar); y *= static_cast<TypeName>(scalar); z *= static_cast<TypeName>(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / static_cast<TypeName>(scalar)); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; z <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; z >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; z &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; z |= scalar; return *this; }

			Vec operator ~() { return Vec(~x, ~y, ~z); }
			Vec operator -() { return Vec(-x, -y, -z); }
		};

		template<typename TypeName>
		struct Vec<TypeName, 4>
		{
#pragma warning( push )
#pragma warning( disable : 4201 )
			union {
				TypeName data[4];
				struct { TypeName x, y, z, w; };
			};
#pragma warning( pop )

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y, TypeName z, TypeName w) : x{ x }, y{ y }, z{ z }, w{ w } {}
			explicit Vec(TypeName val) : Vec(val, val, val, val) {}
			explicit Vec(TypeName components[4]) : Vec(components[0], components[1], components[2], components[4]) {}
			template<typename T>
			explicit Vec(const Vec<T, 3>& v, TypeName w = static_cast<TypeName>(0)) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y), static_cast<TypeName>(v.z), w) {}
			template<typename T>
			explicit Vec(const Vec<T, 2>& v, TypeName z = static_cast<TypeName>(0), TypeName w = static_cast<TypeName>(0)) : Vec(static_cast<TypeName>(v.x), static_cast<TypeName>(v.y), z, w) {}

			inline static Vec<TypeName, 4> Zero() { return Vec<TypeName, 4>{static_cast<TypeName>(0)}; }

			TypeName operator [](size_t index) const { return data[index]; }
			TypeName& operator [](size_t index) { return data[index]; }
			template<typename T>
			bool operator ==(const Vec<T, 4>& rhs) const { return Maths::Equals<TypeName>(x, static_cast<TypeName>(rhs.x)) && Maths::Equals<TypeName>(y, static_cast<TypeName>(rhs.y)) && Maths::Equals<TypeName>(z, static_cast<TypeName>(rhs.z)) && Maths::Equals<TypeName>(w, static_cast<TypeName>(rhs.w)); }
			template<typename T>
			bool operator !=(const Vec<T, 4>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 4>& rhs) { x += static_cast<TypeName>(rhs.x); y += static_cast<TypeName>(rhs.y); z += static_cast<TypeName>(rhs.z); w += static_cast<TypeName>(rhs.w); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 4>& rhs) { x -= static_cast<TypeName>(rhs.x); y -= static_cast<TypeName>(rhs.y); z -= static_cast<TypeName>(rhs.z); w -= static_cast<TypeName>(rhs.w); return *this; }
			template<typename T>	
			Vec& operator *=(const Vec<T, 4>& rhs) { x *= static_cast<TypeName>(rhs.x); y *= static_cast<TypeName>(rhs.y); z *= static_cast<TypeName>(rhs.z); w *= static_cast<TypeName>(rhs.w); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 4>& rhs) { x /= static_cast<TypeName>(rhs.x); y /= static_cast<TypeName>(rhs.y); z /= static_cast<TypeName>(rhs.z); w /= static_cast<TypeName>(rhs.w); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= static_cast<TypeName>(scalar); y *= static_cast<TypeName>(scalar); z *= static_cast<TypeName>(scalar); w *= static_cast<TypeName>(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / scalar); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; z <<= scalar; w <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; z >>= scalar; w >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; z &= scalar; w &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; z |= scalar, w |= scalar; return *this; }

			Vec operator ~() { return Vec(~x, ~y, ~z, ~w); }
			Vec operator -() { return Vec(-x, -y, -z, -w); }
		};

		template<typename U, typename T>
		inline static U Dot(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return lhs.x * U(rhs.x) + lhs.y * U(rhs.y); }
		template<typename U, typename T>
		inline static U Dot(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs) { return lhs.x * U(rhs.x) + lhs.y * U(rhs.y) + lhs.z * U(rhs.z); }

		template<typename U, typename T>
		inline static U Cross(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return lhs.x * U(rhs.y) - lhs.y * U(rhs.x); }
		template<typename U, typename T>
		inline static Vec<U, 3> Cross(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs) { return Vec(lhs.y * U(rhs.z) - lhs.z * U(rhs.y), -lhs.x * U(rhs.z) + lhs.z * U(rhs.x), lhs.x * U(rhs.y) - lhs.y * U(rhs.x)); }

		template<typename U, typename T, typename = Maths::Enable_64_Type<U>>
		inline static double Angle(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return atan2(Cross<U, T>(lhs, rhs), Dot<U, T>(lhs, rhs)); }
		template<typename U, typename T, typename V, typename = Maths::Enable_64_Type<U>>
		inline static double Angle(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs, const Vec<V, 3>& planeNormal) { return atan2(Dot<U, V>(Cross<U, T>(lhs, rhs), planeNormal), Dot<U, T>(lhs, rhs)); }
		template<typename U, typename T, typename = Maths::Enable_32_Type<U>>
		inline static float Angle(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return atan2f(float(Cross<U, T>(lhs, rhs)), float(Dot<U, T>(lhs, rhs))); }
		template<typename U, typename T, typename V, typename = Maths::Enable_32_Type<U>>
		inline static float Angle(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs, const Vec<V, 3>& planeNormal) { return atan2f(float(Dot<U, V>(Cross<U, T>(lhs, rhs), planeNormal)), float(Dot<U, T>(lhs, rhs))); }

		template<typename U, typename T, typename = Maths::Enable_64_Type<U>>
		inline static double AngleDeg(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return Maths::ToDegrees<double>(Angle<U, T>(lhs, rhs)); }
		template<typename U, typename T, typename V, typename = Maths::Enable_64_Type<U>>
		inline static double AngleDeg(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs, const Vec<V, 3>& planeNormal) { return Maths::ToDegrees<double>(Angle<U, T, V>(lhs, rhs, planeNormal)); }
		template<typename U, typename T, typename = Maths::Enable_32_Type<U>>
		inline static float AngleDeg(const Vec<U, 2>& lhs, const Vec<T, 2>& rhs) { return  Maths::ToDegrees<float>(Angle<U, T>(lhs, rhs)); }
		template<typename U, typename T, typename V, typename = Maths::Enable_32_Type<U>>
		inline static float AngleDeg(const Vec<U, 3>& lhs, const Vec<T, 3>& rhs, const Vec<V, 3>& planeNormal) { return Maths::ToDegrees<float>(Angle<U, T, V>(lhs, rhs, planeNormal)); }

		template<typename U, int N, typename = Vec2_3_Enable<N>>
		inline static U LengthSquared(const Vec<U, N>& v) { return Dot<U, U>(v, v); }
		template<typename U, int N, typename = Vec2_3_Enable<N>, typename = Maths::Enable_64_Type<U>>
		inline static double Length(const Vec<U, N>& v) { return sqrt(LengthSquared<U, N>(v)); }
		template<typename U, int N, typename = Vec2_3_Enable<N>, typename = Maths::Enable_32_Type<U>>
		inline static float Length(const Vec<U, N>& v) { return sqrtf(float(LengthSquared<U, N>(v))); }

		template<typename U, int N, typename = Vec2_3_Enable<N>>
		inline static U Normalize(Vec<U, N>& v) 
		{ 
			U length{ Length<U, N>(v) }; 
			if (length <= std::numeric_limits<U>::epsilon())
				v /= length;
			else
			{
				v = Vec<U, N>();
				length = static_cast<U>(0);
			}
			return length;
		}
		template<typename U, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> Normalized(const Vec<U, N>& v) { return v / Length<U, N>(v); }

		template<typename U, typename T, int N, bool AsVector, typename = Vec2_3_Enable<N>>
		inline static U DistanceSquared(const Vec<U, N, AsVector>& lhs, const Vec<T, N, AsVector>& rhs) { return LengthSquared<U, N>(rhs - lhs); }
		template<typename U, typename T, int N, bool AsVector, typename = Vec2_3_Enable<N>>
		inline static U Distance(const Vec<U, N, AsVector>& lhs, const Vec<T, N, AsVector>& rhs) { return Length<U, N>(rhs - lhs); }

		template<typename U, typename T, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> Project(const Vec<U, N>& lhs, const Vec<T, N>& rhs) { return rhs * Dot<U, T>(lhs, rhs); }
		template<typename U, typename T, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> Reject(const Vec<U, N>& lhs, const Vec<T, N>& rhs) { return lhs - Project<U, T, N>(lhs, rhs); }
		template<typename U, typename T, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> Reflect(const Vec<U, N>& incident, const Vec<T, N>& normal) { return incident - 2 * Dot<U, T>(incident, normal) * normal; }
		template<typename U, typename T, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> ProjectUnorm(const Vec<U, N>& lhs, const Vec<T, N>& rhs) { return rhs * (Dot<U, T>(lhs, rhs) / Dot<U, U>(rhs, rhs)); }
		template<typename U, typename T, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> RejectUnorm(const Vec<U, N>& lhs, const Vec<T, N>& rhs) { return lhs - ProjectUnorm<U, T, N>(lhs, rhs); }

		template<typename U, typename T>
		inline static Vec<U, 3> ProjectOnPlane(const Vec<U, 3>& v, const Vec<T, 3>& planeNormal) { return Reject<U, T, 3>(v, planeNormal); }

		template<int N, bool AsVector = true>
		using Vecf = Vec<float, N, AsVector>;
		template<int N, bool AsVector = true>
		using Vecd = Vec<double, N, AsVector>;
		template<int N, bool AsVector = true>
		using Vecl = Vec<uint64_t, N, AsVector>;
		template<int N, bool AsVector = true>
		using Veci = Vec<uint32_t, N, AsVector>;
		template<int N, bool AsVector = true>
		using Veci_16 = Vec<uint16_t, N, AsVector>;
		template<int N, bool AsVector = true>
		using Veci_8 = Vec<uint8_t, N, AsVector>;

		template<typename T>
		using Vec2 = Vec<T, 2, true>;
		template<typename T>
		using Point2 = Vec<T, 2, false>;

		template<typename T>
		using Vec3 = Vec<T, 3, true>;
		template<typename T>
		using Point3 = Vec<T, 3, false>;

		template<typename T>
		using Vec4 = Vec<T, 4>;

		using Vec2f = Vec2<float>;
		using Vec2d = Vec2<double>;
		using Vec2l = Vec2<uint64_t>;
		using Vec2i = Vec2<uint32_t>;
		using Vec2i_16 = Vec2<uint16_t>;
		using Vec2i_8 = Vec2<uint8_t>;

		using Vec3f = Vec3<float>;
		using Vec3d = Vec3<double>;
		using Vec3l = Vec3<uint64_t>;
		using Vec3i = Vec3<uint32_t>;
		using Vec3i_16 = Vec3<uint16_t>;
		using Vec3i_8 = Vec3<uint8_t>;

		using Point2f = Point2<float>;
		using Point2d = Point2<double>;
		using Point2l = Point2<uint64_t>;
		using Point2i = Point2<uint32_t>;
		using Point2i_16 = Point2<uint16_t>;
		using Point2i_8 = Point2<uint8_t>;

		using Point3f = Point3<float>;
		using Point3d = Point3<double>;
		using Point3l = Point3<uint64_t>;
		using Point3i = Point3<uint32_t>;
		using Point3i_16 = Point3<uint16_t>;
		using Point3i_8 = Point3<uint8_t>;

		using Vec4f	= Vec4<float>;
		using Vec4d	= Vec4<double>;
		using Vec4l	= Vec4<uint64_t>;
		using Vec4i	= Vec4<uint32_t>;
		using Vec4i_16 = Vec4<uint16_t>;
		using Vec4i_8 = Vec4<uint8_t>;
	}
}

template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator +(const SDBX::Vector::Vec<U, N, AsVector>& lhs, const SDBX::Vector::Vec<T, N, AsVector>& rhs) { return SDBX::Vector::Vec<U, N, AsVector>{ lhs } += rhs; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator -(const SDBX::Vector::Vec<U, N, AsVector>& lhs, const SDBX::Vector::Vec<T, N, AsVector>& rhs) { return SDBX::Vector::Vec<U, N, AsVector>{ lhs } -= rhs; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator *(const SDBX::Vector::Vec<U, N, AsVector>& lhs, const SDBX::Vector::Vec<T, N, AsVector>& rhs) { return SDBX::Vector::Vec<U, N, AsVector>{ lhs } *= rhs; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator /(const SDBX::Vector::Vec<U, N, AsVector>& lhs, const SDBX::Vector::Vec<T, N, AsVector>& rhs) { return SDBX::Vector::Vec<U, N, AsVector>{ lhs } /= rhs; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator *(const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } *= scalar; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector> operator /(const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } /= scalar; }
template<typename U, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator <<(const SDBX::Vector::Vec<U, N, AsVector>& v, int scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } <<= scalar; }
template<typename U, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator >>(const SDBX::Vector::Vec<U, N, AsVector>& v, int scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } >>= scalar; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator &(const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } &= scalar; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator |(const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } |= scalar; }
