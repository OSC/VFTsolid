//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(long npoin,long nelt,long nWeldGroup,long nWeldPass,TComponent* Owner) : TForm(Owner)
{Edit1->Text=FloatToStr(float(npoin));Edit2->Text=FloatToStr(float(nelt));
 Edit3->Text=FloatToStr(float(nWeldGroup));Edit4->Text=FloatToStr(float(nWeldPass));
}
//---------------------------------------------------------------------------
