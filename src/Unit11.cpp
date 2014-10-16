//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit11.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm11 *Form11;
//---------------------------------------------------------------------------
__fastcall TForm11::TForm11
 (int sectionsel0,float Angle0,float Angle1,float Angle2,
  float Bngle1,float Bngle2,float Bngle3,
  float Bngle4,float Bngle5,float Bngle6,
  float Bngle7,float Bngle8,float Bngle9,TComponent* Owner) : TForm(Owner)
{ScrollBar1->Max=100;ScrollBar1->Min=0;ScrollBar1->LargeChange=10;ScrollBar1->SmallChange=1;
 sect_ic=sectionsel0;ac1=Angle1;ac2=Angle2;setAngle0(Angle0);
 setBngle1(Bngle1);setBngle2(Bngle2);setBngle3(Bngle3);
 setBngle4(Bngle4);setBngle5(Bngle5);setBngle6(Bngle6);
 setBngle7(Bngle7);setBngle8(Bngle8);setBngle9(Bngle9);
 if(sect_ic)RadioButton1->Checked=true;else RadioButton2->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button1Click(TObject *Sender)
{float AA=0.,VV=0.;
 if(Edit1->Text=="0") //Necessary since Edit ignores 0
   {if(ac2>0. && ac1<0.)
      {if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
       ScrollBar1->Position=int( -100.*ac1/VV+0.5);
      }
    else ScrollBar1->Position=0;
   }
 else
   {//AA=atof(Edit1->Text.c_str());
    AA=StrToFloat(Edit1->Text);
	if(AA>ac2)AA=ac2;else if(AA<ac1)AA=ac1;
    if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
    ScrollBar1->Position=int(100.*(AA-ac1)/VV+0.5);
   }
//
// if(Edit2->Text=="0")x1=0.;else x1=atof(Edit2->Text.c_str());
// if(Edit3->Text=="0")y1=0.;else y1=atof(Edit3->Text.c_str());
// if(Edit4->Text=="0")z1=0.;else z1=atof(Edit4->Text.c_str());
// if(Edit5->Text=="0")x2=0.;else x2=atof(Edit5->Text.c_str());
// if(Edit6->Text=="0")y2=0.;else y2=atof(Edit6->Text.c_str());
// if(Edit7->Text=="0")z2=0.;else z2=atof(Edit7->Text.c_str());
// if(Edit8->Text=="0")x3=0.;else x3=atof(Edit8->Text.c_str());
// if(Edit9->Text=="0")y3=0.;else y3=atof(Edit9->Text.c_str());
// if(Edit10->Text=="0")z3=0.;else z3=atof(Edit10->Text.c_str());

 x1=StrToFloat(Edit2->Text);
 y1=StrToFloat(Edit3->Text);
 z1=StrToFloat(Edit4->Text);
 x2=StrToFloat(Edit5->Text);
 y2=StrToFloat(Edit6->Text);
 z2=StrToFloat(Edit7->Text);
 x3=StrToFloat(Edit8->Text);
 y3=StrToFloat(Edit9->Text);
 z3=StrToFloat(Edit10->Text);
 Form1->secthea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm11::ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit1->Text=FloatToStr(ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.);
}
//---------------------------------------------------------------------------
void TForm11::setAngle0(float s)
{float VV=0.,BB=0.;
 if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
 if(s>ac2)BB=ac2;else if(s<ac1)BB=ac1;else BB=s;
 Edit1->Text=FloatToStr(BB);ScrollBar1->Position=int(100.*(BB-ac1)/VV+0.5);
}
//---------------------------------------------------------------------------
void TForm11::setAngle1(float s){ac1=s;}
void TForm11::setAngle2(float s){ac2=s;}
void TForm11::setSection0(int s){sect_ic=s;}
//---------------------------------------------------------------------------
void TForm11::setBngle1(float s){Edit2->Text=FloatToStr(s);}
void TForm11::setBngle2(float s){Edit3->Text=FloatToStr(s);}
void TForm11::setBngle3(float s){Edit4->Text=FloatToStr(s);}
void TForm11::setBngle4(float s){Edit5->Text=FloatToStr(s);}
void TForm11::setBngle5(float s){Edit6->Text=FloatToStr(s);}
void TForm11::setBngle6(float s){Edit7->Text=FloatToStr(s);}
void TForm11::setBngle7(float s){Edit8->Text=FloatToStr(s);}
void TForm11::setBngle8(float s){Edit9->Text=FloatToStr(s);}
void TForm11::setBngle9(float s){Edit10->Text=FloatToStr(s);}
//---------------------------------------------------------------------------
float TForm11::getAngle0(){return ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.;}
float TForm11::getAngle1(){return 0.;}
float TForm11::getAngle2(){return 0.;}
int TForm11::getSection0(){return sect_ic;}
float TForm11::getBngle1(){return x1;}
float TForm11::getBngle2(){return y1;}
float TForm11::getBngle3(){return z1;}
float TForm11::getBngle4(){return x2;}
float TForm11::getBngle5(){return y2;}
float TForm11::getBngle6(){return z2;}
float TForm11::getBngle7(){return x3;}
float TForm11::getBngle8(){return y3;}
float TForm11::getBngle9(){return z3;}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button3Click(TObject *Sender){Form1->secthea1_public();}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button5Click(TObject *Sender)
{if(sect_ic)sect_ic=sect_ic+10;else sect_ic=sect_ic-10;
 Form1->secthea2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button4Click(TObject *Sender)
{if(sect_ic)sect_ic=sect_ic+100;else sect_ic=sect_ic-100;
 Form1->secthea2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button2Click(TObject *Sender){Form1->secthea3_public();}
//---------------------------------------------------------------------------
void __fastcall TForm11::RadioButton1Click(TObject *Sender){sect_ic=abs(sect_ic);}
void __fastcall TForm11::RadioButton2Click(TObject *Sender){sect_ic= -abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm11::Button6Click(TObject *Sender)
{Close();Form1->Sect3ptTRASH_public();
}
//---------------------------------------------------------------------------

