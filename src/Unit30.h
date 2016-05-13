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
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TCheckListBox *CheckListBox2;
	TShape *Shape1;
	TButton *Button8;
	TLabel *Label4;
	TEdit *Edit1;
	TListBox *ListBox1;
	void __fastcall Button1Click(TObject *Sender);
//	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall CheckListBox2Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
//	void __fastcall CheckListBox1Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
private:	// User declarations
	int local_isel,seqIndex,getISEL(),getSeq(),localseqInv[256],origseqInv[256];
	long local_index,getEdit1();
	bool getDir();
	void setISEL(int),setSeqIndex(int),setWColor1(TColor),setEdit1(long);
	TColor getWColor1(),invColor[256],invorigColor[256];  //Cat's ass coding...
public:		// User declarations
	__property int CheckISEL={read=getISEL,write=setISEL};
	__property int CheckSeq={read=getSeq,write=setSeqIndex};
	__property bool CheckDir={read=getDir};
	__property TColor CheckWeldColor1={read=getWColor1,write=setWColor1};
	__property long CheckEdit1={read=getEdit1,write=setEdit1};
	__fastcall TForm30(int,long,const String [],const TColor [],const int [],const long [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm30 *Form30;
//---------------------------------------------------------------------------
#endif
