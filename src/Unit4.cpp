// Rotation Dialog ---------------------------------------
// 2002/08/06 EFP Button1Click/etc: Edit1->Text.c_str() can not accomodate 0
#include <vcl.h>
#pragma hdrstop
#include <stdlib.h>
#include <math.h>
#include "Unit4.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(float A1,float A2,float A3,TComponent* Owner) : TForm(Owner)
{ScrollBar1->Max=180;ScrollBar1->Min= -180;ScrollBar1->LargeChange=1;ScrollBar1->SmallChange=1;
 ScrollBar2->Max=180;ScrollBar2->Min= -180;ScrollBar2->LargeChange=1;ScrollBar2->SmallChange=1;
 ScrollBar3->Max=180;ScrollBar3->Min= -180;ScrollBar3->LargeChange=1;ScrollBar3->SmallChange=1;
 setAngle0(A1);setAngle1(A2);setAngle2(A3);AA0=AA1=AA2=0.;
}
////---------------------------------------------------------------------------
//void __fastcall TForm4::Button1Click(TObject *Sender)
//{char buf[5];float AA=0.,VV=0.,BB=0.;int nper=0;
// if(Edit1->Text=="0")ScrollBar1->Position=0;
// else if(atoi(Edit1->Text.c_str()))
//   {AA=atof(Edit1->Text.c_str());VV=(AA+180.)/360.;
//    if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
//    ScrollBar1->Position=int(BB+0.5)-180;
//   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox("Integer X-degrees only.","Try again.",MB_OK);
//       Edit1->Text=itoa(ScrollBar1->Position,buf,10);
//      }
////
// if(Edit2->Text=="0")ScrollBar2->Position=0;
// else if(atoi(Edit2->Text.c_str()))
//   {AA=atof(Edit2->Text.c_str());VV=(AA+180.)/360.;
//    if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
//    ScrollBar2->Position=int(BB+0.5)-180;
//   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox("Integer Y-degrees only.","Try again.",MB_OK);
//       Edit2->Text=itoa(ScrollBar2->Position,buf,10);
//      }
////
// if(Edit3->Text=="0")ScrollBar3->Position=0;
// else if(atoi(Edit3->Text.c_str()))
//   {AA=atof(Edit3->Text.c_str());VV=(AA+180.)/360.;
//    if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
//    ScrollBar3->Position=int(BB+0.5)-180;
//   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox("Integer Z-degrees only.","Try again.",MB_OK);
//       Edit3->Text=itoa(ScrollBar3->Position,buf,10);
//      }
////
// Angle0=float(ScrollBar1->Position);Angle1=float(ScrollBar2->Position);Angle2=float(ScrollBar3->Position);Form1->rothea_public();
//}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender) // TBD: Untested input
{char buf[5];float AA=0.,VV=0.,BB=0.;int nper=0;
 if(Edit1->Text=="0"){ScrollBar1->Position=0;AA0=0.;}
 else {//AA=atof(Edit1->Text.c_str());
	   AA=StrToFloat(Edit1->Text);
	   VV=(AA+180.)/360.;
       if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
       ScrollBar1->Position=int(BB+0.5)-180;AA0=AA;
      }
//
 if(Edit2->Text=="0"){ScrollBar2->Position=0;AA1=0.;}
 else {//AA=atof(Edit2->Text.c_str());
	   AA=StrToFloat(Edit2->Text);
	   VV=(AA+180.)/360.;
	   if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
	   ScrollBar2->Position=int(BB+0.5)-180;AA1=AA;
	  }
//
 if(Edit3->Text=="0"){ScrollBar3->Position=0;AA2=0.;}
 else {//AA=atof(Edit3->Text.c_str());
	   AA=StrToFloat(Edit3->Text);
	   VV=(AA+180.)/360.;
	   if(VV<0.){nper=int(VV+0.001);BB=AA+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=AA+180.-360.*float(nper);}
	   ScrollBar3->Position=int(BB+0.5)-180;AA2=AA;
	  }
 Form1->rothea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender) // NOT working
