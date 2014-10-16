//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit5.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(long NodeNum,float X,float Y,float Z,int ResValid,float Value,TComponent* Owner) : TForm(Owner)
{setNodeQueryNum(NodeNum);setNodeQueryXCor(X);setNodeQueryYCor(Y);setNodeQueryZCor(Z);setNodeQueryValue(ResValid,Value);
}
//---------------------------------------------------------------------------
//void TForm5::setNodeQueryNum(long s){Edit1->Text=IntToStr(s);}
void TForm5::setNodeQueryNum(long s){Edit1->Text=FloatToStr(float(s));}

void TForm5::setNodeQueryXCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// long double ss=double(s);
// Edit2->Text=FloatToStrF(ss,ffGeneral,15,5);
//// Edit2->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit2->Text=FloatToStr(s);
}
void TForm5::setNodeQueryYCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit3->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit3->Text=FloatToStr(s);
}
void TForm5::setNodeQueryZCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit4->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit4->Text=FloatToStr(s);
}
void TForm5::setNodeQueryValue(int valid,float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(valid)Edit5->Text=FloatToStrF(s,ffGeneral,15,5);
 if(valid)Edit5->Text=FloatToStr(s);
 else Edit5->Text="********";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::Button1Click(TObject *Sender)
{Close();Form1->QNodeTRASH_public();
}
//---------------------------------------------------------------------------



