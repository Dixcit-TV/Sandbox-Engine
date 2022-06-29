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
			TypeName data[4];

			explicit Quaternion(TypeName real, TypeName i, TypeName j, TypeName k) : data{ real, i, j, k } {  }
			explicit Quaternion(TypeName angleRad, const Vector::Vec3<TypeName>& axis)
				: data{ }
			{
				TypeName halfAngle{ angleRad * 0.5f };
				TypeName s{ static_cast<TypeName>(sin(halfAngle)) };
				data[0] = static_cast<TypeName>(cos(halfAngle));
				data[1] = axis.x * s;
				data[2] = axis.y * s;
				data[3] = axis.z * s;
			}
			explicit Quaternion(TypeName i, TypeName j, TypeName k) : data{ static_cast<TypeName>(0), i, j, k } {  }

			inline T Real() const { return data[0]; };
			inline Vector::Point3<TypeName> ImgAsPoint() const { return Vector::Point3<TypeName>(data[1], data[2], data[3]); };
			inline Vector::Vec3<TypeName> ImgAsVector() const { return Vector::Vec3<TypeName>(data[1], data[2], data[3]); };

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
				data[0] /= static_cast<TypeName>(scalar);
				data[1] /= static_cast<TypeName>(scalar);
				data[2] /= static_cast<TypeName>(scalar);
				data[3] /= static_cast<TypeName>(scalar);
				return *this;
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
			Vector::Vec3<T> img{ q.data[1], q.data[2], q.data[3] };
			m[0] += Vector::Cross((img + img), (Vector::Cross(img, Elite::Vector<3, T>(m[0])) + q.data[0] * Elite::Vector<3, T>(m[0])));
			m[1] += Vector::Cross((img + img), (Vector::Cross(img, Elite::Vector<3, T>(m[1])) + q.data[0] * Elite::Vector<3, T>(m[1])));
			m[2] += Vector::Cross((img + img), (Vector::Cross(img, Elite::Vector<3, T>(m[2])) + q.data[0] * Elite::Vector<3, T>(m[2])));
		}

		template<typename T>
		inline static void Rotate(Matrix::Mat44<T>& m, float radians, const Vector::Vec3<T>& unitAxis) { Rotate(m, Quaternion<T>(radians, unitAxis)); }

		template<typename T>
		inline static void Rotate(Vector::Vec3<T>& v, const Quaternion<T>& q)
		{
			Vector::Vec3<T> img{ q.data[1], q.data[2], q.data[3] };
			v += Vector::Cross((img + img), (Vector::Cross(img, v) + q.data[0] * v));
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

