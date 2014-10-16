//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit30.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm30 *Form30;
//---------------------------------------------------------------------------
//__fastcall TForm30::TForm30(long nWeldPass,String name[],TColor WeldColor[],int seqNum[],TComponent* Owner)
__fastcall TForm30::TForm30(long nWeldPass,String name[],TColor WeldColor[],int seqNum[],long reset[],TComponent* Owner)
	: TForm(Owner)
{long i=0,j=0;
 for(i=0;i<nWeldPass;i++) //Each order
   {for(j=0;j<nWeldPass;j++)if(seqNum[j]-1==i)break;   // Note that seqNum is numbered 1 to...
	CheckListBox1->Items->Add(name[j].t_str());CheckListBox2->Items->Add(name[j].t_str());
	if(reset[j]-10*(reset[j]/10)){CheckListBox1->Checked[j]=true;CheckListBox2->Checked[j]=true;}
	invColor[i]=invorigColor[i]=WeldColor[j];localseqInv[i]=origseqInv[i]=j;
   }
 CheckListBox1->Enabled=false;CheckListBox2->ItemIndex=0;seqIndex=0;
}
//---------------------------------------------------------------------------
long TForm30::getEdit1(){
//                           if(input_checker(0,Edit23->Text))CheckEdit23=StrToInt(Edit23->Text);
//						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter integer number",L"Halt",MB_OK);
// 								 CheckEdit23=1;
//								}
//						   return CheckEdit23;
						   return StrToInt(Edit1->Text);
						  }
//---------------------------------------------------------------------------
void TForm30::setEdit1(long x){Edit1->Text=IntToStr(INT64(x));}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button1Click(TObject *Sender){Form1->WeldPassEditSeqn2_public();} //Enter
//---------------------------------------------------------------------------
void __fastcall TForm30::Button2Click(TObject *Sender)//Animate seq
//{Form1->WeldPassEditSeqn2_public();
{try {if(StrToInt(Edit1->Text)>0)Form1->WeldPassEditSeqn1_public();
	  else {extern PACKAGE void __fastcall Beep(void);ShowMessage(Label4->Caption +" "+ Edit1->Text + " must be positive");}
	 } //CAUTION: The user might have to click "Continue" after the following message.
 catch (EConvertError &E){extern PACKAGE void __fastcall Beep(void);ShowMessage(Label4->Caption +" "+ Edit1->Text + " must be positive integer");}
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button3Click(TObject *Sender)//Move up
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count)
   {swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex-1];
	localseqInv[CheckListBox2->ItemIndex-1]=swp;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex-1];
	invColor[CheckListBox2->ItemIndex-1]=swpColor;
	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex-1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex-1;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a lower list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button4Click(TObject *Sender)//Move down
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>=0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count-1)
   {swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex+1];
	localseqInv[CheckListBox2->ItemIndex+1]=swp;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex+1];
	invColor[CheckListBox2->ItemIndex+1]=swpColor;
	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex+1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex+1;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click an upper list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button5Click(TObject *Sender)//restore current directions EFP 3/29/2012
{long i=0,j=0;
 for(i=0;i<CheckListBox2->Items->Count;i++)
   {for(j=0;j<CheckListBox1->Items->Count;j++)
	   if(localseqInv[i]==origseqInv[j]){CheckListBox2->Checked[i]=CheckListBox1->Checked[j];
										 break;
										}
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button6Click(TObject *Sender)//restore current seq+dir
{for(int i=0;i<CheckListBox1->Items->Count;i++)
   {CheckListBox2->Items->Strings[i]=CheckListBox1->Items->Strings[i]; //TBD: allow for different incoming order
	CheckListBox2->Checked[i]=CheckListBox1->Checked[i];
	invColor[i]=invorigColor[i];localseqInv[i]=origseqInv[i];
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button7Click(TObject *Sender)//Reverse all directions EFP 12/31/2011
{for(long i=0;i<CheckListBox2->Items->Count;i++)CheckListBox2->Checked[i]=1-CheckListBox2->Checked[i];
}
//---------------------------------------------------------------------------
void __fastcall TForm30::CheckListBox2Click(TObject *Sender){setWColor1(invColor[CheckListBox2->ItemIndex]);}
//---------------------------------------------------------------------------
TColor TForm30::getWColor1(){return Shape1->Brush->Color;}
void TForm30::setWColor1(TColor s){Shape1->Brush->Color=s;}
//---------------------------------------------------------------------------
//int TForm30::getSeq(){return localseqInv[seqIndex]+1;}
int TForm30::getSeq(){return seqIndex+1;}
bool TForm30::getDir(){return CheckListBox2->Checked[seqIndex];}
//---------------------------------------------------------------------------
//void TForm23::setSeqIndex(int s){seqIndex=s;}
void TForm30::setSeqIndex(int s){long j=0;    //Receive WP# s and convert to associated Seq# j
//								 seqIndex=s;
	for(j=0;j<CheckListBox2->Items->Count;j++)if(localseqInv[j]==s){seqIndex=j;break;}   // Note that seqNum is numbered 1 to...
								}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button8Click(TObject *Sender){Form1->WeldPassEditSeqn3_public();}
//---------------------------------------------------------------------------

