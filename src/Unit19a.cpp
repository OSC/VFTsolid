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
__fastcall TForm19::TForm19(int mess_isel,TComponent* Owner)
	: TForm(Owner)
{isel=mess_isel;
}
//---------------------------------------------------------------------------
String TForm19::getEdit1(){return Edit1->Text;}
String TForm19::getEdit2(){return Edit2->Text;}
String TForm19::getEdit3(){return Edit3->Text;}
String TForm19::getEdit4(){return Edit4->Text;}
String TForm19::getEdit5(){return Edit5->Text;}
String TForm19::getEdit6(){return Edit6->Text;}
void TForm19::setEdit1(String s){Edit1->Text=s;}
void TForm19::setEdit2(String s){Edit2->Text=s;}
void TForm19::setEdit3(String s){Edit3->Text=s;}
void TForm19::setEdit4(String s){Edit4->Text=s;}
void TForm19::setEdit5(String s){Edit5->Text=s;}
void TForm19::setEdit6(String s){Edit6->Text=s;}
//---------------------------------------------------------------------------
int TForm19::getIsel(){return isel;}
void TForm19::setIsel(int s){isel=s;}
//---------------------------------------------------------------------------
void __fastcall TForm19::Button1Click(TObject *Sender)
{
 if(isel==3){Form1->exportCTSP1_public();
			 Close(); //Emergency Close() EFP 4/19/2010
			}
 else if(isel==2){Form1->exportCTSP_public();
//			 Close(); //Emergency Close() EFP 4/19/2010
			}
 else if(isel==1){Form1->exportABAQUS1_public();
				  Close(); //Emergency Close() EFP 4/19/2010
				 }
 else    Form1->exportABAQUS_public();
}
//---------------------------------------------------------------------------
