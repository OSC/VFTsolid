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
double TForm22::getEdit2(){
//						   return StrToFloat(Edit2->Text);
 UnicodeString mess1=L"Zero weld current ",mess2=L"Neg/zero weld current ",mess3=L"Inadmissable weld current ";
 try {StrToFloat(Edit2->Text);
	  if(Edit2->Text=="0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess1+Edit2->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit2->Text)>0.000001f)return StrToFloat(Edit2->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess2+Edit2->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess3+Edit2->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit3(){
//						   return StrToFloat(Edit3->Text);
 UnicodeString mess1=L"Zero weld voltage ",mess2=L"Neg/zero weld voltage ",mess3=L"Inadmissable weld voltage ";
 try {StrToFloat(Edit3->Text);
	  if(Edit3->Text=="0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess1+Edit3->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit3->Text)>0.000001f)return StrToFloat(Edit3->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess2+Edit3->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess3+Edit3->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit4(){
//						   return StrToFloat(Edit4->Text);
 UnicodeString mess1=L"Zero arc efficiency ",mess2=L"Neg/zero arc efficiency ",mess3=L"Inadmissable arc efficiency ";
 try {StrToFloat(Edit4->Text);
	  if(Edit4->Text=="0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess1+Edit4->Text).w_str(),L"Reenter <=1",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit4->Text)>0.000001f && StrToFloat(Edit4->Text)<=1.f)return StrToFloat(Edit4->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess2+Edit4->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess3+Edit4->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm22::getEdit5(){
//						   return StrToFloat(Edit5->Text);
 UnicodeString mess1=L"Zero torch speed ",mess2=L"Neg/zero torch speed ",mess3=L"Inadmissable torch speed ";
 try {StrToFloat(Edit5->Text);
	  if(Edit5->Text=="0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess1+Edit5->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit5->Text)>0.000001f)return StrToFloat(Edit5->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess2+Edit5->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess3+Edit5->Text).w_str(),L"Reenter +float",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
void TForm22::setEdit2(double x){Edit2->Text=FloatToStr(x);}
void TForm22::setEdit3(double x){Edit3->Text=FloatToStr(x);}
void TForm22::setEdit4(double x){Edit4->Text=FloatToStr(x);}
void TForm22::setEdit5(double x){Edit5->Text=FloatToStr(x);}
//---------------------------------------------------------------------------
void __fastcall TForm22::Button1Click(TObject *Sender)
{int isw=1;float TOL=0.000001f;
 UnicodeString mess1=L"Zero weld current ",mess2=L"Neg/zero weld current ",mess3=L"Zero weld voltage ",mess4=L"Neg/zero weld voltage ",
			   mess5=L"Zero arc efficiency ",mess6=L"Neg/zero arc efficiency ",mess7=L"Zero torch speed ",mess8=L"Neg/zero torch speed ";
 try {StrToFloat(Edit2->Text);
	  if(Edit2->Text=="0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess1+Edit2->Text).w_str(),L"Reenter +float",MB_OK);}
	  else if(StrToFloat(Edit2->Text)<=TOL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess2+Edit2->Text).w_str(),L"Reenter +float",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label2->Caption +L" "+ Edit2->Text + L" must be positive floating-point");
						  }
 try {StrToFloat(Edit3->Text);
	  if(Edit3->Text=="0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess3+Edit3->Text).w_str(),L"Reenter +float",MB_OK);}
	  else if(StrToFloat(Edit3->Text)<=TOL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess4+Edit3->Text).w_str(),L"Reenter +float",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label3->Caption +L" "+ Edit3->Text + L" must be positive floating-point");
						  }
 try {StrToFloat(Edit4->Text);
	  if(Edit4->Text=="0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess5+Edit4->Text).w_str(),L"Reenter 0-to-1",MB_OK);}
	  else if(StrToFloat(Edit4->Text)<=TOL || StrToFloat(Edit4->Text)>1.f){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess6+Edit4->Text).w_str(),L"Reenter 0-to-1",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label4->Caption +L" "+ Edit4->Text + L" must be positive floating-point 0-to-1");
						  }
 try {StrToFloat(Edit5->Text);
	  if(Edit5->Text=="0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess7+Edit5->Text).w_str(),L"Reenter +float",MB_OK);}
	  else if(StrToFloat(Edit5->Text)<=TOL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox((mess8+Edit5->Text).w_str(),L"Reenter +float",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label5->Caption +L" "+ Edit5->Text + L" must be positive floating-point");
						  }
//////////////////// Special-purpose coding for VFT only  EFP 8/22/2010
// if(StrToFloat(Edit2->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								 ShowMessage(Label2->Caption +" "+ Edit2->Text + " must be positive");
//								}
// if(StrToFloat(Edit3->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								 ShowMessage(Label3->Caption +" "+ Edit3->Text + " must be positive");
//								}
// if(StrToFloat(Edit4->Text)<TOL || StrToFloat(Edit4->Text)>1.)
//								{isw=0;extern PACKAGE void __fastcall Beep(void);
//								 ShowMessage(Label4->Caption +" "+ Edit4->Text + " must range 0-to-1");
//								}
// if(StrToFloat(Edit5->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								 ShowMessage(Label5->Caption +" "+ Edit5->Text + " must be positive");
//								}
/////////////////////////////////////////////////////////
// if(isw){if(F22_sel)Form1->EditWeldParam1_public();
//		 else {Form1->WeldParam_public();
////			   Close();
//			  }
//		}
//***********************
 int buffersize=WideCharToMultiByte(CP_UTF8,0,Edit1->Text.w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,Edit1->Text.w_str(), -1,m,buffersize,NULL,NULL);
 if(strchr(m,' ') != NULL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit1->Text.w_str(),L"Repeat: inadmissible space\n in WeldParam name",MB_OK);}
 delete m;m=NULL;
//***********************
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

