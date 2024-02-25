#pragma once
#include <type_traits>

#include "Core\Log\Logger.h"
#include "Core\Maths\Vec.h"

namespace SDBX
{
	namespace Matrix
	{
		template<typename TypeName, int M, int N>
		struct Mat
		{
#pragma warning( push )
#pragma warning( disable : 4201 )
			union {
				TypeName data[N][M];
				struct { Vector::Vec<TypeName, M> V[N]; };
			};
#pragma warning( pop )

			explicit Mat() : data{ 0 } {};
			template<int O, int P>
			explicit Mat(TypeName values[P][O]) : Mat()
			{
				constexpr int m = O > M ? M : O;
				constexpr int n = P > N ? N : P;

				for (int n0{ 0 }; n0 < n; ++n0)
					for (int m0{ 0 }; m0 < m; ++m0)
						data[n0][m0] = static_cast<TypeName>(values[n0][m0]);
			}

			template<typename... ValType, typename = std::enable_if_t<std::is_same_v<TypeName, ValType>...>>
			explicit Mat(ValType&&... values) : Mat()
			{
				SDBX_STATIC_ASSERT(sizeof...(values) <= (N * M), "Too many arguments provided to initialize Matrix!");
				for (int n{ 0 }; n < N; ++n)
					for (int m{ 0 }; m < M; ++m)
						data[n][m] = static_cast<TypeName>((values, ...));
			}

			template<bool AsVector>
			explicit Mat(SDBX::Vector::Vec<TypeName, M, AsVector> vectors[N]) : Mat()
			{
				for (int n{ 0 }; n < N; ++n)
					memcpy(data[n], vectors[n].data, M);
			}

			template<typename T, int O, int P, typename = std::enable_if_t<O >= M && P >= N>>
			explicit Mat(const Mat<T, O, P>& m) : Mat()
			{
				for (int n{ 0 }; n < N; ++n)
					for (int m{ 0 }; m < M; ++m)
						data[n][m] = static_cast<TypeName>(m.data[n][m]);
			}

			template<typename T, int O, int P, typename = std::enable_if_t<O >= M && P >= N>>
			explicit Mat(Mat<T, O, P>&& m) : Mat()
			{
				for (int p{ 0 }; p < P; ++p)
					for (int o{ 0 }; o < O; ++o)
						data[p][o] = static_cast<TypeName>(m.data[p][o]);
			}
		
			template<typename = std::enable_if_t<M == N>>
			inline static Mat Identity() 
			{ 
				Mat mat{};
				for (int idx{ 0 }; idx < M; ++idx)
					mat.data[idx][idx] = static_cast<TypeName>(1);

				return mat;
			}

			template<typename T, int O, int P, typename = std::enable_if_t<O >= M && P >= N>>
			Mat& operator=(const Mat<T, O, P>& m)
			{
				for (int n{ 0 }; n < N; ++n)
					for (int m{ 0 }; m < M; ++m)
						data[n][m] = static_cast<TypeName>(m.data[n][m]);

				return *this;
			}

			template<typename T, int O, int P, typename = std::enable_if_t<O >= M && P >= N>>
			Mat& operator=(Mat<T, O, P>&& m)
			{
				for (int n{ 0 }; n < N; ++n)
					for (int m{ 0 }; m < M; ++m)
						data[n][m] = static_cast<TypeName>(m.data[n][m]);

				return *this;
			}

			template<typename T>
			bool operator ==(const Mat<T, M, N>& rhs) const 
			{ 
				bool equal = true;
				for (int m{ 0 }; m < M; ++m)
					for (int n{ 0 }; n < N; ++n)
						equal &= Maths::Equals<TypeName>(data[m][n], static_cast<TypeName>(rhs.data[m][n]));

				return equal;
			}
			template<typename T>
			bool operator !=(const Mat<T, M, N>& rhs) const { return !(*this == rhs); }

