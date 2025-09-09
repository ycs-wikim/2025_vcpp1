// 0909.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0909.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0909, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0909));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0909));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0909);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

/// 전역 변수의 특징 : Data 영역, 0으로 항상 초기화된다. 선언된 이후 어디서나 보인다.
int g_x;        /// 이전 좌표의 x 값
int g_y;        /// 이전 좌표의 y 값
bool g_push = false;    /// 마우스 왼쪽 버튼이 눌려 있는지 여부를 확인하기 위한 변수
                /// 플래그 변수 : 상태 값을 확인할 수 있는 정보를 갖는 변수


/// RECT 자료형 : Win32에서 제공되는 기본 자료형. 사각형 좌표 표현을 위한 자료형
RECT g_a;
/*
typedef struct tagRECT
{
    LONG    left;       /// 좌상단 x 좌표
    LONG    top;        /// 좌상단 y 좌표
    LONG    right;      /// 우하단 x 좌표
    LONG    bottom;     /// 우하단 y 좌표
} RECT, *PRECT, NEAR *NPRECT, FAR *LPRECT;
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case 'w':
            break;
        case 'a':
            break;
        case 's':
            break;
        case 'd':
            break;
        }
        /// 키보드 정보 : wParam
        WCHAR buf[32] = { 0, };
        wsprintfW(buf, L"누른 키는  %d  입니다.", wParam);
        HDC hdc = GetDC(hWnd);
        TextOut(hdc, 10, 50, buf, lstrlenW(buf));
        ReleaseDC(hWnd, hdc);
    }
        break;

    case WM_RBUTTONDOWN:
    {
        /// 프로그래머가 WM_PAINT의 호출을 OS에게 요청 API
        /// 무효화한다. 뭐를 사각형을!
        InvalidateRect(hWnd, NULL, TRUE);
        /// 마지막 인수 : TRUE이면 화면을 모두 지운 후에 WM_PAINT를 호출한다.
        ///               FALSE면 화면을 그대로 둔 채 WM_PAINT를 호출한다.
    }
        break;

    case WM_LBUTTONDOWN:
    {
        /// 왼쪽 마우스 버튼을 누른상태
        g_push = true;
    }
        break;
    case WM_LBUTTONUP:
    {
        /// 왼쪽 마우스 버튼을 뗀 상태
        g_push = false;
    }
        break;
        
    case WM_MOUSEMOVE:
    {
        if (false == g_push)
            break;

        int x, y;
        WCHAR buf[128] = { 0, };
        x = LOWORD(lParam);
        y = HIWORD(lParam);     /// x, y 좌표 값을 획득

        HDC hdc = GetDC(hWnd);      /// HDC 획득 : OS에게 요청해서 획득한 상태

        /// 시작점 : 이전 좌표 값으로 설정
        MoveToEx(hdc, g_x, g_y, NULL);
        /// 끝점 : 현재 좌표 값으로 설정
        LineTo(hdc, x, y);

        wsprintfW(buf, L"x: %d - y: %d", x, y);
        TextOut(hdc, 500, 10, buf, lstrlenW(buf));

        g_a.left = g_x;
        g_a.top = g_y;
        g_a.right = x;
        g_a.bottom = y;
        
        /// 선을 모두 그린 상태
        /// 현재 상태를 전역 변수에 보관 --> 이전 정보
        g_x = x;
        g_y = y;

        Rectangle(hdc, g_a.left, g_a.top, g_a.right, g_a.bottom);
        Ellipse(hdc, g_a.left, g_a.top, g_a.right, g_a.bottom); /// 타원 그리기

        ReleaseDC(hWnd, hdc);       /// HDC 해제
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
            /// 화면에 Hello World 글자를 표기하기!
            /// 현재 윈도우 시스템은 대부분 Uni-Code를 지원한다.
            /// Ex) MessageBox(hWnd, L"내용", L"타이틀", MB_OK);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            /// wchar_t  ==> WCHAR로 정의해서 사용
            WCHAR buf[16] = L"Hello World!";
            /// Hello World! ==> HNeNlNlNoN N...
            /// 글자 화면 출력 API를 사용
            TextOut(hdc, 10, 10, buf, lstrlenW(buf));
            EndPaint(hWnd, &ps);
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

// 정보 대화 상자의 메시지 처리기입니다.
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
