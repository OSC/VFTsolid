//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
#include "Unit21.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner) : TForm(Owner){}
//---------------------------------------------------------------------------
int TForm5::getItemIndex(){return ListBox1->ItemIndex;}
//---------------------------------------------------------------------------
void __fastcall TForm5::ListBox1Click(TObject *Sender)
{if(ListBox1->ItemIndex> -1 && ListBox1->ItemIndex <ListBox1->Items->Count)Form21->getItemIndex5_public();
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click on material file",L"Selection unknown:",MB_OK);}
}
//---------------------------------------------------------------------------
