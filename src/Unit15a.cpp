//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit15.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm15 *Form15;
//---------------------------------------------------------------------------
__fastcall TForm15::TForm15(TComponent* Owner) : TForm(Owner){}
//---------------------------------------------------------------------------
void TForm15::setQNDnodeN(int valid,long s)
{if(valid==1)Edit1->Text=FloatToStr(float(s));else if(valid==2)Edit5->Text=FloatToStr(float(s));
 else if(valid== -1)Edit1->Text="********";else Edit5->Text="********";
}
//---------------------------------------------------------------------------
void TForm15::setQNDnodeXYZD(int valid,float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(valid==1)Edit2->Text=FloatToStrF(s,ffGeneral,15,5);      else if(valid==2)Edit3->Text=FloatToStrF(s,ffGeneral,15,5);
// else if(valid==3)Edit4->Text=FloatToStrF(s,ffGeneral,15,5); else if(valid==4)Edit6->Text=FloatToStrF(s,ffGeneral,15,5);
// else if(valid==5)Edit7->Text=FloatToStrF(s,ffGeneral,15,5); else if(valid==6)Edit8->Text=FloatToStrF(s,ffGeneral,15,5);
// else if(valid==7)Edit10->Text=FloatToStrF(s,ffGeneral,15,5);else if(valid==8)Edit11->Text=FloatToStrF(s,ffGeneral,15,5);
// else if(valid==9)Edit12->Text=FloatToStrF(s,ffGeneral,15,5);else if(valid==10)Edit9->Text=FloatToStrF(s,ffGeneral,15,5);

 if(valid==1)Edit2->Text=FloatToStr(s);      else if(valid==2)Edit3->Text=FloatToStr(s);
 else if(valid==3)Edit4->Text=FloatToStr(s); else if(valid==4)Edit6->Text=FloatToStr(s);
 else if(valid==5)Edit7->Text=FloatToStr(s); else if(valid==6)Edit8->Text=FloatToStr(s);
 else if(valid==7)Edit10->Text=FloatToStr(s);else if(valid==8)Edit11->Text=FloatToStr(s);
 else if(valid==9)Edit12->Text=FloatToStr(s);else if(valid==10)Edit9->Text=FloatToStr(s);
 else if(valid== -1)Edit2->Text="********"; else if(valid== -2)Edit3->Text="********";
 else if(valid== -3)Edit4->Text="********"; else if(valid== -4)Edit6->Text="********";
 else if(valid== -5)Edit7->Text="********"; else if(valid== -6)Edit8->Text="********";
 else if(valid== -7)Edit10->Text="********";else if(valid== -8)Edit11->Text="********";
 else if(valid== -9)Edit12->Text="********";else Edit9->Text="********";
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button1Click(TObject *Sender)
{Close();Form1->QNDistTRASH_public();
}
//---------------------------------------------------------------------------

