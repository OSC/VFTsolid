//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit10.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm10 *Form10;
//---------------------------------------------------------------------------
__fastcall TForm10::TForm10(float g0,float g1,float g2,float g3,float g4,float g5,TComponent* Owner) : TForm(Owner)
{setC1(g0);setC2(g1);setC3(g2);setC4(g3);setC5(g4);setC6(g5);ac1=g0;ac2=g1;ac3=g2;ac4=g3;ac5=g4;ac6=g5;
}
//---------------------------------------------------------------------------
void TForm10::setC1(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit1->Text=FloatToStrF(s,ffGeneral,15,5);
Edit1->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void TForm10::setC2(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit2->Text=FloatToStrF(s,ffGeneral,15,5);
Edit2->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void TForm10::setC3(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit3->Text=FloatToStrF(s,ffGeneral,15,5);
Edit3->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void TForm10::setC4(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit4->Text=FloatToStrF(s,ffGeneral,15,5);
Edit4->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void TForm10::setC5(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit5->Text=FloatToStrF(s,ffGeneral,15,5);
Edit5->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
void TForm10::setC6(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit6->Text=FloatToStrF(s,ffGeneral,15,5);
Edit6->Text=FloatToStr(s);
}
//---------------------------------------------------------------------------
float TForm10::getC1(){return ac1;}float TForm10::getC2(){return ac2;}
float TForm10::getC3(){return ac3;}float TForm10::getC4(){return ac4;}
float TForm10::getC5(){return ac5;}float TForm10::getC6(){return ac6;}
//---------------------------------------------------------------------------
void __fastcall TForm10::Button1Click(TObject *Sender)
{  //Necessary since Edit ignores 0
// if(Edit1->Text=="0")ac1=0.;else ac1=atof(Edit1->Text.c_str());
// if(Edit2->Text=="0")ac2=0.;else ac2=atof(Edit2->Text.c_str());
// if(Edit3->Text=="0")ac3=0.;else ac3=atof(Edit3->Text.c_str());
// if(Edit4->Text=="0")ac4=0.;else ac4=atof(Edit4->Text.c_str());
// if(Edit5->Text=="0")ac5=0.;else ac5=atof(Edit5->Text.c_str());
// if(Edit6->Text=="0")ac6=0.;else ac6=atof(Edit6->Text.c_str());

 ac1=StrToFloat(Edit1->Text);
 ac2=StrToFloat(Edit2->Text);
 ac3=StrToFloat(Edit3->Text);
 ac4=StrToFloat(Edit4->Text);
 ac5=StrToFloat(Edit5->Text);
 ac6=StrToFloat(Edit6->Text);
 Close();Form1->genaxhea_public();
}
//---------------------------------------------------------------------------


