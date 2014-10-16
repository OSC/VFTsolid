//---------------------------------------------------------------------------

#ifndef Unit30H
#define Unit30H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm30 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TCheckListBox *CheckListBox1;
	TCheckListBox *CheckListBox2;
	TShape *Shape1;
	TEdit *Edit1;
	TLabel *Label4;
	TButton *Button8;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall CheckListBox2Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
private:	// User declarations
	int seqIndex;
	TColor invColor[256],invorigColor[256];  //Cat's ass coding...
	int localseqInv[256],origseqInv[256];
	int getSeq();void setSeqIndex(int);
	bool getDir();
	long getEdit1();void setEdit1(long);
	TColor getWColor1();void setWColor1(TColor);
public:		// User declarations
	__property int CheckSeq={read=getSeq,write=setSeqIndex};
	__property bool CheckDir={read=getDir};
	__property TColor CheckWeldColor1={read=getWColor1,write=setWColor1};
	__property long CheckEdit1={read=getEdit1,write=setEdit1};
	__fastcall TForm30(long,String [],TColor [],int [],long [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm30 *Form30;
//---------------------------------------------------------------------------
#endif
