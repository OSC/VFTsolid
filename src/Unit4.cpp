//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner) : TForm(Owner){}
//---------------------------------------------------------------------------
void TForm4::setQNDnodeN(int valid,long s)
{if(valid==1)Edit1->Text=FloatToStr(float(s));else if(valid==2)Edit5->Text=FloatToStr(float(s));
 else if(valid== -1)Edit1->Text=L"***";else Edit5->Text=L"***";
}
//---------------------------------------------------------------------------
void TForm4::setQNDnodeXYZD(int valid,float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
 if(valid==1)Edit2->Text=FloatToStr(s);      else if(valid==2)Edit3->Text=FloatToStr(s);
 else if(valid==3)Edit4->Text=FloatToStr(s); else if(valid==4)Edit6->Text=FloatToStr(s);
 else if(valid==5)Edit7->Text=FloatToStr(s); else if(valid==6)Edit8->Text=FloatToStr(s);
 else if(valid==7)Edit10->Text=FloatToStr(s);else if(valid==8)Edit11->Text=FloatToStr(s);
 else if(valid==9)Edit12->Text=FloatToStr(s);else if(valid==10)Edit9->Text=FloatToStr(s);
 else if(valid== -1)Edit2->Text=L"***"; else if(valid== -2)Edit3->Text=L"***";
 else if(valid== -3)Edit4->Text=L"***"; else if(valid== -4)Edit6->Text=L"***";
 else if(valid== -5)Edit7->Text=L"***"; else if(valid== -6)Edit8->Text=L"***";
 else if(valid== -7)Edit10->Text=L"***";else if(valid== -8)Edit11->Text=L"***";
 else if(valid== -9)Edit12->Text=L"***";else Edit9->Text=L"***";
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{//Close();
 Form1->QNDistTRASH_public();
}
//---------------------------------------------------------------------------
