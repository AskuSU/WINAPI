#include"MainWindow.h"

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Пересчет макета чертежа при изменении размера окна.
void MainWindow::CalculateLayout()
{
    if (pRenderTarget != NULL)
    {
        D2D1_SIZE_F size = pRenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = min(x, y);
        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

        // Вычислим метки часов.

        D2D_POINT_2F pt1 = D2D1::Point2F(
            ellipse.point.x,
            ellipse.point.y - (ellipse.radiusY * 0.9f)
        );

        D2D_POINT_2F pt2 = D2D1::Point2F(
            ellipse.point.x,
            ellipse.point.y - ellipse.radiusY
        );
        
        for (DWORD i = 0; i < 12; i++)
        {
            D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation(
                (360.0f / 12) * i, ellipse.point);

            Ticks[i * 2] = mat.TransformPoint(pt1);
            Ticks[i * 2 + 1] = mat.TransformPoint(pt2);
        }
    }
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            D2D1_COLOR_F color = D2D1::ColorF(D2D1::ColorF::Pink, 1.0f);//D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
            if (SUCCEEDED(hr))
            {
                color = D2D1::ColorF(D2D1::ColorF::Black);
                hr = pRenderTarget->CreateSolidColorBrush(color, &pStroke);
                if (SUCCEEDED(hr))
                {
                    CalculateLayout();
                }
            }                
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hWnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Aqua));
        pRenderTarget->FillEllipse(ellipse, pBrush);

        RenderScene();

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hWnd, &ps);
    }
}

void MainWindow::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth)
{
    pRenderTarget->SetTransform(
        D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point)
    );

    // endPoint определяет один конец Стрелки.
    D2D_POINT_2F endPoint = D2D1::Point2F(
        ellipse.point.x,
        ellipse.point.y - (ellipse.radiusY * fHandLength)
    );

    // Рисуем линию от центра эллипса до endPoint.
    pRenderTarget->DrawLine(
        ellipse.point, endPoint, pStroke, fStrokeWidth);
}

void MainWindow::RenderScene()
{
    //pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

    //pRenderTarget->FillEllipse(ellipse, pFill);
    pRenderTarget->DrawEllipse(ellipse, pStroke);

    // Рисуем отметки часов
    for (DWORD i = 0; i < 12; i++)
    {
        pRenderTarget->DrawLine(Ticks[i * 2], Ticks[i * 2 + 1], pStroke, 2.0f);
    }

    // Рисуем стрелки
    SYSTEMTIME time;
    GetLocalTime(&time);

    // 60 minutes = 30 degrees, 1 minute = 0.5 degree
    const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f) + (time.wSecond * 0.00833f);
    const float fMinuteAngle = (360.0f / 60) * (time.wMinute) + (time.wSecond * 0.1f);
    const float fSecondAngle = (360.0f / 60) * (time.wSecond) + (360.0f / 60000) * (time.wMilliseconds);

    DrawClockHand(0.6f, fHourAngle, 6);
    DrawClockHand(0.85f, fMinuteAngle, 4);
    DrawClockHand(0.85f, fSecondAngle, 2);

    // Восстанавливаем преобразование идентичности.
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)) || !InitializeTimer())
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        OnPaint();
        /*PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(m_hWnd, &ps);*/
    }
    return 0;

    case WM_SIZE:
        Resize();
        return 0;

    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

#pragma warning(disable : 4996)
void MainWindow::InitializeDPIScale(ID2D1Factory* pFactory)
{
    FLOAT dpiX, dpiY;

    pFactory->GetDesktopDpi(&dpiX, &dpiY);

    g_DPIScaleX = dpiX / 96.0f;
    g_DPIScaleY = dpiY / 96.0f;
}


template <typename T>
float MainWindow::PixelsToDipsX(T x)
{
    return static_cast<float>(x) / g_DPIScaleX;
}

template <typename T>
float MainWindow::PixelsToDipsY(T y)
{
    return static_cast<float>(y) / g_DPIScaleY;
}

BOOL MainWindow::InitializeTimer()
{
    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (hTimer == NULL)
    {
        return FALSE;
    }

    LARGE_INTEGER li = { 0 };

    if (!SetWaitableTimer(hTimer, &li, (1000 / 60), NULL, NULL, FALSE))
    {
        CloseHandle(hTimer);
        hTimer = NULL;
        return FALSE;
    }

    return TRUE;
}

void MainWindow::WaitTimer()
{
    // Подождите, пока не истечет время таймера или пока не будет опубликовано какое-либо сообщение.
    if (MsgWaitForMultipleObjects(1, &hTimer, FALSE, INFINITE, QS_ALLINPUT)
        == WAIT_OBJECT_0)
    {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}