

#include "win32gui.h"




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR CmdLine, int CmdShow)
{



	Application app;

	Form form1(L"This is my title");


	TrayIcon ti;

	ti.show();


	Button *btn = new Button(L"Button Text", &form1);
	//btn->CommandHandler = btnhandler;


	btn->setSize(100, 20);
	btn->setPosition(50, 20);


	Label *lvl = new Label(L"this is my label", &form1);
	lvl->setPosition(50, 60);





	TextBox *txt = new TextBox(L"text", &form1);
	txt->setPosition(50, 80);
	



	form1.show();



	app.run();


	return 1;
}