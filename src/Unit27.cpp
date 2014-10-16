//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit27.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm27 *Form27;
//---------------------------------------------------------------------------
__fastcall TForm27::TForm27(int isel,long nWP,String names[],TComponent* Owner) : TForm(Owner)
{ckIndex=0;selMode=isel;
// for(long i=0;i<nWP;i++){CheckListBox1->Items->Add(names[i]);CheckListBox1->Checked[i]=true;}
 for(long i=0;i<nWP;i++){CheckListBox1->Items->Add(names[i].t_str());CheckListBox1->Checked[i]=true;}
}
//---------------------------------------------------------------------------
long TForm27::getIndex(){return (long)CheckListBox1->Checked[ckIndex];}
void TForm27::setIndex(long s){ckIndex=s;}
long TForm27::getSel(){return selMode;}
//---------------------------------------------------------------------------
void __fastcall TForm27::Button1Click(TObject *Sender)
{
 Form1->SelStrings_public();
 Close();
}
//---------------------------------------------------------------------------
