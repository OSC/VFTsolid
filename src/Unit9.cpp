//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit9.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm9 *Form9;
//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner) : TForm(Owner){}
//---------------------------------------------------------------------------
//bool TForm9::getCheck(){return Checked;}
//---------------------------------------------------------------------------
void __fastcall TForm9::Button1Click(TObject *Sender)
{int ic=0,icount=0;
 icount=0;
 for(ic=0;ic<CheckListBox1->Count;ic++)if(CheckListBox1->Checked[ic])icount++;
 if(icount)Form1->exportWARP3D5_public();
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User must check at least one entity checkbox",L"Repeat",MB_OK);}
}
//---------------------------------------------------------------------------
