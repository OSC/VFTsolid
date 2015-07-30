//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TListBox *ListBox1;
	TListBox *ListBox2;
	TListBox *ListBox3;
	TButton *Button1;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox3Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
private:	// User declarations
	int LB1ItemIndex,nlist,iClickOrder,getNlist();
	void setLB2ItemIndex(int);
	String getLB2Item();
public:		// User declarations
	__property int CheckNlist={read=getNlist};
	__property int CheckLB2ItemIndex={write=setLB2ItemIndex};
	__property String CheckLB2Item={read=getLB2Item};
	__fastcall TForm7(int,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
