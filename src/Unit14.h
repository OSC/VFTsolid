//---------------------------------------------------------------------------
#ifndef Unit14H
#define Unit14H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm14 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TEdit *Edit7;
        TEdit *Edit8;
        TEdit *Edit9;
        TEdit *Edit10;
        TEdit *Edit11;
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
        TLabel *Label11;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        int sect_ic;float ac1,ac2,x1,y1,z1,x2,y2,z2,x3,y3,z3;
        float getBngle1();void setBngle1(float);
        float getBngle2();void setBngle2(float);
        float getBngle3();void setBngle3(float);
        float getBngle4();void setBngle4(float);
        float getBngle5();void setBngle5(float);
        float getBngle6();void setBngle6(float);
        float getBngle7();void setBngle7(float);
        float getBngle8();void setBngle8(float);
        float getBngle9();void setBngle9(float);
public:		// User declarations
        int getSection0();void setSection0(int);
        float getAngle0();void setAngle0(float);
        float getAngle1();void setAngle1(float);
 __fastcall TForm14(int,float,float,float,float,float,
     float,float,float,float,float,float,TComponent*);
 __property int sectionsel0={read=getSection0,write=setSection0};
 __property float Angle0={read=getAngle0,write=setAngle0};
 __property float Angle1={read=getAngle1,write=setAngle1};
 __property float Bngle1={read=getBngle1,write=setBngle1};
 __property float Bngle2={read=getBngle2,write=setBngle2};
 __property float Bngle3={read=getBngle3,write=setBngle3};
 __property float Bngle4={read=getBngle4,write=setBngle4};
 __property float Bngle5={read=getBngle5,write=setBngle5};
 __property float Bngle6={read=getBngle6,write=setBngle6};
 __property float Bngle7={read=getBngle7,write=setBngle7};
 __property float Bngle8={read=getBngle8,write=setBngle8};
 __property float Bngle9={read=getBngle9,write=setBngle9};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm14 *Form14;
//---------------------------------------------------------------------------
#endif
