//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(long NodeNum,float X,float Y,float Z,TComponent* Owner) : TForm(Owner)
{setNodeQueryNum(NodeNum);setNodeQueryXCor(X);setNodeQueryYCor(Y);setNodeQueryZCor(Z);
}
//---------------------------------------------------------------------------
//void TForm5::setNodeQueryNum(long s){Edit1->Text=IntToStr(s);}
void TForm3::setNodeQueryNum(long s){Edit1->Text=FloatToStr(float(s)+0.5f);}

void TForm3::setNodeQueryXCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// long double ss=double(s);
// Edit2->Text=FloatToStrF(ss,ffGeneral,15,5);
//// Edit2->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit2->Text=FloatToStr(s);
}
void TForm3::setNodeQueryYCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit3->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit3->Text=FloatToStr(s);
}
void TForm3::setNodeQueryZCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit4->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit4->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{Form1->QNodeTRASH_public();
}
//---------------------------------------------------------------------------

