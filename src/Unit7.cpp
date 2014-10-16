//---------------------------------------------------------------------------
// Revision
//  2003/11/19 Button5&6 replaced with RadioButton1&2 (but Button5&6 coding not removable)
#include <vcl.h>
#pragma hdrstop
#include <stdlib.h>
#include <math.h>
#include "Unit7.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7
 (int sectionsel0,float Angle0,float Angle1,float Angle2,TComponent* Owner) : TForm(Owner)
{ScrollBar1->Max=100;ScrollBar1->Min=0;ScrollBar1->LargeChange=10;ScrollBar1->SmallChange=1;
 sect_ic=sectionsel0;ac1=Angle1;ac2=Angle2;setAngle0(Angle0);
 if(sect_ic)RadioButton1->Checked=true;else RadioButton2->Checked=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button1Click(TObject *Sender)
{float AA=0.,VV=0.;
 if(Edit1->Text=="0")
//Necessary since Edit ignores 0
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
 Form1->secthea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{Edit1->Text=FloatToStr(ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.);
}
//---------------------------------------------------------------------------
void TForm7::setAngle0(float s)
{float VV=0.,BB=0.;
 if(ac2-ac1> 0.0001)VV=ac2-ac1;else VV=0.0001;
 if(s>ac2)BB=ac2;else if(s<ac1)BB=ac1;else BB=s;
 Edit1->Text=FloatToStr(BB);ScrollBar1->Position=int(100.*(BB-ac1)/VV+0.5);
}
//---------------------------------------------------------------------------
void TForm7::setAngle1(float s){}
void TForm7::setAngle2(float s){}
void TForm7::setSection0(int s){sect_ic=s;}
//---------------------------------------------------------------------------
float TForm7::getAngle0(){return ac1+(ac2-ac1)*float(ScrollBar1->Position)/100.;}
float TForm7::getAngle1(){return 0.;}
float TForm7::getAngle2(){return 0.;}
int TForm7::getSection0(){return sect_ic;}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button3Click(TObject *Sender){Form1->secthea1_public();}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button7Click(TObject *Sender)
{if(sect_ic)sect_ic=sect_ic+10;else sect_ic=sect_ic-10;Form1->secthea2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button4Click(TObject *Sender)
{if(sect_ic)sect_ic=sect_ic+100;else sect_ic=sect_ic-100;Form1->secthea2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button2Click(TObject *Sender){Form1->secthea3_public();}
//---------------------------------------------------------------------------
void __fastcall TForm7::RadioButton1Click(TObject *Sender){sect_ic=abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm7::RadioButton2Click(TObject *Sender){sect_ic= -abs(sect_ic);}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button5Click(TObject *Sender)
{Close();Form1->sectheaTRASH_public();
}
//---------------------------------------------------------------------------

