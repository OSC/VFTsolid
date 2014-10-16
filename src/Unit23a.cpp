//---------------------------------------------------------------------------
//Formerly VFTgen Unit 18
#include <vcl.h>
#pragma hdrstop

#include "Unit23.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm23 *Form23;
//---------------------------------------------------------------------------
__fastcall TForm23::TForm23(long nWeldPass,String name[],TComponent* Owner)
	: TForm(Owner)
{
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
 for(int i=0;i<nWeldPass;i++)
   {CheckListBox1->Items->Add(name[i]);CheckListBox1->Checked[i]=true;
	CheckListBox2->Items->Add(name[i]);CheckListBox2->Checked[i]=true;
   }
 CheckListBox1->Enabled=false;CheckListBox2->ItemIndex=0;
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
void __fastcall TForm23::Button5Click(TObject *Sender)
{for(long i=0;i<CheckListBox2->Items->Count;i++)CheckListBox2->Checked[i]=CheckListBox1->Checked[i];
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button3Click(TObject *Sender)//Move up
{if(CheckListBox2->ItemIndex>0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count)
   {CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex-1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex-1;
	ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button4Click(TObject *Sender)//Move down
{if(CheckListBox2->ItemIndex>=0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count-1)
   {CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex+1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex+1;
	ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm23::Button6Click(TObject *Sender)
{//CheckListBox2->Clear();
//// for(long i=0;i<5;i++)CheckListBox2->Items->Add(tempWeldPass[i]);
// for(long i=0;i<CheckListBox1->Items->Count;i++)CheckListBox2->Items->Add(tempWeldPass[i]);
// for(long i=0;i<CheckListBox1->Items->Count;i++)CheckListBox2->Items->Add(CheckListBox1->Items->Value[i]);
//CheckListBox2->Assign(CheckListBox1);
 for(int i=0;i<CheckListBox1->Items->Count;i++)CheckListBox2->Items->Strings[i]=CheckListBox1->Items->Strings[i];
}
//---------------------------------------------------------------------------
void __fastcall TForm23::CheckListBox2Click(TObject *Sender)
{ColorListBox1->ItemIndex=CheckListBox2->ItemIndex;
}
//---------------------------------------------------------------------------

