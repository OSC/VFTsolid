//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit9.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm9 *Form9;
//---------------------------------------------------------------------------
__fastcall TForm9::TForm9
   (int sectionsel0,float Angle0,float Angle1,float Angle2,
    float Bngle0,float Bngle1,float Bngle2,float Cngle0,float Cngle1,float Cngle2,
    TComponent* Owner) : TForm(Owner)
{ScrollBar1->Max=100;ScrollBar1->Min=0;ScrollBar1->LargeChange=10;ScrollBar1->SmallChange=1;
 ScrollBar2->Max=100;ScrollBar2->Min=0;ScrollBar2->LargeChange=10;ScrollBar2->SmallChange=1;
 ScrollBar3->Max=100;ScrollBar3->Min=0;ScrollBar3->LargeChange=10;ScrollBar3->SmallChange=1;
 sect_ic=sectionsel0;
 ac1=Angle1;ac2=Angle2;setAngle0(Angle0);
 bc1=Bngle1;bc2=Bngle2;setBngle0(Bngle0);
 cc1=Cngle1;cc2=Cngle2;setCngle0(Cngle0);
}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button1Click(TObject *Sender)
{float AA=0.,VV=0.;
 if(Edit1->Text=="0") //Necessary since Edit ignores 0
   {if(ac2>0. && ac1<0.)
      {if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
       ScrollBar1->Position=int( -100.*ac1/VV+0.5);
      }
    else ScrollBar1->Position=0;
   }
 else
//   {AA=atof(Edit1->Text.c_str());
   {AA=StrToFloat(Edit1->Text);
	if(AA>ac2)AA=ac2;else if(AA<ac1)AA=ac1;
    if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
    ScrollBar1->Position=int(100.*(AA-ac1)/VV+0.5);
   }
 if(Edit2->Text=="0") //Necessary since Edit ignores 0
   {if(bc2>0. && bc1<0.)
      {if(bc2-bc1> 0.0001)VV=bc2-bc1;else VV=0.0001;
       ScrollBar2->Position=int( -100.*bc1/VV+0.5);
      }
    else ScrollBar2->Position=0;
   }
 else
//   {AA=atof(Edit2->Text.c_str());
   {AA=StrToFloat(Edit2->Text);
	if(AA>bc2)AA=bc2;else if(AA<bc1)AA=bc1;
    if(bc2-bc1> 0.0001)VV=bc2-bc1;else VV=0.0001;
    ScrollBar2->Position=int(100.*(AA-bc1)/VV+0.5);
   }
 if(Edit3->Text=="0") //Necessary since Edit ignores 0
   {if(cc2>0. && cc1<0.)
      {if(cc2-cc1> 0.0001)VV=cc2-cc1;else VV=0.0001;
       ScrollBar3->Position=int( -100.*cc1/VV+0.5);
      }
    else ScrollBar3->Position=0;
   }
 else
//   {AA=atof(Edit3->Text.c_str());
   {AA=StrToFloat(Edit3->Text);
	if(AA>cc2)AA=cc2;else if(AA<cc1)AA=cc1;
	if(cc2-cc1> 0.0001)VV=cc2-cc1;else VV=0.0001;
	ScrollBar3->Position=int(100.*(AA-cc1)/VV+0.5);
   }
 Form1->secthea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm9::ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit1->Text=FloatToStr(ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.);
}
//---------------------------------------------------------------------------
void __fastcall TForm9::ScrollBar2Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit2->Text=FloatToStr(bc1+(bc2-bc1)*float(ScrollBar2->Position)/100.);
}
//---------------------------------------------------------------------------
void __fastcall TForm9::ScrollBar3Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit3->Text=FloatToStr(cc1+(cc2-cc1)*float(ScrollBar3->Position)/100.);
}
//---------------------------------------------------------------------------
void TForm9::setAngle0(float s)
{float VV=0.,BB=0.;
 if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
 if(s>ac2)BB=ac2;else if(s<ac1)BB=ac1;else BB=s;
 Edit1->Text=FloatToStr(BB);ScrollBar1->Position=int(100.*(BB-ac1)/VV+0.5);
}
//---------------------------------------------------------------------------
void TForm9::setBngle0(float s)
{float VV=0.,BB=0.;
 if(bc2-bc1> 0.0001)VV=bc2-bc1;else VV=0.0001;
 if(s>bc2)BB=bc2;else if(s<bc1)BB=bc1;else BB=s;
 Edit2->Text=FloatToStr(BB);ScrollBar2->Position=int(100.*(BB-bc1)/VV+0.5);
}
//---------------------------------------------------------------------------
void TForm9::setCngle0(float s)
{float VV=0.,BB=0.;
 if(cc2-cc1> 0.0001)VV=cc2-cc1;else VV=0.0001;
 if(s>cc2)BB=cc2;else if(s<cc1)BB=cc1;else BB=s;
 Edit3->Text=FloatToStr(BB);ScrollBar3->Position=int(100.*(BB-cc1)/VV+0.5);
}
//---------------------------------------------------------------------------
void TForm9::setAngle1(float s){}
void TForm9::setAngle2(float s){}
void TForm9::setBngle1(float s){}
void TForm9::setBngle2(float s){}
void TForm9::setCngle1(float s){}
void TForm9::setCngle2(float s){}
void TForm9::setSection0(int s){sect_ic=s;}
//---------------------------------------------------------------------------
float TForm9::getAngle0(){return ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.;}
float TForm9::getAngle1(){return 0.;}
float TForm9::getAngle2(){return 0.;}
float TForm9::getBngle0(){return bc1+(bc2-bc1)*float(ScrollBar2->Position)/100.;}
float TForm9::getBngle1(){return 0.;}
float TForm9::getBngle2(){return 0.;}
float TForm9::getCngle0(){return cc1+(cc2-cc1)*float(ScrollBar3->Position)/100.;}
float TForm9::getCngle1(){return 0.;}
float TForm9::getCngle2(){return 0.;}
int TForm9::getSection0(){return sect_ic;}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button3Click(TObject *Sender){Form1->secthea1_public();}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button2Click(TObject *Sender){Form1->secthea3_public();}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button4Click(TObject *Sender){if(sect_ic)sect_ic= sect_ic+10;else sect_ic= sect_ic-10;Form1->secthea2_public();}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button5Click(TObject *Sender){if(sect_ic)sect_ic=sect_ic+100;else sect_ic=sect_ic-100;Form1->secthea2_public();}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button6Click(TObject *Sender)
{Close();Form1->SectWedgeTRASH_public();
}
//---------------------------------------------------------------------------