//{Angle0= float(ScrollBar1->Position);Angle1= float(ScrollBar2->Position);
// Angle2= float(ScrollBar3->Position);Form1->rothea1_public();
{AA0= float(ScrollBar1->Position);AA1= float(ScrollBar2->Position);
 AA2= float(ScrollBar3->Position);Form1->rothea1_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button3Click(TObject *Sender) // NOT working
{float VV=0.;setAngle0(VV);setAngle1(VV);setAngle2(VV);Form1->rothea_reset_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button4Click(TObject *Sender)
{float convent=3.4e38;AA0=AA1=AA2=convent;Close();Form1->rothea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{char buf[5];Edit1->Text=itoa(ScrollBar1->Position,buf,10);
}
//---------------------------------------------------------------------------
void __fastcall TForm4::ScrollBar2Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{char buf[5];Edit2->Text=itoa(ScrollBar2->Position,buf,10);
}
//---------------------------------------------------------------------------
void __fastcall TForm4::ScrollBar3Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos)
{char buf[5];Edit3->Text=itoa(ScrollBar3->Position,buf,10);
}
//---------------------------------------------------------------------------
void TForm4::setAngle0(float s)
{float VV=0.,BB=0.;int is=0,nper=0;VV=(s+180.)/360.;
 if(VV<0.){nper=int(VV+0.001);BB=s+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=s+180.-360.*float(nper);}
// is=int(BB+0.5)-180;Edit1->Text=IntToStr(is);ScrollBar1->Position=is;
 is=int(BB+0.5)-180;Edit1->Text=FloatToStr(s);ScrollBar1->Position=is;
}
//---------------------------------------------------------------------------
void TForm4::setAngle1(float s)
{float VV=0.,BB=0.;int is=0,nper=0;VV=(s+180.)/360.;
 if(VV<0.){nper=int(VV+0.001);BB=s+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=s+180.-360.*float(nper);}
// is=int(BB+0.5)-180;Edit2->Text=IntToStr(is);ScrollBar2->Position=is;
 is=int(BB+0.5)-180;Edit2->Text=FloatToStr(s);ScrollBar2->Position=is;
}
//---------------------------------------------------------------------------
void TForm4::setAngle2(float s)
{float VV=0.,BB=0.;int is=0,nper=0;VV=(s+180.)/360.;
 if(VV<0.){nper=int(VV+0.001);BB=s+540.+360.*float(nper);}else {nper=int(VV-0.001);BB=s+180.-360.*float(nper);}
// is=int(BB+0.5)-180;Edit3->Text=IntToStr(is);ScrollBar3->Position=is;
 is=int(BB+0.5)-180;Edit3->Text=FloatToStr(s);ScrollBar3->Position=is;
}
//---------------------------------------------------------------------------
//float TForm4::getAngle0(){return float(ScrollBar1->Position);}
//float TForm4::getAngle1(){return float(ScrollBar2->Position);}
//float TForm4::getAngle2(){return float(ScrollBar3->Position);}
float TForm4::getAngle0(){return AA0;}
float TForm4::getAngle1(){return AA1;}
float TForm4::getAngle2(){return AA2;}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button5Click(TObject *Sender){Form1->XYrot_public();}
void __fastcall TForm4::Button6Click(TObject *Sender){Form1->XYrotm_public();}
void __fastcall TForm4::Button7Click(TObject *Sender){Form1->YZrot_public();}
void __fastcall TForm4::Button8Click(TObject *Sender){Form1->YZrotm_public();}
void __fastcall TForm4::Button9Click(TObject *Sender){Form1->ZXrot_public();}
void __fastcall TForm4::Button10Click(TObject *Sender){Form1->ZXrotm_public();}
//---------------------------------------------------------------------------

