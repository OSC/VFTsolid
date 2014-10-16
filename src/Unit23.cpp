//---------------------------------------------------------------------------
// VFTmisc Form23 == VFTgen Form18  EFP 8/08/2010
// Drastic warning: localColor[128],origColor[128],localseqNum[128],origseqNum[128] HARDWIRED
#include <vcl.h>
#pragma hdrstop

#include "Unit23.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm23 *Form23;
//---------------------------------------------------------------------------
__fastcall TForm23::TForm23(long nWeldPass,String name[],TColor WeldColor[],int seqNum[],TComponent* Owner)
	: TForm(Owner)
{int i=0,j=0;
//// String *tempWeldPass;
// tempWeldPass=new String[5];
// tempWeldPass[0]="WeldPass1";
// tempWeldPass[1]="WeldPass2";
// tempWeldPass[2]="WeldPass3";
// tempWeldPass[3]="WeldPass4";
// tempWeldPass[4]="WeldPass5";
//
/*
 CheckListBox1->Items->Add(tempWeldPass[0]);
 CheckListBox1->Items->Add(tempWeldPass[1]);
 CheckListBox1->Items->Add(tempWeldPass[2]);
 CheckListBox1->Items->Add(tempWeldPass[3]);
 CheckListBox1->Items->Add(tempWeldPass[4]);
 CheckListBox1->Checked[0]=true;
 CheckListBox1->Checked[1]=true;
 CheckListBox1->Checked[2]=true;
 CheckListBox1->Checked[3]=true;
 CheckListBox1->Checked[4]=true;
 CheckListBox1->Enabled=false;
//
 CheckListBox2->Items->Add(tempWeldPass[0]);
 CheckListBox2->Items->Add(tempWeldPass[1]);
 CheckListBox2->Items->Add(tempWeldPass[2]);
 CheckListBox2->Items->Add(tempWeldPass[3]);
 CheckListBox2->Items->Add(tempWeldPass[4]);
 CheckListBox2->ItemIndex=0;
 CheckListBox2->Checked[0]=true;
 CheckListBox2->Checked[1]=true;
 CheckListBox2->Checked[2]=true;
 CheckListBox2->Checked[3]=true;
 CheckListBox2->Checked[4]=true;
 */
 /////////////////////////////////////////
 for(i=0;i<nWeldPass;i++) //Each order
   {
//    CheckListBox1->Items->Add(name[i]);CheckListBox1->Checked[i]=true;
//	CheckListBox2->Items->Add(name[i]);CheckListBox2->Checked[i]=true;
	for(j=0;j<nWeldPass;j++)if(seqNum[j]-1==i)break;   // Note that seqNum is numbered 1 to...
//	CheckListBox1->Items->Add(name[j]);CheckListBox1->Checked[j]=true;
//	CheckListBox2->Items->Add(name[j]);CheckListBox2->Checked[j]=true;
	CheckListBox1->Items->Add(name[j].t_str());CheckListBox1->Checked[j]=true; //Correction EFP 3/28/2011
	CheckListBox2->Items->Add(name[j].t_str());CheckListBox2->Checked[j]=true; //Correction EFP 3/28/2011
	localColor[i]=origColor[i]=WeldColor[j];
	localseqNum[i]=origseqNum[i]=i+1; //Directions unfinished
	invColor[i]=invorigColor[i]=WeldColor[j];
	localseqInv[i]=origseqInv[i]=j; //Directions unfinished
   }
 CheckListBox1->Enabled=false;CheckListBox2->ItemIndex=0;
 seqIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button1Click(TObject *Sender)
{Form1->WeldPassEditSeqn1_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button2Click(TObject *Sender)
{Form1->WeldPassEditSeqn2_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button5Click(TObject *Sender) //restore original directions (unfinished)
{for(long i=0;i<CheckListBox2->Items->Count;i++)CheckListBox2->Checked[i]=CheckListBox1->Checked[i];
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button3Click(TObject *Sender)//Move up
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count)
   {
//	swp=localseqNum[CheckListBox2->ItemIndex];
//	localseqNum[CheckListBox2->ItemIndex]=localseqNum[CheckListBox2->ItemIndex-1];
//	localseqNum[CheckListBox2->ItemIndex-1]=swp;
	swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex-1];
	localseqInv[CheckListBox2->ItemIndex-1]=swp;

//	swpColor=localColor[CheckListBox2->ItemIndex];
//	localColor[CheckListBox2->ItemIndex]=localColor[CheckListBox2->ItemIndex-1];
//	localColor[CheckListBox2->ItemIndex-1]=swpColor;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex-1];
	invColor[CheckListBox2->ItemIndex-1]=swpColor;

	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex-1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex-1;
//	ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button4Click(TObject *Sender)//Move down
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>=0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count-1)
   {
//	swp=localseqNum[CheckListBox2->ItemIndex];
//	localseqNum[CheckListBox2->ItemIndex]=localseqNum[CheckListBox2->ItemIndex+1];
//	localseqNum[CheckListBox2->ItemIndex+1]=swp;
//	swpColor=localColor[CheckListBox2->ItemIndex];
//	localColor[CheckListBox2->ItemIndex]=localColor[CheckListBox2->ItemIndex+1];
//	localColor[CheckListBox2->ItemIndex+1]=swpColor;
	swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex+1];
	localseqInv[CheckListBox2->ItemIndex+1]=swp;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex+1];
	invColor[CheckListBox2->ItemIndex+1]=swpColor;

	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex+1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex+1;
//	ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button6Click(TObject *Sender) //restore original seq
{//CheckListBox2->Clear();
//// for(long i=0;i<5;i++)CheckListBox2->Items->Add(tempWeldPass[i]);
// for(long i=0;i<CheckListBox1->Items->Count;i++)CheckListBox2->Items->Add(tempWeldPass[i]);
// for(long i=0;i<CheckListBox1->Items->Count;i++)CheckListBox2->Items->Add(CheckListBox1->Items->Value[i]);
//CheckListBox2->Assign(CheckListBox1);
// for(int i=0;i<CheckListBox1->Items->Count;i++)
//   {CheckListBox2->Items->Strings[i]=CheckListBox1->Items->Strings[i]; //TBD: allow for different incoming order
//	localColor[i]=origColor[i];
//	localseqNum[i]=origseqNum[i];
//   }
 for(int i=0;i<CheckListBox1->Items->Count;i++)
   {CheckListBox2->Items->Strings[i]=CheckListBox1->Items->Strings[i]; //TBD: allow for different incoming order
	invColor[i]=invorigColor[i];
	localseqInv[i]=origseqInv[i];
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm23::CheckListBox2Click(TObject *Sender)
{
////ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
// setWColor1(localColor[CheckListBox2->ItemIndex]);
 setWColor1(invColor[CheckListBox2->ItemIndex]);
}
//---------------------------------------------------------------------------
TColor TForm23::getWColor1(){return Shape1->Brush->Color;}
void TForm23::setWColor1(TColor s){Shape1->Brush->Color=s;}
//---------------------------------------------------------------------------
////int TForm23::getSeq(){return localseqNum[seqIndex];}
//int TForm23::getSeq(){return localseqInv[seqIndex]+1;}
int TForm23::getSeq(){return seqIndex+1;}
bool TForm23::getDir(){return CheckListBox2->Checked[seqIndex];}
//---------------------------------------------------------------------------
//void TForm23::setSeqIndex(int s){seqIndex=s;}
void TForm23::setSeqIndex(int s){long j=0;    //Receive WP# s and convert to associated Seq# j
//								 seqIndex=s;
	for(j=0;j<CheckListBox2->Items->Count;j++)if(localseqInv[j]==s){seqIndex=j;break;}   // Note that seqNum is numbered 1 to...
								}
//---------------------------------------------------------------------------

