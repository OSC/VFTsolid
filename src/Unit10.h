//---------------------------------------------------------------------------
#ifndef Unit10H
#define Unit10H
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm10 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TLabel *Label2;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TLabel *Label3;
        TButton *Button1;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
  float ac1,ac2,ac3,ac4,ac5,ac6;
public:		// User declarations
  void setC1(float);void setC2(float);void setC3(float);
  void setC4(float);void setC5(float);void setC6(float);
  float getC1();float getC2();float getC3();float getC4();float getC5();float getC6();
  __fastcall TForm10(float,float,float,float,float,float,TComponent*);
  __property float GAxC0={read=getC1,write=setC1}; __property float GAxC1={read=getC2,write=setC2};
  __property float GAxC2={read=getC3,write=setC3}; __property float GAxC3={read=getC4,write=setC4};
  __property float GAxC4={read=getC5,write=setC5}; __property float GAxC5={read=getC6,write=setC6};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm10 *Form10;
//---------------------------------------------------------------------------
#endif
