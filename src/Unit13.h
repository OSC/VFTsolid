//---------------------------------------------------------------------------
#ifndef Unit13H
#define Unit13H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm13 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        int sect_ic;long nodeno;
public:		// User declarations
        int getSection0();void setSection0(int);
        float getAngle0();void setAngle0(float);
        long getAngle1();void setAngle1(long);
        float getBngle1();void setBngle1(float);
        float getBngle2();void setBngle2(float);
        float getBngle3();void setBngle3(float);
        float getBngle4();void setBngle4(float);
 __fastcall TForm13(int,float,long,float,float,float,float,TComponent*);
 __property int sectionsel0={read=getSection0,write=setSection0};
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property long Angle1={read=getAngle1,write=setAngle1};
 __property float Bngle1={read=getBngle1,write=setBngle1};
 __property float Bngle2={read=getBngle2,write=setBngle2};
 __property float Bngle3={read=getBngle3,write=setBngle3};
 __property float Bngle4={read=getBngle4,write=setBngle4};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm13 *Form13;
//---------------------------------------------------------------------------
#endif
