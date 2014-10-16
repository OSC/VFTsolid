//---------------------------------------------------------------------------

#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm6 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TLabel *Label4;
        TEdit *Edit4;
        TLabel *Label5;
        TEdit *Edit5;
        TLabel *Label6;
        TEdit *Edit6;
        TLabel *Label7;
        TEdit *Edit7;
        TLabel *Label8;
        TEdit *Edit8;
        TLabel *Label9;
        TEdit *Edit9;
        TLabel *Label10;
        TEdit *Edit10;
        TButton *Button1;
	TLabel *Label11;
	TLabel *Label12;
	TEdit *Edit11;
	TLabel *Label13;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        void setElemQueryNum(long);
        void setElemQuerySide(long);
        void setElemQueryGID(long);
        void setElemQueryXCor(float);
        void setElemQueryYCor(float);
        void setElemQueryZCor(float);
        void setElemQueryNx(float);
        void setElemQueryNy(float);
        void setElemQueryNz(float);
        void setElemQueryValue(int,float);
        void setFacetArea(float); //Note: Facet area is NOT in TForm6() argument list
		__fastcall TForm6(long,long,long,float,float,float,
             float,float,float,int,float,TComponent*);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm6 *Form6;
//---------------------------------------------------------------------------
#endif
