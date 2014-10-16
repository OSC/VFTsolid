//---------------------------------------------------------------------------

#ifndef Unit21H
#define Unit21H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm21 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
	TGroupBox *GroupBox1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TRadioGroup *RadioGroup1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton3;
	TRadioButton *RadioButton4;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton6;
	TCheckBox *CheckBox1;
	TEdit *Edit5;
	TPanel *Panel1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *Edit6;
	TEdit *Edit7;
	TEdit *Edit8;
	TRadioButton *RadioButton9;
	TRadioButton *RadioButton10;
	TGroupBox *GroupBox4;
	TLabel *Label10;
	TEdit *Edit9;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TOpenDialog *OpenDialog1;
	TRadioButton *RadioButton7;
	TRadioButton *RadioButton8;
	TButton *Button4;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RadioButton1Click(TObject *Sender);
	void __fastcall RadioButton9Click(TObject *Sender);
	void __fastcall RadioButton10Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Bananas(TObject *Sender);
	void __fastcall Plantains(TObject *Sender);
	void __fastcall RadioButton2Click(TObject *Sender);
	void __fastcall RadioButton3Click(TObject *Sender);
	void __fastcall RadioButton4Click(TObject *Sender);
	void __fastcall RadioButton5Click(TObject *Sender);
	void __fastcall RadioButton6Click(TObject *Sender);
private:	// User declarations
	int F21_sel;
//	int ParseSteps(String);
	int getISEL();
	long RB1_count;
//	AnsiString getEdit1();void setEdit1(AnsiString);
//	AnsiString getEdit8();void setEdit8(AnsiString);
	String getEdit1();void setEdit1(String);
	String getEdit5a();void setEdit5a(String);
	String getEdit8();void setEdit8(String);
//	char getEdit1();void setEdit1(char []);
//	char getEdit8();void setEdit8(char []);
	double getEdit2();void setEdit2(double);
	double getEdit3();void setEdit3(double);
	double getEdit4();void setEdit4(double);
	double getEdit5();void setEdit5(double);
	double getEdit6();void setEdit6(double);
	double getEdit7();void setEdit7(double);
	int getRadioGroup1();void setRadioGroup1(int);
	bool getGroupBox3();void setGroupBox3(bool);
	bool getGroupBox4();void setGroupBox4(bool);
	bool getCheckBox1();void setCheckBox1(bool);
public:		// User declarations
	__property int CheckISEL={read=getISEL};
//	__property AnsiString CheckEdit1={read=getEdit1,write=setEdit1};
//	__property char CheckEdit1={read=getEdit1,write=setEdit1};
	__property String CheckEdit1={read=getEdit1,write=setEdit1};
	__property double CheckEdit2={read=getEdit2,write=setEdit2};
	__property double CheckEdit3={read=getEdit3,write=setEdit3};
	__property double CheckEdit4={read=getEdit4,write=setEdit4};
	__property double CheckEdit5={read=getEdit5,write=setEdit5};
	__property String CheckEdit5a={read=getEdit5a,write=setEdit5a};
	__property double CheckEdit6={read=getEdit6,write=setEdit6};
	__property double CheckEdit7={read=getEdit7,write=setEdit7};
//	__property AnsiString CheckEdit8={read=getEdit8,write=setEdit8};
//	__property char CheckEdit8={read=getEdit8,write=setEdit8};
	__property String CheckEdit8={read=getEdit8,write=setEdit8};
	__property int CheckRadioGroup1={read=getRadioGroup1,write=setRadioGroup1};
	__property bool CheckGroupBox3={read=getGroupBox3,write=setGroupBox3};//Note that this refers to Form21/GroupBox2 (but names were discombobulated during revisions) EFP 2/27/2012
	__property bool CheckGroupBox4={read=getGroupBox4,write=setGroupBox4};//Note that this refers to Form21/GroupBox3 (but names were discombobulated during revisions) EFP 2/27/2012
	__property bool CheckCheckBox1={read=getCheckBox1,write=setCheckBox1};
	__fastcall TForm21(int,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm21 *Form21;
//---------------------------------------------------------------------------
#endif
