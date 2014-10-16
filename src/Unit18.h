//---------------------------------------------------------------------------

#ifndef Unit18H
#define Unit18H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm18 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TListBox *ListBox1;
        TListBox *ListBox2;
        TListBox *ListBox3;
        TButton *Button1;
        TButton *Button2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ListBox1ClickExecute(TObject *Sender);
        void __fastcall ListBox2ClickExecute(TObject *Sender);
        void __fastcall ListBox3ClickExecute(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        float getFixAngle0();float getFixAngle1();float getFixAngle2();void setFixAngle(float,int*);
public:		// User declarations
        __fastcall TForm18(float,float,float,TComponent* Owner);
        __property float Angle0={read=getFixAngle0};
        __property float Angle1={read=getFixAngle1};
        __property float Angle2={read=getFixAngle2};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm18 *Form18;
//---------------------------------------------------------------------------
#endif
