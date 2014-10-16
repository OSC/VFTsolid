//---------------------------------------------------------------------------

#ifndef Unit25H
#define Unit25H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm25 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	int isel;
	int getIsel();void setIsel(int);
	String getEdit1();void setEdit1(String);
	String getEdit2();void setEdit2(String);
	String getEdit3();void setEdit3(String);
	String getEdit4();void setEdit4(String);
	String getEdit5();void setEdit5(String);
	String getEdit6();void setEdit6(String);
public:		// User declarations
	__property int CheckIsel={read=getIsel,write=setIsel};
	__property String CheckEdit1={read=getEdit1,write=setEdit1};
	__property String CheckEdit2={read=getEdit2,write=setEdit2};
	__property String CheckEdit3={read=getEdit3,write=setEdit3};
	__property String CheckEdit4={read=getEdit4,write=setEdit4};
	__property String CheckEdit5={read=getEdit5,write=setEdit5};
	__property String CheckEdit6={read=getEdit6,write=setEdit6};
	__fastcall TForm25(int,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm25 *Form25;
//---------------------------------------------------------------------------
#endif
