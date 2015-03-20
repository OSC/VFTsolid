//---------------------------------------------------------------------------

#ifndef Unit15H
#define Unit15H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <strsafe.h>
//---------------------------------------------------------------------------
class TForm15 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Edit1;
        TLabel *Label2;
        TEdit *Edit2;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TPageControl *PageControl1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TLabel *Label9;
        TPanel *Panel1;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TGroupBox *GroupBox1;
        TLabel *Label14;
        TLabel *Label15;
        TLabel *Label16;
        TLabel *Label17;
        TEdit *Edit9;
        TEdit *Edit10;
        TEdit *Edit11;
        TEdit *Edit12;
        TEdit *Edit13;
        TEdit *Edit14;
        TEdit *Edit15;
        TEdit *Edit16;
        TGroupBox *GroupBox2;
        TLabel *Label18;
        TLabel *Label19;
        TGroupBox *GroupBox3;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TEdit *Edit17;
        TEdit *Edit18;
        TEdit *Edit19;
        TEdit *Edit20;
        TGroupBox *GroupBox4;
        TLabel *Label24;
        TLabel *Label25;
        TEdit *Edit21;
        TEdit *Edit22;
        TGroupBox *GroupBox5;
        TRadioButton *RadioButton5;
        TRadioButton *RadioButton6;
        TLabel *Label26;
        TEdit *Edit23;
        TCheckBox *CheckBox3;
        TGroupBox *GroupBox6;
        TLabel *Label27;
        TCheckBox *CheckBox4;
        TEdit *Edit24;
        TRadioButton *RadioButton7;
        TComboBox *ComboBox1;
        TComboBox *ComboBox2;
	TColorDialog *ColorDialog1;
	TListBox *ListBox2;
	TTabSheet *TabSheet1;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TShape *Shape1;
	TLabel *Label6;
	TRadioGroup *RadioGroup2;
	TCheckBox *CheckBox1;
	TButton *Button4;
	TListBox *ListBox1;
	TRadioGroup *RadioGroup1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RadioGroup1Click(TObject *Sender);

private:	// User declarations
        int iflagForm15;
		int F15_isel,kflagForm15,jflagForm15,nMatPS,nWeldPS,getRadioBstatus(),getCheckBstatus(),getEdit19(),
			getISEL(),getFunc(),getType(),getShape(),getMatName(),getWeldParName();
		long Pl1norm,Pl2norm,getEdit2(),getEdit23(),getEdit24();
		double getEdit9(),getEdit10(),getEdit11(),getEdit12(),getEdit13(),getEdit14(),getEdit15(),getEdit16(),
			   getEdit17(),getEdit18(),getEdit20(),getEdit21(),getEdit22();
		void setEdit1(String),setEdit2(long),setEdit9(double),setEdit10(double),setEdit11(double),setEdit12(double),
			 setEdit13(double),setEdit14(double),setEdit15(double),setEdit16(double),setEdit17(double),setEdit18(double),
			 setEdit19(int),setEdit20(double),setEdit21(double),setEdit22(double),setEdit23(long),setEdit24(long),
			 setFunc(int),setType(int),setShape(int),setWColor(TColor),setNorm1(long),setNorm2(long); //EFP 5/31/2011
		String getEdit1();
		TColor getWColor();
public:		// User declarations
		__property int CheckRadioB={read=getRadioBstatus};
		__property String CheckEdit1={read=getEdit1,write=setEdit1};
		__property long CheckEdit2={read=getEdit2,write=setEdit2};
		__property int CheckCheckB={read=getCheckBstatus};
//		__property double CheckEdit9={read=getEdit9,write=setEdit9};
//		__property double CheckEdit10={read=getEdit10,write=setEdit10};
//		__property double CheckEdit11={read=getEdit11,write=setEdit11};
//		__property double CheckEdit12={read=getEdit12,write=setEdit12};
		__property double CheckEdit9={write=setEdit9};
		__property double CheckEdit10={write=setEdit10};
		__property double CheckEdit11={write=setEdit11};
		__property double CheckEdit12={write=setEdit12};
		__property double CheckEdit13={read=getEdit13,write=setEdit13};
		__property double CheckEdit14={read=getEdit14,write=setEdit14};
		__property double CheckEdit15={read=getEdit15,write=setEdit15};
		__property double CheckEdit16={read=getEdit16,write=setEdit16};
		__property double CheckEdit17={read=getEdit17,write=setEdit17};
		__property double CheckEdit18={read=getEdit18,write=setEdit18};
		__property int CheckEdit19={read=getEdit19,write=setEdit19};
		__property double CheckEdit20={read=getEdit20,write=setEdit20};
		__property double CheckEdit21={read=getEdit21,write=setEdit21};
		__property double CheckEdit22={read=getEdit22,write=setEdit22};
		__property long CheckEdit23={read=getEdit23,write=setEdit23};
		__property long CheckEdit24={read=getEdit24,write=setEdit24};
		__property int CheckISEL={read=getISEL};
		__property int CheckFunction={read=getFunc,write=setFunc};
		__property int CheckType={read=getType,write=setType};
		__property int CheckShape={read=getShape,write=setShape};
		__property int CheckMatName={read=getMatName};
		__property int CheckWeldParName={read=getWeldParName};
		__property TColor CheckWeldColor={read=getWColor,write=setWColor};
		__property long CheckNorm1={write=setNorm1}; //EFP 5/31/2011
		__property long CheckNorm2={write=setNorm2}; //EFP 5/31/2011
		__fastcall TForm15(int,long,long,long,long,long,long,long,String [],long,String [],TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm15 *Form15;
//---------------------------------------------------------------------------
#endif
