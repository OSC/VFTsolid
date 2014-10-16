//---------------------------------------------------------------------------

#ifndef Unit21H
#define Unit21H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
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
	TGroupBox *GroupBox5;
	TLabel *Label8;
	TEdit *Edit8;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TOpenDialog *OpenDialog1;
	TPanel *Panel1;
	TGroupBox *GroupBox6;
	TRadioButton *RadioButton11;
	TRadioButton *RadioButton12;
	TGroupBox *GroupBox3;
	TRadioButton *RadioButton7;
	TRadioButton *RadioButton8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *Edit9;
	TEdit *Edit10;
	TEdit *Edit11;
	TLabel *Label5;
	TCheckBox *CheckBox1;
	TEdit *Edit5;
	TLabel *Label6;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall RadioButton11Click(TObject *Sender);
	void __fastcall RadioButton12Click(TObject *Sender);
	void __fastcall RadioButton1Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
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
	__property bool CheckGroupBox3={read=getGroupBox3,write=setGroupBox3};
	__property bool CheckGroupBox4={read=getGroupBox4,write=setGroupBox4};
	__property bool CheckCheckBox1={read=getCheckBox1,write=setCheckBox1};
	__fastcall TForm21(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm21 *Form21;
//---------------------------------------------------------------------------
#endif
