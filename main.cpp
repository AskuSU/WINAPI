
#include <shobjidl.h>
#include <atlbase.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "framework.h"
#include "main.h"


#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
int nCmdShow_G;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    OpenFileDialog();
INT_PTR CALLBACK    OpenTestWindow(int, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HWL8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    nCmdShow_G = nCmdShow;

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HWL8));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HWL8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HWL8);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       GetSystemMetrics(SM_CXSCREEN) / 2 - 260, // centruoja forma
       GetSystemMetrics(SM_CYSCREEN) / 2 - 220, // centruoja forma
       520, /* width */
       440, /* height */
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        CreateWindow(TEXT("button"), TEXT("Button"),
            WS_VISIBLE | WS_CHILD,
            100, 100, //позиция кнопки по x y
            90, 35, //размеры кнопки по x y
            hWnd, (HMENU)IDB_button1, NULL, NULL
        );
        CreateWindow(TEXT("button"), TEXT("Knopke"),
            WS_VISIBLE | WS_CHILD,
            10, 10, 90, 35,
            hWnd, (HMENU)IDB_button2, NULL, NULL
        );

        CreateWindow(TEXT("edit"), TEXT(" REDAGE"),
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
            10, 70, 90, 22, hWnd, (HMENU)2, NULL, NULL
        );

        CreateWindow(TEXT("STATIC"), TEXT("Statika"), // Label tipo 
            WS_VISIBLE | WS_CHILD,
            10, 170, 90, 22,
            hWnd, (HMENU)3, NULL, NULL
        );
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_TESTWINDOW:
                OpenTestWindow(nCmdShow_G, 480, 400);
                break;
            case IDM_OPENFILE:
                OpenFileDialog();
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, NULL, NULL);
                //PostMessage(hWnd, WM_CLOSE, NULL, NULL);
                //DestroyWindow(hWnd);
                break;
            case IDM_MYMESSAGE:
                MessageBox(NULL,
                    _T("Вот оно я!"),
                    _T("Заголовок сообщения"),
                    NULL);
                break;
            case IDB_button1:            
                MessageBox(0, _T("Sveiki!"), _T("Informacija"), MB_OK | MB_ICONINFORMATION);
                break;   
            case IDB_button2:
                MessageBox(0, _T("Кнопик 2!"), _T("Информация"), MB_OK | MB_ICONINFORMATION);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            SetBkMode(hdc, TRANSPARENT); //прозрачность
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW-1));
            TextOut(hdc,
                150, 70,
                _T("Hello, Windows desktop1!"), _tcslen(_T("Hello, Windows desktop1!")));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
        if (MessageBox(hWnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK OpenFileDialog()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        //IFileOpenDialog* pFileOpen;
        CComPtr<IFileOpenDialog> pFileOpen;

        // Create the FileOpenDialog object.
        /*hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));*/
        /*hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL,
            __uuidof(pFileOpen), reinterpret_cast<void**>(&pFileOpen));*/
        /*hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL,
            IID_PPV_ARGS(&pFileOpen));*/
        hr = pFileOpen.CoCreateInstance(__uuidof(FileOpenDialog));
        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    //pItem->Release();
                    //SafeRelease(&pItem);
                }
            }
            //pFileOpen->Release();
            //SafeRelease(&pFileOpen);
        }
        CoUninitialize();
    }
    return 0;
}

INT_PTR CALLBACK OpenTestWindow(
    int nCmdShow,
    int nWidth = CW_USEDEFAULT,
    int nHeight = CW_USEDEFAULT)
{
    MainWindow testWin;

    if (!testWin.Create(
        L"Мое тестовое окно",
        WS_OVERLAPPEDWINDOW,
        0,
        GetSystemMetrics(SM_CXSCREEN) / 2 - nWidth / 2, // centruoja forma
        GetSystemMetrics(SM_CYSCREEN) / 2 - nHeight / 2, // centruoja forma
        nWidth, /* width */
        nHeight /* height */
        ))
    {
        return 0;
    }

    ShowWindow(testWin.Window(), nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}