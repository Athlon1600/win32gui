
#ifndef APPLICATION_H
#define APPLICATION_H

#include "win32gui.h"

class Application {

private:
	static HINSTANCE hInstance;

public:

	Application(){

		//Application::m_HINSTANCE = ::GetModuleHandle(NULL);
		//Application::m_instance = this;

		INITCOMMONCONTROLSEX icce;
		icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icce.dwICC = 0
			| ICC_WIN95_CLASSES
			| ICC_BAR_CLASSES
			| ICC_COOL_CLASSES
			;

		InitCommonControlsEx(&icce);
	}

	void run(){

		// Main message loop:
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			HWND hwnd = GetForegroundWindow();
			if (hwnd == NULL || !IsDialogMessage(hwnd, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

	}

};

#endif