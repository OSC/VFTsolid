//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit13.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm13 *Form13;
//---------------------------------------------------------------------------
__fastcall TForm13::TForm13(int sectionsel0,float Angle0,long Angle1,float Bngle1,
  float Bngle2,float Bngle3,float Bngle4,TComponent* Owner) : TForm(Owner)
{sect_ic=sectionsel0;setAngle0(Angle0);setAngle1(Angle1);
 setBngle1(Bngle1);setBngle2(Bngle2);setBngle3(Bngle3);setBngle4(Bngle4);
 if(sect_ic)RadioButton1->Checked=true;else RadioButton2->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm13::Button1Click(TObject *Sender) //Necessary since Edit ignores 0
//{if(Edit2->Text=="0")nodeno=0;else nodeno=atol(Edit2->Text.c_str());
{nodeno=StrToInt(Edit2->Text);
 Form1->secthea_public();
}
//---------------------------------------------------------------------------
void TForm13::setSection0(int s){sect_ic=s;}
void TForm13::setAngle0(float s){Edit1->Text=FloatToStr(s);}
void TForm13::setAngle1(long s){Edit2->Text=FloatToStr(float(s));}
void TForm13::setBngle1(float s){Edit3->Text=FloatToStr(s);}
void TForm13::setBngle2(float s){Edit4->Text=FloatToStr(s);}
void TForm13::setBngle3(float s){Edit5->Text=FloatToStr(s);}
void TForm13::setBngle4(float s){Edit6->Text=FloatToStr(s);}
//---------------------------------------------------------------------------
int TForm13::getSection0(){return sect_ic;}
float TForm13::getAngle0(){}
long TForm13::getAngle1(){return nodeno;}
float TForm13::getBngle1(){}
float TForm13::getBngle2(){}
float TForm13::getBngle3(){}
float TForm13::getBngle4(){}
//---------------------------------------------------------------------------
void __fastcall TForm13::RadioButton1Click(TObject *Sender){sect_ic=abs(sect_ic);}
void __fastcall TForm13::RadioButton2Click(TObject *Sender){sect_ic= -abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm13::Button2Click(TObject *Sender)
{Close();Form1->SectqsdTRASH_public();
}
//---------------------------------------------------------------------------