			template<typename T, int O>
			Mat<TypeName, M, O> operator *(const Mat<T, N, O>& rhs) const 
			{
				Mat m{ };
				for (int o{ 0 }; o < O; ++o)
					for (int m{ 0 }; m < M; ++m)
						for (int n{ 0 }; n < N; ++n)
							m.data[o][m] += data[n][m] * static_cast<TypeName>(rhs.data[o][n]);

				return m;
			}
			template<typename T>
			Vector::Vec<T, M> operator *(const Vector::Vec<T, N>& v) const
			{
				Vector::Vec<T, M> ret{ };
				for (int m{ 0 }; m < M; ++m)
					for (int n{ 0 }; n < N; ++n)
						ret[m] += data[n][m] * static_cast<TypeName>(v[n]);

				return ret;
			}
			template<typename T>
			Mat operator +(const Mat<T, M, N>& rhs) const { Mat m{ *this }; return m += rhs; }
			template<typename T>
			Mat operator -(const Mat<T, M, N>& rhs) const { Mat m{ *this }; return m -= rhs; }
			template<typename T>
			Mat operator *(T scalar) const { Mat m{ *this }; return m *= scalar; }
			template<typename T>
			Mat operator /(T scalar) const { Mat m{ *this }; return m *= (1 / scalar); }

			template<typename T, typename = std::enable_if_t<M == N>>
			Mat& operator *=(const Mat<T, M, N>& rhs) { return *this = *this * rhs; }
			template<typename T>
			Mat& operator +=(const Mat<T, M, N>& rhs)
			{
				for (int m{ 0 }; m < M; ++m)
					for (int n{ 0 }; n < N; ++n)
						data[m][n] += static_cast<TypeName>(rhs.data[m][n]);

				return *this;
			}
			template<typename T>
			Mat& operator -=(const Mat<T, M, N>& rhs){
				for (int m{ 0 }; m < M; ++m)
					for (int n{ 0 }; n < N; ++n)
						data[m][n] -= static_cast<TypeName>(rhs.data[m][n]);

				return *this;
			}
			template<typename T>
			Mat& operator *=(T scalar)
			{ 
				for (int m{ 0 }; m < M; ++m)
					for (int n{ 0 }; n < N; ++n)
						data[m][n] *= static_cast<TypeName>(scalar);

				return *this;
			}
			template<typename T>
			Mat& operator /=(T scalar) { return (*this) *= (1 / scalar); }
		};

		template<typename TypeName>
		struct Mat22 : Mat<TypeName, 2, 2>
		{
			explicit Mat22(TypeName x1, TypeName y1, TypeName x2, TypeName y2) 
				: Mat<TypeName, 2, 2>(x1, y1, x2, y2) {};
			explicit Mat22(const Vector::Vec2<TypeName>& v1, const Vector::Vec2<TypeName>& v2) 
				: Mat<TypeName, 2, 2>(v1.x, v1.y, v2.x, v2.y) {};
		};

		template<typename TypeName>
		struct Mat23 : Mat<TypeName, 2, 3>
		{
			explicit Mat23(TypeName x1, TypeName y1, TypeName x2, TypeName y2, TypeName x3, TypeName y3) 
				: Mat<TypeName, 2, 3>(x1, y1, x2, y2, x3, y3) {};
			explicit Mat23(const Vector::Vec2<TypeName>& v1, const Vector::Vec2<TypeName>& v2, const Vector::Vec2<TypeName>& v3) 
				: Mat<TypeName, 2, 3>(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y) {};
			explicit Mat23(const Vector::Vec2<TypeName>& v1, const Vector::Vec2<TypeName>& v2, const Vector::Point2<TypeName>& v3)
				: Mat<TypeName, 2, 3>(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y) {};

			template<typename T>
			Vector::Vec2<T> operator *(const Vector::Vec2<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 2, 3>::data };
				return Vector::Vec<T, 2>{ data[0][0] * v.x + data[1][0] * v.y
										, data[0][1] * v.x + data[1][1] * v.y};
			}

			template<typename T>
			Vector::Point2<T> operator *(const Vector::Point2<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 2, 3>::data };
				return Vector::Point2<T>{ data[0][0] * v.x + data[1][0] * v.y + data[2][0]
										, data[0][1] * v.x + data[1][1] * v.y + data[2][1]};
			}

