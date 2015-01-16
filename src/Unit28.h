//---------------------------------------------------------------------------

#ifndef Unit28H
#define Unit28H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TForm28 : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
	int F28_isel,F28_imode,F28_fORint,getISEL(),getMODE(),getForI();
	long lreset,lmin,lmax,lc1,getLval();
	float freset,fmin,fmax,fc1,getFval();
	void setLval(long),setFval(float);
public:		// User declarations
	__property int CheckISEL={read=getISEL};
	__property int CheckMODE={read=getMODE};
	__property int CheckForI={read=getForI};
	__property long CheckIVAL={read=getLval,write=setLval};
	__property float CheckFVAL={read=getFval,write=setFval};
	__fastcall TForm28(int,int,int,long,long,long,float,float,float,TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm28 *Form28;
//---------------------------------------------------------------------------
#endif
