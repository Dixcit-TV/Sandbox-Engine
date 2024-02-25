#include "pch.h"

#if defined(_DEBUG) || defined(DEBUG)
    #include <vld.h>
#endif

#include <iostream>

#include "Platform\Target\Windows\Window.h"
#include "Core\Maths\Mat.h"
#include "Core\Maths\Vec.h"
#include "Core\Log\Logger.h"
#include "Core\Profiling\Profiler.h"
#include "Renderer/API/DX11/DX11Render.h"

LRESULT _stdcall WndProc_Implementation(HWND, UINT msg, WPARAM wParam, LPARAM);

void Foo()
{
    //SCOPED_TIMER_PROFILLING();

    float res = 0;
    for (int i = 0; i < 100; ++i)
    {
        //BEGIN_TIMER_PROFILLING();
        res += (float)sqrt(i);
        //END_TIMER_PROFILLING();
    }
}

int wmain(int, wchar_t* [])
{
    wchar_t windowName[]{ TEXT("Sandbox") };
    SDBX::Window wnd{ windowName, 1280u, 720u };
    wnd.Init(&WndProc_Implementation);

    const int vpCount{ 1 };
    SDBX::Viewport viewports[vpCount]{ SDBX::Viewport{ 1280.f, 720.f, 0.f, 0.f } };

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

    int i5[5]{ 1, 2,  3, 4, 5 };
    SDBX::Vector::Vec<int, 5> v5{ 1, 2, 3, 4, 5 };
    SDBX::Vector::Vec<int, 5> v5_2{ i5 };

    [[maybe_unused]] float l3 = SDBX::Vector::Length(v3);
    [[maybe_unused]] float l2 = SDBX::Vector::Length(v2i);

    SDBX_W_ASSERT(v10 == v10_2);

    v10_2.data[8] = 15.0;

    SDBX_W_ASSERT(v10 != v10_2);

    SDBX_W_ASSERT(v5 == v5_2);

    SDBX::Vector::Vec<int, 5> v5_4{ 1, 2, 3, 4 };

    int i5_4[5]{ 1, 2, 3, 4, 0 };
    SDBX::Vector::Vec<int, 5> v5_4_2{ i5_4 };

    SDBX_W_ASSERT(v5_4 == v5_4_2);

    SDBX::Matrix::Mat22f m{ SDBX::Matrix::Mat22f::Identity() };

    SDBX_W_ASSERT(m == (SDBX::Matrix::Mat22f::Identity()));

    SDBX::Matrix::Mat33f m33{ SDBX::Matrix::Mat33f::Identity() };
    SDBX::Matrix::Mat33f t{ SDBX::Matrix::Transpose(m33) };

    SDBX::Vector::Vec<float, 3> vForMat{ 15.f, 984.f, 2.f};
    //SDBX::Matrix::Matf<3, 5> Mat35_0{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
    SDBX::Vector::Vec<float, 3> vArray[5]{ vForMat, vForMat, vForMat, vForMat, vForMat };
    SDBX::Matrix::Matf<3, 5> Mat35{ vArray };
    //SDBX::Matrix::Matf<3, 5> Mat35_2{ vForMat, vForMat, vForMat, vForMat, vForMat };

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    SDBX::DX11::DX11Render renderer;
    renderer.Init(wnd, viewports, vpCount);

    while (msg.message != WM_QUIT)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                break;
        }

        renderer.Present();
    }
}

LRESULT _stdcall WndProc_Implementation(HWND, UINT msg, WPARAM wParam, LPARAM)
{
    switch (msg)
    {
    case WM_KEYUP:
    {
        if (wParam == VK_F1)
        {
            return 0;
        }
    }
    break;
    default: break;
    }

    return -1;
}