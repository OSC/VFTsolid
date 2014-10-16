//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TScrollBar *ScrollBar1;
        TEdit *Edit1;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button7;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TButton *Button5;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall ScrollBar1Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
private:	// User declarations
        int sect_ic;float ac1,ac2;
        float getAngle1();void setAngle1(float);
        float getAngle2();void setAngle2(float);
public:		// User declarations
        int getSection0();void setSection0(int);
        float getAngle0();void setAngle0(float);
 __fastcall TForm7(int,float,float,float,TComponent*);
 __property int sectionsel0={read=getSection0,write=setSection0};
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property float Angle1={read=getAngle1,write=setAngle1};
 __property float Angle2={read=getAngle2,write=setAngle2};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
