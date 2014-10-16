//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit22.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm22 *Form22;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(int isel,TComponent* Owner): TForm(Owner){F22_sel=isel;}
//---------------------------------------------------------------------------
String TForm22::getEdit1(){return Edit1->Text;}
void TForm22::setEdit1(String s){Edit1->Text=s;}
//---------------------------------------------------------------------------
double TForm22::getEdit2(){return StrToFloat(Edit2->Text);}
double TForm22::getEdit3(){return StrToFloat(Edit3->Text);}
double TForm22::getEdit4(){return StrToFloat(Edit4->Text);}
double TForm22::getEdit5(){return StrToFloat(Edit5->Text);}
//---------------------------------------------------------------------------
void TForm22::setEdit2(double x){Edit2->Text=FloatToStr(x);}
void TForm22::setEdit3(double x){Edit3->Text=FloatToStr(x);}
void TForm22::setEdit4(double x){Edit4->Text=FloatToStr(x);}
void TForm22::setEdit5(double x){Edit5->Text=FloatToStr(x);}
//---------------------------------------------------------------------------
void __fastcall TForm22::Button1Click(TObject *Sender)
{int isw=1;float TOL=0.000001;
 try {StrToFloat(Edit2->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label2->Caption +" "+ Edit2->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit3->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label3->Caption +" "+ Edit3->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit4->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label4->Caption +" "+ Edit4->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit5->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label5->Caption +" "+ Edit5->Text + " must be floating-point");
						  }
//////////////////// Special-purpose coding for VFT only  EFP 8/22/2010
 if(StrToFloat(Edit2->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label2->Caption +" "+ Edit2->Text + " must be positive");
								}
 if(StrToFloat(Edit3->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label3->Caption +" "+ Edit3->Text + " must be positive");
								}
 if(StrToFloat(Edit4->Text)<TOL || StrToFloat(Edit4->Text)>1.)
								{isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label4->Caption +" "+ Edit4->Text + " must range 0-to-1");
								}
 if(StrToFloat(Edit5->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label5->Caption +" "+ Edit5->Text + " must be positive");
								}
/////////////////////////////////////////////////////////
// if(isw){if(F22_sel)Form1->EditWeldParam1_public();
//		 else {Form1->WeldParam_public();
////			   Close();
//			  }
//		}
 if(isw)Form1->WeldParam_public();
}
//---------------------------------------------------------------------------
//void __fastcall TForm22::Button2Click(TObject *Sender){if(F22_sel)Form1->EditWeldParam2_public();
////													   else Close();
//													  }
void __fastcall TForm22::Button2Click(TObject *Sender){Form1->EditWeldParam_public();}
//---------------------------------------------------------------------------
int TForm22::getISEL(){return F22_sel;}
//---------------------------------------------------------------------------

