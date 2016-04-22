//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(int numlist,TComponent* Owner) : TForm(Owner)
{nlist=numlist;iClickOrder=1;
}
//---------------------------------------------------------------------------
void __fastcall TForm7::Button1Click(TObject *Sender)
{int isw=1,i=0;
 for(i=0;i<nlist;i++){//ListBox2->ItemIndex=i;
					  if(ListBox2->Items->Strings[i]==L"****")isw=0;
					 }
 if(isw){Form1->exportWARP4_public();Close();}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"There is an unassigned entity.",L"Incomplete",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ListBox1Click(TObject *Sender)
{if(ListBox1->ItemIndex> -1 && ListBox1->ItemIndex< ListBox1->Items->Count){LB1ItemIndex=ListBox1->ItemIndex;iClickOrder=1;}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click again",L"Selection unknown",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ListBox3Click(TObject *Sender)
{if(ListBox3->ItemIndex> -1 && ListBox3->ItemIndex< ListBox3->Items->Count){
 if(iClickOrder)
   {iClickOrder=0;
	ListBox2->ItemIndex=LB1ItemIndex;
////ListBox2->AddItem(ListBox3->Items(ListBox3->ItemIndex,this));
//ListBox2->AddItem(ListBox3->Items,this);
	ListBox2->Items->Strings[ListBox2->ItemIndex]=ListBox3->Items->Strings[ListBox3->ItemIndex];
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First click on Entity; then click Avaliable Material.",L"Incorrect",MB_OK);}
                                                                           }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click again",L"Selection unknown",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm7::ListBox2Click(TObject *Sender)
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First click on Entity; then click Avaliable Material (Can not click Chosen material)",L"Repeat",MB_OK);
}
//---------------------------------------------------------------------------
int TForm7::getNlist(){return nlist;}
//---------------------------------------------------------------------------
void TForm7::setLB2ItemIndex(int s){ListBox2->ItemIndex=s;}
//---------------------------------------------------------------------------
String TForm7::getLB2Item(){return ListBox2->Items->Strings[ListBox2->ItemIndex];}
//---------------------------------------------------------------------------

