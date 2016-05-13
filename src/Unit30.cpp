//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit30.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm30 *Form30;
//---------------------------------------------------------------------------
__fastcall TForm30::TForm30(int isel,long nWeldPass,const String name[],const TColor WeldColor[],const int seqNum[],const long reset[],TComponent* Owner) : TForm(Owner)
{long i=0,j=0;local_isel=isel;
 for(i=0;i<nWeldPass;i++) //Each order
   {for(j=0;j<nWeldPass;j++)if(seqNum[j]-1==i)break;   // Note that seqNum is numbered 1 to...
//	CheckListBox1->Items->Add(name[j].w_str());
	ListBox1->Items->Add(name[j].w_str());
	if(local_isel)CheckListBox2->Items->Add(IntToStr(seqNum[i]).w_str());
	else          CheckListBox2->Items->Add(name[j].w_str());
	if(reset[j]-10*(reset[j]/10))CheckListBox2->Checked[j]=true;
	invColor[i]=invorigColor[i]=WeldColor[j];localseqInv[i]=origseqInv[i]=j;
   }
 if(local_isel)ListBox1->ItemIndex=0;
 CheckListBox2->ItemIndex=0;seqIndex=0;
}
////---------------------------------------------------------------------------
//long TForm30::getEdit1(){
////                           if(input_checker(0,Edit23->Text))CheckEdit23=StrToInt(Edit23->Text);
////						   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter integer number",L"Halt",MB_OK);
//// 								 CheckEdit23=1;
////								}
////						   return CheckEdit23;
//						   return StrToInt(Edit1->Text);
//						  }
////---------------------------------------------------------------------------
//void TForm30::setEdit1(long x){Edit1->Text=IntToStr(__int64(x));}
//---------------------------------------------------------------------------
long TForm30::getEdit1(){return StrToInt(Edit1->Text);}
void TForm30::setEdit1(long x){Edit1->Text=IntToStr(__int64(x));}
//---------------------------------------------------------------------------
int TForm30::getISEL(){return local_isel;}
void TForm30::setISEL(int x){local_isel=x;}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button1Click(TObject *Sender) //Remember that seq (1,...)
{if(local_isel){long in=0,ip=0,sum=0,lastabsent=0,lastduplicate=0,*occur=NULL;occur=new long[CheckListBox2->Count];
				for(in=0;in<CheckListBox2->Count;in++)
				  {sum=0;for(ip=0;ip<CheckListBox2->Count;ip++)if(in+1== StrToInt(CheckListBox2->Items->Strings[ip]))sum++;
				   occur[in]=sum;
				  }
				lastabsent=lastduplicate=0;
				for(in=0;in<CheckListBox2->Count;in++){if(occur[in]==0)lastabsent=in+1;
													   else if(occur[in]>1)lastduplicate=in+1;
													  }
				delete [] occur;
				if(lastabsent==0 && lastduplicate==0)Form1->WeldPassEditSeqn4_public();
				else if(lastabsent!=0 && lastduplicate!=0){extern PACKAGE void __fastcall Beep(void);
														   ShowMessage(L"Repeat: Order# "+IntToStr(__int64(lastabsent))+L" missing but Order# "+IntToStr(__int64(lastduplicate))+L" duplicated.");
														  }
				else if(lastabsent!=0){extern PACKAGE void __fastcall Beep(void);ShowMessage(L"Repeat: Order# "+IntToStr(__int64(lastabsent))+L" missing.");}
				else if(lastduplicate!=0){extern PACKAGE void __fastcall Beep(void);ShowMessage(L"Repeat: Order# "+IntToStr(__int64(lastduplicate))+L" duplicated.");}
			   }
 else Form1->WeldPassEditSeqn2_public();
} //Enter
////---------------------------------------------------------------------------
//void __fastcall TForm30::Button2Click(TObject *Sender)//Animate seq
////{Form1->WeldPassEditSeqn2_public();
//{try {if(StrToInt(Edit1->Text)>0)Form1->WeldPassEditSeqn1_public();
//	  else {extern PACKAGE void __fastcall Beep(void);ShowMessage(Label4->Caption +L" "+ Edit1->Text + L" must be positive");}
//	 } //CAUTION: The user might have to click "Continue" after the following message.
// catch (EConvertError &E){extern PACKAGE void __fastcall Beep(void);ShowMessage(Label4->Caption +L" "+ Edit1->Text + L" must be positive integer");}
//}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button3Click(TObject *Sender)//Move up
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count)
   {swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex-1];
	localseqInv[CheckListBox2->ItemIndex-1]=swp;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex-1];
	invColor[CheckListBox2->ItemIndex-1]=swpColor;
	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex-1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex-1;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click a lower list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button4Click(TObject *Sender)//Move down
{int swp=0;TColor swpColor;
 if(CheckListBox2->ItemIndex>=0 && CheckListBox2->ItemIndex<CheckListBox2->Items->Count-1)
   {swp=localseqInv[CheckListBox2->ItemIndex];
	localseqInv[CheckListBox2->ItemIndex]=localseqInv[CheckListBox2->ItemIndex+1];
	localseqInv[CheckListBox2->ItemIndex+1]=swp;
	swpColor=invColor[CheckListBox2->ItemIndex];
	invColor[CheckListBox2->ItemIndex]=invColor[CheckListBox2->ItemIndex+1];
	invColor[CheckListBox2->ItemIndex+1]=swpColor;
	CheckListBox2->Items->Move(CheckListBox2->ItemIndex,CheckListBox2->ItemIndex+1);
	CheckListBox2->ItemIndex=CheckListBox2->ItemIndex+1;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must click an upper list item",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button5Click(TObject *Sender)//restore current directions EFP 3/29/2012
//{long i=0,j=0;
// for(i=0;i<CheckListBox2->Items->Count;i++)
//   {for(j=0;j<CheckListBox1->Items->Count;j++)
//	   if(localseqInv[i]==origseqInv[j]){CheckListBox2->Checked[i]=CheckListBox1->Checked[j];
//										 break;
//										}
//   }
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Inactive feature",L"Failure",MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button6Click(TObject *Sender)//restore current seq+dir
//{for(int i=0;i<CheckListBox1->Items->Count;i++)
//   {CheckListBox2->Items->Strings[i]=CheckListBox1->Items->Strings[i]; //TBD: allow for different incoming order
//	CheckListBox2->Checked[i]=CheckListBox1->Checked[i];
//	invColor[i]=invorigColor[i];localseqInv[i]=origseqInv[i];
//   }
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Inactive feature",L"Failure",MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button7Click(TObject *Sender)//Reverse all directions EFP 12/31/2011
{if(local_isel){CheckListBox2->Items->Delete(local_index);
				CheckListBox2->Items->Insert(local_index,Edit1->Text);
			   }
 else for(long i=0;i<CheckListBox2->Items->Count;i++)CheckListBox2->Checked[i]=1-CheckListBox2->Checked[i];
}
//---------------------------------------------------------------------------
void __fastcall TForm30::CheckListBox2Click(TObject *Sender)
{if(local_isel){
if(CheckListBox2->Checked[CheckListBox2->ItemIndex]==false){extern PACKAGE void __fastcall Beep(void);
															Application->MessageBox(L"Must click precisely on checkbox to reverse direction.",L"Warning",MB_OK);
														   }
			   }
 else {if(CheckListBox2->ItemIndex> -1 && CheckListBox2->ItemIndex <CheckListBox2->Items->Count)setWColor1(invColor[CheckListBox2->ItemIndex]);
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click right checklistbox item",L"Selection unknown",MB_OK);}
	  }
}
//---------------------------------------------------------------------------
TColor TForm30::getWColor1(){return Shape1->Brush->Color;}
void TForm30::setWColor1(TColor s){Shape1->Brush->Color=s;}
//---------------------------------------------------------------------------
int TForm30::getSeq(){if(local_isel)return StrToInt(CheckListBox2->Items->Strings[seqIndex]);
					  else return seqIndex+1;
					 }
bool TForm30::getDir(){return CheckListBox2->Checked[seqIndex];}
//---------------------------------------------------------------------------
//void TForm23::setSeqIndex(int s){seqIndex=s;}
void TForm30::setSeqIndex(int s){long j=0;    //Receive WP# s and convert to associated Seq# j
//								 seqIndex=s;
	for(j=0;j<CheckListBox2->Items->Count;j++)if(localseqInv[j]==s){seqIndex=j;break;}   // Note that seqNum is numbered 1 to...
								}
//---------------------------------------------------------------------------
void __fastcall TForm30::Button8Click(TObject *Sender){Form1->WeldPassEditSeqn3_public();}
////---------------------------------------------------------------------------
//void __fastcall TForm30::CheckListBox1Click(TObject *Sender)
//{if(local_isel){if(CheckListBox1->ItemIndex> -1 && CheckListBox1->ItemIndex <CheckListBox1->Items->Count)
//				  {CheckListBox2->ItemIndex=local_index=CheckListBox1->ItemIndex;
//				   setWColor1(invColor[CheckListBox1->ItemIndex]);
//				  }
//				else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click left checklistbox item",L"Selection unknown",MB_OK);}
//			   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click on other checklistbox",L"Halt",MB_OK);}
//}
//---------------------------------------------------------------------------
void __fastcall TForm30::ListBox1Click(TObject *Sender)
{if(local_isel){if(ListBox1->ItemIndex> -1 && ListBox1->ItemIndex <ListBox1->Items->Count)
				  {CheckListBox2->ItemIndex=local_index=ListBox1->ItemIndex;
				   setWColor1(invColor[ListBox1->ItemIndex]);
				  }
				else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click left listbox item",L"Selection unknown",MB_OK);}
			   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Click on right checklistbox",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------

