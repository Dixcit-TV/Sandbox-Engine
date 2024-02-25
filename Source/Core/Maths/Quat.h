#pragma once
#include "Core/Maths/Vec.h"
#include "Core/Maths/Mat.h"

namespace SDBX
{
	namespace Vector
	{
		template<typename TypeName>
		struct Quaternion
		{
			//Data Member
#pragma warning( push )
#pragma warning( disable : 4201 )
			union {
				TypeName data[4];
				struct { TypeName x, y, z, w; };
				struct { TypeName real; Vector::Vec3<TypeName> img };
			};
#pragma warning( pop )

			explicit Quaternion(TypeName real, TypeName i, TypeName j, TypeName k) : data{ real, i, j, k } {  }
			explicit Quaternion(TypeName angleRad, const Vector::Vec3<TypeName>& axis)
				: data{ }
			{
				const TypeName halfAngle{ angleRad * 0.5f };
				real = static_cast<TypeName>(cos(halfAngle));
				img = axis * static_cast<TypeName>(sin(halfAngle));
			}
			explicit Quaternion(TypeName i, TypeName j, TypeName k) : data{ static_cast<TypeName>(0), i, j, k } {  }
			explicit Quaternion(const Vector::Vec3<TypeName>& img) : img{ img } {  }

			inline T Real() const { return data[0]; };
			inline Vector::Point3<TypeName> ImgAsPoint() const { return Vector::Point3<TypeName>(img); };
			inline const Vector::Vec3<TypeName>& ImgAsVector() const { return img; };

			template<typename U>
			inline Quaternion& operator *=(U scalar)
			{
				data[0] *= static_cast<TypeName>(scalar);
				data[1] *= static_cast<TypeName>(scalar);
				data[2] *= static_cast<TypeName>(scalar);
				data[3] *= static_cast<TypeName>(scalar);
				return *this;
			}

			template<typename U>
			inline Quaternion& operator /=(U scalar)
			{
				TypeName invScalar{ 1 / static_cast<TypeName>(scalar) };
				return (*this) *= invScalar;
			}

			template<typename U>
			inline Quaternion operator *(const Quaternion<U> rhs) const
			{
				return Quaternion{ data[0] * static_cast<TypeName>(rhs.data[0]) - data[1] * static_cast<TypeName>(rhs.data[1]) - data[2] * static_cast<TypeName>(rhs.data[2]) - data[3] * static_cast<TypeName>(rhs.data[3])
					, data[0] * static_cast<TypeName>(rhs.data[1]) + data[1] * static_cast<TypeName>(rhs.data[0]) + data[2] * static_cast<TypeName>(rhs.data[3]) - data[3] * static_cast<TypeName>(rhs.data[2])
					, data[0] * static_cast<TypeName>(rhs.data[2]) + data[2] * static_cast<TypeName>(rhs.data[0]) - data[1] * static_cast<TypeName>(rhs.data[3]) + data[3] * static_cast<TypeName>(rhs.data[1])
					, data[0] * static_cast<TypeName>(rhs.data[3]) + data[3] * static_cast<TypeName>(rhs.data[0]) + data[1] * static_cast<TypeName>(rhs.data[2]) - data[2] * static_cast<TypeName>(rhs.data[1])};
			}
			template<typename U>
			inline Quaternion operator *(U scalar) const { return Quaternion{ *this } *= scalar; }
			template<typename U>
			inline Quaternion operator /(U scalar) const { return Quaternion{ *this } /= scalar; }
			template<typename U>
			inline Quaternion operator *(U scalar) const { return Quaternion{ *this } *= scalar; }
			template<typename U>
			inline Quaternion operator *(U scalar) const { return Quaternion{ *this } *= scalar; }
		};

		template<typename T>
		inline static T SqrMagnitude(const Quaternion<T>& q) { return q.data[0] * q.data[0] + q.data[1] * q.data[1] + q.data[2] * q.data[2] + q.data[3] * q.data[3]; }
		template<typename T, typename = Maths::Enable_64_Type<T>>
		inline static double Magnitude(const Quaternion<T>& q) { return sqrt(SqrMagnitude(q)); }
		template<typename T, typename = Maths::Enable_32_Type<T>>
		inline static float Magnitude(const Quaternion<T>& q) { return sqrtf(static_cast<float>(SqrMagnitude(q))); }
		template<typename T>
		inline static Quaternion<T> Conjugate(const Quaternion<T>& q) { return Quaternion<T>{ data[0], -data[1], -data[2], -data[3] }; }
		template<typename T>
		inline static Quaternion<T> Inverse(const Quaternion<T>& q) { return Conjugate(q) /= (data[0] * data[0] + SqrMagnitude(q)); }

		template<typename T>
		inline static void Normalize(Quaternion<T>& q) { q /= Magnitude(q); }

		//Rodriguez formula - wikipedia says it uses less calculation than qvq'
		template<typename T>
		inline static void Rotate(Matrix::Mat44<T>& m, const Quaternion<T>& q)
		{
			const Vector::Vec3<T> v1{ Vector::Vec3<T>(m[0]) }, v2{ Vector::Vec3<T>(m[1]) }, v3{ Vector::Vec3<T>(m[2]) };
			m[0] += Vector::Cross((q.img + q.img), (Vector::Cross(q.img, v1) + q.real * v1));
			m[1] += Vector::Cross((q.img + q.img), (Vector::Cross(q.img, v2) + q.real * v2));
			m[2] += Vector::Cross((q.img + q.img), (Vector::Cross(q.img, v3) + q.real * v3));
		}

		template<typename T>
		inline static void Rotate(Matrix::Mat44<T>& m, float radians, const Vector::Vec3<T>& unitAxis) { Rotate(m, Quaternion<T>(radians, unitAxis)); }

		template<typename T>
		inline static void Rotate(Vector::Vec3<T>& v, const Quaternion<T>& q)
		{
			v += Vector::Cross((q.img + q.img), (Vector::Cross(q.img, v) + q.real * v));
		}

		template<typename T>
		inline static void Rotate(Vector::Vec4<T>& v, const Quaternion<T>& q)
		{
			Vector::Vec3<T> v3{ v };
			Rotate(v3, q);
			v = Vector::Vec4<T>(v3, v.w);
		}

		template<typename T>
		inline static void Rotate(Vector::Vec3<T>& v, float radians, const Vector::Vec3<T>& unitAxis)
		{
			Rotate(v, Quaternion<T>(radians, unitAxis));
		}

		template<typename T>
		inline static void Rotate(Vector::Vec4<T>& v, float radians, const Vector::Vec3<T>& unitAxis)
		{
			Rotate(v, Quaternion<T>(radians, unitAxis));
		}
	}
}

