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
			explicit Vec(T... values) : data{ TypeName(values)...} {}
			
			static void Fill(TypeName val) { std::fill(std::begin(data), std::end(data), val); }

			inline static Vec<TypeName, N> Zero() { return Vec<TypeName, N>{TypeName(0)}; }

			template<typename T>
			bool operator ==(const Vec<T, N>& rhs) const { return std::equal(std::begin(data), std::end(data), std::begin(rhs.data)); }
			template<typename T>
			bool operator !=(const Vec<T, N>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, N>& rhs) 
			{ 
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] += TypeName(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator -=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] -= TypeName(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator *=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] *= TypeName(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator /=(const Vec<T, N>& rhs)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] /= TypeName(rhs.data[idx]);

				return (*this);
			}
			template<typename T>
			Vec& operator *=(T scalar)
			{
				for (int idx{ 0 }; idx < N; ++idx)
					data[idx] *= TypeName(scalar);

				return (*this);
			}
			template<typename T>
			Vec& operator /=(T scalar)
			{
				return (*this) *= 1 / scalar;
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
			TypeName x, y;

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y) : x{ x }, y{ y } {}
			explicit Vec(TypeName val) : Vec(val, val) {}
			explicit Vec(TypeName components[2]) : Vec(components[0], components[1]) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 2, AsVec>& v) : Vec(TypeName(v.x), TypeName(v.y)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 3, AsVec>& v) : Vec(TypeName(v.x), TypeName(v.y)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 4, AsVec>& v) : Vec(TypeName(v.x), TypeName(v.y)) {}

			inline static Vec<TypeName, 2> Zero() { return Vec<TypeName, 3>{TypeName(0)}; }

			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 2, AsVec>& v) { x = v.x; y = v.y; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 3, AsVec>& v) { x = v.x; y = v.y; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 4, AsVec>& v) { x = v.x; y = v.y; return *this; }

			template<typename T>
			bool operator ==(const Vec<T, 2, AsVector>& rhs) const { return x == TypeName(rhs.x) && y == TypeName(rhs.y); }
			template<typename T>
			bool operator !=(const Vec<T, 2, AsVector>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 2, AsVector>& rhs) { x += TypeName(rhs.x); y += TypeName(rhs.y); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 2, AsVector>& rhs) { x -= TypeName(rhs.x); y -= TypeName(rhs.y); return *this; }
			template<typename T>
			Vec& operator *=(const Vec<T, 2, AsVector>& rhs) { x *= TypeName(rhs.x); y *= TypeName(rhs.y); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 2, AsVector>& rhs) { x /= TypeName(rhs.x); y /= TypeName(rhs.y); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= TypeName(scalar); y *= TypeName(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / scalar); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; return *this; }
		};

		template<typename TypeName, bool AsVector>
		struct Vec<TypeName, 3, AsVector>
		{
			TypeName x, y, z;

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y, TypeName z) : x{ x }, y{ y }, z{ z } {}
			explicit Vec(TypeName val) : Vec(val, val, val) {}
			explicit Vec(TypeName components[3]) : Vec(components[0], components[1], components[2]) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 2, AsVec>& v, TypeName z = TypeName(0)) : Vec(TypeName(v.x), TypeName(v.y), z) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 3, AsVec>& v) : Vec(TypeName(v.x), TypeName(v.y), TypeName(v.z)) {}
			template<typename T, bool AsVec>
			explicit Vec(const Vec<T, 4, AsVec>& v) : Vec(TypeName(v.x), TypeName(v.y), TypeName(v.z)) {}

			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 2, AsVec>& v) { x = v.x; y = v.y; z = 0; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 3, AsVec>& v) { x = v.x; y = v.y; z = v.z; return *this; }
			template<typename T, bool AsVec>
			Vec& operator =(const Vec<T, 4, AsVec>& v) { x = v.x; y = v.y; z = v.z; return *this; }

			inline static Vec<TypeName, 3> Zero() { return Vec<TypeName, 3>{TypeName(0)}; }

			template<typename T>
			bool operator ==(const Vec<T, 3, AsVector>& rhs) const { return x == TypeName(rhs.x) && y == TypeName(rhs.y) && z == TypeName(rhs.z); }
			template<typename T>
			bool operator !=(const Vec<T, 3, AsVector>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 3, AsVector>& rhs) { x += TypeName(rhs.x); y += TypeName(rhs.y); z += TypeName(rhs.z); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 3, AsVector>& rhs) { x -= TypeName(rhs.x); y -= TypeName(rhs.y); z -= TypeName(rhs.z); return *this; }
			template<typename T>
			Vec& operator *=(const Vec<T, 3, AsVector>& rhs) { x *= TypeName(rhs.x); y *= TypeName(rhs.y); z *= TypeName(rhs.z); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 3, AsVector>& rhs) { x /= TypeName(rhs.x); y /= TypeName(rhs.y); z /= TypeName(rhs.z); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= TypeName(scalar); y *= TypeName(scalar); z *= TypeName(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / scalar); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; z <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; z >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; z &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; z |= scalar; return *this; }
		};

		template<typename TypeName>
		struct Vec<TypeName, 4>
		{
			TypeName x, y, z, w;

			explicit Vec() = default;
			explicit Vec(TypeName x, TypeName y, TypeName z, TypeName w) : x{ x }, y{ y }, z{ z }, w{ w } {}
			explicit Vec(TypeName val) : Vec(val, val, val, val) {}
			explicit Vec(TypeName components[4]) : Vec(components[0], components[1], components[2], components[4]) {}
			template<typename T>
			explicit Vec(const Vec<T, 3>& v, TypeName w = TypeName(0)) : Vec(TypeName(v.x), TypeName(v.y), TypeName(v.z), w) {}
			template<typename T>
			explicit Vec(const Vec<T, 2>& v, TypeName z = TypeName(0), TypeName w = TypeName(0)) : Vec(TypeName(v.x), TypeName(v.y), z, w) {}

			inline static Vec<TypeName, 4> Zero() { return Vec<TypeName, 4>{TypeName(0)}; }

			template<typename T>
			bool operator ==(const Vec<T, 4>& rhs) const { return x == TypeName(rhs.x) && y == TypeName(rhs.y) && z == TypeName(rhs.z) && w == TypeName(rhs.w); }
			template<typename T>
			bool operator !=(const Vec<T, 4>& rhs) const { return !(*this == rhs); }

			template<typename T>
			Vec& operator +=(const Vec<T, 4>& rhs) { x += TypeName(rhs.x); y += TypeName(rhs.y); z += TypeName(rhs.z); w += TypeName(rhs.w); return *this; }
			template<typename T>
			Vec& operator -=(const Vec<T, 4>& rhs) { x -= TypeName(rhs.x); y -= TypeName(rhs.y); z -= TypeName(rhs.z); w -= TypeName(rhs.w); return *this; }
			template<typename T>	
			Vec& operator *=(const Vec<T, 4>& rhs) { x *= TypeName(rhs.x); y *= TypeName(rhs.y); z *= TypeName(rhs.z); w *= TypeName(rhs.w); return *this; }
			template<typename T>
			Vec& operator /=(const Vec<T, 4>& rhs) { x /= TypeName(rhs.x); y /= TypeName(rhs.y); z /= TypeName(rhs.z); w /= TypeName(rhs.w); return *this; }
			template<typename T>
			Vec& operator *=(T scalar) { x *= TypeName(scalar); y *= TypeName(scalar); z *= TypeName(scalar); w *= TypeName(scalar); return *this; }
			template<typename T>
			Vec& operator /=(T scalar) { return (*this) *= (1 / scalar); }

			Vec& operator <<=(int scalar) { x <<= scalar; y <<= scalar; z <<= scalar; w <<= scalar; return *this; }
			Vec& operator >>=(int scalar) { x >>= scalar; y >>= scalar; z >>= scalar; w >>= scalar; return *this; }
			template<typename T>
			Vec& operator &=(T scalar) { x &= scalar; y &= scalar; z &= scalar; w &= scalar; return *this; }
			template<typename T>
			Vec& operator |= (T scalar) { x |= scalar; y |= scalar; z |= scalar, w |= scalar; return *this; }

			Vec operator ~() { return Vec(~x, ~y, ~z, ~w); }
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
		inline static void Normalize(Vec<U, N>& v) { v /= Length<U, N>(v); }
		template<typename U, int N, typename = Vec2_3_Enable<N>>
		inline static Vec<U, N> Normalize(const Vec<U, N>& v) { return v / Length<U, N>(v); }

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
		using Veci = Vec<uint32_t, N>;
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

		using Vec4f	= Vecf<4>;
		using Vec4d	= Vecd<4>;
		using Vec4l	= Vecl<4>;
		using Vec4i	= Veci<4>;
		using Vec4i_16 = Veci_16<4>;
		using Vec4i_8 = Veci_8<4>;
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
SDBX::Vector::Vec<U, N, AsVector>& operator >>=(const SDBX::Vector::Vec<U, N, AsVector>& v, int scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } >>= scalar; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator &=(const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } &= scalar; }
template<typename U, typename T, int N, bool AsVector>
SDBX::Vector::Vec<U, N, AsVector>& operator |= (const SDBX::Vector::Vec<U, N, AsVector>& v, T scalar) { return SDBX::Vector::Vec<U, N, AsVector>{ v } |= scalar; }
