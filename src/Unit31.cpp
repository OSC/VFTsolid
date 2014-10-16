//---------------------------------------------------------------------------
#include <vcl.h>
#include <Jpeg.hpp> //Must be added  EFP 2/25/2012
#pragma hdrstop
#include "Unit31.h" //Remember to remove from Autocreate list
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm31 *Form31;
//---------------------------------------------------------------------------
__fastcall TForm31::TForm31(char comboCWD[],TComponent* Owner) : TForm(Owner)
{TJPEGImage *jp;jp= new TJPEGImage();
// jp->LoadFromFile(L"icons/image001.jpg");
 jp->LoadFromFile(comboCWD);
 jp->DIBNeeded();// VFT(tm) image
// Image1->Visible=false;
 Image1->Picture->Bitmap->Assign(jp);delete jp;
// Label1->Caption="VFTsolid version 3.2.53b 2012";
}
//---------------------------------------------------------------------------
