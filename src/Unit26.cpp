//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit26.h"
#include "Unit1.h"
//#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm26 *Form26;
//---------------------------------------------------------------------------
__fastcall TForm26::TForm26(int isel,TComponent* Owner) : TForm(Owner)
{F26_sel=isel;
}
//---------------------------------------------------------------------------
int TForm26::getItemIndex(){return ListBox1->ItemIndex;}
//---------------------------------------------------------------------------
int TForm26::getISEL(){return F26_sel;}
//---------------------------------------------------------------------------
void __fastcall TForm26::Button1Click(TObject *Sender)
{if(ListBox1->ItemIndex> -1 && ListBox1->ItemIndex <ListBox1->Items->Count)Form1->VFTlistbox_public();
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click an item",L"Selection unknown:",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm26::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

