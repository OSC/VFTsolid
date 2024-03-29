//---------------------------------------------------------------------------

#ifndef Unit22H
#define Unit22H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm22 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
    int F22_sel;
	int getISEL();
	String getEdit1();
	void setEdit1(String),setEdit2(double),setEdit3(double),setEdit4(double),setEdit5(double);
	double getEdit2(),getEdit3(),getEdit4(),getEdit5();
public:		// User declarations
	__property int CheckISEL={read=getISEL};
	__property String CheckEdit1={read=getEdit1,write=setEdit1};
	__property double CheckEdit2={read=getEdit2,write=setEdit2};
	__property double CheckEdit3={read=getEdit3,write=setEdit3};
	__property double CheckEdit4={read=getEdit4,write=setEdit4};
	__property double CheckEdit5={read=getEdit5,write=setEdit5};
	__fastcall TForm22(int,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm22 *Form22;
//---------------------------------------------------------------------------
#endif
