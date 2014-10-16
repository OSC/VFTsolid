//---------------------------------------------------------------------------
#ifndef Unit17H
#define Unit17H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
//---------------------------------------------------------------------------
class TForm17 : public TForm
{
__published:	// IDE-managed Components
        TCheckListBox *CheckListBox1;
        TButton *Button1;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        void setGIDCheckInfo(long []);long getXXXCheckOX();void setXXXCheckOX(long);
public:		// User declarations
        __property long CheckOX={read=getXXXCheckOX,write=setXXXCheckOX};
        __fastcall TForm17(int,long,long [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm17 *Form17;
//---------------------------------------------------------------------------
#endif
