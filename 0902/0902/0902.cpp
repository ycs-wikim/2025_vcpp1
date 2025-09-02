// 0902.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0902.h"

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
    LoadStringW(hInstance, IDC_MY0902, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0902));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0902));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0902);
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

/// 모든 CALLBACK 함수는 동일하게 4개의 인수를 받는다. ==> 고정되어 있다!
///  모든 프로그램이 동일한 함수 인수로 구성되므로 OS가 편하게 정보 전달 가능
/// HWND : Handle of Windows : 어떤 윈도우가 메시지를 받았는가
/// UINT : message(사용자의 입력) : 키보드, 마우스를 입력
/// WPARAM : 키보드에 관련된 정보
/// LPARAM : 마우스에 관련된 정보

int g_x = 10;
int g_y = 10;

/// 콜백 함수도 함수!!!!!!!!
/// OS에 의한 호출이 완료되면 내부 정보는 모두 삭제되고 유지되지 않음.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /// OS가 보낸 메시지가 사용자의 어떤 입력인지 확인하기 위한 switch
    switch (message)
    {
    case WM_KEYDOWN:
        MessageBox(hWnd, L"키보드가 눌렸음", L"야호", MB_OK);
        break;
    /// 마우스 왼쪽 버튼을 누른 순간! 동작하는 코드
    case WM_LBUTTONDOWN:
    /// 마우스 왼쪽 버튼을 뗀 순간! 동작하는 코드
    //case WM_LBUTTONUP:
    /// 마우스가 이동할때 동작하는 코드
    //case WM_MOUSEMOVE:
    {
        //MessageBox(hWnd, L"마우스 왼쪽 버튼이 눌렸음", L"야호", MB_OK);
        /// WM_PAINT 이외의 영역에서 hdc를 확보하는 API
        HDC hdc = GetDC(hWnd);

        /// 마우스 좌표값 : lParam에 x, y 좌표 값이 모두 포함되어 전달
        int x = LOWORD(lParam);     // LOW WORD
        int y = HIWORD(lParam);     // HI WORD

        MoveToEx(hdc, g_x, g_y, NULL);
        LineTo(hdc, x, y);
        g_x += 10;
        g_y += 10;

        /// WM_PAINT 이외의 영역에서 hdc를 해제하는 API
        ReleaseDC(hWnd, hdc);
    }
        break;
    /// 메뉴를 눌렀을 때 전달되는 사용자 메시지
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
    /// 화면을 그린다. ==> OS가 화면을 하나하나 조절하면서 그리고 있다.
    /// 어떠한 경우에도 화면에 정보가 출력되어야 하는 내용을 그리는 윈도우 메시지
    /// 다른 위치에서 그린 정보들은 지속적으로 유지되지 않는다.
    /// 정보가 지워지는 경우
    ///     1. 창의 크기를 조절하는 경우
    ///     2. 창을 최소화하는 경우
    ///     3. 창을 최대화하는 경우
    ///     4. 창을 원래 크기로 되돌리는 경우
    ///     5. 전체 화면 밖으로 이동하는 경우
    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            /// Set로 구성되는 API들이 존재!
            /// 반드시 하나를 사용하면 종료할때, 다른 하나를 사용하는 형식

            /// BeginPaint() API : 화면에 그리기를 시작하겠다.
            ///     --> WM_PAINT 에서만 정상 동작 합니다.
            HDC hdc = BeginPaint(hWnd, &ps);
            /// 내 화면에 지금부터 그리기 시작할래~ ==> OS한테 전달
            /// API : OS와 정보를 교환하는 함수 -> Request
            /// HDC를 반환 ==> 그려도 문제 없어~ 마구마구 그려버리렴~

            /// HDC : H(Handle) DC(Device -> 모니터(화면) Context(문맥, 정보구성체))
            ///     내 프로그램의 화면에 무언가 그릴때 필요한 정보체
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /// 2. 사각형 그리기
            ///     - Rectangle API
            Rectangle(hdc, 10, 10, 150, 150);

            /// 1. 선그리기
            ///     - 시작점 설정 API : MoveToEx( )
            ///     - 끝점 설정 API : LineTo( )
            MoveToEx(hdc, 10, 10, NULL);
            LineTo(hdc, 170, 250);


            EndPaint(hWnd, &ps);
            /// EndPaint( ) API : 화면에 그리기를 종료하겠다.
            ///     --> WM_PAINT에서만 정상 동작한다.
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    /// 메시지 처리를 할 수 없는 경우의 처리
    default:
        /// DefWindowProc( ) 함수를 호출 (호출 인수 == WndProc)
        /// OS가 가지고 있는 콜백 함수
        /// 모든 콜백에 대한 정보는 OS가 관리하고 있다.
        /// OS가 App.에게 처리를 요구 ==> App.에서 처리하지 않을때.
        /// 단순히 App.에서 처리하지 않고, OS에게도 알리지 않으면... ==> 문제 발생 가능!
        /// 반드시 처리하지 않은 정보도 OS에 알려야 한다!
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
