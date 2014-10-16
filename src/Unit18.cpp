//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit18.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm18 *Form18;
int lb1_18=0,lb2_18=0,lb3_18=0;
//---------------------------------------------------------------------------
__fastcall TForm18::TForm18(float A1,float A2,float A3,TComponent* Owner) : TForm(Owner)
{int rec1=0,rec2=0,rec3=0;
 setFixAngle(A1,&rec1);lb1_18=rec1;
 setFixAngle(A2,&rec2);lb2_18=rec2;
 setFixAngle(A3,&rec3);lb3_18=rec3;
//
 ListBox1->Items->Add("-90");
 ListBox1->Items->Add("-45");
 ListBox1->Items->Add("-30");
 ListBox1->Items->Add("-10");
 ListBox1->Items->Add(" 10");
 ListBox1->Items->Add(" 30");
 ListBox1->Items->Add(" 45");
 ListBox1->Items->Add(" 90");
 ListBox1->ItemIndex=rec1;
//
 ListBox2->Items->Add("-90");
 ListBox2->Items->Add("-45");
 ListBox2->Items->Add("-30");
 ListBox2->Items->Add("-10");
 ListBox2->Items->Add(" 10");
 ListBox2->Items->Add(" 30");
 ListBox2->Items->Add(" 45");
 ListBox2->Items->Add(" 90");
 ListBox2->ItemIndex=rec2;
//
 ListBox3->Items->Add("-90");
 ListBox3->Items->Add("-45");
 ListBox3->Items->Add("-30");
 ListBox3->Items->Add("-10");
 ListBox3->Items->Add(" 10");
 ListBox3->Items->Add(" 30");
 ListBox3->Items->Add(" 45");
 ListBox3->Items->Add(" 90");
 ListBox3->ItemIndex=rec3;
}
//---------------------------------------------------------------------------
void __fastcall TForm18::ListBox1ClickExecute(TObject *Sender){lb1_18=ListBox1->ItemIndex;}
void __fastcall TForm18::ListBox2ClickExecute(TObject *Sender){lb2_18=ListBox2->ItemIndex;}
void __fastcall TForm18::ListBox3ClickExecute(TObject *Sender){lb3_18=ListBox3->ItemIndex;}
void __fastcall TForm18::Button1Click(TObject *Sender){Form1->rotheaFix_public();}
float TForm18::getFixAngle0(){float aval[8]={-90.,-45.,-30.,-10.,10.,30.,45.,90.};return aval[lb1_18];}
float TForm18::getFixAngle1(){float aval[8]={-90.,-45.,-30.,-10.,10.,30.,45.,90.};return aval[lb2_18];}
float TForm18::getFixAngle2(){float aval[8]={-90.,-45.,-30.,-10.,10.,30.,45.,90.};return aval[lb3_18];}
//---------------------------------------------------------------------------
void TForm18::setFixAngle(float s,int *rec)
{int i=0;float dist=0.,tdist=1.e10,aval[8]={-90.,-45.,-30.,-10.,10.,30.,45.,90.};
 for(i=0;i<8;i++){if(s-aval[i]<0)dist=aval[i]-s;else dist=s-aval[i];
                  if(tdist>dist){tdist=dist;*rec=i;}
                 }
}
//---------------------------------------------------------------------------
void __fastcall TForm18::Button2Click(TObject *Sender)
{Close();Form1->rotheaFixTRASH_public();
}
//---------------------------------------------------------------------------


