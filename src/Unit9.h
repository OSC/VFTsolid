//---------------------------------------------------------------------------
#ifndef Unit9H
#define Unit9H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm9 : public TForm
{
__published:	// IDE-managed Components
        TScrollBar *ScrollBar1;
        TScrollBar *ScrollBar2;
        TScrollBar *ScrollBar3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TLabel *Label7;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall ScrollBar1Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall ScrollBar2Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall ScrollBar3Scroll
          (TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall Button6Click(TObject *Sender);
private:	// User declarations
        int sect_ic;float ac1,ac2,bc1,bc2,cc1,cc2;
        float getAngle1();void setAngle1(float);
        float getAngle2();void setAngle2(float);
        float getBngle1();void setBngle1(float);
        float getBngle2();void setBngle2(float);
        float getCngle1();void setCngle1(float);
        float getCngle2();void setCngle2(float);
public:		// User declarations
        __fastcall TForm9(int,float,float,float,
          float,float,float,float,float,float,TComponent*);
        int getSection0();void setSection0(int);
        float getAngle0();void setAngle0(float);
        float getBngle0();void setBngle0(float);
        float getCngle0();void setCngle0(float);
 __property int sectionsel0={read=getSection0,write=setSection0};
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property float Angle1={read=getAngle1,write=setAngle1};
 __property float Angle2={read=getAngle2,write=setAngle2};
 __property float Bngle0={read=getBngle0,write=setBngle0};
 __property float Bngle1={read=getBngle1,write=setBngle1};
 __property float Bngle2={read=getBngle2,write=setBngle2};
 __property float Cngle0={read=getCngle0,write=setCngle0};
 __property float Cngle1={read=getCngle1,write=setCngle1};
 __property float Cngle2={read=getCngle2,write=setCngle2};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm9 *Form9;
//---------------------------------------------------------------------------
#endif
