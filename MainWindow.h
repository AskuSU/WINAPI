#pragma once
#include"basewin.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include<WindowsX.h>
#include"resource.h"


template <class T> void SafeRelease(T**);

class MainWindow : public BaseWindow<MainWindow>
{
    ID2D1Factory*           pFactory;       //Где будем рисовать, цель рендеринга
    ID2D1HwndRenderTarget*  pRenderTarget;  //Кисть, чем рисовать
    ID2D1SolidColorBrush*   pBrush, *pStroke;
    D2D1_ELLIPSE            ellipse;
    D2D_POINT_2F            Ticks[24];
    HANDLE                  hTimer;
    D2D_POINT_2F            ptMouse;

    float g_DPIScaleX = 1.0f;
    float g_DPIScaleY = 1.0f;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();
    void    DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
    void    RenderScene();

    void    OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void    OnLButtonUp();
    void    OnMouseMove(int pixelX, int pixelY, DWORD flags);

public:   

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
        ellipse(D2D1::Ellipse(D2D1::Point2F(), 0, 0)),
        ptMouse(D2D1::Point2F())
    {
    }

    PCWSTR  ClassName() const { return L"Sample Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void InitializeDPIScale(ID2D1Factory* pFactory);
    
    template <typename T>
    float PixelsToDipsX(T x);
    template <typename T>
    float PixelsToDipsY(T y);

    BOOL InitializeTimer();
    void WaitTimer();
};