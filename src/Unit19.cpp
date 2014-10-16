//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit19.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm19 *Form19;
//---------------------------------------------------------------------------
__fastcall TForm19::TForm19(long npoin,long nelt,TComponent* Owner) : TForm(Owner)
{Edit1->Text=FloatToStr(float(npoin));Edit2->Text=FloatToStr(float(nelt));
 Edit3->Text="****";Edit4->Text="****";Edit5->Text="****";
 Label1->Caption="****";Label4->Caption="****";Label5->Caption="****";Label6->Caption="****";Label7->Caption="****";
}
//---------------------------------------------------------------------------
void TForm19::setAnalProgStr(char s0[],char s1[],char s2[],char s3[],char s4[])
{Label1->Caption=s0;Label4->Caption=s1;Label5->Caption=s2;Label6->Caption=s3;Label7->Caption=s4;
}
//---------------------------------------------------------------------------
void TForm19::setAnalProgLong(long s0,long s1,long s2)
{Edit3->Text=FloatToStr(float(s0));Edit4->Text=FloatToStr(float(s1));Edit5->Text=FloatToStr(float(s2));
}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{Close();Form1->Form19TRASH_public();
}
//---------------------------------------------------------------------------


