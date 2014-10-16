//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit26.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm26 *Form26;
//---------------------------------------------------------------------------
__fastcall TForm26::TForm26(int isel,TComponent* Owner) : TForm(Owner)
{F26_sel=isel;
}
//---------------------------------------------------------------------------
void __fastcall TForm26::ListBox1Click(TObject *Sender)
{
// Form1->VFTlistbox_public();Close();
// if(F26_isel==1)Form1->VFTlistbox_public();Close();
 Form1->VFTlistbox_public();
// Close();
}
//---------------------------------------------------------------------------
int TForm26::getItemIndex(){return ListBox1->ItemIndex;}
//---------------------------------------------------------------------------
int TForm26::getISEL(){return F26_sel;}
//---------------------------------------------------------------------------
