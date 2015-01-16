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
void __fastcall TForm5::ListBox1Click(TObject *Sender){Form21->getItemIndex5_public();}
//---------------------------------------------------------------------------
