// 0930.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0930.h"
#include <stdlib.h>
#include <time.h>

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
    LoadStringW(hInstance, IDC_MY0930, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0930));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0930));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0930);
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

#define GAME_TIME 60

/// 나와 상대 + 음식 객체
RECT g_me, g_you, g_food, g_time;
/// 게임 시간 설정용 변수 : 60 - 게임 시간
int g_timer = GAME_TIME;
/// 음식을 몇개나 먹었는지 확인하기 위한 변수
int g_food_eat;
/// 현재 게임 중인지 여부를 확인할 변수 : FALSE(게임중), TRUE(게임오버)
BOOL g_gameover;    /// g_gameover의 초기값은 0(FALSE)
/// 타이머 설정을 위한 변수
int g_interval = 1000;      /// 타이머는 mesc 단위이므로, 초기가 1초
/// 치트 키 설정을 위한 변수
BOOL g_cheat;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 나의 객체 초기 좌표
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 150;
        g_me.bottom = 150;
        /*
        * /// 잘못된 left, top, right, bottom 값의 입력
        g_me.left = 490;
        g_me.top = 350;
        g_me.right = 350;
        g_me.bottom = 490;
        */

        /// 상대 좌표 초기 설정
        g_you.left = 500;
        g_you.top = 500;
        g_you.right = 640;
        g_you.bottom = 640;

        /// 남은 시간 출력 사각형의 초기 좌표
        g_time.left = 10;
        g_time.top = 10;
        g_time.right = 600;
        g_time.bottom = 30;

        /// 랜덤 Seed 값 변경
        srand(time(NULL));
        /// 음식의 초기 좌표 설정
        g_food.left = rand() % 700;
        g_food.top = rand() % 700;
        g_food.right = g_food.left + 50;
        g_food.bottom = g_food.top + 50;

        /// 상대 이동을 위한 주기적인 윈도우 메시지 호출 : WM_TIMER
        /// 타이머는 한번 설정되면, 그 이후로 변경이 불가능!
        SetTimer(hWnd, 1, g_interval, NULL);
        /// 시간 타이머 설정을 위한 주기적인 윈도우 메시지 호출
        SetTimer(hWnd, 2, 1000, NULL);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        KillTimer(hWnd, 1);
    }
        break;
    case WM_TIMER:
    {
        if (1 == wParam)
        {
            /// x 좌표 비교
            if (g_me.left < g_you.left)
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                g_you.left += 10;
                g_you.right += 10;
            }

            /// y 좌표
            if (g_me.top < g_you.top)
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }

            /// 겹침 처리 테스트
            RECT ret;

            if (IntersectRect(&ret, &g_me, &g_you))
            {
                if (FALSE == g_cheat)
                {
                    g_gameover = TRUE;      /// 게임 종료가 발생함!
                    KillTimer(hWnd, 1);
                    MessageBox(hWnd, L"잡았다.", L"야호", MB_OK);
                    break;
                }
            }

            /// 타이머 설정을 변경 : 게임 지속에 관련된 코드
            g_interval -= 51;           /// 음수가 되면 타이머가 동작하지 않음!!!
            if (100 >= g_interval)
                g_interval = 100;
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_interval, NULL);

            /// 상대의 좌표 이동 완료
            InvalidateRect(hWnd, NULL, TRUE);
        }
        /// 게임 시간을 관리하는 타이머가 도착
        else if (2 == wParam)
        {
            g_time.right -= 10;
            /// 게임 종료 직전 g_timer의 값은 1인 상태
            g_timer--;      /// 1초 감소 => 화면에는 출력되지 않은 상태
            if (0 >= g_timer)       /// 게임 종료 조건
            {
                g_gameover = TRUE;      /// 게임 종료 설정
                KillTimer(hWnd, 1);     /// 상대의 움직임 제거
                KillTimer(hWnd, 2);     /// 게임 시간 확인 타이머 제거
                InvalidateRect(hWnd, NULL, TRUE);   /// 게임 시간 0 표시
            }
        }
    }
        break;

        case WM_KEYDOWN:
        {
            /// 게임 중인지 여부를 확인
            if (g_gameover)
                break;
            /// 게임 오버라면 나의 입력은 무시해야 함.
            switch (wParam)
            {
            case VK_F3:
            {
                /// 게임 시간 증가
                g_timer = 100000;
            }
                break;
            case VK_F4:
            {
                /// 무적 상태 만들기. 토글 가능!
                if (FALSE == g_cheat)
                    g_cheat = TRUE;
                else
                    g_cheat = FALSE;
            }
                break;
            case VK_LEFT:
            {
                g_me.left -= 10;
                g_me.right -= 10;
            }
                break;
            case VK_RIGHT:
            {
                g_me.left += 10;
                g_me.right += 10;
            }
                break;
            case VK_UP:
            {
                g_me.top -= 10;
                g_me.bottom -= 10;
            }
                break;
            case VK_DOWN:
            {
                g_me.top += 10;
                g_me.bottom += 10;
            }
                break;
            }
            /// 나의 좌표 값 이동이 완료
            
            /// 내가 음식을 먹었는지 확인
            RECT ret;
            if (IntersectRect(&ret, &g_me, &g_food))
            {
                g_food_eat++;

                g_food.left = rand() % 700;
                g_food.top = rand() % 700;
                g_food.right = g_food.left + 50;
                g_food.bottom = g_food.top + 50;
            }

            /// 화면 다시 그리기를 OS에게 요청
            InvalidateRect(hWnd, NULL, TRUE);
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
            /// 나의 객체 그리기
            HBRUSH myBrush, osBrush;
            myBrush = CreateSolidBrush(RGB(100, 100, 200));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            TextOut(hdc, g_me.left + 40, g_me.top + 60, L"This is Me", 10);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            /// 상대 객체 그리기
            HPEN myPen, osPen;
            myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 50));
            osPen = (HPEN)SelectObject(hdc, myPen);
            Ellipse(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
            TextOut(hdc, g_you.left + 40, g_you.top + 60, L"This is You", 11);
            SelectObject(hdc, osPen);
            DeleteObject(myPen);
            /// 음식 객체 그리기
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);

            /// 게임의 남은 시간을 출력
            WCHAR buf[128] = { 0, };
            wsprintf(buf, L"먹은 음식 개수[ %d ] 남은 시간[ %d ]", g_food_eat, g_timer);
            TextOut(hdc, 700, 10, buf, lstrlenW(buf));

            Rectangle(hdc, g_time.left, g_time.top, g_time.right, g_time.bottom);
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
