

#ifndef WIN32GUI_H
#define WIN32GUI_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>



// for initcommoncontrols..
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

// for tray icon
#include <shellapi.h>




#include "Widget.h"
#include "TrayIcon.h"

#include "Application.h"
#include "Form.h"



#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")



#endif