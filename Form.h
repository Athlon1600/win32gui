

#ifndef FORM_H
#define FORM_H

#include "win32gui.h"

#define WINDOW_CLASS TEXT("window_class")

class Form : public Widget {

private:

	static LRESULT CALLBACK globalWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		// redirect message to a specific FORM instance
		Form *form = 0;

		if (Msg == WM_NCCREATE){

			// CREATESTRUCT contains identical parameters used in CreateWindowEx
			//LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			CREATESTRUCT *lpcs = (CREATESTRUCT*)lParam;

			form = (Form*)(lpcs->lpCreateParams);

			SetWindowLongPtr(hWnd, GWL_USERDATA, (ULONG_PTR)form);
		}
		else {
			form = (Form*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}

		if (form){
			return form->LocalWindowProc(hWnd, Msg, wParam, lParam);
		}

		if (Msg == WM_NCDESTROY){
			delete form;
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	LRESULT CALLBACK LocalWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

		if (Msg == WM_COMMAND){

			int id = LOWORD(wParam);
			DWORD nCode = HIWORD(wParam);
			HWND handle = (HWND)lParam;

			Widget *widget = Widget::FromHandle(handle);

			if (widget && widget->CommandHandler){
				widget->CommandHandler(nCode, 0);
			}
		}

		if (Msg == WM_DESTROY){
			PostQuitMessage(0);
			return 0;
		}

		if (WindowProc){
			return WindowProc(hWnd, Msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

public:

	WNDPROC WindowProc = 0;
	TrayIcon *trayIcon = 0;

	Form(TCHAR *windowName, int windowStyle = WS_OVERLAPPEDWINDOW) {

		registerWindowClass();

		this->hWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,	/* Extended possibilites for variation */
			WINDOW_CLASS,       /* Classname */
			windowName,			/* Title Text */
			windowStyle,		/* default window */
			CW_USEDEFAULT,		/* Windows decides the position */
			CW_USEDEFAULT,		/* where the window ends up on the screen */
			300,				/* The programs width */
			200,                /* and height in pixels */
			HWND_DESKTOP,       /* The window is a child-window to desktop */
			NULL,               /* No menu */
			GetModuleHandle(NULL),           /* Program Instance handler */
			(void*)this                 /* No Window Creation data */
			);

		//wrap(hWnd);
	}

	// each window class shares the same wndproc
	static void registerWindowClass(){

		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = globalWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = 0;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = WINDOW_CLASS;
		wcex.hIconSm = 0;

		RegisterClassEx(&wcex);
	}

	void setIcon(HICON icon){
		SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		SendMessage(this->hWnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
	}

	void center()
	{
		RECT rc;
		GetWindowRect(hWnd, &rc);

		int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

		SetWindowPos(hWnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	~Form(){
		UnregisterClass(WINDOW_CLASS, GetModuleHandle(NULL));
	}

};


#endif