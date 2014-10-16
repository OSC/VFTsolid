//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit14.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm14 *Form14;
//---------------------------------------------------------------------------
__fastcall TForm14::TForm14
 (int sectionsel0,float Angle0,float Angle1,
  float Bngle1,float Bngle2,float Bngle3,
  float Bngle4,float Bngle5,float Bngle6,
  float Bngle7,float Bngle8,float Bngle9,TComponent* Owner) : TForm(Owner)
{sect_ic=sectionsel0;setAngle0(Angle0);setAngle1(Angle1);
 setBngle1(Bngle1);setBngle2(Bngle2);setBngle3(Bngle3);
 setBngle4(Bngle4);setBngle5(Bngle5);setBngle6(Bngle6);
 setBngle7(Bngle7);setBngle8(Bngle8);setBngle9(Bngle9);
}
//---------------------------------------------------------------------------
void __fastcall TForm14::Button1Click(TObject *Sender)
//{if(Edit1->Text=="0")ac1=0.;else ac1=atof(Edit1->Text.c_str());
// if(Edit2->Text=="0")ac2=0.;else ac2=atof(Edit2->Text.c_str());
// if(Edit3->Text=="0")x1=0.;else x1=atof(Edit3->Text.c_str());
// if(Edit4->Text=="0")y1=0.;else y1=atof(Edit4->Text.c_str());
// if(Edit5->Text=="0")z1=0.;else z1=atof(Edit5->Text.c_str());
// if(Edit6->Text=="0")x2=0.;else x2=atof(Edit6->Text.c_str());
// if(Edit7->Text=="0")y2=0.;else y2=atof(Edit7->Text.c_str());
// if(Edit8->Text=="0")z2=0.;else z2=atof(Edit8->Text.c_str());
// if(Edit9->Text=="0")x3=0.;else x3=atof(Edit9->Text.c_str());
// if(Edit10->Text=="0")y3=0.;else y3=atof(Edit10->Text.c_str());
// if(Edit11->Text=="0")z3=0.;else z3=atof(Edit11->Text.c_str());
{ac1=StrToFloat(Edit1->Text);
 ac2=StrToFloat(Edit2->Text);
 x1=StrToFloat(Edit3->Text);
 y1=StrToFloat(Edit4->Text);
 z1=StrToFloat(Edit5->Text);
 x2=StrToFloat(Edit6->Text);
 y2=StrToFloat(Edit7->Text);
 z2=StrToFloat(Edit8->Text);
 x3=StrToFloat(Edit9->Text);
 y3=StrToFloat(Edit10->Text);
 z3=StrToFloat(Edit11->Text);
// Form1->Crack_output_public(); //Disconnected EFP 11/27/2010
       Close(); //Emergency
}
//---------------------------------------------------------------------------
int TForm14::getSection0(){return sect_ic;}
float TForm14::getAngle0(){return ac1;}
float TForm14::getAngle1(){return ac2;}
float TForm14::getBngle1(){return x1;}
float TForm14::getBngle2(){return y1;}
float TForm14::getBngle3(){return z1;}
float TForm14::getBngle4(){return x2;}
float TForm14::getBngle5(){return y2;}
float TForm14::getBngle6(){return z2;}
float TForm14::getBngle7(){return x3;}
float TForm14::getBngle8(){return y3;}
float TForm14::getBngle9(){return z3;}
void TForm14::setSection0(int s){sect_ic=s;}
void TForm14::setAngle0(float s){Edit1->Text=FloatToStr(s);}
void TForm14::setAngle1(float s){Edit2->Text=FloatToStr(s);}
void TForm14::setBngle1(float s){Edit3->Text=FloatToStr(s);}
void TForm14::setBngle2(float s){Edit4->Text=FloatToStr(s);}
void TForm14::setBngle3(float s){Edit5->Text=FloatToStr(s);}
void TForm14::setBngle4(float s){Edit6->Text=FloatToStr(s);}
void TForm14::setBngle5(float s){Edit7->Text=FloatToStr(s);}
void TForm14::setBngle6(float s){Edit8->Text=FloatToStr(s);}
void TForm14::setBngle7(float s){Edit9->Text=FloatToStr(s);}
void TForm14::setBngle8(float s){Edit10->Text=FloatToStr(s);}
void TForm14::setBngle9(float s){Edit11->Text=FloatToStr(s);}
//---------------------------------------------------------------------------
void __fastcall TForm14::Button2Click(TObject *Sender)
{Close();Form1->SpecSCkTRASH_public();
}
//---------------------------------------------------------------------------

