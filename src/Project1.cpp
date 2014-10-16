//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit4.cpp", Form4);
USEFORM("unit3.cpp", Form3);
USEFORM("Unit7.cpp", Form7);
USEFORM("Unit8.cpp", Form8);
USEFORM("Unit13.cpp", Form13);
USEFORM("Unit22.cpp", Form22);
USEFORM("Unit6.cpp", Form6);
USEFORM("Unit28.cpp", Form28);
USEFORM("Unit24.cpp", Form24);
USEFORM("Unit5.cpp", Form5);
USEFORM("Unit15.cpp", Form15);
USEFORM("Unit9.cpp", Form9);
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit14.cpp", Form14);
USEFORM("Unit16.cpp", Form16);
USEFORM("Unit21.cpp", Form21);
USEFORM("Unit25.cpp", Form25);
USEFORM("Unit19.cpp", Form19);
USEFORM("Unit11.cpp", Form11);
USEFORM("Unit20.cpp", Form20);
USEFORM("Unit10.cpp", Form10);
USEFORM("Unit18.cpp", Form18);
USEFORM("Unit26.cpp", Form26);
USEFORM("Unit12.cpp", Form12);
USEFORM("Unit29.cpp", Form29);
USEFORM("Unit17.cpp", Form17);
USEFORM("Unit27.cpp", Form27);
USEFORM("Unit30.cpp", Form30);
USEFORM("Unit31.cpp", Form31);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
