//---------------------------------------------------------------------------
// VFTmisc Form21 == VFTgen Form16  EFP 8/07/2010
// Note: Function int ParseSteps(String) is duplicated in Unit1.cpp/h  EFP 3/14/2012
#include <vcl.h>
//#include <fstream.h> //Need for use with ofstream konk()
#pragma hdrstop

#include "Unit21.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm21 *Form21;

//ofstream konk("Chequers.txt");
//---------------------------------------------------------------------------
__fastcall TForm21::TForm21(int isel,TComponent* Owner) : TForm(Owner)
{F21_sel=isel;
 RB1_count=0;
//////////////////// EFP 3/14/2012
 CheckBox1->Enabled=false;CheckBox1->Visible=false;
 Edit5->Enabled=false;Edit5->Visible=false;
 Button4->Enabled=false;Button4->Visible=false;
////////////////////
}
//---------------------------------------------------------------------------
//Unicode TForm16::getEdit1(){return Edit1->Text;}
//Unicode TForm16::getEdit8(){return Edit8->Text;}
//void TForm16::setEdit1(Unicode s){Edit1->Text=s;}
//void TForm16::setEdit8(Unicode s){Edit8->Text=s;}
////AnsiString TForm16::getEdit1(){return Edit1->Text;}
////AnsiString TForm16::getEdit8(){return Edit8->Text;}
////void TForm16::setEdit1(AnsiString s){Edit1->Text=s;}
////void TForm16::setEdit8(AnsiString s){Edit8->Text=s;}
String TForm21::getEdit1(){return Edit1->Text;}
String TForm21::getEdit5a(){return Edit5->Text;}
String TForm21::getEdit8(){return Edit9->Text;}
void TForm21::setEdit1(String s){Edit1->Text=s;}
void TForm21::setEdit5a(String s){Edit5->Text=s;}
void TForm21::setEdit8(String s){Edit9->Text=s;}
//char TForm16::getEdit1(){return *Edit1->Text.c_str();}
//char TForm16::getEdit8(){return *Edit8->Text.c_str();}
//void TForm16::setEdit1(char *s[]){Edit1->Text=s;}
//void TForm16::setEdit8(char *s[]){Edit8->Text=s;}
//---------------------------------------------------------------------------
double TForm21::getEdit2(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit2->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit3(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit3->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit4(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit4->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit5(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit6->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit6(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit7->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit7(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit8->Text);
						  }
//---------------------------------------------------------------------------
//int TForm16::getRadioGroup1(){return RadioGroup1->ItemIndex;}
int TForm21::getRadioGroup1(){if(RadioButton1->Checked==true)return 0;
							  else if(RadioButton2->Checked==true)return 1;
							  else if(RadioButton3->Checked==true)return 2;
							  else if(RadioButton4->Checked==true)return 3;
							  else if(RadioButton5->Checked==true)return 4;
							  else return 5;
							 }
//---------------------------------------------------------------------------
//bool TForm21::getGroupBox3(){if(RadioButton10->Checked==true)return true;
//							 else return false;
//							}
bool TForm21::getGroupBox3(){return RadioButton10->Checked;}//Note that this refers to Form21/GroupBox2 (but names were discombobulated during revisions) EFP 2/27/2012
//---------------------------------------------------------------------------
bool TForm21::getGroupBox4(){return RadioButton8->Checked;}//Note that this refers to Form21/GroupBox3 (but names were discombobulated during revisions) EFP 2/27/2012
//---------------------------------------------------------------------------
bool TForm21::getCheckBox1(){return CheckBox1->Checked;}
//---------------------------------------------------------------------------
//void TForm16::setRadioGroup1(int s){if(s>5)RadioGroup1->ItemIndex=5;
//									else if(s<0)RadioGroup1->ItemIndex=0;
//									else RadioGroup1->ItemIndex=s;
//								   }
void TForm21::setRadioGroup1(int s){RadioButton1->Checked=false;
									RadioButton2->Checked=false;
									RadioButton3->Checked=false;
									RadioButton4->Checked=false;
									RadioButton5->Checked=false;
									RadioButton6->Checked=false;
									if(s<1){RadioButton1->Checked=true;
Button1->Caption="Choose Mechanical Property File for Isotropic (multi-linear)...";
/////////////////// start EFP 9/23/2011
if(CheckBox1->Checked){
 RadioButton2->Enabled=false;
 RadioButton3->Enabled=false;
 RadioButton4->Enabled=false;
 RadioButton5->Enabled=false;
 RadioButton6->Enabled=false;
// CheckBox1->Enabled=true;
 Edit5->Enabled=true;
 Button4->Enabled=true;
 RadioButton2->Visible=false;
 RadioButton3->Visible=false;
 RadioButton4->Visible=false;
 RadioButton5->Visible=false;
 RadioButton6->Visible=false;
 CheckBox1->Visible=true;
 Edit5->Visible=true;
 Button4->Visible=true;
 Label6->Visible=true;
 Label6->Enabled=false;
 RadioGroup1->Visible=false;
					  }
/////////////////// end
										   }
									else if(s==1){RadioButton2->Checked=true;
//Button1->Caption="Choose Mechanical Property File for Combined Hardening (multi-linear)...";
Button1->Caption="Choose Mechanical Property File for Mixed Hardening...";
												 }
									else if(s==2){RadioButton3->Checked=true;
Button1->Caption="Choose Mechanical Property File for Simple Phase Transformation...";
												 }
									else if(s==3){RadioButton4->Checked=true;
//Button1->Caption="Choose Mechanical Property File for Combined Hardening ...";
Button1->Caption="Choose Mechanical Property File for Non-linear Kinematic...";
												 }
									else if(s==4){RadioButton5->Checked=true;
Button1->Caption="Choose Mechanical Property File for Elastic-Plastic-Creep...";
/////////////////// start EFP 3/14/2012
if(CheckBox1->Checked){
 RadioButton1->Enabled=false;
 RadioButton2->Enabled=false;
 RadioButton3->Enabled=false;
 RadioButton4->Enabled=false;
 RadioButton6->Enabled=false;
// CheckBox1->Enabled=true;
 Edit5->Enabled=true;
 Button4->Enabled=true;
 RadioButton1->Visible=false;
 RadioButton2->Visible=false;
 RadioButton3->Visible=false;
 RadioButton4->Visible=false;
 RadioButton6->Visible=false;
 CheckBox1->Visible=true;
 Edit5->Visible=true;
 Button4->Visible=true;
 Label6->Visible=true;
 Label6->Enabled=false;
 RadioGroup1->Visible=false;
					  }
/////////////////// end
												 }
									else {RadioButton6->Checked=true;
Button1->Caption="Choose Mechanical Property File for Complete Phase Transformation...";
										 }
									RB1_count=0;
								   }
//---------------------------------------------------------------------------
void TForm21::setGroupBox3(bool s){if(s==false){RadioButton9->Checked=true;//Note that this is called "GroupBox2" on form
												RadioButton10->Checked=false;
											   }
								   else {RadioButton9->Checked=false;
										 RadioButton10->Checked=true;
										}
								  }
//---------------------------------------------------------------------------
void TForm21::setGroupBox4(bool s){if(s==false){RadioButton7->Checked=true;//Note that this is called "GroupBox3" on form
												RadioButton8->Checked=false;
											   }
								   else {RadioButton7->Checked=false;
										 RadioButton8->Checked=true;
										}
								  }
//---------------------------------------------------------------------------
void TForm21::setCheckBox1(bool s){CheckBox1->Checked=s;
////                                   Edit5->FocusControl();
//                                   Edit5.SetSelection(0,2);
								  }
//---------------------------------------------------------------------------
void TForm21::setEdit2(double x){Edit2->Text=FloatToStr(x);}
void TForm21::setEdit3(double x){Edit3->Text=FloatToStr(x);}
void TForm21::setEdit4(double x){Edit4->Text=FloatToStr(x);}
void TForm21::setEdit5(double x){Edit6->Text=FloatToStr(x);}
void TForm21::setEdit6(double x){Edit7->Text=FloatToStr(x);}
void TForm21::setEdit7(double x){Edit8->Text=FloatToStr(x);}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button2Click(TObject *Sender)
{int isw=1,j=0,icount=0,iactive=0;float TOL=0.000001;
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
 try {StrToFloat(Edit6->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label6->Caption +" "+ Edit6->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit7->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label7->Caption +" "+ Edit7->Text + " must be floating-point");
						  }
 try {StrToFloat(Edit8->Text);}
 catch (EConvertError &E) {isw=0;extern PACKAGE void __fastcall Beep(void);
						   ShowMessage(Label8->Caption +" "+ Edit8->Text + " must be floating-point");
						  }
//////////////////// Special-purpose coding for VFT only  EFP 8/22/2010
 if(StrToFloat(Edit2->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label2->Caption +" "+ Edit2->Text + " must be positive");
								}
 if(StrToFloat(Edit3->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label3->Caption +" "+ Edit3->Text + " must be positive");
								}
 if(StrToFloat(Edit4->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label4->Caption +" "+ Edit4->Text + " must be positive");
								}
 if(StrToFloat(Edit6->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label7->Caption +" "+ Edit6->Text + " must be positive");//Note Label/Edit noncoincidence
								}
 if(StrToFloat(Edit7->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label8->Caption +" "+ Edit7->Text + " must be positive");//Note Label/Edit noncoincidence
								}
 if(StrToFloat(Edit8->Text)<TOL){isw=0;extern PACKAGE void __fastcall Beep(void);
								 ShowMessage(Label9->Caption +" "+ Edit8->Text + " must be positive");//Note Label/Edit noncoincidence
								}
 if(Edit9->Text==L"****"){isw=0;extern PACKAGE void __fastcall Beep(void);
						  ShowMessage(Label10->Caption +" "+ Edit9->Text + " must be a filename");//Note Label/Edit noncoincidence
						 }
 if(CheckBox1->Checked){
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
						icount=iactive=0;        //THIS IS ALL WRONG
						for(j=0;j<Edit5->Text.Length();j++){if(Edit5->Text==' ')iactive=0;
															else if(iactive==0){icount++;iactive=1;}
														   }
//yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
						if(Edit5->Text==L"****"){isw=0;extern PACKAGE void __fastcall Beep(void);
												 ShowMessage(Label6->Caption +" "+ Edit5->Text + " must be a set of steps (e.g. 9,10,11)");//Note Label/Edit noncoincidence
												}
//						else if(!ParseSteps(Edit5->Text)){isw=0;extern PACKAGE void __fastcall Beep(void);
						else if(!icount){isw=0;extern PACKAGE void __fastcall Beep(void);
										 ShowMessage(Label6->Caption +" "+ Edit5->Text + " must be a set of steps (e.g. 9,10,11)");//Note Label/Edit noncoincidence
										}
					   }
/////////////////////////////////////////////////////////
 if(isw){Form1->MatProperties_public();
//		 Close();
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button1Click(TObject *Sender)
{OpenDialog1->Filter= "ABA_mat (*.dat)|*.dat;*.DAT";
 if(OpenDialog1->Execute())setEdit8(OpenDialog1->FileName);
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unable to create MAT file-open dialog",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton9Click(TObject *Sender)
{Edit6->Enabled=false;Edit7->Enabled=false;Edit8->Enabled=false;
 Label7->Enabled=false;Label8->Enabled=false;Label9->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton10Click(TObject *Sender)
{Edit6->Enabled=true;Edit7->Enabled=true;Edit8->Enabled=true;
 Label7->Enabled=true;Label8->Enabled=true;Label9->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton1Click(TObject *Sender){RB1_count=0;
 Button1->Caption="Choose Mechanical Property File for Isotropic (multi-linear)...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton2Click(TObject *Sender){RB1_count=0;
 Button1->Caption="Choose Mechanical Property File for Combined Hardening (multi-linear)...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton3Click(TObject *Sender){RB1_count=0;
 Button1->Caption="Choose Mechanical Property File for Simple Phase Transformation...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton4Click(TObject *Sender){RB1_count=0;
 Button1->Caption="Choose Mechanical Property File for Combined Hardening (linear)...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton5Click(TObject *Sender){RB1_count=0;
 Button1->Caption="Choose Mechanical Property File for Elastic-Plastic-Creep...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton6Click(TObject *Sender){RB1_count=0;
Button1->Caption="Choose Mechanical Property File for Complete Phase Transformation...";
														   }
//---------------------------------------------------------------------------
void __fastcall TForm21::CheckBox1Click(TObject *Sender)
{if(CheckBox1->Checked){
 Edit5->Enabled=true;
 Label6->Enabled=true;
// SetSelection(0,2);
	 }
 else {
 Edit5->Enabled=false;
 Label6->Enabled=false;
	  }
}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button3Click(TObject *Sender)
{RB1_count=0;
// Close();
 Form1->EditMatProp_public();
}
//---------------------------------------------------------------------------
int TForm21::getISEL(){return F21_sel;}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button4Click(TObject *Sender)
{RadioButton1->Enabled=true;
 RadioButton2->Enabled=true;
 RadioButton3->Enabled=true;
 RadioButton4->Enabled=true;
 RadioButton5->Enabled=true;
 RadioButton6->Enabled=true;
// CheckBox1->Enabled=false;
 Edit5->Enabled=false;
 RadioButton1->Visible=true;
 RadioButton2->Visible=true;
 RadioButton3->Visible=true;
 RadioButton4->Visible=true;
 RadioButton5->Visible=true;
 RadioButton6->Visible=true;
 CheckBox1->Visible=false;
 Edit5->Visible=false;
 Label6->Enabled=false;
 Label6->Visible=false;
 RadioGroup1->Visible=true;
 setCheckBox1(false);
 RB1_count=0;
 setRadioGroup1(0);
 Button4->Enabled=false;
 Button4->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::Bananas(TObject *Sender)//Doubleclick RadioButton1
{RadioButton2->Enabled=false;
 RadioButton3->Enabled=false;
 RadioButton4->Enabled=false;
 RadioButton5->Enabled=false;
 RadioButton6->Enabled=false;
 CheckBox1->Enabled=true;
 Edit5->Enabled=false;
 Button4->Enabled=true;
 RadioButton2->Visible=false;
 RadioButton3->Visible=false;
 RadioButton4->Visible=false;
 RadioButton5->Visible=false;
 RadioButton6->Visible=false;
 CheckBox1->Visible=true;
 Edit5->Visible=true;
 Button4->Visible=true;
 Label6->Visible=true;
 Label6->Enabled=false;
 RadioGroup1->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::Plantains(TObject *Sender)//Doubleclick RadioButton5
{RadioButton1->Enabled=false;
 RadioButton2->Enabled=false;
 RadioButton3->Enabled=false;
 RadioButton4->Enabled=false;
 RadioButton6->Enabled=false;
 CheckBox1->Enabled=true;
 Edit5->Enabled=false;
 Button4->Enabled=true;
 RadioButton1->Visible=false;
 RadioButton2->Visible=false;
 RadioButton3->Visible=false;
 RadioButton4->Visible=false;
 RadioButton6->Visible=false;
 CheckBox1->Visible=true;
 Edit5->Visible=true;
 Button4->Visible=true;
 Label6->Visible=true;
 Label6->Enabled=false;
 RadioGroup1->Visible=false;
}
/*
//---------------------------------------------------------------------------
int TForm21::ParseSteps(String value) //Test integrity of "#Steps input"  EFP 3/14/2012
{int nvalue=value.Length(),is=0,ic=0,flag=0,prevcomma=1,sumcomma= -1;
 char accept[11]={'0','1','2','3','4','5','6','7','8','9',','};
 char *szBuffer=new char[value.Length()+1];StrCopy(szBuffer,value.c_str());
 if(nvalue){if(*szBuffer != accept[11-1] && *(szBuffer+nvalue-1)!= accept[11-1])
			  {sumcomma=0;for(is=0;is<nvalue;is++){flag= -1;for(ic=0;ic<11;ic++)if(*(szBuffer+is) == accept[ic]){flag=ic;break;}
												   if(flag<0 || (prevcomma==1 && flag==11-1)){sumcomma= -1;break;}
												   else if(flag==11-1){prevcomma=1;sumcomma++;}
												   else prevcomma=0;
												  }
			  }
		   }
 delete [] szBuffer;return sumcomma+1;
}
*/
//---------------------------------------------------------------------------


