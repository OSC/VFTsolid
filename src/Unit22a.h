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
	TLabel *Label1;
	TEdit *Edit1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	String getEdit1();void setEdit1(String);
	double getEdit2();void setEdit2(double);
	double getEdit3();void setEdit3(double);
	double getEdit4();void setEdit4(double);
	double getEdit5();void setEdit5(double);
public:		// User declarations
	__property String CheckEdit1={read=getEdit1,write=setEdit1};
	__property double CheckEdit2={read=getEdit2,write=setEdit2};
	__property double CheckEdit3={read=getEdit3,write=setEdit3};
	__property double CheckEdit4={read=getEdit4,write=setEdit4};
	__property double CheckEdit5={read=getEdit5,write=setEdit5};
	__fastcall TForm22(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm22 *Form22;
//---------------------------------------------------------------------------
#endif
