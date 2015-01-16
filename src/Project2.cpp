//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Unit30.cpp", Form30);
USEFORM("Unit31.cpp", Form31);
USEFORM("Unit29.cpp", Form29);
USEFORM("Unit3.cpp", Form3);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit7.cpp", Form7);
USEFORM("Unit8.cpp", Form8);
USEFORM("Unit5.cpp", Form5);
USEFORM("Unit6.cpp", Form6);
USEFORM("Unit28.cpp", Form28);
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit15.cpp", Form15);
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit25.cpp", Form25);
USEFORM("Unit26.cpp", Form26);
USEFORM("Unit21.cpp", Form21);
USEFORM("Unit22.cpp", Form22);
USEFORM("Unit9.cpp", Form9);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
