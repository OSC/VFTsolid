//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit12.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm12 *Form12;
//---------------------------------------------------------------------------
__fastcall TForm12::TForm12(int sectionsel0,float Angle0,float Angle1,
  float Bngle1,float Bngle2,float Bngle3,TComponent* Owner) : TForm(Owner)
{ac1= -180.;ac2=180.;
 ScrollBar1->Max=100;ScrollBar1->Min=0;ScrollBar1->LargeChange=10;ScrollBar1->SmallChange=1;
 ScrollBar2->Max=100;ScrollBar2->Min=0;ScrollBar2->LargeChange=10;ScrollBar2->SmallChange=1;
 sect_ic=sectionsel0;setAngle0(Angle0);setAngle1(Angle1);
 setBngle1(Bngle1);setBngle2(Bngle2);setBngle3(Bngle3);
 if(sect_ic)RadioButton1->Checked=true;else RadioButton2->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button1Click(TObject *Sender)
{int nperiod=0;float AA=0.;
 if(Edit1->Text=="0") //Necessary since Edit ignores 0
   {ang0=0.;ScrollBar1->Position=0;
   }
 else
   {//AA=atof(Edit1->Text.c_str());
	AA=StrToFloat(Edit1->Text);
	nperiod=int((AA+180.-1.)/360.);ang0=AA-360.*float(nperiod);
	ScrollBar1->Position=int(100.*(ang0-ac1)/(ac2-ac1)+0.5);
   }
//
 if(Edit2->Text=="0"){ang1=0.;ScrollBar2->Position=0;}
 else
   {//AA=atof(Edit2->Text.c_str());
	AA=StrToFloat(Edit2->Text);
	nperiod=int((AA+180.-1.)/360.);ang1=AA-360.*float(nperiod);
	ScrollBar2->Position=int(100.*(ang1-ac1)/(ac2-ac1)+0.5);
   }
//
// if(Edit3->Text=="0")x1=0.;else x1=atof(Edit3->Text.c_str());
// if(Edit4->Text=="0")y1=0.;else y1=atof(Edit4->Text.c_str());
// if(Edit5->Text=="0")z1=0.;else z1=atof(Edit5->Text.c_str());
 x1=StrToFloat(Edit3->Text);
 y1=StrToFloat(Edit4->Text);
 z1=StrToFloat(Edit5->Text);
 Form1->secthea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm12::ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit1->Text=FloatToStr(ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.);
}
//---------------------------------------------------------------------------
void __fastcall TForm12::ScrollBar2Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit2->Text=FloatToStr(ac1+(ac2-ac1)*float(ScrollBar2->Position)/100.);
}
//---------------------------------------------------------------------------
void TForm12::setAngle0(float s)
{int nperiod=0;float BB=0.;nperiod=int((s+180.-1.)/360.);BB=s-360.*float(nperiod);
 Edit1->Text=FloatToStr(BB);ScrollBar1->Position=int(100.*(BB-ac1)/(ac2-ac1)+0.5);
}
//---------------------------------------------------------------------------
void TForm12::setAngle1(float s)
{int nperiod=0;float BB=0.;nperiod=int((s+180.-1.)/360.);BB=s-360.*float(nperiod);
 Edit2->Text=FloatToStr(BB);ScrollBar2->Position=int(100.*(BB-ac1)/(ac2-ac1)+0.5);
}
//---------------------------------------------------------------------------
void TForm12::setSection0(int s){sect_ic=s;}
void TForm12::setBngle1(float s){Edit3->Text=FloatToStr(s);}
void TForm12::setBngle2(float s){Edit4->Text=FloatToStr(s);}
void TForm12::setBngle3(float s){Edit5->Text=FloatToStr(s);}
float TForm12::getAngle0(){return ang0;}
float TForm12::getAngle1(){return ang1;}
int TForm12::getSection0(){return sect_ic;}
float TForm12::getBngle1(){return x1;}
float TForm12::getBngle2(){return y1;}
float TForm12::getBngle3(){return z1;}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button2Click(TObject *Sender){Form1->secthea3_public();}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button3Click(TObject *Sender){Form1->secthea1_public();}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button4Click(TObject *Sender){if(sect_ic)sect_ic=sect_ic+100;else sect_ic=sect_ic-100;Form1->secthea2_public();}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button5Click(TObject *Sender){if(sect_ic)sect_ic=sect_ic+10;else sect_ic=sect_ic-10;Form1->secthea2_public();}
//---------------------------------------------------------------------------
void __fastcall TForm12::RadioButton1Click(TObject *Sender){sect_ic=abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm12::RadioButton2Click(TObject *Sender){sect_ic= -abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm12::Button6Click(TObject *Sender)
{Close();Form1->Sect1p2rTRASH_public();
}
//---------------------------------------------------------------------------

