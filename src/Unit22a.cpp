//---------------------------------------------------------------------------
// Formerly VFTgen Unit17
#include <vcl.h>
#pragma hdrstop

#include "Unit22.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm22 *Form22;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
String TForm22::getEdit1(){return Edit1->Text;}
void TForm22::setEdit1(String s){Edit1->Text=s;}
//---------------------------------------------------------------------------
double TForm22::getEdit2(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit2->Text);
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit3(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit3->Text);
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit4(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit4->Text);
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit5(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit5->Text);
						  }
//---------------------------------------------------------------------------
void TForm22::setEdit2(double x){Edit2->Text=FloatToStr(x);}
void TForm22::setEdit3(double x){Edit3->Text=FloatToStr(x);}
void TForm22::setEdit4(double x){Edit4->Text=FloatToStr(x);}
void TForm22::setEdit5(double x){Edit5->Text=FloatToStr(x);}
//---------------------------------------------------------------------------
void __fastcall TForm22::Button1Click(TObject *Sender)
{
 Form1->WeldParam_public();
 Close();
}
//---------------------------------------------------------------------------

