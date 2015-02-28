//---------------------------------------------------------------------------
// A generalized version of Form8  EFP 9/02/2011
// Change long or float data
#include <vcl.h>
#pragma hdrstop

#include "Unit28.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm28 *Form28;
// Usage:
// imode=0 (modal) & 1 (modeless)
// fORint=0 (long) & 1 (float)
// If long,  vlmin<vl0<vlmax
// If float, vfmin<vf0<vfmax
// Selector isel

//---------------------------------------------------------------------------
__fastcall TForm28::TForm28(int isel,int imode,int fORint,
	long vl0,long vlmin,long vlmax,
	float vf0,float vfmin,float vfmax,TComponent* Owner) : TForm(Owner)
{F28_isel=isel;F28_imode=imode;F28_fORint=fORint;
 if(F28_fORint){freset=vf0;fmin=vfmin;fmax=vfmax;
				setFval(vf0);
			   }
 else {lreset=vl0;lmin=vlmin;lmax=vlmax;
	   setLval(vl0);
	  }
// if(!F28_imode){Button3->Enabled=false;Button3->Visible=false;}
}
//---------------------------------------------------------------------------
void TForm28::setFval(float s){Edit1->Text=FloatToStr(s);}
float TForm28::getFval(){return fc1;}
//---------------------------------------------------------------------------
void TForm28::setLval(long s){Edit1->Text=IntToStr(__int64(s));}
long TForm28::getLval(){return lc1;}
//---------------------------------------------------------------------------
int TForm28::getISEL(){return F28_isel;}
int TForm28::getMODE(){return F28_imode;}
int TForm28::getForI(){return F28_fORint;}
//---------------------------------------------------------------------------
void __fastcall TForm28::Button1Click(TObject *Sender)//Enter
{int isw=1;

 if(F28_fORint){try {StrToFloat(Edit1->Text);
					 if(Edit1->Text==L"0"){if(0.f<fmin || 0.f>fmax)
					   {isw=0;extern PACKAGE void __fastcall Beep(void);
						ShowMessage(Edit1->Text+L" outside limits "+FloatToStr(fmin)+L", "+FloatToStr(fmax));
					   }
										  else {lc1=0;fc1=0.f;}
										 }
					 else if(StrToFloat(Edit1->Text)<fmin || StrToFloat(Edit1->Text)>fmax)
					   {isw=0;extern PACKAGE void __fastcall Beep(void);
						ShowMessage(Edit1->Text+L" outside limits "+FloatToStr(fmin)+L", "+FloatToStr(fmax));
					   }
					 else {lc1=0;fc1=StrToFloat(Edit1->Text);}
					}
				catch (EConvertError &E){isw=0;extern PACKAGE void __fastcall Beep(void);
										 ShowMessage(Edit1->Text + L" must be floating-point");
										}
			   }
 else {try {StrToInt(Edit1->Text);
			if(Edit1->Text==L"0"){if(0<lmin || 0>lmax)
			  {isw=0;extern PACKAGE void __fastcall Beep(void);
			   ShowMessage(Edit1->Text+L" outside limits "+IntToStr(__int64(lmin))+L", "+IntToStr(__int64(lmax)));
			  }
			                     else {lc1=0;fc1=0.f;}
								}
			else if(StrToInt(Edit1->Text)<lmin || StrToInt(Edit1->Text)>lmax)
			  {isw=0;extern PACKAGE void __fastcall Beep(void);
			   ShowMessage(Edit1->Text+L" outside limits "+IntToStr(__int64(lmin))+L", "+IntToStr(__int64(lmax)));
			  }
			else {lc1=StrToInt(Edit1->Text);fc1=0.f;}
		   }
	   catch (EConvertError &E){isw=0;extern PACKAGE void __fastcall Beep(void);
								ShowMessage(Edit1->Text + L" must be integer");
							   }
	  }
 if(isw)Form1->Form28_public();

// Form1->Form28_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm28::Button2Click(TObject *Sender)//Reset
{if(F28_fORint)setFval(freset);
 else setLval(lreset);
}
//---------------------------------------------------------------------------
void __fastcall TForm28::Button3Click(TObject *Sender)//Close if modeless
{
//Close();
Form1->Form28Close_public();
}
//---------------------------------------------------------------------------
