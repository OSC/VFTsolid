//---------------------------------------------------------------------------
//#include <fstream.h>
#include <vcl.h>
#pragma hdrstop
#include "Unit3.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(float s1,float s2,TComponent* Owner) : TForm(Owner)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit1->Text=FloatToStrF(s1,ffGeneral,15,5);
// Edit2->Text=FloatToStrF(s2,ffGeneral,15,5);
 Edit1->Text=FloatToStr(s1);
 Edit2->Text=FloatToStr(s2);
 curMax=Edit1->Text;curMin=Edit2->Text;SMaxF=SMinF=0.;
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
//float rvalue;int PassFail;
//ResultMax=Edit1->Text;
//aaaaaaaaaaaa
//EFP_Atod(ResultMax,&rvalue,&PassFail);
//zzzzzzzzzzzz
//ResultMin=Edit2->Text;
//RSMaxF=StrToFloat(Edit1->Text);
//RSMinF=StrToFloat(Edit2->Text);
// try
//   {RSMaxF=StrToFloat(Edit1->Text);
//   }
//// catch (Exception &exception)
// catch (EConvertError&)
//   {
////    Application->ShowException(&exception);
////    extern PACKAGE void __fastcall Beep(void);
//    Application->MessageBox("Must enter real number","Switched to auto_max",MB_OK);
//    RSMaxF=StrToFloat(curMax);
////    throw EConvertError("My max_error message");
//   }
// try
//   {RSMinF=StrToFloat(Edit2->Text);
//   }
// catch (Exception &exception)
//   {
////    Application->ShowException(&exception);
//    extern PACKAGE void __fastcall Beep(void);
//    Application->MessageBox("Must enter real number","Switched to auto_min",MB_OK);
//    RSMinF=StrToFloat(curMin);
//   }
 if(float_checker(Edit1->Text))SMaxF=StrToFloat(Edit1->Text);
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Switched to auto_max",MB_OK);
	   SMaxF=StrToFloat(curMax);
	  }
 if(float_checker(Edit2->Text))SMinF=StrToFloat(Edit2->Text);
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter real number",L"Switched to auto_min",MB_OK);
       SMinF=StrToFloat(curMin);
      }
 Form1->wahea_public();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender)
{float convent=3.4e38;SMaxF=convent;SMinF=convent;Close();Form1->wahea_public();
}
//---------------------------------------------------------------------------
void TForm3::setDMax(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit1->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit1->Text=FloatToStr(s);
}
void TForm3::setDMin(float s)
{enum TFloatFormat { ffGeneral, ffExponent, ffFixed, ffNumber, ffCurrency };
// Edit2->Text=FloatToStrF(s,ffGeneral,15,5);
 Edit2->Text=FloatToStr(s);
}
//float TForm3::getDMax(){return StrToFloat(Edit1->Text);}
//float TForm3::getDMin(){return StrToFloat(Edit2->Text);}
float TForm3::getDMax(){return SMaxF;}float TForm3::getDMin(){return SMinF;}
//------------------------------------------------------
void TForm3::EFP_Atod(AnsiString RM,float* rvalue,int* PassFail)
{float rsign=1.; *rvalue=0.; *PassFail=0; int lenS=RM.Length();
// char* locDecPoint=StrScan(RM.c_str(),'.');
// char* locDec1=StrScan(RM.c_str(),'4');
// char* locDec2=StrScan(RM.c_str(),'9');
// char* locDec3=StrScan(RM.c_str(),'6');
 int iPower=lenS-1;
 float tenprod=1.;
 for(int ip=0;ip<iPower;ip++)tenprod=tenprod*10.;
 if(lenS)
   {for(int ic=0;ic<lenS;ic++)
       {if     (RM[ic+1]== '-'){rsign= -1.;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '.'){}
        else if(RM[ic+1]== '0'){tenprod=tenprod/10.;}
        else if(RM[ic+1]== '1'){*rvalue=*rvalue+   tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '2'){*rvalue=*rvalue+2.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '3'){*rvalue=*rvalue+3.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '4'){*rvalue=*rvalue+4.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '5'){*rvalue=*rvalue+5.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '6'){*rvalue=*rvalue+6.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '7'){*rvalue=*rvalue+7.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '8'){*rvalue=*rvalue+8.*tenprod;tenprod=tenprod/10.;}
        else if(RM[ic+1]== '9'){*rvalue=*rvalue+9.*tenprod;tenprod=tenprod/10.;}
        else {Edit1->Text="Please input again";return;}
       }
    *rvalue=*rvalue*rsign;
   }
 else {Edit1->Text="Please input again";}
}
//------------------------------------------------------
int TForm3::float_checker(AnsiString RM)
{int ans=0;if(!StrLen(RM.c_str()))return ans=0;
 if(AnsiStrScan(RM.c_str(),'a') || AnsiStrScan(RM.c_str(),'A'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'b') || AnsiStrScan(RM.c_str(),'B'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'c') || AnsiStrScan(RM.c_str(),'C'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'d') || AnsiStrScan(RM.c_str(),'D'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'f') || AnsiStrScan(RM.c_str(),'F'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'g') || AnsiStrScan(RM.c_str(),'G'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'h') || AnsiStrScan(RM.c_str(),'H'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'i') || AnsiStrScan(RM.c_str(),'I'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'j') || AnsiStrScan(RM.c_str(),'J'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'k') || AnsiStrScan(RM.c_str(),'K'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'l') || AnsiStrScan(RM.c_str(),'L'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'m') || AnsiStrScan(RM.c_str(),'M'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'n') || AnsiStrScan(RM.c_str(),'N'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'o') || AnsiStrScan(RM.c_str(),'O'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'p') || AnsiStrScan(RM.c_str(),'P'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'q') || AnsiStrScan(RM.c_str(),'Q'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'r') || AnsiStrScan(RM.c_str(),'R'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'s') || AnsiStrScan(RM.c_str(),'S'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'t') || AnsiStrScan(RM.c_str(),'T'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'u') || AnsiStrScan(RM.c_str(),'U'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'v') || AnsiStrScan(RM.c_str(),'V'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'w') || AnsiStrScan(RM.c_str(),'W'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'x') || AnsiStrScan(RM.c_str(),'X'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'y') || AnsiStrScan(RM.c_str(),'Y'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'z') || AnsiStrScan(RM.c_str(),'Z'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'`') || AnsiStrScan(RM.c_str(),'~'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'!') || AnsiStrScan(RM.c_str(),'@'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'#') || AnsiStrScan(RM.c_str(),'$'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'%') || AnsiStrScan(RM.c_str(),'^'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'&') || AnsiStrScan(RM.c_str(),'*'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'(') || AnsiStrScan(RM.c_str(),')'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'_') || AnsiStrScan(RM.c_str(),'='))return ans=0;
 if(AnsiStrScan(RM.c_str(),'[') || AnsiStrScan(RM.c_str(),'{'))return ans=0;
 if(AnsiStrScan(RM.c_str(),']') || AnsiStrScan(RM.c_str(),'}'))return ans=0;
// if(AnsiStrScan(RM.c_str(),'\'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'|'))return ans=0;
 if(AnsiStrScan(RM.c_str(),';') || AnsiStrScan(RM.c_str(),':'))return ans=0;
// if(AnsiStrScan(RM.c_str(),'''))return ans=0;
 if(AnsiStrScan(RM.c_str(),'"'))return ans=0;
 if(AnsiStrScan(RM.c_str(),',') || AnsiStrScan(RM.c_str(),'<'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'>'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'/') || AnsiStrScan(RM.c_str(),'?'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'.') != AnsiStrRScan(RM.c_str(),'.'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'e') != AnsiStrRScan(RM.c_str(),'e'))return ans=0;
 if(AnsiStrScan(RM.c_str(),'E') != AnsiStrRScan(RM.c_str(),'E'))return ans=0;
 return ans=1;
}








