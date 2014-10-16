//---------------------------------------------------------------------------
// Formerly VFTgen Unit18
#ifndef Unit23H
#define Unit23H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm23 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TLabel *Label1;
	TCheckListBox *CheckListBox1;
	TCheckListBox *CheckListBox2;
	TColorListBox *ColorListBox1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label4;
	TButton *Button5;
	TButton *Button6;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall CheckListBox2Click(TObject *Sender);
private:	// User declarations
// String *tempWeldPass;
public:		// User declarations
	__fastcall TForm23(long,String [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm23 *Form23;
//---------------------------------------------------------------------------
#endif
