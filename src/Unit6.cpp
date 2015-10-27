//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//long gForm6=0;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(long eprec,long sprec,long GIDprec,
     float X,float Y,float Z,float Nx,float Ny,float Nz,
     int ResValid,float Value,TComponent* Owner) : TForm(Owner)
{gForm6=eprec;
 setElemQueryNum(eprec);setElemQuerySide(sprec);setElemQueryGID(GIDprec);
 setElemQueryXCor(X);setElemQueryYCor(Y);setElemQueryZCor(Z);
 setElemQueryNx(Nx);setElemQueryNy(Ny);setElemQueryNz(Nz);
 setElemQueryValue(ResValid,Value);
}
//---------------------------------------------------------------------------
void TForm6::setElemQueryNum(long s){gForm6=s;
//                                     if(gForm6>0)Edit1->Text=IntToStr(s);
									 if(gForm6>0)Edit1->Text=IntToStr(__int64(s));
									 else Edit1->Text=L"Not found";
									}
void TForm6::setElemQuerySide(long s){
//									  if(gForm6>0)Edit2->Text=IntToStr(s);
									  if(gForm6>0)Edit2->Text=IntToStr(__int64(s));
									  else Edit2->Text=L"******";
									 }
void TForm6::setElemQueryGID(long s){
//									 if(gForm6>0)Edit3->Text=IntToStr(s);
									 if(gForm6>0)Edit3->Text=IntToStr(__int64(s));
									 else Edit3->Text=L"******";
									}

void TForm6::setElemQueryXCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit4->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit4->Text=FloatToStr(s);
 else Edit4->Text=L"******";
}
void TForm6::setElemQueryYCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit5->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit5->Text=FloatToStr(s);
 else Edit5->Text=L"******";
}
void TForm6::setElemQueryZCor(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit6->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit6->Text=FloatToStr(s);
 else Edit6->Text=L"******";
}
void TForm6::setElemQueryNx(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit7->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit7->Text=FloatToStr(s);
 else Edit7->Text=L"******";
}
void TForm6::setElemQueryNy(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit8->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit8->Text=FloatToStr(s);
 else Edit8->Text=L"******";
}
void TForm6::setElemQueryNz(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit9->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit9->Text=FloatToStr(s);
 else Edit9->Text=L"******";
}
void TForm6::setElemQueryValue(int valid,float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(valid && gForm6>0)Edit10->Text=FloatToStrF(s,ffGeneral,15,5);
 if(valid && gForm6>0)Edit10->Text=FloatToStr(s);
 else Edit10->Text=L"********";
}
void TForm6::setFacetArea(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// if(gForm6>0)Edit6->Text=FloatToStrF(s,ffGeneral,15,5);
 if(gForm6>0)Edit11->Text=FloatToStr(s);
 else Edit11->Text=L"******";
}
//---------------------------------------------------------------------------
void __fastcall TForm6::Button1Click(TObject *Sender)
{//Close();
 Form1->QElemTRASH_public();
}
//---------------------------------------------------------------------------


