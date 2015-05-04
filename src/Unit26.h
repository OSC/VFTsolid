//---------------------------------------------------------------------------

#ifndef Unit26H
#define Unit26H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm26 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
    int F26_sel,getISEL(),getItemIndex();
public:		// User declarations
	__property int CheckISEL={read=getISEL};
	__property int CheckItemIndex={read=getItemIndex};
	__fastcall TForm26(int,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm26 *Form26;
//---------------------------------------------------------------------------
#endif
