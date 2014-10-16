//---------------------------------------------------------------------------

#ifndef Unit24H
#define Unit24H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm24 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TButton *Button1;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TEdit *Edit8;
	TLabel *Label9;
	TEdit *Edit9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TEdit *Edit10;
	TEdit *Edit11;
	TEdit *Edit12;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    void setQNDnodeN(int,long);void setQNDnodeXYZD(int,float);
	__fastcall TForm24(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm24 *Form24;
//---------------------------------------------------------------------------
#endif