			template<typename T>
			Mat23 operator *(const Mat23<T>& rhs) const
			{
				TypeName* data{ Mat<TypeName, 2, 3>::data };

				Mat23 m{ };
				for (int o{ 0 }; o < 3; ++o)
				{
					m.data[o][0] += data[0][0] * static_cast<TypeName>(rhs.data[o][0])
								+ data[1][0] * static_cast<TypeName>(rhs.data[o][1]);

					m.data[o][1] += data[0][1] * static_cast<TypeName>(rhs.data[o][0])
						+ data[1][1] * static_cast<TypeName>(rhs.data[o][1]);
				}

				m.data[2][0] += data[0][2];
				m.data[2][1] += data[1][2];

				return m;
			}

			template<typename T>
			Mat23& operator *=(const Mat23<T>& rhs) const { return (*this) = (*this) * rhs; }

			template<typename T>
			static inline Mat23 MakeTranslation(const Vector::Point2<T>& position) { return Mat23{ 1.f, 0.f, 0.f, 1.f, static_cast<TypeName>(position.x), static_cast<TypeName>(position.y) }; }
			template<typename T>
			static inline Mat23 MakeTranslation(T x, T y) { return Mat23{ 1.f, 0.f, 0.f, 1.f, static_cast<TypeName>(x), static_cast<TypeName>(y) }; }
			
			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat23<double> MakeRotation(T rotation)
			{
				double cosSin[2]{ cos(rotation), sin(rotation) };
				return Mat23<float>{ cosSin[0], cosSin[1], -cosSin[1], cosSin[0], 0.0, 0.0 };
			}

			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat23<float> MakeRotation(T rotation)
			{
				float cosSin[2]{ cosf(rotation), sinf(rotation) };
				return Mat23<float>{ cosSin[0], cosSin[1], -cosSin[1], cosSin[0], 0.f, 0.f };
			}
			
