// WindowsAppTests.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsAppTests.h"

#include <iostream>
#include<string>
using namespace std;

#include <Unknwn.h>    

#include "gdiplus.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

BLENDFUNCTION bStruct = { 0 };

//====================================Buttons======================================================================
HWND hWndB;



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSAPPTESTS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPPTESTS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPPTESTS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)0;
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_WINDOWSAPPTESTS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   /*HWND hWndOwner = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/
   
   HWND hWnd = CreateWindowEx(
       WS_EX_LAYERED,                              // Optional window styles.
       szWindowClass,                     // Window class
       szTitle,    // Window text
       /*WS_POPUP,*/WS_EX_LAYERED,//WS_OVERLAPPEDWINDOW,            // Window style

       // Size and position
       0, 0, 1920, 1080,

       NULL,       // Parent window    
       NULL,       // Menu
       hInstance,  // Instance handle
       NULL        // Additional application data
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);
   

   bStruct.BlendOp = AC_SRC_OVER;
   bStruct.BlendFlags = 0;
   bStruct.SourceConstantAlpha = 255; // any 0 to 255
   bStruct.AlphaFormat = AC_SRC_ALPHA;

   tagSIZE wndSize;
   wndSize.cx = 1920;
   wndSize.cy = 1080;
   tagPOINT Zpos;
   Zpos.x = 0;
   Zpos.y = 0;

   HDC hdcMem = CreateCompatibleDC(NULL);


   //The madness to create a transparent background:
   HBITMAP hbmpSplash = NULL;
   Bitmap* bmpBase = Bitmap::FromFile(L"C:/Users/thefi/Desktop/blenderRender/photos/blank.png");
   bmpBase->GetHBITMAP(Color(0,0,0),&hbmpSplash);
   (HBITMAP)SelectObject(hdcMem, hbmpSplash);
   

   //GDI+ stuff here:
   Graphics m_g(hdcMem);
   
   //Render images to screen

   Bitmap* bmpT3 = Bitmap::FromFile(L"C:/Users/thefi/Desktop/blenderRender/photos/elevator.png");
   m_g.DrawImage(bmpT3, 300, 300, bmpT3->GetWidth() / 2, bmpT3->GetHeight() / 2);

   Bitmap* bmpSplash = Bitmap::FromFile(L"C:/Users/thefi/Desktop/blenderRender/photos/img.png");
   m_g.DrawImage(bmpSplash, (INT)wndSize.cx / 2 - bmpSplash->GetWidth() / 2, 100, bmpSplash->GetWidth() / 2, bmpSplash->GetHeight() / 2);

   //Button graphics:
   Bitmap* bmpT2 = Bitmap::FromFile(L"C:/Users/thefi/Documents/UI/QuitB.png");
   m_g.DrawImage(bmpT2, 10, 50+30, 100, 100-30);
   


   //update window
   UpdateLayeredWindow(
       hWnd,
       hdcMem,
       &Zpos,
       &wndSize,
       hdcMem,
       &Zpos,
       RGB(0, 0, 0),
       &bStruct,
       ULW_ALPHA
   );


   /*=====================================TODO:Add buttons!===========================================================*/
   hWndB = CreateWindowEx(
       NULL,                              // Optional window styles.
       szWindowClass,                     // Window class
       NULL,    // Window text
       WS_VISIBLE|WS_CHILD,            // Window style

       // Size and position
       10, 50, 100, 100,

       hWnd,       // Parent window    
       NULL,       // Menu
       hInstance,  // Instance handle
       NULL        // Additional application data
   );






   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
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
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONUP:
    {
        if (hWnd == hWndB) {
            PostQuitMessage(0);
        }
        
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

// Message handler for about box.
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
