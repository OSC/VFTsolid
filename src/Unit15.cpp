//---------------------------------------------------------------------------
// Note: To add a component to a tab, drag the component from Tool Palette/etc
//       and place it on the tab. Unfortunately, it will not appear "on the tab"
//       in Structure (it will be on PageControlx), but it can dragged up
//       to the tab by Drag/Click on (e.g.) TTabSheet1.
#include <vcl.h>
#pragma hdrstop
#include "Unit15.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm15 *Form15;
int iflagForm15=0;
//TColor color_f15=clBlue;
//TColor color_f15;
//---------------------------------------------------------------------------
__fastcall TForm15::TForm15(int isel,long currWeldPass,long currSeq,long nStartElem,
							long endElem,long Pl1normEl,long Pl2normEl,
							long nMatPropSet,String gMatPropName[],
							long nWeldParSet,String gWeldParName[],
   TComponent* Owner) : TForm(Owner) // All arguments start with 1 (not 0)
{int i=0;F15_isel=isel;Pl1norm=Pl1normEl;Pl2norm=Pl2normEl;
 kflagForm15=jflagForm15= -1;nMatPS=nMatPropSet;nWeldPS=nWeldParSet;
// Edit1->Text=IntToStr(INT64(currWeldPass));
 Edit2->Text=IntToStr(__int64(currSeq));
// if(nStartElem)Edit3->Text=IntToStr(nStartElem);else Edit3->Text="***";
// if(endElem)Edit4->Text=IntToStr(endElem);else Edit4->Text="***";
// if(Pl1normEl/10)Edit5->Text=IntToStr(Pl1normEl/10);else Edit5->Text="***";
// if(Pl2normEl/10)Edit6->Text=IntToStr(Pl2normEl/10);else Edit6->Text="***";
// if(Pl1normEl-10*(Pl1normEl/10))Edit7->Text=IntToStr(Pl1normEl-10*(Pl1normEl/10));else Edit7->Text="***";
// if(Pl2normEl-10*(Pl2normEl/10))Edit8->Text=IntToStr(Pl2normEl-10*(Pl2normEl/10));else Edit8->Text="***";
 RadioGroup2->ItemIndex=0;
 CheckBox1->Checked=true;
 RadioButton5->Checked=true;RadioButton6->Checked=false;//MovingArc is default
//// TListBox *ListBox1= new TListBox(this);
// for(i=0;i<nMatPS;i++)ListBox1->Items->Add(gMatPropName[i].c_str());
// for(i=0;i<nWeldPS;i++)ListBox2->Items->Add(gWeldParName[i].c_str());
 for(i=0;i<nMatPS;i++)ListBox1->Items->Add(gMatPropName[i].w_str());
 for(i=0;i<nWeldPS;i++)ListBox2->Items->Add(gWeldParName[i].w_str());
 }
//---------------------------------------------------------------------------
int TForm15::getRadioBstatus(){
							   if(RadioGroup2->ItemIndex==0)iflagForm15=1;
							   else if(RadioGroup2->ItemIndex==1)iflagForm15=2;
							   else if(RadioGroup2->ItemIndex==2)iflagForm15=3;
							   else if(RadioGroup2->ItemIndex==3)iflagForm15=4;
							   else iflagForm15=5;
							   return iflagForm15;
							  }
