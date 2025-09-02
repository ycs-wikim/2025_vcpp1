// 0826.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0826.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다. => this
/// H로 시작하는 자료형 : H --> HANDLE을 의미 -> 객체 전체를 제어할 수 있는 시작 위치!
///                     : 실제 자료형이 void* -> 주소를 가리킨다.
///                     : 변수 -> 운영체제에 존재하는 값을 가리키는 용도

WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

/// win32에서 대부분의 내장 자료형은 모두 대문자로 선언되어 있다.


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
/// 함수 원형
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    /// *****
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,      /// argc, argv
                     _In_ int       nCmdShow)       /// 화면 표시 여부
{
    /// main에서 별도로 정보를 획득하지 않은 경우의 인수를 제거하는 매크로
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0826, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0826));

    MSG msg;
/*
    typedef struct tagMSG {
        HWND        hwnd;           /// 어떤 윈도우가 받아야 하는 메시지인지. 주체를 나타냄
        UINT        message;        /// 사용자가 입력한 액션(마우스 클릭, 이동 ...)
        WPARAM      wParam;         /// 사용자 액션을 처리하기 위한 부가 정보 - 1
        LPARAM      lParam;         /// 사용자 액션을 처리하기 위한 부가 정보 - 2
        DWORD       time;           /// 액션이 발생한 시간
        POINT       pt;             /// 위치 정보
    } MSG,
*/



    // 기본 메시지 루프입니다:    ==> 무한 대기 상태
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
    /// window class extension wide
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    /// 화면에 메인 윈도우가 표시되고 동작하기 위한 기반 정보 설정
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0826));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0826);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    /// 실제로 OS에 현재 윈도우의 정보를 등록하는 함수
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

   /// 우리가 실행했을때 화면에 보이는 하얀 윈도우 생성
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   /// 윈도우 생성 확인
   if (!hWnd)
   {
      return FALSE;
   }

   /// 윈도우를 화면에 출력해줘~ API
   ShowWindow(hWnd, nCmdShow);
   /// 화면에 내용이 변경되었으면 적용해줘~ API
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 메뉴를 눌렀을때.
    case WM_COMMAND:
        {
            /// modal
            MessageBox(hWnd, L"메뉴가 눌려버렸네.", L"아이쿠", MB_OK);
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
    case WM_KEYDOWN:
        MessageBox(hWnd, L"키보드가 눌려버렸네.", L"하하하하하하하", MB_OK);
        break;
    /// 화면을 그릴때 사용한다.
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    /// 내 윈도우가 종료될때 호출되는 메시지
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