			template<typename T>
			static inline Mat23 MakeScale(const Vector::Vec2<T>& scale) { return Mat23{ static_cast<TypeName>(scale.x), 0.f, 0.f, static_cast<TypeName>(scale.y), 0.f, 0.f }; }
			template<typename T>
			static inline Mat23 MakeScale(T x, T y) { return Mat23{ static_cast<TypeName>(x), 0.f, 0.f, static_cast<TypeName>(y), 0.f, 0.f }; }
		};

		template<typename TypeName>
		struct Mat33 : Mat<TypeName, 3, 3>
		{
			explicit Mat33(TypeName x1, TypeName y1, TypeName z1, TypeName x2, TypeName y2, TypeName z2, TypeName x3, TypeName y3, TypeName z3) 
				: Mat<TypeName, 3, 3>(x1, y1, z1, x2, y2, z2, x3, y3, z3) {};
			explicit Mat33(const Vector::Vec3<TypeName>& v1, const Vector::Vec3<TypeName>& v2, const Vector::Vec3<TypeName>& v3)
				: Mat<TypeName, 3, 3>(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z) {};
			explicit Mat33(const Vector::Vec2<TypeName>& v1, const Vector::Vec2<TypeName>& v2, const Vector::Vec2<TypeName>& v3)
				: Mat<TypeName, 3, 3>(v1.x, v1.y, static_cast<TypeName>(0), v2.x, v2.y, static_cast<TypeName>(0), v3.x, v3.y, static_cast<TypeName>(0)) {};
			explicit Mat33(const Vector::Vec2<TypeName>& v1, const Vector::Vec2<TypeName>& v2, const Vector::Point2<TypeName>& v3)
				: Mat<TypeName, 3, 3>(v1.x, v1.y, static_cast<TypeName>(0), v2.x, v2.y, static_cast<TypeName>(0), v3.x, v3.y, static_cast<TypeName>(1)) {};

			template<typename T>
			Vector::Vec2<T> operator *(const Vector::Vec2<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 2, 3>::data };
				return Vector::Vec<T, 2>{ data[0][0] * v.x + data[1][0] * v.y
					, data[0][1] * v.x + data[1][1] * v.y};
			}

			template<typename T>
			Vector::Point2<T> operator *(const Vector::Point2<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 2, 3>::data };
				return Vector::Point2<T>{ data[0][0] * v.x + data[1][0] * v.y + data[2][0]
					, data[0][1] * v.x + data[1][1] * v.y + data[2][1]};
			}

			template<typename T>
			static inline Mat33 MakeTranslation(const Vector::Point2<T>& position) { return Mat33{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, static_cast<TypeName>(position.x), static_cast<TypeName>(position.y), 1.f }; }
			template<typename T>
			static inline Mat33 MakeTranslation(T x, T y) { return Mat33{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, static_cast<TypeName>(x), static_cast<TypeName>(y), 1.f }; }

			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat33<double> MakeRotation(T rotation)
			{
				double cosSin[2]{ cos(rotation), sin(rotation) };
				return Mat<double, 3, 3>{ cosSin[0], cosSin[1], 0.0, -cosSin[1], cosSin[0], 0.0, 0.0, 0.0, 1.0};
			}

			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat33<float> MakeRotation(T rotation)
			{
				float cosSin[2]{ cosf(rotation), sinf(rotation) };
				return Mat<float, 3, 3>{ cosSin[0], cosSin[1], 0.f, -cosSin[1], cosSin[0], 0.f, 0.f, 0.f, 1.f };
			}

			template<typename T>
			static inline Mat33 MakeScale(const Vector::Vec2<T>& scale) { return Mat33{ static_cast<TypeName>(scale.x), 0.f, 0.f, 0.f, static_cast<TypeName>(scale.y), 0.f, 0.f, 0.f, 1.f }; }
			template<typename T>
			static inline Mat33 MakeScale(T x, T y) { return Mat33{ static_cast<TypeName>(x), 0.f, 0.f, 0.f, static_cast<TypeName>(y), 0.f, 0.f, 0.f, 1.f }; }
		};

		template<typename TypeName>
		struct Mat34 : Mat<TypeName, 3, 4>
		{
			explicit Mat34(TypeName x1, TypeName y1, TypeName z1, TypeName x2, TypeName y2, TypeName z2, TypeName x3, TypeName y3, TypeName z3, TypeName x4, TypeName y4, TypeName z4)
				: Mat<TypeName, 3, 4>(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4) {};
			explicit Mat34(const Vector::Vec3<TypeName>& v1, const Vector::Vec3<TypeName>& v2, const Vector::Vec3<TypeName>& v3, const Vector::Vec3<TypeName>& v4)
				: Mat<TypeName, 3, 4>(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, v4.x, v4.y, v4.z) {};
			explicit Mat34(const Vector::Vec3<TypeName>& v1, const Vector::Vec3<TypeName>& v2, const Vector::Vec3<TypeName>& v3, const Vector::Point3<TypeName>& v4)
				: Mat<TypeName, 3, 4>(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, v4.x, v4.y, v4.z) {};

			template<typename T>
			Vector::Vec3<T> operator *(const Vector::Vec3<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 3, 4>::data };
				return Vector::Vec3<T>{ data[0][0] * v.x + data[1][0] * v.y + data[2][0] * v.z + data[3][0]
										, data[0][1] * v.x + data[1][1] * v.y + data[2][1] * v.z + data[3][1]
										, data[0][2] * v.x + data[1][2] * v.y + data[2][2] * v.z + data[3][2] };
			}

			template<typename T>
			Vector::Point3<T> operator *(const Vector::Point3<T>& p) const
			{
				TypeName* data{ Mat<TypeName, 3, 4>::data };
				return Vector::Point3<T>{ data[0][0] * p.x + data[1][0] * p.y + data[2][0] * p.z + data[3][0]
										, data[0][1] * p.x + data[1][1] * p.y + data[2][1] * p.z + data[3][1]
										, data[0][2] * p.x + data[1][2] * p.y + data[2][2] * p.z + data[3][2] };
			}

			template<typename T>
			Mat34 operator *(const Mat34<T>& rhs) const
			{
				TypeName* data{ Mat<TypeName, 3, 4>::data };

				Mat34 m{ };
				for (int o{ 0 }; o < 4; ++o)
				{
					m.data[o][0] += data[0][0] * static_cast<TypeName>(rhs.data[o][0])
								+ data[1][0] * static_cast<TypeName>(rhs.data[o][1])
								+ data[2][0] * static_cast<TypeName>(rhs.data[o][2]);

					m.data[o][1] += data[0][1] * static_cast<TypeName>(rhs.data[o][0])
								+ data[1][1] * static_cast<TypeName>(rhs.data[o][1])
								+ data[2][1] * static_cast<TypeName>(rhs.data[o][2]);

					m.data[o][2] += data[0][2] * static_cast<TypeName>(rhs.data[o][0])
								+ data[1][2] * static_cast<TypeName>(rhs.data[o][1])
								+ data[2][2] * static_cast<TypeName>(rhs.data[o][2]);
				}

				m.data[3][0] += data[0][3];
				m.data[3][1] += data[1][3];
				m.data[3][2] += data[2][3];

				return m;
			}

			template<typename T>
			Mat34& operator *=(const Mat34<T>& rhs) const { return (*this) = (*this) * rhs; }

			template<typename T>
			static inline Mat34 MakeTranslation(const Vector::Point3<T>& position) { return Mat34{ 1, 0, 0, 0, 1, 0, 0, 0, 1, static_cast<TypeName>(position.x), static_cast<TypeName>(position.y), static_cast<TypeName>(position.z) }; }
			template<typename T>
			static inline Mat34 MakeTranslation(T x, T y, T z) { return Mat34{ 1, 0, 0, 0, 1, 0, 0, 0, 1, static_cast<TypeName>(x), static_cast<TypeName>(y), static_cast<TypeName>(z) }; }

			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat34<double> MakeRotation(T yaw, T pitch, T roll)
			{
				const double cosSinYaw[2]{ cos(yaw), sin(yaw) };
				const double cosSinPitch[2]{ cos(pitch), sin(pitch) };
				const double cosSinRoll[2]{ cos(roll), sin(roll) };

				const double sPsR{ cosSinPitch[1] * cosSinRoll[1] };
				const double sPcR{ cosSinPitch[1] * cosSinRoll[0] };
				return Mat34<double>{ cosSinYaw[0] * cosSinPitch[0], cosSinYaw[1] * cosSinPitch[0], -cosSinPitch[1]
					, cosSinYaw[0] * sPsR - cosSinYaw[1] * cosSinRoll[0], cosSinYaw[1] * sPsR + cosSinYaw[0] * cosSinRoll[0], cosSinPitch[0] * cosSinRoll[1]
					, cosSinYaw[0] * sPcR + cosSinYaw[1] * cosSinRoll[1], cosSinYaw[1] * sPcR - cosSinYaw[0] * cosSinRoll[1], cosSinPitch[0] * cosSinRoll[0]
					, 0.0, 0.0, 0.0};
			}

			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat34<float> MakeRotation(T yaw, T pitch, T roll)
			{
				const float cosSinYaw[2]{ cosf(yaw), sinf(yaw) };
				const float cosSinPitch[2]{ cosf(pitch), sinf(pitch) };
				const float cosSinRoll[2]{ cosf(roll), sinf(roll) };

				const float sPsR{ cosSinPitch[1] * cosSinRoll[1] };
				const float sPcR{ cosSinPitch[1] * cosSinRoll[0] };
				return Mat34<float>{ cosSinYaw[0] * cosSinPitch[0], cosSinYaw[1] * cosSinPitch[0], -cosSinPitch[1], 0.f
					, cosSinYaw[0] * sPsR - cosSinYaw[1] * cosSinRoll[0], cosSinYaw[1] * sPsR + cosSinYaw[0] * cosSinRoll[0], cosSinPitch[0] * cosSinRoll[1]
					, cosSinYaw[0] * sPcR + cosSinYaw[1] * cosSinRoll[1], cosSinYaw[1] * sPcR - cosSinYaw[0] * cosSinRoll[1], cosSinPitch[0] * cosSinRoll[0]
					, 0.f, 0.f, 0.f};
			}

			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat34<double> MakeRotation(const Vector::Vec3<T>& euler) { return MakeRotation(euler.x, euler.y, euler.z); }
			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat34<float> MakeRotation(const Vector::Vec3<T>& euler) { return MakeRotation(euler.x, euler.y, euler.z); }

			template<typename T>
			static inline Mat34 MakeScale(const Vector::Vec3<T>& scale) { return Mat34{ static_cast<TypeName>(scale.x), 0, 0, 0, static_cast<TypeName>(scale.y), 0, 0, 0, static_cast<TypeName>(scale.z), 0, 0, 0 }; }
			template<typename T>
			static inline Mat34 MakeScale(T x, T y, T z) { return Mat34{ static_cast<TypeName>(x), 0, 0, 0, static_cast<TypeName>(y), 0, 0, 0, static_cast<TypeName>(z), 0, 0, 0 }; }
		};

		template<typename TypeName>
		struct Mat44 : Mat<TypeName, 4, 4>
		{
			explicit Mat44(TypeName x1, TypeName y1, TypeName z1, TypeName w1
						, TypeName x2, TypeName y2, TypeName z2, TypeName w2
						, TypeName x3, TypeName y3, TypeName z3, TypeName w3
						, TypeName x4, TypeName y4, TypeName z4, TypeName w4) 
				: Mat<TypeName, 4, 4>(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4) {};
			explicit Mat44(const Vector::Vec4<TypeName>& v1, const Vector::Vec4<TypeName>& v2, const Vector::Vec4<TypeName>& v3, const Vector::Vec4<TypeName>& v4) 
				: Mat<TypeName, 4, 4>(v1.x, v1.y, v1.z, v1.w, v2.x, v2.y, v2.z, v2.w, v3.x, v3.y, v3.z, v3.w, v4.x, v4.y, v4.z, v4.w) {};
			explicit Mat44(const Vector::Vec3<TypeName>& v1, const Vector::Vec3<TypeName>& v2, const Vector::Vec3<TypeName>& v3, const Vector::Vec3<TypeName>& v4)
				: Mat<TypeName, 4, 4>(v1.x, v1.y, v1.z, static_cast<TypeName>(0), v2.x, v2.y, v2.z, static_cast<TypeName>(0), v3.x, v3.y, v3.z, static_cast<TypeName>(0), v4.x, v4.y, v4.z, static_cast<TypeName>(0)) {};
			explicit Mat44(const Vector::Vec3<TypeName>& v1, const Vector::Vec3<TypeName>& v2, const Vector::Vec3<TypeName>& v3, const Vector::Point3<TypeName>& v4)
				: Mat<TypeName, 4, 4>(v1.x, v1.y, v1.z, static_cast<TypeName>(0), v2.x, v2.y, v2.z, static_cast<TypeName>(0), v3.x, v3.y, v3.z, static_cast<TypeName>(0), v4.x, v4.y, v4.z, static_cast<TypeName>(1)) {};

			template<typename T>
			Vector::Vec3<T> operator *(const Vector::Vec3<T>& v) const
			{
				TypeName* data{ Mat<TypeName, 3, 4>::data };
				return Vector::Vec3<T>{ data[0][0] * v.x + data[1][0] * v.y + data[2][0] * v.z + data[3][0]
					, data[0][1] * v.x + data[1][1] * v.y + data[2][1] * v.z + data[3][1]
					, data[0][2] * v.x + data[1][2] * v.y + data[2][2] * v.z + data[3][2] };
			}

			template<typename T>
			Vector::Point3<T> operator *(const Vector::Point3<T>& p) const
			{
				TypeName* data{ Mat<TypeName, 3, 4>::data };
				return Vector::Point3<T>{ data[0][0] * p.x + data[1][0] * p.y + data[2][0] * p.z + data[3][0]
					, data[0][1] * p.x + data[1][1] * p.y + data[2][1] * p.z + data[3][1]
					, data[0][2] * p.x + data[1][2] * p.y + data[2][2] * p.z + data[3][2] };
			}

			template<typename T>
			static inline Mat44 MakeTranslation(const Vector::Point3<T>& position) { return Mat44{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, static_cast<TypeName>(position.x), static_cast<TypeName>(position.y), static_cast<TypeName>(position.z), 1 }; }
			template<typename T>
			static inline Mat44 MakeTranslation(T x, T y, T z) { return Mat44{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, static_cast<TypeName>(x), static_cast<TypeName>(y), static_cast<TypeName>(z), 1 }; }

			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat44<double> MakeRotation(T yaw, T pitch, T roll)
			{
				const double cosSinYaw[2]{ cos(yaw), sin(yaw) };
				const double cosSinPitch[2]{ cos(pitch), sin(pitch) };
				const double cosSinRoll[2]{ cos(roll), sin(roll) };

				const double sPsR{ cosSinPitch[1] * cosSinRoll[1] };
				const double sPcR{ cosSinPitch[1] * cosSinRoll[0] };
				return Mat44<double>{ cosSinYaw[0] * cosSinPitch[0], cosSinYaw[1] * cosSinPitch[0], -cosSinPitch[1], 0.0
									, cosSinYaw[0] * sPsR - cosSinYaw[1] * cosSinRoll[0], cosSinYaw[1] * sPsR + cosSinYaw[0] * cosSinRoll[0], cosSinPitch[0] * cosSinRoll[1], 0.0
									, cosSinYaw[0] * sPcR + cosSinYaw[1] * cosSinRoll[1], cosSinYaw[1] * sPcR - cosSinYaw[0] * cosSinRoll[1], cosSinPitch[0] * cosSinRoll[0], 0.0
									, 0.0, 0.0, 0.0, 1.0};
			}

			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat44<float> MakeRotation(T yaw, T pitch, T roll)
			{
				const float cosSinYaw[2]{ cosf(yaw), sinf(yaw) };
				const float cosSinPitch[2]{ cosf(pitch), sinf(pitch) };
				const float cosSinRoll[2]{ cosf(roll), sinf(roll) };

				const float sPsR{ cosSinPitch[1] * cosSinRoll[1] };
				const float sPcR{ cosSinPitch[1] * cosSinRoll[0] };
				return Mat44<float>{ cosSinYaw[0] * cosSinPitch[0], cosSinYaw[1] * cosSinPitch[0], -cosSinPitch[1], 0.f
									, cosSinYaw[0] * sPsR - cosSinYaw[1] * cosSinRoll[0], cosSinYaw[1] * sPsR + cosSinYaw[0] * cosSinRoll[0], cosSinPitch[0] * cosSinRoll[1], 0.f
									, cosSinYaw[0] * sPcR + cosSinYaw[1] * cosSinRoll[1], cosSinYaw[1] * sPcR - cosSinYaw[0] * cosSinRoll[1], cosSinPitch[0] * cosSinRoll[0], 0.f
									, 0.f, 0.f, 0.f, 1.f};
			}

			template<typename T, typename = Maths::Enable_64_Type<T>>
			static inline Mat44<double> MakeRotation(const Vector::Vec3<T>& euler) { return MakeRotation(euler.x, euler.y, euler.z); }
			template<typename T, typename = Maths::Enable_32_Type<T>>
			static inline Mat44<float> MakeRotation(const Vector::Vec3<T>& euler) { return MakeRotation(euler.x, euler.y, euler.z); }

			template<typename T>
			static inline Mat44 MakeScale(const Vector::Vec3<T>& scale) { return Mat44{ static_cast<TypeName>(scale.x), 0, 0, 0, 0, static_cast<TypeName>(scale.y), 0, 0, 0, 0, static_cast<TypeName>(scale.z), 0, 0, 0, 0, 1 }; }
			template<typename T>
			static inline Mat44 MakeScale(T x, T y, T z) { return Mat44{ static_cast<TypeName>(x), 0, 0, 0, 0, static_cast<TypeName>(y), 0, 0, 0, 0, static_cast<TypeName>(z), 0, 0, 0, 0, 1 }; }
		};

		template<typename T, int M>
		static T Determinant(const Mat<T, M, M>& mat)
		{
			if constexpr (M == 2)
				return mat.data[0][0] * mat.data[1][1] - mat.data[1][0] * mat.data[0][1];
			else if constexpr (M == 3)
				return mat.data[0][0] * (mat.data[1][1] * mat.data[2][2] - mat.data[2][1] * mat.data[1][2])
				- mat.data[1][0] * (mat.data[0][1] * mat.data[2][2] - mat.data[2][1] * mat.data[0][2])
				+ mat.data[2][0] * (mat.data[0][1] * mat.data[1][2] - mat.data[1][1] * mat.data[0][2]);
			else
			{
				T det = 0;
				Mat<T, M - 1, M - 1> subMat{};

				for (int x = 0; x < M; x++)
				{
					int subi = 0;
					for (int i = 1; i < M; i++)
					{
						int subj = 0;
						for (int j = 0; j < M; j++)
						{
							subMat.data[subj][subi] = mat.data[j][i];
							subj += (j == x) * 1;
						}
						subi++;
					}
					det += T(pow(-1, x)) * mat.data[x][0] * Determinant(subMat);
				}

				return det;
			}
		}

		template<typename T, int M>
		static Mat<T, M, M> Transpose(const Mat<T, M, M>& mat)
		{
			Mat<T, M, M> t{};
			for (int m{ 0 }; m < M; ++m)
				for (int n{ 0 }; n < M; ++n)
					t.data[n][m] = mat.data[m][n];

			return t;
		}

		template<int M, int N>
		using Matf = Mat<float, M, N>;
		template<int M, int N>
		using Matd = Mat<double, M, N>;
		template<int M, int N>
		using Matl = Mat<uint64_t, M, N>;
		template<int M, int N>
		using Mati = Mat<uint32_t, M, N>;
		template<int M, int N>
		using Mati_16 = Mat<uint16_t, M, N>;
		template<int M, int N>
		using Mati_8 = Mat<uint8_t, M, N>;

		using Mat22f = Matf<2, 2>;
		using Mat22d = Matd<2, 2>;
		using Mat22l = Matl<2, 2>;
		using Mat22i = Mati<2, 2>;
		using Mat22i_16 = Mati_16<2, 2>;
		using Mat22i_8 = Mati_8<2, 2>;

		using Mat33f = Matf<3, 3>;
		using Mat33d = Matd<3, 3>;
		using Mat33l = Matl<3, 3>;
		using Mat33i = Mati<3, 3>;
		using Mat33i_16 = Mati_16<3, 3>;
		using Mat33i_8 = Mati_8<3, 3>;

		using Mat44f = Matf<4, 4>;
		using Mat44d = Matd<4, 4>;
		using Mat44l = Matl<4, 4>;
		using Mat44i = Mati<4, 4>;
		using Mat44i_16 = Mati_16<4, 4>;
		using Mat44i_8 = Mati_8<4, 4>;
	}
}

template<typename T, int N, int M>
SDBX::Vector::Vec<T, N>& operator *=(SDBX::Vector::Vec<T, N>& v, const SDBX::Matrix::Mat<T, N, M>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Vec2<T>& operator *=(SDBX::Vector::Vec2<T>& v, const SDBX::Matrix::Mat23<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Point2<T>& operator *=(SDBX::Vector::Point2<T>& v, const SDBX::Matrix::Mat23<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Vec2<T>& operator *=(SDBX::Vector::Vec2<T>& v, const SDBX::Matrix::Mat33<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Point2<T>& operator *=(SDBX::Vector::Point2<T>& v, const SDBX::Matrix::Mat33<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Vec3<T>& operator *=(SDBX::Vector::Vec3<T>& v, const SDBX::Matrix::Mat34<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Point3<T>& operator *=(SDBX::Vector::Point3<T>& v, const SDBX::Matrix::Mat34<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Vec3<T>& operator *=(SDBX::Vector::Vec3<T>& v, const SDBX::Matrix::Mat44<T>& m) { return v = m * v; }
template<typename T>
SDBX::Vector::Point3<T>& operator *=(SDBX::Vector::Point3<T>& v, const SDBX::Matrix::Mat44<T>& m) { return v = m * v; }