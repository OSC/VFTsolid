//---------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TLabel *Label1;TLabel *Label2;
        TEdit *Edit1;TEdit *Edit2;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        float getDMax();float getDMin();float SMaxF;float SMinF;
        void setDMax(float);void setDMin(float);
        AnsiString curMax;AnsiString curMin;
        void EFP_Atod(AnsiString,float*,int*);
        int float_checker(AnsiString);
public:		// User declarations
        __fastcall TForm3(float,float,TComponent*);
        __property float MaxForm3={read=getDMax,write=setDMax};
        __property float MinForm3={read=getDMin,write=setDMin};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
