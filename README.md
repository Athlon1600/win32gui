# win32gui

Instead of writing **CreateWindowEx** everywhere and remebering all the constants, flags, and messages that go for each control, I made this helper library that wraps around the Windows API and makes it fast and easy to develop simple GUI applications on Windows platform.

## Example code:

```C++

#include "win32gui.h"

Form *form;
Label* caption;

ComboBox *cb;
TextBox *txt_proxy;

Button *btn_enable;
Button *btn_disable;

void btnEnableHandler(int code, void *extra){
	MessageBox(0, "Button was clicked!", L"caption", 0);
}

int _main(){
	
	Application app;

	// give us a form with everything except maximize box
	form = new Form(L"IE Proxy Switcher", WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX);
	form->setSize(300, 220);


	HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

	form->setIcon(icon);
	
	caption = new Label(L"Nova Proxy Switcher v1.1", form);
	caption->setPosition(50, 20);
	caption->setSize(300, 30);

	caption->setFont(L"Arial", 20, true);

	cb = new ComboBox(form);
	cb->setSize(250, 50);
	cb->setPosition(10, 60);
	//cb->CommandHandler = cbHandler;

	cb->addItem(L"Direct Connection - No Proxy");
	cb->addItem(L"Proxy Auto-Config URL");
	cb->addItem(L"Proxy Server - manual");
	cb->addItem(L"-------");

	txt_proxy = new TextBox(L"", form);
	txt_proxy->setPosition(10, 90);
	txt_proxy->setSize(220, 22);
	//txt_proxy->CommandHandler = textHandler;

	btn_enable = new Button(L"Enable Proxy", form);
	btn_enable->setPosition(10, 120);
	btn_enable->setSize(100, 25);
	btn_enable->CommandHandler = btnEnableHandler;

	btn_disable = new Button(L"Disable Proxy", form);
	btn_disable->setPosition(120, 120);
	btn_disable->setSize(100, 25);

	form->center();
	form->show();

	app.run();
	
	return 0;
}

```


## Result:
![alt](http://i.imgur.com/U4uuZz8.png)

http://www.softpedia.com/get/Internet/Servers/Proxy-Servers/Nova-Proxy-Switcher.shtml
