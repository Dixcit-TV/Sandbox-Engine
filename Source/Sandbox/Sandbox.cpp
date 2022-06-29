#include "pch.h"

#if defined(_DEBUG) || defined(DEBUG)
    #include <vld.h>
#endif

#include <iostream>

#include "Core\Maths\Mat.h"
#include "Core\Maths\Vec.h"
#include "Core\Log\Logger.h"
#include "Core\Profiling\Profiler.h"

void Foo()
{
    //SCOPED_TIMER_PROFILLING();

    float res = 0;
    for (int i = 0; i < 100'000'000; ++i)
    {
        BEGIN_TIMER_PROFILLING();
        res += (float)sqrt(i);
        END_TIMER_PROFILLING();
    }
}

int main()
{
   Foo();

   SDBX::Vector::Vec3f vec1{ 16, 23, 52 };
   vec1 = vec1 + vec1;
   vec1 += vec1;

   SDBX::Vector::Point3f point1{ 16, 23, 52 };
   point1 = point1 + point1;
   point1 += point1;

   SDBX::Vector::Vec3f vec2{ point1 };

   SDBX::Vector::Vec2d v2d{ 16, 23 };
   SDBX::Vector::Vec2i v2i{ 16, 23 };

   SDBX::Vector::Angle(v2d, v2i);
   SDBX::Vector::Angle(v2i, v2d);

   SDBX::Vector::Vec3f v3{ 16, 23, 52 };

   double d10[10]{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
   SDBX::Vector::Vec<double, 10> v10{ d10 };
   SDBX::Vector::Vec<double, 10> v10_2{ d10 };

   int i5[5]{ 1, 2,  3, 4, 5};
   SDBX::Vector::Vec<int, 5> v5{ 1, 2, 3, 4, 5 };
   SDBX::Vector::Vec<int, 5> v5_2{ i5 };

   [[maybe_unused]] float l3 = SDBX::Vector::Length(v3);
   [[maybe_unused]] float l2 = SDBX::Vector::Length(v2i);

   SDBX_ASSERT(v10 == v10_2, "");

   v10_2.data[8] = 15.0;

   SDBX_ASSERT(v10 != v10_2, "");

   SDBX_ASSERT(v5 == v5_2, "");

   SDBX::Vector::Vec<int, 5> v5_4{ 1, 2, 3, 4 };

   int i5_4[5]{ 1, 2, 3, 4, 0 };
   SDBX::Vector::Vec<int, 5> v5_4_2{ i5_4 };

   SDBX_ASSERT(v5_4 == v5_4_2, "");

   SDBX::Matrix::Mat22f m{ SDBX::Matrix::Mat22f::Identity() };
    
   SDBX_ASSERT(m == (SDBX::Matrix::Mat22f::Identity()), "");

   SDBX::Matrix::Matf<3, 3> m33{ SDBX::Matrix::Matf<3, 3>::Identity() };
   SDBX::Matrix::Matf<3, 3> t{ SDBX::Matrix::Transpose(m33) };


}