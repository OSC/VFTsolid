//---------------------------------------------------------------------------

#ifndef Unit12H
#define Unit12H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm12 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TScrollBar *ScrollBar1;
        TScrollBar *ScrollBar2;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TButton *Button6;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall ScrollBar1Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall ScrollBar2Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall Button6Click(TObject *Sender);
private:	// User declarations
        int sect_ic;float ang0,ang1,ac1,ac2,x1,y1,z1;
        float getBngle1();void setBngle1(float);
        float getBngle2();void setBngle2(float);
        float getBngle3();void setBngle3(float);
public:		// User declarations
        int getSection0();void setSection0(int);
        float getAngle0();void setAngle0(float);
        float getAngle1();void setAngle1(float);
 __fastcall TForm12(int,float,float,float,float,float,TComponent*);
 __property int sectionsel0={read=getSection0,write=setSection0};
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property float Angle1={read=getAngle1,write=setAngle1};
 __property float Bngle1={read=getBngle1,write=setBngle1};
 __property float Bngle2={read=getBngle2,write=setBngle2};
 __property float Bngle3={read=getBngle3,write=setBngle3};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm12 *Form12;
//---------------------------------------------------------------------------
#endif
