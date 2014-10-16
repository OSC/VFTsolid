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
//int iflagForm15=0,kflagForm15= -1,jflagForm15= -1,nMatPS=0,nWeldPS=0;
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
 Edit2->Text=IntToStr(INT64(currSeq));
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
 for(i=0;i<nMatPS;i++)ListBox1->Items->Add(gMatPropName[i].t_str());
 for(i=0;i<nWeldPS;i++)ListBox2->Items->Add(gWeldParName[i].t_str());
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
//                           if(input_checker(0,Edit23->Text))CheckEdit23=StrToInt(Edit23->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter integer number",L"Halt",MB_OK);
// 								 CheckEdit23=1;
//								}
//						   return CheckEdit23;
						   return StrToInt(Edit2->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit9(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit9->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit10(){
//                           if(input_checker(1,Edit10->Text))CheckEdit10=StrToFloat(Edit10->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit10=0.;
//								}
//						   return CheckEdit10;
						   return StrToFloat(Edit10->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit11(){
//						   if(input_checker(1,Edit11->Text))CheckEdit11=StrToFloat(Edit11->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit11=0.;
//								}
//						   return CheckEdit11;
						   return StrToFloat(Edit11->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit12(){
//                           if(input_checker(1,Edit12->Text))CheckEdit12=StrToFloat(Edit12->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit12=0.;
//								}
//						   return CheckEdit12;
						   return StrToFloat(Edit12->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit13(){
//                           if(input_checker(1,Edit13->Text))CheckEdit13=StrToFloat(Edit13->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit13=0.;
//								}
//						   return CheckEdit13;
						   return StrToFloat(Edit13->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit14(){
//                           if(input_checker(1,Edit14->Text))CheckEdit14=StrToFloat(Edit14->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit14=0.;
//								}
//						   return CheckEdit14;
						   return StrToFloat(Edit14->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit15(){
//                           if(input_checker(1,Edit15->Text))CheckEdit15=StrToFloat(Edit15->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit15=0.;
//								}
//						   return CheckEdit15;
						   return StrToFloat(Edit15->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit16(){
//                           if(input_checker(1,Edit16->Text))CheckEdit16=StrToFloat(Edit16->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit16=0.;
//								}
//						   return CheckEdit16;
						   return StrToFloat(Edit16->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit17(){
//                           if(input_checker(1,Edit17->Text))CheckEdit17=StrToFloat(Edit17->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit17=0.;
//								}
//						   return CheckEdit17;
						   return StrToFloat(Edit17->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit18(){
//                           if(input_checker(1,Edit18->Text))CheckEdit18=StrToFloat(Edit18->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit18=0.;
//								}
//						   return CheckEdit18;
						   return StrToFloat(Edit18->Text);
						  }
//---------------------------------------------------------------------------
int TForm15::getEdit19(){
//                           if(input_checker(1,Edit19->Text))CheckEdit19=StrToFloat(Edit19->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit19=0.;
//								}
//						   return CheckEdit19;
						   return StrToInt(Edit19->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit20(){
//                           if(input_checker(1,Edit20->Text))CheckEdit20=StrToFloat(Edit20->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit20=0.;
//								}
//						   return CheckEdit20;
						   return StrToFloat(Edit20->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit21(){
//                           if(input_checker(1,Edit21->Text))CheckEdit21=StrToFloat(Edit21->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit21=0.;
//								}
//						   return CheckEdit21;
						   return StrToFloat(Edit21->Text);
						  }
//---------------------------------------------------------------------------
double TForm15::getEdit22(){
//                           if(input_checker(1,Edit22->Text))CheckEdit22=StrToFloat(Edit22->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit22=0.;
//								}
//						   return CheckEdit22;
						   return StrToFloat(Edit22->Text);
						  }
//---------------------------------------------------------------------------
long TForm15::getEdit23(){
//                           if(input_checker(0,Edit23->Text))CheckEdit23=StrToInt(Edit23->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter integer number",L"Halt",MB_OK);
// 								 CheckEdit23=1;
//								}
//						   return CheckEdit23;
						   return StrToInt(Edit23->Text);
						  }
//---------------------------------------------------------------------------
long TForm15::getEdit24(){
//                           if(input_checker(0,Edit24->Text))CheckEdit24=StrToInt(Edit24->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter integer number",L"Halt",MB_OK);
//								 CheckEdit24=1;
//								}
//						   return CheckEdit24;
						   return StrToInt(Edit24->Text);
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
void TForm15::setEdit2(long x){Edit2->Text=IntToStr(INT64(x));}
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
void TForm15::setEdit23(long x){Edit23->Text=IntToStr(INT64(x));}
void TForm15::setEdit24(long x){Edit24->Text=IntToStr(INT64(x));}
void TForm15::setType(int s){ComboBox1->ItemIndex=s;}
void TForm15::setShape(int s){ComboBox2->ItemIndex=s;}
void TForm15::setWColor(TColor s){Shape1->Brush->Color=s;}
//---------------------------------------------------------------------------
void __fastcall TForm15::Button1Click(TObject *Sender)
{int isw=1;float TOL=0.000001;
 try {StrToFloat(Edit9->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label10->Caption +" "+ Edit9->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit10->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label11->Caption +" "+ Edit10->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit11->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label12->Caption +" "+ Edit11->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit12->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label13->Caption +" "+ Edit12->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit13->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label14->Caption +" "+ Edit13->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit14->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label15->Caption +" "+ Edit14->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit15->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label16->Caption +" "+ Edit15->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit16->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label17->Caption +" "+ Edit16->Text + " must be floating-point");
						  }

 try {StrToFloat(Edit17->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label20->Caption +" "+ Edit17->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit18->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be floating-point");
						  }
 try {StrToInt(Edit19->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label21->Caption +" "+ Edit19->Text + " must be integer");
						  }
 try {StrToFloat(Edit20->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be floating-point");
						  }

 try {StrToFloat(Edit21->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label24->Caption +" "+ Edit21->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit22->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label25->Caption +" "+ Edit22->Text + " must be floating-point");
						  }

//////////////////// Special-purpose coding for VFT only  EFP 8/22/2010
 if(StrToFloat(Edit9->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label10->Caption +" "+ Edit9->Text + " must be positive");
								 }
 if(StrToFloat(Edit10->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label11->Caption +" "+ Edit10->Text + " must be positive");
								 }
 if(StrToFloat(Edit11->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label12->Caption +" "+ Edit11->Text + " must be positive");
								 }
 if(StrToFloat(Edit12->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label13->Caption +" "+ Edit12->Text + " must be positive");
								 }
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

 if(kflagForm15<0){isw=0;extern PACKAGE void __fastcall Beep(void);ShowMessage(Label6->Caption);}
 if(jflagForm15<0){isw=0;extern PACKAGE void __fastcall Beep(void);ShowMessage(Label9->Caption);}

 if(StrToFloat(Edit17->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label20->Caption +" "+ Edit17->Text + " must be positive");
								 }
 if(StrToFloat(Edit18->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label22->Caption +" "+ Edit18->Text + " must be positive");
								 }
 if(StrToInt(Edit19->Text)<0){isw=0;extern PACKAGE void __fastcall Beep(void);
							  ShowMessage(Label21->Caption +" "+ Edit19->Text + " must be greater/equal 0");
							 }
 if(StrToFloat(Edit20->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								  ShowMessage(Label23->Caption +" "+ Edit20->Text + " must be positive");
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
int TForm15::input_checker(int isw,AnsiString RM)
// isw=0 --> integer; 1 --> float
{int ans=0;
 if(!StrLen(RM.c_str()))return ans=0;
 if(AnsiStrScan(RM.c_str(),'a') || AnsiStrScan(RM.c_str(),'A'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'b') || AnsiStrScan(RM.c_str(),'B'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'c') || AnsiStrScan(RM.c_str(),'C'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'d') || AnsiStrScan(RM.c_str(),'D'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'f') || AnsiStrScan(RM.c_str(),'F'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'g') || AnsiStrScan(RM.c_str(),'G'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'h') || AnsiStrScan(RM.c_str(),'H'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'i') || AnsiStrScan(RM.c_str(),'I'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'j') || AnsiStrScan(RM.c_str(),'J'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'k') || AnsiStrScan(RM.c_str(),'K'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'l') || AnsiStrScan(RM.c_str(),'L'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'m') || AnsiStrScan(RM.c_str(),'M'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'n') || AnsiStrScan(RM.c_str(),'N'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'o') || AnsiStrScan(RM.c_str(),'O'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'p') || AnsiStrScan(RM.c_str(),'P'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'q') || AnsiStrScan(RM.c_str(),'Q'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'r') || AnsiStrScan(RM.c_str(),'R'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'s') || AnsiStrScan(RM.c_str(),'S'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'t') || AnsiStrScan(RM.c_str(),'T'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'u') || AnsiStrScan(RM.c_str(),'U'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'v') || AnsiStrScan(RM.c_str(),'V'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'w') || AnsiStrScan(RM.c_str(),'W'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'x') || AnsiStrScan(RM.c_str(),'X'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'y') || AnsiStrScan(RM.c_str(),'Y'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'z') || AnsiStrScan(RM.c_str(),'Z'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'`') || AnsiStrScan(RM.c_str(),'~'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'!') || AnsiStrScan(RM.c_str(),'@'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'#') || AnsiStrScan(RM.c_str(),'$'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'%') || AnsiStrScan(RM.c_str(),'^'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'&') || AnsiStrScan(RM.c_str(),'*'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'(') || AnsiStrScan(RM.c_str(),')'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'_') || AnsiStrScan(RM.c_str(),'='))return ans=0;
 if(AnsiStrScan(RM.c_str(),'[') || AnsiStrScan(RM.c_str(),'{'))return ans=0;
 if(AnsiStrScan(RM.c_str(),']') || AnsiStrScan(RM.c_str(),'}'))return ans=0;
// if(AnsiStrScan(RM.c_str(),'\'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'|'))return ans=0;
 if(AnsiStrScan(RM.c_str(),';') || AnsiStrScan(RM.c_str(),':'))return ans=0;
// if(AnsiStrScan(RM.c_str(),'''))return ans=0;
 if(AnsiStrScan(RM.c_str(),'"'))return ans=0;
 if(AnsiStrScan(RM.c_str(),',') || AnsiStrScan(RM.c_str(),'<'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'>'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'/') || AnsiStrScan(RM.c_str(),'?'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'.') != AnsiStrRScan(RM.c_str(),'.'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'e') != AnsiStrRScan(RM.c_str(),'e'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'E') != AnsiStrRScan(RM.c_str(),'E'))return ans=0;
 if(!isw){if(AnsiStrScan(RM.c_str(),'e') || AnsiStrScan(RM.c_str(),'E'))return ans=0;
		  if(AnsiStrScan(RM.c_str(),'.'))return ans=0;
		 }
 return ans=1;
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

