//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit20.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm20 *Form20;
//---------------------------------------------------------------------------
__fastcall TForm20::TForm20(TComponent* Owner) : TForm(Owner)
{curMax=Edit1->Text;
}
//---------------------------------------------------------------------------
void __fastcall TForm20::Button1Click(TObject *Sender)
{curMax=Edit1->Text;Close();Form1->annot_public();
}
//---------------------------------------------------------------------------
AnsiString TForm20::getDMax(){return curMax;}
//---------------------------------------------------------------------------

