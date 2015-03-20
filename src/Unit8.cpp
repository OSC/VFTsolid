//---------------------------------------------------------------------------
// Multiuse TForm8(fsw...) Select 0->ShapeForm (Modeless), 1->ScaleForm (Modal),
//                                2->tshiftCTSP (Modal)
//                                3->tdeltCTSP (Modal)
//                                4->tshiftVED (Modal)
//                                5->tshiftCTSP+VED (Modal)
//                                6->epWARP3d steps (Modal)   //Not used anymore
//                                7->merge_tshiftCTSP (Modal)
//                                8->merge_tshiftVED (Modal)
//                                9->merge_tshiftCTSP+VED (Modal)
#include <vcl.h>
#pragma hdrstop
#include "Unit8.h"
#include "Unit1.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm8 *Form8;
//---------------------------------------------------------------------------
__fastcall TForm8::TForm8(int fsw,float Angle0,TComponent* Owner) : TForm(Owner){isw=fsw;setAngle0(Angle0);ac1=Angle0;}
//---------------------------------------------------------------------------
void TForm8::setAngle0(float s){Edit1->Text=FloatToStr(s);}
float TForm8::getAngle0(){return ac1;}
//---------------------------------------------------------------------------
void TForm8::setISW(int s){isw=s;}
int TForm8::getISW(){return isw;}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button1Click(TObject *Sender)
{
 try {StrToFloat(Edit1->Text);
	  if(Edit1->Text=="0")ac1=0.;
	  else ac1=StrToFloat(Edit1->Text);  //Necessary since Edit ignores 0
//	  if(isw==1){Close();Form1->res_scalehea_public();}
	  if(isw==2 || isw==4 || isw==5){Close();Form1->tshiftCTSP_public();}
//	  else if(isw==3){Close();Form1->tdeltCTSP_public();}
	  else if(isw==7 || isw==8 || isw==9){Close();Form1->tshiftCTSP3_public();} //EFP 5/09/2013
//	  else Form1->defshapehea_public();
	 }
 catch (const EConvertError &E){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(Edit1->Text.w_str(),L"Reenter float",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button2Click(TObject *Sender){
//													  if(isw==1){Close();Form1->res_scalehea1_public();}
													  if(isw==2 || isw==4 || isw==5 ||
															  isw==7 || isw==8 || isw==9){//Close();
																   Form1->tshiftCTSP1_public();
																  }
//													  else if(isw){//Close();
//																   Form1->tdeltCTSP1_public();
//																  }
//													  else Form1->defshapehea1_public();
													  }
//---------------------------------------------------------------------------
//// The following is only used for *ShapeForm (Modeless)
//void __fastcall TForm8::Button3Click(TObject *Sender)
//{if(!isw){Close();Form1->Form8TRASH_public();} // Modeless (isw=1 -> Modal)
//}
//---------------------------------------------------------------------------

