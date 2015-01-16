//---------------------------------------------------------------------------

#ifndef Unit8H
#define Unit8H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm8 : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
//        void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
  int isw;float ac1;
public:		// User declarations
 int getISW();float getAngle0();
 void setAngle0(float),setISW(int);
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property int checkISW={read=getISW,write=setISW};
 __fastcall TForm8(int,float,TComponent*);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm8 *Form8;
//---------------------------------------------------------------------------
#endif
