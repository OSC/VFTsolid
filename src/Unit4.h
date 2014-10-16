//---------------------------------------------------------------------------
#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TScrollBar *ScrollBar1;
        TLabel *Label2;
        TEdit *Edit1;
        TLabel *Label3;
        TScrollBar *ScrollBar2;
        TLabel *Label4;
        TEdit *Edit2;
        TLabel *Label5;
        TScrollBar *ScrollBar3;
        TLabel *Label6;
        TEdit *Edit3;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall ScrollBar1Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall ScrollBar2Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall ScrollBar3Scroll(TObject *Sender,TScrollCode ScrollCode,int &ScrollPos);
        void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
private:	// User declarations
        float getAngle0();float getAngle1();float getAngle2();float AA0;float AA1;float AA2;
        void setAngle0(float);void setAngle1(float);void setAngle2(float);
public:		// User declarations
        __fastcall TForm4(float,float,float,TComponent*);
        __property float Angle0={read=getAngle0,write=setAngle0};
        __property float Angle1={read=getAngle1,write=setAngle1};
        __property float Angle2={read=getAngle2,write=setAngle2};
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
