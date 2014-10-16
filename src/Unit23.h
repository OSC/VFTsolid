//---------------------------------------------------------------------------

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
	TLabel *Label1;
	TLabel *Label2;
	TCheckListBox *CheckListBox1;
	TCheckListBox *CheckListBox2;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TLabel *Label4;
	TShape *Shape1;
	TShape *Shape2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall CheckListBox2Click(TObject *Sender);
	int getSeq();void setSeqIndex(int);
	bool getDir();
	TColor getWColor1();void setWColor1(TColor);
private:	// User declarations
	int seqIndex;
	TColor localColor[128],origColor[128];  //Cat's ass coding...
	int localseqNum[128],origseqNum[128];
	TColor invColor[128],invorigColor[128];  //Cat's ass coding...
	int localseqInv[128],origseqInv[128];
public:		// User declarations
	__property int CheckSeq={read=getSeq,write=setSeqIndex};
	__property bool CheckDir={read=getDir};
	__property TColor CheckWeldColor1={read=getWColor1,write=setWColor1};
	__fastcall TForm23(long,String [],TColor [],int [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm23 *Form23;
//---------------------------------------------------------------------------
#endif
