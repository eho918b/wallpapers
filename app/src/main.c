#include <windows.h>
#include <commctrl.h>
#include "ribbon.h"
#include "config.h"

#define IDC_MAIN_EDIT 101
#define IDC_MAIN_STATUS	102

// global variables
const char g_szClassName[] = "myWindowClass";
extern const config_t config;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			// Ribbon
			if(!InitializeFramework(hwnd)) return(FALSE);
			
			// Edit
			HFONT hfDefault;
			HWND hEdit;

			// Create Edit Control
			hEdit = CreateWindowEx(0, "EDIT", "", 
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 
				0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
			if(hEdit == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);
			hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			SetWindowText(hEdit, "**************************************************\r\n*                     SC2 AI                     *\r\n*                     Eric Ho                    *\r\n**************************************************");
			SendMessage(hEdit, EM_SETREADONLY, 1, 0);

			// Status Bar
			HWND hStatus;
			int statwidths[] = {100, -1};
			hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
				hwnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);
			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"Hello!");

			// Font
			HFONT hFont = CreateFont(14, 8, 0, 0, 0, FALSE, 0, 0, OEM_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, "Consolas");
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

			// done
			break;
		}
		case WM_SIZE:
		{
			HWND hEdit, hStatus;
			RECT rcClient;

			// Calculate remaining height and size edit
			GetClientRect(hwnd, &rcClient);

			// resize edit
			hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 150, rcClient.right, rcClient.bottom-170, SWP_NOZORDER);

			// resize status
			hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
			SetWindowPos(hStatus, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);

			// done
			break;
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			DestroyRibbon();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// init
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// COM
	if(CoInitialize(0)) return FALSE;

	//Step 1: Registering the Window Class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"SC2AI by Eric Ho",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, config.app.width, config.app.height, NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	// clean up
	CoUninitialize();

	// done
	return Msg.wParam;
}
