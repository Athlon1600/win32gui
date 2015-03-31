
#ifndef TRAYICON_H
#define TRAYICON_H

#include "win32gui.h"

#define TRAY_ICON_CLASS TEXT("tray_icon_class")
#define WM_TRAYICON (WM_USER+5000)

class TrayIcon {

private:
	NOTIFYICONDATA nid;

public:
	HMENU hMenu;
	HWND hWnd;

	TrayIcon(){

		// all to zero
		memset(&nid, 0, sizeof(nid));

		nid.cbSize = NOTIFYICONDATA_V2_SIZE;// sizeof(NOTIFYICONDATA);

		registerWindowClass();

		// dummy window
		hWnd = CreateWindowEx(0, TRAY_ICON_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(NULL), 0);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

		nid.hWnd = hWnd;

		// Flags that either indicate which of the other members of the structure contain valid data or provide additional information to the tooltip as to how it should display. 
		nid.uFlags = NIF_TIP | NIF_MESSAGE;

		nid.uCallbackMessage = WM_TRAYICON;

		// text for a standard tooltip - TCHAR[64]
		wcscpy(nid.szTip, L"TrayIcon Text");
	}

	void setIcon(HICON icon){

		nid.hIcon = icon;

		nid.uFlags |= NIF_ICON;
	}

	void setContextMenu(HMENU menu){
		hMenu = menu;// GetSubMenu(menu, 0);
	}

	void setToolTip(TCHAR *text){

		nid.uFlags |= NIF_TIP;

		wcscpy(nid.szTip, text);
	}

	void setMenuItemBitmap(HBITMAP bmap, int menuId){
		MENUITEMINFO mii = { 0 };

		// HBITMAP bmpMenu = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 16, 16, LR_DEFAULTSIZE | LR_DEFAULTCOLOR);//LR_LOADTRANSPARENT);
		// SetMenuItemBitmaps(GetSubMenu(GetMenu(form1->hWnd), 0), i, MF_BYPOSITION, bmpMenu, bmpMenu);

		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_BITMAP;
		mii.hbmpItem = bmap; // HBMMENU_MBAR_CLOSE;

		SetMenuItemInfo(hMenu, menuId, false, &mii);

		//DeleteObject(bmap);
	}

	void setMenuItemIcon(HICON icon, int menuId){

		//HICON icon = (HICON)LoadImage(hInstance, MAKEINTRESOURCEW(IDI_ICON3), IMAGE_ICON, 32, 32, 0);
		ICONINFO iconinfo;

		GetIconInfo(icon, &iconinfo);
		HBITMAP hBitmap = iconinfo.hbmColor;

		setMenuItemBitmap(hBitmap, menuId);
	}

	void showMessage(const TCHAR *title, const TCHAR *text){

		nid.uTimeout = 5000;

		// balloon message  - Type: TCHAR[256]
		wcscpy(nid.szInfo, text);

		// title for balloon notification TCHAR[64];
		wcscpy(nid.szInfoTitle, title);

		// behavior and appearance of a balloon
		nid.dwInfoFlags = NIIF_INFO;

		nid.uFlags |= NIF_INFO;

		Shell_NotifyIcon(NIM_MODIFY, &nid);
	}

	void show(){
		Shell_NotifyIcon(NIM_ADD, &nid);
	}

	void hide(){
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}

	~TrayIcon(){
		Shell_NotifyIcon(NIM_DELETE, &nid);

		// 	ZeroMemory(&m_sysTray, sizeof(NOTIFYICONDATA));
	}

	void(*onMenuItemClick)(int id, HMENU menu) = 0;

private:

	LRESULT CALLBACK LocalWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

		if (lParam == WM_RBUTTONUP){

			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);

			/*

			the return value is the menu-item identifier of the item that the user selected.
			If the user cancels the menu without making a selection, or if an error occurs, the return value is zero.

			*/

			int cmd = TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hWnd, NULL);

			if (cmd && onMenuItemClick){
				onMenuItemClick(cmd, hMenu);
			}

			PostMessage(hWnd, WM_NULL, 0, 0);
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){

		if (Msg == WM_TRAYICON){

			long ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);

			if (ptr != 0){

				TrayIcon *ti = (TrayIcon*)ptr;

				ti->LocalWindowProc(hWnd, Msg, wParam, lParam);
			}
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	int getNotifyIconDataSize()
	{
		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));

		// Before calling the GetVersionEx function, set the dwOSVersionInfoSize member of the structure as 
		// appropriate to indicate which data structure is being passed to this function.
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		if (GetVersionEx(&osvi))
		{
			// got this from github
			if (osvi.dwMajorVersion >= 6)
			{
				return sizeof(NOTIFYICONDATA);
			}
			else if (osvi.dwMajorVersion == 5)
			{
				if (osvi.dwMinorVersion >= 1)
					return NOTIFYICONDATA_V3_SIZE;
				else
					return NOTIFYICONDATA_V2_SIZE;
			}
			else
			{
				return NOTIFYICONDATA_V1_SIZE;
			}
		}

		return sizeof(NOTIFYICONDATA);
	}

	static void registerWindowClass(){

		WNDCLASSEX wcex;

		memset(&wcex, 0, sizeof(wcex));

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.lpfnWndProc = WindowProc;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.lpszClassName = TRAY_ICON_CLASS;

		RegisterClassEx(&wcex);
	}


};

#endif