//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit21.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm21 *Form21;
//---------------------------------------------------------------------------
__fastcall TForm21::TForm21(TComponent* Owner)
	: TForm(Owner)
{
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
String TForm21::getEdit8(){return Edit8->Text;}
void TForm21::setEdit1(String s){Edit1->Text=s;}
void TForm21::setEdit5a(String s){Edit5->Text=s;}
void TForm21::setEdit8(String s){Edit8->Text=s;}
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
						   return StrToFloat(Edit9->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit6(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit10->Text);
						  }
//---------------------------------------------------------------------------
double TForm21::getEdit7(){
//						   if(input_checker(1,Edit9->Text))CheckEdit9=StrToFloat(Edit9->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Halt",MB_OK);
//								 CheckEdit9=0.;
//								}
//						   return CheckEdit9;
						   return StrToFloat(Edit11->Text);
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
bool TForm21::getGroupBox3(){if(RadioButton12->Checked==true)return true;
							 else return false;
							}
//---------------------------------------------------------------------------
bool TForm21::getGroupBox4(){if(RadioButton8->Checked==true)return true;
							 else return false;
							}
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
									if(s<1)RadioButton1->Checked=true;
									else if(s==1)RadioButton2->Checked=true;
									else if(s==2)RadioButton3->Checked=true;
									else if(s==3)RadioButton4->Checked=true;
									else if(s==4)RadioButton5->Checked=true;
									else RadioButton6->Checked=true;
								   }
//---------------------------------------------------------------------------
void TForm21::setGroupBox3(bool s){if(s==false){RadioButton11->Checked=true;
												RadioButton12->Checked=false;
											   }
								   else {RadioButton11->Checked=false;
										 RadioButton12->Checked=true;
										}
								  }
//---------------------------------------------------------------------------
void TForm21::setGroupBox4(bool s){if(s==false){RadioButton7->Checked=true;
												RadioButton8->Checked=false;
											   }
								   else {RadioButton7->Checked=false;
										 RadioButton8->Checked=true;
										}
								  }
//---------------------------------------------------------------------------
void TForm21::setCheckBox1(bool s){CheckBox1->Checked=s;}
//---------------------------------------------------------------------------
void TForm21::setEdit2(double x){Edit2->Text=FloatToStr(x);}
void TForm21::setEdit3(double x){Edit3->Text=FloatToStr(x);}
void TForm21::setEdit4(double x){Edit4->Text=FloatToStr(x);}
void TForm21::setEdit5(double x){Edit9->Text=FloatToStr(x);}
void TForm21::setEdit6(double x){Edit10->Text=FloatToStr(x);}
void TForm21::setEdit7(double x){Edit11->Text=FloatToStr(x);}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button2Click(TObject *Sender)
{
 Form1->MatProperties_public();
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm21::Button1Click(TObject *Sender)
{OpenDialog1->Filter= "ABA_mat (*.dat)|*.dat;*.DAT";
 if(OpenDialog1->Execute())setEdit8(OpenDialog1->FileName);
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unable to create MAT file-open dialog",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton11Click(TObject *Sender)
{Edit9->Enabled=false;Edit10->Enabled=false;Edit11->Enabled=false;
 Label9->Enabled=false;Label10->Enabled=false;Label11->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton12Click(TObject *Sender)
{Edit9->Enabled=true;Edit10->Enabled=true;Edit11->Enabled=true;
 Label9->Enabled=true;Label10->Enabled=true;Label11->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::RadioButton1Click(TObject *Sender)
{RadioButton2->Enabled=false;
 RadioButton3->Enabled=false;
 RadioButton4->Enabled=false;
 RadioButton5->Enabled=false;
 RadioButton6->Enabled=false;
 CheckBox1->Enabled=true;
 Edit5->Enabled=false;
 RadioButton2->Visible=false;
 RadioButton3->Visible=false;
 RadioButton4->Visible=false;
 RadioButton5->Visible=false;
 RadioButton6->Visible=false;
 CheckBox1->Visible=true;
 Edit5->Visible=true;
 Label6->Visible=true;
 Label6->Enabled=false;
 RadioGroup1->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm21::CheckBox1Click(TObject *Sender)
{Edit5->Enabled=true;
 Label6->Enabled=true;
}
//---------------------------------------------------------------------------

