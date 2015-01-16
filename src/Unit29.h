//---------------------------------------------------------------------------

#ifndef Unit29H
#define Unit29H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm29 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label8;
	TEdit *Edit5;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit6;
	void __fastcall Button1Click(TObject *Sender);
//	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	int loc_isel,getIsel(),getEdit4();
	long loc_pass;
	float getEdit6();
	void setIsel(int),setEdit1(String),setEdit2(String),setEdit3(String),setEdit5(String),setEdit4(int),setEdit6(float);
	String getEdit1(),getEdit2(),getEdit3(),getEdit5();
public:		// User declarations
	__property int CheckIsel={read=getIsel,write=setIsel};
	__property String CheckEdit1={read=getEdit1,write=setEdit1};
	__property String CheckEdit2={read=getEdit2,write=setEdit2};
	__property String CheckEdit3={read=getEdit3,write=setEdit3};
	__property String CheckEdit5={read=getEdit5,write=setEdit5};
	__property int CheckEdit4={read=getEdit4,write=setEdit4};
	__property float CheckEdit6={read=getEdit6,write=setEdit6};
	__fastcall TForm29(int,long,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm29 *Form29;
//---------------------------------------------------------------------------
#endif
