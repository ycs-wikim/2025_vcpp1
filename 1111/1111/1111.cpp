// 1111.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1111.h"

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
    HANDLE mux = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"없습니다");

    if (NULL == mux)
    {
        //MessageBox(NULL, L"없습니다가 없습니다.", L"어이쿠", MB_OK);
    }
    else
    {
        //MessageBox(NULL, L"없습니다가 있군요", L"얏호", MB_OK);
        return 0;
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1111, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1111));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1111));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1111);
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

typedef struct data_set
{
    HWND            m_hWnd;
    LPARAM          m_lParam;
} DS, *PDS;

/// 공유 자원
int g_x;

/// OS가 해결 1. Critical Section
CRITICAL_SECTION g_cs;  /// 자료형 : OS에게 전달. 이 프로그램 내부에서 동기화 문제 해결이 있을 예정

/// OS가 해결 2. Semaphore
HANDLE g_sem;   /// 자료형 선언
/// y 좌표값 변경을 위한 변수
int g_pos = 15;

/// 스레드 함수 == main 함수
DWORD WINAPI pig_sem(LPVOID param)
{
    PDS pds = (PDS)param;
    int pos = g_pos;
    g_pos += 15;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    WCHAR buf[128] = { 0, };
    WCHAR sbuf[16] = { 0, };
    HDC hdc = GetDC(pds->m_hWnd);

    WaitForSingleObject(g_sem, INFINITE);        /// OS의 제어가 시작
    /// Semaphore의 기본 자원의 개수 : 3개
    for (int i = 0; i < 100; i++)
    {
        memset(buf, 0x00, 128);         /// buf의 초기화
        for (int j = 0; j < i; j++)
        {
            lstrcat(buf, L"|");     /// buf : ||||||||||||...
        }
        wsprintf(sbuf, L" %d%%", i + 1);    /// sbuf : 20%
        lstrcat(buf, sbuf);         /// |||||||||| 20%
        TextOut(hdc, 10, pos, buf, lstrlen(buf));
        Sleep(100);
    }
    ReleaseSemaphore(g_sem, 1, NULL);               /// OS의 제어 종료

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}


/// 스레드 함수 == main 함수
DWORD WINAPI pig_cs(LPVOID param)
{
    PDS pds = (PDS)param;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    HDC hdc = GetDC(pds->m_hWnd);

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);        /// OS의 제어가 시작
        ////////////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////////////// 임계 영역 종료
        LeaveCriticalSection(&g_cs);        /// OS의 제어 종료
        Sleep(100);
    }

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}

/// 자료형 선언
HANDLE g_mux;

/// 스레드 함수 == main 함수
DWORD WINAPI pig_mux(LPVOID param)
{
    PDS pds = (PDS)param;
    int x = LOWORD(pds->m_lParam);
    int y = HIWORD(pds->m_lParam);
    HDC hdc = GetDC(pds->m_hWnd);

    for (int i = 0; i < y; i++)
    {
        WaitForSingleObject(g_mux, INFINITE);        /// OS의 제어가 시작
        ////////////////////////////////////////// 임계 영역 시작
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ////////////////////////////////////////// 임계 영역 종료
        ReleaseMutex(g_mux);        /// OS의 제어 종료
        Sleep(100);
    }

    ReleaseDC(pds->m_hWnd, hdc);

    ExitThread(0);
    return 0;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 동기화 객체 생성과 초기화
        /// Critical Section
        InitializeCriticalSection(&g_cs);
        /// Semaphore
        /// 1. 보안 속성 : NULL --> 프로세스와 동일한 권한
        /// 2. 초기 개수 : 최초부터 사용할 수 있는 자원의 개수
        /// 3. 최대 개수 : 이 자원의 최대 개수
        /// 4. 이름 : 프로세스 내부(지역 네임 스페이스)에서
        ///           이름 검색을 지원하기 위한 ID 같은 것
        g_sem = CreateSemaphore(NULL, 3, 3, NULL);
        /// Mutex
        /// 1. 보안 속성
        /// 2. 초기 소유자 설정 : TRUE(소유자 있음), FALSE(없음)
        /// 3. 이름 : 글로벌 네임 스페이스(OS)에서 이름 검색 지원 ID 같은 것
        /// API가 실행되면, 이름이 즉시 OS에 등록
        g_mux = CreateMutex(NULL, FALSE, L"없습니다");
    }
        break;

    case WM_LBUTTONDOWN:
    {
        /// 스레드에 전달할 정보 생성과 설정
        DS ds = { 0, };
        ds.m_hWnd = hWnd;
        ds.m_lParam = lParam;
        /// 스레드 생성
        /// Critical Section 스레드 생성
        //CreateThread(NULL, 0, pig_cs, (LPVOID)&ds, 0, NULL);
        /// Semaphore 스레드 생성
        //CreateThread(NULL, 0, pig_sem, (LPVOID)&ds, 0, NULL);
        /// Mutex 스레드 생성
        CreateThread(NULL, 0, pig_mux, (LPVOID)&ds, 0, NULL);
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        /// 동기화 객체 해제
        /// Critical Section
        DeleteCriticalSection(&g_cs);
        /// Semaphore 해제
        CloseHandle(g_sem);
        /// Mutex 해제
        CloseHandle(g_mux);
        PostQuitMessage(0);
    }
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
