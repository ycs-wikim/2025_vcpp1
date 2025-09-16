// 0916.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0916.h"

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
    LoadStringW(hInstance, IDC_MY0916, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0916));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0916));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0916);
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

/// WM_LBUTTONDOWN 좌표를 보관할 전역 변수
int g_x, g_y;
/// 선택 메뉴가 무엇인지 보관하기 위한 전역 변수(플래그 변수)
/// 0 - 선택하지 않았다. 1 - 선 그리기, 2 - 사각형 그리기, 3 - 타원 그리기
/// 4 - 자유선 그리기
int g_flag;

/// 나의 객체 : 0으로 모두 초기화
RECT g_me;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 클래스의 생성자와 같은 역할을 담당하는 윈도우 메시지 입니다.
    /// 프로그램 실행 시 단 한번만 OS에 의해 호출되는 윈도우 메시지.
    /// 별도로 프로그래머가 강제 호출하지 않는 이상 절대 OS가 호출하지 않음.
    /// 프로그램의 초기화 정보, 동적 메모리 할당, 변수 초기화 등을 수행
    case WM_CREATE:
    {
        //MessageBox(hWnd, L"생성자", L"WM_CREATE", MB_OK);
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 250;
        g_me.bottom = 250;
    }
        break;

    case WM_KEYDOWN:
    {
        if (wParam == VK_LEFT)
        {
            g_me.left -= 10;
            g_me.right -= 10;
        }
        else if (wParam == VK_RIGHT)
        {
            g_me.left += 10;
            g_me.right += 10;
        }

        /// WM_PAINT를 호출
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        if (0 == g_flag)
            break;

        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        if (0 == g_flag)
            break;

        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);
        
        /// 선의 속성을 변경 : HPEN 자료형을 사용
        HPEN myPen, osPen;      /// 내가 생성할 펜, OS의 현재 펜
        /// 펜을 생성 - OS가 펜을 생성하여 반환한다. - 동적 메모리 할당
        myPen = CreatePen(PS_DASHDOTDOT, 1, RGB(188, 67, 200));
        /// OS에게 펜을 전달. 즉시 OS의 현재 펜이 반환된다.
        osPen = (HPEN)SelectObject(hdc, myPen);
        /// SelectObject 성공 이후로는 펜이 변경되어 있는 상태

        if (1 == g_flag)
        {
            /// WM_LBUTTONDOWN 위치를 시작 좌표로 설정
            MoveToEx(hdc, g_x, g_y, NULL);
            /// WM_LBUTTONUP 위치를 끝 좌표로 설정
            LineTo(hdc, x, y);
        }
        else if (2 == g_flag)
        {
            /// 면색 변경을 위한 자료형 : HBRUSH
            HBRUSH myBrush, osBrush;

            /// 1. 브러시를 생성한다.
            myBrush = CreateSolidBrush(RGB(100, 200, 100));
            /// 2. OS에게 전달하면서 동시에 현재 브러시를 백업
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            /// 3. 사용한다.

            /// Rectangle 그리기
            Rectangle(hdc, g_x, g_y, x, y);

            /// 4. 원래 브러시로 변경
            SelectObject(hdc, osBrush);
            /// 5. 동적 생성 브러시를 해제
            DeleteObject(myBrush);
        }
        else if (3 == g_flag)
        {
            /// Ellipse 그리기
            Ellipse(hdc, g_x, g_y, x, y);
        }
        /// TODO: 자유선 그리기


        /// 선을 다 사용한 상태
        SelectObject(hdc, osPen);
        /// OS가 동적 생성한 메모리(펜) 정보 해제
        DeleteObject(myPen);

        ReleaseDC(hWnd, hdc);
    }
        break;

    case WM_RBUTTONDOWN:
    {
        RECT a;

        /// 오른쪽 마우스 버튼이 눌리면, 그리기 선택 초기화
        g_flag = 0;
    }
        break;
    /// 메뉴를 클릭했을 때 전달되는 메시지 정보 : wParam
    case WM_COMMAND:
        {
            /// 하위 16비트에서 값을 획득
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_DRAW_LINE:
                g_flag = 1;
                break;
            case ID_DRAW_RECTANGLE:
                g_flag = 2;
                break;
            case ID_DRAW_ELLIPSE:
                g_flag = 3;
                break;
            case ID_DRAW_FREE:
                g_flag = 4;
                break;
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            EndPaint(hWnd, &ps);
        }
        break;

    /// 클래스의 소멸자와 같은 역할을 담당하는 윈도우 메시지입니다.
    case WM_DESTROY:
        //MessageBox(hWnd, L"소멸자", L"WM_DESTORY", MB_OK);
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