//---------------------------------------------------------------------------
int TForm15::getCheckBstatus(){if(CheckBox1->Checked==true)return 1;else return 0;}
//---------------------------------------------------------------------------
long TForm15::getEdit2(){
 try {StrToInt(Edit2->Text);
	  if(Edit2->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit2->Text.w_str(),L"Zero pass seq.: Reenter a positive integer",MB_OK);return 0;}
	  else if(StrToInt(Edit2->Text)>0)return StrToInt(Edit2->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit2->Text.w_str(),L"Negative pass seq.: Reenter a positive integer",MB_OK);return 0;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit2->Text.w_str(),L"Inadmissable pass seq.: Reenter a positive integer",MB_OK);return 0;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit9(){
 try {StrToFloat(Edit9->Text);
	  if(Edit9->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit9->Text.w_str(),L"Zero weld current: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit9->Text)>0.000001f)return StrToFloat(Edit9->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit9->Text.w_str(),L"Negative/zero weld current: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit9->Text.w_str(),L"Inadmissable weld current: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit10(){
 try {StrToFloat(Edit10->Text);
	  if(Edit10->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit10->Text.w_str(),L"Zero weld voltage: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit10->Text)>0.000001f)return StrToFloat(Edit10->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit10->Text.w_str(),L"Negative/zero weld voltage: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit10->Text.w_str(),L"Inadmissable weld voltage: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit11(){
 try {StrToFloat(Edit11->Text);
	  if(Edit11->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit11->Text.w_str(),L"Zero arc efficiency: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit11->Text)>0.000001f && StrToFloat(Edit11->Text)<=1.f)return StrToFloat(Edit11->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit11->Text.w_str(),L"Arc efficiency out-of-range: Reenter a floating point 0-to-1",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit11->Text.w_str(),L"Inadmissable arc efficiency: Reenter a floating point 0-to-1",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit12(){
 try {StrToFloat(Edit12->Text);
	  if(Edit12->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit12->Text.w_str(),L"Zero torch speed: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit12->Text)>0.000001f)return StrToFloat(Edit12->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit12->Text.w_str(),L"Negative/zero torch speed: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit12->Text.w_str(),L"Inadmissable torch speed: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit13(){
 try {StrToFloat(Edit13->Text);
	  if(Edit13->Text==L"0")return 0.f;
	  else return StrToFloat(Edit13->Text);
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit13->Text.w_str(),L"Inadmissable room temperature: Reenter a floating point number",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit14(){
 try {StrToFloat(Edit14->Text);
	  if(Edit14->Text==L"0")return 0.f;
	  else return StrToFloat(Edit14->Text);
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit14->Text.w_str(),L"Inadmissable melting temperature: Reenter a floating point number",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit15(){
 try {StrToFloat(Edit15->Text);
	  if(Edit15->Text==L"0")return 0.f;
	  else return StrToFloat(Edit15->Text);
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit15->Text.w_str(),L"Inadmissable low-cut temperature: Reenter a floating point number",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit16(){
 try {StrToFloat(Edit16->Text);
	  if(Edit16->Text==L"0")return 0.f;
	  else return StrToFloat(Edit16->Text);
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit16->Text.w_str(),L"Inadmissable preheat temperature: Reenter a floating point number",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit17(){
 try {StrToFloat(Edit17->Text);
	  if(Edit17->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit17->Text.w_str(),L"Zero interpass cooling time: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit17->Text)>0.000001f)return StrToFloat(Edit17->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit17->Text.w_str(),L"Negative/zero interpass cooling time: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit17->Text.w_str(),L"Inadmissable interpass cooling time: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit18(){
 try {StrToFloat(Edit18->Text);
	  if(Edit18->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit18->Text.w_str(),L"Zero max heating time: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit18->Text)>0.000001f)return StrToFloat(Edit18->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit18->Text.w_str(),L"Negative/zero max heating time: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit18->Text.w_str(),L"Inadmissable max heating time: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
int TForm15::getEdit19(){
 try {StrToInt(Edit19->Text);
	  if(Edit19->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit19->Text.w_str(),L"Zero interpass cooling steps: Reenter a positive integer",MB_OK);return 0.f;}
	  else if(StrToInt(Edit19->Text)>0)return StrToInt(Edit19->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit19->Text.w_str(),L"Negative interpass cooling steps: Reenter a positive integer",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit19->Text.w_str(),L"Inadmissable interpass cooling steps: Reenter a positive integer",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit20(){
 try {StrToFloat(Edit20->Text);
	  if(Edit20->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit20->Text.w_str(),L"Zero min heating time: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit20->Text)>0.000001f)return StrToFloat(Edit20->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit20->Text.w_str(),L"Negative/zero min heating time: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit20->Text.w_str(),L"Inadmissable min heating time: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit21(){
 try {StrToFloat(Edit21->Text);
	  if(Edit21->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit21->Text.w_str(),L"Zero Plate 1 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit21->Text)>0.000001f)return StrToFloat(Edit21->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit21->Text.w_str(),L"Negative/zero Plate 1 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit21->Text.w_str(),L"Inadmissable Plate 1 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit22(){
 try {StrToFloat(Edit22->Text);
	  if(Edit22->Text==L"0"){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit22->Text.w_str(),L"Zero Plate 2 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
	  else if(StrToFloat(Edit22->Text)>0.000001f)return StrToFloat(Edit22->Text);
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit22->Text.w_str(),L"Negative/zero Plate 2 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit22->Text.w_str(),L"Inadmissable Plate 2 thickness: Reenter a positive floating point",MB_OK);return 0.f;}
						  }
//---------------------------------------------------------------------------
long TForm15::getEdit23(){
						   return StrToInt(Edit23->Text); // NOT USED
						  }
//---------------------------------------------------------------------------
long TForm15::getEdit24(){
						   return StrToInt(Edit24->Text); // NOT USED
						  }
//---------------------------------------------------------------------------
int TForm15::getISEL(){return F15_isel;}
int TForm15::getFunc(){return 4+RadioGroup1->ItemIndex;}
void TForm15::setFunc(int s){if(s==4)RadioGroup1->ItemIndex=0;
							 else if(s==5)RadioGroup1->ItemIndex=1;
							 else if(s==6)RadioGroup1->ItemIndex=2;
							 else RadioGroup1->ItemIndex=3;
							}
void TForm15::setNorm1(long v){Pl1norm=v;}
void TForm15::setNorm2(long v){Pl2norm=v;}
//---------------------------------------------------------------------------
int TForm15::getType(){return ComboBox1->ItemIndex;}
//---------------------------------------------------------------------------
int TForm15::getShape(){return ComboBox2->ItemIndex;}
int TForm15::getMatName(){return kflagForm15;}
int TForm15::getWeldParName(){return jflagForm15;}
//TColor TForm15::getWColor(){return clYellow;}
//TColor TForm15::getWColor(){return color_f15;}
TColor TForm15::getWColor(){return Shape1->Brush->Color;}
String TForm15::getEdit1(){return Edit1->Text;}
//---------------------------------------------------------------------------
//void TForm15::setEdit1(long x){Edit1->Text=IntToStr(INT64(x));}
void TForm15::setEdit1(String x){Edit1->Text=x;}
void TForm15::setEdit2(long x){Edit2->Text=IntToStr(__int64(x));}
void TForm15::setEdit9(double x){Edit9->Text=FloatToStr(x);}
void TForm15::setEdit10(double x){Edit10->Text=FloatToStr(x);}
void TForm15::setEdit11(double x){Edit11->Text=FloatToStr(x);}
void TForm15::setEdit12(double x){Edit12->Text=FloatToStr(x);}
void TForm15::setEdit13(double x){Edit13->Text=FloatToStr(x);}
void TForm15::setEdit14(double x){Edit14->Text=FloatToStr(x);}
void TForm15::setEdit15(double x){Edit15->Text=FloatToStr(x);}
void TForm15::setEdit16(double x){Edit16->Text=FloatToStr(x);}
void TForm15::setEdit17(double x){Edit17->Text=FloatToStr(x);}
void TForm15::setEdit18(double x){Edit18->Text=FloatToStr(x);}
void TForm15::setEdit19(int x){Edit19->Text=IntToStr(x);}
void TForm15::setEdit20(double x){Edit20->Text=FloatToStr(x);}
void TForm15::setEdit21(double x){Edit21->Text=FloatToStr(x);}
void TForm15::setEdit22(double x){Edit22->Text=FloatToStr(x);}
void TForm15::setEdit23(long x){Edit23->Text=IntToStr(__int64(x));}
void TForm15::setEdit24(long x){Edit24->Text=IntToStr(__int64(x));}
void TForm15::setType(int s){ComboBox1->ItemIndex=s;}
void TForm15::setShape(int s){ComboBox2->ItemIndex=s;}
void TForm15::setWColor(TColor s){Shape1->Brush->Color=s;}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button1Click(TObject *Sender)
{int isw=1,testTflag=1,heattimeflag=1;float TOL=0.000001f,roomT=0.f,meltT=0.f,lowcutT=0.f,preheatT=0.f,maxheattime=0.f,minheattime=0.f;
 try {StrToFloat(Edit9->Text);
	  if(Edit9->Text==L"0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit9->Text.w_str(),L"Zero weld current: Reenter a positive floating point",MB_OK);}
	  else if(StrToFloat(Edit9->Text)<=0.000001f){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit9->Text.w_str(),L"Negative/zero weld current: Reenter a positive floating point",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label10->Caption +" "+ Edit9->Text + " must be positive floating-point");
						  }
 try {StrToFloat(Edit10->Text);
	  if(Edit10->Text==L"0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit10->Text.w_str(),L"Zero weld voltage: Reenter a positive floating point",MB_OK);}
	  else if(StrToFloat(Edit10->Text)<=0.000001f){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit10->Text.w_str(),L"Negative/zero weld voltage: Reenter a positive floating point",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label11->Caption +" "+ Edit10->Text + " must be positive floating-point");
						  }
 try {StrToFloat(Edit11->Text);
	  if(Edit11->Text==L"0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit11->Text.w_str(),L"Zero arc efficiency: Reenter a positive floating point",MB_OK);}
	  else if(StrToFloat(Edit11->Text)<=0.000001f || StrToFloat(Edit11->Text)>1.f){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit11->Text.w_str(),L"Arc efficiency out-of-range: Reenter a floating point 0-to-1",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label12->Caption +" "+ Edit11->Text + " must be 0-to-1 floating-point");
						  }
 try {StrToFloat(Edit12->Text);
	  if(Edit12->Text==L"0"){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit12->Text.w_str(),L"Zero torch speed: Reenter a positive floating point",MB_OK);}
	  else if(StrToFloat(Edit12->Text)<=0.000001f){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit12->Text.w_str(),L"Negative/zero torch speed: Reenter a positive floating point",MB_OK);}
	 }
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label13->Caption +" "+ Edit12->Text + " must be positive floating-point");
						  }

 try {StrToFloat(Edit13->Text);
	  if(Edit13->Text==L"0")roomT=0.f;
	  else roomT=StrToFloat(Edit13->Text);
	 }
 catch (EConvertError &E) {isw=testTflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label14->Caption +" "+ Edit13->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit14->Text);
	  if(Edit14->Text==L"0")meltT=0.f;
	  else meltT=StrToFloat(Edit14->Text);
	 }
 catch (EConvertError &E) {isw=testTflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label15->Caption +" "+ Edit14->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit15->Text);
	  if(Edit15->Text==L"0")lowcutT=0.f;
	  else lowcutT=StrToFloat(Edit15->Text);
	 }
 catch (EConvertError &E) {isw=testTflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label16->Caption +" "+ Edit15->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit16->Text);
	  if(Edit16->Text==L"0")preheatT=0.f;
	  else preheatT=StrToFloat(Edit16->Text);
	 }
 catch (EConvertError &E) {isw=testTflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label17->Caption +" "+ Edit16->Text + " must be floating-point");
						  }

 try {StrToFloat(Edit17->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label20->Caption +" "+ Edit17->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit18->Text);
	  if(Edit18->Text==L"0"){isw=heattimeflag=0;extern PACKAGE void __fastcall Beep(void);
							ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be positive floating-point");
						   }
	  else if(StrToFloat(Edit18->Text)>0.000001f)maxheattime=StrToFloat(Edit18->Text);
	  else {extern PACKAGE void __fastcall Beep(void);
			ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be positive floating-point");
		   }
	 }
 catch (EConvertError &E) {isw=heattimeflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be positive floating-point");
						  }
 try {StrToInt(Edit19->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label21->Caption +" "+ Edit19->Text + " must be positive integer");
						  }
 try {StrToFloat(Edit20->Text);
	  if(Edit20->Text==L"0"){isw=heattimeflag=0;extern PACKAGE void __fastcall Beep(void);
							ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be positive floating-point");
						   }
	  else if(StrToFloat(Edit20->Text)>0.000001f)minheattime=StrToFloat(Edit20->Text);
	  else {extern PACKAGE void __fastcall Beep(void);
			ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be positive floating-point");
		   }
	 }
 catch (EConvertError &E) {isw=heattimeflag=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be positive floating-point");
						  }

 try {StrToFloat(Edit21->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label24->Caption +" "+ Edit21->Text + " must be positive floating-point");
						  }
 try {StrToFloat(Edit22->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label25->Caption +" "+ Edit22->Text + " must be positive floating-point");
						  }

//////////////////// Special-purpose coding for VFT only  EFP 8/22/2010
// if(StrToFloat(Edit9->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label10->Caption +" "+ Edit9->Text + " must be positive");
//								 }
// if(StrToFloat(Edit10->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label11->Caption +" "+ Edit10->Text + " must be positive");
//								 }
// if(StrToFloat(Edit11->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label12->Caption +" "+ Edit11->Text + " must be positive");
//								 }
// if(StrToFloat(Edit12->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label13->Caption +" "+ Edit12->Text + " must be positive");
//								 }
// if(StrToFloat(Edit13->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label14->Caption +" "+ Edit13->Text + " must be positive");
//								 }
// if(StrToFloat(Edit14->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label15->Caption +" "+ Edit14->Text + " must be positive");
//								 }
// if(StrToFloat(Edit15->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label16->Caption +" "+ Edit15->Text + " must be positive");
//								 }
// if(StrToFloat(Edit16->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label17->Caption +" "+ Edit16->Text + " must be positive");
//								 }
 if(testTflag){if(meltT<=roomT){isw=0;extern PACKAGE void __fastcall Beep(void);
								ShowMessage("Inadmissable: room temperature exceeds melting temperature");
							   }
			   if(meltT<=lowcutT){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage("Inadmissable: lowcut temperature exceeds melting temperature");
								 }
			   if(meltT<=preheatT){isw=0;extern PACKAGE void __fastcall Beep(void);
								   ShowMessage("Inadmissable: preheat temperature exceeds melting temperature");
								  }
			  }
 if(kflagForm15<0){isw=0;extern PACKAGE void __fastcall Beep(void);ShowMessage(Label6->Caption);}
 if(jflagForm15<0){isw=0;extern PACKAGE void __fastcall Beep(void);ShowMessage(Label9->Caption);}

 if(StrToFloat(Edit17->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label20->Caption +" "+ Edit17->Text + " must be positive");
								 }
// if(StrToFloat(Edit18->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be positive");
//								 }
 if(StrToInt(Edit19->Text)<0){isw=0;extern PACKAGE void __fastcall Beep(void);
							  ShowMessage(Label21->Caption +" "+ Edit19->Text + " must be greater/equal 0");
							 }
// if(StrToFloat(Edit20->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
//								  ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be positive");
// 								 }
 if(heattimeflag){if(maxheattime<=minheattime){isw=0;extern PACKAGE void __fastcall Beep(void);
											   ShowMessage("Inadmissable: min heating time exeeds max heating time");
											  }
				 }

 if(StrToFloat(Edit21->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label24->Caption +" "+ Edit21->Text + " thickness must be positive");
								 }
 if(StrToFloat(Edit22->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label25->Caption +" "+ Edit22->Text + " thickness must be positive");
								 }
 if(Pl1norm<0 && Pl2norm<0){isw=0;extern PACKAGE void __fastcall Beep(void); //EFP 5/31/2011
							ShowMessage("Plate normals #1 & #2 are missing.");
						   }
 else if(Pl1norm<0){isw=0;extern PACKAGE void __fastcall Beep(void);
					ShowMessage("Plate normal #1 is missing.");
				   }
 else if(Pl2norm<0){isw=0;extern PACKAGE void __fastcall Beep(void);
					ShowMessage("Plate normal #2 is missing.");
				   }

/////////////////////////////////////////////////////////
// if(isw)Form1->wpCreate_public();// Close(); Do not use Close() because it was created with Show()
 if(isw){
//		 if(F15_isel==2 || F15_isel==3){Form1->wpEdit_public();
		 if(F15_isel>3 && F15_isel<=7){Form1->wpEdit_public();
//										Close(); //This caused Access Violation  EFP 4/18/2012
									   }
		 else Form1->wpCreate_public();
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button4Click(TObject *Sender)
{if(ColorDialog1->Execute())Shape1->Brush->Color=ColorDialog1->Color;
 else Shape1->Brush->Color=clLime;
}
//---------------------------------------------------------------------------
void __fastcall TForm15::ListBox1Click(TObject *Sender)
{for(int i=0;i<nMatPS;i++)if(ListBox1->Selected[i]){kflagForm15=i;break;}
}
//---------------------------------------------------------------------------
void __fastcall TForm15::ListBox2Click(TObject *Sender)
{for(int i=0;i<nWeldPS;i++)if(ListBox2->Selected[i]){jflagForm15=i;break;}
 Form1->wpCreate1_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button3Click(TObject *Sender)
{
////Form1->wpCreate2_public();
// if(F15_isel==2 || F15_isel==3){Form1->wpEdit2_public();Close();}
 if(F15_isel>3 && F15_isel<=7){Form1->wpEdit2_public();Close();}
 else Form1->wpCreate2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button2Click(TObject *Sender)
{
////Form1->wpCreate2_public();
// if(F15_isel==2 || F15_isel==3){Form1->wpEdit2_public();Close();}
 if(F15_isel>3 && F15_isel<=7){Form1->wpEdit2_public();Close();}
 else Form1->wpCreate2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm15::RadioGroup1Click(TObject *Sender)
{if(RadioGroup1->ItemIndex==1){F15_isel=5;
							   Label3->Caption="All start elements";
							   Label4->Caption="One stop element";
							  }
else if(RadioGroup1->ItemIndex==2){F15_isel=6;
								   Label3->Caption="One start element";
								   Label4->Caption="Auto-gen to end run (click for direction)";
								  }
else if(RadioGroup1->ItemIndex==3){F15_isel=7;
								   Label3->Caption="One start element";
								   Label4->Caption="One stop element";
								  }
else {F15_isel=4;
	  Label3->Caption="All start elements"; //0
	  Label4->Caption="Auto-gen to end run (click for direction)";
	 }
}
//---------------------------------------------------------------------------

