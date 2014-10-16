//---------------------------------------------------------------------------
#include <vcl.h>
//#include <math.h>
#pragma hdrstop
#include "Unit17.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm17 *Form17;
// TBD: problem with conflicting non-modal global "multiForm" numberGIDmax1, XXX ????
// Base 16 used because of problems with unsigned long (i.e. base 32)
int rsel_f17=0;long numberGIDmax17=0,XXX17=0;
//---------------------------------------------------------------------------
__fastcall TForm17::TForm17(int isel,long nGIDmax1,long saflag[],TComponent* Owner) : TForm(Owner)
{long i=0;numberGIDmax17=nGIDmax1;rsel_f17=isel;
 for(i=1;i<numberGIDmax17+1;i++)CheckListBox1->Items->Add(IntToStr(INT64(i)));
 setGIDCheckInfo(saflag);for(i=0;i<numberGIDmax17;i++)if(CheckListBox1->Checked[i]==true)CheckListBox1->ItemEnabled[i]=true;else CheckListBox1->ItemEnabled[i]=false;
}
//---------------------------------------------------------------------------
void TForm17::setGIDCheckInfo(long s[])
{long i=0,is=0,cur=0,prod=1,aflag=0,GIDspac=1+(numberGIDmax17-1)/16,mint=0;
 for(is=GIDspac-1;is> -1;is--)
   {if(16*is<numberGIDmax17)
	  {prod=1;aflag=0;cur=s[is];
	   if(numberGIDmax17<16*(is+1))mint=numberGIDmax17;else mint=16*(is+1);
	   if(numberGIDmax17>1){
							for(i=0;i<mint-16*is;i++){aflag=aflag+prod;prod=2*prod;}
//	   if(numberGIDmax17>1){for(i=0;i<min(numberGIDmax17,16*(is+1))-16*is;i++){aflag=aflag+prod;prod=2*prod;}
						   prod=prod/2;
						  }
	   for(i=mint-16*is;i>0;i--)
//	   for(i=min(numberGIDmax17,16*(is+1))-16*is;i>0;i--)
		  {aflag=aflag-prod;if(cur>aflag){cur=cur-prod;CheckListBox1->Checked[i+16*is-1]=true;}else {CheckListBox1->Checked[i+16*is-1]=false;}
		   prod=prod/2;
		  }
	  }
   }
}
//---------------------------------------------------------------------------
void TForm17::setXXXCheckOX(long x){XXX17=x;}
////---------------------------------------------------------------------------
//long TForm17::getXXXCheckOX(){long i=0,prod=1,cur=0;  // Global XXX17,numberGIDmax17
//                              if(16*XXX17<numberGIDmax17)for(i=16*XXX17;i<min(numberGIDmax17,16*(XXX17+1));i++){if(CheckListBox1->Checked[i]==true)cur=cur+prod;prod=prod*2;}
//                              return cur;
//                             }
//---------------------------------------------------------------------------
long TForm17::getXXXCheckOX(){long i=0,prod=1,cur=0,mint=0;  // Global XXX17,numberGIDmax17
							  if(numberGIDmax17<16*(XXX17+1))mint=numberGIDmax17;else mint=16*(XXX17+1);
////                              if(XXX17> -1 && XXX17<numberGIDmax17)if(CheckListBox1->Checked[XXX17]==true)cur=1;
							  if(XXX17> -1 && 16*XXX17<numberGIDmax17)for(i=16*XXX17;i<mint;i++){if(CheckListBox1->Checked[i]==true)cur=cur+prod;prod=prod*2;}
//							  if(XXX17> -1 && 16*XXX17<numberGIDmax17)for(i=16*XXX17;i<min(numberGIDmax17,16*(XXX17+1));i++){if(CheckListBox1->Checked[i]==true)cur=cur+prod;prod=prod*2;}
							  return cur;
							 }
//---------------------------------------------------------------------------
void __fastcall TForm17::Button1Click(TObject *Sender){
//													   if(rsel_f17==3){Form1->EditWeldPass_public();Close();}
//													   else
													   if(rsel_f17==2)Form1->GIDRestore_public();
													   else if(rsel_f17==1){
//																			Form1->WriteResultbyCol_public();
																			Close();
																		   }
													   else {
//															 Form1->WriteUNVselGID_public();
															 Close();
															}
													  }
//---------------------------------------------------------------------------
void __fastcall TForm17::Button2Click(TObject *Sender)
{
if(rsel_f17==2){ //Temporary fix in case user presses CLOSE button for "ShowModal"

Close();Form1->SelForm17TRASH_public();

			   }
}
//---------------------------------------------------------------------------



