//---------------------------------------------------------------------------

#ifndef Unit27H
#define Unit27H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TForm27 : public TForm
{
__published:	// IDE-managed Components
	TCheckListBox *CheckListBox1;
	TButton *Button1;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	long ckIndex,selMode;
	long getIndex();void setIndex(long);long getSel();
public:		// User declarations
	__property long CheckIndex={read=getIndex,write=setIndex};
	__property long CheckSel={read=getSel};
	__fastcall TForm27(int,long,String [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm27 *Form27;
//---------------------------------------------------------------------------
#endif
