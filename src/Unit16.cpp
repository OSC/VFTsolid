//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit16.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm16 *Form16;
//---------------------------------------------------------------------------
__fastcall TForm16::TForm16(long npoin,long nelt,long nWeldGroup,long nWeldPass,TComponent* Owner) : TForm(Owner)
{Edit1->Text=FloatToStr(float(npoin));Edit2->Text=FloatToStr(float(nelt));
 Edit3->Text=FloatToStr(float(nWeldGroup));Edit4->Text=FloatToStr(float(nWeldPass));
}
//---------------------------------------------------------------------------
void __fastcall TForm16::Button1Click(TObject *Sender){Close();}
//---------------------------------------------------------------------------

