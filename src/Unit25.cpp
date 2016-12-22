//---------------------------------------------------------------------------
// VFTmisc Form25 == VFTgen Form19
#include <vcl.h>
#pragma hdrstop

#include "Unit25.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm25 *Form25;
//---------------------------------------------------------------------------
__fastcall TForm25::TForm25(int mess_isel,TComponent* Owner) : TForm(Owner)
{isel=mess_isel;
}
//---------------------------------------------------------------------------
String TForm25::getEdit1(){return Edit1->Text;}
String TForm25::getEdit2(){return Edit2->Text;}
String TForm25::getEdit3(){return Edit3->Text;}
String TForm25::getEdit4(){return Edit4->Text;}
String TForm25::getEdit5(){return Edit5->Text;}
String TForm25::getEdit6(){return Edit6->Text;}
void TForm25::setEdit1(String s){Edit1->Text=s;}
void TForm25::setEdit2(String s){Edit2->Text=s;}
void TForm25::setEdit3(String s){Edit3->Text=s;}
void TForm25::setEdit4(String s){Edit4->Text=s;}
void TForm25::setEdit5(String s){Edit5->Text=s;}
void TForm25::setEdit6(String s){Edit6->Text=s;}
//---------------------------------------------------------------------------
int TForm25::getIsel(){return isel;}
void TForm25::setIsel(int s){isel=s;}
//---------------------------------------------------------------------------
void __fastcall TForm25::Button1Click(TObject *Sender)
{long isw=1;
// if(isel==3){Form1->exportCTSP1_public();Close();} //Emergency Close() EFP 4/19/2010
// else if(isel==2)Form1->exportCTSP_public();
// else if(isel==1){Form1->exportABAQUS1_public();Close();} //Emergency Close() EFP 4/19/2010

//// if(isel==3){Form1->exportWARP3D1_public();Close();} //Emergency Close() EFP 12/16/2012
// if(isel==3){Form1->exportWARP3D1a_public();Close();} //Emergency Close() EFP 12/16/2012
//***********************
 int buffersize=WideCharToMultiByte(CP_UTF8,0,Edit1->Text.w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,Edit1->Text.w_str(), -1,m,buffersize,NULL,NULL);
 if(strchr(m,' ') != NULL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit1->Text.w_str(),L"Repeat: inadmissible space\n in file name",MB_OK);}
 else if(strchr(m,'*') != NULL){isw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit1->Text.w_str(),L"Repeat: Choose file name",MB_OK);}
 delete m;m=NULL;
//***********************
 if(isw==1){
 if(isel==3){Form1->exportWARP3D1a_public();Close();} //Emergency Close() EFP 12/16/2012
 else if(isel==2){Form1->exportWARP3D_public();
//                  Close();
				 } //EFP 12/16/2012
// else if(isel==1){Form1->exportABAQUS1_public();Close();} //Emergency Close() EFP 4/19/2010
// else     Form1->exportABAQUS_public();
 else if(isel==5){Form1->exportWARP3D2a_public();Close();} //Emergency Close() EFP 12/16/2012
 else if(isel==4)Form1->exportWARP3Dcut_public();
		   }
}
//---------------------------------------------------------------------------
//void __fastcall TForm25::Button2Click(TObject *Sender){Close();}
//---------------------------------------------------------------------------

