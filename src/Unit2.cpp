//---------------------------------------------------------------------------
#include <vcl.h>

//#include <fstream.h>
//#include <math.h>
//#include <iomanip.h>

#pragma hdrstop
#include "Unit2.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
extern int iResSel;
// Policy: This form uses global int iResSel instead of __property
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(int aflag,TComponent* Owner) : TForm(Owner)
{int i=0;
 char mstr[48];
// wchar_t curMess0[]=L"Col# ",string1[11];
 if(aflag==1){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ux");
 ListBox1->Items->Add("Uy");
 ListBox1->Items->Add("Uz");
 ListBox1->Items->Add("Sxx");
 ListBox1->Items->Add("Syy");
 ListBox1->Items->Add("Szz");
 ListBox1->Items->Add("Sxy");
 ListBox1->Items->Add("Sxz");
 ListBox1->Items->Add("Syz");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");ListBox1->ItemIndex=17;
             }
 else if(aflag==2){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Uz");
 ListBox1->Items->Add("Srr");
 ListBox1->Items->Add("Stt");
 ListBox1->Items->Add("Szz*");
 ListBox1->Items->Add("Srt");
 ListBox1->Items->Add("Srz*");
 ListBox1->Items->Add("Stz*");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");ListBox1->ItemIndex=17;
             }
 else if(aflag==3){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Up");
 ListBox1->Items->Add("Srr");
 ListBox1->Items->Add("Stt");
 ListBox1->Items->Add("Spp");
 ListBox1->Items->Add("Srt");
 ListBox1->Items->Add("Srp");
 ListBox1->Items->Add("Stp");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");ListBox1->ItemIndex=17;
             }
 else if(aflag==4){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("U'x");
 ListBox1->Items->Add("U'y");
 ListBox1->Items->Add("U'z");
 ListBox1->Items->Add("S'xx");
 ListBox1->Items->Add("S'yy");
 ListBox1->Items->Add("S'zz");
 ListBox1->Items->Add("S'xy");
 ListBox1->Items->Add("S'xz");
 ListBox1->Items->Add("S'yz");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");ListBox1->ItemIndex=17;
             }
 else if(aflag==20){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ux");
 ListBox1->Items->Add("Uy");
 ListBox1->Items->Add("Uz");ListBox1->ItemIndex=3;
             }
 else if(aflag==21){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Uz");ListBox1->ItemIndex=1;
             }
 else if(aflag==22){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Up");ListBox1->ItemIndex=1;
             }
 else if(aflag==23){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("U'x");
 ListBox1->Items->Add("U'y");
 ListBox1->Items->Add("U'z");ListBox1->ItemIndex=1;
             }
 else if(aflag==30){ // Old CAP results format
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Time        ");
 ListBox1->Items->Add("Temperature ");
 ListBox1->Items->Add("Cooling rate");
 ListBox1->Items->Add("Liquid. time");
 ListBox1->Items->Add("Solidif.time");ListBox1->ItemIndex=2;
                  }
 else if(aflag==31){
 ListBox1->Items->Add("Close");
// ListBox1->Items->Add("Temperature ");ListBox1->ItemIndex=1; //EFP 5/12/2010
 ListBox1->Items->Add("T_max envelope ");ListBox1->ItemIndex=1;
				  }
 else if(aflag==32){ // New CAP results format
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Velocity Vx ");
 ListBox1->Items->Add("Velocity Vy ");
 ListBox1->Items->Add("Velocity Vz ");
 ListBox1->Items->Add("PRF         ");
 ListBox1->Items->Add("FL          ");
 ListBox1->Items->Add("Temperature ");ListBox1->ItemIndex=6;
                  }
 else if(aflag==33){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Velocity Vr ");
 ListBox1->Items->Add("Velocity Vt ");
 ListBox1->Items->Add("Velocity Vz ");
 ListBox1->Items->Add("PRF         ");
 ListBox1->Items->Add("FL          ");
 ListBox1->Items->Add("Temperature ");ListBox1->ItemIndex=6;
                  }
 else if(aflag==34){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Velocity Vr ");
 ListBox1->Items->Add("Velocity Vt ");
 ListBox1->Items->Add("Velocity Vp ");
 ListBox1->Items->Add("PRF         ");
 ListBox1->Items->Add("FL          ");
 ListBox1->Items->Add("Temperature ");ListBox1->ItemIndex=6;
                  }
 else if(aflag==35){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Velocity V'x");
 ListBox1->Items->Add("Velocity V'y");
 ListBox1->Items->Add("Velocity V'z");
 ListBox1->Items->Add("PRF         ");
 ListBox1->Items->Add("FL          ");
 ListBox1->Items->Add("Temperature ");ListBox1->ItemIndex=6;
                  }
 else if(aflag==41){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ux");
 ListBox1->Items->Add("Uy");
 ListBox1->Items->Add("Uz");
 ListBox1->Items->Add("Sxx");
 ListBox1->Items->Add("Syy");
 ListBox1->Items->Add("Szz");
 ListBox1->Items->Add("Sxy");
 ListBox1->Items->Add("Sxz");
 ListBox1->Items->Add("Syz");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");
 ListBox1->Items->Add("Tot.exx");
 ListBox1->Items->Add("Tot.eyy");
 ListBox1->Items->Add("Tot.ezz");
 ListBox1->Items->Add("Tot.exy");
 ListBox1->Items->Add("Tot.exz");
 ListBox1->Items->Add("Tot.eyz");
 ListBox1->Items->Add("Tot.eN1");
 ListBox1->Items->Add("Tot.eN2");
 ListBox1->Items->Add("Tot.eN3");
 ListBox1->Items->Add("Tot.eP1");
 ListBox1->Items->Add("Tot.eP2");
 ListBox1->Items->Add("Tot.eP3");
 ListBox1->Items->Add("e.Eff");ListBox1->ItemIndex=30;
             }
 else if(aflag==42){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Uz");
 ListBox1->Items->Add("Srr");
 ListBox1->Items->Add("Stt");
 ListBox1->Items->Add("Szz*");
 ListBox1->Items->Add("Srt");
 ListBox1->Items->Add("Srz*");
 ListBox1->Items->Add("Stz*");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");
 ListBox1->Items->Add("Tot.err");
 ListBox1->Items->Add("Tot.ett");
 ListBox1->Items->Add("Tot.ezz*");
 ListBox1->Items->Add("Tot.ert");
 ListBox1->Items->Add("Tot.erz*");
 ListBox1->Items->Add("Tot.etz*");
 ListBox1->Items->Add("Tot.eN1");
 ListBox1->Items->Add("Tot.eN2");
 ListBox1->Items->Add("Tot.eN3");
 ListBox1->Items->Add("Tot.eP1");
 ListBox1->Items->Add("Tot.eP2");
 ListBox1->Items->Add("Tot.eP3");
 ListBox1->Items->Add("e.Eff");ListBox1->ItemIndex=30;
             }
 else if(aflag==43){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("Ur");
 ListBox1->Items->Add("Ut");
 ListBox1->Items->Add("Up");
 ListBox1->Items->Add("Srr");
 ListBox1->Items->Add("Stt");
 ListBox1->Items->Add("Spp");
 ListBox1->Items->Add("Srt");
 ListBox1->Items->Add("Srp");
 ListBox1->Items->Add("Stp");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");
 ListBox1->Items->Add("Tot.err");
 ListBox1->Items->Add("Tot.ett");
 ListBox1->Items->Add("Tot.epp");
 ListBox1->Items->Add("Tot.ert");
 ListBox1->Items->Add("Tot.erp");
 ListBox1->Items->Add("Tot.etp");
 ListBox1->Items->Add("Tot.eN1");
 ListBox1->Items->Add("Tot.eN2");
 ListBox1->Items->Add("Tot.eN3");
 ListBox1->Items->Add("Tot.eP1");
 ListBox1->Items->Add("Tot.eP2");
 ListBox1->Items->Add("Tot.eP3");
 ListBox1->Items->Add("e.Eff");ListBox1->ItemIndex=30;
             }
 else if(aflag==44){
 ListBox1->Items->Add("Close");
 ListBox1->Items->Add("U'x");
 ListBox1->Items->Add("U'y");
 ListBox1->Items->Add("U'z");
 ListBox1->Items->Add("S'xx");
 ListBox1->Items->Add("S'yy");
 ListBox1->Items->Add("S'zz");
 ListBox1->Items->Add("S'xy");
 ListBox1->Items->Add("S'xz");
 ListBox1->Items->Add("S'yz");
 ListBox1->Items->Add("N1");
 ListBox1->Items->Add("N2");
 ListBox1->Items->Add("N3");
 ListBox1->Items->Add("P1");
 ListBox1->Items->Add("P2");
 ListBox1->Items->Add("P3");
 ListBox1->Items->Add("Tresca");
 ListBox1->Items->Add("vonMises");
 ListBox1->Items->Add("Tot.e'xx");
 ListBox1->Items->Add("Tot.e'yy");
 ListBox1->Items->Add("Tot.e'zz");
 ListBox1->Items->Add("Tot.e'xy");
 ListBox1->Items->Add("Tot.e'xz");
 ListBox1->Items->Add("Tot.e'yz");
 ListBox1->Items->Add("Tot.eN1");
 ListBox1->Items->Add("Tot.eN2");
 ListBox1->Items->Add("Tot.eN3");
 ListBox1->Items->Add("Tot.eP1");
 ListBox1->Items->Add("Tot.eP2");
 ListBox1->Items->Add("Tot.eP3");
 ListBox1->Items->Add("e.Eff");ListBox1->ItemIndex=30;
             }
 else if(aflag<0){ListBox1->Items->Add("Close");
				  for(i=0;i< -aflag;i++){
////										 strcpy(mstr,"Col# ");
////										 strcat(mstr,IntToStr(i+1).c_str());
////										 ListBox1->Items->Add(mstr);
//										 _ltow(i+1,string1,10);
//										 StringCchCatW(curMess0,11,string1);
//										 ListBox1->Items->Add(curMess0);

//										 strcpy(mstr,"Col# ");
										 StringCchCopy(mstr,40,"Col# ");
//										 strcat(mstr,IntToStr(i+1).t_str());
										 StringCchCat(mstr,40,IntToStr(i+1).t_str());
										 ListBox1->Items->Add(mstr);
										}
                  ListBox1->ItemIndex=1;  // Generic
                 }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported results type",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ListBox1ClickExecute(TObject *Sender)
{iResSel=ListBox1->ItemIndex -1;if(!ListBox1->ItemIndex)Close();Form1->CmPmContour_public();
}

/*
 wchar_t curMess0[]=L" Begin mesh nodes (not 1).",curMess1[]=L" Mesh/geometry nodes (surplus) ",
	  curMess2[]=L" Mesh/geometry nodes (INSUFFICIENT) ",curMess3[]=L" Geom nodes in/out of mesh ",
	  curMess4[]=L" Good/bad elements ",string0[11],string1[11];
// char durMess0[]=" Begin mesh nodes (not 1).",donga0[11];
 //
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
try {
 POSGP8[0]= -1./sqrt(3.);POSGP8[1]= -POSGP8[0];WEIGP8[0]=WEIGP8[1]=1.00000;nmin=LONG_INT;nmax= -1;
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;node=(matno[ie]-eltype*t7-bscode*t5)/t3;
                        for(in=0;in<node;in++){if(nmax<nop1[MXNPEL*ie+in])nmax=nop1[MXNPEL*ie+in];
                                               if(nmin>nop1[MXNPEL*ie+in])nmin=nop1[MXNPEL*ie+in];
                                              }
                       }
 honk<<nmin+1<<" Max/min nodes found in mesh "<<nmax+1<<" vs #nodes in geometry "<<npoin<<"\n";
 if(nmin){
////		  ltoa(nmin+1,string0,10);strcat(string0,curMess0);
////		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Warning",MB_OK);
//		  StringCbCopy(donga0,11,"dingo");StringCbCat(donga0,11,durMess0);
//		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(_TCHAR(*donga0),L"Warning",MB_OK);
		  _ltow(nmin+1,string0,10);
		  StringCchCatW(string0,11,curMess0);
		  extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(string0,L"Warning",MB_OK);
		 }
 if(nmax+1<npoin){
//				  ltoa(nmax+1,string0,10);ltoa(npoin,string1,10);strcat(string0,curMess1);strcat(string0,string1);
//				  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Warning",MB_OK);
				  _ltow(nmax+1,string0,10);
				  _ltow(npoin,string1,10);
				  StringCchCatW(string0,11,curMess1);
				  StringCchCatW(string0,11,string1);
				  extern PACKAGE void __fastcall Beep(void);
				  Application->MessageBox(string0,L"Warning",MB_OK);
				 }
				 */
