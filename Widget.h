

#ifndef WIDGET_H
#define WIDGET_H

#include "win32gui.h"

// A widget that is not embedded in a parent widget is called a window.
class Widget {

protected:
	HWND parent;
	HFONT font;

	void wrap(HWND hWnd){

		// default font
		this->setFont(L"Verdana", 13);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	}

public:

	HWND hWnd;

	// OnCommand (WPARAM wParam, LPARAM lParam)
	void(*CommandHandler)(int nCode, void* extra) = 0;

	static Widget* FromHandle(HWND hWnd){

		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWLP_USERDATA);

		Widget *widget = 0;

		if (ptr){
			widget = reinterpret_cast<Widget*>(ptr);
		}

		return widget;
	}

	void setEnabled(bool enabled){
		EnableWindow(this->hWnd, enabled);
	}

	void show(){
		ShowWindow(hWnd, SW_SHOW);
	}

	void hide(){
		ShowWindow(hWnd, SW_HIDE);
	}

	void setText(const TCHAR *text){
		SetWindowText(this->hWnd, text);
	}

	void setFont(const TCHAR *name, int fontSize, bool bold = false){

		DeleteObject(font);

		font = CreateFont(
			fontSize,					// font size
			0,							// width or zero to pick the closest value		
			0,							// nEscapement
			0,							// the angle in tenths of degrees
			bold ? FW_BOLD : FW_MEDIUM,	// font weight using integers or constants
			false ? TRUE : FALSE,		// true for italic 
			FALSE,						// true for underline
			FALSE,						// true for strikeout
			ANSI_CHARSET,				// charset
			OUT_TT_PRECIS,				// the output precision
			CLIP_DEFAULT_PRECIS,		// clipping precision
			DEFAULT_QUALITY,			// the output quality
			DEFAULT_PITCH | FF_DONTCARE,	// the pitch and the family of the font
			name						// pointer to string that specifies typeface name of font
			);

		SendMessage(hWnd, WM_SETFONT, (WPARAM)font, TRUE);
	}

	const TCHAR* getText(){

		int len = GetWindowTextLength(this->hWnd);

		TCHAR *res = new TCHAR[len + 1];

		GetWindowText(this->hWnd, res, len + 1);

		return res;
	}

	void setSize(int width, int height)
	{
		RECT rc;

		// coordinates to the window entire screen
		GetWindowRect(this->hWnd, &rc);

		// child window's position relative to its parent
		MapWindowPoints(HWND_DESKTOP, GetParent(this->hWnd), (LPPOINT)&rc, 2);

		MoveWindow(this->hWnd, rc.left, rc.top, width, height, true);
	}

	void setPosition(int x, int y)
	{
		RECT rc;

		// coordinates to the window entire screen
		GetWindowRect(this->hWnd, &rc);

		// no need to do adjust - width height stay the same relative to whatever
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		MoveWindow(this->hWnd, x, y, width, height, true);
	}

	Widget(){
		// do nothing?
	}

	~Widget(){
		DeleteObject(font);
		DestroyWindow(hWnd);
	}


};


class Button : public Widget {

public:
	Button(const TCHAR *text, Widget *parent) {

		hWnd = CreateWindowEx(0, L"BUTTON", text, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX, 0, 0, 100, 20, parent->hWnd, 0, GetModuleHandle(NULL), 0);

		wrap(hWnd);
	}

};


class TextBox : public Widget {

public:
	TextBox(const TCHAR *text, Widget *parent){

		hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", text, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | ES_AUTOHSCROLL | ES_MULTILINE, 0, 0, 100, 20, parent->hWnd, 0, GetModuleHandle(NULL), 0);

		wrap(hWnd);
	}

};

struct ComboBoxItem {

	TCHAR text[256];
	void *data;
};


class ComboBox : public Widget {

public:
	ComboBox(Widget *parent){

		// DropDownStyle
		this->hWnd = CreateWindowEx(0, WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST,
			0, 0, 220, 16, parent->hWnd, 0, GetModuleHandle(NULL), 0);

		wrap(hWnd);

		// item height is 16 by default on .net editor
		SendMessage(this->hWnd, CB_SETITEMHEIGHT, -1, 16);
		SendMessage(this->hWnd, CB_SETITEMHEIGHT, 0, 16);
	}

	void addItem(const TCHAR *str, int index = -1, void *data = 0){

		// CB_ERR. If there is insufficient space available to store the new string, it is CB_ERRSPACE. 
		int new_index = SendMessage(this->hWnd, CB_INSERTSTRING, index, (LPARAM)str);

		if (data){
			SendMessage(this->hWnd, CB_SETITEMDATA, new_index, (LPARAM)data);
		}
	}

	void removeItem(int index){
		SendMessage(this->hWnd, CB_DELETESTRING, index, 0);
	}

	void setSelectedIndex(int index){
		SendMessage(this->hWnd, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
	}

	int getSelectedIndex(){
		return (int)SendMessage(this->hWnd, CB_GETCURSEL, 0, 0);
	}

	// responsibility of the caller to clear heap?
	ComboBoxItem* getItem(int index){

		ComboBoxItem *cbi = new ComboBoxItem();

		int len = SendMessage(this->hWnd, CB_GETLBTEXTLEN, index, 0);

		if (!len){
			return 0;
		}

		TCHAR *buf = new TCHAR[len + 1];

		// delete would somehow fail if buf was smaller than incoming text
		SendMessage(this->hWnd, CB_GETLBTEXT, index, (LPARAM)buf);

		// set text
		wcscpy(cbi->text, buf);

		// set data
		cbi->data = (void*)SendMessage(this->hWnd, CB_GETITEMDATA, index, 0);

		// free the memory
		delete buf;

		return cbi;
	}

};

class Label : public Widget {

public:
	Label(TCHAR *text, Widget *parent){

		hWnd = CreateWindowEx(0, L"STATIC", text, WS_VISIBLE | WS_CHILD | SS_LEFT, 0, 0, 100, 20, parent->hWnd, 0, GetModuleHandle(NULL), 0);

		wrap(hWnd);
	}
};


#endif