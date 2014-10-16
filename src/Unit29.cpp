//---------------------------------------------------------------------------
// New CTSP output dialog, including CTSP sub-task  EFP 10/26/2011
#include <vcl.h>
#pragma hdrstop

#include "Unit29.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm29 *Form29;
//Frantic warning: max_core=999 is hardwired below  EFP 5/01/2013
//---------------------------------------------------------------------------
__fastcall TForm29::TForm29(int isel,long nWeldPass,TComponent* Owner) : TForm(Owner)
{loc_isel=isel;loc_pass=nWeldPass;
}
//---------------------------------------------------------------------------
//String TForm29::getEdit1(){return Edit1->Text;}
String TForm29::getEdit1(){return Edit1->Text.w_str();}// EFP 2/27/2012
String TForm29::getEdit2(){return Edit2->Text;}
String TForm29::getEdit3(){return Edit3->Text;}
String TForm29::getEdit5(){return Edit5->Text;}
int TForm29::getEdit4(){return StrToInt(Edit4->Text);}
float TForm29::getEdit6(){return StrToFloat(Edit6->Text);}
//void TForm29::setEdit1(String s){Edit1->Text=s;}
void TForm29::setEdit1(String s){Edit1->Text=s.c_str();}// EFP 2/27/2012
void TForm29::setEdit2(String s){Edit2->Text=s;}
void TForm29::setEdit3(String s){Edit3->Text=s;}
void TForm29::setEdit5(String s){Edit5->Text=s;}
void TForm29::setEdit4(int s){Edit4->Text=IntToStr(s);}
void TForm29::setEdit6(float s){Edit6->Text=FloatToStr(s);}
//---------------------------------------------------------------------------
int TForm29::getIsel(){return loc_isel;}
void TForm29::setIsel(int s){loc_isel=s;}
//---------------------------------------------------------------------------
void __fastcall TForm29::Button1Click(TObject *Sender)
{int isw=1,jsw=1;long max_core=999,mval=0;
 if(loc_isel){Form1->exportCTSP2_public();
			  Close(); //Emergency Close() EFP 4/19/2010
			 }
 else {try {StrToInt(Edit4->Text);}
	   catch (EConvertError &E){isw=0;extern PACKAGE void __fastcall Beep(void);
								ShowMessage(Label4->Caption +" "+ Edit4->Text + " must be integer");
							   }
	   try {StrToFloat(Edit6->Text);}
	   catch (EConvertError &E){jsw=0;extern PACKAGE void __fastcall Beep(void);
								ShowMessage(Label10->Caption +" "+ Edit6->Text + " must be float");
							   }
	   if(isw & jsw){if(loc_pass<max_core)mval=loc_pass;else mval=max_core;
//					 if(StrToInt(Edit4->Text)>0 && StrToInt(Edit4->Text)<loc_pass+1)
					 if(StrToInt(Edit4->Text)>0 && StrToInt(Edit4->Text)<=mval)Form1->exportCTSP3_public();
					 else ShowMessage("Multi-core value must be in range 1 to "+IntToStr(__int64(mval)));
			        }
	  }
}
//---------------------------------------------------------------------------
void __fastcall TForm29::Button2Click(TObject *Sender){Close();}
//---------------------------------------------------------------------------

