//---------------------------------------------------------------------------
// Warning:
//   This VFTsolid version writes the multi-materials version of WARP3D uexternal_data_file.inp
//   .
//   .
//2
//./monel_file.dat     //In order of appearance in wms.XXX list (per BobD request 1/13/2015)
//./inconel718_file.dat
//./VED.dat
//./warp_temp_2_files
//   .
//   .
//   instead of the single file
//./material.dat
//./VED.dat
//./warp_temp_2_files

//---------------------------------------------------------------------------
// Policy:
// (i) All output files are UTF8 (i.e. char, no markers, no Unicode, no nothing...)
// (ii) Current convention for base.arrELSET=new long[base.nelt]
//      This stores one GID (Geometric ID) per element. All non-weldgroup elements are bundled into a single GID=0 basemetal.
//      WeldGroups are numbered in the order found in the *.msh/*.abq/*.inp/*/VFTr data file, and are stored in base.arrELSET[], begining with 1.
//      #weldgroups= nWeldGroup
//      base.arrELSET[] is allocated/deleted by FDdynamic_manage(15 plus & minus.
//      Note that an EFP "multiGID per element" base.trackELSET[] scheme failed.
//      It crashed VFT GUI in ImportAba_prog() readin. I do not know why. Excessive memory requirement?
// (iii) indat.arrELSET=new long[base.nelt]. This is the plotting version of base.arrELSET[]
//       indat.arrELSET[] is allocated/deleted by FDdynamic_manage(17 plus & minus.
// (iv) base.groupsname=new String[wp.nWeldGroup+1].
//       These are the WeldGroup ELSET names associated with base.arrELSET[] above.
//       base.groupsname[] is allocated/deleted by FDdynamic_manage(13 plus & minus.
//       Note that the imputed "basemetal" ELSET GID=0 is NOT STORED in base.groupsname[].
//       (i.e. The first weldgroup ELSET [GID=1] is base.groupsname[0].
//       There is no need for the "+1" in [storage]. Just in case wp.nWeldGroup is zero?)
// (v) base.ELSETinputnames=new String[base.allGrp]
//     These are the ELSET names read from *.msh/*/abq/*.inp/*.VFTr
//       base.ELSETinputname[] is allocated/deleted by FDdynamic_manage(20 plus & minus.
//		 base.ELSETinputname[0]=L"ALLEL", whether ALLEL is present in the datafile or not.
//       The base.allGrp count begins with 1 to allow for ALLEL.
//       There is not any test for duplication.
// (vi) The first 3 digits of base.matno=new long[base.nelt]
//      stores the ELSET number of this element, numbered in the order read from datafile,
//      corresponding to base.ELSETinputname [] above.
//      The value of 0 is given to ALLEL, whether present in the data or not. There can be only one "value per element",
//      with destructive-overwrite for multiple occurrences per element.
//      After weldpasses have been created, they are introduced to these first 3 digits of base.matno by
//      destructive overwrite, using a negative value (i.e. first weldpass => overwrite -1).
//      Curently there is no memory provision to Edit/Undo this overwrite.
//      NOTE: Max #ELSETS=999 but no test for this.
// (vii) base.arELEM=new int[base.nelt] is used to authorize plotting of individual elements.
// (viii) Weld passes can be created only from user-generated weld groups.
//        To be a weld group, an ELSET must have (somewhere) in its name (capitals or lower case)
//        WD
//        WELD
//        WG
//        WP
// (ix) Although the term ELSET comes from Simulia/ABAQUS, it is inferred here to mean any non-node/non-element "**" data in *.msh.
//
// 32-to-64 bit conversion issues:
// (i) View/Project manager=> Look on the right for Target Platforms=> Right click =>Add platform =>64
//     This is not in Project/Options... for some reason?
// (ii) Change <fstream.h> to <fstream>; ditto <iomanip>
// (iii) max & min functions require <algorithm>
// (iv) It is necessary to add
//       using std::min;..max..setw..scientific..dec..showpoint..setprecision
// (v) pow(,) function needs pow(double,double) to satisfy Unit21 linker problem
// (vi) It is wise to use std::XXX throughout.

// To create new C++ Builder form:
//   File/New/VCL form => FormX
//   Select FormX => File/Use unit... => Unit1
//   Select Unit1 => File/Use unit... => UnitX
//   Open Project.cpp => Delete line initially creating UnitX
//   Do not add TFormX *FormX to masthead of Unit1 since it is already global in UnitX
//   UnitX/FormStyle => fsStayOnTop
//   If Modal, UnitX/Button1/Modal result => mrClose
//   Save
//
// Bugfixes:
//   (i) stchar[] dimensions in most parse_..() routines
//   (ii) ImportAba_prog() & ImportMshExecute() for(in=0;in<base.nelt;in++)base.arrELSET..)
#include <vcl.h>
#pragma hdrstop
#include <Jpeg.hpp>
//#include <GIFImg.hpp> // Bug in CodeGear: Unable to load *.gif ???
#include <memory>
//#include <fstream.h>
#include <fstream>
//#include <iomanip.h>
#include <iomanip>
#include <algorithm> //max & min functions  EFP 1/16/2015

using std::min;
using std::max;
using std::setw;
using std::scientific;
using std::dec;
using std::showpoint;
using std::setprecision;

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit6.h"
#include "Unit22.h"
#include "Unit26.h"
#include "Unit21.h"
#include "Unit15.h"
#include "Unit30.h"
#include "Unit29.h"
#include "Unit25.h"
#include "Unit28.h"
#include "Unit31.h"
#include "Unit8.h"
#include "Unit7.h"
#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TForm2 *FEstatForm; //FEstatistics (Modal)
TForm3 *QNode; //QueryNode (Modeless)
TForm4 *QNDist; //Nodal distance (Modeless)
//TForm5 is launched from Unit21, not Unit1
TForm6 *QElem; // QueryElem (Modeless)
//TForm7 *Form7; // Assign material files to non-WPs (Modal)
TForm8 *tshiftCTSP; //Modal (but can be Modeless)
//TForm9 *Form9; // Select WARP3D output entities (Modal)
TForm15 *CreateLinWeldPass; // Modeless
//TForm21 *Form21; // Renamed from *CreateMatPropSet; // (Modal)
TForm22 *CreateWeldingParamSet; // (Modal)
TForm25 *ABAQnames; //Modal
TForm26 *VFTlistbox; //Multi-use dialog (Modal)
TForm28 *F28_Form; // Modal (but can be Modeless)
TForm29 *CTSPnames; // Modal
TForm30 *WeldPassEditSeqn; // (Modeless)
TForm31 *About_VFT; //Modal

ofstream honk("VFTsolidlog.out");
String VFTversion=L"VFTsolid version 3.2.57e_64 2015";
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
 TJPEGImage *jp;
//TGIFImage *gp;
// Form1->Caption=L"VFTsolid version 3.2.56h 2014"; //Remember to change in TForm31 & FileClose
 Form1->Caption=VFTversion;
 base.nop1=indat.nop1=NULL;  // Pointer NULL applies to int & long only
 iplotflag=iCircleplot=iCullyesno=nGIDmax=nGID=0;
// fl_feres=nMultFile=iplotType=GIFcount=CAPres_count=GIFsw=ANNOTcount=ANLINcount=iBackGroundColor=0;
 fl_feres=iplotType=0;
 base.npoin=base.nelt=wp.nWeldGroup=wp.nWeldPass=0;
 shapecombo=0.f;
 iBackroundColor=0;BackroundColorF0->Checked= true;BackroundColorF1->Checked=false;BackroundColorF2->Checked=false;
				   BackroundColorF3->Checked=false;BackroundColorF4->Checked=false;
 wp.PAINT_SEL=1;qndNodeNum1= -1;
 base.GIDcol=indat.GIDcol=2; // i.e. Prioritize plotting of WP
//Convention: For multiple GIDs per element, use GID in column 2 (of 0,1,2...) if extant.
//                In VFTsolid, Col=0 is base metal, 1 is WG, 2 is WP
//                In VFTshell, Col=0 is base metal, 1 is WG, 2 is 2nd WG (if extant), 3 is WP
 iPaintyesno=0; //AllGIDs
 gl_indatMedian[0]=gl_indatMedian[1]=gl_indatMedian[2]=0.f;glGdiff=1.f;
 rangle[0]=rangle[1]=rangle[2]=0.f;rangle0[0]=rangle0[1]=rangle0[2]=0.f;
 trans_op[0]=trans_op[1]=trans_op[2]=0.f;prod_op[0][0]=prod_op[1][1]=prod_op[2][2]=1.f;
 prod_op[0][1]=prod_op[0][2]=prod_op[1][0]=0.f;prod_op[1][2]=prod_op[2][0]=prod_op[2][1]=0.f;
 trans_zoom[0]=0.f;trans_zoom[1]=0.f;trans_zoom[2]=0.f;
 prod_zoom[0]=1.f;prod_zoom[1]=1.f;prod_zoom[2]=1.f;rot_op[0][0]=rot_op[1][1]=rot_op[2][2]=1.f;
 rot_op[0][1]=rot_op[0][2]=rot_op[1][0]=0.f;rot_op[1][2]=rot_op[2][0]=rot_op[2][1]=0.f;
 zoomDrag=false;FD_LButtonstatus=0;
 iPersistVFT=0;stateVFT=0;
 new_nelt=new_npoin=new_nvfix=new_nedge=new_mat=0;
 wp.memWGa=wp.highest=wp.avis=wms.highest=wps.highest=0;
 wp.PRECORD=wp.EDIT_SEL=wp.elStart= -1;wp.pending=0;
 wp.nWeldPass=wp.nWeldGroup=wp.nWeldPassEl=wms.nMatPropSet=wps.nWeldParamSet=VFTon=wp.Record=wp.edit_sel=wp.paint_sel=0;
 wp.CreateWPassMode=0;
 axis_plot_init();
 if((wp.avis-100*(wp.avis/100))/10)Normal_direction1->Checked=false;
 else                              Normal_direction1->Checked=true;
 Complementary_direction1->Checked=1-Normal_direction1->Checked;
 Form1->WindowState=wsMaximized;
 QNode=NULL;
 QElem=NULL;
 QNDist=NULL;
 CreateLinWeldPass=NULL;
 WeldPassEditSeqn=NULL;
 F28_Form=NULL;
 tshiftCTSP=NULL;
 if((wp.avis-100*(wp.avis/100))/10)Normal_direction1->Checked=false;
 else                              Normal_direction1->Checked=true;
 Complementary_direction1->Checked=1-Normal_direction1->Checked;
 tBitmap = new Graphics::TBitmap(); // Borland Graphics::TBitmap to distinguish from MSWin Bitmap
 jp= new TJPEGImage();
///////////////////////////
 jp->LoadFromFile(L"icons/image035.jpg");jp->DIBNeeded();// Selection mode of graphics (Arrow graphic)

 Image1->Visible=false;
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn1->Glyph->Monochrome=false;BitBtn1->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image037.jpg");jp->DIBNeeded();// View mode: pivot or pan (Hand graphic)
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn2->Glyph->Monochrome=false;
 BitBtn2->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image039.jpg");jp->DIBNeeded();// 3DMasterSuiteHelp (not used)
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn3->Glyph->Monochrome=false;BitBtn3->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image041.jpg");jp->DIBNeeded();// Set current view as home view
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn4->Glyph->Monochrome=false;BitBtn4->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image042.jpg");jp->DIBNeeded();//Return to this home view
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn5->Glyph->Monochrome=false;BitBtn5->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image043.jpg");jp->DIBNeeded();//View all regions of model
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn6->Glyph->Monochrome=false;BitBtn6->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image050.jpg");jp->DIBNeeded();//Target zoom
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn7->Glyph->Monochrome=false;BitBtn7->Glyph=Image1->Picture->Bitmap;

 jp->LoadFromFile(L"icons/image034.JPEG");jp->DIBNeeded();// Box zoom. I had to name this *.JPEG to get code to read it???
 Image1->Picture->Bitmap->Assign(jp);
 BitBtn8->Glyph->Monochrome=false;BitBtn8->Glyph=Image1->Picture->Bitmap;
///////////////////////////
 jp->LoadFromFile(L"icons/image001.jpg");jp->DIBNeeded(); //VFT image
 Image1->Visible=true;
 Image1->Picture->Bitmap->Assign(jp);
 delete jp;
//////////////////////////////////////////////
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FileCloseExecute(TObject *Sender)
{
 Form1->Caption=VFTversion;
 long dum=0;base.nop1=indat.nop1=NULL;  // Pointer NULL applies to int & long only
 iplotflag=iCircleplot=iCullyesno=nGIDmax=nGID=nColRes=0;
 base.nop1=indat.nop1=NULL;  // Pointer NULL applies to int & long only
 iplotflag=iCircleplot=iCullyesno=nGIDmax=nGID=0;
// fl_feres=nMultFile=iplotType=GIFcount=CAPres_count=GIFsw=ANNOTcount=ANLINcount=iBackGroundColor=0;
 fl_feres=iplotType=0;
 base.npoin=base.nelt=wp.nWeldGroup=wp.nWeldPass=0;
 shapecombo=0.f;
 iBackroundColor=0;BackroundColorF0->Checked= true;BackroundColorF1->Checked=false;BackroundColorF2->Checked=false;
				   BackroundColorF3->Checked=false;BackroundColorF4->Checked=false;
 wp.PAINT_SEL=1;qndNodeNum1= -1;
 base.GIDcol=indat.GIDcol=2; // i.e. Prioritize plotting of WP
//Convention: For multiple GIDs per element, use GID in column 2 (of 0,1,2...) if extant.
//                In VFTsolid, Col=0 is base metal, 1 is WG, 2 is WP
//                In VFTshell, Col=0 is base metal, 1 is WG, 2 is 2nd WG (if extant), 3 is WP
 iPaintyesno=0; //AllGIDs
 gl_indatMedian[0]=gl_indatMedian[1]=gl_indatMedian[2]=0.f;glGdiff=1.f;
 rangle[0]=rangle[1]=rangle[2]=0.f;rangle0[0]=rangle0[1]=rangle0[2]=0.f;
 trans_op[0]=trans_op[1]=trans_op[2]=0.f;prod_op[0][0]=prod_op[1][1]=prod_op[2][2]=1.f;
 prod_op[0][1]=prod_op[0][2]=prod_op[1][0]=0.f;prod_op[1][2]=prod_op[2][0]=prod_op[2][1]=0.f;
 trans_zoom[0]=0.f;trans_zoom[1]=0.f;trans_zoom[2]=0.f;
 prod_zoom[0]=1.f;prod_zoom[1]=1.f;prod_zoom[2]=1.f;rot_op[0][0]=rot_op[1][1]=rot_op[2][2]=1.f;
 rot_op[0][1]=rot_op[0][2]=rot_op[1][0]=0.f;rot_op[1][2]=rot_op[2][0]=rot_op[2][1]=0.f;
 zoomDrag=false;FD_LButtonstatus=0;
 iPersistVFT=0;stateVFT=0;
 new_nelt=new_npoin=new_nvfix=new_nedge=new_mat=0;
 wp.memWGa=wp.highest=wp.avis=wms.highest=wps.highest=0;
 wp.PRECORD=wp.EDIT_SEL=wp.elStart= -1;wp.pending=0;
 wp.nWeldPass=wp.nWeldGroup=wp.nWeldPassEl=wms.nMatPropSet=wps.nWeldParamSet=VFTon=wp.Record=wp.edit_sel=wp.paint_sel=0;
 wp.CreateWPassMode=0;
 axis_plot_init();
 Form1->WindowState=wsMaximized;
 if((wp.avis-100*(wp.avis/100))/10)Normal_direction1->Checked=false;
 else                              Normal_direction1->Checked=true;
 Complementary_direction1->Checked=1-Normal_direction1->Checked;
 if(QNode){delete QNode;QNode=NULL;}
 if(QElem){delete QElem;QElem=NULL;}
 if(QNDist){delete QNDist;QNDist=NULL;}
 if(CreateLinWeldPass){delete CreateLinWeldPass;CreateLinWeldPass=NULL;}
 if(WeldPassEditSeqn){delete WeldPassEditSeqn;WeldPassEditSeqn=NULL;}
 if(F28_Form){delete F28_Form;F28_Form=NULL;} //Can be Modal or Modeless
 if(tshiftCTSP){delete tshiftCTSP;tshiftCTSP=NULL;} //Can be Modal or Modeless
	if(wp.nWeldPass){wp.nWeldPass=wp.nWeldPassEl=wp.nWeldGroup=0; //nGID= 1 base + #weldgroups
					 FDdynmem_manage(-6,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
					}
	if(wms.nMatPropSet){wms.nMatPropSet=0;
						FDdynmem_manage(-7,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
					   }
	if(wps.nWeldParamSet){wps.nWeldParamSet=0;
						  FDdynmem_manage(-8,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
						 }
	FDdynmem_manage(-20,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	base.nofix=NULL;base.nldel=NULL;base.lodpt=NULL;base.matyp=NULL;base.ebody=NULL;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExitExecute(TObject *Sender){exit(0);}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportAbaExecute(TObject *Sender){ImportAba_prog(0);} //Argument not used
//---------------------------------------------------------------------------
void TForm1::ImportAba_prog(int iswtype)
{  //OPEN01
// Note#1: When reading XXX_ABA.inp files, this subroutine does NOT read "INCLUDEd" XXX_bc.inp files below
//********** Boundary Condition Definition **********
//*INCLUDE, INPUT=testingVFT_bc.inp
//
// Note#2: Node cards must precede element cards in this version
//*NODE, SYSTEM=R, NSET=NDALL, INPUT=testingVDT_node.inp
//*ELEMENT,TYPE=C3D8R,ELSET=ALLEL, INPUT=testingVDT_element.inp
//
// Note#3: iswtype=0->Abaqus *.inp;1->Abaqus *.abq
// Multiple node & element INCLUDEs are accommodated.
// nodelolim,nodeuplim,eluplim begin with 1 (not 0)
// DIRE WARNING: "Node Prin/Node Out/etc" cards MUST PRECEDE "Node" card below, because of "No" detection
//               Ditto for Ele & Material, etc
// Note current convention: *.msh & Simulia/Abaqus *.inp/*.abq files contain weld groups (never weld passes), regardless of name
//     Hence wp.nWeldGroup is incremented but not wp.nWeldPass.
//
// ProcessAllWG() above was an attempt to accommodate "multiple base metal material ELSETS" but
//   it crashes large Abaqus models??? EFP 6/19/2014
//
// FRANTIC NOTE: Cartesian frame only. This version does NOT currently support Abaqus datacards
// *SYSTEM
// *TIE, etc
//
 int nic=0,nic1=0,nrc=0,jsw=0,iswNode=0,iswElem=0, *attendEl=NULL;
 long
// itype=0,
in=0,kn=0,klim=0,
//n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,
n8=0,
//dresno=0,sresno=0,
dummy=0,
//ltype=0,
i=0,j=0,k=0,kk=0,kp=0,jrec=0,eltype=0,bscode=0,node=0,t7=10000000,t5=100000,t3=1000,larr[10],larr1[10]
//*GIDarr=NULL,
//ipid=0,MXNPELX=0
//,MUL=1,inpGIDmax= -1,matstep=0
//,nodeuplim=0,totNnum=0,eluplim=0,totEnum=0,nGIDmax=0;
,nodeuplim=0,nodelolim=0,totNnum=0,eluplim=0,ellolim=0,totEnum=0,sumWG=0,sumlim=0,sumELSETel=0,totBMG=0,totWG=0
, *revnode_map=NULL;
// float r1=0.,r2=0.,r3=0.,r4=0.,r5=0.,r6=0.,darr[10];
 float darr[10];
 char cht[200],extensChar[]=".inp"
 //,chELSET[78+1]
 , *temp_cht=NULL, *fnNeed1=NULL,*fnNeed2=NULL,
	  ch_I='I',ch_i='i',ch_N='N',ch_n='n',ch_P='P',ch_p='p',ch_U='U',ch_u='u',ch_T='T',ch_t='t',ch_eq='=';
 wchar_t string0[11];
////////////////
//String *tw_groupsname=NULL;
////////////////
 if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First, close current file->FileClose",L"Halt",MB_OK);}
 else {  //OPEN02
 base.matsteps=base.ncoorf=1;
 base.npoin=base.nelt=base.nvfix=base.nedge=base.pload=base.mat=base.nblod=0;
 base.allGrp=1; //Try insisting on a base group???
 base.ELSETelsum=MXNPEL=wp.nWeldGroup=0; //Establish MXNPEL
 OpenDialog1->Filter= "SIMULIA/ABAQ (*.abq/*.inp)|*.inp;*.ABQ";

//// if(iswtype)OpenDialog1->Filter= "SIMULIA_Abq (*.abq)|*.abq;*.ABQ|SIMULIA_Abq (*.inp)|*.inp;*.INP";
//// else       OpenDialog1->Filter= "SIMULIA_Abq (*.inp)|*.inp;*.INP|SIMULIA_Abq (*.abq)|*.abq;*.ABQ";
// if(iswtype)OpenDialog1->Filter= "SIMULIA_Abq (*.abq)|*.abq;*.ABQ";
// else       OpenDialog1->Filter= "SIMULIA_Abq (*.inp)|*.inp;*.INP";
/////////////////////////////////////
 if(OpenDialog1->Execute())
//   {ifstream ntape2(OpenDialog1->FileName.t_str(),ios::nocreate|ios::binary,0); //OPEN03
   {ifstream ntape2(OpenDialog1->FileName.w_str(),ios::nocreate|ios::binary,0); //OPEN03
	if(ntape2)
	  { //OPEN04

gWsiAlias=(String)modelName_g; // where char modelName_g[260] in *.h
//honk<<gWsiAlias.t_str()<<" gWsiAliasImportAbmmmmmm\n";
// Perhaps the above should be moved within   if(ntape){  ??? EFP 2/27/2012
///////////////////////////// end

	   ofstream tmpfile("record.tmp",ios::binary,0); //Sanitize by writing file without comment/blank lines
	   if(tmpfile)
		 {do {ntape2.getline(cht,200-1);  //OPEN05
			  if((cht[0]=='*' && cht[1]=='*') || strlen(cht)<2)continue;
			  else tmpfile<<cht<<"\n";
			 }
		  while (!ntape2.eof());
		  tmpfile.close();
		  ifstream ntape("record.tmp",ios::nocreate|ios::binary,0);
		  if(ntape)
//
	  {  //OPEN06
//////////////////////////////////////////////////////////////
//TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//////////////////////////////////////////////////////////////
	   nodeuplim=totNnum=eluplim=totEnum=0;nodelolim=ellolim=LONG_INT;
	   do {ntape.getline(cht,200-1);  //START_DO01
		   if(cht[0]=='*' && cht[1]=='*')continue; //Comment ** & ***include & ***ORIENTATION
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='O' || cht[6]=='o') && (cht[7]=='U' || cht[7]=='u') && (cht[8]=='T' || cht[8]=='t') && (cht[9]=='P' || cht[9]=='p'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *NODE OUTP
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='F' || cht[6]=='f') && (cht[7]=='I' || cht[7]=='i') && (cht[8]=='L' || cht[8]=='l') && (cht[9]=='E' || cht[9]=='e'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *NODE FILE
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='P' || cht[6]=='p') && (cht[7]=='R' || cht[7]=='r') && (cht[8]=='I' || cht[8]=='i') && (cht[9]=='N' || cht[9]=='n'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *NODE PRIN
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='R' || cht[6]=='r') && (cht[7]=='E' || cht[7]=='e') && (cht[8]=='S' || cht[8]=='s') && (cht[9]=='P' || cht[9]=='p'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *NODE RESP
////		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o'))
//		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==',')
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e')) //comma unnecessary because of above NODE XXXX
				  {iswNode=0;
int ck_INPUT=0;
for(j=8;j<int(strlen(cht))-4;j++){


							 ck_INPUT=0;
							 if(cht[j-5]==ch_I || cht[j-5]==ch_i)ck_INPUT++;
							 if(cht[j-4]==ch_N || cht[j-4]==ch_n)ck_INPUT++;
							 if(cht[j-3]==ch_P || cht[j-3]==ch_p)ck_INPUT++;
							 if(cht[j-2]==ch_U || cht[j-2]==ch_u)ck_INPUT++;
							 if(cht[j-1]==ch_T || cht[j-1]==ch_t)ck_INPUT++;
							 if(cht[j]==ch_eq)ck_INPUT++;
							 if(ck_INPUT==6)
											{iswNode=1;
											 for(in=j+1;in<int(strlen(cht))-1;in++)if(cht[in]=='.')break;
											 fnNeed1=new char[in-j-1+strlen(extensChar)+1];
											 for(kk=j+1;kk<in;kk++)fnNeed1[kk-j-1]=cht[kk];
//											 StringCchCat(fnNeed1,in-j-1+strlen(extensChar)+1,extensChar);

////for(k=0;k<in-j-1+strlen(extensChar)+1;k++)honk<<fnNeed1[k]<<" fnNeed1\n";
////if(k> -1)exit(0);
											 fnNeed1[in+0-j-1]='.';fnNeed1[in+1-j-1]='i';fnNeed1[in+2-j-1]='n';fnNeed1[in+3-j-1]='p';fnNeed1[in+4-j-1]='\0';
											 ifstream viewfile1(fnNeed1,ios::nocreate);
											 if(viewfile1){
//honk<<" starting to read NODE file\n";
														   do {viewfile1.getline(cht,200-1);
//															   parse_cdm(cht,4,&nic,&nrc,larr,darr); // *NODE
															   if(strlen(cht))
																 {parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
/////////////////////
//honk<<larr[0]<<" velaq "<<darr[0]<<" "<<darr[1]<<" "<<darr[2]<<"\n";
//if(1==1)exit(0);
/////////////////////
																  if(nodeuplim<larr[0])nodeuplim=larr[0];
																  if(nodelolim>larr[0])nodelolim=larr[0];
																  totNnum++;
																 }
															  }
//														   while (viewfile1.peek()!= '*');
														   while (!viewfile1.eof());
//honk<<" finished to read NODE file\n";
														   viewfile1.close();
//totNnum--;
//honk<<totNnum<<" First totNnum from node coord file\n";
														  }
											 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Node *.inp file not found",L"Terminate",MB_OK);exit(0);}
											 delete [] fnNeed1; *fnNeed1=NULL; // NODE, stored in fnNeed1 file
											 break;
											}
							}
				   if(!iswNode)
					 {do {ntape.getline(cht,200-1);
////						  parse_cdm(cht,4,&nic,&nrc,larr,darr); // *NODE
//						  parse_cdm3f(cht,4,&nic,&nrc,larr,darr);
						  parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
						  if(nodeuplim<larr[0])nodeuplim=larr[0];
						  if(nodelolim>larr[0])nodelolim=larr[0];
						  totNnum++; // This totNum might be +1 wrong.
//honk<<nodeuplim<<" "<<nodelolim<<" "<<totNnum<<" "<<cht<<" Node-1st pass\n";

						 }
					  while (ntape.peek()!= '*');
					 }
				  }


		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='O' || cht[ 9]=='o') && (cht[10]=='U' || cht[10]=='u') && (cht[11]=='T' || cht[11]=='t') && (cht[12]=='P' || cht[12]=='p') && (cht[13]=='U' || cht[13]=='u') && (cht[14]=='T' || cht[14]=='t'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *ELEMENT OUTPUT
		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='M' || cht[ 9]=='m') && (cht[10]=='A' || cht[10]=='a') && (cht[11]=='T' || cht[11]=='t') && (cht[12]=='R' || cht[12]=='r') && (cht[13]=='I' || cht[13]=='i') && (cht[14]=='X' || cht[14]=='x'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *ELEMENT MATRIX
		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='R' || cht[ 9]=='r') && (cht[10]=='E' || cht[10]=='e') && (cht[11]=='S' || cht[11]=='s') && (cht[12]=='P' || cht[12]=='p') && (cht[13]=='O' || cht[13]=='o') && (cht[14]=='N' || cht[14]=='n'))
				  {while (ntape.peek()!= '*')ntape.getline(cht,200-1);} // *ELEMENT RESPON
////		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e'))
//		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='M' || cht[4]=='m') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='N' || cht[6]=='n') && (cht[7]=='T' || cht[7]=='t') && cht[8]==',')
		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='M' || cht[4]=='m') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='N' || cht[6]=='n') && (cht[7]=='T' || cht[7]=='t'))//comma not necessary
																			{iswElem=0;
for(j=8;j<int(strlen(cht))-1;j++)if((cht[j-5]=='I' || cht[j-5]=='i') &&
							   (cht[j-4]=='N' || cht[j-4]=='n') &&
							   (cht[j-3]=='P' || cht[j-3]=='p') &&
							   (cht[j-2]=='U' || cht[j-2]=='u') &&
							   (cht[j-1]=='T' || cht[j-1]=='t') &&
								cht[j]=='='){
											 iswElem=1;
											 for(in=j+1;in<int(strlen(cht))-1;in++)if(cht[in]=='.')break;
											 fnNeed2=new char[in-j-1+strlen(extensChar)+1];
											 for(kk=j+1;kk<in;kk++)fnNeed2[kk-j-1]=cht[kk]; //StringCchCat(fnNeed2,in-j-1+strlen(extensChar)+1,extensChar);
											 fnNeed2[in+0-j-1]='.';fnNeed2[in+1-j-1]='i';fnNeed2[in+2-j-1]='n';fnNeed2[in+3-j-1]='p';fnNeed2[in+4-j-1]='\0';
											 ifstream viewfile2(fnNeed2,ios::nocreate);
											 if(viewfile2){
//honk<<" starting to read ELEM file\n";
														   do {viewfile2.getline(cht,200-1);
															   for(i=0;i<10;i++)larr[i]=0;
															   parse_cdmQ(cht,&nic,&nrc,larr,darr); // *ELEMENT, stored in fnNeed2 file
//honk<<larr[0]<<" trackEfile "<<nic<<" "<<nrc<<" "<<strlen(cht)<<"\n";
//honk<<larr[1]<<" "<<larr[2]<<" "<<larr[3]<<" "<<larr[4]<<" "<<larr[5]<<" "<<larr[6]<<" "<<larr[7]<<" "<<larr[8]<<"\n";
//if(1==1)exit(0);
															   if(nic-1==4) //Caution: eluplim & ellolim begin with 1
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Correction EFP 6/28/2011
																 }
															   else if(nic-1==5)
																 {nic=5;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==6)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==7)
																 {nic=7;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==8)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(!larr[8])viewfile2.getline(cht,200-1); //Read extra line but assume NX 8-n  EFP 4/05/2012
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==9)
																 {nic=9;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else break; //Preceding abort does not work???
															  }
														   while (!viewfile2.eof());
//honk<<" finished to read ELEM file\n";
														   viewfile2.close();
														  }
											 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Element *.inp file not found",L"Terminate",MB_OK);exit(0);}
											 delete [] fnNeed2; *fnNeed2=NULL; // NODE, stored in fnNeed2 file
											 break;
											}
//////////
																			 if(!iswElem)
																			   {do {ntape.getline(cht,200-1); // This might be +1 wrong
																					for(i=0;i<10;i++)larr[i]=0;
																					parse_cdmQ(cht,&nic,&nrc,larr,darr); // *ELEMENT
//honk<<larr[0]+1<<" trackE "<<nic<<"\n";
															   if(nic-1==4)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Correction EFP 6/28/2011
																 }
															   else if(nic-1==5)
																 {nic=5;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==6)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==7)
																 {nic=7;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==8)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(!larr[8])ntape.getline(cht,200-1); //Read extra line but assume NX 8-n  EFP 4/05/2012
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else if(nic-1==9)
																 {nic=9;totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
//															   else {honk<<"Halt1: Unsupported element with #nodes "<<nic-1<<"\n";
//																	 extern PACKAGE void __fastcall Beep(void);Application->MessageBox(_ltow(nic-1,string0,10),L"Halt1: Unsupported element with #nodes",MB_OK);
//																	 exit(0);
//																	}
															   else break; //Preceding abort does not work???



																				   }
																				while (ntape.peek()!= '*');
																			   }
																			}
		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='S' || cht[3]=='s'))
//Caution: ELSET of ELSETs unsupported
//Caution: Remember that "GENERATE & SYSTEM & other" can occur in any order
//Caution: Remember that strlen() already excludes end-of-line, so check that the following is correct. EFP 6/19/2014
				  {
				   for(i=7;i<int(strlen(cht))-1;i++)if(cht[i]=='=')break;
				   for(jrec=i+1;jrec<int(strlen(cht))-1;jrec++)if(cht[jrec]!=' ')break;
				   k=0;
///////////////////
				   for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='D' || cht[i+4]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWD
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='E' || cht[i+4]=='e') &&
																		  (cht[i+5]=='L' || cht[i+5]=='l') &&
																		  (cht[i+6]=='D' || cht[i+6]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWELD
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='A' || cht[i+2]=='a') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject ElAll
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='E' || cht[i+3]=='e') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllEl
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='A' || cht[i+1]=='a') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject EAll  EFP 4/08/2012
						 }
///////////////////
				   jsw=0;
				   if(k==0){base.allGrp=base.allGrp+1;nGID=nGID+1;
							for(i=jrec;i<int(strlen(cht))-1;i++){if(cht[i  ]=='W' || cht[i  ]=='w')
														   {if(i+1<int(strlen(cht))){if(cht[i+1]=='D' || cht[i+1]=='d'){jsw=1;break;}
																				else if(cht[i+1]=='P' || cht[i+1]=='p'){jsw=1;break;}
																				else if(cht[i+1]=='G' || cht[i+1]=='g'){jsw=1;break;}
																				else if(cht[i+1]=='E' || cht[i+1]=='e')
																					   {if(i+3<int(strlen(cht)))
																						  {if((cht[i+2]=='L' || cht[i+2]=='l') &&
																							  (cht[i+3]=='D' || cht[i+3]=='d')){jsw=1;break;}
																						  }
																						else break;
																					   }
																				else break;
																			   }
															else break;
														   }  // Accept WD, WP, WG and WELD
																}
							if(jsw)wp.nWeldGroup=wp.nWeldGroup+1;
						   }
////////////////////////
				   do {ntape.getline(cht,200-1);
///////////////// Start EMERGENCY check to exclude ELSET alphabetic data  EFP 4/23/2011
					   for(i=0;i<int(strlen(cht))-1;i++)
						 {if(cht[i]==',' || cht[i]==' ' || cht[i]=='0' || cht[i]=='1' || cht[i]=='2' || cht[i]=='3' || cht[i]=='4' ||
														   cht[i]=='5' || cht[i]=='6' || cht[i]=='7' || cht[i]=='8' || cht[i]=='9')continue;
						  else {
//						        extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Please remove unsupported *ELSET card with non-numeric data from *.abq/*.inp",L"Terminate",MB_OK);exit(0);
honk<<"\n"<<cht<<" Warning: ELSET of ELSETs datacard found\n";break;
							   }
						 }
///////////////// End
					  } // *ELSET
				   while (ntape.peek()!= '*');
				  }
		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='N' || cht[2]=='n') && (cht[3]=='D' || cht[3]=='d'))
				  {if(ntape.peek()!= '*')break; // multiple use for *End/*End Part/*End Assembly/*End Instance/*End Step  EFP 4/22/2011
				   else continue;
				  }
				   // *end step CORRECTED EFP 10/22/2010
		   else {if(iswtype)honk<<"WARNING: The following unsupported datacard found in *.abq\n";
				 else       honk<<"WARNING: The following unsupported datacard found in *.inp\n";
				 honk<<cht[0]<<" "<<cht[1]<<" "<<cht[2]<<" "<<cht[3]<<"\n";
				 while (ntape.peek()!= '*')ntape.getline(cht,200-1);
				}
		  }
	   while (!ntape.eof()); //END_DO01
///////////////////////////
//Screen->Cursor=Save_Cursor;
///////////////////////////
	   ntape.close();
//	   base.matsteps=matstep;
honk<<nodeuplim<<" "<<nodelolim<<" "<<totNnum<<" "<<eluplim<<" "<<ellolim<<" "<<totEnum<<" "<<MXNPEL<<" DDDDDDDD\n";
//if(1==1)exit(0);

	   if(wp.nWeldGroup==0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld groups (*ELSET, ELSET=...weld...) found in *.abq",L"Terminate",MB_OK);exit(0);}
honk<<base.allGrp<<" "<<wp.nWeldGroup<<" Early A & WG\n";
//if(1==1)exit(0);

GeomFileName=OpenDialog1->FileName;
//	   base.npoin=nodeuplim;
	   base.npoin=totNnum;
//	   base.nelt=eluplim;
	   base.nelt=totEnum; //Policy: Reserve storage for #elements read-in, even if there is duplication  EFP 4/19/2012
//
////
////// Integrity test for WARP3D unitary-start consecutive numbering
	   if(nodeuplim != totNnum){honk<<nodeuplim<<"TERMINATE: Nonconsecutive node numbers in file "<<totNnum<<"\n";}
	   if(eluplim != totEnum){honk<<eluplim<<"TERMINATE: Nonconsecutive/duplicate element numbers in file "<<totEnum<<"\n";}
	   if(nodeuplim != totNnum && eluplim != totEnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive elements & nodes found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
	   else if(nodeuplim != totNnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive nodes found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
	   else if(eluplim != totEnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive elements found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
//////
////
//
	   if(base.nelt> LONG_INT/t3){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Excessive #elements in geometry file",L"Terminate",MB_OK);exit(0);}
	   if(base.npoin>0 && base.nelt>0) //StartNPOIN/NELT
		 {FDdynmem_manage(1,base.npoin,base.nelt,dummy,dummy,dummy,base.npoin,dummy,dummy,dummy,dummy,dummy,dummy,MXNPEL);
//		  FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
		  FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,wp.nWeldGroup+1);
		  FDdynmem_manage(15,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);//EFP 8/07/2011
//		  base.groupsname[0]=L"ElAll"; //EFP 10/23/2011
//		  base.groupsname[base.allGrp-wp.nWeldGroup-1]=L"AllWeld"; //EFP 10/23/2011
////		  ifstream ntape1(OpenDialog1->FileName.t_str(),ios::nocreate|ios::binary,0);
		  ifstream ntape1("record.tmp",ios::nocreate|ios::binary,0);
		  if(ntape1) //seek() can be used with binary-opened files (NOT ascii) so close & reopen file  EFP 12/18/2011
			{  //StartReopen02
//////////////////////////////////////////////////////////////
//TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//////////////////////////////////////////////////////////////
//			 ipid=nGID=1;  //Assumption: All elements start with GID=1
			 nGID=1;
//			 wp.nWeldGroup=0;
			 totNnum=totEnum=sumELSETel=sumlim=0;
//			 totBMG=0;totWG= -1;//EFP 10/22/2011
			 totBMG=0;totWG=0;//EFP 10/22/2011
			 for(in=0;in<NDF*base.npoin;in++)base.c1[in]=0.f;
//vvvvvvvvvvvvvvvvvvv
			 for(in=0;in<2*base.npoin;in++)base.nofix[in]=0;
			 for(in=0;in<base.npoin;in++)base.nrfix[in]=0;
			 for(in=0;in<NDF*base.npoin;in++)base.presc[in]=0.f;
//vvvvvvvvvvvvvvvvvvv
			 for(in=0;in<base.nelt;in++)base.arELEM[in]=1;
//			 for(in=0;in<base.nelt;in++)base.el_map[in]= -1;
//			 for(in=0;in<base.npoin;in++)base.node_map[in]= -1;
//////////// EFP 4/01/2011
revnode_map=new long[nodeuplim-nodelolim+1];
//			 temp_allGID=new int[base.allGrp*base.nelt]; //No WP in ImportAba()
////			   temp_orgGID=new int[base.allGrp]; //EFP 3/11/2012
//			 for(in=0;in<base.allGrp*base.nelt;in++)temp_allGID[in]=0;
//			 for(in=0;in<base.nelt;in++)temp_allGID[in]=1;
////			   for(in=0;in<base.allGrp;in++)temp_orgGID[in]=0;
			 attendEl=new int[eluplim-ellolim+1];
			 for(in=0;in<eluplim-ellolim+1;in++)attendEl[in]=0;
//			 for(in=0;in<base.npoin;in++)base.arrELSET[in]=0;
			 for(in=0;in<base.nelt;in++)base.arrELSET[in]=0; //Correction EFP 1/14/2015
////////////

honk<<base.allGrp<<" "<<wp.nWeldGroup<<" EEELate A & WG\n";
honk<<base.npoin<<" "<<base.nelt<<" npoin/nelt\n";

//if(1==1)exit(0);
////////////
			 do {ntape1.getline(cht,200-1); //StartDO02

				 if(cht[0]=='*' && cht[1]=='*')continue; //Comment ** & ***include & ***ORIENTATION
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='O' || cht[6]=='o') && (cht[7]=='U' || cht[7]=='u') && (cht[8]=='T' || cht[8]=='t') && (cht[9]=='P' || cht[9]=='p'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *NODE OUTP
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='F' || cht[6]=='f') && (cht[7]=='I' || cht[7]=='i') && (cht[8]=='L' || cht[8]=='l') && (cht[9]=='E' || cht[9]=='e'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *NODE FILE
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='P' || cht[6]=='p') && (cht[7]=='R' || cht[7]=='r') && (cht[8]=='I' || cht[8]=='i') && (cht[9]=='N' || cht[9]=='n'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *NODE PRIN
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==' ' &&
								  (cht[6]=='R' || cht[6]=='r') && (cht[7]=='E' || cht[7]=='e') && (cht[8]=='S' || cht[8]=='s') && (cht[9]=='P' || cht[9]=='p'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *NODE RESP
////		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o'))
//		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e') && cht[5]==',')
		   else if(cht[0]=='*' && (cht[1]=='N' || cht[1]=='n') && (cht[2]=='O' || cht[2]=='o') && (cht[3]=='D' || cht[3]=='d') && (cht[4]=='E' || cht[4]=='e'))
//// Dire warning: Never have a space between number & trailing comma, or an extra 0. will be inferred, as follows:
//*NODE,
//           1,   0.0000000E+00,   0.0000000E+00,   0.0000000E+00
//           2,   0.5000000    ,   0.0000000E+00,   0.0000000E+00
//           3,    1.000000    ,   0.0000000E+00,   0.0000000E+00
//           4,    1.500000    ,   0.0000000E+00,   0.0000000E+00
//           5,    2.000000    ,   0.0000000E+00,   0.0000000E+00
//// This must be corrected to
//*NODE,
//           1,   0.0000000E+00,   0.0000000E+00,   0.0000000E+00
//           2,   0.5000000,   0.0000000E+00,   0.0000000E+00
//           3,    1.000000,   0.0000000E+00,   0.0000000E+00
//           4,    1.500000,   0.0000000E+00,   0.0000000E+00
//           5,    2.000000,   0.0000000E+00,   0.0000000E+00
						{iswNode=0;
int ck_INPUT=0;
for(j=8;j<int(strlen(cht))-4;j++){


							 ck_INPUT=0;
							 if(cht[j-5]==ch_I || cht[j-5]==ch_i)ck_INPUT++;
							 if(cht[j-4]==ch_N || cht[j-4]==ch_n)ck_INPUT++;
							 if(cht[j-3]==ch_P || cht[j-3]==ch_p)ck_INPUT++;
							 if(cht[j-2]==ch_U || cht[j-2]==ch_u)ck_INPUT++;
							 if(cht[j-1]==ch_T || cht[j-1]==ch_t)ck_INPUT++;
							 if(cht[j]==ch_eq)ck_INPUT++;
							 if(ck_INPUT==6)
											{iswNode=1;
											 for(in=j+1;in<int(strlen(cht))-1;in++)if(cht[in]=='.')break;
											 fnNeed1=new char[in-j-1+strlen(extensChar)+1];
											 for(kk=j+1;kk<in;kk++)fnNeed1[kk-j-1]=cht[kk]; //StringCchCat(fnNeed1,in-j-1+strlen(extensChar)+1,extensChar);
											 fnNeed1[in+0-j-1]='.';fnNeed1[in+1-j-1]='i';fnNeed1[in+2-j-1]='n';fnNeed1[in+3-j-1]='p';fnNeed1[in+4-j-1]='\0';
											 ifstream viewfile3(fnNeed1,ios::nocreate);
											 if(viewfile3){
//honk<<" reenter NODEfile\n";
														   do {viewfile3.getline(cht,200-1); //parse_cdm(cht,4,&nic,&nrc,larr,darr); // *NODE
															   if(strlen(cht))
																{parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
																 in=larr[0]-1;base.c1[NDF*totNnum]=darr[0];base.c1[NDF*totNnum+1]=darr[1];base.c1[NDF*totNnum+2]=darr[2];
																 base.node_map[totNnum]=in;
																 revnode_map[in-nodelolim+1]=totNnum;
																 totNnum++;
																}
															   else break;
															  }
//														   while (viewfile1.peek()!= '*');
														   while (!viewfile3.eof());
//honk<<" releave NODEfile\n";
														   viewfile3.close();
														  }
											 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Node *.inp file not found",L"Terminate",MB_OK);exit(0);}
											 delete [] fnNeed1; *fnNeed1=NULL; // NODE, stored in fnNeed1 file
											 break;
											}
							}
				   if(!iswNode)
					 {do {ntape1.getline(cht,200-1); //parse_cdm(cht,4,&nic,&nrc,larr,darr); // *NODE, stored in same file
						  parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
						  in=larr[0]-1;base.c1[NDF*totNnum]=darr[0];base.c1[NDF*totNnum+1]=darr[1];base.c1[NDF*totNnum+2]=darr[2];
						  base.node_map[totNnum]=in;  // Check this
						  revnode_map[in-nodelolim+1]=totNnum;

///////////////// start trash
//honk<<totNnum+1<<" "<<in+1<<" "<<in-nodelolim+1<<" ReadNODE "<<base.c1[NDF*totNnum]<<" "<<base.c1[NDF*totNnum+1]<<" "<<base.c1[NDF*totNnum+2]<<"\n";
///////////////// end trash

						  totNnum++;
						 }
					  while (ntape1.peek()!= '*');
					 }
						}
		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='O' || cht[ 9]=='o') && (cht[10]=='U' || cht[10]=='u') && (cht[11]=='T' || cht[11]=='t') && (cht[12]=='P' || cht[12]=='p') && (cht[13]=='U' || cht[13]=='u') && (cht[14]=='T' || cht[14]=='t'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *ELEMENT OUTPUT
		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='M' || cht[ 9]=='m') && (cht[10]=='A' || cht[10]=='a') && (cht[11]=='T' || cht[11]=='t') && (cht[12]=='R' || cht[12]=='r') && (cht[13]=='I' || cht[13]=='i') && (cht[14]=='X' || cht[14]=='x'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *ELEMENT MATRIX
		   else if(cht[ 0]=='*' && (cht[ 1]=='E' || cht[ 1]=='e') && (cht[ 2]=='L' || cht[ 2]=='l') && (cht[ 3]=='E' || cht[ 3]=='e') && (cht[ 4]=='M' || cht[ 4]=='m') && (cht[ 5]=='E' || cht[ 5]=='e') && (cht[ 6]=='N' || cht[ 6]=='n') && (cht[ 7]=='T' || cht[ 7]=='t') && cht[ 8]==' ' &&
								   (cht[ 9]=='R' || cht[ 9]=='r') && (cht[10]=='E' || cht[10]=='e') && (cht[11]=='S' || cht[11]=='s') && (cht[12]=='P' || cht[12]=='p') && (cht[13]=='O' || cht[13]=='o') && (cht[14]=='N' || cht[14]=='n'))
				  {while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);} // *ELEMENT RESPON
////		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e'))
//		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='M' || cht[4]=='m') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='N' || cht[6]=='n') && (cht[7]=='T' || cht[7]=='t') && cht[8]==',')
		   else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='M' || cht[4]=='m') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='N' || cht[6]=='n') && (cht[7]=='T' || cht[7]=='t')) //comma not necessary
						{


																			iswElem=0;
for(j=8;j<int(strlen(cht))-1;j++)if((cht[j-5]=='I' || cht[j-5]=='i') &&
							   (cht[j-4]=='N' || cht[j-4]=='n') &&
							   (cht[j-3]=='P' || cht[j-3]=='p') &&
							   (cht[j-2]=='U' || cht[j-2]=='u') &&
							   (cht[j-1]=='T' || cht[j-1]=='t') &&
								cht[j]=='='){
								iswElem=1;
											 for(in=j+1;in<int(strlen(cht))-1;in++)if(cht[in]=='.')break;
											 fnNeed2=new char[in-j-1+strlen(extensChar)+1];
											 for(kk=j+1;kk<in;kk++)fnNeed2[kk-j-1]=cht[kk]; //StringCchCat(fnNeed2,in-j-1+strlen(extensChar)+1,extensChar);
											 fnNeed2[in+0-j-1]='.';fnNeed2[in+1-j-1]='i';fnNeed2[in+2-j-1]='n';fnNeed2[in+3-j-1]='p';fnNeed2[in+4-j-1]='\0';
											 ifstream viewfile4(fnNeed2,ios::nocreate);
											 if(viewfile4){
//honk<<" reenter ELEMfile\n";
														   do {viewfile4.getline(cht,200-1);
															   if(strlen(cht))
																{
																 for(i=0;i<10;i++)larr[i]=0;
//																 parse_cdmQ(cht,9,&nic,&nrc,larr,darr,strlen(cht)); // *ELEMENT, stored in fnNeed2 file
																 parse_cdmQn(cht,25,&nic,&nrc,larr,darr);
//honk<<larr[0]<<" track2Efile "<<nic<<"\n";
															   if(nic-1==4)eltype=5;
															   else if(nic-1==5){nic=5;eltype=5;}
															   else if(nic-1==6)eltype=7;
															   else if(nic-1==7){nic=7;eltype=7;}
															   else if(nic-1==8)
																 {eltype=8;
																  if(!larr[8]){viewfile4.getline(cht,200-1); //Read extra line but assume NX 8-n  EFP 4/05/2012
																			   parse_cdmQn(cht,25,&nic1,&nrc,larr1,darr);
																			   larr[8]=larr1[0];
																			  }
																 }
															   else if(nic-1==9){nic=9;eltype=8;}
															   else {honk<<"Halt2: Unsupported element with #nodes "<<(nic-1)<<"\n";
																	 extern PACKAGE void __fastcall Beep(void);Application->MessageBox(_ltow(nic-1,string0,10),L"Halt2: Unsupported element with #nodes in ImportAba_prog()",MB_OK);
																	 exit(0);
																	}

																 n8=nic-1;in=larr[0]-1;
/////////////// start New code to manage element duplication  EFP 4/19/2012
if(attendEl[in-ellolim+1])attendEl[in-ellolim+1]= -1;
else {attendEl[in-ellolim+1]=1;

//if(n8==8) //EFP 12/19/2011
//  {if(larr[0+1]==larr[4+1] && larr[3+1]==larr[7+1])
///////////////////////////// Coding to accommodate "degenerate hex" wedges  EFP 4/14/2011
////17619, 23561, 23562, 23592, 23591, 19210, 19211, 19241, 19240
////17620, 23562, 22301, 22302, 23592, 19211, 17950, 17951, 19241
////17621,   571, 23563, 22311,    82,   571, 19212, 17960,    82   This one in *.inp & *.abq
////17622, 23563, 23564, 22310, 22311, 19212, 19213, 17959, 17960
////17623, 23564, 23565, 22309, 22310, 19213, 19214, 17958, 17959
//	{eltype=7;n8=6;
//	 larr[4+1]=larr[6+1];i=larr[1+1];larr[1+1]=larr[5+1];larr[5+1]=larr[2+1];larr[2+1]=i;//larr[6+1]=larr[7+1]=0;
//honk<<in+1<<" degenerate hex as wedge\n";
//	}
//   else if(larr[4+1]==larr[5+1] && larr[4+1]==larr[6+1] && larr[4+1]==larr[7+1])
///////////////////////////// Coding to accommodate tetras presented as 8-n  EFP 12/19/2011
////20000,20259,20260,20261,20262,1,1,1,1
////20001,20263,20264,20265,20266,1,1,1,1
////20002,20265,20267,20266,20268,1,1,1,1
//	{eltype=5;n8=4;//for(i=4+1;i<8+1;i++)larr[i]=0;
//honk<<in+1<<" tetra presented as 8-n\n";
//	}
//  }
if(n8==8)degen8_test(&eltype,&n8,larr);
																 for(i=0;i<n8;i++)base.nop1[MXNPEL*totEnum+i]=larr[i+1]-1;
//if(n8==8)
//  {if(larr[0+1]==larr[4+1] && larr[3+1]==larr[7+1])
///////////////////////////// Coding to accommodate "degenerate hex" wedges  EFP 4/14/2011
////17619, 23561, 23562, 23592, 23591, 19210, 19211, 19241, 19240
////17620, 23562, 22301, 22302, 23592, 19211, 17950, 17951, 19241
////17621,   571, 23563, 22311,    82,   571, 19212, 17960,    82   This one in *.inp & *.abq
////17622, 23563, 23564, 22310, 22311, 19212, 19213, 17959, 17960
////17623, 23564, 23565, 22309, 22310, 19213, 19214, 17958, 17959
//	{eltype=7;n8=6;
//	 base.nop1[MXNPEL*totEnum+0]=larr[0+1]-1;
//	 base.nop1[MXNPEL*totEnum+1]=larr[5+1]-1;
//	 base.nop1[MXNPEL*totEnum+2]=larr[1+1]-1;
//	 base.nop1[MXNPEL*totEnum+3]=larr[3+1]-1;
//	 base.nop1[MXNPEL*totEnum+4]=larr[6+1]-1;
//	 base.nop1[MXNPEL*totEnum+5]=larr[2+1]-1;
//	 base.nop1[MXNPEL*totEnum+6]=base.nop1[MXNPEL*totEnum+7]= -1;
//	}
//   else if(larr[4+1]==larr[5+1] && larr[4+1]==larr[6+1] && larr[4+1]==larr[7+1] && larr[4+1]==larr[8+1])
///////////////////////////// Coding to accommodate tetras presented as 8-n  EFP 12/19/2011
////20000,20259,20260,20261,20262,1,1,1,1
////20001,20263,20264,20265,20266,1,1,1,1
////20002,20265,20267,20266,20268,1,1,1,1
//	{eltype=5;n8=4;for(i=4;i<8;i++)base.nop1[MXNPEL*totEnum+i]= -1;
//	}
//  }
/////////////////////////////
//																 base.matno[totEnum]=eltype*t7+n8*t3+ipid-1;
																 base.matno[totEnum]=eltype*t7+n8*t3;
																 base.el_map[totEnum]=in;
//////////// EFP 1/30/2011
//base.orig_matno[totEnum]=eltype*t7+n8*t3+ipid-1;
base.orig_matno[totEnum]=eltype*t7+n8*t3;
////////////
																 totEnum++;
	 }
/////////////// end
																}
															   else break;

															  }
														   while (!viewfile4.eof());
//honk<<" releave ELEMfile\n";
														   viewfile4.close();
														  }
											 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Element *.inp file not found",L"Terminate",MB_OK);exit(0);}
											 delete [] fnNeed2; *fnNeed2=NULL; // NODE, stored in fnNeed2 file
											 break;
											}
//////////
																			 if(!iswElem)
																			   {
							do {ntape1.getline(cht,200-1);
//honk<<" NonINPUTFILEelem "<<cht<<"\n";
								for(i=0;i<10;i++)larr[i]=0;
								parse_cdmQ(cht,&nic,&nrc,larr,darr); // *ELEMENT, stored in same file
//honk<<larr[0]<<" track2E "<<nic<<"\n";

															   if(nic-1==4)eltype=5;
															   else if(nic-1==5){nic=5;eltype=5;}
															   else if(nic-1==6){eltype=7;
//honk<<larr[0]<<" C3D6 "<<larr[1]<<" "<<larr[2]<<" "<<larr[3]<<" "<<larr[4]<<" "<<larr[5]<<" "<<larr[6]<<"\n";
																				}
															   else if(nic-1==7){nic=7;eltype=7;}
															   else if(nic-1==8)
																 {eltype=8;
																  if(!larr[8]){ntape1.getline(cht,200-1); //Read extra line but assume NX 8-n  EFP 4/05/2012
																			   parse_cdmQn(cht,25,&nic1,&nrc,larr1,darr);
																			   larr[8]=larr1[0];
																			  }
																 }
															   else if(nic-1==9){nic=9;eltype=8;}
															   else {honk<<"Halt3: Unsupported element with #nodes "<<(nic-1)<<"\n";
																	 extern PACKAGE void __fastcall Beep(void);Application->MessageBox(_ltow(nic-1,string0,10),L"Halt3: Unsupported element with #nodes in ImportAba_prog()",MB_OK);
																	 exit(0);
																	}



								n8=nic-1;in=larr[0]-1;
/////////////// start New code to manage element duplication  EFP 4/19/2012
if(attendEl[in-ellolim+1])attendEl[in-ellolim+1]= -1;
else {attendEl[in-ellolim+1]=1;

//if(n8==8) //EFP 12/19/2011
//  {if(larr[0+1]==larr[4+1] && larr[3+1]==larr[7+1])
///////////////////////////// Coding to accommodate "degenerate hex" wedges  EFP 4/14/2011
////17619, 23561, 23562, 23592, 23591, 19210, 19211, 19241, 19240
////17620, 23562, 22301, 22302, 23592, 19211, 17950, 17951, 19241
////17621,   571, 23563, 22311,    82,   571, 19212, 17960,    82   This one in *.inp & *.abq
////17622, 23563, 23564, 22310, 22311, 19212, 19213, 17959, 17960
////17623, 23564, 23565, 22309, 22310, 19213, 19214, 17958, 17959
//	{eltype=7;n8=6;
//	 larr[4+1]=larr[6+1];i=larr[1+1];larr[1+1]=larr[5+1];larr[5+1]=larr[2+1];larr[2+1]=i;//larr[6+1]=larr[7+1]=0;
//	}
//   else if(larr[4+1]==larr[5+1] && larr[4+1]==larr[6+1] && larr[4+1]==larr[7+1] && larr[4+1]==larr[8+1])
///////////////////////////// Coding to accommodate tetras presented as 8-n  EFP 12/19/2011
////20000,20259,20260,20261,20262,1,1,1,1
////20001,20263,20264,20265,20266,1,1,1,1
////20002,20265,20267,20266,20268,1,1,1,1
//	{
////honk<<totEnum+1<<" "<<in+1<<" bingoB "<<larr[4+1]<<" "<<larr[5+1]<<" "<<larr[6+1]<<" "<<larr[7+1]<<"\n";
//	 eltype=5;n8=4;//for(i=4+1;i<8+1;i++)larr[i]=0;
//	}
//  }
if(n8==8){degen8_test(&eltype,&n8,larr);
//		  if(n8!=8)honk<<totEnum+1<<" degen "<<eltype<<" "<<n8<<"\n";
		 }
								for(i=0;i<n8;i++)base.nop1[MXNPEL*totEnum+i]=larr[i+1]-1;

//								base.matno[totEnum]=eltype*t7+n8*t3+ipid-1;
								base.matno[totEnum]=eltype*t7+n8*t3;
								base.el_map[totEnum]=in;
//////////// EFP 1/30/2011
//base.orig_matno[totEnum]=eltype*t7+n8*t3+ipid-1;
base.orig_matno[totEnum]=eltype*t7+n8*t3;
////////////
//honk<<totEnum+1<<" "<<in+1<<" ElemB "<<n8<<" "<<ipid<<"\n";
								totEnum++;
	 }
/////////////// end
							   }
							while (ntape1.peek()!= '*');
//honk<<" end of elem2nd read\n";
//if(1==1)exit(0);
																			   }
						}
				 else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='L' || cht[2]=='l') && (cht[3]=='S' || cht[3]=='s')) // *ELSET
				   {
					in=jsw=kn=0;
					for(i=0;i<int(strlen(cht))-1;i++)if(cht[i]==','){kn++;jrec=i;
																	 break; //Find first comma
																	}  // Code to handle "generate" EFP 4/22/2011
//honk<<kn<<" "<<jrec<<" locCOMMA\n";
//					if(kn>1 && int(strlen(cht))-1-jrec >=8)
					if(kn>0 && int(strlen(cht))-1-jrec >=3)
//					  {for(i=jrec+1;i<int(strlen(cht))-8;i++)
					  {for(i=jrec+1;i<int(strlen(cht))-3;i++)
						 {if((cht[i  ]=='G' || cht[i  ]=='g') &&
							 (cht[i+1]=='E' || cht[i+1]=='e') &&
							 (cht[i+2]=='N' || cht[i+2]=='n')  //GENERATE can be shortened to GEN
															 ){in=2;
//honk<<i<<" "<<jrec<<" ELSETfoundGEN\n";
																	break;} //Note:in=2 signifies GENERATE, not #commas
						 }
					  }
				   for(i=7;i<int(strlen(cht))-1;i++)if(cht[i]=='=')break; //Coding to accommodate *ELSET,ELSET= & *ELSET, ELSET=
				   for(jrec=i+1;jrec<int(strlen(cht))-1;jrec++)if(cht[jrec]!=' ')break;
				   klim=int(strlen(cht))-1;
				   for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i]==','){klim=i;break;}
				   k=0;
///////////////////
				   for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='D' || cht[i+4]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWD
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='E' || cht[i+4]=='e') &&
																		  (cht[i+5]=='L' || cht[i+5]=='l') &&
																		  (cht[i+6]=='D' || cht[i+6]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWELD
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='A' || cht[i+2]=='a') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject ElAll
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='E' || cht[i+3]=='e') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllEl
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='A' || cht[i+1]=='a') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject EAll  EFP 4/08/2012
						 }
///////////////////
				   jsw=0;
				   if(k==0){for(i=jrec;i<klim;i++){if(cht[i  ]=='W' || cht[i  ]=='w')
														   {if(i+1<klim+1){if(cht[i+1]=='D' || cht[i+1]=='d'){jsw=1;break;}
																		   else if(cht[i+1]=='P' || cht[i+1]=='p'){jsw=1;break;}
																		   else if(cht[i+1]=='G' || cht[i+1]=='g'){jsw=1;break;}
																		   else if(cht[i+1]=='E' || cht[i+1]=='e')
																					   {if(i+3<klim+1)
																						  {if((cht[i+2]=='L' || cht[i+2]=='l') &&
																							  (cht[i+3]=='D' || cht[i+3]=='d')){jsw=1;break;}
																						  }
																						else break;
																					   }
																		   else break;
																		  }
															else break;
														   }  // Accept WDx, WPx, WGx and WELDx
												  }
////							if(jsw)totWG++;
////							else totBMG++;
if(jsw){
		kp=0;for(i=jrec;i<int(strlen(cht))-1;i++){
//honk<<i<<" "<<int(strlen(cht))-1<<" "<<kp<<" Monitor "<<cht[i]<<"\n";
												 if(cht[i]==',')break;
												 else kp++;
												}
		temp_cht=new char[kp+1];
		for(i=0;i<kp;i++)temp_cht[i]=cht[i+jrec];
		temp_cht[kp]='\0';
		base.groupsname[totWG]=temp_cht; //EFP 3/25/2011
//honk<<kp<<" "<<strlen(cht)<<" "<<temp_cht<<" temp_cht\n";
//honk<<totWG<<" posteriorWG_NoGEN "<<base.groupsname[totWG].c_str()<<"\n";
		delete [] temp_cht; *temp_cht=NULL;
		totWG++;nGID++;sumWG=0;
					if(in==2){
//*ELSET, ELSET=PTBOT, GENERATE
//   33049,   33057,       1
//   33085,   33093,       1
//   33121,   33129,       1
							  do {ntape1.getline(cht,200-1);  // ELSET.... GENERATE
////								  if(jsw){parse_cdm(cht,3,&nic,&nrc,larr,darr);
//								  if(!kp){parse_cdm(cht,3,&nic,&nrc,larr,darr);
								  if( kp){parse_cdm(cht,3,&nic,&nrc,larr,darr); //TBD: Unnecessary test??
										  if(larr[1]-larr[0]+1<base.nelt){for(i=larr[0]-1;i<larr[1];i=i+larr[2])
																		   {
j= -1;for(kk=0;kk<totEnum;kk++)if(base.el_map[kk]==i){j=kk;break;}
if(j== -1){honk<<"TERMINATE: GENERATED WG el_map crash in *.abq/*.inp\n";exit(0);}
else {base.arrELSET[j]=totWG;sumWG++;}
																		   }
																		 }
										 }
								 }
							  while (ntape1.peek()!= '*');
							 }
					else {
//*ELSET, ELSET=PTTOP
//   52369,   52370,   52371,   52372,   52373,   52374,   52375,   52376,
//   52401,   52402,   52403,   52404,   52405,   52406,   52407,   52408,
//   52433,   52434,   52435,   52436,   52437,   52438,   52439,   52440,
//					base.groupsname[totWG]="WG"; //EFP 4/09/2011
					do {ntape1.getline(cht,200-1);
						parse_cdmQ(cht,&nic,&nrc,larr,darr); //This accommodates comma-end or no-comma EFP 4/15/2011
						for(i=0;i<nic;i++){if(larr[i]) //This accommodates comma-end or no-comma EFOP 4/15/2011
											 {
j= -1;for(kk=0;kk<totEnum;kk++)if(base.el_map[kk]==larr[i]-1){j=kk;break;}  //Correction EFP 4/01/2011
if(j== -1){honk<<"TERMINATE: WG el_map crash in *.abq/*.inp\n";exit(0);}
else {base.arrELSET[j]=totWG;sumWG++;}
											 }
										  }
					   }
					while (ntape1.peek()!= '*');
						 }
					if(sumlim<sumWG)sumlim=sumWG;
//honk<<totWG<<" guavaACCEPTwg "<<j+1<<"\n";
	   }
else {do {ntape1.getline(cht,200-1);
		 }
	  while (ntape1.peek()!= '*');
	  totBMG++;
	 }
						   }

				   }
				 else if(cht[0]=='*' && (cht[1]=='E' || cht[1]=='e') && (cht[2]=='N' || cht[2]=='n') && (cht[3]=='D' || cht[3]=='d'))
				  {
////				   if(ntape1.peek()!= '*')break; // multiple use for *End/*End Part/*End Assembly/*End Instance  EFP 4/22/2011
////				   else continue;
honk<<" *END found\n";break;
//honk<<" *END found\n";if(1==1)exit(0);
				  }
							// *end step CORRECTED EFP 10/22/2010
				 else {
while (ntape1.peek()!= '*')ntape1.getline(cht,200-1);
					  }



				}
			 while (!ntape1.eof()); //EndDO02

//////////////////////////////////////////
// This did not work for *.msh so maybe it should be revised for *.inp/*.abq  EFP 4/06/2011
			 for(j=0;j<totEnum;j++)
			   {eltype=base.matno[j]/t7;bscode=(base.matno[j]-eltype*t7)/t5;node=(base.matno[j]-eltype*t7-bscode*t5)/t3;
				for(in=0;in<node;in++)base.nop1[MXNPEL*j+in]=revnode_map[base.nop1[MXNPEL*j+in]-nodelolim+1];
			   }


//			 delete [] revnode_map; //THIS CAUSES MEMORY CRASH BUT WHY??? NECESSARY!!! EFP 7/31/2014
			 *revnode_map=NULL;

base.allGrp=nGID; //Special restriction to 1 basemetal + WGs
honk<<wp.nWeldGroup<<" Revised nWeldGroup "<<(base.nelt+sumELSETel)<<"\n";
honk<<base.nelt<<" nelt/allGrp "<<base.allGrp<<"\n";
			 old_npoin=new_npoin=base.npoin;new_nelt=base.nelt;new_mat=base.mat;new_ncoorf=base.ncoorf;nGID=wp.nWeldGroup+1;
			 ntape1.close();DeleteFile("record.tmp");

//aaaaaaaaaaaaa
//aaaaaaaaaaaaaaa
//aaaaaaaaaaaaaaaaa
//ifstream ntape3(OpenDialog1->FileName.t_str(),ios::nocreate|ios::binary,0);
ifstream ntape3(OpenDialog1->FileName.w_str(),ios::nocreate|ios::binary,0);
if(ntape3){ofstream tmpfile1("omnibusAba.inp",ios::binary,0);
		   ofstream tmpfile3("scratchAba3.tmp",ios::binary,0);
		   if(tmpfile1 && tmpfile3)
			 {jsw=0;
			  do {ntape3.getline(cht,200-1);
//				  if(cht[0]=='*' && (cht[1]=='e' || cht[1]=='E') && (cht[2]=='n' || cht[2]=='N') &&
//									(cht[3]=='d' || cht[3]=='D')){tmpfile3.close();break;}
				  if(cht[0]=='*' &&
(((cht[1]=='e' || cht[1]=='E') && (cht[2]=='n' || cht[2]=='N') && (cht[3]=='d' || cht[3]=='D'))
||
 ((cht[1]=='s' || cht[1]=='S') && (cht[2]=='t' || cht[2]=='T') && (cht[3]=='e' || cht[3]=='E') && (cht[3]=='p' || cht[3]=='P')))
					)
					{tmpfile3.close();break;} //Read+write until *STEP or *END is encountered  EFP 1/14/2015
				  else if(cht[0]=='*' && (cht[1]=='e' || cht[1]=='E') && (cht[2]=='l' || cht[2]=='L') &&
										 (cht[3]=='s' || cht[3]=='S') && (cht[4]=='e' || cht[4]=='E') &&
										 (cht[5]=='t' || cht[5]=='T')){if(!jsw)tmpfile1.close();jsw=1;}
				  if(jsw){tmpfile3.write(cht,strlen(cht));tmpfile3.put('\n');}
				  else   {tmpfile1.write(cht,strlen(cht));tmpfile1.put('\n');}
				 }
			  while (!ntape3.eof());
			 }
		   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open Aba scratch files",L"Terminate",MB_OK);exit(0);}
		   ntape3.close();
		  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen input file",L"Terminate",MB_OK);exit(0);}
//bbbbbbbbbbbbbbbbb
//bbbbbbbbbbbbbbb
//bbbbbbbbbbbbb

			 FDbase_indat(1,shapecombo,iplotflag,nColRes);
//			   FDdynmem_manage(-16,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
			   FDdynmem_manage(-17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
////			   FDdynmem_manage(16,dummy,base.nelt,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);//EFP 8/07/2011
//			   FDdynmem_manage(17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt+2*sumELSETel);//EFP 8/07/2011
			   FDdynmem_manage(17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);//EFP 8/07/2011
			   indat.GIDcol=base.GIDcol;
////			   for(in=0;in<base.nelt+1;in++)indat.trackELSET[in]=base.trackELSET[in];
//			   for(in=0;in<base.nelt+2*sumELSETel;in++)indat.arrELSET[in]=base.arrELSET[in];
			   for(in=0;in<base.nelt;in++)indat.arrELSET[in]=base.arrELSET[in];
////			   for(in=0;in<base.nelt+2*sumELSETel;in++)indat.orig_arrELSET[in]=base.arrELSET[in];
////

//if(1==1)exit(0);
//////////////////////////////////////
	if(nGID<1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No geometry IDs found",L"Halt",MB_OK);return;}
	else {if(iPaintyesno/10==0){
								if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
								else {
FDcomp_nGID(indat.nelt,&nGID,arGID);
							FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);
									 }
								iPaintyesno=10+1;iCullyesno=0;
							   }

			 stateVFT=2;FD_LButtonstatus=11;

Form1->Caption=GeomFileName;

////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
//////////
			 wp.memWGa=sumlim;
//			 wp.memWGa=base.nelt; //Temporary assignment EFP 3/26/2011
honk<<sumlim<<" ImpAbq MEM\n";
			 iplotflag=1;iCircleplot=1;
			 Invalidate();
		 }

			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen input file",L"Failure",MB_OK);}
		 } //EndNPOIN/NELT
	   else {extern PACKAGE void __fastcall Beep(void);
			 if(iswtype)Application->MessageBox(L"Nodes/elements/GID missing from *.abq datafile",L"Failure",MB_OK);
			 else       Application->MessageBox(L"Nodes/elements/GID missing from *.inp datafile",L"Failure",MB_OK);
			}
			} //CLOSE06
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen *.tmp file",L"Failure",MB_OK);}
		 } //CLOSE05
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *.tmp file",L"Failure",MB_OK);}
	   ntape2.close();
	  } //COSE04
	else {extern PACKAGE void __fastcall Beep(void);
		  if(iswtype)Application->MessageBox(L"Could not open *.abq file",L"Failure",MB_OK);
		  else       Application->MessageBox(L"Could not open *.inp file",L"Failure",MB_OK);
		 }
   } //CLOSE03
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
	  } //CLOSE02
} //CLOSE01
//---------------------------------------------------------------------------
void TForm1::degen8_test(long* eltype,long* n8,long larr[])// Convert degenerate hex to wedge/tetra  EFP 8/30/2014
{long n0=0,n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0;
 if     (larr[5]==larr[6] && larr[5]==larr[7] && larr[5]==larr[8])
   {*eltype=5; *n8=4;n1=larr[2];n2=larr[3];n3=larr[4];larr[4]=larr[5];
//	if     (n2==n3)continue;
//	else if(n0==n3)continue;
	if     (n1==n2){larr[2]=n1;larr[3]=n3;}
	else if(n0==n1){larr[2]=n2;larr[3]=n3;}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[2] && larr[1]==larr[3] && larr[1]==larr[4])
   {*eltype=5; *n8=4;n4=larr[5];n5=larr[6];n6=larr[7];n7=larr[8];larr[4]=larr[1];larr[1]=n4;
	if     (n5==n6){larr[2]=n7;larr[3]=n5;}
	else if(n6==n7){larr[2]=n6;larr[3]=n5;}
	else if(n4==n5){larr[2]=n7;larr[3]=n6;}
	else if(n4==n7){larr[2]=n6;larr[3]=n5;}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[3]==larr[4] && larr[3]==larr[7] && larr[3]==larr[8])
   {*eltype=5; *n8=4;n1=larr[2];n2=larr[3];larr[4]=n2;
	if     (larr[5]==larr[6]){larr[2]=larr[5];larr[3]=n1;}
	else if(n1==larr[6]){larr[2]=larr[5];larr[3]=n1;}
	else if(larr[1]==larr[5]){larr[2]=larr[6];larr[3]=n1;}
	else if(larr[1]==n1){larr[2]=larr[5];larr[3]=larr[6];}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[2] && larr[1]==larr[5] && larr[1]==larr[6])
   {*eltype=5; *n8=4;n0=larr[1];n2=larr[3];n3=larr[4];larr[4]=n0;larr[1]=n2;
	if     (n3==larr[8]){larr[2]=larr[7];larr[3]=n3;}
	else if(larr[7]==larr[8]){larr[2]=larr[7];larr[3]=n3;}
	else if(n2==larr[7]){larr[2]=larr[8];larr[3]=n3;}
	else if(n2==n3){larr[2]=larr[7];larr[3]=larr[8];}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[2]==larr[3] && larr[2]==larr[6] && larr[2]==larr[7])
   {*eltype=5; *n8=4;n1=larr[2];n2=larr[3];n3=larr[4];
	if     (larr[5]==larr[8]){larr[2]=n3;larr[3]=larr[5];larr[4]=n1;}
	else if(n3==larr[8]){larr[2]=n3;larr[3]=larr[5];larr[4]=n1;}
	else if(larr[1]==larr[5]){larr[2]=n3;larr[3]=larr[8];larr[4]=n1;}
	else if(larr[1]==n3){larr[2]=larr[8];larr[3]=larr[5];larr[4]=n2;}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[4] && larr[1]==larr[5] && larr[1]==larr[8])
   {*eltype=5; *n8=4;n0=larr[1];n1=larr[2];n2=larr[3];larr[4]=n0;larr[1]=n1;
	if     (n2==larr[7])larr[1]=larr[6];
	else if(larr[6]==larr[7])larr[2]=larr[6];
	else if(n1==larr[6])larr[2]=larr[7];
	else if(n1==n2){larr[2]=larr[6];larr[3]=larr[7];}
	else {Application->MessageBox(L"Unusual degenerate tet-to-hex in *.abq file",L"Terminate",MB_OK);exit(0);}
	larr[5]=larr[6]=larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[4] && larr[2]==larr[3])
   {*eltype=7; *n8=6;n0=larr[1];n1=larr[2];n4=larr[5];n5=larr[6];larr[1]=n1;larr[2]=n5;larr[3]=larr[7];larr[4]=n0;larr[6]=larr[8];larr[7]=larr[8]= -1;
   }
 else if((larr[5]==larr[8] && larr[6]==larr[7]) || (larr[3]==larr[7] && larr[4]==larr[8]))
   {*eltype=7; *n8=6;n0=larr[1];n1=larr[2];n3=larr[4];n5=larr[6];larr[1]=n1;larr[2]=n5;larr[4]=n0;larr[6]=n3;larr[7]=larr[8]= -1;
   }
 else if(larr[5]==larr[6] && larr[7]==larr[8])
   {*eltype=7; *n8=6;n1=larr[2];n2=larr[3];n4=larr[5];larr[2]=n4;larr[3]=n1;larr[5]=larr[7];larr[6]=n2;larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[2] && larr[3]==larr[4])
   {*eltype=7; *n8=6;n2=larr[3];n4=larr[5];n5=larr[6];larr[2]=n4;larr[3]=n5;larr[4]=n2;larr[5]=larr[8];larr[6]=larr[7];larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[5] && larr[4]==larr[8])
   {*eltype=7; *n8=6;n1=larr[2];n2=larr[3];n3=larr[4];n4=larr[5];n5=larr[6];larr[2]=n5;larr[3]=n1;larr[5]=larr[7];larr[6]=n2;larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[5] && larr[2]==larr[6])
   {*eltype=7; *n8=6;n0=larr[1];n1=larr[2];n3=larr[4];larr[1]=n1;larr[2]=larr[7];larr[4]=n0;larr[5]=larr[8];larr[6]=n3;larr[7]=larr[8]= -1;
   }
 else if(larr[2]==larr[6] && larr[3]==larr[7])
   {*eltype=7; *n8=6;n1=larr[2];n2=larr[3];n3=larr[4];n4=larr[5];larr[2]=n4;larr[3]=n1;larr[5]=larr[8];larr[6]=n2;larr[7]=larr[8]= -1;
   }
 else if(larr[1]==larr[2] && larr[5]==larr[6])
   {*eltype=7; *n8=6;n2=larr[3];n3=larr[4];n4=larr[5];larr[2]=n2;larr[3]=n3;larr[4]=n4;larr[5]=larr[7];larr[6]=larr[8];larr[7]=larr[8]= -1;
   }
 else if(larr[2]==larr[3] && larr[6]==larr[7])
   {*eltype=7; *n8=6;n3=larr[4];n4=larr[5];n5=larr[6];larr[3]=n3;larr[4]=n4;larr[5]=n5;larr[6]=larr[8];larr[7]=larr[8]= -1;
   }
 else if((larr[3]==larr[4] && larr[7]==larr[8]) || (larr[1]==larr[4] && larr[5]==larr[8]))
   {*eltype=7; *n8=6;n4=larr[5];n5=larr[6];larr[4]=n4;larr[5]=n5;larr[6]=larr[7];larr[7]=larr[8]= -1;
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportMshExecute(TObject *Sender)
// Warning: possible discontinuous node/element numbers in input *.msh
// Observation: It is not necessary to have weld groups, since the user can place weld passes in the base mesh???
// Data card input length=200 below
// This version does NOT require   ** End    by means of ntape.peek()== -1 (i.e. no more characters in file)
// nodelolim,nodeuplim,eluplim begin with 1 (not 0)
// Note current convention: *.msh & Simulia/Abaqus *.inp/*.abq files contain weld groups (never weld passes), regardless of name
//     Hence wp.nWeldGroup is incremented but not wp.nWeldPass
// 8n hex elements only
{
 int nic=0,nrc=0,jsw=0;
 long in=0,n8=0,dummy=0,jrec=0,//iswELSET2=0,
i=0,j=0,k=0,kk=0,kp=0,eltype=0,bscode=0,node=0,t7=10000000,t5=100000,t3=1000,larr[10],//larr1[10],
nodeuplim=0,nodelolim=0,totNnum=0,eluplim=0,ellolim=0,totEnum=0,sumWG=0,sumlim=0,sumELSETel=0,//totBMG=0,
totWG=0,ELSETmobsize=0,exALLEL=0,exALLWD=0,iallGrp=0, *revnode_map=NULL;
 float //fval=0.f,
 darr[10];
 char cht[200], *temp_cht=NULL, *temp_cht1=NULL//,extensChar[]=".msh",chELSET[78+1], *fnNeed1=NULL,*fnNeed2=NULL
 ;
 wchar_t string0[11];
////////////////
//String *tw_groupsname=NULL;
////////////////
 if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First, close current file->FileClose",L"Halt",MB_OK);}
 else {

 base.matsteps=base.ncoorf=1;
 base.npoin=base.nelt=base.nvfix=base.nedge=base.pload=base.mat=base.nblod=0;
 base.allGrp=1; //Try insisting on a base group???
 base.ELSETelsum=MXNPEL=wp.nWeldGroup=0; //Establish MXNPEL
 OpenDialog1->Filter= "Msh (*.msh)|*.msh;*.MSH";
/////////////////////////////////////
 if(OpenDialog1->Execute())
//   {ifstream ntape2(OpenDialog1->FileName.t_str(),ios::nocreate|ios::binary,0);
   {ifstream ntape2(OpenDialog1->FileName.w_str(),ios::nocreate|ios::binary,0);
	if(ntape2)
	  {

gWsiAlias=(String)modelName_g; // where char modelName_g[260] in *.h
//honk<<gWsiAlias.t_str()<<" gWsiAliasImportAbmmmmmm\n";
// Perhaps the above should be moved within   if(ntape){  ??? EFP 2/27/2012
///////////////////////////// end

	   ofstream tmpfile("record.tmp",ios::binary,0); //Sanitize by writing file without comment/blank lines
	   if(tmpfile)
		 {do {ntape2.getline(cht,200-1);
			  if(cht[0]=='c')continue; // Vague assumption: *.msh comment card begins with c
			  else tmpfile<<cht<<"\n";
			 }
		  while (!ntape2.eof());
		  tmpfile.close();
		  ifstream ntape("record.tmp",ios::nocreate|ios::binary,0);
		  if(ntape)
//
	  {
	   nodeuplim=totNnum=eluplim=totEnum=ELSETmobsize=j=0;nodelolim=ellolim=LONG_INT;exALLEL=exALLWD= -1;
	   do {ntape.getline(cht,200-1);
		   if     (cht[0]=='*' && cht[1]=='*' &&                 (cht[2]=='E' || cht[2]=='e') && (cht[3]=='N' || cht[3]=='n') && (cht[4]=='D' || cht[4]=='d'))break;
		   else if(cht[0]=='*' && cht[1]=='*' && cht[2]==' ' &&  (cht[3]=='E' || cht[3]=='e') && (cht[4]=='N' || cht[4]=='n') && (cht[5]=='D' || cht[5]=='d'))break;
		   else if((cht[0]=='*' && cht[1]=='*' &&                (cht[2]=='N' || cht[2]=='n') && (cht[3]=='O' || cht[3]=='o') && (cht[4]=='D' || cht[4]=='d') && (cht[5]=='E' || cht[5]=='e')) ||
				   (cht[0]=='*' && cht[1]=='*' && cht[2]==' ' && (cht[3]=='N' || cht[3]=='n') && (cht[4]=='O' || cht[4]=='o') && (cht[5]=='D' || cht[5]=='d') && (cht[6]=='E' || cht[6]=='e')))
				  {do {ntape.getline(cht,200-1);
					   parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
					   if(nodeuplim<larr[0])nodeuplim=larr[0];
					   if(nodelolim>larr[0])nodelolim=larr[0];
					   totNnum++; // This totNum might be +1 wrong.
					  }
				   while (ntape.peek()!= '*');
				  }
		   else if((cht[0]=='*' && cht[1]=='*' &&                (cht[2]=='E' || cht[2]=='e') && (cht[3]=='L' || cht[3]=='l') && (cht[4]=='E' || cht[4]=='e') && (cht[5]=='M' || cht[5]=='m') && (cht[6]=='E' || cht[6]=='e') && (cht[7]=='N' || cht[7]=='n') && (cht[8]=='T' || cht[8]=='t')) ||
				   (cht[0]=='*' && cht[1]=='*' && cht[2]==' ' && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='L' || cht[4]=='l') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='M' || cht[6]=='m') && (cht[7]=='E' || cht[7]=='e') && (cht[8]=='N' || cht[8]=='n') && (cht[9]=='T' || cht[9]=='t')))
																			{
																			   do {ntape.getline(cht,200-1); // This might be +1 wrong
																					for(i=0;i<10;i++)larr[i]=0;
																					parse_cdmQ(cht,&nic,&nrc,larr,darr); // *ELEMENT
															   if(nic-1==8)
																 {totEnum++;if(eluplim<larr[0])eluplim=larr[0];
																  if(ellolim>larr[0])ellolim=larr[0];
																  if(MXNPEL<nic-1)MXNPEL=nic-1;//Allow for possible trailing comma EFP 6/28/2011
																 }
															   else break; //Preceding abort does not work???
																				   }
																				while (ntape.peek()!= '*');
																			}
//Caution: ELSET of ELSETs unsupported
//Caution: Remember that "GENERATE & SYSTEM & other" can occur in any order
//Caution: Remember that strlen() already excludes end-of-line, so check that the following is correct. EFP 6/19/2014
///////////////////////
/////////////////////////
///////////////////////////
		   else   {for(jrec=2;jrec<int(strlen(cht))-1;jrec++)if(cht[jrec]!=' ')break;
				   k=0;
///////////////////
				   for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='D' || cht[i+4]=='d'))
																			{k=1;exALLWD=base.allGrp;break;}
																	  }
												   else break;
												  }  // Reject AllWD
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='E' || cht[i+4]=='e') &&
																		  (cht[i+5]=='L' || cht[i+5]=='l') &&
																		  (cht[i+6]=='D' || cht[i+6]=='d'))
																			{k=1;exALLWD=base.allGrp;break;}
																	  }
												   else break;
												  }  // Reject AllWELD
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='A' || cht[i+2]=='a') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject ElAll
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='E' || cht[i+3]=='e') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllEl
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='A' || cht[i+1]=='a') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject EAll  EFP 4/08/2012
						 }
///////////////////
				   jsw=0;
				   if(k==0){base.allGrp=base.allGrp+1;nGID=nGID+1;
							for(i=jrec;i<int(strlen(cht))-1;i++){if(cht[i  ]=='W' || cht[i  ]=='w')
														   {if(i+1<int(strlen(cht))){if(cht[i+1]=='D' || cht[i+1]=='d'){jsw=1;break;}
																				else if(cht[i+1]=='P' || cht[i+1]=='p'){jsw=1;break;}
																				else if(cht[i+1]=='G' || cht[i+1]=='g'){jsw=1;break;}
																				else if(cht[i+1]=='E' || cht[i+1]=='e')
																					   {if(i+3<int(strlen(cht)))
																						  {if((cht[i+2]=='L' || cht[i+2]=='l') &&
																							  (cht[i+3]=='D' || cht[i+3]=='d')){jsw=1;break;}
																						  }
																						else break;
																					   }
																				else break;
																			   }
															else break;
														   }  // Accept WD, WP, WG and WELD
																}
							if(jsw)wp.nWeldGroup=wp.nWeldGroup+1;
						   }
				   j=0;while (ntape.peek()!= '*'){ntape.getline(cht,200-1);j++;}
				  }
///////////////////////////
/////////////////////////
///////////////////////
		   if(ELSETmobsize<j){ELSETmobsize=j;if(ELSETmobsize==totEnum){exALLEL=base.allGrp-1;base.allGrp=base.allGrp-1;} //if ALLEL is present
							 }  //CAUTION: This can handle only one ALLEL/ELALL/EALL/ALLWD/ALLWELD exclusion
		  }
	   while (!ntape.eof());
	   ntape.close();
honk<<nodeuplim<<" "<<nodelolim<<" "<<totNnum<<" "<<eluplim<<" "<<ellolim<<" "<<totEnum<<" "<<MXNPEL<<" DDDDDDDD\n";
honk<<ELSETmobsize<<" "<<totEnum<<" "<<exALLEL<<" "<<base.allGrp<<" zoot suit\n";
	   if(wp.nWeldGroup==0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld groups (*ELSET, ELSET=...weld...) found in *.msh",L"Terminate",MB_OK);exit(0);}
honk<<base.allGrp<<" "<<wp.nWeldGroup<<" Early A & WG\n";
GeomFileName=OpenDialog1->FileName;
	   base.npoin=totNnum;
	   base.nelt=totEnum; //Policy: Reserve storage for #elements read-in, even if there is duplication  EFP 4/19/2012
//
////
////// Integrity test for WARP3D unitary-start consecutive numbering
	   if(nodeuplim != totNnum){honk<<nodeuplim<<"TERMINATE: Nonconsecutive node numbers in file "<<totNnum<<"\n";}
	   if(eluplim != totEnum){honk<<eluplim<<"TERMINATE: Nonconsecutive/duplicate element numbers in file "<<totEnum<<"\n";}
	   if(nodeuplim != totNnum && eluplim != totEnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive elements & nodes found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
	   else if(nodeuplim != totNnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive nodes found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
	   else if(eluplim != totEnum)
		 {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Nonconsecutive elements found. Please renumber.",L"Terminate: WARP3D-inadmissable",MB_OK);
		  exit(0);
		 }
//////
////
//
	   if(base.nelt> LONG_INT/t3){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Excessive #elements in geometry file",L"Terminate",MB_OK);exit(0);}
	   if(base.npoin>0 && base.nelt>0)
		 {FDdynmem_manage(1,base.npoin,base.nelt,dummy,dummy,dummy,base.npoin,dummy,dummy,dummy,dummy,dummy,dummy,MXNPEL);
//		  FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
		  FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,wp.nWeldGroup+1);
		  FDdynmem_manage(15,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);//EFP 8/07/2011
		  FDdynmem_manage(20,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.allGrp);//EFP 8/07/2011
		  base.ELSETinputnames[0]=L"ALLEL";
		  ifstream ntape1("record.tmp",ios::nocreate|ios::binary,0);
		  if(ntape1) //seek() can be used with binary-opened files (NOT ascii) so close & reopen file  EFP 12/18/2011
			{
//			 ipid=nGID=1;  //Assumption: All elements start with GID=1
			 nGID=iallGrp=1;
			 totNnum=totEnum=sumELSETel=sumlim=0;
//			 totBMG=0;
			 totWG=0;//EFP 10/22/2011
			 for(in=0;in<NDF*base.npoin;in++)base.c1[in]=0.f;
//vvvvvvvvvvvvvvvvvvv
			 for(in=0;in<2*base.npoin;in++)base.nofix[in]=0;
			 for(in=0;in<base.npoin;in++)base.nrfix[in]=0;
			 for(in=0;in<NDF*base.npoin;in++)base.presc[in]=0.f;
//vvvvvvvvvvvvvvvvvvv
			 for(in=0;in<base.nelt;in++)base.arELEM[in]=1;
revnode_map=new long[nodeuplim-nodelolim+1];
//			 for(in=0;in<base.npoin;in++)base.arrELSET[in]=0;
			 for(in=0;in<base.nelt;in++)base.arrELSET[in]=0; //Correction EFP 1/14/2015
////////////
honk<<base.allGrp<<" "<<wp.nWeldGroup<<" EEELate A & WG\n";
honk<<base.npoin<<" "<<base.nelt<<" npoin/nelt\n";
////////////
			 do {ntape1.getline(cht,200-1);
				 if(cht[0]=='*' && cht[1]=='*' && (cht[2]=='E' || cht[2]=='e') && (cht[3]=='N' || cht[3]=='n') && (cht[4]=='D' || cht[4]=='d'))break;
				 else if(cht[0]=='*' && cht[1]=='*' && cht[2]==' ' && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='N' || cht[4]=='n') && (cht[5]=='D' || cht[5]=='d'))break;
				 else if((cht[0]=='*' && cht[1]=='*' &&                (cht[2]=='N' || cht[2]=='n') && (cht[3]=='O' || cht[3]=='o') && (cht[4]=='D' || cht[4]=='d') && (cht[5]=='E' || cht[5]=='e')) ||
						 (cht[0]=='*' && cht[1]=='*' && cht[2]==' ' && (cht[3]=='N' || cht[3]=='n') && (cht[4]=='O' || cht[4]=='o') && (cht[5]=='D' || cht[5]=='d') && (cht[6]=='E' || cht[6]=='e')))
					 {do {ntape1.getline(cht,200-1); //parse_cdm(cht,4,&nic,&nrc,larr,darr); // *NODE, stored in same file
						  parse_cdm3ff(cht,4,&nic,&nrc,larr,darr);
						  in=larr[0]-1;base.c1[NDF*totNnum]=darr[0];base.c1[NDF*totNnum+1]=darr[1];base.c1[NDF*totNnum+2]=darr[2];
						  base.node_map[totNnum]=in;  // Check this
						  revnode_map[in-nodelolim+1]=totNnum;
						  totNnum++;
						 }
					  while (ntape1.peek()!= '*');
					 }
		   else if((cht[0]=='*' && cht[1]=='*' &&                (cht[2]=='E' || cht[2]=='e') && (cht[3]=='L' || cht[3]=='l') && (cht[4]=='E' || cht[4]=='e') && (cht[5]=='M' || cht[5]=='m') && (cht[6]=='E' || cht[6]=='e') && (cht[7]=='N' || cht[7]=='n') && (cht[8]=='T' || cht[8]=='t')) ||
				   (cht[0]=='*' && cht[1]=='*' && cht[2]==' ' && (cht[3]=='E' || cht[3]=='e') && (cht[4]=='L' || cht[4]=='l') && (cht[5]=='E' || cht[5]=='e') && (cht[6]=='M' || cht[6]=='m') && (cht[7]=='E' || cht[7]=='e') && (cht[8]=='N' || cht[8]=='n') && (cht[9]=='T' || cht[9]=='t')))
						{
							do {ntape1.getline(cht,200-1);
								for(i=0;i<10;i++)larr[i]=0;
								parse_cdmQ(cht,&nic,&nrc,larr,darr); // *ELEMENT, stored in same file
								if(nic-1==8)eltype=8;
								else {honk<<"Halt3: Unsupported element with #nodes "<<(nic-1)<<"\n";
									  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(_ltow(nic-1,string0,10),L"Halt3: Unsupported element with #nodes in MSHgeomOpen()",MB_OK);
									  exit(0);
									 }
								n8=nic-1;in=larr[0]-1;
								for(i=0;i<n8;i++)base.nop1[MXNPEL*totEnum+i]=larr[i+1]-1;
								base.matno[totEnum]=eltype*t7+n8*t3;
								base.el_map[totEnum]=in;
								base.orig_matno[totEnum]=eltype*t7+n8*t3;
								totEnum++;
							   }
							while (ntape1.peek()!= '*');
						}
///////////////////////
/////////////////////////
///////////////////////////
		   else   {for(jrec=2;jrec<int(strlen(cht))-1;jrec++)if(cht[jrec]!=' ')break;
				   k=0;
///////////////////
				   for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='D' || cht[i+4]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWD
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='W' || cht[i+3]=='w') &&
																		  (cht[i+4]=='E' || cht[i+4]=='e') &&
																		  (cht[i+5]=='L' || cht[i+5]=='l') &&
																		  (cht[i+6]=='D' || cht[i+6]=='d')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllWELD
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='A' || cht[i+2]=='a') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject ElAll
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='A' || cht[i  ]=='a')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='L' || cht[i+1]=='l') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='E' || cht[i+3]=='e') &&
																		  (cht[i+4]=='L' || cht[i+4]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject AllEl
						 }
				   if(!k){for(i=jrec;i<int(strlen(cht))-1;i++)if(cht[i  ]=='E' || cht[i  ]=='e')
												  {if(i+4<int(strlen(cht))){if((cht[i+1]=='A' || cht[i+1]=='a') &&
																		  (cht[i+2]=='L' || cht[i+2]=='l') &&
																		  (cht[i+3]=='L' || cht[i+3]=='l')){k=1;break;}
																	  }
												   else break;
												  }  // Reject EAll  EFP 4/08/2012
						 }
///////////////////
				   jsw=0;
				   if(k==0){ //Acceptable ELSET name
							for(i=jrec;i<int(strlen(cht))-1;i++){if(cht[i  ]=='W' || cht[i  ]=='w')
														   {if(i+1<int(strlen(cht))){if(cht[i+1]=='D' || cht[i+1]=='d'){jsw=1;break;}
																				else if(cht[i+1]=='P' || cht[i+1]=='p'){jsw=1;break;}
																				else if(cht[i+1]=='G' || cht[i+1]=='g'){jsw=1;break;}
																				else if(cht[i+1]=='E' || cht[i+1]=='e')
																					   {if(i+3<int(strlen(cht)))
																						  {if((cht[i+2]=='L' || cht[i+2]=='l') &&
																							  (cht[i+3]=='D' || cht[i+3]=='d')){jsw=1;break;}
																						  }
																						else break;
																					   }
																				else break;
																			   }
															else break;
														   }  // Accept WD, WP, WG and WELD
																}
							if(jsw){ //Acceptable weld group name
////
					kp=int(strlen(cht))-3;temp_cht=new char[kp+1];for(i=0;i<kp;i++)temp_cht[i]=cht[i+3];
					temp_cht[kp]='\0';base.groupsname[totWG]=temp_cht; //EFP 3/25/2011
					temp_cht1=new char[kp];for(i=0;i<kp-1;i++)temp_cht1[i]=cht[i+3];
					base.ELSETinputnames[iallGrp]=temp_cht1;
					iallGrp++;delete [] temp_cht1; *temp_cht1=NULL;delete [] temp_cht; *temp_cht=NULL;
		            totWG++;nGID++;sumWG=0;
					do {ntape1.getline(cht,200-1);
						parse_cdmQ(cht,&nic,&nrc,larr,darr); //This accommodates comma-end or no-comma EFP 4/15/2011
						for(i=0;i<nic;i++){if(larr[i]) //This accommodates comma-end or no-comma EFOP 4/15/2011
											 {
j= -1;for(kk=0;kk<totEnum;kk++)if(base.el_map[kk]==larr[i]-1){j=kk;break;}  //Correction EFP 4/01/2011
if(j== -1){honk<<"TERMINATE: WG el_map crash in *.abq/*.inp\n";exit(0);}
else {base.arrELSET[j]=totWG;sumWG++;
	  k=base.matno[j]-t3*(base.matno[j]/t3);base.matno[j]=base.matno[j]-k+iallGrp-1;
	 }
											 }
										  }
					   }
					while (ntape1.peek()!= '*');
					if(sumlim<sumWG)sumlim=sumWG;
////
								   }
							else { //Read acceptable ELSET name
////
						 kp=int(strlen(cht))-3;temp_cht=new char[kp+1];for(i=0;i<kp;i++)temp_cht[i]=cht[i+3];
		temp_cht[kp]='\0';
		if(iallGrp != exALLEL){temp_cht1=new char[kp];for(i=0;i<kp-1;i++)temp_cht1[i]=cht[i+3];
							   base.ELSETinputnames[iallGrp]=temp_cht1;
							   iallGrp++;delete [] temp_cht1; *temp_cht1=NULL;
							  }
		delete [] temp_cht; *temp_cht=NULL;nGID++;
				  do {ntape1.getline(cht,200-1);
//					  if(iallGrp != exALLEL)
					  if(iallGrp != exALLEL && iallGrp != exALLWD)
					   {parse_cdmQ(cht,&nic,&nrc,larr,darr); //This accommodates comma-end or no-comma EFP 4/15/2011
						for(i=0;i<nic;i++){if(larr[i]) //This accommodates comma-end or no-comma EFOP 4/15/2011
											 {
j= -1;for(kk=0;kk<totEnum;kk++)if(base.el_map[kk]==larr[i]-1){j=kk;break;}  //Correction EFP 4/01/2011
if(j== -1){honk<<"TERMINATE: ELSETinput el_map crash in *.abq/*.inp\n";exit(0);}
else {k=base.matno[j]-t3*(base.matno[j]/t3);base.matno[j]=base.matno[j]-k+iallGrp-1;
	 }
											 }
										  }
					   }
					 }
				  while (ntape1.peek()!= '*');
////
								 }
						   }
				   else {while (ntape.peek()!= '*'){ntape.getline(cht,200-1);} //Read+ignore unacceptable ELSET
						}
				  }
///////////////////////////
/////////////////////////
///////////////////////



				}
			 while (!ntape1.eof());
////
// Place integrity check here
////
//////////////////////////////////////////
// This did not work for *.msh so maybe it should be revised for *.inp/*.abq  EFP 4/06/2011
			 for(j=0;j<totEnum;j++)
			   {eltype=base.matno[j]/t7;bscode=(base.matno[j]-eltype*t7)/t5;node=(base.matno[j]-eltype*t7-bscode*t5)/t3;
				for(in=0;in<node;in++)base.nop1[MXNPEL*j+in]=revnode_map[base.nop1[MXNPEL*j+in]-nodelolim+1];
			   }
//			 delete [] revnode_map; //THIS CAUSES MEMORY CRASH BUT WHY??? NECESSARY!!! EFP 7/31/2014
			 *revnode_map=NULL;
base.allGrp=nGID; //Special restriction to 1 basemetal + WGs
honk<<wp.nWeldGroup<<" Revised nWeldGroup "<<(base.nelt+sumELSETel)<<"\n";
honk<<base.nelt<<" nelt/allGrp "<<base.allGrp<<" "<<nGID<<"\n";
			 old_npoin=new_npoin=base.npoin;new_nelt=base.nelt;new_mat=base.mat;new_ncoorf=base.ncoorf;nGID=wp.nWeldGroup+1;
			 ntape1.close();DeleteFile("record.tmp");
			 FDbase_indat(1,shapecombo,iplotflag,nColRes);
			   FDdynmem_manage(-17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
			   FDdynmem_manage(17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);//EFP 8/07/2011
			   indat.GIDcol=base.GIDcol;
			   for(in=0;in<base.nelt;in++)indat.arrELSET[in]=base.arrELSET[in];
	if(nGID<1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No geometry IDs found",L"Halt",MB_OK);return;}
	else {if(iPaintyesno/10==0){
								if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
								else {FDcomp_nGID(indat.nelt,&nGID,arGID);
									  FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);
									 }
								iPaintyesno=10+1;iCullyesno=0;
							   }

			 stateVFT=2;FD_LButtonstatus=11;

Form1->Caption=GeomFileName;

////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
//////////
			 wp.memWGa=sumlim;
////honk<<sumlim<<" ImpAbq MEM\n";
honk<<base.allGrp<<" allGrp/exALL "<<exALLEL<<"\n";
			 for(j=0;j<base.allGrp;j++){
 int bufferSize1=WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[j].w_str(), -1,NULL,0,NULL,NULL);
 char* m1=new char[bufferSize1];WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[j].w_str(), -1,m1,bufferSize1,NULL,NULL);
 honk<<m1<<"\n";// EFP 12/10/2014
 delete[] m1;
//								 honk<<base.ELSETinputnames[j].c_str()<<" ELSETinput "<<(j+1)<<"\n";
									   }
//			 for(j=0;j<base.nelt;j++)honk<<(j+1)<<" "<<base.matno[j]<<" kkkoooppp\n";

			 iplotflag=1;iCircleplot=1;
			 Invalidate();
		 }

			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen input file",L"Failure",MB_OK);}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);
			 Application->MessageBox(L"Nodes/elements/GID missing from *.msh datafile",L"Failure",MB_OK);
			}
			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen *.tmp file",L"Failure",MB_OK);}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *.tmp file",L"Failure",MB_OK);}
	   ntape2.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Could not open *.msh file",L"Failure",MB_OK);
		 }
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
	  }
//}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportVFTrExecute(TObject *Sender)
//void __fastcall TForm1::CAPgeomOpenExecute(TObject *Sender)
// Import *.VFTr
// TBD: Currently using & reading highest node number. Switch to memory-saving "true node number" scheme
// Policy on 4/09/2011 Nodal geom list & topology list are shifted. Subsequent data is NOT shifted.
// Note current policy: *.VFTr files can have WG & WP. However, whenever an element has both WG & WP, WP takes precedence.
//    A subroutine checks for annihilation of WG.  EFP 1/23/2012
// TBD: increase 199 to allow for 20n hex element connectivity read from 1 line
{
 int nic=0,nrc=0
//, *temp_allGID=NULL
 ,bufferSize1=0,bufferSize2=0,bufferSize3=0
;
// int *tw_allGID=NULL;
 float pval=0.,darr[9];
// long ix=0,i=0,ii=0,ie=0,ir=0,ipid=0,nop0=0,nop1=0,nop2=0,nop3=0,nop4=0,nop5=0,nop6=0,nop7=0,dummy=0,larr[9]
 long ix=0,i=0,ii=0,ie=0,ir=0,ipid=0,nop0=0,nop1=0,nop2=0,nop3=0,nop4=0,nop5=0,nop6=0,nop7=0,dummy=0,larr[20+1]
	  ,control[10],accum=0,j=0,eltype=0,bscode=0,node=0,ieGID=0,t7=10000000,t5=100000,t3=1000,sumlim=0,
	  nodeuplim=0,nodelolim=0,eluplim=0,wpWG=0,wpWP=0,in=0,ip=0,inp=0,sumELSETel=0, *revnode_map=NULL;
 char cht[10*(MXNPELS+3)],chtm[200]; // Anticipate 23I10
 _TCHAR descript0[41],descript1[41],descript2[41], *temp_cht=NULL; // Anticipate 23I10
//String efpAnsi1[20];
//String *efpAnsi=NULL;
//String *SWPTname=NULL;
_TCHAR *efpChar=NULL, *texasbuf;
 wchar_t curMess0[]=L"WeldParam",string1[15],curMess1[16],curMess2[]=L"Reduced from ",curMess3[]=L" to ",string0[32];
// wchar_t string0[40],curMess0[]=L"WeldParam",string1[15];
//wchar_t curMess0[]=L"WeldParam",string1[15];
// _ltow(wps.nWeldParamSet+1,string1,10);
// StringCchCatW(curMess0,39+1,string1); //Length of destination curMess0 is provided.
// wps.name[wps.nWeldParamSet]=curMess0;

//String *tw_groupsname=NULL;

 base.npoin=base.nelt=base.nvfix=base.nedge=base.pload=base.mat=base.nblod=nGIDmax=base.allGrp=base.ELSETelsum=0;
 base.matsteps=base.ncoorf=1;MXNPEL=8;
 if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First, close current file->FileClose",L"Halt",MB_OK);}
 else {
 OpenDialog1->Filter= "VFTr format (*.VFTr,*.vftr)|*.VFTr;*.vftr";
 if(OpenDialog1->Execute())
   {ifstream ntape0(OpenDialog1->FileName.c_str(),ios::nocreate|ios::binary,0);
	if(ntape0)
	  {
////////////////////////////////////////// start EFP 11/13/2011
//char *driveEFP=NULL,*dirEFP=NULL,*extEFP=NULL;
//driveEFP=new char[260];dirEFP=new char[260];extEFP=new char[260];
////fnsplit(OpenDialog1->FileName.c_str(),driveEFP,dirEFP,modelName_g,extEFP);
//_wfnsplit(OpenDialog1->FileName,driveEFP,dirEFP,modelName_g,extEFP);
//honk<< driveEFP<<" driveEFP\n";honk<< dirEFP<<" dirEFP\n";honk<< modelName_g<<" modelName_g\n";honk<< extEFP<<" extEFP\n";
//delete [] driveEFP;delete [] dirEFP;delete [] extEFP;
//
//gWsiAlias=(String)modelName_g; // where char modelName_g[260] in *.h
//honk<<gWsiAlias.t_str()<<" gWsiAliasImportVFTrmmmmmm\n";
//// Perhaps the above should be moved within   if(ntape){  ??? EFP 2/27/2012
////////////////////////////////////////// end




//TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//try {




//				 ntape>>control[0]>>control[1]>>control[2]>>control[3]>>control[4]>>
//						control[5]>>control[6]>>control[7]>>control[8]>>control[9];
				 ntape0.getline(chtm,199);parse_cdm(chtm,8,&nic,&nrc,larr,darr);
/////////////// EFP 4/04/2011
//	   if(larr[0]==2){sumELSETel=0;
	   if(larr[0]==3){sumELSETel=0;
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
///////////////
//				 base.npoin=control[1]; //#nodes (Currently, highest node-NOT #active nodes)
//				 base.nelt=control[2]; //#elem
				 base.npoin=larr[1]; //#nodes (Currently, highest node-NOT #active nodes)
				 base.nelt=larr[2]; //#elem
honk<<base.npoin<<" Total npoin/nelt "<<base.nelt<<"\n";
honk<<base.npoin<<" "<<base.nelt<<" "<<MXNPEL<<" #nodes,#nelm,MXNPEL VFTgeomOpenExecute\n";
	   if(base.nelt> LONG_INT/t3){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Excessive #elements in *.VFT file",L"Terminate",MB_OK);exit(0);}
	   nodeuplim=0;nodelolim=LONG_INT;
	   for(i=0;i<base.npoin;i++)
		 {ntape0.getline(chtm,199);parse_cdm(chtm,4,&nic,&nrc,larr,darr);ix=larr[0];
		  if(nodeuplim<ix)nodeuplim=ix;
		  if(nodelolim>ix)nodelolim=ix;
		 }
	   eluplim=0;
	   for(i=0;i<base.nelt;i++){ntape0.getline(chtm,199);parse_cdm(chtm,9,&nic,&nrc,larr,darr);ii=larr[0];
								if(eluplim<ii)eluplim=ii;
							   }
//////////////////////  EFP 2/11/2012
///////////////////////
////////////////////////
/////////////////////////
//////////////////////////
///////////////////////////
ntape0.getline(chtm,199); //Just in case  EFP 4/03/2011
ntape0.getline(chtm,199);// Omit JavaVFT group[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsall[]
ntape0.getline(chtm,199);// Omit JavaVFT groupA[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsallname[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsallindex[]
ntape0.getline(chtm,199);// Omit JavaVFT groups[]
ntape0.getline(chtm,199);// Omit JavaVFT groupscount[]
ntape0.getline(chtm,199);// Omit JavaVFT groupname[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsname[]
				 ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wpWG=larr[0];
/////////////////////////////////
//FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.ncoorf,dummy,dummy);//EFP 3/15/2011
/////////////////////////////////
				 if(wpWG)         {sumlim=0;
								   for(j=0;j<wpWG;j++)
								   {
ntape0.getline(chtm,199); //Just in case  EFP 4/03/2011
ntape0.getline(chtm,199);// Omit JavaVFT group[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsall[]
ntape0.getline(chtm,199);// Omit JavaVFT groupA[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsallname[]
ntape0.getline(chtm,199);// Omit JavaVFT groupsallindex[]
ntape0.getline(chtm,199);// Omit JavaVFT groups[]
ntape0.getline(chtm,199);// Omit JavaVFT groupscount[]
ntape0.getline(chtm,199);// Omit JavaVFT groupname[]
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
ntape0.getline(chtm,199);
									if(sumlim<accum)sumlim=accum;
									for(ii=0;ii<accum;ii++){ntape0.getline(chtm,199);
															sumELSETel++;
														   }
								   }
								   wp.memWGa=sumlim;
								  }
				 else wp.memWGa=0;
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wpWP=larr[0];
			   if(wpWP)     {   for(i=0;i<wpWP;i++){
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		wp.memWGa=max(wp.memWGa,accum);//Emergency Code to plot WP without WG
		for(j=0;j<accum;j++){ntape0.getline(chtm,199);
//parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.eles[wp.memWGa*i+j]=larr[0];
							 sumELSETel++;
							}
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++)ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++)ntape0.getline(chtm,199);
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++)ntape0.getline(chtm,199);
		for(j=0;j<4;j++)ntape0.getline(chtm,199);
		for(j=0;j<4;j++)ntape0.getline(chtm,199);
		for(j=0;j<3;j++)ntape0.getline(chtm,199);
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		if(accum){ //Attempt to accommodate uncertain edgeEles/edgeNodes  EFP 4/09/2011
				  for(j=0;j<accum;j++)ntape0.getline(chtm,199);
				  for(j=0;j<4;j++)ntape0.getline(chtm,199);
				 }
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++)ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++)ntape0.getline(chtm,199);
		for(j=0;j<3*NDF;j++)ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
ntape0.getline(chtm,199);
														   }
							   }
/*
				 for(i=0;i<wp.nWeldPass;i++){
											 for(j=0;j<wp.memWGa;j++)
											   {if(wp.eles[wp.memWGa*i+j]<0)break;
												else {ie=wp.eles[wp.memWGa*i+j]/10;
eltype=base.matno[ie]/t7;bscode=(base.matno[ie]-eltype*t7)/t5;node=(base.matno[ie]-eltype*t7-bscode*t5)/t3;ieGID=base.matno[ie]-eltype*t7-bscode*t5-node*t3;
base.matno[ie]=base.matno[ie]-ieGID+wp.nWeldGroup+i+1;
													 }
											   }
											 wp.prevGID[i]=ieGID; //Correction EFP 3/27/2011
											}
nGID=wp.nWeldGroup+wp.nWeldPass+1;nGIDmax=nGID;
*/
///////////////////////////
//////////////////////////
/////////////////////////
////////////////////////
///////////////////////
//////////////////////


//HERO
	   GeomFileName=OpenDialog1->FileName;ntape0.close();
/////////// Cursor EFP 1/03/2013
//Screen->Cursor=crSizeAll;
Screen->Cursor=Save_Cursor;
///////////
//	   base.matsteps=matstep;
//honk<<nodeuplim<<" "<<nodelolim<<" "<<base.npoin<<" "<<eluplim<<" "<<base.nelt<<" "<<MXNPEL<<" DDDDDDDD\n";
	   if(nodeuplim != base.npoin)honk<<nodeuplim<<" Nonconsecutive node numbers in file "<<base.npoin<<"\n";
	   if(eluplim != base.nelt)honk<<eluplim<<" Nonconsecutive element numbers in file "<<base.nelt<<"\n";
	   if(base.npoin>0 && base.nelt>0)
//		 {FDdynmem_manage(1,base.npoin,base.nelt,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.ncoorf,dummy,MXNPEL);
		 {FDdynmem_manage(1,base.npoin,base.nelt,dummy,dummy,dummy,base.npoin,dummy,dummy,dummy,dummy,base.ncoorf,dummy,MXNPEL);
		  FDdynmem_manage(15,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);
//vvvvvvvvvvvvvvvvvvv //Correction  EFp 4/13/2013
			 for(in=0;in<2*base.npoin;in++)base.nofix[in]=0;
			 for(in=0;in<base.npoin;in++)base.nrfix[in]=0;
			 for(in=0;in<NDF*base.npoin;in++)base.presc[in]=0.;
//vvvvvvvvvvvvvvvvvvv
		  ifstream ntape(OpenDialog1->FileName.c_str(),ios::nocreate|ios::binary,0);
		  if(ntape)
			{
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//			 base.allGrp=wpWG+1;
			 base.allGrp=wpWG+wpWP+1;//Emergency Code to plot WP without WG
honk<<base.allGrp<<" "<<wpWG<<" "<<wpWP<<" Zooey0 "<<wp.memWGa<<"\n";
//if(1==1)exit(0);

	   for(i=0;i<base.nelt;i++)base.arELEM[i]=1; //Correction EFP 12/03/2010
	   for(i=0;i<base.nelt;i++)base.arrELSET[i]=0;
//	   for(i=0;i<base.npoin;i++)base.node_map[i]= -1;
revnode_map=new long[nodeuplim-nodelolim+1];
//			   temp_allGID=new int[(base.allGrp+wpWP)*base.nelt];
//			   for(in=0;in<(base.allGrp+wpWP)*base.nelt;in++)temp_allGID[in]=0;
//			   for(in=0;in<base.nelt;in++)temp_allGID[in]=1;
				 ntape.getline(chtm,199);
	   for(i=0;i<base.npoin;i++) //Remember that this is cat's ass max-memory coding
		 {
//		  ntape>>ix>>descript0>>descript1>>descript2;
//		  if(ix>0){parse_cdv(descript0,&pval);base.c1[NDF*ix-3]= pval;
//				   parse_cdv(descript1,&pval);base.c1[NDF*ix-2]= pval;
//				   parse_cdv(descript2,&pval);base.c1[NDF*ix-1]= pval;
//				   base.node_map[ix-1]=1;
//				  }
//		  else {base.c1[NDF*ix-3]=base.c1[NDF*ix-2]=base.c1[NDF*ix-1]=0.;}

		  ntape.getline(chtm,199);parse_cdm(chtm,4,&nic,&nrc,larr,darr);ix=larr[0]-1;
////		  if(ix>0){base.c1[NDF*ix-3]=darr[0];base.c1[NDF*ix-2]=darr[1];base.c1[NDF*ix-1]=darr[2];}
////		  else {base.c1[NDF*ix-3]=base.c1[NDF*ix-2]=base.c1[NDF*ix-1]=0.;}
//		  base.c1[NDF*ix-3]=darr[0];base.c1[NDF*ix-2]=darr[1];base.c1[NDF*ix-1]=darr[2]; //EFP 11/24/2010
		  base.c1[NDF*i]=darr[0];base.c1[NDF*i+1]=darr[1];base.c1[NDF*i+2]=darr[2]; //EFP 4/03/2011
		  base.node_map[i]=ix;
		  revnode_map[ix-nodelolim+1]=i;
///////////////////////////////////////////////////
//honk<<i+1<<" "<<ix+1<<" headtohead "<<i<<" "<<ix-nodelolim+1<<" "<<base.node_map[i]+1<<" IngressC "<<base.c1[NDF*i]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n";
///////////////////////////////////////////////////
		 }
		 ipid=1; //Treat all elements as base metal initially
				 for(i=0;i<base.nelt;i++){
				 ntape.getline(chtm,199);
//////				 parse_cdm(chtm,9,&nic,&nrc,larr,darr);
////				 parse_cdmQ(chtm,21,&nic,&nrc,larr,darr,strlen(chtm)); //Test this to make sure it can read correctly
//				 parse_cdmQb(chtm,21,&nic,&nrc,larr,darr,strlen(chtm)-1); //strlen() should not include end-of-line BUT IT DOES SO USE -1??? Why? EFP 6/19/2014
				 parse_cdmQb(chtm,&nic,&nrc,larr,darr,strlen(chtm)-1); //strlen() should not include end-of-line BUT IT DOES SO USE -1??? Why? EFP 6/19/2014

//honk<<i<<" "<<nic<<" "<<strlen(chtm)<<" Quasimodo "<<chtm<<"\n";
//if(nic==9)honk<<larr[0]<<" VFTin8 "<<larr[1]<<" "<<larr[2]<<" "<<larr[3]<<" "<<larr[4]<<" "<<larr[5]<<" "<<larr[6]<<" "<<larr[7]<<" "<<larr[8]<<"\n";
//if(nic==7)honk<<larr[0]<<" VFTin6666666 "<<larr[1]<<" "<<larr[2]<<" "<<larr[3]<<" "<<larr[4]<<" "<<larr[5]<<" "<<larr[6]<<"\n";
////if(1==1)exit(0);

				 ii=larr[0]-1;// TBD: shift this
				 base.el_map[i]=ii;
				 if(nic-1==8){
base.nop1[MXNPEL*i  ]=revnode_map[larr[1]-nodelolim];base.nop1[MXNPEL*i+1]=revnode_map[larr[2]-nodelolim];
base.nop1[MXNPEL*i+2]=revnode_map[larr[3]-nodelolim];base.nop1[MXNPEL*i+3]=revnode_map[larr[4]-nodelolim];
base.nop1[MXNPEL*i+4]=revnode_map[larr[5]-nodelolim];base.nop1[MXNPEL*i+5]=revnode_map[larr[6]-nodelolim];
base.nop1[MXNPEL*i+6]=revnode_map[larr[7]-nodelolim];base.nop1[MXNPEL*i+7]=revnode_map[larr[8]-nodelolim];
//				 base.matno[i]=8*t7+8*1000; // EFP 1/30/2011
//				 base.orig_matno[i]=8*t7+8*1000;

//honk<<ii+1<<" VFTc3D888888 "<<i<<"\n";
				 base.matno[i]=base.orig_matno[i]=8*t7+8*1000;

//honk<<(i+1)<<" "<<base.matno[i]
//<<" "<<(base.nop1[MXNPEL*i+0]+1)
//<<" "<<(base.nop1[MXNPEL*i+1]+1)
//<<" "<<(base.nop1[MXNPEL*i+2]+1)
//<<" "<<(base.nop1[MXNPEL*i+3]+1)
//<<" "<<(base.nop1[MXNPEL*i+4]+1)
//<<" "<<(base.nop1[MXNPEL*i+5]+1)
//<<" "<<(base.nop1[MXNPEL*i+6]+1)
//<<" "<<(base.nop1[MXNPEL*i+7]+1)<<" EL\n";
							 }
				 else if(nic-1==6){
base.nop1[MXNPEL*i  ]=revnode_map[larr[1]-nodelolim];base.nop1[MXNPEL*i+1]=revnode_map[larr[2]-nodelolim];
base.nop1[MXNPEL*i+2]=revnode_map[larr[3]-nodelolim];base.nop1[MXNPEL*i+3]=revnode_map[larr[4]-nodelolim];
base.nop1[MXNPEL*i+4]=revnode_map[larr[5]-nodelolim];base.nop1[MXNPEL*i+5]=revnode_map[larr[6]-nodelolim];

//honk<<ii+1<<" VFTc3D6 "<<i<<"\n";

				 base.matno[i]=base.orig_matno[i]=7*t7+6*1000;
								  }
				 else if(nic-1==4){
base.nop1[MXNPEL*i  ]=revnode_map[larr[1]-nodelolim];base.nop1[MXNPEL*i+1]=revnode_map[larr[2]-nodelolim];
base.nop1[MXNPEL*i+2]=revnode_map[larr[3]-nodelolim];base.nop1[MXNPEL*i+3]=revnode_map[larr[4]-nodelolim];
				 base.matno[i]=base.orig_matno[i]=5*t7+4*1000;
								  }
				 else if(nic-1==20){
base.nop1[MXNPEL*i   ]=revnode_map[larr[ 1]-nodelolim];base.nop1[MXNPEL*i+ 1]=revnode_map[larr[ 2]-nodelolim];
base.nop1[MXNPEL*i+ 2]=revnode_map[larr[ 3]-nodelolim];base.nop1[MXNPEL*i+ 3]=revnode_map[larr[ 4]-nodelolim];
base.nop1[MXNPEL*i+ 4]=revnode_map[larr[ 5]-nodelolim];base.nop1[MXNPEL*i+ 5]=revnode_map[larr[ 6]-nodelolim];
base.nop1[MXNPEL*i+ 6]=revnode_map[larr[ 7]-nodelolim];base.nop1[MXNPEL*i+ 7]=revnode_map[larr[ 8]-nodelolim];
base.nop1[MXNPEL*i+ 8]=revnode_map[larr[ 9]-nodelolim];base.nop1[MXNPEL*i+ 9]=revnode_map[larr[10]-nodelolim];
base.nop1[MXNPEL*i+10]=revnode_map[larr[11]-nodelolim];base.nop1[MXNPEL*i+11]=revnode_map[larr[12]-nodelolim];
base.nop1[MXNPEL*i+12]=revnode_map[larr[13]-nodelolim];base.nop1[MXNPEL*i+13]=revnode_map[larr[14]-nodelolim];
base.nop1[MXNPEL*i+14]=revnode_map[larr[15]-nodelolim];base.nop1[MXNPEL*i+15]=revnode_map[larr[16]-nodelolim];
base.nop1[MXNPEL*i+16]=revnode_map[larr[17]-nodelolim];base.nop1[MXNPEL*i+17]=revnode_map[larr[18]-nodelolim];
base.nop1[MXNPEL*i+18]=revnode_map[larr[19]-nodelolim];base.nop1[MXNPEL*i+19]=revnode_map[larr[20]-nodelolim];
				 base.matno[i]=base.orig_matno[i]=8*t7+20*1000;
								   }
				 else if(nic-1==15){
base.nop1[MXNPEL*i   ]=revnode_map[larr[ 1]-nodelolim];base.nop1[MXNPEL*i+ 1]=revnode_map[larr[ 2]-nodelolim];
base.nop1[MXNPEL*i+ 2]=revnode_map[larr[ 3]-nodelolim];base.nop1[MXNPEL*i+ 3]=revnode_map[larr[ 4]-nodelolim];
base.nop1[MXNPEL*i+ 4]=revnode_map[larr[ 5]-nodelolim];base.nop1[MXNPEL*i+ 5]=revnode_map[larr[ 6]-nodelolim];
base.nop1[MXNPEL*i+ 6]=revnode_map[larr[ 7]-nodelolim];base.nop1[MXNPEL*i+ 7]=revnode_map[larr[ 8]-nodelolim];
base.nop1[MXNPEL*i+ 8]=revnode_map[larr[ 9]-nodelolim];base.nop1[MXNPEL*i+ 9]=revnode_map[larr[10]-nodelolim];
base.nop1[MXNPEL*i+10]=revnode_map[larr[11]-nodelolim];base.nop1[MXNPEL*i+11]=revnode_map[larr[12]-nodelolim];
base.nop1[MXNPEL*i+12]=revnode_map[larr[13]-nodelolim];base.nop1[MXNPEL*i+13]=revnode_map[larr[14]-nodelolim];
base.nop1[MXNPEL*i+14]=revnode_map[larr[15]-nodelolim];
				 base.matno[i]=base.orig_matno[i]=7*t7+15*1000;
								   }
				 else if(nic-1==10){
base.nop1[MXNPEL*i   ]=revnode_map[larr[ 1]-nodelolim];base.nop1[MXNPEL*i+ 1]=revnode_map[larr[ 2]-nodelolim];
base.nop1[MXNPEL*i+ 2]=revnode_map[larr[ 3]-nodelolim];base.nop1[MXNPEL*i+ 3]=revnode_map[larr[ 4]-nodelolim];
base.nop1[MXNPEL*i+ 4]=revnode_map[larr[ 5]-nodelolim];base.nop1[MXNPEL*i+ 5]=revnode_map[larr[ 6]-nodelolim];
base.nop1[MXNPEL*i+ 6]=revnode_map[larr[ 7]-nodelolim];base.nop1[MXNPEL*i+ 7]=revnode_map[larr[ 8]-nodelolim];
base.nop1[MXNPEL*i+ 8]=revnode_map[larr[ 9]-nodelolim];base.nop1[MXNPEL*i+ 9]=revnode_map[larr[10]-nodelolim];
				 base.matno[i]=base.orig_matno[i]=5*t7+10*1000;
								   }
				 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"ImportVFTr: Unsupported element type",L"Terminate",MB_OK);exit(0);}
///////////


										 }
			 delete [] revnode_map;

//if(1==1)exit(0);

ntape.getline(chtm,199); //Just in case  EFP 4/03/2011
ntape.getline(chtm,199);// Omit JavaVFT group[]
ntape.getline(chtm,199);// Omit JavaVFT groupsall[]
ntape.getline(chtm,199);// Omit JavaVFT groupA[]
ntape.getline(chtm,199);// Omit JavaVFT groupsallname[]
ntape.getline(chtm,199);// Omit JavaVFT groupsallindex[]
ntape.getline(chtm,199);// Omit JavaVFT groups[]
ntape.getline(chtm,199);// Omit JavaVFT groupscount[]
ntape.getline(chtm,199);// Omit JavaVFT groupname[]
ntape.getline(chtm,199);// Omit JavaVFT groupsname[]

//	   ntape>>base.npoin>>base.nelt;
honk<<base.npoin<<" Highest npoin/nelt "<<base.nelt<<"\n";
honk<<base.npoin<<" "<<base.nelt<<" "<<MXNPEL<<" #nodes,#nelm,MXNPEL VFTgeomOpenExecute\n";
honk<<"\n";
	   for(i=0;i<9*base.ncoorf;i++)base.cangl[i]=0.;for(i=0;i<base.ncoorf;i++){base.cangl[9*i]=base.cangl[9*i+4]=base.cangl[9*i+8]=1.;}
//				 ntape>>wp.nWeldGroup;

				 ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.nWeldGroup=larr[0];
/////////////////////////////////
//FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.ncoorf,dummy,dummy);//EFP 3/15/2011
FDdynmem_manage(13,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.ncoorf,dummy,wp.nWeldGroup+1);//EFP 3/15/2011
/////////////////////////////////
honk<<wp.nWeldGroup<<" ImportVFTr wp.nWeldGroup\n";
//if(1==1)exit(0);


				 if(wp.nWeldGroup){//sumlim=0;
								   for(j=0;j<wp.nWeldGroup;j++)
								   {

/////////////  Include just in case  EFP 4/03/2011
//////////////
//////////////
ntape.getline(chtm,199); //Just in case  EFP 4/03/2011
ntape.getline(chtm,199);// Omit JavaVFT group[]
ntape.getline(chtm,199);// Omit JavaVFT groupsall[]
ntape.getline(chtm,199);// Omit JavaVFT groupA[]
ntape.getline(chtm,199);// Omit JavaVFT groupsallname[]
ntape.getline(chtm,199);// Omit JavaVFT groupsallindex[]
ntape.getline(chtm,199);// Omit JavaVFT groups[]
ntape.getline(chtm,199);// Omit JavaVFT groupscount[]
ntape.getline(chtm,199);// Omit JavaVFT groupname[]
//////////////
/////////////
////////////


//									ntape>>accum;
				 ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
//									ntape>>descript0;
honk<<larr[0]<<" mmm1\n";

ntape.getline(chtm,199);

honk<<chtm<<" base.groupsname\n";//if(1==1)exit(0);

//////////////////////////////
//temp_cht=new _TCHAR[strlen(chtm)-1]; // This presumes the format  ** WeldGroupName\n
//for(i=0;i<int(strlen(chtm))-1;i++)temp_cht[i]=chtm[i];
//base.groupsname[j]= *temp_cht; //EFP 3/15/2011
//delete [] temp_cht;
////*temp_cht=NULL;
//////////////////////////////
//aaaaaaaaaaaaaaa
base.groupsname[j]=chtm;

base.groupsname[j].SetLength(base.groupsname[j].Length()-1);




// int bufferSize1=WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,NULL,0,NULL,NULL);
// char* m1=new char[bufferSize1-1];WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,m1,bufferSize1-1,NULL,NULL);
// honk<<m1<<" mm3\n";// EFP 12/10/2014
// delete[] m1;
//bbbbbbbbbbbbbbb
// bufferSize1=MultiByteToWideChar(CP_UTF8,0,chtm, -1,NULL,0);
// honk<<bufferSize1<<" bufferSize1\n";
// MultiByteToWideChar(CP_UTF8,0,chtm, -1,base.groupsname[j].w_str(),bufferSize1-2);

// int bufferSize2=WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,NULL,0,NULL,NULL);
// honk<<bufferSize2<<" bufferSize2\n";
// char* m1=new char[bufferSize1-2];WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,m1,bufferSize1-2,NULL,NULL);
//// char* m1=new char[bufferSize2];WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,m1,0,NULL,NULL);
// honk<<m1<<" mm3\n";// EFP 12/10/2014
// delete[] m1;
//if(1==1)exit(0);
 //ccccccccccccccc

//									if(sumlim<accum)sumlim=accum;
									for(ii=0;ii<accum;ii++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);i=larr[0]; //DO I NEED A GLOBAL-LOCAL TRANSF HERE...?
//									i=i-1;
//temp_allGID[base.nelt*(j+1)+i]=1;//Add coding for basemetal&weldgroup
base.arrELSET[i-1]=j+1;//Add coding for basemetal&weldgroup
														   }
								   }
//								   wp.memWGa=sumlim;
								  }
//				 else wp.memWGa=0;

honk<<wp.nWeldGroup<<" ImportVFTr wp.nWeldGroup/wp.memWGa "<<wp.memWGa<<"\n";

//				 for(i=0;i<base.npoin;i++){
////				                           ntape>>ii>>j;
//ntape.getline(chtm,199);parse_cdm(chtm,2,&nic,&nrc,larr,darr);ii=larr[0];j=larr[1];
////										   base.node_map[ii-1]=j-1; // Correction EFP 11/24/2010
//										   base.node_map[i]=j-1; // Node hashtable
//										  }
//				 for(i=0;i<base.nelt;i++){
////				                          ntape>>ii>>j;
//ntape.getline(chtm,199);parse_cdm(chtm,2,&nic,&nrc,larr,darr);ii=larr[0];j=larr[1];
//										  base.el_map[i]=j-1; // Element hashtable
//										 }
//
//				 ntape>>wp.nWeldPass;
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.nWeldPass=larr[0];
honk<<wp.nWeldPass<<" wp.nWeldPass ImpVFTr\n";
////if(wp.nWeldPass> -1000000)exit(0);
			   if(wp.nWeldPass){

//iPersistVFT=1;
ir=iPersistVFT/10;
iPersistVFT=10*ir+1;

			   FDdynmem_manage(9,dummy,wp.memWGa,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH Brain
								for(i=0;i<wp.nWeldPass;i++){



////		ntape>>descript0;wp.name[i]=L"WeldPassName";
//		ntape>>descript0;wp.name[i]=descript0;
ntape.getline(chtm,199);
//efpChar=new char[strlen(chtm)];StringCchCopyA(efpChar,strlen(chtm),chtm);
//wp.name[i]=PAnsiChar(efpChar);delete efpChar; *efpChar=NULL;



//efpChar=new _TCHAR[12];
////_TCHAR buf[3+1];
////if     (i+1< 10)gcvt(double(i+1),1,buf);
////else if(i+1<100)gcvt(double(i+1),2,buf);
////else            gcvt(double(i+1),3,buf);
////StringCchCopyA(efpChar,12,"weldpass");
////StringCchCatA(efpChar,12,buf);
//_TCHAR *texasbuf; _ltot(double(i+1),texasbuf,10);
//StringCchCopyW(efpChar,12,L"weldpass");
//StringCchCatW(efpChar,12,texasbuf);
////wp.name[i]=PAnsiChar(efpChar);
//wp.name[i]= *efpChar;
//delete efpChar; *efpChar=NULL; //Ignore input WP name
//aaaaaaaaaaaaaaaaaa

honk<<chtm<<" wp.name\n";//if(1==1)exit(0);
wp.name[i]=chtm;

wp.name[i].SetLength(wp.name[i].Length()-1);

// bufferSize2=MultiByteToWideChar(CP_UTF8,0,chtm, -1,NULL,0);
// MultiByteToWideChar(CP_UTF8,0,chtm, -1,wp.name[i].w_str(),bufferSize2-2);
//bbbbbbbbbbbbbbbbbb



//////		ntape>>descript0;wp.matName[i]=L"MatName";
////		ntape>>descript0;wp.matName[i]=descript0;
ntape.getline(chtm,199);
//////wp.matName[i]=chtm;
////efpChar=new _TCHAR[strlen(chtm)];
////StringCchCopyA(efpChar,strlen(chtm),chtm);
////wp.matName[i]=PAnsiChar(efpChar);
//wp.matName[i]= *efpChar;
//delete efpChar; *efpChar=NULL;
//aaaaaaaaaaaaaaaaaaa

honk<<chtm<<" wp.matName\n";//if(1==1)exit(0);
wp.matName[i]=chtm;

wp.matName[i].SetLength(wp.matName[i].Length()-1);

// bufferSize3=MultiByteToWideChar(CP_UTF8,0,chtm, -1,NULL,0);
// MultiByteToWideChar(CP_UTF8,0,chtm, -1,wp.matName[i].w_str(),bufferSize3-2);
//bbbbbbbbbbbbbbbbbbb
////////////////////////////////////////////////////////////////
//honk<<wp.name[i].c_str()<<" wp.name[i]\n";
//honk<<wp.matName[i].c_str()<<" wp.matName[i]\n";
////////////////////////////////////////////////////////////////



//		ntape>>wp.seqNum[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.seqNum[i]=larr[0];
wp.reset[i]=wp.seqNum[i]*10+0; //Convention: 1col=weld reversal & other cols=original seqNum (reset is NOT in *.VFTr)  EFP 3/24/2012
//		ntape>>wp.boolFlags[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.boolFlags[i]=larr[0];
//		ntape>>wp.type[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.type[i]=larr[0];
//		ntape>>wp.shape[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.shape[i]=larr[0];
//		ntape>>wp.stepInterval[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.stepInterval[i]=larr[0];
//		ntape>>wp.hp[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.hp[i]=larr[0];
//		ntape>>wp.nsegs[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.nsegs[i]=larr[0];
//		ntape>>wp.iselect[i]; //Remember wp.eles[]=10*ie+iside in the following (ditto wp.sttEles[])
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.iselect[i]=larr[0];
////		for(j=0;j<base.nelt;j++)ntape>>wp.eles[i*base.nelt+j];  // Elements in this weld pass (C++ convention)
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.eles[wp.memWGa*i+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.eles[wp.memWGa*i+j]=larr[0];
////temp_allGID[base.nelt*(base.allGrp+i+1)+larr[0]/10]=1;//Add coding for basemetal&weldgroup
//temp_allGID[base.nelt*(base.allGrp+i)+larr[0]/10]=1;//Add coding for basemetal&weldgroup
							 base.arrELSET[larr[0]/10]=i+1;//Emergency Code to plot WP without WG
								}

//////		wp.sttEles=new long[wp.nWeldPass+1*bnelt*4];  // No need for *4 ????
////		for(j=0;j<base.nelt;j++)ntape>>wp.sttEles[i*base.nelt+j];  // No need for *4 ????
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.sttEles[wp.memWGa*i+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.sttEles[wp.memWGa*i+j]=larr[0];
								}

////		wp.stpEle=new long[wp.nWeldPass+1*bnelt];  // No need for *bnelt
//		ntape>>wp.stpEle[i];  // No need for *bnelt
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.stpEle[i]=larr[0];
////		for(j=0;j<base.nelt;j++)ntape>>wp.firstEle[i*base.nelt+j];
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.firstEle[wp.memWGa*i+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.firstEle[wp.memWGa*i+j]=larr[0];
								}
////		for(j=0;j<base.nelt;j++)ntape>>wp.nextEle[i*base.nelt+j];
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.nextEle[wp.memWGa*i+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.nextEle[wp.memWGa*i+j]=larr[0];
								}
//		for(j=0;j<4;j++)ntape>>wp.snorm1[i*4+j];
		for(j=0;j<4;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.snorm1[i*4+j]=larr[0];
						}
//		for(j=0;j<4;j++)ntape>>wp.snorm2[i*4+j];
		for(j=0;j<4;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.snorm2[i*4+j]=larr[0];
						}
//		for(j=0;j<3;j++)ntape>>wp.circEles[i*3+j]; //What policy???
		for(j=0;j<3;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.circEles[i*3+j]=larr[0];
						}
////		for(j=0;j<base.nelt;j++)ntape>>wp.edgeEles[i*base.nelt+j];
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.edgeEles[wp.memWGa*i+j]=larr[0];
//								}



ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
//		for(j=0;j<accum;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.edgeEles[wp.memWGa*i+j]=larr[0];
//							}
////		for(j=0;j<4;j++)ntape>>wp.edgeNodes[i*4+j]; // correct???
//		for(j=0;j<4;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.edgeNodes[i*4+j]=larr[0];
//						}
		if(accum){ //Attempt to accommodate uncertain edgeEles/edgeNodes  EFP 4/09/2011
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.edgeEles[wp.memWGa*i+j]=larr[0];
								}
//		for(j=0;j<4;j++)ntape>>wp.edgeNodes[i*4+j]; // correct???
		for(j=0;j<4;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.edgeNodes[i*4+j]=larr[0];
						}
				 }


////		for(j=0;j<4*base.nelt;j++)ntape>>wp.sttEleNodes[i*base.nelt*4+j];
//		for(j=0;j<4*wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.sttEleNodes[i*wp.memWGa*4+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.sttEleNodes[i*wp.memWGa*4+j]=larr[0];
								}
//		ntape>>wp.curr[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.curr[i]=darr[0];
//		ntape>>wp.volt[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.volt[i]=darr[0];
//		ntape>>wp.eff[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.eff[i]=darr[0];
//		ntape>>wp.speed[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.speed[i]=darr[0];

//honk<<wp.curr[i]<<" "<<wp.volt[i]<<" "<<wp.eff[i]<<" "<<wp.speed[i]<<" MAATTTPPPROOPP\n";
//if(1==1)exit(0);

//		ntape>>wp.troom[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.troom[i]=darr[0];
//		ntape>>wp.tmelt[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.tmelt[i]=darr[0];
//		ntape>>wp.tcutl[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.tcutl[i]=darr[0];
//		ntape>>wp.tcuth[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.tcuth[i]=darr[0];
//		ntape>>wp.timeInterval[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.timeInterval[i]=darr[0];
//		ntape>>wp.maxiHeatStep[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.maxiHeatStep[i]=darr[0];
//		ntape>>wp.miniHeatStep[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.miniHeatStep[i]=darr[0];
//		ntape>>wp.thk1[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.thk1[i]=darr[0];
//		ntape>>wp.thk2[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.thk2[i]=darr[0];
//		ntape>>wp.lstart[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.lstart[i]=darr[0];
//		ntape>>wp.lend[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.lend[i]=darr[0];
////		for(j=0;j<base.nelt;j++)ntape>>wp.hlightel[i*base.nelt+j]; //Convention: 1st col= elem side#
//		for(j=0;j<wp.memWGa;j++){
//ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.hlightel[wp.memWGa*i+j]=larr[0];
//								}
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);accum=larr[0];
		for(j=0;j<accum;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.hlightel[wp.memWGa*i+j]=larr[0];
								}
//		for(j=0;j<3*NDF;j++)ntape>>wp.arrows[3*NDF*i+j]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(j=0;j<3*NDF;j++){
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.arrows[3*NDF*i+j]=darr[0];
							}
//		ntape>>wp.n_curr_sttEl[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.n_curr_sttEl[i]=larr[0];
//		ntape>>wp.wpTimes[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.wpTimes[i]=darr[0];
//		ntape>>wp.source[i];
ntape.getline(chtm,199);

//honk<<chtm<<" chtmHEATSOURCE\n";

parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.source[i]=larr[0];

//honk<<larr[0]<<" #heat refl source must be odddd\n";if(1==1)exit(0);

if(wp.source[i]==2*(wp.source[i]/2)){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"#heat reflection sources must be odd",L"Error",MB_OK);}//EFP 3/13/2012
//		ntape>>wp.mcr[i]; // Not part of original VFT
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wp.mcr[i]=larr[0];
//		ntape>>ii;wp.WeldColor[i]=(TColor)ii;
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);
////////////////////////////////////
honk<<i<<" VFTrImpColor "<<larr[0]<<"\n";
////////////////////////////////////
wp.WeldColor[i]=(TColor)larr[0];

ntape.getline(chtm,199);

														   }
							   }
//				 for(i=0;i<wp.nWeldPass;i++){
//											 for(j=0;j<wp.memWGa;j++)
//											   {if(wp.eles[wp.memWGa*i+j]<0)break;
//												else {ie=wp.eles[wp.memWGa*i+j]/10;
////eltype=base.matno[ie]/t7;bscode=(base.matno[ie]-eltype*t7)/t5;node=(base.matno[ie]-eltype*t7-bscode*t5)/t3;
////ieGID=base.matno[ie]-eltype*t7-bscode*t5-node*t3;
//////base.matno[ie]=base.matno[ie]-ieGID+wp.nWeldGroup+i+1;
////base.matno[ie]=base.matno[ie]-ieGID+base.allGrp+i;
//temp_allGID[base.nelt*(base.allGrp+i)+ie]=1;//Add coding for basemetal&weldgroup
//													 }
//											   }
////											 wp.prevGID[i]=ieGID; //Correction EFP 3/27/2011
///////////////
////honk<<i<<" read in wp.prevGID[i] "<<wp.prevGID[i]<<"\n";
///////////////
//											}
//nGID=wp.nWeldGroup+wp.nWeldPass+1;nGIDmax=nGID;
//NNNNNNNNNNNNNNNNNNNN
//				 ntape>>wps.nWeldParamSet;
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wps.nWeldParamSet=larr[0];

honk<<wps.nWeldParamSet<<" wps.nWeldParamSet\n";

				 if(wps.nWeldParamSet){
									   FDdynmem_manage(11,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
									   for(i=0;i<wps.nWeldParamSet;i++){








////		ntape>>descript0;wps.name[i]=L"WeldParamName";
//		ntape>>descript0;wps.name[i]=descript0;
ntape.getline(chtm,199);

wps.name[i]=chtm;

wps.name[i].SetLength(wps.name[i].Length()-1);

//wps.name[i]=(String)chtm;
//wps.name[i]=chtm.t_str();
//wps.name[i]=L"ding";

//honk<<chtm<<" chtm\n";
//honk<<wps.name[i].c_str()<<" wps.name[i]\n";

//// _ltow(i+1,string1,10);
//// StringCchCopyW(curMess1,39+1,curMess0); //Length of destination curMess0 is provided.
//// StringCchCatW(curMess1,39+1,string1); //Length of destination curMess0 is provided.
//// wps.name[i]=curMess1;
//efpChar=new char[strlen(chtm)];StringCchCopyA(efpChar,strlen(chtm),chtm);
//wps.name[i]=PAnsiChar(efpChar);delete efpChar; *efpChar=NULL;



//////////////////////////////
//temp_cht=new _TCHAR[strlen(chtm)-1]; // This presumes the format  ** WeldGroupName\n
//for(j=0;j<int(strlen(chtm))-1;j++)temp_cht[j]=chtm[j];
//wps.name[i]= *temp_cht; //EFP 3/15/2011
//delete [] temp_cht;*temp_cht=NULL;
//////////////////////////////




//		ntape>>wps.curr[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wps.curr[i]=darr[0];

honk<<chtm<<" chtm\n";
honk<<wps.curr[i]<<" wps.curr[i]\n";

//		ntape>>wps.volt[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wps.volt[i]=darr[0];

honk<<chtm<<" chtm\n";
honk<<wps.volt[i]<<" wps.volt[i]\n";

//		ntape>>wps.eff[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wps.eff[i]=darr[0];

honk<<chtm<<" chtm\n";
honk<<wps.eff[i]<<" wps.eff[i]\n";

//		ntape>>wps.speed[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wps.speed[i]=darr[0];

honk<<chtm<<" chtm\n";
honk<<wps.speed[i]<<" wps.speed[i]\n";
ntape.getline(chtm,199);

																	  }
									  }
//				 ntape>>wms.nMatPropSet;
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.nMatPropSet=larr[0];
				 if(wms.nMatPropSet){
									 FDdynmem_manage(10,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
									 for(i=0;i<wms.nMatPropSet;i++){
//		ntape>>descript0;wms.name[i]=L"WeldMaterialSet";
//		ntape>>descript0;wms.matFileName[i]=L"MatPropFileName";
//		ntape>>descript0;wms.Steps[i]=L"Steps";

////		ntape>>descript0;wms.name[i]=descript0;
ntape.getline(chtm,199);
//ntape.getline(efpAnsi1,19);

//honk<<strlen(chtm)<<" wms.name strlen\n";
//honk<<strlen(efpAnsi1)<<" wms.name strlen\n";

//wms.name[i]=chtm;
// StringCchCopyW(wms.name[i],strlen(chtm),(String)chtm);
//StrPCopy();
//StrPLCopy(wms.name[i],chtm,strlen(chtm)-1);

//efpAnsi=new String[strlen(chtm)];
//efpAnsi=chtm;
// StringCchCopyA(efpAnsi,strlen(chtm),chtm);
//wms.name[i]=efpAnsi;

//delete efpAnsi;
//AnsiToUnicode(chtm,&efpAnsi);
//wms.name[i]=string(chtm);
//wms.name[i]=PAnsiChar(chtm);




//efpChar=new _TCHAR[strlen(chtm)];
////StringCchCopyA(efpChar,strlen(chtm),chtm);
////wms.name[i]=PAnsiChar(efpChar);
//wms.name[i]= *texasbuf;
//delete efpChar; *efpChar=NULL;
//aaaaaaaaaaaaaaaa
wms.name[i]=chtm;

wms.name[i].SetLength(wms.name[i].Length()-1);
//bbbbbbbbbbbbbbbb


//		ntape>>descript0;wms.matFileName[i]=descript0;
ntape.getline(chtm,199);
wms.matFileName[i]=chtm;

wms.matFileName[i].SetLength(wms.matFileName[i].Length()-1);
//efpChar=new _TCHAR[strlen(chtm)];
////StringCchCopyA(efpChar,strlen(chtm),chtm);
////wms.matFileName[i]=PAnsiChar(efpChar);
//wms.matFileName[i]= *texasbuf;
//delete efpChar; *efpChar=NULL;




//		ntape>>descript0;wms.Steps[i]=descript0;
ntape.getline(chtm,199);
wms.Steps[i]=chtm;

wms.Steps[i].SetLength(wms.Steps[i].Length()-1);
//efpChar=new _TCHAR[strlen(chtm)];
////StringCchCopyA(efpChar,strlen(chtm),chtm);
////wms.Steps[i]=PAnsiChar(efpChar);
//wms.Steps[i]= *texasbuf;
//delete efpChar; *efpChar=NULL;



//		ntape>>wms.mcr[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.mcr[i]=larr[0];
//		ntape>>wms.annjd[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.annjd[i]=larr[0];
//		ntape>>wms.ved[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.ved[i]=larr[0];
//		ntape>>wms.hetjd[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.hetjd[i]=larr[0];
//		ntape>>wms.nprops[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.nprops[i]=larr[0];
//		ntape>>wms.cond[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.cond[i]=darr[0];
//		ntape>>wms.heat[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.heat[i]=darr[0];
//		ntape>>wms.den[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.den[i]=darr[0];
//		ntape>>wms.Ti[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.Ti[i]=darr[0];
//		ntape>>wms.Ta[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.Ta[i]=darr[0];
//		ntape>>wms.Tm[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.Tm[i]=darr[0];
//		ntape>>wms.switc[i];
ntape.getline(chtm,199);parse_cdm(chtm,1,&nic,&nrc,larr,darr);wms.switc[i]=bool(larr[0]);
ntape.getline(chtm,199);
																  }
									}
ntape.getline(chtm,199);
	   ntape.close();
//	   GeomFileName=OpenDialog1->FileName;

//TBD: NodalCoincidence

nGID=wp.nWeldGroup+wp.nWeldPass+1;nGIDmax=nGID;
//honk<<base.allGrp<<" "<<wp.nWeldGroup<<" "<<wp.nWeldPass<<" Deschanel\n";
///////////////////////////////////


///////////////////// EFP 2/12/2012
//////////////////////
///////////////////////
base.ELSETelsum=base.nelt+sumELSETel;

for(i=0;i<wp.nWeldPass;i++){for(j=0;j<wp.memWGa;j++)
							   {if(wp.eles[wp.memWGa*i+j]>=0)
								  {ie=wp.eles[wp.memWGa*i+j]/10;
//								   wp.prevGID[i]=base.arrELSET[ base.trackELSET[ie]+1 ];
								   wp.prevGID[i]=base.arrELSET[ie];
								   break;
								  }
							   }
						   }
//if(1==1)exit(0);
///////////////////////
//////////////////////
/////////////////////

for(ii=0;ii<base.nelt;ii++)base.orig_matno[ii]=base.matno[ii]; //TBD: Revise foregoing coding to allow for this new line  EFP 1/23/2012
/////////////////////////////////////
	   wp.highest=wp.nWeldPass;wps.highest=wps.nWeldParamSet;wms.highest=wms.nMatPropSet;
	   FDbase_indat(3,shapecombo,iplotflag,nColRes);
/////////////////////////////////////

//			   FDdynmem_manage(-16,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
			   FDdynmem_manage(-17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);
////			   FDdynmem_manage(16,dummy,base.nelt,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy);//EFP 8/07/2011
//			   FDdynmem_manage(17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt+2*sumELSETel);//EFP 8/07/2011
			   FDdynmem_manage(17,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,dummy,base.nelt);//EFP 8/07/2011
			   indat.GIDcol=base.GIDcol;
			   for(in=0;in<base.nelt;in++)indat.arrELSET[in]=base.arrELSET[in];

////	}
////__finally {
//Screen->Cursor=Save_Cursor;
////}





	if(nGID<1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No geometry IDs found",L"Halt",MB_OK);return;}
//    else {if(iPaintyesno/10==0){FDelemfacets2();iPaintyesno=10;iCullyesno=0;}iCircleplot=1;Invalidate();}
	else {if(iPaintyesno/10==0){
//								FDcomp_nGID();FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);
								if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
//								else {FDcomp_nGID();FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);}
//								iPaintyesno=10;iCullyesno=0;
								else {
////								FDcomp_nGID();
//									  FDcomp_nGID(indat.nelt,indat.matno,&nGID,arGID);
FDcomp_nGID(indat.nelt,&nGID,arGID);
									  FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);
									 }
								iPaintyesno=10+1;iCullyesno=0;
//								iPaintyesno=10;iCullyesno=0;
							   }
//			 stateVFT=0;FD_LButtonstatus=11;
			 stateVFT=2;FD_LButtonstatus=11;

honk<<base.npoin<<" exiting ImportVFTr "<<base.nelt<<"\n";
Form1->Caption=GeomFileName;
///////////// Cursor EFP 1/21/2011
//Screen->Cursor=crSizeAll;
/////////////

		  iplotflag=1;iCircleplot=1;Invalidate();
		 }
////////////////
/////////// Cursor EFP 1/03/2013
//Screen->Cursor=crSizeAll;
Screen->Cursor=Save_Cursor;
///////////
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen *.VFTr file",L"Failure",MB_OK);}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Nodes/elements missing from *.VFTr datafile",L"Failure",MB_OK);}








					 }
//	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not a *.VFTr format file",L"Failure",MB_OK);}
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not a version3 *.VFTr format file",L"Failure",MB_OK);}
////////////////
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *.VFTr file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
 }
}
//---------------------------------------------------------------------------
void TForm1::FDdynmem_manage(int isel,long bnpoin,long bnelt,long inpoin,long inelt,long nCR,long bnvfix,long bnedge,long bpload,long bnblod,long bmat,long bcoorf,long infsect,long MXNP)
// Global NDF
{
//**************** Start from VFTgen
 int *SWTseqNum=NULL,*SWTboolFlags=NULL,*SWTtype=NULL,*SWTshape=NULL,*SWTstepInterval=NULL,
	 *SWThp=NULL,*SWTnsegs=NULL,*SWTiselect=NULL,*SWTmcr=NULL,*SWTsource=NULL,*SWTutil_arr=NULL
	 ;
// GIDwp: 1st col= current iside, remainder=GID
 long in=0,dum=0,*SWTeles=NULL,*SWTsttEles=NULL,*SWTstpEle=NULL,*SWTfirstEle=NULL,
	  *SWTnextEle=NULL,*SWTsnorm1=NULL,*SWTsnorm2=NULL,
	  *SWTcircEles=NULL,*SWTedgeEles=NULL,*SWTedgeNodes=NULL,*SWTsttEleNodes=NULL,
	  *SWThlightel=NULL,*SWTn_curr_sttEl=NULL,*SWTprevGID=NULL,*SWTreset=NULL;
 float *SWTcurr=NULL,*SWTvolt=NULL,*SWTeff=NULL,*SWTspeed=NULL,*SWTtroom=NULL,*SWTtmelt=NULL,
	   *SWTtcutl=NULL,*SWTtcuth=NULL,*SWTtimeInterval=NULL,
	   *SWTmaxiHeatStep=NULL,*SWTminiHeatStep=NULL,*SWTthk1=NULL,*SWTthk2=NULL,*SWTwpTimes=NULL;
 double *SWTlstart=NULL,*SWTlend=NULL,*SWTarrows=NULL;
 String *SWTname=NULL,*SWTmatName=NULL;
 TColor *SWTWeldColor=NULL;
// boolFlags[]: showFlag=1st digit,circFlag=2nd,edgeFlag=3rd,merge=4th,girthFlag=5th

 int *SWMTmcr=NULL,*SWMTannjd=NULL,*SWMTved=NULL,*SWMThetjd=NULL,*SWMTnprops=NULL;
 float *SWMTcond=NULL,*SWMTheat=NULL,*SWMTden=NULL,*SWMTTi=NULL,*SWMTTa=NULL,*SWMTTm=NULL;
 String *SWMTname=NULL,*SWMTmatFileName=NULL,*SWMTSteps=NULL;
 bool *SWMTswitc=NULL;

 float *SWPTcurr=NULL,*SWPTvolt=NULL,*SWPTeff=NULL,*SWPTspeed=NULL;
 String *SWPTname=NULL;
//**************** End from VFTgen

 if(isel==1)
  {
   try {
		base.nop1=new long[MXNP*bnelt];base.matno=new long[bnelt];base.c1=new float[NDF*bnpoin];
		base.el_map=new long[bnelt];base.arELEM=new int[bnelt];
		base.node_map=new long[bnpoin];base.orig_matno=new long[bnelt];
// SuperEl
		base.nlp=new long[bnelt];base.beta=new float[NDF*bnelt];
// New FE-related arrays
		if(bnvfix){base.nofix=new long[2*bnvfix];base.nrfix=new long[bnvfix];base.presc=new float[NDF*bnvfix];}
		if(bcoorf)base.cangl=new float[9*bcoorf];
		if(bnedge){base.nldel=new long[2*bnedge];base.press=new float[3*bnedge];}
		if(bpload){base.lodpt=new long[bpload];base.pointld=new float[NDF*bpload];}
		if(bnblod){base.ebody=new long[bnblod];base.body=new float[NDF*bnblod];}
		if(bmat){base.matyp=new long[bmat];base.prop=new float[MPROP*bmat];}
	   }
   catch (EOutOfMemory &e){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Exit",MB_OK);exit(0);}
  }
 else if(isel==2)
  {
   try {
		indat.nop1=new long[MXNP*inelt];indat.matno=new long[inelt];
		indat.matnoW=new long[inelt];
		indat.c1=new float[NDF*inpoin];indat.bc1=new float[NDF*inpoin];
		if(infsect)indat.fsect=new long[infsect];
// SuperEl
		indat.nlp=new long[inelt];indat.beta=new float[NDF*inelt];
	   }
   catch (EOutOfMemory &e){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Exit",MB_OK);exit(0);}
  }
 else if(isel==3)
  {
   try {
		base.res1=new float[nCR*bnpoin];
	   }
   catch (EOutOfMemory &e){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Exit",MB_OK);exit(0);}
  }
 else if(isel==4)
  {
   try {
		indat.res1=new float[nCR*inpoin];indat.result=new float[inpoin];
	   }
   catch (EOutOfMemory &e){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Exit",MB_OK);exit(0);}
  }
 else if(isel==6)
  {
//   try {
/*              // Taken from WeldPass.java
	protected String name="";
	protected int seqNum;
	protected Vector eles;
	protected Vector sttEles;
	protected int stpEle;
	protected int firstEle;
	protected int nextEle;
	protected int[] snorm1, snorm2;
	protected Color color;
	protected boolean showFlag;
	protected String matName="";
	protected float curr;
	protected float volt;
	protected float eff;
	protected float speed;
	protected float troom;
	protected float tmelt;
	protected float tcutl;
	protected float tcuth;
	protected int source=11;
	protected int type;
	protected int shape;
	protected float timeInterval;
	protected int stepInterval;
	protected float maxiHeatStep;
	protected float miniHeatStep;
	protected float thk1, thk2;
	protected double lstart,lend;
	protected boolean circFlag=false;
	protected int[] circEles;
	protected boolean edgeFlag=false;
	protected Vector edgeEles;
	protected Vector edgeNodes;

	protected Hashtable secHt;
	protected Vector sttEleNodes;
//  Lump Pass Version
	protected int hp=0;
//    protected float thaz=70;
//    protected float halfw;
//    protected float halfs;
	protected boolean merge=false;
	protected int nsegs=1;
//
// Girth Weld Added
	protected boolean girthFlag=false;
// Edit Purposes added selected group for weld pass
	protected int iselect=0;
*/
/* // Start temporary disconnect of VFTmisc code
		wp.seqNum=new int[MAX_WELDPASSES];
		wp.boolFlags=new int[MAX_WELDPASSES];
		wp.type=new int[MAX_WELDPASSES];
		wp.shape=new int[MAX_WELDPASSES];
		wp.stepInterval=new int[MAX_WELDPASSES];
		wp.hp=new int[MAX_WELDPASSES];
		wp.nsegs=new int[MAX_WELDPASSES];
		wp.iselect=new int[MAX_WELDPASSES];
		wp.eles=new long[MAX_WELDPASSES*bnelt];  // Elements in this weld pass
//		wp.sttEles=new long[MAX_WELDPASSES*bnelt*4];  // No need for *4 ????
		wp.sttEles=new long[MAX_WELDPASSES*bnelt];  // No need for *4 ????
//		wp.stpEle=new long[MAX_WELDPASSES*bnelt];  // No need for *bnelt
		wp.stpEle=new long[MAX_WELDPASSES];  // No need for *bnelt
		wp.firstEle=new long[MAX_WELDPASSES*bnelt];
		wp.nextEle=new long[MAX_WELDPASSES*bnelt];
		wp.snorm1=new long[MAX_WELDPASSES*4];
		wp.snorm2=new long[MAX_WELDPASSES*4];
		wp.circEles=new long[MAX_WELDPASSES*3];
		wp.edgeEles=new long[MAX_WELDPASSES*bnelt];
		wp.edgeNodes=new long[MAX_WELDPASSES*4]; // correct???
		wp.sttEleNodes=new long[MAX_WELDPASSES*bnelt*4];
		wp.curr=new float[MAX_WELDPASSES];
		wp.volt=new float[MAX_WELDPASSES];
		wp.eff=new float[MAX_WELDPASSES];
		wp.speed=new float[MAX_WELDPASSES];
		wp.troom=new float[MAX_WELDPASSES];
		wp.tmelt=new float[MAX_WELDPASSES];
		wp.tcutl=new float[MAX_WELDPASSES];
		wp.tcuth=new float[MAX_WELDPASSES];
		wp.timeInterval=new float[MAX_WELDPASSES];
		wp.maxiHeatStep=new float[MAX_WELDPASSES];
		wp.miniHeatStep=new float[MAX_WELDPASSES];
		wp.thk1=new float[MAX_WELDPASSES];
		wp.thk2=new float[MAX_WELDPASSES];
		wp.lstart=new double[MAX_WELDPASSES];
		wp.lend=new double[MAX_WELDPASSES];
		wp.hlightel=new long[MAX_WELDPASSES*bnelt]; //Convention: 1st col= elem side#
		wp.arrows=new double[3*NDF*MAX_WELDPASSES]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		wp.n_curr_sttEl=new long[MAX_WELDPASSES];
		wp.wpTimes=new long[MAX_WELDPASSES];
		wp.source=new float[MAX_WELDPASSES];
			// End temporary disconnect of VFTmisc code
*/
//	  wp.edgeFlag=new int[MAX_WELDPASSES];
//        wp.circFlag=new int[MAX_WELDPASSES];
//        wp.merge==new int[MAX_WELDPASSES];

//	   }
//   catch (xalloc){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Exit",MB_OK);exit(0);}

//**************** Start from VFTgen
   if(wp.nWeldPass){
		SWTname=new String[wp.nWeldPass+1];
		SWTmatName=new String[wp.nWeldPass+1];
		SWTseqNum=new int[wp.nWeldPass+1];
		SWTboolFlags=new int[wp.nWeldPass+1];
		SWTtype=new int[wp.nWeldPass+1];
		SWTshape=new int[wp.nWeldPass+1];
		SWTstepInterval=new int[wp.nWeldPass+1];
		SWThp=new int[wp.nWeldPass+1];
		SWTnsegs=new int[wp.nWeldPass+1];
		SWTiselect=new int[wp.nWeldPass+1];
		SWTeles=new long[(wp.nWeldPass+1)*bnelt];  // Elements in this weld pass
//		SWTsttEles=new long[wp.nWeldPass*bnelt*4];  // No need for *4 ????
		SWTsttEles=new long[(wp.nWeldPass+1)*bnelt];  // No need for *4 ????
//		SWTstpEle=new long[wp.nWeldPass*bnelt];  // No need for *bnelt
		SWTstpEle=new long[wp.nWeldPass+1];  // No need for *bnelt
		SWTfirstEle=new long[(wp.nWeldPass+1)*bnelt];
		SWTnextEle=new long[(wp.nWeldPass+1)*bnelt];
		SWTsnorm1=new long[(wp.nWeldPass+1)*4]; //These are actually NODE NUMBERS
		SWTsnorm2=new long[(wp.nWeldPass+1)*4]; //These are actually NODE NUMBERS
		SWTcircEles=new long[(wp.nWeldPass+1)*3]; //These are actually NODE NUMBERS
		SWTedgeEles=new long[(wp.nWeldPass+1)*bnelt];
		SWTedgeNodes=new long[(wp.nWeldPass+1)*4]; // correct???
		SWTsttEleNodes=new long[(wp.nWeldPass+1)*bnelt*4];
		SWTcurr=new float[wp.nWeldPass+1];
		SWTvolt=new float[wp.nWeldPass+1];
		SWTeff=new float[wp.nWeldPass+1];
		SWTspeed=new float[wp.nWeldPass+1];
		SWTtroom=new float[wp.nWeldPass+1];
		SWTtmelt=new float[wp.nWeldPass+1];
		SWTtcutl=new float[wp.nWeldPass+1];
		SWTtcuth=new float[wp.nWeldPass+1];
		SWTtimeInterval=new float[wp.nWeldPass+1];
		SWTmaxiHeatStep=new float[wp.nWeldPass+1];
		SWTminiHeatStep=new float[wp.nWeldPass+1];
		SWTthk1=new float[wp.nWeldPass+1];
		SWTthk2=new float[wp.nWeldPass+1];
		SWTlstart=new double[wp.nWeldPass+1];
		SWTlend=new double[wp.nWeldPass+1];
		SWThlightel=new long[(wp.nWeldPass+1)*bnelt]; //Convention: 1st col= elem side#
		SWTarrows=new double[3*NDF*(wp.nWeldPass+1)]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		SWTn_curr_sttEl=new long[wp.nWeldPass+1];
		SWTreset=new long[wp.nWeldPass+1];
//		SWTwpTimes=new long[wp.nWeldPass];
		SWTwpTimes=new float[wp.nWeldPass+1];
		SWTsource=new int[wp.nWeldPass+1];
		SWTmcr=new int[wp.nWeldPass+1]; // Not part of original VFT
		SWTWeldColor=new TColor[wp.nWeldPass+1];
		SWTprevGID=new long[wp.nWeldPass+1];
		SWTutil_arr=new int[wp.nWeldPass+1];

		for(in=0;in<wp.nWeldPass;in++) *(SWTname+in)= *(wp.name+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTmatName+in)= *(wp.matName+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTseqNum+in)= *(wp.seqNum+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTboolFlags+in)= *(wp.boolFlags+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtype+in)= *(wp.type+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTshape+in)= *(wp.shape+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTstepInterval+in)= *(wp.stepInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWThp+in)= *(wp.hp+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTnsegs+in)= *(wp.nsegs+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTiselect+in)= *(wp.iselect+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWTeles+in)= *(wp.eles+in);  // Elements in this weld pass
//		SWTsttEles=new long[wp.nWeldPass*bnelt*4];  // No need for *4 ????
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWTsttEles+in)= *(wp.sttEles+in);  // No need for *4 ????
//		SWTstpEle=new long[wp.nWeldPass*bnelt];  // No need for *bnelt
		for(in=0;in<wp.nWeldPass;in++) *(SWTstpEle+in)= *(wp.stpEle+in);  // No need for *bnelt
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWTfirstEle+in)= *(wp.firstEle+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWTnextEle+in)= *(wp.nextEle+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(SWTsnorm1+in)= *(wp.snorm1+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(SWTsnorm2+in)= *(wp.snorm2+in);
		for(in=0;in<wp.nWeldPass*3;in++) *(SWTcircEles+in)= *(wp.circEles+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWTedgeEles+in)= *(wp.edgeEles+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(SWTedgeNodes+in)= *(wp.edgeNodes+in); // correct???
		for(in=0;in<wp.nWeldPass*bnelt*4;in++) *(SWTsttEleNodes+in)= *(wp.sttEleNodes+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTcurr+in)= *(wp.curr+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTvolt+in)= *(wp.volt+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTeff+in)= *(wp.eff+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTspeed+in)= *(wp.speed+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtroom+in)= *(wp.troom+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtmelt+in)= *(wp.tmelt+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtcutl+in)= *(wp.tcutl+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtcuth+in)= *(wp.tcuth+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTtimeInterval+in)= *(wp.timeInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTmaxiHeatStep+in)= *(wp.maxiHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTminiHeatStep+in)= *(wp.miniHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTthk1+in)= *(wp.thk1+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTthk2+in)= *(wp.thk2+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTlstart+in)= *(wp.lstart+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTlend+in)= *(wp.lend+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(SWThlightel+in)= *(wp.hlightel+in); //Convention: 1st col= elem side#
		for(in=0;in<wp.nWeldPass*3*NDF;in++) *(SWTarrows+in)= *(wp.arrows+in); //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(in=0;in<wp.nWeldPass;in++) *(SWTn_curr_sttEl+in)= *(wp.n_curr_sttEl+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTreset+in)= *(wp.reset+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTwpTimes+in)= *(wp.wpTimes+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTsource+in)= *(wp.source+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTmcr+in)= *(wp.mcr+in); // Not part of original VFT
		for(in=0;in<wp.nWeldPass;in++) *(SWTWeldColor+in)= *(wp.WeldColor+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTprevGID+in)= *(wp.prevGID+in);
		for(in=0;in<wp.nWeldPass;in++) *(SWTutil_arr+in)= *(wp.util_arr+in);

//		FDdynmem_manageD(-6,dum,bnelt,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
		FDdynmem_manage(-6,dum,bnelt,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
//
		wp.name=new String[wp.nWeldPass+1+MXNP];
		wp.matName=new String[wp.nWeldPass+1+MXNP];
		wp.seqNum=new int[wp.nWeldPass+1+MXNP];
		wp.boolFlags=new int[wp.nWeldPass+1+MXNP];
		wp.type=new int[wp.nWeldPass+1+MXNP];
		wp.shape=new int[wp.nWeldPass+1+MXNP];
		wp.stepInterval=new int[wp.nWeldPass+1+MXNP];
		wp.hp=new int[wp.nWeldPass+1+MXNP];
		wp.nsegs=new int[wp.nWeldPass+1+MXNP];
		wp.iselect=new int[wp.nWeldPass+1+MXNP];
		wp.eles=new long[(wp.nWeldPass+1+MXNP)*bnelt];  // Elements in this weld pass
//		wp.sttEles=new long[wp.nWeldPass+2*bnelt*4];  // No need for *4 ????
		wp.sttEles=new long[(wp.nWeldPass+1+MXNP)*bnelt];  // No need for *4 ????
//		wp.stpEle=new long[wp.nWeldPass+2*bnelt];  // No need for *bnelt
		wp.stpEle=new long[wp.nWeldPass+1+MXNP];  // No need for *bnelt
		wp.firstEle=new long[(wp.nWeldPass+1+MXNP)*bnelt];
		wp.nextEle=new long[(wp.nWeldPass+1+MXNP)*bnelt];
		wp.snorm1=new long[(wp.nWeldPass+1+MXNP)*4]; //These are actually NODE NUMBERS
		wp.snorm2=new long[(wp.nWeldPass+1+MXNP)*4]; //These are actually NODE NUMBERS
		wp.circEles=new long[(wp.nWeldPass+1+MXNP)*3]; //These are actually NODE NUMBERS
		wp.edgeEles=new long[(wp.nWeldPass+1+MXNP)*bnelt];
		wp.edgeNodes=new long[(wp.nWeldPass+1+MXNP)*4]; // correct???
		wp.sttEleNodes=new long[(wp.nWeldPass+1+MXNP)*bnelt*4];
		wp.curr=new float[wp.nWeldPass+1+MXNP];
		wp.volt=new float[wp.nWeldPass+1+MXNP];
		wp.eff=new float[wp.nWeldPass+1+MXNP];
		wp.speed=new float[wp.nWeldPass+1+MXNP];
		wp.troom=new float[wp.nWeldPass+1+MXNP];
		wp.tmelt=new float[wp.nWeldPass+1+MXNP];
		wp.tcutl=new float[wp.nWeldPass+1+MXNP];
		wp.tcuth=new float[wp.nWeldPass+1+MXNP];
		wp.timeInterval=new float[wp.nWeldPass+1+MXNP];
		wp.maxiHeatStep=new float[wp.nWeldPass+1+MXNP];
		wp.miniHeatStep=new float[wp.nWeldPass+1+MXNP];
		wp.thk1=new float[wp.nWeldPass+1+MXNP];
		wp.thk2=new float[wp.nWeldPass+1+MXNP];
		wp.lstart=new double[wp.nWeldPass+1+MXNP];
		wp.lend=new double[wp.nWeldPass+1+MXNP]; //Unused so use it for weld-set time  EFP 5/19/2012
		wp.hlightel=new long[(wp.nWeldPass+1+MXNP)*bnelt]; //Convention: 1st col= elem side#
		wp.arrows=new double[3*NDF*(wp.nWeldPass+1+MXNP)]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		wp.n_curr_sttEl=new long[wp.nWeldPass+1+MXNP];
		wp.reset=new long[wp.nWeldPass+1+MXNP];
//		wp.wpTimes=new long[wp.nWeldPass+2];
		wp.wpTimes=new float[wp.nWeldPass+1+MXNP];
		wp.source=new int[wp.nWeldPass+1+MXNP];
		wp.mcr=new int[wp.nWeldPass+1+MXNP]; // Not part of original VFT
		wp.WeldColor=new TColor[wp.nWeldPass+1+MXNP];
		wp.prevGID=new long[wp.nWeldPass+1+MXNP];
//		wp.util_arr=new int[wp.nWeldPass+2];
		wp.util_arr=new long[wp.nWeldPass+1+MXNP];

		for(in=0;in<wp.nWeldPass;in++) *(wp.name+in)= *(SWTname+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.matName+in)= *(SWTmatName+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.seqNum+in)= *(SWTseqNum+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.boolFlags+in)= *(SWTboolFlags+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.type+in)= *(SWTtype+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.shape+in)= *(SWTshape+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.stepInterval+in)= *(SWTstepInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.hp+in)= *(SWThp+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.nsegs+in)= *(SWTnsegs+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.iselect+in)= *(SWTiselect+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.eles+in)= *(SWTeles+in);  // Elements in this weld pass
//		wp.sttEles=new long[wp.nWeldPass*bnelt*4];  // No need for *4 ????
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.sttEles+in)= *(SWTsttEles+in);  // No need for *4 ????
//		wp.stpEle=new long[wp.nWeldPass*bnelt];  // No need for *bnelt
		for(in=0;in<wp.nWeldPass;in++) *(wp.stpEle+in)= *(SWTstpEle+in);  // No need for *bnelt
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.firstEle+in)= *(SWTfirstEle+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.nextEle+in)= *(SWTnextEle+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.snorm1+in)= *(SWTsnorm1+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.snorm2+in)= *(SWTsnorm2+in);
		for(in=0;in<wp.nWeldPass*3;in++) *(wp.circEles+in)= *(SWTcircEles+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.edgeEles+in)= *(SWTedgeEles+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.edgeNodes+in)= *(SWTedgeNodes+in); // correct???
		for(in=0;in<wp.nWeldPass*bnelt*4;in++) *(wp.sttEleNodes+in)= *(SWTsttEleNodes+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.curr+in)= *(SWTcurr+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.volt+in)= *(SWTvolt+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.eff+in)= *(SWTeff+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.speed+in)= *(SWTspeed+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.troom+in)= *(SWTtroom+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tmelt+in)= *(SWTtmelt+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tcutl+in)= *(SWTtcutl+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tcuth+in)= *(SWTtcuth+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.timeInterval+in)= *(SWTtimeInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.maxiHeatStep+in)= *(SWTmaxiHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.miniHeatStep+in)= *(SWTminiHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.thk1+in)= *(SWTthk1+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.thk2+in)= *(SWTthk2+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.lstart+in)= *(SWTlstart+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.lend+in)= *(SWTlend+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.hlightel+in)= *(SWThlightel+in); //Convention: 1st col= elem side#
		for(in=0;in<wp.nWeldPass*3*NDF;in++) *(wp.arrows+in)= *(SWTarrows+in); //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(in=0;in<wp.nWeldPass;in++) *(wp.n_curr_sttEl+in)= *(SWTn_curr_sttEl+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.reset+in)= *(SWTreset+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.wpTimes+in)= *(SWTwpTimes+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.source+in)= *(SWTsource+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.mcr+in)= *(SWTmcr+in); // Not part of original VFT
		for(in=0;in<wp.nWeldPass;in++) *(wp.WeldColor+in)= *(SWTWeldColor+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.prevGID+in)= *(SWTprevGID+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.util_arr+in)= *(SWTutil_arr+in);

					delete [] SWTname;SWTname=NULL;
					delete [] SWTmatName;SWTmatName=NULL;
					delete [] SWTseqNum;SWTseqNum=NULL;
					delete [] SWTboolFlags;SWTboolFlags=NULL;
					delete [] SWTtype;SWTtype=NULL;
					delete [] SWTshape;SWTshape=NULL;
					delete [] SWTstepInterval;SWTstepInterval=NULL;
					delete [] SWThp;SWThp=NULL;
					delete [] SWTnsegs;SWTnsegs=NULL;
					delete [] SWTiselect;SWTiselect=NULL;
					delete [] SWTeles;SWTeles=NULL;
					delete [] SWTsttEles;SWTsttEles=NULL;
					delete [] SWTstpEle;SWTstpEle=NULL;
					delete [] SWTfirstEle;SWTfirstEle=NULL;
					delete [] SWTnextEle;SWTnextEle=NULL;
					delete [] SWTsnorm1;SWTsnorm1=NULL;
					delete [] SWTsnorm2;SWTsnorm2=NULL;
					delete [] SWTcircEles;SWTcircEles=NULL;
					delete [] SWTedgeEles;SWTedgeEles=NULL;
					delete [] SWTedgeNodes;SWTedgeNodes=NULL;
					delete [] SWTsttEleNodes;SWTsttEleNodes=NULL;
					delete [] SWTcurr;SWTcurr=NULL;
					delete [] SWTvolt;SWTvolt=NULL;
					delete [] SWTeff;SWTeff=NULL;
					delete [] SWTspeed;SWTspeed=NULL;
					delete [] SWTtroom;SWTtroom=NULL;
					delete [] SWTtmelt;SWTtmelt=NULL;
					delete [] SWTtcutl;SWTtcutl=NULL;
					delete [] SWTtcuth;SWTtcuth=NULL;
					delete [] SWTtimeInterval;SWTtimeInterval=NULL;
					delete [] SWTmaxiHeatStep;SWTmaxiHeatStep=NULL;
					delete [] SWTminiHeatStep;SWTminiHeatStep=NULL;
					delete [] SWTthk1;SWTthk1=NULL;
					delete [] SWTthk2;SWTthk2=NULL;
					delete [] SWTlstart;SWTlstart=NULL;
					delete [] SWTlend;SWTlend=NULL;
					delete [] SWThlightel;SWThlightel=NULL;
					delete [] SWTarrows;SWTarrows=NULL;
					delete [] SWTn_curr_sttEl;SWTn_curr_sttEl=NULL;
					delete [] SWTreset;SWTreset=NULL;
					delete [] SWTwpTimes;SWTwpTimes=NULL;
					delete [] SWTsource;SWTsource=NULL;
					delete [] SWTmcr;SWTmcr=NULL;// Not part of original VFT
					delete [] SWTWeldColor;SWTWeldColor=NULL;
					delete [] SWTprevGID;SWTprevGID=NULL;
					delete [] SWTutil_arr;SWTutil_arr=NULL;

//			   for(in=(wp.nWeldPass-1)*bnelt;in<wp.nWeldPass*bnelt;in++)wp.eles[in]= -1;
//			   for(in=(wp.nWeldPass-1)*bnelt;in<wp.nWeldPass*bnelt;in++)wp.sttEles[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.eles[in]= -1;
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.sttEles[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.firstEle[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.nextEle[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.edgeEles[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass*bnelt*4;in<(wp.nWeldPass+1+MXNP)*bnelt*4;in++)wp.sttEleNodes[in]= -1;// No need for *4 ????
			   for(in=wp.nWeldPass;in<wp.nWeldPass+1+MXNP;in++)wp.stpEle[in]= -1;  // No need for *bnelt
//			   for(in=(wp.nWeldPass-1)*4;in<wp.nWeldPass*4;in++)wp.snorm1[in]= -1;
//			   for(in=(wp.nWeldPass-1)*4;in<wp.nWeldPass*4;in++)wp.snorm2[in]= -1;
			   for(in=wp.nWeldPass*4;in<(wp.nWeldPass+1+MXNP)*4;in++)wp.snorm1[in]= -1;
			   for(in=wp.nWeldPass*4;in<(wp.nWeldPass+1+MXNP)*4;in++)wp.snorm2[in]= -1;
			   for(in=wp.nWeldPass*bnelt;in<(wp.nWeldPass+1+MXNP)*bnelt;in++)wp.hlightel[in]= -1;// No need for *4 ????

////        wp.firstEle=new long[MAX_WELDPASSES*bnet],
////        wp.nextEle=new long[MAX_WELDPASSES],
////        wp.snorm1=new long[MAX_WELDPASSES*4],
////        wp.snorm2=new long[MAX_WELDPASSES*4],
////        wp.circEles=new long[MAX_WELDPASSES*3],
////        wp.edgeEles=new long[MAX_WELDPASSES*bnelt],
////        wp.edgeNodes=new long[MAX_WELDPASSES*4],
////        wp.sttEleNodes=new long[MAX_WELDPASSES*bnelt*4],
			   for(in=wp.nWeldPass;in<wp.nWeldPass+1+MXNP;in++)wp.source[in]=11; //#heat reflections, which must be odd
// Ref. "Input Instructions and User's Guide for CTSP-Shell," Zhenning Cao, Battelle, Feb. 2005
			   for(in=wp.nWeldPass;in<wp.nWeldPass+1+MXNP;in++)wp.hp[in]=0;//Lump Pass (1) or Moving Arc (0) Heating Procedure
			   for(in=wp.nWeldPass;in<wp.nWeldPass+1+MXNP;in++)wp.nsegs[in]=1;
			   for(in=wp.nWeldPass;in<wp.nWeldPass+1+MXNP;in++)wp.iselect[in]=0;

				   }
   else {
		wp.name=new String[1+MXNP];
		wp.matName=new String[1+MXNP];
		wp.seqNum=new int[1+MXNP];
		wp.boolFlags=new int[1+MXNP];
		wp.type=new int[1+MXNP];
		wp.shape=new int[1+MXNP];
		wp.stepInterval=new int[1+MXNP];
		wp.hp=new int[1+MXNP];
		wp.nsegs=new int[1+MXNP];
		wp.iselect=new int[1+MXNP];
		wp.eles=new long[(1+MXNP)*bnelt];  // Elements in this weld pass
//		wp.sttEles=new long[bnelt*4];  // No need for *4 ????
		wp.sttEles=new long[(1+MXNP)*bnelt];  // No need for *4 ????
//		wp.stpEle=new long[bnelt];  // No need for *bnelt
		wp.stpEle=new long[1+MXNP];  // No need for *bnelt
		wp.firstEle=new long[(1+MXNP)*bnelt];
		wp.nextEle=new long[(1+MXNP)*bnelt];
		wp.snorm1=new long[(1+MXNP)*4];
		wp.snorm2=new long[(1+MXNP)*4];
		wp.circEles=new long[(1+MXNP)*3];
		wp.edgeEles=new long[(1+MXNP)*bnelt];
		wp.edgeNodes=new long[(1+MXNP)*4]; // correct???
		wp.sttEleNodes=new long[(1+MXNP)*bnelt*4];
		wp.curr=new float[1+MXNP];
		wp.volt=new float[1+MXNP];
		wp.eff=new float[1+MXNP];
		wp.speed=new float[1+MXNP];
		wp.troom=new float[1+MXNP];
		wp.tmelt=new float[1+MXNP];
		wp.tcutl=new float[1+MXNP];
		wp.tcuth=new float[1+MXNP];
		wp.timeInterval=new float[1+MXNP];
		wp.maxiHeatStep=new float[1+MXNP];
		wp.miniHeatStep=new float[1+MXNP];
		wp.thk1=new float[1+MXNP];
		wp.thk2=new float[1+MXNP];
		wp.lstart=new double[1+MXNP];
		wp.lend=new double[1+MXNP];
		wp.hlightel=new long[(1+MXNP)*bnelt]; //Convention: 1st col= elem side#
		wp.arrows=new double[(1+MXNP)*NDF*3]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		wp.n_curr_sttEl=new long[1+MXNP];
		wp.reset=new long[1+MXNP];
//		wp.wpTimes=new long[2];
		wp.wpTimes=new float[1+MXNP];
		wp.source=new int[1+MXNP];
		wp.mcr=new int[1+MXNP]; // Not part of original VFT
		wp.WeldColor=new TColor[1+MXNP];
		wp.prevGID=new long[1+MXNP];
//		wp.util_arr=new int[2];
		wp.util_arr=new long[1+MXNP];

			   for(in=0;in<bnelt*(1+MXNP);in++)wp.eles[in]= -1;
			   for(in=0;in<bnelt*(1+MXNP);in++)wp.sttEles[in]= -1;// No need for *4 ????
			   for(in=0;in<bnelt*(1+MXNP);in++)wp.firstEle[in]= -1;// No need for *4 ????
			   for(in=0;in<bnelt*(1+MXNP);in++)wp.nextEle[in]= -1;// No need for *4 ????
			   for(in=0;in<bnelt*(1+MXNP);in++)wp.edgeEles[in]= -1;// No need for *4 ????
			   for(in=0;in<bnelt*4*(1+MXNP);in++)wp.sttEleNodes[in]= -1;// No need for *4 ????
			   for(in=0;in<1+MXNP;in++)wp.stpEle[in]= -1;  // No need for *bnelt
			   for(in=0;in<4*(1+MXNP);in++)wp.snorm1[in]= -1;
			   for(in=0;in<4*(1+MXNP);in++)wp.snorm2[in]= -1;
////        wp.firstEle=new long[bnet],
////        wp.nextEle=new long[1],
////        wp.snorm1=new long[4],
////        wp.snorm2=new long[4],
////        wp.circEles=new long[3],
////        wp.edgeEles=new long[bnelt],
////        wp.edgeNodes=new long[4],
////        wp.sttEleNodes=new long[bnelt*4],
			   for(in=0;in<1+MXNP;in++)wp.source[in]=11;
			   for(in=0;in<1+MXNP;in++)wp.hp[in]=0;//Lump Pass (1) or Moving Arc (0) Heating Procedure
			   for(in=0;in<1+MXNP;in++)wp.nsegs[in]=1;
			   for(in=0;in<1+MXNP;in++)wp.iselect[in]=0;
			   for(in=0;in<bnelt*(1+MXNP);in++)wp.hlightel[in]= -1;// No need for *4 ????
		}
//**************** End from VFTgen

  }
//**************** Start from VFTgen
 else if(isel==9)
  {
		wp.name=new String[wp.nWeldPass+1];
		wp.matName=new String[wp.nWeldPass+1];
		wp.seqNum=new int[wp.nWeldPass+1];
		wp.boolFlags=new int[wp.nWeldPass+1];
		wp.type=new int[wp.nWeldPass+1];
		wp.shape=new int[wp.nWeldPass+1];
		wp.stepInterval=new int[wp.nWeldPass+1];
		wp.hp=new int[wp.nWeldPass+1];
		wp.nsegs=new int[wp.nWeldPass+1];
		wp.iselect=new int[wp.nWeldPass+1];
		wp.eles=new long[(wp.nWeldPass+1)*bnelt];  // Elements in this weld pass
//		sttEles=new long[wp.nWeldPass*bnelt*4];  // No need for *4 ????
		wp.sttEles=new long[(wp.nWeldPass+1)*bnelt];  // No need for *4 ????
//		stpEle=new long[wp.nWeldPass*bnelt];  // No need for *bnelt
		wp.stpEle=new long[wp.nWeldPass+1];  // No need for *bnelt
		wp.firstEle=new long[(wp.nWeldPass+1)*bnelt];
		wp.nextEle=new long[(wp.nWeldPass+1)*bnelt];
		wp.snorm1=new long[(wp.nWeldPass+1)*4];
		wp.snorm2=new long[(wp.nWeldPass+1)*4];
		wp.circEles=new long[(wp.nWeldPass+1)*3];
		wp.edgeEles=new long[(wp.nWeldPass+1)*bnelt];
		wp.edgeNodes=new long[(wp.nWeldPass+1)*4]; // correct???
		wp.sttEleNodes=new long[(wp.nWeldPass+1)*bnelt*4];
		wp.curr=new float[wp.nWeldPass+1];
		wp.volt=new float[wp.nWeldPass+1];
		wp.eff=new float[wp.nWeldPass+1];
		wp.speed=new float[wp.nWeldPass+1];
		wp.troom=new float[wp.nWeldPass+1];
		wp.tmelt=new float[wp.nWeldPass+1];
		wp.tcutl=new float[wp.nWeldPass+1];
		wp.tcuth=new float[wp.nWeldPass+1];
		wp.timeInterval=new float[wp.nWeldPass+1];
		wp.maxiHeatStep=new float[wp.nWeldPass+1];
		wp.miniHeatStep=new float[wp.nWeldPass+1];
		wp.thk1=new float[wp.nWeldPass+1];
		wp.thk2=new float[wp.nWeldPass+1];
		wp.lstart=new double[wp.nWeldPass+1];
		wp.lend=new double[wp.nWeldPass+1];
		wp.hlightel=new long[(wp.nWeldPass+1)*bnelt]; //Convention: 1st col= elem side#
		wp.arrows=new double[3*NDF*(wp.nWeldPass+1)]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		wp.n_curr_sttEl=new long[wp.nWeldPass+1];
		wp.reset=new long[wp.nWeldPass+1];
//		wp.wpTimes=new long[wp.nWeldPass+1];
		wp.wpTimes=new float[wp.nWeldPass+1];
		wp.source=new int[wp.nWeldPass+1];
		wp.mcr=new int[wp.nWeldPass+1]; // Not part of original VFT
		wp.WeldColor=new TColor[wp.nWeldPass+1];
		wp.prevGID=new long[wp.nWeldPass+1];
//		wp.util_arr=new int[wp.nWeldPass+1];
		wp.util_arr=new long[wp.nWeldPass+1];

//			   for(in=(wp.nWeldPass-1)*bnelt;in<wp.nWeldPass*bnelt;in++)wp.eles[in]= -1;
//			   for(in=(wp.nWeldPass-1)*bnelt;in<wp.nWeldPass*bnelt;in++)wp.sttEles[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.eles[in]= -1;
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.sttEles[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.firstEle[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.nextEle[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.edgeEles[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass*bnelt*4;in++)wp.sttEleNodes[in]= -1;// No need for *4 ????
			   for(in=0;in<wp.nWeldPass;in++)wp.stpEle[in]= -1;  // No need for *bnelt
//			   for(in=(wp.nWeldPass-1)*4;in<wp.nWeldPass*4;in++)wp.snorm1[in]= -1;
//			   for(in=(wp.nWeldPass-1)*4;in<wp.nWeldPass*4;in++)wp.snorm2[in]= -1;
			   for(in=0;in<wp.nWeldPass*4;in++)wp.snorm1[in]= -1;
			   for(in=0;in<wp.nWeldPass*4;in++)wp.snorm2[in]= -1;

////        wp.firstEle=new long[MAX_WELDPASSES*bnet],
////        wp.nextEle=new long[MAX_WELDPASSES],
////        wp.snorm1=new long[MAX_WELDPASSES*4],
////        wp.snorm2=new long[MAX_WELDPASSES*4],
////        wp.circEles=new long[MAX_WELDPASSES*3],
////        wp.edgeEles=new long[MAX_WELDPASSES*bnelt],
////        wp.edgeNodes=new long[MAX_WELDPASSES*4],
////        wp.sttEleNodes=new long[MAX_WELDPASSES*bnelt*4],
			   for(in=0;in<wp.nWeldPass;in++)wp.source[in]=11;
			   for(in=0;in<wp.nWeldPass;in++)wp.hp[in]=0;//Lump Pass (1) or Moving Arc (0) Heating Procedure
			   for(in=0;in<wp.nWeldPass;in++)wp.nsegs[in]=1;
			   for(in=0;in<wp.nWeldPass;in++)wp.iselect[in]=0;
			   for(in=0;in<wp.nWeldPass;in++)wp.prevGID[in]=0;
			   for(in=0;in<wp.nWeldPass;in++)wp.util_arr[in]=0;
			   for(in=0;in<wp.nWeldPass*bnelt;in++)wp.hlightel[in]= -1;// No need for *4 ????
  }
 else if(isel==7)
  {
/*
//		wms.name=new char[MAX_MATPROPSET*64];
//		wms.matFileName=new char[MAX_MATPROPSET*64];
//		wms.Steps=new char[MAX_MATPROPSET*64];
		wms.name=new String[MAX_MATPROPSET];
		wms.matFileName=new String[MAX_MATPROPSET];
		wms.Steps=new String[MAX_MATPROPSET];
		wms.cond=new float[MAX_MATPROPSET];
		wms.heat=new float[MAX_MATPROPSET];
		wms.den=new float[MAX_MATPROPSET];
		wms.Ti=new float[MAX_MATPROPSET];
		wms.Ta=new float[MAX_MATPROPSET];
		wms.Tm=new float[MAX_MATPROPSET];
		wms.mcr=new int[MAX_MATPROPSET];
		wms.ved=new int[MAX_MATPROPSET];
		wms.annjd=new int[MAX_MATPROPSET];
		wms.hetjd=new int[MAX_MATPROPSET];
		wms.nprops=new int[MAX_MATPROPSET];
		wms.switc=new bool[MAX_MATPROPSET];
*/
   if(wms.nMatPropSet){
//		SWMTname=new char[wms.nMatPropSet*64];
//		SWMTmatFileName=new char[wms.nMatPropSet*64];
//		SWMTSteps=new char[wms.nMatPropSet*64];
		SWMTname=new String[wms.nMatPropSet];
		SWMTmatFileName=new String[wms.nMatPropSet];
		SWMTSteps=new String[wms.nMatPropSet];
		SWMTcond=new float[wms.nMatPropSet];
		SWMTheat=new float[wms.nMatPropSet];
		SWMTden=new float[wms.nMatPropSet];
		SWMTTi=new float[wms.nMatPropSet];
		SWMTTa=new float[wms.nMatPropSet];
		SWMTTm=new float[wms.nMatPropSet];
		SWMTmcr=new int[wms.nMatPropSet];
		SWMTved=new int[wms.nMatPropSet];
		SWMTannjd=new int[wms.nMatPropSet];
		SWMThetjd=new int[wms.nMatPropSet];
		SWMTnprops=new int[wms.nMatPropSet];
		SWMTswitc=new bool[wms.nMatPropSet];

//		SWMTname=new char[wms.nMatPropSet*64];
//		SWMTmatFileName=new char[wms.nMatPropSet*64];
//		SWMTSteps=new char[wms.nMatPropSet*64];
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTname+in)= *(wms.name+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTmatFileName+in)= *(wms.matFileName+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTSteps+in)= *(wms.Steps+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTcond+in)= *(wms.cond+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTheat+in)= *(wms.heat+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTden+in)= *(wms.den+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTTi+in)= *(wms.Ti+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTTa+in)= *(wms.Ta+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTTm+in)= *(wms.Tm+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTmcr+in)= *(wms.mcr+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTved+in)= *(wms.ved+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTannjd+in)= *(wms.annjd+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMThetjd+in)= *(wms.hetjd+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTnprops+in)= *(wms.nprops+in);
		for(in=0;in<wms.nMatPropSet;in++) *(SWMTswitc+in)= *(wms.switc+in);

//		FDdynmem_manageD(-7,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
		FDdynmem_manage(-7,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);

//		wms.name=new char[MAX_MATPROPSET*64];
//		wms.matFileName=new char[MAX_MATPROPSET*64];
//		wms.Steps=new char[MAX_MATPROPSET*64];
		wms.name=new String[wms.nMatPropSet+1];
		wms.matFileName=new String[wms.nMatPropSet+1];
		wms.Steps=new String[wms.nMatPropSet+1];
		wms.cond=new float[wms.nMatPropSet+1];
		wms.heat=new float[wms.nMatPropSet+1];
		wms.den=new float[wms.nMatPropSet+1];
		wms.Ti=new float[wms.nMatPropSet+1];
		wms.Ta=new float[wms.nMatPropSet+1];
		wms.Tm=new float[wms.nMatPropSet+1];
		wms.mcr=new int[wms.nMatPropSet+1];
		wms.ved=new int[wms.nMatPropSet+1];
		wms.annjd=new int[wms.nMatPropSet+1];
		wms.hetjd=new int[wms.nMatPropSet+1];
		wms.nprops=new int[wms.nMatPropSet+1];
		wms.switc=new bool[wms.nMatPropSet+1];

//		SWMTname=new char[wms.nMatPropSet*64];
//		SWMTmatFileName=new char[wms.nMatPropSet*64];
//		SWMTSteps=new char[wms.nMatPropSet*64];
		for(in=0;in<wms.nMatPropSet;in++) *(wms.name+in)= *(SWMTname+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.matFileName+in)= *(SWMTmatFileName+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.Steps+in)= *(SWMTSteps+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.cond+in)= *(SWMTcond+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.heat+in)= *(SWMTheat+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.den+in)= *(SWMTden+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.Ti+in)= *(SWMTTi+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.Ta+in)= *(SWMTTa+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.Tm+in)= *(SWMTTm+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.mcr+in)= *(SWMTmcr+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.ved+in)= *(SWMTved+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.annjd+in)= *(SWMTannjd+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.hetjd+in)= *(SWMThetjd+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.nprops+in)= *(SWMTnprops+in);
		for(in=0;in<wms.nMatPropSet;in++) *(wms.switc+in)= *(SWMTswitc+in);

		delete [] SWMTname;SWMTname=NULL;
		delete [] SWMTmatFileName;SWMTmatFileName=NULL;
		delete [] SWMTSteps;SWMTSteps=NULL;
		delete [] SWMTcond;SWMTcond=NULL;
		delete [] SWMTheat;SWMTheat=NULL;
		delete [] SWMTden;SWMTden=NULL;
		delete [] SWMTTi;SWMTTi=NULL;
		delete [] SWMTTa;SWMTTa=NULL;
		delete [] SWMTTm;SWMTTm=NULL;
		delete [] SWMTmcr;SWMTmcr=NULL;
		delete [] SWMTved;SWMTved=NULL;
		delete [] SWMTannjd;SWMTannjd=NULL;
		delete [] SWMThetjd;SWMThetjd=NULL;
		delete [] SWMTnprops;SWMTnprops=NULL;
		delete [] SWMTswitc;SWMTswitc=NULL;

					  }
   else {
//		wms.name=new char[MAX_MATPROPSET*64];
//		wms.matFileName=new char[MAX_MATPROPSET*64];
//		wms.Steps=new char[MAX_MATPROPSET*64];
		wms.name=new String[1];
		wms.matFileName=new String[1];
		wms.Steps=new String[1];
		wms.cond=new float[1];
		wms.heat=new float[1];
		wms.den=new float[1];
		wms.Ti=new float[1];
		wms.Ta=new float[1];
		wms.Tm=new float[1];
		wms.mcr=new int[1];
		wms.ved=new int[1];
		wms.annjd=new int[1];
		wms.hetjd=new int[1];
		wms.nprops=new int[1];
		wms.switc=new bool[1];
		}
  }
 else if(isel==10)
  {
		wms.name=new String[wms.nMatPropSet];
		wms.matFileName=new String[wms.nMatPropSet];
		wms.Steps=new String[wms.nMatPropSet];
		wms.cond=new float[wms.nMatPropSet];
		wms.heat=new float[wms.nMatPropSet];
		wms.den=new float[wms.nMatPropSet];
		wms.Ti=new float[wms.nMatPropSet];
		wms.Ta=new float[wms.nMatPropSet];
		wms.Tm=new float[wms.nMatPropSet];
		wms.mcr=new int[wms.nMatPropSet];
		wms.ved=new int[wms.nMatPropSet];
		wms.annjd=new int[wms.nMatPropSet];
		wms.hetjd=new int[wms.nMatPropSet];
		wms.nprops=new int[wms.nMatPropSet];
		wms.switc=new bool[wms.nMatPropSet];
  }
 else if(isel==8)
  {
   if(wps.nWeldParamSet){
	   SWPTname=new String[wps.nWeldParamSet];
	   SWPTcurr=new float[wps.nWeldParamSet];
	   SWPTvolt=new float[wps.nWeldParamSet];
	   SWPTeff=new float[wps.nWeldParamSet];
	   SWPTspeed=new float[wps.nWeldParamSet];

//for(in=0;in<wms.nMatPropSet;in++) *(SWMTname+in)= *(wms.name+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(SWPTname+in)= *(wps.name+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(SWPTcurr+in)= *(wps.curr+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(SWPTvolt+in)= *(wps.volt+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(SWPTeff+in)= *(wps.eff+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(SWPTspeed+in)= *(wps.speed+in);

//	   FDdynmem_manageD(-8,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	   FDdynmem_manage(-8,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);

	   wps.name=new String[wps.nWeldParamSet+1];
	   wps.curr=new float[wps.nWeldParamSet+1];
	   wps.volt=new float[wps.nWeldParamSet+1];
	   wps.eff=new float[wps.nWeldParamSet+1];
	   wps.speed=new float[wps.nWeldParamSet+1];

	   for(in=0;in<wps.nWeldParamSet;in++) *(wps.name+in)= *(SWPTname+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(wps.curr+in)= *(SWPTcurr+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(wps.volt+in)= *(SWPTvolt+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(wps.eff+in)= *(SWPTeff+in);
	   for(in=0;in<wps.nWeldParamSet;in++) *(wps.speed+in)= *(SWPTspeed+in);

	   delete [] SWPTname;SWPTname=NULL;
	   delete [] SWPTcurr;SWPTcurr=NULL;
	   delete [] SWPTvolt;SWPTvolt=NULL;
	   delete [] SWPTeff;SWPTeff=NULL;
	   delete [] SWPTspeed;SWPTspeed=NULL;
						}
   else {
	   wps.name=new String[1];
	   wps.curr=new float[1];
	   wps.volt=new float[1];
	   wps.eff=new float[1];
	   wps.speed=new float[1];
		}
  }
 else if(isel==11)
  {
	   wps.name=new String[wps.nWeldParamSet];
	   wps.curr=new float[wps.nWeldParamSet];
	   wps.volt=new float[wps.nWeldParamSet];
	   wps.eff=new float[wps.nWeldParamSet];
	   wps.speed=new float[wps.nWeldParamSet];
  }
// else if(isel==13){base.groupsname=new String[wp.nWeldGroup+1];} //Return to pragmatic storage EFP 3/12/2012
 else if(isel==13){base.groupsname=new String[MXNP];} //Return to pragmatic storage EFP 3/12/2012
 else if(isel==15){//EFP 8/07/2011 & 2/08/2012
				   base.arrELSET=new long[MXNP];//Warning: highest number of ELSETs=unlimited
				  } //EFP 8/07/2011 & 2/08/2012
 else if(isel==17){//EFP 8/25/2013
				   indat.arrELSET=new long[MXNP];//Warning: highest number of ELSETs=unlimited
				  }
 else if(isel==20){base.ELSETinputnames=new String[MXNP];//ELSET names in input deck (including basemetal ALLEL even if absent)
				  } //EFP 01/01/2015

//**************** End from VFTgen
//
 else if(isel== -1)
  {if(base.nop1)
	 {delete [] base.nop1;delete [] base.matno;delete [] base.c1;
	  delete [] base.el_map;delete [] base.node_map;delete [] base.nlp;
	  delete [] base.beta;delete [] base.orig_matno; // SuperEl
	  base.nop1=NULL;base.matno=NULL;base.c1=NULL;base.el_map=NULL;base.node_map=NULL;
	  base.nlp=NULL;base.beta=NULL;base.orig_matno=NULL;
// SuperEl
//	delete [] base.lbce;delete [] base.lbcn;
//	base.lbce=NULL;base.lbcn=NULL;
	  if(base.nofix){delete [] base.nofix;base.nofix=NULL;delete [] base.nrfix;base.nrfix=NULL;delete [] base.presc;base.presc=NULL;}
	  if(base.ncoorf){delete [] base.cangl;base.cangl=NULL;}
	  if(base.nldel){delete [] base.nldel;base.nldel=NULL;delete [] base.press;base.press=NULL;}
	  if(base.lodpt){delete [] base.lodpt;base.lodpt=NULL;delete [] base.pointld;base.pointld=NULL;}
	  if(base.ebody){delete [] base.ebody;base.ebody=NULL;delete [] base.body;base.body=NULL;}
	  if(base.matyp){delete [] base.matyp;base.matyp=NULL;delete [] base.prop;base.prop=NULL;}
	 }
  }
 else if(isel== -2)
  {if(indat.nop1)
	 {delete [] indat.nop1;delete [] indat.matno;delete [] indat.c1;delete [] indat.bc1;
	  if(indat.fsect)delete [] indat.fsect;

	  delete [] indat.nlp;delete [] indat.beta; // SuperEl
	  indat.nop1=NULL;indat.matno=NULL;indat.c1=NULL;indat.bc1=NULL;indat.fsect=NULL;indat.nlp=NULL;indat.beta=NULL;
// SuperEl
//   ,nofix[2*MXIMEM],nldel[2*MXIMEM],lbce[2*MXIMEM],lbcn[2*MXIMEM]
	 }
  }
 else if(isel== -3){if(base.res1){delete [] base.res1;base.res1=NULL;}}
 else if(isel== -4){if(indat.res1){delete [] indat.res1;indat.res1=NULL;}
					if(indat.result){delete [] indat.result;indat.result=NULL;}
				   }
/*
				   // Start temporary disconnect VFTmisc coding
 else if(isel== -6){delete [] wp.seqNum;wp.seqNum=NULL;
					delete [] wp.boolFlags;wp.boolFlags=NULL;
					delete [] wp.type;wp.type=NULL;
					delete [] wp.shape;wp.shape=NULL;
					delete [] wp.stepInterval;wp.stepInterval=NULL;
					delete [] wp.hp;wp.hp=NULL;
					delete [] wp.nsegs;wp.nsegs=NULL;
					delete [] wp.iselect;wp.iselect=NULL;
					delete [] wp.eles;wp.eles=NULL;
					delete [] wp.sttEles;wp.sttEles=NULL;
					delete [] wp.stpEle;wp.stpEle=NULL;
					delete [] wp.firstEle;wp.firstEle=NULL;
					delete [] wp.nextEle;wp.nextEle=NULL;
					delete [] wp.snorm1;wp.snorm1=NULL;
					delete [] wp.snorm2;wp.snorm2=NULL;
					delete [] wp.circEles;wp.circEles=NULL;
					delete [] wp.edgeEles;wp.edgeEles=NULL;
					delete [] wp.edgeNodes;wp.edgeNodes=NULL;
					delete [] wp.sttEleNodes;wp.sttEleNodes=NULL;
					delete [] wp.curr;wp.curr=NULL;
					delete [] wp.volt;wp.volt=NULL;
					delete [] wp.eff;wp.eff=NULL;
					delete [] wp.speed;wp.speed=NULL;
					delete [] wp.troom;wp.troom=NULL;
					delete [] wp.tmelt;wp.tmelt=NULL;
					delete [] wp.tcutl;wp.tcutl=NULL;
					delete [] wp.tcuth;wp.tcuth=NULL;
					delete [] wp.timeInterval;wp.timeInterval=NULL;
					delete [] wp.maxiHeatStep;wp.maxiHeatStep=NULL;
					delete [] wp.miniHeatStep;wp.miniHeatStep=NULL;
					delete [] wp.thk1;wp.thk1=NULL;
					delete [] wp.thk2;wp.thk2=NULL;
					delete [] wp.lstart;wp.lstart=NULL;
					delete [] wp.lend;wp.lend=NULL;
					delete [] wp.hlightel;wp.hlightel=NULL;
					delete [] wp.arrows;wp.arrows=NULL;
					delete [] wp.n_curr_sttEl;wp.n_curr_sttEl=NULL;
					delete [] wp.reset;wp.reset=NULL;
					delete [] wp.wpTimes;wp.wpTimes=NULL;
					delete [] wp.source;wp.source=NULL;

//                    delete [] wp.edgeFlag;wp.edgeFlag=NULL;
//                    delete [] wp.circFlag;wp.circFlag=NULL;
//                    delete [] wp.merge;wp.merge=NULL;
				   }
				   //End temporary disconnect VFTmisc coding
*/
//**************** Start from VFTgen
 else if(isel== -6 || isel== -9){
					delete [] wp.name;wp.name=NULL;
					delete [] wp.matName;wp.matName=NULL;
					delete [] wp.seqNum;wp.seqNum=NULL;
					delete [] wp.boolFlags;wp.boolFlags=NULL;
					delete [] wp.type;wp.type=NULL;
					delete [] wp.shape;wp.shape=NULL;
					delete [] wp.stepInterval;wp.stepInterval=NULL;
					delete [] wp.hp;wp.hp=NULL;
					delete [] wp.nsegs;wp.nsegs=NULL;
					delete [] wp.iselect;wp.iselect=NULL;
					delete [] wp.eles;wp.eles=NULL;
					delete [] wp.sttEles;wp.sttEles=NULL;
					delete [] wp.stpEle;wp.stpEle=NULL;
					delete [] wp.firstEle;wp.firstEle=NULL;
					delete [] wp.nextEle;wp.nextEle=NULL;
					delete [] wp.snorm1;wp.snorm1=NULL;
					delete [] wp.snorm2;wp.snorm2=NULL;
					delete [] wp.circEles;wp.circEles=NULL;
					delete [] wp.edgeEles;wp.edgeEles=NULL;
					delete [] wp.edgeNodes;wp.edgeNodes=NULL;
					delete [] wp.sttEleNodes;wp.sttEleNodes=NULL;
					delete [] wp.curr;wp.curr=NULL;
					delete [] wp.volt;wp.volt=NULL;
					delete [] wp.eff;wp.eff=NULL;
					delete [] wp.speed;wp.speed=NULL;
					delete [] wp.troom;wp.troom=NULL;
					delete [] wp.tmelt;wp.tmelt=NULL;
					delete [] wp.tcutl;wp.tcutl=NULL;
					delete [] wp.tcuth;wp.tcuth=NULL;
					delete [] wp.timeInterval;wp.timeInterval=NULL;
					delete [] wp.maxiHeatStep;wp.maxiHeatStep=NULL;
					delete [] wp.miniHeatStep;wp.miniHeatStep=NULL;
					delete [] wp.thk1;wp.thk1=NULL;
					delete [] wp.thk2;wp.thk2=NULL;
					delete [] wp.lstart;wp.lstart=NULL;
					delete [] wp.lend;wp.lend=NULL;
					delete [] wp.hlightel;wp.hlightel=NULL;
					delete [] wp.arrows;wp.arrows=NULL;
					delete [] wp.n_curr_sttEl;wp.n_curr_sttEl=NULL;
					delete [] wp.reset;wp.reset=NULL;
					delete [] wp.wpTimes;wp.wpTimes=NULL;
					delete [] wp.source;wp.source=NULL;
					delete [] wp.mcr;wp.mcr=NULL;// Not part of original VFT
					delete [] wp.WeldColor;wp.WeldColor=NULL;
					delete [] wp.prevGID;wp.prevGID=NULL;
					delete [] wp.util_arr;wp.util_arr=NULL;

//		delete [] wms.name;wms.name=NULL;
//		delete [] wms.matFileName;wms.matFileName=NULL;
//		delete [] wms.Steps;wms.Steps=NULL;
//		delete [] wms.cond;wms.cond=NULL;
//		delete [] wms.heat;wms.heat=NULL;
//		delete [] wms.den;wms.den=NULL;
//		delete [] wms.Ti;wms.Ti=NULL;
//		delete [] wms.Ta;wms.Ta=NULL;
//		delete [] wms.Tm;wms.Tm=NULL;
//		delete [] wms.mcr;wms.mcr=NULL;
//		delete [] wms.ved;wms.ved=NULL;
//		delete [] wms.annjd;wms.annjd=NULL;
//		delete [] wms.hetjd;wms.hetjd=NULL;
//		delete [] wms.nprops;wms.nprops=NULL;
//		delete [] wms.switc;wms.switc=NULL;
//	   delete [] wps.name;wps.name=NULL;
//	   delete [] wps.curr;wps.curr=NULL;
//	   delete [] wps.volt;wps.volt=NULL;
//	   delete [] wps.eff;wps.eff=NULL;
//	   delete [] wps.speed;wps.speed=NULL;
//                    delete [] wp.edgeFlag;wp.edgeFlag=NULL;
//                    delete [] wp.circFlag;wp.circFlag=NULL;
//                    delete [] wp.merge;wp.merge=NULL;
				   }
 else if(isel== -7 || isel== -10){
		delete [] wms.name;wms.name=NULL;
		delete [] wms.matFileName;wms.matFileName=NULL;
		delete [] wms.Steps;wms.Steps=NULL;
		delete [] wms.cond;wms.cond=NULL;
		delete [] wms.heat;wms.heat=NULL;
		delete [] wms.den;wms.den=NULL;
		delete [] wms.Ti;wms.Ti=NULL;
		delete [] wms.Ta;wms.Ta=NULL;
		delete [] wms.Tm;wms.Tm=NULL;
		delete [] wms.mcr;wms.mcr=NULL;
		delete [] wms.ved;wms.ved=NULL;
		delete [] wms.annjd;wms.annjd=NULL;
		delete [] wms.hetjd;wms.hetjd=NULL;
		delete [] wms.nprops;wms.nprops=NULL;
		delete [] wms.switc;wms.switc=NULL;
								 }
 else if(isel== -8 || isel== -11){
	   delete [] wps.name;wps.name=NULL;
	   delete [] wps.curr;wps.curr=NULL;
	   delete [] wps.volt;wps.volt=NULL;
	   delete [] wps.eff;wps.eff=NULL;
	   delete [] wps.speed;wps.speed=NULL;
								 }
 else if(isel== -13){if(base.groupsname){delete [] base.groupsname;base.groupsname=NULL;}} //EFP 3/15/2011
 else if(isel== -15){if(base.arrELSET){delete [] base.arrELSET;base.arrELSET=NULL;}
					}
 else if(isel== -17){if(indat.arrELSET){delete [] indat.arrELSET;indat.arrELSET=NULL;}
					}
 else if(isel== -20){if(base.ELSETinputnames){delete [] base.ELSETinputnames;base.ELSETinputnames=NULL;}
				  } //EFP 01/01/2015
//**************** End from VFTgen
 else {honk<<isel<<" Unsupported dyn.memory option\n";extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported dyn.memory option",L"Exit",MB_OK);exit(0);}
}
//---------------------------------------------------------------------------
void TForm1::axis_plot_init()
{int ic=0;ap.isw=0; // Select xyz or 123 (not used yet)
 ic=0;ap.x[ic]=ap.y[ic]=ap.z[ic]=0.f;
 ic=1;ap.x[ic]=float(min(ClientWidth,ClientHeight))/16.f;ap.y[ic]=ap.z[ic]=0.f;
 ic=2;ap.x[ic]=0.f;ap.y[ic]= float(min(ClientWidth,ClientHeight))/16.f;ap.z[ic]=0.f;
 ic=3;ap.x[ic]=ap.y[ic]=0.f;ap.z[ic]= float(min(ClientWidth,ClientHeight))/16.f;
 ic=4;ap.x[ic]=(float(min(ClientWidth,ClientHeight)) +float(min(ClientWidth,ClientHeight))/3.f)/16.f;ap.y[ic]=ap.z[ic]=0.f;
 ic=5;ap.x[ic]=0.f;ap.y[ic]= (float(min(ClientWidth,ClientHeight)) +float(min(ClientWidth,ClientHeight))/3.f)/16.f;ap.z[ic]=0.f;
 ic=6;ap.x[ic]=ap.y[ic]=0.f;ap.z[ic]= (float(min(ClientWidth,ClientHeight)) +float(min(ClientWidth,ClientHeight))/3.f)/16.f;

// for(ic=0;ic<7;ic++)honk<<ic<<" "<<ap.x[ic]<<" "<<ap.y[ic]<<" "<<ap.z[ic]<<" initAX\n";
}
//---------------------------------------------------------------------------
void TForm1::axis_plotXYZ(int CWidth,int CHeight)
// Simple axis plot    EFP 9/17/2010
{int xloc=CWidth/16,yloc=CHeight/16;

//honk<<iBackgroundColor<<" "<<xloc<<" "<<yloc<<" "<<CWidth<<" "<<CHeight<<" iB\n";
 if     (iBackroundColor==1)tBitmap->Canvas->Pen->Color=clBlack;
 else if(iBackroundColor==2)tBitmap->Canvas->Pen->Color=clWhite;
 else if(iBackroundColor==3)tBitmap->Canvas->Pen->Color=clWhite;
 else if(iBackroundColor==4)tBitmap->Canvas->Pen->Color=clWhite;
 else                       tBitmap->Canvas->Pen->Color=clWhite;
 tBitmap->Canvas->Pen->Width=1;//TBD: Shift up by a small amount because Z gets lost for ZX_rot() EFP 3/22/2012
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[1] +0.5f),CHeight-yloc- int(ap.y[1]+0.5f));

// honk<<(xloc+int(ap.x[1] +0.5f))<<" "<<(CHeight-yloc- int(ap.y[1]+0.5f))<<" a\n";
 tBitmap->Canvas->LineTo(xloc+int(ap.x[0] +0.5f),CHeight-yloc- int(ap.y[0]+0.5f));

// honk<<(xloc+int(ap.x[0] +0.5f))<<" "<<(CHeight-yloc- int(ap.y[0]+0.5f))<<" a\n";
 tBitmap->Canvas->LineTo(xloc+int(ap.x[2] +0.5f),CHeight-yloc- int(ap.y[2]+0.5f));

// honk<<(xloc+int(ap.x[2] +0.5f))<<" "<<(CHeight-yloc- int(ap.y[2]+0.5f))<<" a\n";
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[0] +0.5f),CHeight-yloc- int(ap.y[0]+0.5f));
 tBitmap->Canvas->LineTo(xloc+int(ap.x[3] +0.5f),CHeight-yloc- int(ap.y[3]+0.5f));
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[4] +0.5f)-4,CHeight-yloc- int(ap.y[4]+0.5f)-4);//Character x
 tBitmap->Canvas->LineTo(xloc+int(ap.x[4] +0.5f)+4,CHeight-yloc- int(ap.y[4]+0.5f)+4);
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[4] +0.5f)-4,CHeight-yloc- int(ap.y[4]+0.5f)+4);
 tBitmap->Canvas->LineTo(xloc+int(ap.x[4] +0.5f)+4,CHeight-yloc- int(ap.y[4]+0.5f)-4);
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[5] +0.5f)-4,CHeight-yloc- int(ap.y[5]+0.5f)-4);//Character y
 tBitmap->Canvas->LineTo(xloc+int(ap.x[5] +0.5f),CHeight-yloc- int(ap.y[5]+0.5f));
 tBitmap->Canvas->LineTo(xloc+int(ap.x[5] +0.5f)+4,CHeight-yloc- int(ap.y[5]+0.5f)-4);
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[5] +0.5f),CHeight-yloc- int(ap.y[5]+0.5f));
 tBitmap->Canvas->LineTo(xloc+int(ap.x[5] +0.5f),CHeight-yloc- int(ap.y[5]+0.5f)+4);
 tBitmap->Canvas->MoveTo(xloc+int(ap.x[6] +0.5f)-4,CHeight-yloc- int(ap.y[6]+0.5f)-4);//Character z
 tBitmap->Canvas->LineTo(xloc+int(ap.x[6] +0.5f)+4,CHeight-yloc- int(ap.y[6]+0.5f)-4);
 tBitmap->Canvas->LineTo(xloc+int(ap.x[6] +0.5f)-4,CHeight-yloc- int(ap.y[6]+0.5f)+4);
 tBitmap->Canvas->LineTo(xloc+int(ap.x[6] +0.5f)+4,CHeight-yloc- int(ap.y[6]+0.5f)+4);
}
//---------------------------------------------------------------------------
void TForm1::FDpmargin(long indat_npoin,long indat_nelt,long indat_nop1[],long indat_matno[],float indat_c1[],float trans_op[])
// Global NDF,MXNPEI,etc
{float xdiff=0.f,ydiff=0.f,gdiff=1.E38f,xmax=0.f,ymax=0.f,zmax=0.f,xmin=0.f,ymin=0.f,zmin=0.f;
 long in=0,i=0,eltype=0,bscode=0,node=0,tf3=1000,tf5=100000,tf7=10000000;
 xmax= -1.E28f;ymax= -1.E28f;zmax= -1.E28f;xmin=  1.E28f;ymin=  1.E28f;zmin=  1.E28f;
 for(i=0;i<indat_nelt;i++){
	eltype=indat_matno[i]/tf7;bscode=(indat_matno[i]-eltype*tf7)/tf5;node=(indat_matno[i]-eltype*tf7-bscode*tf5)/tf3;
	for(in=0;in<node;in++){
	   if(xmin>indat_c1[NDF*indat_nop1[MXNPEI*i+in]  ])xmin=indat_c1[NDF*indat_nop1[MXNPEI*i+in]  ];
	   if(ymin>indat_c1[NDF*indat_nop1[MXNPEI*i+in]+1])ymin=indat_c1[NDF*indat_nop1[MXNPEI*i+in]+1];
	   if(zmin>indat_c1[NDF*indat_nop1[MXNPEI*i+in]+2])zmin=indat_c1[NDF*indat_nop1[MXNPEI*i+in]+2];
	   if(xmax<indat_c1[NDF*indat_nop1[MXNPEI*i+in]  ])xmax=indat_c1[NDF*indat_nop1[MXNPEI*i+in]  ];
	   if(ymax<indat_c1[NDF*indat_nop1[MXNPEI*i+in]+1])ymax=indat_c1[NDF*indat_nop1[MXNPEI*i+in]+1];
	   if(zmax<indat_c1[NDF*indat_nop1[MXNPEI*i+in]+2])zmax=indat_c1[NDF*indat_nop1[MXNPEI*i+in]+2];
					  }
	}
//
 xdiff=float(ClientWidth)/(xmax-xmin);ydiff=float(ClientHeight)/(ymax-ymin);
 if(gdiff>xdiff)gdiff=xdiff;if(gdiff>ydiff)gdiff=ydiff;gdiff=0.95f*gdiff;
glGdiff=glGdiff*gdiff;
 for(i=0;i<indat_npoin;i++)
	{indat_c1[NDF*i  ]=(indat_c1[NDF*i  ]-0.5f*(xmax+xmin))*gdiff +0.5f*(float)ClientWidth;
	 indat_c1[NDF*i+1]=(indat_c1[NDF*i+1]-0.5f*(ymax+ymin))*gdiff +0.5f*(float)ClientHeight;
	 indat_c1[NDF*i+2]=(indat_c1[NDF*i+2]-0.5f*(zmax+zmin))*gdiff;
	}
 trans_op[0]=0.5f*(float)ClientWidth;
 trans_op[1]=0.5f*(float)ClientHeight;
 trans_op[2]=0.f;
 gl_indatMedian[0]=0.5f*(float)ClientWidth;gl_indatMedian[1]=0.5f*(float)ClientHeight;gl_indatMedian[2]=0.f;
}
//---------------------------------------------------------------------------
void TForm1::FDpscale(long indat_npoin,float indat_c1[],float prod_op[][3])
// Global NDF,etc
{long i=0;float gdiff=0.f,hscale=0.f,vscale=0.f;
 hscale=float(ClientWidth)/float(cxWidth0);
 vscale=float(ClientHeight)/float(cyHeight0);
 if(hscale>vscale)gdiff=vscale;else gdiff=hscale;
glGdiff=glGdiff*gdiff;
 for(i=0;i<NDF*indat_npoin;i++)indat_c1[i]=indat_c1[i]*gdiff;
 for(int k=0;k<NDF;k++){for(int j=0;j<NDF;j++)prod_op[k][j]=prod_op[k][j]/gdiff;}
//gl_indatMedian UNCHANGED
}
//---------------------------------------------------------------------------
void TForm1::FDwireplot(long indat_nelt,long indat_nop1[],long indat_matno[],float indat_c1[]) //tBitmap->
// Global MAXPOINTS,NDF,MXNPEI,etc
{long ie=0,eltype=0,t7=10000000;TPoint ptDraw[MAXPOINTS];
 tBitmap->Canvas->Pen->Width=1;
 for(ie=0;ie<indat_nelt;ie++)
   {eltype=indat_matno[ie]/t7;
	if(eltype==5)
	  {for(int jp=0;jp<3;jp++){ptDraw[jp].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]  ]+0.5f);ptDraw[jp].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]+1]+0.5f);}
	   ptDraw[3].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]]+0.5f);ptDraw[3].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]+1]+0.5f);
	   ptDraw[4].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]]+0.5f);ptDraw[4].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]+1]+0.5f);
	   ptDraw[5].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]]+0.5f);ptDraw[5].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]+1]+0.5f);
	   ptDraw[6].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]]+0.5f);ptDraw[6].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]+1]+0.5f);
	   ptDraw[7].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]]+0.5f);ptDraw[7].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]+1]+0.5f);
	   tBitmap->Canvas->Polyline(ptDraw,7);
      }
	else if(eltype==7)
	  {for(int jp=0;jp<3;jp++){ptDraw[jp].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]  ]+0.5f);ptDraw[jp].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]+1]+0.5f);}
	   ptDraw[ 3].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]]+0.5f);ptDraw[ 3].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]+1]+0.5f);
	   ptDraw[ 4].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]]+0.5f);ptDraw[ 4].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]+1]+0.5f);
	   ptDraw[ 5].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]]+0.5f);ptDraw[ 5].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]+1]+0.5f);
	   ptDraw[ 6].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]]+0.5f);ptDraw[ 6].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]+1]+0.5f);
	   ptDraw[ 7].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]]+0.5f);ptDraw[ 7].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]+1]+0.5f);
	   ptDraw[ 8].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]]+0.5f);ptDraw[ 8].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]+1]+0.5f);
	   ptDraw[ 9].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]]+0.5f);ptDraw[ 9].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]+1]+0.5f);
	   ptDraw[10].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]]+0.5f);ptDraw[10].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]+1]+0.5f);
	   ptDraw[11].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]]+0.5f);ptDraw[11].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]+1]+0.5f);
	   tBitmap->Canvas->Polyline(ptDraw,11);
      }
    else
	  {for(int jp=0;jp<4;jp++){ptDraw[jp].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]  ]+0.5f);ptDraw[jp].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+jp]+1]+0.5f);}
	   ptDraw[ 4].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]]+0.5f);ptDraw[ 4].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+0]+1]+0.5f);
	   ptDraw[ 5].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]]+0.5f);ptDraw[ 5].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]+1]+0.5f);
	   ptDraw[ 6].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]]+0.5f);ptDraw[ 6].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]+1]+0.5f);
	   ptDraw[ 7].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]]+0.5f);ptDraw[ 7].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+1]+1]+0.5f);
	   ptDraw[ 8].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]]+0.5f);ptDraw[ 8].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+5]+1]+0.5f);
	   ptDraw[ 9].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+6]]+0.5f);ptDraw[ 9].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+6]+1]+0.5f);
	   ptDraw[10].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]]+0.5f);ptDraw[10].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+2]+1]+0.5f);
	   ptDraw[11].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+6]]+0.5f);ptDraw[11].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+6]+1]+0.5f);
	   ptDraw[12].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+7]]+0.5f);ptDraw[12].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+7]+1]+0.5f);
	   ptDraw[13].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]]+0.5f);ptDraw[13].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+3]+1]+0.5f);
	   ptDraw[14].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+7]]+0.5f);ptDraw[14].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+7]+1]+0.5f);
	   ptDraw[15].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]]+0.5f);ptDraw[15].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+4]+1]+0.5f);
	   tBitmap->Canvas->Polyline(ptDraw,15);
      }
	}
}
//---------------------------------------------------------------------------
void TForm1::FDpaintplot(int isel,long indat_nop1[],long indat_matno[],float indat_c1[],long nFace,long arbFace[])
// Global NDF,MXNPEI,MAXPOINTS,MAX_GID
// 4&6&8-n elements only; isel=0 Canvas & isel=1 tBitmap->Canvas
//{int gdata8[24]={0,1,5,4,
//				 1,2,6,5,
//				 3,2,6,7,
//				 0,3,7,4,
//				 0,1,2,3,
//				 4,5,6,7};
{int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
//  These are IDEAS conventions (NOT CAP)
//	  .      JOINT6/1, 2, 3, 1, 4, 0,
//	  .             2, 3, 1, 3, 5, 0,
//	  .             5, 6, 4, 2, 6, 0,
//	  .             4, 5, 6, 0, 0, 0/
 int gdata6[20]={0,1,4,3,
				 1,2,5,4,
				 2,0,3,5,
				 0,2,1,0,
				 3,4,5,0};
 int gdata4[12]={0,2,1,
				 1,2,3,
				 2,0,3,
				 3,0,1};
 int npts=0;long ic=0,ie=0,fType=0,iside=0,ip=0,ir=0,eltype=0,ieGID=0,t3=1000,t7=10000000;TPoint ptDraw[MAXPOINTS];

 for(ic=0;ic<nFace;ic++)
   {ie=arbFace[ic]/t3;fType=(arbFace[ic]-ie*t3)/10;iside=arbFace[ic]-ie*t3-fType*10;eltype=indat_matno[ie]/t7;
ieGID=indat.arrELSET[ie];

	  if(isel){

			   if(ieGID==0 || ieGID==MAX_GID-1)tBitmap->Canvas->Pen->Width=1;
			   else tBitmap->Canvas->Pen->Width=2;


if(ieGID==0 || ieGID==MAX_GID-1)tBitmap->Canvas->Pen->Color=clLtGray;
else if(ieGID<base.allGrp)
	   {
		if(wp.PAINT_SEL){
		  ir=ieGID-8*(ieGID/8);
		  if     (ir==0)tBitmap->Canvas->Pen->Color=clLtGray;else if(ir==1)tBitmap->Canvas->Pen->Color=clBlue;
		  else if(ir==2)tBitmap->Canvas->Pen->Color=clGreen ;else if(ir==3)tBitmap->Canvas->Pen->Color=clLime;
		  else if(ir==4)tBitmap->Canvas->Pen->Color=clNavy  ;else if(ir==5)tBitmap->Canvas->Pen->Color=clYellow;
		  else if(ir==6)tBitmap->Canvas->Pen->Color=clOlive ;else tBitmap->Canvas->Pen->Color=clMaroon;
						}
		else tBitmap->Canvas->Pen->Color=clRed;
	   }
else {
	  tBitmap->Canvas->Pen->Color=wp.WeldColor[ieGID-base.allGrp];
	 }
			  }
	  else    {
			   if(ieGID==0 || ieGID==MAX_GID-1)Canvas->Pen->Width=1;
			   else Canvas->Pen->Width=2;

	if(ieGID==MAX_GID-1)Canvas->Pen->Color=clLtGray;else Canvas->Pen->Color=clWhite;
	if(fType==1)Canvas->Brush->Color=clLtGray;
	else {
if(ieGID==0 || ieGID==MAX_GID-1)Canvas->Pen->Color=clLtGray;
else if(ieGID<base.allGrp)
	   {
		if(wp.PAINT_SEL){
		  ir=ieGID-8*(ieGID/8);
		  if     (ir==0)Canvas->Pen->Color=clLtGray;else if(ir==1)Canvas->Pen->Color=clBlue;
		  else if(ir==2)Canvas->Pen->Color=clGreen ;else if(ir==3)Canvas->Pen->Color=clLime;
		  else if(ir==4)Canvas->Pen->Color=clNavy  ;else if(ir==5)Canvas->Pen->Color=clYellow;
		  else if(ir==6)Canvas->Pen->Color=clOlive ;else Canvas->Pen->Color=clMaroon;
						}
		else Canvas->Pen->Color=clRed;
	   }
else {
//	  Canvas->Pen->Color=wp.WeldColor[ieGID-wp.nWeldGroup-1];
	  Canvas->Pen->Color=wp.WeldColor[ieGID-base.allGrp];
	 }
		 }
			  }

	if(eltype==5)
	  {for(ip=0;ip<3;ip++){ptDraw[ip].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+ip]]]+0.5f);
						   ptDraw[ip].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+ip]]+1]+0.5f);
						  }
	   if(ieGID==MAX_GID-1)tBitmap->Canvas->Polyline(ptDraw,2);else tBitmap->Canvas->Polygon(ptDraw,2);
	  }
	else if(eltype==7)
	  {if(iside>2)npts=3;else npts=4;
	   for(ip=0;ip<npts;ip++){ptDraw[ip].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+ip]]]+0.5f);
							  ptDraw[ip].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+ip]]+1]+0.5f);
							 }
	   if(ieGID==MAX_GID-1)tBitmap->Canvas->Polyline(ptDraw,npts-1);else tBitmap->Canvas->Polygon(ptDraw,npts-1);
	  }
	else
	  {for(ip=0;ip<4;ip++){ptDraw[ip].x=int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+ip]]]+0.5f);
						   ptDraw[ip].y=ClientHeight-int(indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+ip]]+1]+0.5f);
						  }
	   if(ieGID==MAX_GID-1)tBitmap->Canvas->Polyline(ptDraw,3);else tBitmap->Canvas->Polygon(ptDraw,3);
	  }
   }
 tBitmap->Canvas->Pen->Color=clBlack;
}
//---------------------------------------------------------------------------
void TForm1::FDcomp_nGID(long indat_nelt,int *nGID,long arGID[])
// Global MAX_GID
{long i=0,ieGID=0;
 for(i=0;i<MAX_GID;i++)arGID[i]=0;
// for(i=0;i<indat_nelt;i++)arGID[i]=0;// NOT for use in shell analysis
 for(i=0;i<indat_nelt;i++)
	{ieGID=indat.arrELSET[i];
//	 if(ieGID<0 || ieGID>MAX_GID){
//								 }

//honk<<(i+1)<<" "<<ieGID<<" FDcomp_nGID\n";
	 arGID[ieGID]=1;
	}
 *nGID=0;for(i=0;i<MAX_GID;i++)if(arGID[i]>0) *nGID= *nGID+1;
}
//---------------------------------------------------------------------------
void TForm1::FDelemfacets3(long npoin,long nelt,long nop1[],long matno[],long arGID[])
// Version using huge memory for speed (Code allows for quadratic elements)  Global LONG_INT,MXNPEI,MAX_GID,nGIDmax
// Corrcted 2008/08/15 for several erroneous "isw=1;break;".
// Version with log search throughout
{int isw=0,side_arr8[26]={16,0,0,0,0,16,1,1,0,0,8,2,2,8,0,0,4,4,0,0,32,0,0,0,0,32},
	 side_arr6[10]={8,1,4,2,0,0,0,0,0,16},side_arr4[4]={1,8,4,2};
 long ie=0,eltype=0,bscode=0,
// node=0,
 ieGID=0,t3=1000,t5=100000,t7=10000000,
	  ip=0,in=0,iv=0,ivp=0,nside=0,in0=0,in0p=0,in0pp=0,inx=0,inxp=0,inxpp=0,oppn0=0,oppn1=0,oppn3=0,prod=0,
	  iex=0,eltypex=0,bscodex=0,nodex=0,ieGIDx=0,mxnelvertex=0,accum=0,
	  lows=0,mids=0,tops=0,istx=0,lowsp=0,midsp=0,topsp=0,istxp=0,corruptf[4],*arr1=NULL,*arr3=NULL;
// Note: matno[i]=8*10000000+nodes*1000+ipid -1; Also bscodex=1+2+4+8+16+32......
// corruptf[0]=counter for corrupt hex-wedge, [1] wedge-wedge, [2] tetra-wedge, [3] unsupported elements
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//try {
//
 arr1=new long[npoin+1];for(ip=0;ip<npoin+1;ip++) *(arr1+ip)=0;
 for(ip=0;ip<4;ip++)corruptf[ip]=0; //EFP 8/01/2014
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
 //node=(matno[ie]-eltype*t7-bscode*t5)/t3;
ieGID=indat.arrELSET[ie];
//////////////
						if(arGID[ieGID]){if(eltype==8){bscodex=63;nodex=8;}
										 else if(eltype==7){bscodex=31;nodex=6;}
										 else {bscodex=15;nodex=4;}
										 for(in=0;in<nodex;in++) *(arr1+nop1[MXNPEI*ie+in])= *(arr1+nop1[MXNPEI*ie+in])+1;
										}
						else bscodex=0;
						matno[ie]=matno[ie]-(bscode-bscodex)*t5;
					   }
 mxnelvertex=accum=0;for(ip=0;ip<npoin;ip++){if(mxnelvertex< *(arr1+ip))mxnelvertex= *(arr1+ip);accum=accum+ *(arr1+ip); *(arr1+ip)=accum;}
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
 try {
	  arr3=new long[accum];
	  }catch (EOutOfMemory &e){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient facet dynamic memory",L"Exit",MB_OK);exit(0);}
// honk<<mxnelvertex<<" MxNelVert "<<accum<<" "<<npoin<<"\n";
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
// node=(matno[ie]-eltype*t7-bscode*t5)/t3;
ieGID=indat.arrELSET[ie];
//////////////
						if(arGID[ieGID]){if(eltype==8)nodex=8;else if(eltype==7)nodex=6;else nodex=4;
										 for(in=0;in<nodex;in++){ip=nop1[MXNPEI*ie+in]; *(arr3+ *(arr1+ip))=10*ie+in; *(arr1+ip)= *(arr1+ip)+1;}
										}
					   }
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
//
 for(ie=0;ie<nelt-1;ie++)
   {eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
//   node=(matno[ie]-eltype*t7-bscode*t5)/t3;
ieGID=indat.arrELSET[ie];
//////////////
	if(arGID[ieGID])
	  {if(eltype==8)
		 {nside=6;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0)     {oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+5];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+6];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+7];}
				else if(iv==3){oppn0=nop1[MXNPEI*ie+3];oppn3=nop1[MXNPEI*ie+4];}
				else if(iv==4){oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+2];}
				else          {oppn0=nop1[MXNPEI*ie+4];oppn3=nop1[MXNPEI*ie+6];}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
ieGID=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{lows=istx;tops= *(arr1+oppn3+1) -1;
								 if(eltypex==8)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
										  ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;
										  in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;ivp=side_arr6[in0+in0p-3];
										  if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
										  else corruptf[0]=corruptf[0]+1;
										  isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==7)
		 {nside=5;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0){oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+4];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+5];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+3];}
				else if(iv==3){oppn0=nop1[MXNPEI*ie+0];oppn1=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+2];istxp= *(arr1+oppn1);}
				else {oppn0=nop1[MXNPEI*ie+3];oppn1=nop1[MXNPEI*ie+4];oppn3=nop1[MXNPEI*ie+5];istxp= *(arr1+oppn1);}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
ieGID=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{
//MMMMMMMMMMMMMMM Correction 5/18/2009
								 lows=istx;tops= *(arr1+oppn3+1) -1;
//MMMMMMMMMMMMMMMMMMMMMMMMMM
								 if(iv<3)
								   {
//                                    lows=istx;tops= *(arr1+oppn3+1) -1;
									if(eltypex==8)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
											 ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;
											 in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;ivp=side_arr6[in0+in0p-3];
											 if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
											 else corruptf[0]=corruptf[0]+1;
											 isw=1;break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 else
								   {if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr6[in0+in0p+in0pp-3];
												   if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												   else corruptf[1]=corruptf[1]+1;
												   isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==5)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==5)
		 {nside=4;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0){oppn0=nop1[MXNPEI*ie+0];oppn1=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+1];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn1=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+3];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn1=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+3];}
				else {oppn0=nop1[MXNPEI*ie+3];oppn1=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+1];}
				isw=0;istx= *(arr1+oppn3);istxp= *(arr1+oppn1);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
ieGID=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{
//MMMMMMMMMMMMMMM Correction 5/18/2009
								 lows=istx;tops= *(arr1+oppn3+1) -1;
//MMMMMMMMMMMMMMMMMMMMMMMMMM
								 if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr6[in0+in0p+in0pp-3];
												if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												else corruptf[2]=corruptf[2]+1;
												isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==5)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
	   else {
			 corruptf[3]=corruptf[3]+1;
			}
	  }
   }
 delete [] arr1;delete [] arr3;
//	}
//__finally {
Screen->Cursor=Save_Cursor;
//}
 if(corruptf[3]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported elements found in FDelemfacets3()",L"Warning",MB_OK);}
 if     (corruptf[0] && corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge/hex found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[0] && corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge/hex found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[2] && corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/hex found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-hex found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-wedge found in FDelemfacets3()",L"Warning",MB_OK);}
 else if(corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet-wedge found in FDelemfacets3()",L"Warning",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::FDelemfacets3a(long npoin,long nelt,long nop1[],long matno[])
// Version using huge memory for speed (Code allows for quadratic elements)  Global LONG_INT,MXNPEI,MAX_GID,nGIDmax
// Corrcted 2008/08/15 for several erroneous "isw=1;break;".
// Version with log search throughout
{int isw=0,side_arr8[26]={16,0,0,0,0,16,1,1,0,0,8,2,2,8,0,0,4,4,0,0,32,0,0,0,0,32},
	 side_arr6[10]={8,1,4,2,0,0,0,0,0,16},side_arr4[4]={1,8,4,2};
 long ie=0,eltype=0,bscode=0,t3=1000,t5=100000,t7=10000000,
	  ip=0,in=0,iv=0,ivp=0,nside=0,in0=0,in0p=0,in0pp=0,inx=0,inxp=0,inxpp=0,oppn0=0,oppn1=0,oppn3=0,prod=0,
	  iex=0,eltypex=0,bscodex=0,nodex=0,mxnelvertex=0,accum=0,
	  lows=0,mids=0,tops=0,istx=0,lowsp=0,midsp=0,topsp=0,istxp=0,corruptf[4],*arr1=NULL,*arr3=NULL;
// Note: matno[i]=8*10000000+nodes*1000+ipid -1; Also bscodex=1+2+4+8+16+32......
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//try {
//
 arr1=new long[npoin+1];for(ip=0;ip<npoin+1;ip++) *(arr1+ip)=0;
 for(ip=0;ip<4;ip++)corruptf[ip]=0; //EFP 8/01/2014
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
						if(eltype==8){bscodex=63;nodex=8;}
						else if(eltype==7){bscodex=31;nodex=6;}
						else if(eltype==5){bscodex=15;nodex=4;}
						for(in=0;in<nodex;in++) *(arr1+nop1[MXNPEI*ie+in])= *(arr1+nop1[MXNPEI*ie+in])+1;
						matno[ie]=matno[ie]-(bscode-bscodex)*t5;
					   }
 mxnelvertex=accum=0;for(ip=0;ip<npoin;ip++){if(mxnelvertex< *(arr1+ip))mxnelvertex= *(arr1+ip);accum=accum+ *(arr1+ip); *(arr1+ip)=accum;}
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
// try {arr3=new long[accum];}catch (xalloc){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient facet dynamic memory",L"Exit",MB_OK);exit(0);}
 arr3=new long[accum];if(arr3==NULL){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient facet dynamic memory",L"Exit",MB_OK);exit(0);}
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
						if(eltype==8)nodex=8;else if(eltype==7)nodex=6;else nodex=4;
						for(in=0;in<nodex;in++){ip=nop1[MXNPEI*ie+in]; *(arr3+ *(arr1+ip))=10*ie+in; *(arr1+ip)= *(arr1+ip)+1;}
					   }
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
//
 for(ie=0;ie<nelt-1;ie++)
   {eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;
//    if(arGID[ieGID])
//      {
//
	   if(eltype==8)
		 {nside=6;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0)     {oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+5];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+6];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+7];}
				else if(iv==3){oppn0=nop1[MXNPEI*ie+3];oppn3=nop1[MXNPEI*ie+4];}
				else if(iv==4){oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+2];}
				else          {oppn0=nop1[MXNPEI*ie+4];oppn3=nop1[MXNPEI*ie+6];}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
//                              if(ieGIDx==ieGID)
//                                {
								 lows=istx;tops= *(arr1+oppn3+1) -1;
								 if(eltypex==8)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
										  ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;
										  in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;ivp=side_arr6[in0+in0p-3];
//										  if(ivp>0 && ivp<8){matno[iex]=matno[iex]-ivp*t5;isw=1;break;}
//										  else {extern PACKAGE void __fastcall Beep(void);
////												Application->MessageBox(L"Corrupt hex-wedge mesh",L"Terminate",MB_OK);
//												Application->MessageBox(L"Ignoring corrupt hex-wedge mesh",L"Warning",MB_OK);
////												exit(0);
//												break;
//											   }
										  if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
										  else corruptf[0]=corruptf[0]+1;
										  isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
//                                }
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==7)
		 {nside=5;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0)     {oppn0=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+4];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+5];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+3];}
				else if(iv==3){oppn0=nop1[MXNPEI*ie+0];oppn1=nop1[MXNPEI*ie+1];oppn3=nop1[MXNPEI*ie+2];istxp= *(arr1+oppn1);}
				else          {oppn0=nop1[MXNPEI*ie+3];oppn1=nop1[MXNPEI*ie+4];oppn3=nop1[MXNPEI*ie+5];istxp= *(arr1+oppn1);}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
//                              if(ieGIDx==ieGID)
//                                {
								 lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Moved to here... EFP 4/14/2011
								 if(iv<3)
								   {
//								    lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Moved above... EFP 4/14/2011
									if(eltypex==8)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
											 ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;

											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;
											 in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
											 ivp=side_arr6[in0+in0p-3];
											 if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
											 else corruptf[0]=corruptf[0]+1;
											 isw=1;break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 else
								   {if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr6[in0+in0p+in0pp-3];
												   if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												   else corruptf[1]=corruptf[1]+1;
												   isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==5)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 if(isw)break;
//                                }
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==5)
		 {nside=4;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0)     {oppn0=nop1[MXNPEI*ie+0];oppn1=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+1];}
				else if(iv==1){oppn0=nop1[MXNPEI*ie+1];oppn1=nop1[MXNPEI*ie+2];oppn3=nop1[MXNPEI*ie+3];}
				else if(iv==2){oppn0=nop1[MXNPEI*ie+2];oppn1=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+3];}
				else          {oppn0=nop1[MXNPEI*ie+3];oppn1=nop1[MXNPEI*ie+0];oppn3=nop1[MXNPEI*ie+1];}
				isw=0;istx= *(arr1+oppn3);istxp= *(arr1+oppn1);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
//                              if(ieGIDx==ieGID)
//                                {
								 lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Added to here... EFP 4/14/2011
								 if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr6[in0+in0p+in0pp-3];
												if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												else corruptf[2]=corruptf[2]+1;
												isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==5)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
//                                }
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
	   else {
			 corruptf[3]=corruptf[3]+1;
			}
//      }
//
   }
 delete [] arr1;delete [] arr3;
//	}
//__finally {
Screen->Cursor=Save_Cursor;
//}
 if(corruptf[3]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported elements found in FDelemfacets3a()",L"Warning",MB_OK);}
 if     (corruptf[0] && corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge/hex found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[0] && corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge/hex found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[2] && corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/hex found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-hex found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-wedge found in FDelemfacets3a()",L"Warning",MB_OK);}
 else if(corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet-wedge found in FDelemfacets3a()",L"Warning",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::FDelemfacets_arE3(long npoin,long nelt,long nop1[],long matno[],int arELE[])
// Version using huge memory for speed (Code allows for quadratic elements)  Global LONG_INT,MXNPEL,MAX_GID,nGIDmax
// Corrected 2008/08/15 for several erroneous "isw=1;break;".
// Version with log search throughout
{int isw=0,side_arr8[26]={16,0,0,0,0,16,1,1,0,0,8,2,2,8,0,0,4,4,0,0,32,0,0,0,0,32},
	 side_arr6[10]={8,1,4,2,0,0,0,0,0,16},side_arr4[4]={1,8,4,2};
 long ie=0,eltype=0,bscode=0,node=0,ieGID=0,t3=1000,t5=100000,t7=10000000,
	  ip=0,in=0,iv=0,ivp=0,nside=0,in0=0,in0p=0,in0pp=0,inx=0,inxp=0,inxpp=0,oppn0=0,oppn1=0,oppn3=0,prod=0,
	  iex=0,eltypex=0,bscodex=0,nodex=0,ieGIDx=0,mxnelvertex=0,accum=0,
	  lows=0,mids=0,tops=0,istx=0,lowsp=0,midsp=0,topsp=0,istxp=0,corruptf[4],*arr1=NULL,*arr3=NULL;
// Note: matno[i]=8*10000000+nodes*1000+ipid -1; Also bscodex=1+2+4+8+16+32......
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//try {
//
 arr1=new long[npoin+1];for(ip=0;ip<npoin+1;ip++) *(arr1+ip)=0;
 for(ip=0;ip<4;ip++)corruptf[ip]=0; //EFP 8/01/2014
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;node=(matno[ie]-eltype*t7-bscode*t5)/t3;
////						ieGID=matno[ie]-eltype*t7-bscode*t5-node*t3;
//////////////// EFP 2/20/2012  Note that matno[] here is indat.matno[]
//						if(indat.trackELSET[ie+1]-indat.trackELSET[ie]==1)ieGID=0;
//						else if(indat.arrELSET[ indat.trackELSET[ie+1]-1 ]<0)ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-2 ];
//						else {if(indat.GIDcol==1)ieGID=indat.arrELSET[ indat.trackELSET[ie]+indat.GIDcol ]; //Prioritize WG
//							  else ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-1 ];                      //Prioritize WP
//							 }
ieGID=indat.arrELSET[ie];
//////////////
						if(arELE[ie]){if(eltype==8){bscodex=63;nodex=8;}
									  else if(eltype==7){bscodex=31;nodex=6;}
									  else {bscodex=15;nodex=4;}
									  for(in=0;in<nodex;in++) *(arr1+nop1[MXNPEL*ie+in])= *(arr1+nop1[MXNPEL*ie+in])+1;
									 }
						else bscodex=0;
						matno[ie]=matno[ie]-(bscode-bscodex)*t5;
					   }
 mxnelvertex=accum=0;for(ip=0;ip<npoin;ip++){if(mxnelvertex< *(arr1+ip))mxnelvertex= *(arr1+ip);accum=accum+ *(arr1+ip); *(arr1+ip)=accum;}
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
// try {arr3=new long[accum];}catch (xalloc){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient facet dynamic memory",L"Exit",MB_OK);exit(0);}
 arr3=new long[accum];if(arr3==NULL){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient facet dynamic memory",L"Exit",MB_OK);exit(0);}
// honk<<mxnelvertex<<" MxNelVert "<<accum<<" "<<npoin<<"\n";
 for(ie=0;ie<nelt;ie++){eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;node=(matno[ie]-eltype*t7-bscode*t5)/t3;
////						ieGID=matno[ie]-eltype*t7-bscode*t5-node*t3;
//////////////// EFP 2/20/2012
//						if(indat.trackELSET[ie+1]-indat.trackELSET[ie]==1)ieGID=0;
//						else if(indat.arrELSET[ indat.trackELSET[ie+1]-1 ]<0)ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-2 ];
//						else {if(indat.GIDcol==1)ieGID=indat.arrELSET[ indat.trackELSET[ie]+indat.GIDcol ]; //Prioritize WG
//							  else ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-1 ];                      //Prioritize WP
//							 }
ieGID=indat.arrELSET[ie];
//////////////
						if(arELE[ie]){if(eltype==8)nodex=8;else if(eltype==7)nodex=6;else nodex=4;
									  for(in=0;in<nodex;in++){ip=nop1[MXNPEL*ie+in]; *(arr3+ *(arr1+ip))=10*ie+in; *(arr1+ip)= *(arr1+ip)+1;}
									 }
					   }
 for(ip=npoin;ip>0;ip--) *(arr1+ip)= *(arr1+ip-1); *arr1=0;
//
 for(ie=0;ie<nelt-1;ie++)
   {eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;node=(matno[ie]-eltype*t7-bscode*t5)/t3;
////	ieGID=matno[ie]-eltype*t7-bscode*t5-node*t3;
//////////////// EFP 2/20/2012
//						if(indat.trackELSET[ie+1]-indat.trackELSET[ie]==1)ieGID=0;
//						else if(indat.arrELSET[ indat.trackELSET[ie+1]-1 ]<0)ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-2 ];
//						else {if(indat.GIDcol==1)ieGID=indat.arrELSET[ indat.trackELSET[ie]+indat.GIDcol ]; //Prioritize WG
//							  else ieGID=indat.arrELSET[ indat.trackELSET[ie+1]-1 ];                      //Prioritize WP
//							 }
ieGID=indat.arrELSET[ie];
//////////////
	if(arELE[ie])
	  {if(eltype==8)
		 {nside=6;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0){oppn0=nop1[MXNPEL*ie+0];oppn3=nop1[MXNPEL*ie+5];}
				else if(iv==1){oppn0=nop1[MXNPEL*ie+1];oppn3=nop1[MXNPEL*ie+6];}
				else if(iv==2){oppn0=nop1[MXNPEL*ie+2];oppn3=nop1[MXNPEL*ie+7];}
				else if(iv==3){oppn0=nop1[MXNPEL*ie+3];oppn3=nop1[MXNPEL*ie+4];}
				else if(iv==4){oppn0=nop1[MXNPEL*ie+0];oppn3=nop1[MXNPEL*ie+2];}
				else {oppn0=nop1[MXNPEL*ie+4];oppn3=nop1[MXNPEL*ie+6];}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
////							  ieGIDx=matno[iex]-eltypex*t7-bscodex*t5-nodex*t3;
//////////////// EFP 2/20/2012
//						if(indat.trackELSET[iex+1]-indat.trackELSET[iex]==1)ieGIDx=0;
//						else if(indat.arrELSET[ indat.trackELSET[iex+1]-1 ]<0)ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-2 ];
//						else {if(indat.GIDcol==1)ieGIDx=indat.arrELSET[ indat.trackELSET[iex]+indat.GIDcol ]; //Prioritize WG
//							  else ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-1 ];                      //Prioritize WP
//							 }
ieGIDx=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{lows=istx;tops= *(arr1+oppn3+1) -1;
								 if(eltypex==8)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
										  ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {matno[ie]=matno[ie]-prod*t5;
										  in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;ivp=side_arr6[in0+in0p-3];
//										  if(ivp>0 && ivp<8){matno[iex]=matno[iex]-ivp*t5;isw=1;break;}
//										  else {extern PACKAGE void __fastcall Beep(void);
////												Application->MessageBox(L"Corrupt hex-wedge mesh",L"Terminate",MB_OK);
//												Application->MessageBox(L"Ignoring corrupt hex-wedge mesh",L"Warning",MB_OK);
////												exit(0);
//												break;
//											   }
										  if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
										  else corruptf[0]=corruptf[0]+1;
										  isw=1;break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==7)
		 {nside=5;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0){oppn0=nop1[MXNPEL*ie+0];oppn3=nop1[MXNPEL*ie+4];}
				else if(iv==1){oppn0=nop1[MXNPEL*ie+1];oppn3=nop1[MXNPEL*ie+5];}
				else if(iv==2){oppn0=nop1[MXNPEL*ie+2];oppn3=nop1[MXNPEL*ie+3];}
				else if(iv==3){oppn0=nop1[MXNPEL*ie+0];oppn1=nop1[MXNPEL*ie+1];oppn3=nop1[MXNPEL*ie+2];istxp= *(arr1+oppn1);}
				else {oppn0=nop1[MXNPEL*ie+3];oppn1=nop1[MXNPEL*ie+4];oppn3=nop1[MXNPEL*ie+5];istxp= *(arr1+oppn1);}
				isw=0;istx= *(arr1+oppn3);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
////							  ieGIDx=matno[iex]-eltypex*t7-bscodex*t5-nodex*t3;
//////////////// EFP 2/20/2012
//						if(indat.trackELSET[iex+1]-indat.trackELSET[iex]==1)ieGIDx=0;
//						else if(indat.arrELSET[ indat.trackELSET[iex+1]-1 ]<0)ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-2 ];
//						else {if(indat.GIDcol==1)ieGIDx=indat.arrELSET[ indat.trackELSET[iex]+indat.GIDcol ]; //Prioritize WG
//							  else ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-1 ];                      //Prioritize WP
//							 }
ieGIDx=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{
								 lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Moved to here... EFP 4/14/2011
								 if(iv<3)
								   {
//								    lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Moved above... EFP 4/14/2011
									if(eltypex==8)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;
											 ivp=side_arr8[3*min(in0,in0p)+2*max(in0,in0p)-4];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{matno[ie]=matno[ie]-prod*t5;
											 in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;ivp=side_arr6[in0+in0p-3];
//											 if(ivp>0 && ivp<8){matno[iex]=matno[iex]-ivp*t5;isw=1;break;}
//											 else {extern PACKAGE void __fastcall Beep(void);
////												   Application->MessageBox(L"Corrupt hex-wedge mesh",L"Terminate",MB_OK);
//												   Application->MessageBox(L"Ignoring corrupt hex-wedge mesh",L"Notice",MB_OK);
////												   exit(0);
//												   break;
//												  }
											 if(ivp>0 && ivp<8)matno[iex]=matno[iex]-ivp*t5;
											 else corruptf[0]=corruptf[0]+1;
											 isw=1;break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 else
								   {if(eltypex==7)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr6[in0+in0p+in0pp-3];
//												   if(ivp>4){matno[iex]=matno[iex]-ivp*t5;isw=1;break;}
//												   else {
//														 extern PACKAGE void __fastcall Beep(void);
////														 Application->MessageBox(L"Corrupt wedge-wedge mesh",L"Terminate",MB_OK);
//														 Application->MessageBox(L"Ignoring corrupt wedge-wedge mesh",L"Warning",MB_OK);
////														 exit(0);
//														 break;
//														}
												   if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												   else corruptf[1]=corruptf[1]+1;
												   isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
									else if(eltypex==5)
									  {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
										 {mids=(lows+tops)/2;
										  if( *(arr3+mids)/10==iex)
											{istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
											 for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											   {midsp=(lowsp+topsp)/2;
												if( *(arr3+midsp)/10==iex)
												  {matno[ie]=matno[ie]-prod*t5;
												   in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												   ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
												  }
												else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
												else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											   }
											 break;
											}
										  else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
										  else {istx=mids;if(lows==tops)break;else lows=mids+1;}
										 }
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
//
	   else if(eltype==5)
		 {nside=4;prod=1;
		  for(iv=0;iv<nside;iv++)
			{if(bscode && prod)
			   {if(iv==0){oppn0=nop1[MXNPEL*ie+0];oppn1=nop1[MXNPEL*ie+2];oppn3=nop1[MXNPEL*ie+1];}
				else if(iv==1){oppn0=nop1[MXNPEL*ie+1];oppn1=nop1[MXNPEL*ie+2];oppn3=nop1[MXNPEL*ie+3];}
				else if(iv==2){oppn0=nop1[MXNPEL*ie+2];oppn1=nop1[MXNPEL*ie+0];oppn3=nop1[MXNPEL*ie+3];}
				else {oppn0=nop1[MXNPEL*ie+3];oppn1=nop1[MXNPEL*ie+0];oppn3=nop1[MXNPEL*ie+1];}
				isw=0;istx= *(arr1+oppn3);istxp= *(arr1+oppn1);
				for(inx= *(arr1+oppn0);inx< *(arr1+oppn0+1);inx++)
				  {iex= *(arr3+inx)/10;
				   if(iex>ie){eltypex=matno[iex]/t7;bscodex=(matno[iex]-eltypex*t7)/t5;nodex=(matno[iex]-eltypex*t7-bscodex*t5)/t3;
////				              ieGIDx=matno[iex]-eltypex*t7-bscodex*t5-nodex*t3;
///////////////// EFP 2/20/2012
//						if(indat.trackELSET[iex+1]-indat.trackELSET[iex]==1)ieGIDx=0;
//						else if(indat.arrELSET[ indat.trackELSET[iex+1]-1 ]<0)ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-2 ];
//						else {if(indat.GIDcol==1)ieGIDx=indat.arrELSET[ indat.trackELSET[iex]+indat.GIDcol ]; //Prioritize WG
//							  else ieGIDx=indat.arrELSET[ indat.trackELSET[iex+1]-1 ];                      //Prioritize WP
//							 }
ieGIDx=indat.arrELSET[iex];
//////////////
							  if(ieGIDx==ieGID)
								{
								 lows=istx;tops= *(arr1+oppn3+1) -1; //Correction??? Added to here... EFP 4/14/2011
								 if(eltypex==7)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr6[in0+in0p+in0pp-3];
//												if(ivp>4){matno[iex]=matno[iex]-ivp*t5;isw=1;break;}
//												else {extern PACKAGE void __fastcall Beep(void);
////													  Application->MessageBox(L"Corrupt tetra-wedge mesh",L"Terminate",MB_OK);
//													  Application->MessageBox(L"Ignoring corrupt tetra-wedge mesh",L"Warning",MB_OK);
////													  exit(0);
//													  break;
//													 }
												if(ivp>4)matno[iex]=matno[iex]-ivp*t5;
												else corruptf[2]=corruptf[2]+1;
												isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 else if(eltypex==5)
								   {for(inxp=istx;inxp< *(arr1+oppn3+1);inxp++)
									  {mids=(lows+tops)/2;
									   if( *(arr3+mids)/10==iex)
										 {istx=mids+1;lowsp=istxp;topsp= *(arr1+oppn1+1) -1;
										  for(inxpp=istxp;inxpp< *(arr1+oppn1+1);inxpp++)
											{midsp=(lowsp+topsp)/2;
											 if( *(arr3+midsp)/10==iex)
											   {matno[ie]=matno[ie]-prod*t5;
												in0= *(arr3+inx)-10*iex;in0p= *(arr3+mids)-10*iex;in0pp= *(arr3+midsp)-10*iex;
												ivp=side_arr4[in0+in0p+in0pp-3];matno[iex]=matno[iex]-ivp*t5;isw=1;break;
											   }
											 else if( *(arr3+midsp)/10>iex){if(lowsp==topsp)break;else topsp=midsp-1;}
											 else {istxp=midsp;if(lowsp==topsp)break;else lowsp=midsp+1;}
											}
										  break;
										 }
									   else if( *(arr3+mids)/10>iex){if(lows==tops)break;else tops=mids-1;}
									   else {istx=mids;if(lows==tops)break;else lows=mids+1;}
									  }
								   }
								 if(isw)break;
								}
							 }
				  }
			   }
			 prod=prod*2;
			}
		 }
	   else {
			 corruptf[3]=corruptf[3]+1;
			}
	  }
   }
 delete [] arr1;delete [] arr3;
//	}
//__finally {
Screen->Cursor=Save_Cursor;
//}
 if(corruptf[3]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported elements found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 if     (corruptf[0] && corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge/hex found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[0] && corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge/hex found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[1] && corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/wedge found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[2] && corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet/hex found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[0]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-hex found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt wedge-wedge found in FDelemfacets_arE3()",L"Warning",MB_OK);}
 else if(corruptf[2]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt tet-wedge found in FDelemfacets_arE3()",L"Warning",MB_OK);}
}
//---------------------------------------------------------------------------
long TForm1::FDcullfacetc(long nelt,long nop1[],long matno[],float c1[],long arGID[],long arbFace[],long rbTem[])
// Global MXNPEI.  Shade coding removed for speed  EFP 1/02/2012
{long ies=0,eltypes=0,bscodes=0,ieGIDs=0,fType=0,t3=1000,t5=100000,t7=10000000,nFace=0;
 float CENT=1.f/3.f,DJD=0.f,HN[9],SN[MXNPELS],SG[NDF*MXNPELS],DJR[9],proximi=0.f,distant=0.f;
 int is=0,iprox=0,idist=0,npts=0,iside=0;
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
 int gdata6[20]={0,1,4,3,
				 1,2,5,4,
				 2,0,3,5,
				 0,2,1,0,
				 3,4,5,0};
 int gdata4[12]={0,2,1,
				 1,2,3,
				 2,0,3,
				 3,0,1};
 nFace=0;
 for(ies=0;ies<nelt;ies++)
   {eltypes=matno[ies]/t7;bscodes=(matno[ies]-eltypes*t7)/t5;ieGIDs=indat.arrELSET[ies];
	if(arGID[ieGIDs] && bscodes>0)
	  {if(bscodes >= 32){bscodes=bscodes-32;
						 STFISO8(3,  0.f,  0.f,  1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=5;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	   if(bscodes >= 16){bscodes=bscodes-16;
						 if(eltypes==7)STFISO6(3,CENT,CENT, 1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else          STFISO8(3,  0.f,  0.f, -1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=4;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	   if(bscodes >=  8){bscodes=bscodes-8;
						 if(eltypes==5)     STFISO4(3,CENT,CENT,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else if(eltypes==7)STFISO6(3,CENT,CENT, -1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else               STFISO8(3, -1.f,  0.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=3;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	   if(bscodes >=  4){bscodes=bscodes-4;
						 if(eltypes==5)     STFISO4(3,CENT,  0.f,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else if(eltypes==7)STFISO6(3,0.5f,0.f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else               STFISO8(3,  0.f,  1.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=2;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	   if(bscodes >=  2){bscodes=bscodes-2;
						 if(eltypes==5)     STFISO4(3,  0.f,CENT,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else if(eltypes==7)STFISO6(3,0.f,0.5f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else               STFISO8(3,  1.f,  0.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=1;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	   if(bscodes >=  1){if(eltypes==5)     STFISO4(3,CENT,CENT,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else if(eltypes==7)STFISO6(3,0.5f,0.5f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 else               STFISO8(3,  0.f, -1.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f){
									  iside=0;arbFace[nFace]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTem[nFace]=iprox*100+idist;
//////////////////
									  nFace++;
									 }
						}
	  }
   }
 return nFace;
}
//---------------------------------------------------------------------------
long TForm1::FDcullfacet_arEc(long nelt,long nop1[],long matno[],float c1[],int arELEM[],long arbFacet[],long rbTemp[])
// Version with 20n elements. FDorderfacet() merged below  EFP 1/02/2012
{long nFacets=0,ij=0,ies=0,eltypes=0,bscodes=0,nodes=0,fType=0,t3=1000,t5=100000,t7=10000000;
 float CENT=1.f/3.f,DJD=0.f,HN[9],SN[8],SG[24],DJR[9+1],proximi=0.f,distant=0.f;
 float testcor[4*2]= { -0.9f,-0.9f,
						0.9f,-0.9f,
						0.9f, 0.9f,
					   -0.9f, 0.9f};
 int is=0,iprox=0,idist=0,npts=0,iside=0;
// int gdata8[24]={0,1,5,4,
//				 1,2,6,5,
//				 3,2,6,7,
//				 0,3,7,4,
//				 0,1,2,3,
//				 4,5,6,7};
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
 int gdata6[20]={0,1,4,3,
				 1,2,5,4,
				 2,0,3,5,
				 0,2,1,0,
				 3,4,5,0};
 int gdata4[12]={0,2,1,
				 1,2,3,
				 2,0,3,
				 3,0,1};
 nFacets=0;
 for(ies=0;ies<nelt;ies++)if(arELEM[ies])
   {eltypes=matno[ies]/t7;bscodes=(matno[ies]-eltypes*t7)/t5;nodes=(matno[ies]-eltypes*t7-bscodes*t5)/t3;
	if(bscodes>0)
	  {if(bscodes >= 32){bscodes=bscodes-32;
if(nodes==8)STFISO8(3,0.f,0.f,1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else for(ij=0;ij<4;ij++){STFISO20(3,testcor[2*ij],testcor[2*ij+1],1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
						 if(HN[2]>0.f)break;
						}
						 if(HN[2]>0.f){iside=5;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
	   if(bscodes >= 16){bscodes=bscodes-16;
if(eltypes==7)STFISO6(3,CENT,CENT, 1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else {if(nodes==8)STFISO8(3,  0.f,  0.f, -1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
	  else for(ij=0;ij<4;ij++){STFISO20(3,testcor[2*ij],testcor[2*ij+1],-1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
							   if(HN[2]>0.f)break;
							  }
	 }
						 if(HN[2]>0.f){iside=4;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
	   if(bscodes >=  8){bscodes=bscodes-8;
if(eltypes==5)     STFISO4(3,CENT,CENT,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else if(eltypes==7)STFISO6(3,CENT,CENT, -1.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else {if(nodes==8) STFISO8(3, -1.f,  0.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
	  else for(ij=0;ij<4;ij++){STFISO20(3,-1.f,testcor[2*ij],testcor[2*ij+1],&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
							   if(HN[2]>0.f)break;
							  }
	 }
						 if(HN[2]>0.f){iside=3;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
	   if(bscodes >=  4){bscodes=bscodes-4;
if(eltypes==5)     STFISO4(3,CENT,  0.f,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else if(eltypes==7)STFISO6(3,0.5f,0.f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else               {
// Wrong???   if(nodes==8){STFISO8(3,  1.,  0.,  0.,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ies,c1);test=HN[2];}
   if(nodes==8)STFISO8(3,0.f,1.f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
   else for(ij=0;ij<4;ij++){STFISO20(3,testcor[2*ij],1.f,testcor[2*ij+1],&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
							if(HN[2]>0.f)break;
						   }
				   }
						 if(HN[2]>0.f){iside=2;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
	   if(bscodes >=  2){bscodes=bscodes-2;
if(eltypes==5)     STFISO4(3,  0.f,CENT,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else if(eltypes==7)STFISO6(3,0.f,0.5f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else {if(nodes==8) STFISO8(3,  1.f,  0.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
	  else for(ij=0;ij<4;ij++){STFISO20(3,1.f,testcor[2*ij],testcor[2*ij+1],&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
							   if(HN[2]>0.f)break;
							  }
	 }
						 if(HN[2]>0.f){iside=1;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
	   if(bscodes >=  1){
if(eltypes==5)     STFISO4(3,CENT,CENT,CENT,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else if(eltypes==7)STFISO6(3,0.5f,0.5f,0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
else {if(nodes==8) STFISO8(3,  0.f, -1.f,  0.f,&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
	  else for(ij=0;ij<4;ij++){STFISO20(3,testcor[2*ij],-1.f,testcor[2*ij+1],&DJD,HN,SN,SG,DJR,nop1+MXNPEI*ies,c1);
							   if(HN[2]>0.f)break;
							  }
	 }
						 if(HN[2]>0.f){iside=0;arbFacet[nFacets]=ies*t3+fType*10+iside;
//////////////////
proximi= -1.e28f;distant= 1.e28f;
if(eltypes==5){for(is=0;is<3;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];iprox=gdata4[3*iside+is];}
								   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata4[3*iside+is]]+2];idist=gdata4[3*iside+is];}
								  }
			  }
else if(eltypes==7){if(iside>2)npts=3;else npts=4;
					for(is=0;is<npts;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];iprox=gdata6[4*iside+is];}
										   if(distant>c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata6[4*iside+is]]+2];idist=gdata6[4*iside+is];}
										  }
				   }
else for(is=0;is<4;is++){if(proximi<c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){proximi=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];iprox=gdata8[4*iside+is];}
						 if(distant>c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2]){distant=c1[NDF*nop1[MXNPEI*ies+gdata8[4*iside+is]]+2];idist=gdata8[4*iside+is];}
						}
rbTemp[nFacets]=iprox*100+idist;
//////////////////
									  nFacets++;
									 }
						}
		 }
	 }
 return nFacets;
}
//---------------------------------------------------------------------------
void TForm1::FDsort_low1(long *indat_nop1,float *indat_c1,long nFace,long *arbFace,long *rbTem,long ntranche)
// Version with status[nFace]. Note that constant ntranche=8 (throughout) was found to be fastest....
// Sort in increasing order (Buckets & efficient partition/stride sort used)  Global NDF,MXNPEI
{int isw=0,procsw=0;long ic=0,ip=0,is=0,ie=0,t3=1000,curstat=1,statct=0,outct=0,outct0=nFace,loopct=0,sval=0,irec0=0,irec1=0,*facetmap=NULL,*facettrash=NULL,*facetloc=NULL,*status=NULL;
//// float lolim=0.,upmax=0.,disTOL=0.01,*partition=NULL;
 float lolim=0.f,upmax=0.f,disTOL=0.1f,*partition=NULL;
// float lolim=0.,upmax=0.,disTOL=0.5,*partition=NULL;
 if(nFace<2){honk<<nFace<<" nFacets\n";extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"FDsort_low insufficient nFacets",L"Terminate",MB_OK);exit(0);}
//
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//try {
//
 facetmap=new long[nFace];status=new long[nFace];facettrash=new long[ntranche*nFace];facetloc=new long[2*ntranche+1];partition=new float[ntranche+1];
 for(ic=0;ic<nFace;ic++) *(facetmap+ic)=ic;for(ic=0;ic<nFace;ic++) *(status+ic)=1;isw=1;loopct=0;

//honk<<ntranche*nFace<<" FDsort_low1\n";//if(1==1)exit(0);

 while (isw)
   {irec0=0;sval= *(status+0);
	for(ic=0;ic<nFace;ic++)
	  {if( *(status+ic)==sval)irec1=ic;
	   else {if(sval){if(irec1-irec0==0) *(status+irec0)=0;
					  else if(irec1-irec0+1<9){sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
											   for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
											  }
					  else if(irec1-irec0+1<17){sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
												sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
												for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
											   }
					  else {if(procsw){sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
									   sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
									   for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
									  }
							else {lolim=1.e30f;upmax= -1.e30f;
								  for(ip=irec0;ip<irec1+1;ip++)
									{is= *(facetmap+ip);ie= *(arbFace+is)/t3;
									 if(lolim> *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2))lolim= *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2);
									 if(upmax< *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2))upmax= *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2);
									}
								  if(upmax-lolim>disTOL)sort_by_partition(indat_nop1,indat_c1,irec1-irec0+1,arbFace,rbTem,facetmap,facettrash,facetloc,status,partition,ntranche,irec0,irec1,lolim,upmax,&curstat);
								  else {if(irec1-irec0+1>8)sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                                        sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                                        for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
									   }
								 }
						   }
                     }
			 irec0=irec1=ic;sval= *(status+ic);
			}
      }
	if(sval){irec1=nFace-1;
			 if(irec1-irec0==0) *(status+irec0)=0;
             else if(irec1-irec0+1<9){sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
									  for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
                                     }
			 else if(irec1-irec0+1<17){sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
									   sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                                       for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
                                      }
			 else {if(procsw){sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                              sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                              for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
							 }
				   else {lolim=1.e30f;upmax= -1.e30f;
						 for(ip=irec0;ip<irec1+1;ip++)
						   {is= *(facetmap+ip);ie= *(arbFace+is)/t3;
							if(lolim> *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2))lolim= *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2);
							if(upmax< *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2))upmax= *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2);
						   }
                         if(upmax-lolim>disTOL)sort_by_partition(indat_nop1,indat_c1,irec1-irec0+1,arbFace,rbTem,facetmap,facettrash,facetloc,status,partition,ntranche,irec0,irec1,lolim,upmax,&curstat);
						 else {if(irec1-irec0+1>8)sort_by_stride(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
							   sort_by_bubble(indat_nop1,indat_c1,arbFace,rbTem,facetmap,irec0,irec1);
                               for(ip=irec0;ip<irec1+1;ip++) *(status+ip)=0;
							  }
						}
                  }
            }
	loopct++;statct=0;for(ic=0;ic<nFace;ic++)statct=statct+ *(status+ic);
	if(!statct)isw=0;
	else {outct=0;for(ic=0;ic<nFace;ic++)if( *(status+ic))outct=outct+ *(status+ic);
		  if(outct==outct0 && loopct>3)procsw=1;
		  outct0=outct;
		 }
   }
 delete [] facettrash;delete [] facetloc;delete [] partition;
 for(ic=0;ic<nFace;ic++) *(status+ic)= *(arbFace+ *(facetmap+ic));for(ic=0;ic<nFace;ic++) *(arbFace+ic)= *(status+ic);
 for(ic=0;ic<nFace;ic++) *(status+ic)= *(rbTem+ *(facetmap+ic));for(ic=0;ic<nFace;ic++) *(rbTem+ic)= *(status+ic);
 delete [] facetmap;delete [] status;
//	}
//__finally {
Screen->Cursor=Save_Cursor;
//}
}
//---------------------------------------------------------------------------
void TForm1::sort_by_partition(long *indat_nop1,float *indat_c1,long cFace,long *arbFace,long *rbTem,
   long *facetmap,long *facettrash,long *facetloc,long *status,float *partition,long ctranche,long lwFace,long upFace,float lolim,float upmax,long *curstat)
// Version with status[nFace]
{long pval=0,ic=0,ip=0,is=0,ist=0,ie=0,t3=1000,accum=0,accum0=0,purtranche=0,lows=0,mids=0,tops=0,procd=0,shift=0,ism=0,istart=0,iend=0,itf=0,itr=0;
 float cval=float(cFace)/float(ctranche),deltranche=(upmax-lolim)/(float(ctranche*cFace-1)),dmargin=(deltranche*float(ctranche*cFace)-upmax+lolim)/2.f,zcor=0.f;
 for(ic=0;ic<ctranche*cFace;ic++)*(facettrash+ic)=0;
 for(ic=lwFace;ic<upFace+1;ic++){is=facetmap[ic];ie= *(arbFace+is)/t3;
								 (*(facettrash+long((*(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2)-lolim+dmargin)/deltranche)))++;
								}
 *(partition+0)=lolim-deltranche;accum=accum0=0;purtranche=1;
 for(ip=0;ip<ctranche*cFace;ip++){accum=accum+ *(facettrash+ip);pval=long(float(purtranche)*cval);
								  while (pval<=accum)
									{*(partition+purtranche)=deltranche*(float(ip)+float(pval-accum0)/float(accum-accum0))+lolim-dmargin;
									 purtranche++;pval=long(float(purtranche)*cval);
									}
								  accum0=accum;
								 }
 *(partition+ctranche)=upmax+deltranche;for(ip=0;ip<ctranche;ip++) *(facetloc+2*ip)= *(facetloc+2*ip+1)=ip*cFace; *(facetloc+2*ctranche)=ctranche*cFace;
 for(ic=lwFace;ic<upFace+1;ic++)
   {is= *(facetmap+ic);ie= *(arbFace+is)/t3;zcor=*(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2);lows=0;tops=ctranche-1;
	for(ip=0;ip<ctranche;ip++)
	  {mids=(lows+tops)/2;if(zcor>= *(partition+mids) && zcor< *(partition+mids+1))
							{
/////////////// Code to shift overpopulated regions of facettrash[]
if( *(facetloc+2*mids+1)>= *(facetloc+2*mids+2))
  {procd=0;for(ism=0;ism<ctranche;ism++)procd=procd+ *(facetloc+2*ism+1)- *(facetloc+2*ism);shift=min(cFace-procd,(ctranche*cFace-procd)/ctranche);
   for(itf=1;itf<ctranche;itf++){istart= *(facetloc+2*itf-1)+shift;
								 if(istart< *(facetloc+2*itf)){for(ism= *(facetloc+2*itf);ism< *(facetloc+2*itf+1);ism++) *(facettrash+ism- *(facetloc+2*itf)+istart)= *(facettrash+ism);
															   *(facetloc+2*itf+1)= *(facetloc+2*itf+1)- *(facetloc+2*itf)+istart; *(facetloc+2*itf)=istart;
															  }
								 else if(istart== *(facetloc+2*itf));
								 else {if(ctranche>itf)
                                         {for(itr=ctranche-1;itr>=itf;itr++)
											{iend= *(facetloc+2*itr+2)-shift;
											 if(iend> *(facetloc+2*itr+1)){for(ism= *(facetloc+2*itr+1)-1;ism>= *(facetloc+2*itr);ism--) *(facettrash+ism- *(facetloc+2*itr+1)+iend)= *(facettrash+ism);
                                                                           *(facetloc+2*itr)= *(facetloc+2*itr)- *(facetloc+2*itr+1)+iend; *(facetloc+2*itr+1)=iend;
																		  }
											 else break;
                                            }
										 }
                                       break;
                                      }
								}
  }
///////////////
							 *(facettrash+ *(facetloc+2*mids+1))=is;(*(facetloc+2*mids+1))++;break;
                            }
                          else if(zcor< *(partition+mids))tops=mids-1;
						  else lows=mids+1;
	  }
   }
 accum=lwFace;for(ip=0;ip<ctranche;ip++)
				{ist= *(facetloc+2*ip+1)- *(facetloc+2*ip);
                 if(ist==1){ *(facetmap+accum)= *(facettrash+ *(facetloc+2*ip)); *(status+accum)=0;accum++;}
				 else if(ist>1){for(ic= *(facetloc+2*ip);ic< *(facetloc+2*ip+1);ic++){ *(facetmap+accum)= *(facettrash+ic); *(status+accum)= *curstat;accum++;}
                                *curstat= *curstat+1;
							   }
				}
}
//---------------------------------------------------------------------------
void TForm1::sort_by_stride(long *indat_nop1,float *indat_c1,long *arbFace,long *rbTem,long *facetmap,long lwFace,long upFace)
{long ic=0,ip=0,is=0,isp=0,ie=0,iep=0,t3=1000,twoexp=long(log(float(upFace-lwFace+1))/log(2.f)),interval=long(exp(log(2.f)*float(twoexp)) +0.5f);
 for(ip=0;ip<twoexp;ip++) // This does not perform the final top-to-bottom term-by-term comparison.
   {for(ic=lwFace;ic<upFace+1-interval;ic++)
	  {is= *(facetmap+ic);ie= *(arbFace+is)/t3;isp= *(facetmap+ic+interval);iep= *(arbFace+isp)/t3;
	   if( *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie+ *(rbTem+is)/100)+2) >
		   *(indat_c1+NDF* *(indat_nop1+MXNPEI*iep+ *(rbTem+isp)/100)+2)){ *(facetmap+ic)=isp; *(facetmap+ic+interval)=is;}
      }
	interval=interval>>1;
   }
}
//---------------------------------------------------------------------------
void TForm1::sort_by_bubble(long *indat_nop1,float *indat_c1,long *arbFace,long *rbTem,long *facetmap,long lwFace,long upFace)
{int isw=0;long ic=0,ip=0,is0=0,is1=0,ie0=0,ie1=0,t3=1000; // Inefficient pair-by-pair bubble sort
 for(ic=lwFace;ic<upFace;ic++)
   {isw=0;for(ip=lwFace;ip<upFace+lwFace-ic;ip++)
			{is0= *(facetmap+ip);ie0= *(arbFace+is0)/t3;is1= *(facetmap+ip+1);ie1= *(arbFace+is1)/t3;
			 if( *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie0+ *(rbTem+is0)/100)+2)>
				 *(indat_c1+NDF* *(indat_nop1+MXNPEI*ie1+ *(rbTem+is1)/100)+2)){ *(facetmap+ip)=is1; *(facetmap+ip+1)=is0;isw=1;}
			}
	if(!isw)break;
   }
}
//---------------------------------------------------------------------------
void TForm1::STFISO20(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//  See User Manual for element surface coord.frame.
//  On positive sides: n x t1=t2; on negative HN()= -HN(positive side)
//  so n x t1 = -t2 on negative sides (ie. NOT corkscrew).
//  Normal=HN(1 to 3), t1=HN(4 to 6), t2=HN(7 to 9) so      x = HN.x'
//  CAUTION: do NOT evaluate normals at edges/nodes due to ambiguity!!
{int I=0,IM=0,IP=0,J=0;
//      DIMENSION HN(3),SN(1),SG(3,8),CORD(NDF,1),DJR(3,3),NOP(MXNPEL)
 float PRMT=1.E-10f,GP=0.f,GM=0.f,EP=0.f,EM=0.f,TP=0.f,TM=0.f,GPP=0.f,GMM=0.f,EPP=0.f,EMM=0.f,TPP=0.f,TMM=0.f,
		SL1 =0.f,SL2 =0.f,SL3 =0.f,SL4 =0.f,SL5 =0.f,SL6 =0.f,SL7 =0.f,SL8 =0.f,SL9 =0.f,SL10=0.f,SL11=0.f,SL12=0.f,SL13=0.f,SL14=0.f,SL15=0.f,SL16=0.f,SL17=0.f,SL18=0.f,SL19=0.f,SL20=0.f,
		SL21=0.f,SL22=0.f,SL23=0.f,SL24=0.f,SL25=0.f,SL26=0.f,SL27=0.f,SL28=0.f,SL29=0.f,SL30=0.f,SL31=0.f,SL32=0.f,SL33=0.f,SL34=0.f,SL35=0.f,SL36=0.f,SL37=0.f,SL38=0.f,SL39=0.f,SL40=0.f,
		SL41=0.f,SL42=0.f,SL43=0.f,SL44=0.f,SL45=0.f,SL46=0.f,SL47=0.f,SL48=0.f,SL49=0.f,SL50=0.f,SL51=0.f,SL52=0.f,SL53=0.f,SL54=0.f,SL55=0.f,SL56=0.f,SL57=0.f,SL58=0.f,SL59=0.f,SL60=0.f,
		DJI1=0.f,DJI4=0.f,DJI7=0.f,DJI2=0.f,DJI5=0.f,DJI8=0.f,DJI3=0.f,DJI6=0.f,DJI9=0.f,GG11=0.f,GG22=0.f,GG12=0.f,GGSQ=0.f,FPROD=0.f;
 GP=1.f+G;GM=1.f-G;EP=1.f+E;EM=1.f-E;TP=1.f+T;TM=1.f-T;
 GPP=GP+G;GMM=GM-G;EPP=EP+E;EMM=EM-E;TPP=TP+T;TMM=TM-T;
 if(ISL==1)
	{*DJD=0.f;
	SN[ 0]=GM*EM*TM*(-G-EP-TP)*0.125f;SN[ 1]=GP*EM*TM*(G-EP-TP)*0.125f;SN[ 2]=GP*EP*TM*(G-EM-TP)*0.125f; SN[ 3]=GM*EP*TM*(-G-EM-TP)*0.125f;
	SN[ 4]=GM*EM*TP*(-G-EP-TM)*0.125f;SN[ 5]=GP*EM*TP*(G-EP-TM)*0.125f;SN[ 6]=GP*EP*TP*(G-EM-TM)*0.125f; SN[ 7]=GM*EP*TP*(-G-EM-TM)*0.125f;
	SN[ 8]=EM*TM*GP*GM*0.25f;SN[ 9]=GP*TM*EP*EM*0.25f;SN[10]=EP*TM*GP*GM*0.25f;SN[11]=GM*TM*EP*EM*0.25f;
	SN[12]=GM*EM*TP*TM*0.25f;SN[13]=GP*EM*TP*TM*0.25f;SN[14]=GP*EP*TP*TM*0.25f;SN[15]=GM*EP*TP*TM*0.25f;
		SN[16]=EM*TP*GP*GM*0.25f;SN[17]=GP*TP*EP*EM*0.25f;SN[18]=EP*TP*GP*GM*0.25f;SN[19]=GM*TP*EP*EM*0.25f;
	}
 else if(ISL==2 || ISL==3)
	{SL1 =-EM*TM*(GMM-EP-TP)*0.125f;SL2 =-GM*TM*(EMM-GP-TP)*0.125f;SL3 =-GM*EM*(TMM-GP-EP)*0.125f;SL4 = EM*TM*(GPP-EP-TP)*0.125f;
	SL5 =-GP*TM*(EMM-GM-TP)*0.125f;SL6 =-GP*EM*(TMM-GM-EP)*0.125f;SL7 = EP*TM*(GPP-EM-TP)*0.125f;SL8 = GP*TM*(EPP-GM-TP)*0.125f;
	SL9 =-GP*EP*(TMM-GM-EM)*0.125f;SL10=-EP*TM*(GMM-EM-TP)*0.125f;SL11= GM*TM*(EPP-GP-TP)*0.125f;SL12=-GM*EP*(TMM-GP-EM)*0.125f;
	SL13=-EM*TP*(GMM-EP-TM)*0.125f;SL14=-GM*TP*(EMM-GP-TM)*0.125f;SL15= GM*EM*(TPP-GP-EP)*0.125f;SL16= EM*TP*(GPP-EP-TM)*0.125f;
	SL17=-GP*TP*(EMM-GM-TM)*0.125f;SL18= GP*EM*(TPP-GM-EP)*0.125f;SL19= EP*TP*(GPP-EM-TM)*0.125f;SL20= GP*TP*(EPP-GM-TM)*0.125f;
	SL21= GP*EP*(TPP-GM-EM)*0.125f;SL22=-EP*TP*(GMM-EM-TM)*0.125f;SL23= GM*TP*(EPP-GP-TM)*0.125f;SL24= GM*EP*(TPP-GP-EM)*0.125f;
	SL25=-EM*TM*G*0.5f;SL26=-TM*GP*GM*0.25f;SL27=-EM*GP*GM*0.25f;SL28= TM*EP*EM*0.25f;SL29=-GP*TM*E*0.5f;SL30=-GP*EP*EM*0.25f;SL31=-EP*TM*G*0.5f;SL32= TM*GP*GM*0.25f;
	SL33=-EP*GP*GM*0.25f;SL34=-TM*EP*EM*0.25f;SL35=-GM*TM*E*0.5f;SL36=-GM*EP*EM*0.25f;SL37=-EM*TP*TM*0.25f;SL38=-GM*TP*TM*0.25f;SL39=-GM*EM*T*0.5f;SL40= EM*TP*TM*0.25f;
	SL41=-GP*TP*TM*0.25f;SL42=-GP*EM*T*0.5f;SL43= EP*TP*TM*0.25f;SL44= GP*TP*TM*0.25f;SL45=-GP*EP*T*0.5f;SL46=-EP*TP*TM*0.25f;SL47= GM*TP*TM*0.25f;SL48=-GM*EP*T*0.5f;
	SL49=-EM*TP*G*0.5f;SL50=-TP*GP*GM*0.25f;SL51= EM*GP*GM*0.25f;SL52= TP*EP*EM*0.25f;SL53=-GP*TP*E*0.5f;SL54= GP*EP*EM*0.25f;SL55=-EP*TP*G*0.5f;SL56= TP*GP*GM*0.25f;
	SL57= EP*GP*GM*0.25f;SL58=-TP*EP*EM*0.25f;SL59=-GM*TP*E*0.5f;SL60= GM*EP*EM*0.25f;
	J= -2;
		DJR[3*J+7]=
SL1 *ind_c1[NDF*ind_nop1[ 0]  ]+SL4 *ind_c1[NDF*ind_nop1[ 1]  ]+SL7 *ind_c1[NDF*ind_nop1[ 2]  ]+SL10*ind_c1[NDF*ind_nop1[ 3]  ]+
SL13*ind_c1[NDF*ind_nop1[ 4]  ]+SL16*ind_c1[NDF*ind_nop1[ 5]  ]+SL19*ind_c1[NDF*ind_nop1[ 6]  ]+SL22*ind_c1[NDF*ind_nop1[ 7]  ]+
SL25*ind_c1[NDF*ind_nop1[ 8]  ]+SL28*ind_c1[NDF*ind_nop1[ 9]  ]+SL31*ind_c1[NDF*ind_nop1[10]  ]+SL34*ind_c1[NDF*ind_nop1[11]  ]+
SL37*ind_c1[NDF*ind_nop1[12]  ]+SL40*ind_c1[NDF*ind_nop1[13]  ]+SL43*ind_c1[NDF*ind_nop1[14]  ]+SL46*ind_c1[NDF*ind_nop1[15]  ]+
SL49*ind_c1[NDF*ind_nop1[16]  ]+SL52*ind_c1[NDF*ind_nop1[17]  ]+SL55*ind_c1[NDF*ind_nop1[18]  ]+SL58*ind_c1[NDF*ind_nop1[19]  ];
		DJR[3*J+8]=
SL2 *ind_c1[NDF*ind_nop1[ 0]  ]+SL5 *ind_c1[NDF*ind_nop1[ 1]  ]+SL8 *ind_c1[NDF*ind_nop1[ 2]  ]+SL11*ind_c1[NDF*ind_nop1[ 3]  ]+
SL14*ind_c1[NDF*ind_nop1[ 4]  ]+SL17*ind_c1[NDF*ind_nop1[ 5]  ]+SL20*ind_c1[NDF*ind_nop1[ 6]  ]+SL23*ind_c1[NDF*ind_nop1[ 7]  ]+
SL26*ind_c1[NDF*ind_nop1[ 8]  ]+SL29*ind_c1[NDF*ind_nop1[ 9]  ]+SL32*ind_c1[NDF*ind_nop1[10]  ]+SL35*ind_c1[NDF*ind_nop1[11]  ]+
SL38*ind_c1[NDF*ind_nop1[12]  ]+SL41*ind_c1[NDF*ind_nop1[13]  ]+SL44*ind_c1[NDF*ind_nop1[14]  ]+SL47*ind_c1[NDF*ind_nop1[15]  ]+
SL50*ind_c1[NDF*ind_nop1[16]  ]+SL53*ind_c1[NDF*ind_nop1[17]  ]+SL56*ind_c1[NDF*ind_nop1[18]  ]+SL59*ind_c1[NDF*ind_nop1[19]  ];
		DJR[3*J+9]=
SL3 *ind_c1[NDF*ind_nop1[ 0]  ]+SL6 *ind_c1[NDF*ind_nop1[ 1]  ]+SL9 *ind_c1[NDF*ind_nop1[ 2]  ]+SL12*ind_c1[NDF*ind_nop1[ 3]  ]+
SL15*ind_c1[NDF*ind_nop1[ 4]  ]+SL18*ind_c1[NDF*ind_nop1[ 5]  ]+SL21*ind_c1[NDF*ind_nop1[ 6]  ]+SL24*ind_c1[NDF*ind_nop1[ 7]  ]+
SL27*ind_c1[NDF*ind_nop1[ 8]  ]+SL30*ind_c1[NDF*ind_nop1[ 9]  ]+SL33*ind_c1[NDF*ind_nop1[10]  ]+SL36*ind_c1[NDF*ind_nop1[11]  ]+
SL39*ind_c1[NDF*ind_nop1[12]  ]+SL42*ind_c1[NDF*ind_nop1[13]  ]+SL45*ind_c1[NDF*ind_nop1[14]  ]+SL48*ind_c1[NDF*ind_nop1[15]  ]+
SL51*ind_c1[NDF*ind_nop1[16]  ]+SL54*ind_c1[NDF*ind_nop1[17]  ]+SL57*ind_c1[NDF*ind_nop1[18]  ]+SL60*ind_c1[NDF*ind_nop1[19]  ];
	J= -1;
		DJR[3*J+7]=
SL1 *ind_c1[NDF*ind_nop1[ 0]+1]+SL4 *ind_c1[NDF*ind_nop1[ 1]+1]+SL7 *ind_c1[NDF*ind_nop1[ 2]+1]+SL10*ind_c1[NDF*ind_nop1[ 3]+1]+
SL13*ind_c1[NDF*ind_nop1[ 4]+1]+SL16*ind_c1[NDF*ind_nop1[ 5]+1]+SL19*ind_c1[NDF*ind_nop1[ 6]+1]+SL22*ind_c1[NDF*ind_nop1[ 7]+1]+
SL25*ind_c1[NDF*ind_nop1[ 8]+1]+SL28*ind_c1[NDF*ind_nop1[ 9]+1]+SL31*ind_c1[NDF*ind_nop1[10]+1]+SL34*ind_c1[NDF*ind_nop1[11]+1]+
SL37*ind_c1[NDF*ind_nop1[12]+1]+SL40*ind_c1[NDF*ind_nop1[13]+1]+SL43*ind_c1[NDF*ind_nop1[14]+1]+SL46*ind_c1[NDF*ind_nop1[15]+1]+
SL49*ind_c1[NDF*ind_nop1[16]+1]+SL52*ind_c1[NDF*ind_nop1[17]+1]+SL55*ind_c1[NDF*ind_nop1[18]+1]+SL58*ind_c1[NDF*ind_nop1[19]+1];
		DJR[3*J+8]=
SL2 *ind_c1[NDF*ind_nop1[ 0]+1]+SL5 *ind_c1[NDF*ind_nop1[ 1]+1]+SL8 *ind_c1[NDF*ind_nop1[ 2]+1]+SL11*ind_c1[NDF*ind_nop1[ 3]+1]+
SL14*ind_c1[NDF*ind_nop1[ 4]+1]+SL17*ind_c1[NDF*ind_nop1[ 5]+1]+SL20*ind_c1[NDF*ind_nop1[ 6]+1]+SL23*ind_c1[NDF*ind_nop1[ 7]+1]+
SL26*ind_c1[NDF*ind_nop1[ 8]+1]+SL29*ind_c1[NDF*ind_nop1[ 9]+1]+SL32*ind_c1[NDF*ind_nop1[10]+1]+SL35*ind_c1[NDF*ind_nop1[11]+1]+
SL38*ind_c1[NDF*ind_nop1[12]+1]+SL41*ind_c1[NDF*ind_nop1[13]+1]+SL44*ind_c1[NDF*ind_nop1[14]+1]+SL47*ind_c1[NDF*ind_nop1[15]+1]+
SL50*ind_c1[NDF*ind_nop1[16]+1]+SL53*ind_c1[NDF*ind_nop1[17]+1]+SL56*ind_c1[NDF*ind_nop1[18]+1]+SL59*ind_c1[NDF*ind_nop1[19]+1];
		DJR[3*J+9]=
SL3 *ind_c1[NDF*ind_nop1[ 0]+1]+SL6 *ind_c1[NDF*ind_nop1[ 1]+1]+SL9 *ind_c1[NDF*ind_nop1[ 2]+1]+SL12*ind_c1[NDF*ind_nop1[ 3]+1]+
SL15*ind_c1[NDF*ind_nop1[ 4]+1]+SL18*ind_c1[NDF*ind_nop1[ 5]+1]+SL21*ind_c1[NDF*ind_nop1[ 6]+1]+SL24*ind_c1[NDF*ind_nop1[ 7]+1]+
SL27*ind_c1[NDF*ind_nop1[ 8]+1]+SL30*ind_c1[NDF*ind_nop1[ 9]+1]+SL33*ind_c1[NDF*ind_nop1[10]+1]+SL36*ind_c1[NDF*ind_nop1[11]+1]+
SL39*ind_c1[NDF*ind_nop1[12]+1]+SL42*ind_c1[NDF*ind_nop1[13]+1]+SL45*ind_c1[NDF*ind_nop1[14]+1]+SL48*ind_c1[NDF*ind_nop1[15]+1]+
SL51*ind_c1[NDF*ind_nop1[16]+1]+SL54*ind_c1[NDF*ind_nop1[17]+1]+SL57*ind_c1[NDF*ind_nop1[18]+1]+SL60*ind_c1[NDF*ind_nop1[19]+1];
	J=0;
		DJR[3*J+7]=
SL1 *ind_c1[NDF*ind_nop1[ 0]+2]+SL4 *ind_c1[NDF*ind_nop1[ 1]+2]+SL7 *ind_c1[NDF*ind_nop1[ 2]+2]+SL10*ind_c1[NDF*ind_nop1[ 3]+2]+
SL13*ind_c1[NDF*ind_nop1[ 4]+2]+SL16*ind_c1[NDF*ind_nop1[ 5]+2]+SL19*ind_c1[NDF*ind_nop1[ 6]+2]+SL22*ind_c1[NDF*ind_nop1[ 7]+2]+
SL25*ind_c1[NDF*ind_nop1[ 8]+2]+SL28*ind_c1[NDF*ind_nop1[ 9]+2]+SL31*ind_c1[NDF*ind_nop1[10]+2]+SL34*ind_c1[NDF*ind_nop1[11]+2]+
SL37*ind_c1[NDF*ind_nop1[12]+2]+SL40*ind_c1[NDF*ind_nop1[13]+2]+SL43*ind_c1[NDF*ind_nop1[14]+2]+SL46*ind_c1[NDF*ind_nop1[15]+2]+
SL49*ind_c1[NDF*ind_nop1[16]+2]+SL52*ind_c1[NDF*ind_nop1[17]+2]+SL55*ind_c1[NDF*ind_nop1[18]+2]+SL58*ind_c1[NDF*ind_nop1[19]+2];
		DJR[3*J+8]=
SL2 *ind_c1[NDF*ind_nop1[ 0]+2]+SL5 *ind_c1[NDF*ind_nop1[ 1]+2]+SL8 *ind_c1[NDF*ind_nop1[ 2]+2]+SL11*ind_c1[NDF*ind_nop1[ 3]+2]+
SL14*ind_c1[NDF*ind_nop1[ 4]+2]+SL17*ind_c1[NDF*ind_nop1[ 5]+2]+SL20*ind_c1[NDF*ind_nop1[ 6]+2]+SL23*ind_c1[NDF*ind_nop1[ 7]+2]+
SL26*ind_c1[NDF*ind_nop1[ 8]+2]+SL29*ind_c1[NDF*ind_nop1[ 9]+2]+SL32*ind_c1[NDF*ind_nop1[10]+2]+SL35*ind_c1[NDF*ind_nop1[11]+2]+
SL38*ind_c1[NDF*ind_nop1[12]+2]+SL41*ind_c1[NDF*ind_nop1[13]+2]+SL44*ind_c1[NDF*ind_nop1[14]+2]+SL47*ind_c1[NDF*ind_nop1[15]+2]+
SL50*ind_c1[NDF*ind_nop1[16]+2]+SL53*ind_c1[NDF*ind_nop1[17]+2]+SL56*ind_c1[NDF*ind_nop1[18]+2]+SL59*ind_c1[NDF*ind_nop1[19]+2];
		DJR[3*J+9]=
SL3 *ind_c1[NDF*ind_nop1[ 0]+2]+SL6 *ind_c1[NDF*ind_nop1[ 1]+2]+SL9 *ind_c1[NDF*ind_nop1[ 2]+2]+SL12*ind_c1[NDF*ind_nop1[ 3]+2]+
SL15*ind_c1[NDF*ind_nop1[ 4]+2]+SL18*ind_c1[NDF*ind_nop1[ 5]+2]+SL21*ind_c1[NDF*ind_nop1[ 6]+2]+SL24*ind_c1[NDF*ind_nop1[ 7]+2]+
SL27*ind_c1[NDF*ind_nop1[ 8]+2]+SL30*ind_c1[NDF*ind_nop1[ 9]+2]+SL33*ind_c1[NDF*ind_nop1[10]+2]+SL36*ind_c1[NDF*ind_nop1[11]+2]+
SL39*ind_c1[NDF*ind_nop1[12]+2]+SL42*ind_c1[NDF*ind_nop1[13]+2]+SL45*ind_c1[NDF*ind_nop1[14]+2]+SL48*ind_c1[NDF*ind_nop1[15]+2]+
SL51*ind_c1[NDF*ind_nop1[16]+2]+SL54*ind_c1[NDF*ind_nop1[17]+2]+SL57*ind_c1[NDF*ind_nop1[18]+2]+SL60*ind_c1[NDF*ind_nop1[19]+2];
	if(ISL==2)
	  {*DJD=DJR[1]*(DJR[5]*DJR[9]-DJR[6]*DJR[8])+DJR[2]*(DJR[6]*DJR[7]-DJR[4]*DJR[9])+DJR[3]*(DJR[4]*DJR[8]-DJR[5]*DJR[7]);
	  if(sqrt(*DJD * *DJD)<1.E-30f){
// ::MessageBeep(-1);MessageBox("STFISO8 zero det.");
									   *DJD=1.E-6f;
									  }
	  DJI1=(DJR[5]*DJR[9]-DJR[6]*DJR[8])/ *DJD;DJI4=(DJR[6]*DJR[7]-DJR[4]*DJR[9])/ *DJD;
	  DJI7=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;DJI2=(DJR[3]*DJR[8]-DJR[2]*DJR[9])/ *DJD;
	  DJI5=(DJR[1]*DJR[9]-DJR[3]*DJR[7])/ *DJD;DJI8=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
	  DJI3=(DJR[2]*DJR[6]-DJR[3]*DJR[5])/ *DJD;DJI6=(DJR[3]*DJR[4]-DJR[1]*DJR[6])/ *DJD;
	  DJI9=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;
	  SG[0]=DJI1*SL1+DJI4*SL2+DJI7*SL3;SG[1]=DJI2*SL1+DJI5*SL2+DJI8*SL3;
	  SG[2]=DJI3*SL1+DJI6*SL2+DJI9*SL3;SG[3]=DJI1*SL4+DJI4*SL5+DJI7*SL6;
	  SG[4]=DJI2*SL4+DJI5*SL5+DJI8*SL6;SG[5]=DJI3*SL4+DJI6*SL5+DJI9*SL6;
	  SG[6]=DJI1*SL7+DJI4*SL8+DJI7*SL9;SG[7]=DJI2*SL7+DJI5*SL8+DJI8*SL9;
	  SG[8]=DJI3*SL7+DJI6*SL8+DJI9*SL9;SG[9]=DJI1*SL10+DJI4*SL11+DJI7*SL12;
	  SG[10]=DJI2*SL10+DJI5*SL11+DJI8*SL12;SG[11]=DJI3*SL10+DJI6*SL11+DJI9*SL12;
	  SG[12]=DJI1*SL13+DJI4*SL14+DJI7*SL15;SG[13]=DJI2*SL13+DJI5*SL14+DJI8*SL15;
	  SG[14]=DJI3*SL13+DJI6*SL14+DJI9*SL15;SG[15]=DJI1*SL16+DJI4*SL17+DJI7*SL18;
	  SG[16]=DJI2*SL16+DJI5*SL17+DJI8*SL18;SG[17]=DJI3*SL16+DJI6*SL17+DJI9*SL18;
	  SG[18]=DJI1*SL19+DJI4*SL20+DJI7*SL21;SG[19]=DJI2*SL19+DJI5*SL20+DJI8*SL21;
	  SG[20]=DJI3*SL19+DJI6*SL20+DJI9*SL21;SG[21]=DJI1*SL22+DJI4*SL23+DJI7*SL24;
	  SG[22]=DJI2*SL22+DJI5*SL23+DJI8*SL24;SG[23]=DJI3*SL22+DJI6*SL23+DJI9*SL24;
//
	  SG[24]=DJI1*SL25+DJI4*SL26+DJI7*SL27;SG[25]=DJI2*SL25+DJI5*SL26+DJI8*SL27;
	  SG[26]=DJI3*SL25+DJI6*SL26+DJI9*SL27;SG[27]=DJI1*SL28+DJI4*SL29+DJI7*SL30;
	  SG[28]=DJI2*SL28+DJI5*SL29+DJI8*SL30;SG[29]=DJI3*SL28+DJI6*SL29+DJI9*SL30;
	  SG[30]=DJI1*SL31+DJI4*SL32+DJI7*SL33;SG[31]=DJI2*SL31+DJI5*SL32+DJI8*SL33;
	  SG[32]=DJI3*SL31+DJI6*SL32+DJI9*SL33;SG[33]=DJI1*SL34+DJI4*SL35+DJI7*SL36;
	  SG[34]=DJI2*SL34+DJI5*SL35+DJI8*SL36;SG[35]=DJI3*SL34+DJI6*SL35+DJI9*SL36;
	  SG[36]=DJI1*SL37+DJI4*SL38+DJI7*SL39;SG[37]=DJI2*SL37+DJI5*SL38+DJI8*SL39;
	  SG[38]=DJI3*SL37+DJI6*SL38+DJI9*SL39;SG[39]=DJI1*SL40+DJI4*SL41+DJI7*SL42;
	  SG[40]=DJI2*SL40+DJI5*SL41+DJI8*SL42;SG[41]=DJI3*SL40+DJI6*SL41+DJI9*SL42;
	  SG[42]=DJI1*SL43+DJI4*SL44+DJI7*SL45;SG[43]=DJI2*SL43+DJI5*SL44+DJI8*SL45;
	  SG[44]=DJI3*SL43+DJI6*SL44+DJI9*SL45;SG[45]=DJI1*SL46+DJI4*SL47+DJI7*SL48;
	  SG[46]=DJI2*SL46+DJI5*SL47+DJI8*SL48;SG[47]=DJI3*SL46+DJI6*SL47+DJI9*SL48;
	  SG[48]=DJI1*SL49+DJI4*SL50+DJI7*SL51;SG[49]=DJI2*SL49+DJI5*SL50+DJI8*SL51;
	  SG[50]=DJI3*SL49+DJI6*SL50+DJI9*SL51;SG[51]=DJI1*SL52+DJI4*SL53+DJI7*SL54;
	  SG[52]=DJI2*SL52+DJI5*SL53+DJI8*SL54;SG[53]=DJI3*SL52+DJI6*SL53+DJI9*SL54;
	  SG[54]=DJI1*SL55+DJI4*SL56+DJI7*SL57;SG[55]=DJI2*SL55+DJI5*SL56+DJI8*SL57;
	  SG[56]=DJI3*SL55+DJI6*SL56+DJI9*SL57;SG[57]=DJI1*SL58+DJI4*SL59+DJI7*SL60;
	  SG[58]=DJI2*SL58+DJI5*SL59+DJI8*SL60;SG[59]=DJI3*SL58+DJI6*SL59+DJI9*SL60;
	  }
	else
	  {FPROD=1.f;
		if     (sqrt((G*G-1.f)*(G*G-1.f)) < PRMT){I=1;if(G<0)FPROD= -1.f;}
		else if(sqrt((E*E-1.f)*(E*E-1.f)) < PRMT){I=2;if(E<0)FPROD= -1.f;}
		else if(sqrt((T*T-1.f)*(T*T-1.f)) < PRMT){I=3;if(T<0)FPROD= -1.f;}
		else{
//  ::MessageBeep(-1);MessageBox("STFISO8 invalid surface pt.");
			  exit(0);
			 }
		IP= -2+I*(11-3*I)/2;IM=6-I-IP;
		GG11=DJR[IP]*DJR[IP]+DJR[IP+3]*DJR[IP+3]+DJR[IP+6]*DJR[IP+6];
		GG22=DJR[IM]*DJR[IM]+DJR[IM+3]*DJR[IM+3]+DJR[IM+6]*DJR[IM+6];
		GG12=DJR[IP]*DJR[IM]+DJR[IP+3]*DJR[IM+3]+DJR[IP+6]*DJR[IM+6];
		*DJD=GG11*GG22-GG12*GG12;
	  if(sqrt(*DJD * *DJD)<1.E-30f){
//  ::MessageBeep(-1);MessageBox("STFISO8 zero surf.det.");
									   *DJD=1.E-6f;
									  }
	  else *DJD=sqrt(*DJD);
	  GGSQ=1.f/sqrt(GG11);
	  HN[0]=(DJR[IP+3]*DJR[IM+6]-DJR[IP+6]*DJR[IM+3])/ *DJD;
	  HN[1]=(DJR[IP+6]*DJR[IM  ]-DJR[IP  ]*DJR[IM+6])/ *DJD;
	  HN[2]=(DJR[IP  ]*DJR[IM+3]-DJR[IP+3]*DJR[IM  ])/ *DJD;
	  HN[3]=GGSQ*DJR[IP  ];HN[4]=GGSQ*DJR[IP+3];HN[5]=GGSQ*DJR[IP+6];
	  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
	  HN[8]=HN[0]*HN[4]-HN[1]*HN[3];
	  for(int IC=0;IC<9;IC++)HN[IC]=FPROD*HN[IC];
	}
	 }
		else{
//  ::MessageBeep(-1);MessageBox("STFISO8 unsupported ISL");
			  exit(0);
			 }
}
//---------------------------------------------------------------------------
void TForm1::STFISO15(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//CCCCC Note: G==L1, E==L2, T==natural z coord (=t), L3=1.-L1-L2
//
//   DJR(I,J)= dx/dL1  dy/dL1  dz/dL1  based on L1/L2/1-L1-L2 coords
//             dx/dL2  dy/dL2  dz/dL2  based on L1/L2/1-L1-L2 coords
//             dx/dt   dy/dt   dz/dt   based on L1/L2/1-L1-L2 coords
//
//   However, to compute surface normals it is necessary to use the
//   appropriate basis system on each side (ie. L1/L2 on side L1=0,
//   L1/L2 on side L2=0, L1/L3 on side L3=0)
//
//   Hence the array SLP(I)= dN(L1,L3)/dL1 and DJR6(I,J) thus:
//
//  DJR6(I,J)= dx/dL1  dy/dL1  dz/dL1  based on L1/1-L1-L3/L3 coords
//             dx/dL1  dy/dL1  dz/dL1  based on L1/L2/1-L1-L2 coords
//             dx/dL2  dy/dL2  dz/dL2  based on L1/L2/1-L1-L2 coords
//  	       dx/dt   dy/dt   dz/dt   based on L1/L2/1-L1-L2 coords
//
//  Note that DJD/DJD are RECTANGULAR areas/volumes ...NOT triangular.
//  See User Manual for element surface coord.frame & note that this
//  frame varies drastically on triangular sides....
{int I=0,IM=0,IP=0,J=0,K=0,NODE=15;
//      DIMENSION HN(3),SN(1),SG(3,8),CORD(NDF,1),DJR(3,3),NOP(MXNPEL)
 float PRMT=1.E-10f,GG11=0.f,GG22=0.f,GG12=0.f,WSGN=0.f,FPROD=0.f,S=0.f,SUM=0.f,
		SL[45],DJI[9],SLP[15],DJR6[12];
 S=1.f-G-E;
 if(ISL==1)
	{*DJD=0.f;
	SN[0]=G*(2.f*G-T-2.f)*(1.f-T)*0.5f;
	SN[1]=E*(2.f*E-T-2.f)*(1.f-T)*0.5f;
	SN[2]=S*(2.f*S-T-2.f)*(1.f-T)*0.5f;
	SN[3]=G*(2.f*G+T-2.f)*(1.f+T)*0.5f;
	SN[4]=E*(2.f*E+T-2.f)*(1.f+T)*0.5f;
	SN[5]=S*(2.f*S+T-2.f)*(1.f+T)*0.5f;
	SN[6]=2.f*G*E*(1.f-T);
	SN[7]=2.f*E*S*(1.f-T);
	SN[8]=2.f*G*S*(1.f-T);
	SN[9]=G*(1.f-T*T);
	SN[10]=E*(1.f-T*T);
	SN[11]=S*(1.f-T*T);
	SN[12]=2.f*G*E*(1.f+T);
	SN[13]=2.f*E*S*(1.f+T);
	SN[14]=2.f*G*S*(1.f+T);
	}
 else if(ISL==2 || ISL==3)
	{
	SL[0]=(4.f*G-T-2.f)*(1.f-T)/2.f;
	SL[1]=0.f;
	SL[2]=G*(1.f-2.f*G+2.f*T)/2.f;
	SL[3]=0.f;
	SL[4]=(4.f*E-T-2.f)*(1.f-T)/2.f;
	SL[5]=E*(1.f-2.f*E+2.f*T)/2.f;
	SL[6]=(2.f-4.f*S+T)*(1.f-T)/2.f;
	SL[7]=(2.f-4.f*S+T)*(1.f-T)/2.f;
	SL[8]=S*(1.f-2.f*S+2.f*T)/2.f;
	SL[9]=(4.f*G+T-2.f)*(1.f+T)/2.f;
	SL[10]=0.f;
	SL[11]=G*(-1.f+2.f*G+2.f*T)/2.f;
	SL[12]=0.f;
	SL[13]=(4.f*E+T-2.f)*(1.f+T)/2.f;
	SL[14]=E*(-1.f+2.f*E+2.f*T)/2.f;
	SL[15]=(2.f-4.f*S-T)*(1.f+T)/2.f;
	SL[16]=(2.f-4.f*S-T)*(1.f+T)/2.f;
	SL[17]=S*(-1.f+2.f*S+2.f*T)/2.f;
	SL[18]=2.f*E*(1.f-T);
	SL[19]=2.f*G*(1.f-T);
	SL[20]= -2.f*G*E;
	SL[21]= -2.f*E*(1.f-T);
	SL[22]=2.f*(S-E)*(1.f-T);
	SL[23]= -2.f*E*S;
	SL[24]=2.f*(S-G)*(1.f-T);
	SL[25]= -2.f*G*(1.f-T);
	SL[26]= -2.f*G*S;
	SL[27]=1.f-T*T;
	SL[28]=0.f;
	SL[29]= -2.f*G*T;
	SL[30]=0.f;
	SL[31]=1.f-T*T;
	SL[32]= -2.f*E*T;
	SL[33]= -(1.f-T*T);
	SL[34]= -(1.f-T*T);
	SL[35]= -2.f*S*T;
	SL[36]=2.f*E*(1.f+T);
	SL[37]=2.f*G*(1.f+T);
	SL[38]=2.f*G*E;
	SL[39]= -2.f*E*(1.f+T);
	SL[40]=2.f*(S-E)*(1.f+T);
	SL[41]=2.f*E*S;
	SL[42]=2.f*(S-G)*(1.f+T);
	SL[43]= -2.f*G*(1.f+T);
	SL[44]=2.f*G*S;
// NOTE NOTE NOTE DJR[1 to 9]  !!!!!!!!!!!!!!
		for(J=0;J<3;J++){for(I=0;I<3;I++){SUM=0.f;
										  for(K=0;K<NODE;K++)SUM=SUM+SL[I+3*K]*ind_c1[NDF*ind_nop1[K]+J];
                                          DJR[I+3*J+1]=SUM;
										 }
						}
	if(ISL==2)
	  {*DJD=DJR[1]*(DJR[5]*DJR[9]-DJR[6]*DJR[8])+DJR[2]*(DJR[6]*DJR[7]-DJR[4]*DJR[9])
			+DJR[3]*(DJR[4]*DJR[8]-DJR[5]*DJR[7]);
	  if(sqrt(*DJD * *DJD)<1.E-30f){
// ::MessageBeep(-1);MessageBox("STFISO8 zero det.");
									   *DJD=1.E-6f;
									  }
	  DJI[0]=(DJR[5]*DJR[9]-DJR[6]*DJR[8])/ *DJD;
	  DJI[3]=(DJR[6]*DJR[7]-DJR[4]*DJR[9])/ *DJD;
	  DJI[6]=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;
	  DJI[1]=(DJR[3]*DJR[8]-DJR[2]*DJR[9])/ *DJD;
	  DJI[4]=(DJR[1]*DJR[9]-DJR[3]*DJR[7])/ *DJD;
	  DJI[7]=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
	  DJI[2]=(DJR[2]*DJR[6]-DJR[3]*DJR[5])/ *DJD;
	  DJI[5]=(DJR[3]*DJR[4]-DJR[1]*DJR[6])/ *DJD;
	  DJI[8]=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;
		  for(J=3;J<3*NODE+1;J=J+3){for(I=0;I<3;I++)SG[I+J-3]=DJI[I]*SL[J-3]+DJI[I+3]*SL[J-2]+DJI[I+6]*SL[J-1];}
	  }
	else
	  {
	  SLP[0]=(4.f*G-T-2.f)*(1.f-T)/2.f;
	  SLP[1]=(2.f-4.f*E+T)*(1.f-T)/2.f;
	  SLP[2]=0.f;
	  SLP[3]=(4.f*G+T-2.f)*(1.f+T)/2.f;
	  SLP[4]=(2.f-4.f*E-T)*(1.f+T)/2.f;
	  SLP[5]=0.f;
	  SLP[6]=2.f*(E-G)*(1.f-T);
	  SLP[7]= -2.f*S*(1.f-T);
	  SLP[8]=2.f*S*(1.f-T);
	  SLP[9]=1.f-T*T;
	  SLP[10]= -(1.f-T*T);
	  SLP[11]=0.f;
	  SLP[12]=2.f*(E-G)*(1.f+T);
	  SLP[13]= -2.f*S*(1.f+T);
	  SLP[14]=2.f*S*(1.f+T);
          for(J=1;J<3+1;J++){for(I=1;I<3+1;I++)DJR6[1-I+4*J  -1]=DJR[1-I+3*J];}
		  for(J=1;J<3+1;J++){SUM=0.f;
                             for(K=0;K<NODE;K++)SUM=SUM+SLP[K]*ind_c1[NDF*ind_nop1[K]+J-1];
                             DJR6[4*J-3  -1]=SUM;
							}
	  WSGN=1.f;
		   FPROD=1.f;
	  if(fabs(T*T-1.f)>PRMT){IP=2;IM=3;if(T<0.f)FPROD= -1.f;}
	  else {IM=4;
			if(fabs(G)<PRMT){IP=3;WSGN= -1.f;}
			else if(fabs(E)<PRMT)IP=2;
			else if(fabs(1.f-G-E)<PRMT){IP=1;WSGN= -1.f;}
  	        else{
//  ::MessageBeep(-1);MessageBox("STFISO8 invalid surface pt.");
			  exit(0);
				}
		   }
      GG11=DJR6[IP-1]*DJR6[IP-1]+DJR6[IP+4-1]*DJR6[IP+4-1]+DJR6[IP+8-1]*DJR6[IP+8-1];
	  GG22=DJR6[IM-1]*DJR6[IM-1]+DJR6[IM+4-1]*DJR6[IM+4-1]+DJR6[IM+8-1]*DJR6[IM+8-1];
      GG12=DJR6[IP-1]*DJR6[IM-1]+DJR6[IP+4-1]*DJR6[IM+4-1]+DJR6[IP+8-1]*DJR6[IM+8-1];
	  *DJD=GG11*GG22-GG12*GG12;
	  if(sqrt(*DJD * *DJD)<1.E-30f){
//  ::MessageBeep(-1);MessageBox("STFISO8 zero surf.det.");
									   *DJD=1.E-6f;
                                      }
	  else *DJD=sqrt(*DJD);
	  HN[0]=WSGN*(DJR6[IP+4-1]*DJR6[IM+8-1]-DJR6[IP+8-1]*DJR6[IM+4-1])/ *DJD;
	  HN[1]=WSGN*(DJR6[IP+8-1]*DJR6[IM  -1]-DJR6[IP  -1]*DJR6[IM+8-1])/ *DJD;
	  HN[2]=WSGN*(DJR6[IP  -1]*DJR6[IM+4-1]-DJR6[IP+4-1]*DJR6[IM  -1])/ *DJD;
	  HN[3]=WSGN*DJR6[IP  -1]/sqrt(GG11);
	  HN[4]=WSGN*DJR6[IP+4-1]/sqrt(GG11);
	  HN[5]=WSGN*DJR6[IP+8-1]/sqrt(GG11);
	  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
	  HN[8]=HN[0]*HN[4]-HN[1]*HN[3];
	  for(int IC=0;IC<9;IC++)HN[IC]=FPROD*HN[IC];
	}
	 }
		else{
//  ::MessageBeep(-1);MessageBox("STFISO8 unsupported ISL");
			  exit(0);
			 }
}
//---------------------------------------------------------------------------
void TForm1::STFISO10(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//  See User Manual for element surface coord.frame.
//  On positive sides: n x t1=t2; on negative HN()= -HN(positive side)
//  so n x t1 = -t2 on negative sides (ie. NOT corkscrew).
//  Normal=HN(1 to 3), t1=HN(4 to 6), t2=HN(7 to 9) so      x = HN.x'
//  CAUTION: do NOT evaluate normals at edges/nodes due to ambiguity!!
//CCCCCC Note: G==L1, E==L2, T==natural z coord (=t), L3=1.-L1-L2
//C
//C   DJR(I,J)= dx/dL2  dy/dL2  dz/dL2  based on L1/L2/L3/1-L1-L2-L3 coords
//C             dx/dL3  dy/dL3  dz/dL3  based on L1/L2/L3/1-L1-L2-L3 coords
//C             dx/dL4  dy/dL4  dz/dL4  based on L1/L2/L3/1-L1-L2-L3 coords
//C
//C   However, to compute surface normals it is necessary to use the
//C   appropriate basis system on each side (ie. L1/L2 on side L1=0,
//C   L1/L2 on side L2=0, L1/L3 on side L3=0)
//C
//C  Note that DJD/DJD are true TETRA areas/volumes ...NOT rectangular.
{int I=0,IM=0,IP=0,J=0,K=0,NODE=10;
//      DIMENSION HN(3),SN(1),SG(3,8),CORD(NDF,1),DJR(3,3),NOP(MXNPEL)
 float PRMT=1.E-10f,PMSURF=1.E-8f,GG11=0.f,GG22=0.f,GG12=0.f,GGSQ=0.f,S=0.f,SUM=0.f,SL[30],DJI[9];
 S=1.f-G-E-T;
 if(ISL==1)
	{*DJD=0.f;
	SN[0]=G*(2.f*G-1.f);
	SN[1]=E*(2.f*E-1.f);
	SN[2]=T*(2.f*T-1.f);
	SN[3]=S*(2.f*S-1.f);
	SN[4]=4.f*G*E;
	SN[5]=4.f*E*T;
	SN[6]=4.f*G*T;
	SN[7]=4.f*G*S;
	SN[8]=4.f*E*S;
	SN[9]=4.f*T*S;
	}
 else if(ISL==2 || ISL==3)
	{
	SL[0]=1.f-4.f*G;
	SL[1]=1.f-4.f*G;
	SL[2]=1.f-4.f*G;
	SL[3]=4.f*E-1.f;
	SL[4]=0.f;
	SL[5]=0.f;
	SL[6]=0.f;
	SL[7]=4.f*T-1.f;
	SL[8]=0.f;
	SL[9]=0.f;
	SL[10]=0.f;
	SL[11]=4.f*S-1.f;
	SL[12]=4.f*(G-E);
	SL[13]= -4.f*E;
	SL[14]= -4.f*E;
	SL[15]=4.f*T;
	SL[16]=4.f*E;
	SL[17]=0.f;
	SL[18]= -4.f*T;
	SL[19]=4.f*(G-T);
	SL[20]= -4.f*T;
	SL[21]= -4.f*S;
	SL[22]= -4.f*S;
	SL[23]=4.f*(G-S);
	SL[24]=4.f*S;
	SL[25]=0.f;
	SL[26]=4.f*E;
	SL[27]=0.f;
	SL[28]=4.f*S;
	SL[29]=4.f*T;
// NOTE NOTE NOTE DJR[1 to 9]  !!!!!!!!!!!!!!
		for(J=0;J<3;J++){for(I=0;I<3;I++){SUM=0.f;
										  for(K=0;K<NODE;K++)SUM=SUM+SL[I+3*K]*ind_c1[NDF*ind_nop1[K]+J];
										  DJR[I+3*J+1]=SUM;
                                         }
                        }
	if(ISL==2)
	  {*DJD=DJR[1]*(DJR[5]*DJR[9]-DJR[6]*DJR[8])+DJR[2]*(DJR[6]*DJR[7]-DJR[4]*DJR[9])
			+DJR[3]*(DJR[4]*DJR[8]-DJR[5]*DJR[7]);
	  if(sqrt(*DJD * *DJD)<1.E-30f){
// ::MessageBeep(-1);MessageBox("STFISO8 zero det.");
									   *DJD=1.E-6f;
									  }
	  DJI[0]=(DJR[5]*DJR[9]-DJR[6]*DJR[8])/ *DJD;
	  DJI[3]=(DJR[6]*DJR[7]-DJR[4]*DJR[9])/ *DJD;
	  DJI[6]=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;
	  DJI[1]=(DJR[3]*DJR[8]-DJR[2]*DJR[9])/ *DJD;
	  DJI[4]=(DJR[1]*DJR[9]-DJR[3]*DJR[7])/ *DJD;
	  DJI[7]=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
	  DJI[2]=(DJR[2]*DJR[6]-DJR[3]*DJR[5])/ *DJD;
	  DJI[5]=(DJR[3]*DJR[4]-DJR[1]*DJR[6])/ *DJD;
	  DJI[8]=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;
          for(J=3;J<3*NODE+1;J=J+3){for(I=0;I<3;I++)SG[I+J-3]=DJI[I]*SL[J-3]+DJI[I+3]*SL[J-2]+DJI[I+6]*SL[J-1];}
	  *DJD= *DJD/6.f;
	  }
	  else
		 {if(fabs(S)<PRMT)
			 {IP=2;IM=3;HN[0]= -DJR[IP+3]*DJR[IM+6]+DJR[IP+6]*DJR[IM+3];
			  HN[1]= -DJR[IP+6]*DJR[IM  ]+DJR[IP  ]*DJR[IM+6];
			  HN[2]= -DJR[IP  ]*DJR[IM+3]+DJR[IP+3]*DJR[IM  ];
			  IP=3;IM=1;HN[0]=HN[0]-DJR[IP+3]*DJR[IM+6]+DJR[IP+6]*DJR[IM+3];
			  HN[1]=HN[1]-DJR[IP+6]*DJR[IM  ]+DJR[IP  ]*DJR[IM+6];
			  HN[2]=HN[2]-DJR[IP  ]*DJR[IM+3]+DJR[IP+3]*DJR[IM  ];
			  IP=1;IM=2;HN[0]=HN[0]-DJR[IP+3]*DJR[IM+6]+DJR[IP+6]*DJR[IM+3];
			  HN[1]=HN[1]-DJR[IP+6]*DJR[IM  ]+DJR[IP  ]*DJR[IM+6];
			  HN[2]=HN[2]-DJR[IP  ]*DJR[IM+3]+DJR[IP+3]*DJR[IM  ];
			  *DJD=HN[0]*HN[0]+HN[1]*HN[1]+HN[2]*HN[2];
			  if(*DJD<PMSURF*PMSURF){
//  ::MessageBeep(-1);MessageBox("STFISO10 zero surf.det.");
												 *DJD=1.E-6f;
												}
			  else *DJD=sqrt(*DJD);
			  HN[0]=HN[0]/ *DJD;HN[1]=HN[1]/ *DJD;HN[2]=HN[2]/ *DJD;
			  HN[3]=DJR[3]-DJR[1];HN[4]=DJR[6]-DJR[4];HN[5]=DJR[9]-DJR[7];
			  GGSQ=1.f/sqrt(HN[3]*HN[3]+HN[4]*HN[4]+HN[5]*HN[5]);
			  HN[3]=GGSQ*HN[3];HN[4]=GGSQ*HN[4];HN[5]=GGSQ*HN[5];
			  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
			  HN[8]=HN[0]*HN[4]-HN[1]*HN[3]; *DJD= *DJD/2.f;
			 }
		  else
			 {if(fabs(G)<PRMT){IP=2;IM=3;}
			  else if(fabs(E)<PRMT){IP=3;IM=1;}
			  else if(fabs(T)<PRMT){IP=1;IM=2;}
			  else{
//  ::MessageBeep(-1);MessageBox("STFISO10 invalid surface pt.");
					 exit(0);
					}
			  GG11=DJR[IP]*DJR[IP]+DJR[IP+3]*DJR[IP+3]+DJR[IP+6]*DJR[IP+6];
			  GG22=DJR[IM]*DJR[IM]+DJR[IM+3]*DJR[IM+3]+DJR[IM+6]*DJR[IM+6];
			  GG12=DJR[IP]*DJR[IM]+DJR[IP+3]*DJR[IM+3]+DJR[IP+6]*DJR[IM+6];
			  *DJD=GG11*GG22-GG12*GG12;
			  if(*DJD<PMSURF*PMSURF){
//  ::MessageBeep(-1);MessageBox("STFISO10 zero surf.det.");
												 *DJD=1.E-6f;
                                                }
			  else *DJD=sqrt(*DJD);
			  GGSQ=1.f/sqrt(GG11);
			  HN[0]=(DJR[IP+3]*DJR[IM+6]-DJR[IP+6]*DJR[IM+3])/ *DJD;
			  HN[1]=(DJR[IP+6]*DJR[IM  ]-DJR[IP  ]*DJR[IM+6])/ *DJD;
			  HN[2]=(DJR[IP  ]*DJR[IM+3]-DJR[IP+3]*DJR[IM  ])/ *DJD;
			  HN[3]=GGSQ*DJR[IP  ];HN[4]=GGSQ*DJR[IP+3];HN[5]=GGSQ*DJR[IP+6];
			  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
			  HN[8]=HN[0]*HN[4]-HN[1]*HN[3]; *DJD= *DJD/2.f;
			 }
		 }
	 }
  else{
//  ::MessageBeep(-1);MessageBox("STFISO10 unsupported ISL");
		 exit(0);
		}
}
//---------------------------------------------------------------------------
void TForm1::STFISO8_ncalc(long ie,long iside,float HN[],long ind_nop1[],float ind_c1[])
{long n0=0,n1=0,n2=0,n3=0;float a0=0.f,a1=0.f,a2=0.f,b0=0.f,b1=0.f,b2=0.f,c0=0.f,c1=0.f,c2=0.f,mag=0.f;
 if(iside==0)     {n0=4;n2=1;n1=0;n3=5;}//OK
 else if(iside==1){n0=5;n2=2;n1=1;n3=6;}//OK
 else if(iside==2){n0=6;n2=3;n1=2;n3=7;}//OK
 else if(iside==3){n0=7;n2=0;n1=3;n3=4;}//OK
 else if(iside==4){n0=0;n2=2;n1=3;n3=1;}//OK
 else             {n0=7;n2=5;n1=4;n3=6;}//OK
 a0=ind_c1[NDF*ind_nop1[MXNPEL*ie+n2]+0] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n0]+0];
 a1=ind_c1[NDF*ind_nop1[MXNPEL*ie+n2]+1] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n0]+1];
 a2=ind_c1[NDF*ind_nop1[MXNPEL*ie+n2]+2] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n0]+2];
 mag=sqrt(a0*a0+a1*a1+a2*a2);a0=a0/mag;a1=a1/mag;a2=a2/mag;
 b0=ind_c1[NDF*ind_nop1[MXNPEL*ie+n3]+0] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n1]+0];
 b1=ind_c1[NDF*ind_nop1[MXNPEL*ie+n3]+1] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n1]+1];
 b2=ind_c1[NDF*ind_nop1[MXNPEL*ie+n3]+2] -ind_c1[NDF*ind_nop1[MXNPEL*ie+n1]+2];
 mag=sqrt(b0*b0+b1*b1+b2*b2);b0=b0/mag;b1=b1/mag;b2=b2/mag;

//honk<<a0<<" "<<a1<<" "<<a2<<" Protagonists "<<b0<<" "<<b1<<" "<<b2<<"\n";

 c0=a1*b2-a2*b1;c1=a2*b0-a0*b2;c2=a0*b1-a1*b0;
 mag=sqrt(c0*c0+c1*c1+c2*c2);HN[0]=c0/mag;HN[1]=c1/mag;HN[2]=c2/mag;
}
//---------------------------------------------------------------------------
void TForm1::STFISO8(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//  See User Manual for element surface coord.frame.
//  On positive sides: n x t1=t2; on negative HN()= -HN(positive side)
//  so n x t1 = -t2 on negative sides (ie. NOT corkscrew).
//  Normal=HN(1 to 3), t1=HN(4 to 6), t2=HN(7 to 9) so      x = HN.x'
//  CAUTION: do NOT evaluate normals at edges/nodes due to ambiguity!!
// CAUTION: a surface point MUST be exactly on surface (i.e. one natural coord = +/-1.)
{int I=0,IM=0,IP=0,J=0;
//      DIMENSION HN(3),SN(1),SG(3,8),CORD(NDF,1),DJR(3,3),NOP(MXNPEL)
 float PRMT=1.E-5f,GP=0.f,GM=0.f,EP=0.f,EM=0.f,TP=0.f,TM=0.f,
   SL1=0.f,SL4=0.f,SL7=0.f,SL10=0.f,SL13=0.f,SL16=0.f,SL19=0.f,SL22=0.f,SL2=0.f,SL5=0.f,SL8=0.f,SL11=0.f,
   SL14=0.f,SL17=0.f,SL20=0.f,SL23=0.f,SL3=0.f,SL6=0.f,SL9=0.f,SL12=0.f,SL15=0.f,SL18=0.f,SL21=0.f,SL24=0.f,
   DJI1=0.f,DJI4=0.f,DJI7=0.f,DJI2=0.f,DJI5=0.f,DJI8=0.f,DJI3=0.f,DJI6=0.f,DJI9=0.f,GG11=0.f,GG22=0.f,GG12=0.f,GGSQ=0.f,FPROD=0.f;
 GP=0.5f*(1.f+G);GM=0.5f*(1.f-G);EP=0.5f*(1.f+E);EM=0.5f*(1.f-E);TP=0.5f*(1.f+T);TM=0.5f*(1.f-T);
 if(ISL==1){*DJD=0.f;SN[0]=GM*EM*TM;SN[1]=GP*EM*TM;SN[2]=GP*EP*TM;SN[3]=GM*EP*TM;
				SN[4]=GM*EM*TP;SN[5]=GP*EM*TP;SN[6]=GP*EP*TP;SN[7]=GM*EP*TP;
	   }
 else if(ISL==2 || ISL==3)
	{SL1=-EM*TM;SL4= EM*TM;SL7= EP*TM;SL10=-EP*TM;SL13=-EM*TP;SL16= EM*TP;
	SL19= EP*TP;SL22=-EP*TP;SL2=-GM*TM;SL5=-GP*TM;SL8= GP*TM;SL11= GM*TM;
	SL14=-GM*TP;SL17=-GP*TP;SL20= GP*TP;SL23= GM*TP;SL3=-GM*EM;SL6=-GP*EM;
	SL9=-GP*EP;SL12=-GM*EP;SL15= GM*EM;SL18= GP*EM;SL21= GP*EP;SL24= GM*EP;
	J= -2;  DJR[3*J+6]=0.5f*(
SL1 *ind_c1[NDF*ind_nop1[0]  ]+SL4 *ind_c1[NDF*ind_nop1[1]  ]+
SL7 *ind_c1[NDF*ind_nop1[2]  ]+SL10*ind_c1[NDF*ind_nop1[3]  ]+
SL13*ind_c1[NDF*ind_nop1[4]  ]+SL16*ind_c1[NDF*ind_nop1[5]  ]+
SL19*ind_c1[NDF*ind_nop1[6]  ]+SL22*ind_c1[NDF*ind_nop1[7]  ]);
		DJR[3*J+7]=0.5f*(
SL2 *ind_c1[NDF*ind_nop1[0]  ]+SL5 *ind_c1[NDF*ind_nop1[1]  ]+
SL8 *ind_c1[NDF*ind_nop1[2]  ]+SL11*ind_c1[NDF*ind_nop1[3]  ]+
SL14*ind_c1[NDF*ind_nop1[4]  ]+SL17*ind_c1[NDF*ind_nop1[5]  ]+
SL20*ind_c1[NDF*ind_nop1[6]  ]+SL23*ind_c1[NDF*ind_nop1[7]  ]);
		DJR[3*J+8]=0.5f*(
SL3 *ind_c1[NDF*ind_nop1[0]  ]+SL6 *ind_c1[NDF*ind_nop1[1]  ]+
SL9 *ind_c1[NDF*ind_nop1[2]  ]+SL12*ind_c1[NDF*ind_nop1[3]  ]+
SL15*ind_c1[NDF*ind_nop1[4]  ]+SL18*ind_c1[NDF*ind_nop1[5]  ]+
SL21*ind_c1[NDF*ind_nop1[6]  ]+SL24*ind_c1[NDF*ind_nop1[7]  ]);
	J= -1;  DJR[3*J+6]=0.5f*(
SL1 *ind_c1[NDF*ind_nop1[0]+1]+SL4 *ind_c1[NDF*ind_nop1[1]+1]+
SL7 *ind_c1[NDF*ind_nop1[2]+1]+SL10*ind_c1[NDF*ind_nop1[3]+1]+
SL13*ind_c1[NDF*ind_nop1[4]+1]+SL16*ind_c1[NDF*ind_nop1[5]+1]+
SL19*ind_c1[NDF*ind_nop1[6]+1]+SL22*ind_c1[NDF*ind_nop1[7]+1]);
		DJR[3*J+7]=0.5f*(
SL2 *ind_c1[NDF*ind_nop1[0]+1]+SL5 *ind_c1[NDF*ind_nop1[1]+1]+
SL8 *ind_c1[NDF*ind_nop1[2]+1]+SL11*ind_c1[NDF*ind_nop1[3]+1]+
SL14*ind_c1[NDF*ind_nop1[4]+1]+SL17*ind_c1[NDF*ind_nop1[5]+1]+
SL20*ind_c1[NDF*ind_nop1[6]+1]+SL23*ind_c1[NDF*ind_nop1[7]+1]);
		DJR[3*J+8]=0.5f*(
SL3 *ind_c1[NDF*ind_nop1[0]+1]+SL6 *ind_c1[NDF*ind_nop1[1]+1]+
SL9 *ind_c1[NDF*ind_nop1[2]+1]+SL12*ind_c1[NDF*ind_nop1[3]+1]+
SL15*ind_c1[NDF*ind_nop1[4]+1]+SL18*ind_c1[NDF*ind_nop1[5]+1]+
SL21*ind_c1[NDF*ind_nop1[6]+1]+SL24*ind_c1[NDF*ind_nop1[7]+1]);
	J=0;    DJR[3*J+6]=0.5f*(
SL1 *ind_c1[NDF*ind_nop1[0]+2]+SL4 *ind_c1[NDF*ind_nop1[1]+2]+
SL7 *ind_c1[NDF*ind_nop1[2]+2]+SL10*ind_c1[NDF*ind_nop1[3]+2]+
SL13*ind_c1[NDF*ind_nop1[4]+2]+SL16*ind_c1[NDF*ind_nop1[5]+2]+
SL19*ind_c1[NDF*ind_nop1[6]+2]+SL22*ind_c1[NDF*ind_nop1[7]+2]);
		DJR[3*J+7]=0.5f*(
SL2 *ind_c1[NDF*ind_nop1[0]+2]+SL5 *ind_c1[NDF*ind_nop1[1]+2]+
SL8 *ind_c1[NDF*ind_nop1[2]+2]+SL11*ind_c1[NDF*ind_nop1[3]+2]+
SL14*ind_c1[NDF*ind_nop1[4]+2]+SL17*ind_c1[NDF*ind_nop1[5]+2]+
SL20*ind_c1[NDF*ind_nop1[6]+2]+SL23*ind_c1[NDF*ind_nop1[7]+2]);
		DJR[3*J+8]=0.5f*(
SL3 *ind_c1[NDF*ind_nop1[0]+2]+SL6 *ind_c1[NDF*ind_nop1[1]+2]+
SL9 *ind_c1[NDF*ind_nop1[2]+2]+SL12*ind_c1[NDF*ind_nop1[3]+2]+
SL15*ind_c1[NDF*ind_nop1[4]+2]+SL18*ind_c1[NDF*ind_nop1[5]+2]+
SL21*ind_c1[NDF*ind_nop1[6]+2]+SL24*ind_c1[NDF*ind_nop1[7]+2]);
	if(ISL==2)
	  {*DJD=DJR[0]*(DJR[4]*DJR[8]-DJR[5]*DJR[7])+DJR[1]*(DJR[5]*DJR[6]-DJR[3]*DJR[8])+DJR[2]*(DJR[3]*DJR[7]-DJR[4]*DJR[6]);
	  if(sqrt(*DJD * *DJD)<1.E-6f){
// ::MessageBeep(-1);MessageBox("STFISO8 zero det.");
									   *DJD=1.E-6f;
									  }
	  DJI1=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;DJI4=(DJR[5]*DJR[6]-DJR[3]*DJR[8])/ *DJD;
	  DJI7=(DJR[3]*DJR[7]-DJR[4]*DJR[6])/ *DJD;DJI2=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
	  DJI5=(DJR[0]*DJR[8]-DJR[2]*DJR[6])/ *DJD;DJI8=(DJR[1]*DJR[6]-DJR[0]*DJR[7])/ *DJD;
	  DJI3=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;DJI6=(DJR[2]*DJR[3]-DJR[0]*DJR[5])/ *DJD;
	  DJI9=(DJR[0]*DJR[4]-DJR[1]*DJR[3])/ *DJD;
	  SG[0]=0.5f*(DJI1*SL1+DJI4*SL2+DJI7*SL3);SG[1]=0.5f*(DJI2*SL1+DJI5*SL2+DJI8*SL3);
	  SG[2]=0.5f*(DJI3*SL1+DJI6*SL2+DJI9*SL3);SG[3]=0.5f*(DJI1*SL4+DJI4*SL5+DJI7*SL6);
	  SG[4]=0.5f*(DJI2*SL4+DJI5*SL5+DJI8*SL6);SG[5]=0.5f*(DJI3*SL4+DJI6*SL5+DJI9*SL6);
	  SG[6]=0.5f*(DJI1*SL7+DJI4*SL8+DJI7*SL9);SG[7]=0.5f*(DJI2*SL7+DJI5*SL8+DJI8*SL9);
	  SG[8]=0.5f*(DJI3*SL7+DJI6*SL8+DJI9*SL9);SG[9]=0.5f*(DJI1*SL10+DJI4*SL11+DJI7*SL12);
	  SG[10]=0.5f*(DJI2*SL10+DJI5*SL11+DJI8*SL12);SG[11]=0.5f*(DJI3*SL10+DJI6*SL11+DJI9*SL12);
	  SG[12]=0.5f*(DJI1*SL13+DJI4*SL14+DJI7*SL15);SG[13]=0.5f*(DJI2*SL13+DJI5*SL14+DJI8*SL15);
	  SG[14]=0.5f*(DJI3*SL13+DJI6*SL14+DJI9*SL15);SG[15]=0.5f*(DJI1*SL16+DJI4*SL17+DJI7*SL18);
	  SG[16]=0.5f*(DJI2*SL16+DJI5*SL17+DJI8*SL18);SG[17]=0.5f*(DJI3*SL16+DJI6*SL17+DJI9*SL18);
	  SG[18]=0.5f*(DJI1*SL19+DJI4*SL20+DJI7*SL21);SG[19]=0.5f*(DJI2*SL19+DJI5*SL20+DJI8*SL21);
	  SG[20]=0.5f*(DJI3*SL19+DJI6*SL20+DJI9*SL21);SG[21]=0.5f*(DJI1*SL22+DJI4*SL23+DJI7*SL24);
	  SG[22]=0.5f*(DJI2*SL22+DJI5*SL23+DJI8*SL24);SG[23]=0.5f*(DJI3*SL22+DJI6*SL23+DJI9*SL24);
	  }
	else
	  {FPROD=1.f;
		if     (sqrt((G*G-1.f)*(G*G-1.f)) < PRMT){I=1;if(G<0.f)FPROD= -1.f;}
		else if(sqrt((E*E-1.f)*(E*E-1.f)) < PRMT){I=2;if(E<0.f)FPROD= -1.f;}
		else if(sqrt((T*T-1.f)*(T*T-1.f)) < PRMT){I=3;if(T<0.f)FPROD= -1.f;}
		else{
//  ::MessageBeep(-1);MessageBox("STFISO8 invalid surface pt.");
honk<<G<<" "<<E<<" "<<T<<" STFISO8:Invalid SP\n";
			  exit(0);
			 }
		IP= -2+I*(11-3*I)/2;IM=6-I-IP;
		GG11=DJR[IP-1]*DJR[IP-1]+DJR[IP+2]*DJR[IP+2]+DJR[IP+5]*DJR[IP+5];
		GG22=DJR[IM-1]*DJR[IM-1]+DJR[IM+2]*DJR[IM+2]+DJR[IM+5]*DJR[IM+5];
		GG12=DJR[IP-1]*DJR[IM-1]+DJR[IP+2]*DJR[IM+2]+DJR[IP+5]*DJR[IM+5];
		*DJD=GG11*GG22-GG12*GG12;
	  if(sqrt(*DJD * *DJD)<1.E-6f){
//Application->MessageBox(L"DJD",L"Rotten",MB_OK);
									   *DJD=1.E-6f;
									  }
	  else *DJD=sqrt(*DJD);
//////////////////////
////////////////////////
////////////////////////// start trash
if(fabs(GG11)<1.e-10f){//honk<<ISL<<" "<<GG11<<" GG11 "<<G<<" "<<E<<" "<<T<<"\n";
					  for(J=0;J<8;J++)honk<<(ind_nop1[J]+1)<<" 8-n hex STFISOsurf fail "<<ind_c1[NDF*ind_nop1[J]  ]<<" "<<ind_c1[NDF*ind_nop1[J]+1]<<" "<<ind_c1[NDF*ind_nop1[J]+2]<<"\n";
//					  exit(0);
					  GG11=1.e-4f;
					 }
////////////////////////// end trash
////////////////////////
//////////////////////
	  GGSQ=1.f/sqrt(GG11);
	  HN[0]=(DJR[IP+2]*DJR[IM+5]-DJR[IP+5]*DJR[IM+2])/ *DJD;
	  HN[1]=(DJR[IP+5]*DJR[IM-1]-DJR[IP-1]*DJR[IM+5])/ *DJD;
	  HN[2]=(DJR[IP-1]*DJR[IM+2]-DJR[IP+2]*DJR[IM-1])/ *DJD;
	  HN[3]=GGSQ*DJR[IP-1];HN[4]=GGSQ*DJR[IP+2];HN[5]=GGSQ*DJR[IP+5];
	  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
	  HN[8]=HN[0]*HN[4]-HN[1]*HN[3];for(int IC=0;IC<9;IC++)HN[IC]=FPROD*HN[IC];
	}
	 }
		else{
//  ::MessageBeep(-1);MessageBox("STFISO8 unsupported ISL");
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory",L"Halt",MB_OK);exit(0);
					}
}
//---------------------------------------------------------------------------
void TForm1::STFISO6(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//		SUBROUTINE STFISO6(ISL,HN,SN,SG,DJD,G,E,T,NOP,DJR,CORD)
//      PARAMETER (MXNPEL=8,PRMT=1.E-2,PMSURF=1.E-8)
//      DIMENSION HN(9),SN(6),SG(18),CORD(1),DJR(9),NOP(MXNPEL),DJR6(12)
//      DIMENSION HN(3),SN(6),SG(3,6),CORD(NDF,1),DJR(3,3),
//     . NOP(MXNPEL,1),DJR6(4,3)
//CCCCC Note: G==L1, E==L2, T==natural z coord (=t), L3=1.-L1-L2
//
//   DJR(I,J)= dx/dL1  dy/dL1  dz/dL1  based on L1/L2/1-L1-L2 coords
//             dx/dL2  dy/dL2  dz/dL2  based on L1/L2/1-L1-L2 coords
//             dx/dt   dy/dt   dz/dt   based on L1/L2/1-L1-L2 coords
//
//   However, to compute surface normals it is necessary to use the
//   appropriate basis system on each side (ie. L1/L2 on side L1=0,
//   L1/L2 on side L2=0, L1/L3 on side L3=0)
//
//   Hence the array SLP(I)= dN(L1,L3)/dL1 and DJR6(I,J) thus:
//
//  DJR6(I,J)= dx/dL1  dy/dL1  dz/dL1  based on L1/1-L1-L3/L3 coords
//             dx/dL1  dy/dL1  dz/dL1  based on L1/L2/1-L1-L2 coords
//             dx/dL2  dy/dL2  dz/dL2  based on L1/L2/1-L1-L2 coords
//  	       dx/dt   dy/dt   dz/dt   based on L1/L2/1-L1-L2 coords
//
//  Note that DJD/DJD are RECTANGULAR areas/volumes ...NOT triangular.
//  See User Manual for element surface coord.frame & note that this
//  frame varies drastically on triangular sides....
{int I=0,J=0,IM=0,IP=0,IC=0;
 float PRMT=1.E-5f,PMSURF=1.E-8f,TP=0.f,TM=0.f,DJR6[12+1],SL3=0.f,SL6=0.f,SL9=0.f,WSGN=0.f,
	   DJI1=0.f,DJI4=0.f,DJI7=0.f,DJI2=0.f,DJI5=0.f,DJI8=0.f,DJI3=0.f,DJI6=0.f,DJI9=0.f,GG11=0.f,GG22=0.f,GG12=0.f,GGSQ=0.f,FPROD=0.f;
 TP=(1.f+T)*0.5f;TM=(1.f-T)*0.5f;
 if(ISL==1){*DJD=0.f;SN[0]=G*TM;SN[1]=E*TM;SN[2]=(1.f-G-E)*TM;SN[3]=G*TP;SN[4]=E*TP;SN[5]=(1.f-G-E)*TP;}
 else if(ISL==2 || ISL==3)
	{SL3= -G*0.5f;SL6= -E*0.5f;SL9=-0.5f-SL3-SL6;
	 J= -2;DJR[3*J+6]=TM*(ind_c1[NDF*ind_nop1[0]  ]-ind_c1[NDF*ind_nop1[2]  ])+TP*(ind_c1[NDF*ind_nop1[3]  ]-ind_c1[NDF*ind_nop1[5]  ]);
		DJR[3*J+7]=TM*(ind_c1[NDF*ind_nop1[1]  ]-ind_c1[NDF*ind_nop1[2]  ])+TP*(ind_c1[NDF*ind_nop1[4]  ]-ind_c1[NDF*ind_nop1[5]  ]);
		DJR[3*J+8]=SL3*(ind_c1[NDF*ind_nop1[0]  ]-ind_c1[NDF*ind_nop1[3]  ])+
SL6*(ind_c1[NDF*ind_nop1[1]  ]-ind_c1[NDF*ind_nop1[4]  ])+SL9*(ind_c1[NDF*ind_nop1[2]  ]-ind_c1[NDF*ind_nop1[5]  ]);
	 J= -1;DJR[3*J+6]=TM*(ind_c1[NDF*ind_nop1[0]+1]-ind_c1[NDF*ind_nop1[2]+1])+TP*(ind_c1[NDF*ind_nop1[3]+1]-ind_c1[NDF*ind_nop1[5]+1]);
		DJR[3*J+7]=TM*(ind_c1[NDF*ind_nop1[1]+1]-ind_c1[NDF*ind_nop1[2]+1])+TP*(ind_c1[NDF*ind_nop1[4]+1]-ind_c1[NDF*ind_nop1[5]+1]);
		DJR[3*J+8]=SL3*(ind_c1[NDF*ind_nop1[0]+1]-ind_c1[NDF*ind_nop1[3]+1])+
SL6*(ind_c1[NDF*ind_nop1[1]+1]-ind_c1[NDF*ind_nop1[4]+1])+SL9*(ind_c1[NDF*ind_nop1[2]+1]-ind_c1[NDF*ind_nop1[5]+1]);
	 J=0;DJR[3*J+6]=TM*(ind_c1[NDF*ind_nop1[0]+2]-ind_c1[NDF*ind_nop1[2]+2])+TP*(ind_c1[NDF*ind_nop1[3]+2]-ind_c1[NDF*ind_nop1[5]+2]);
		DJR[3*J+7]=TM*(ind_c1[NDF*ind_nop1[1]+2]-ind_c1[NDF*ind_nop1[2]+2])+TP*(ind_c1[NDF*ind_nop1[4]+2]-ind_c1[NDF*ind_nop1[5]+2]);
		DJR[3*J+8]=SL3*(ind_c1[NDF*ind_nop1[0]+2]-ind_c1[NDF*ind_nop1[3]+2])+
SL6*(ind_c1[NDF*ind_nop1[1]+2]-ind_c1[NDF*ind_nop1[4]+2])+SL9*(ind_c1[NDF*ind_nop1[2]+2]-ind_c1[NDF*ind_nop1[5]+2]);
	if(ISL==2)
	  {*DJD=DJR[0]*(DJR[4]*DJR[8]-DJR[5]*DJR[7])+DJR[1]*(DJR[5]*DJR[6]-DJR[3]*DJR[8])+DJR[2]*(DJR[3]*DJR[7]-DJR[4]*DJR[6]);
	  if(sqrt(*DJD * *DJD)<1.E-6f){
//  ::MessageBeep(-1);MessageBox("STFISO6 zero det.");
									   *DJD=1.E-6f;
									  }
	  DJI1=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;DJI4=(DJR[5]*DJR[6]-DJR[3]*DJR[8])/ *DJD;
	  DJI7=(DJR[3]*DJR[7]-DJR[4]*DJR[6])/ *DJD;DJI2=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
	  DJI5=(DJR[0]*DJR[8]-DJR[2]*DJR[6])/ *DJD;DJI8=(DJR[1]*DJR[6]-DJR[0]*DJR[7])/ *DJD;
	  DJI3=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;DJI6=(DJR[2]*DJR[3]-DJR[0]*DJR[5])/ *DJD;
	  DJI9=(DJR[0]*DJR[4]-DJR[1]*DJR[3])/ *DJD;
	  SG[0]=DJI1*TM+DJI7*SL3;SG[1]=DJI2*TM+DJI8*SL3;SG[2]=DJI3*TM+DJI9*SL3;
	  SG[3]=DJI4*TM+DJI7*SL6;SG[4]=DJI5*TM+DJI8*SL6;SG[5]=DJI6*TM+DJI9*SL6;
	  SG[6]= -TM*(DJI1+DJI4)+DJI7*SL9;SG[7]= -TM*(DJI2+DJI5)+DJI8*SL9;SG[8]= -TM*(DJI3+DJI6)+DJI9*SL9;
	  SG[9]=DJI1*TP-DJI7*SL3;SG[10]=DJI2*TP-DJI8*SL3;SG[11]=DJI3*TP-DJI9*SL3;
	  SG[12]=DJI4*TP-DJI7*SL6;SG[13]=DJI5*TP-DJI8*SL6;SG[14]=DJI6*TP-DJI9*SL6;
	  SG[15]= -TP*(DJI1+DJI4)-DJI7*SL9;SG[16]= -TP*(DJI2+DJI5)-DJI8*SL9;SG[17]= -TP*(DJI3+DJI6)-DJI9*SL9;
	  }
	else
	  {for(J=1;J<=3;J++){for(I=1;I<=3;I++)DJR6[1-I+4*J]=DJR[-I+3*J];}
		J= -2;DJR6[4*J+9]=TM*(ind_c1[NDF*ind_nop1[0]  ]-ind_c1[NDF*ind_nop1[1]  ])+TP*(ind_c1[NDF*ind_nop1[3]  ]-ind_c1[NDF*ind_nop1[4]  ]);
		J= -1;DJR6[4*J+9]=TM*(ind_c1[NDF*ind_nop1[0]+1]-ind_c1[NDF*ind_nop1[1]+1])+TP*(ind_c1[NDF*ind_nop1[3]+1]-ind_c1[NDF*ind_nop1[4]+1]);
		J=0;DJR6[4*J+9]=TM*(ind_c1[NDF*ind_nop1[0]+2]-ind_c1[NDF*ind_nop1[1]+2])+TP*(ind_c1[NDF*ind_nop1[3]+2]-ind_c1[NDF*ind_nop1[4]+2]);
		WSGN=1.f;FPROD=1.f;
		if(fabs(T*T-1.f)<PRMT){IP=2;IM=3;if(T<0.f)FPROD= -1.f;}
		else{IM=4;if(fabs(G)<PRMT){IP=3;WSGN= -1.f;}
			  else if(fabs(E)<PRMT)IP=2;
			  else if(fabs(1.f-G-E)<PRMT){IP=1;WSGN= -1.f;}
			  else{
//  ::MessageBeep(-1);MessageBox("STFISO6 invalid surface pt.");
					 exit(0);
					}
			 }
		GG11=DJR6[IP]*DJR6[IP]+DJR6[IP+4]*DJR6[IP+4]+DJR6[IP+8]*DJR6[IP+8];
		GG22=DJR6[IM]*DJR6[IM]+DJR6[IM+4]*DJR6[IM+4]+DJR6[IM+8]*DJR6[IM+8];
		GG12=DJR6[IP]*DJR6[IM]+DJR6[IP+4]*DJR6[IM+4]+DJR6[IP+8]*DJR6[IM+8];
		*DJD=GG11*GG22-GG12*GG12;
		if(*DJD<PMSURF*PMSURF){
//  ::MessageBeep(-1);MessageBox("STFISO6 zero surf.det.");
									   *DJD=1.E-6f;
                                      }
		else *DJD=sqrt(*DJD);
		GGSQ=WSGN/sqrt(GG11);
		HN[0]=WSGN*(DJR6[IP+4]*DJR6[IM+8]-DJR6[IP+8]*DJR6[IM+4])/ *DJD;
		HN[1]=WSGN*(DJR6[IP+8]*DJR6[IM  ]-DJR6[IP  ]*DJR6[IM+8])/ *DJD;
		HN[2]=WSGN*(DJR6[IP  ]*DJR6[IM+4]-DJR6[IP+4]*DJR6[IM  ])/ *DJD;
		HN[3]=GGSQ*DJR6[IP  ];HN[4]=GGSQ*DJR6[IP+4];HN[5]=GGSQ*DJR6[IP+8];
		HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
		HN[8]=HN[0]*HN[4]-HN[1]*HN[3];for(IC=0;IC<9;IC++)HN[IC]=FPROD*HN[IC];
	  }
	}
 else{
//  ::MessageBeep(-1);MessageBox("STFISO6 unsupported ISL");
		exit(0);
	  }
}
//---------------------------------------------------------------------------
void TForm1::STFISO4(int ISL,float G,float E,float T,float *DJD,float HN[],float SN[],float SG[],float DJR[],long ind_nop1[],float ind_c1[])
//  See User Manual for element surface coord.frame.
//  On positive sides: n x t1=t2; on negative HN()= -HN(positive side)
//  so n x t1 = -t2 on negative sides (ie. NOT corkscrew).
//  Normal=HN(1 to 3), t1=HN(4 to 6), t2=HN(7 to 9) so      x = HN.x'
//  CAUTION: do NOT evaluate normals at edges/nodes due to ambiguity!!
{int J=0,IM=0,IP=0;
//      DIMENSION HN(3),SN(1),SG(3,8),CORD(NDF,1),DJR(3,3),NOP(MXNPEL)
 float PRMT=1.E-5f,PMSURF=1.E-8f,S=0.f,DJI1=0.f,DJI4=0.f,DJI7=0.f,DJI2=0.f,DJI5=0.f,DJI8=0.f,DJI3=0.f,DJI6=0.f,DJI9=0.f,GG11=0.f,GG22=0.f,GG12=0.f,GGSQ=0.f;
//      DIMENSION HN(3),SN(4),SG(3,4),CORD(NDF,1),DJR(3,3),NOP(MXNPEL,1)
//CCCCC Note: G==L1, E==L2, T==natural z coord (=t), L3=1.-L1-L2
//
//   DJR(I,J)= dx/dL1  dy/dL1  dz/dL1  based on L1/L2/L3/1-L1-L2-L3 coords
//             dx/dL2  dy/dL2  dz/dL2  based on L1/L2/L3/1-L1-L2-L3 coords
//             dx/dL3  dy/dL3  dz/dL3  based on L1/L2/L3/1-L1-L2-L3 coords
//
//   However, to compute surface normals it is necessary to use the
//   appropriate basis system on each side (ie. L1/L2 on side L1=0,
//   L1/L2 on side L2=0, L1/L3 on side L3=0)
//
//  Note that DJD/DJD are true TETRA areas/volumes ...NOT rectangular.
//  See User Manual for element surface coord.frame.
  S=1.f-G-E-T;
  if(ISL==1){*DJD=0.f;SN[0]=G;SN[1]=E;SN[2]=T;SN[3]=S;}
  else if(ISL==2 || ISL==3)
	 {J= -2;DJR[3*J+6]= -ind_c1[NDF*ind_nop1[3]  ]+ind_c1[NDF*ind_nop1[0]  ];
		  DJR[3*J+7]= -ind_c1[NDF*ind_nop1[3]  ]+ind_c1[NDF*ind_nop1[1]  ];
		  DJR[3*J+8]= -ind_c1[NDF*ind_nop1[3]  ]+ind_c1[NDF*ind_nop1[2]  ];
	  J= -1;DJR[3*J+6]= -ind_c1[NDF*ind_nop1[3]+1]+ind_c1[NDF*ind_nop1[0]+1];
		  DJR[3*J+7]= -ind_c1[NDF*ind_nop1[3]+1]+ind_c1[NDF*ind_nop1[1]+1];
		  DJR[3*J+8]= -ind_c1[NDF*ind_nop1[3]+1]+ind_c1[NDF*ind_nop1[2]+1];
	  J=0;DJR[3*J+6]= -ind_c1[NDF*ind_nop1[3]+2]+ind_c1[NDF*ind_nop1[0]+2];
		  DJR[3*J+7]= -ind_c1[NDF*ind_nop1[3]+2]+ind_c1[NDF*ind_nop1[1]+2];
		  DJR[3*J+8]= -ind_c1[NDF*ind_nop1[3]+2]+ind_c1[NDF*ind_nop1[2]+2];
	  if(ISL==2)
		 {*DJD=DJR[0]*(DJR[4]*DJR[8]-DJR[5]*DJR[7])+DJR[1]*(DJR[5]*DJR[6]-DJR[3]*DJR[8])+DJR[2]*(DJR[3]*DJR[7]-DJR[4]*DJR[6]);
		  if(sqrt(*DJD * *DJD)<1.E-6f){
//  ::MessageBeep(-1);MessageBox("STFISO4 zero det.");
											   *DJD=1.E-6f;
											  }
		  DJI1=(DJR[4]*DJR[8]-DJR[5]*DJR[7])/ *DJD;DJI4=(DJR[5]*DJR[6]-DJR[3]*DJR[8])/ *DJD;
		  DJI7=(DJR[3]*DJR[7]-DJR[4]*DJR[6])/ *DJD;DJI2=(DJR[2]*DJR[7]-DJR[1]*DJR[8])/ *DJD;
		  DJI5=(DJR[0]*DJR[8]-DJR[2]*DJR[6])/ *DJD;DJI8=(DJR[1]*DJR[6]-DJR[0]*DJR[7])/ *DJD;
		  DJI3=(DJR[1]*DJR[5]-DJR[2]*DJR[4])/ *DJD;DJI6=(DJR[2]*DJR[3]-DJR[0]*DJR[5])/ *DJD;
		  DJI9=(DJR[0]*DJR[4]-DJR[1]*DJR[3])/ *DJD;
		  SG[0]=-DJI1-DJI4-DJI7;SG[1]=-DJI2-DJI5-DJI8;SG[2]=-DJI3-DJI6-DJI9;
		  SG[3]=DJI1;SG[4]=DJI2;SG[5]=DJI3;SG[6]=DJI4;SG[7]=DJI5;
		  SG[8]=DJI6;SG[9]=DJI7;SG[10]=DJI8;SG[11]=DJI9; *DJD= *DJD/6.f;
		 }
	  else
		 {if(fabs(S)<PRMT)
			 {IP=2;IM=3;HN[0]= -DJR[IP+2]*DJR[IM+5]+DJR[IP+5]*DJR[IM+2];
			  HN[1]= -DJR[IP+5]*DJR[IM-1]+DJR[IP-1]*DJR[IM+5];HN[2]= -DJR[IP-1]*DJR[IM+2]+DJR[IP+2]*DJR[IM-1];
			  IP=3;IM=1;HN[0]=HN[0]-DJR[IP+2]*DJR[IM+5]+DJR[IP+5]*DJR[IM+2];
			  HN[1]=HN[1]-DJR[IP+5]*DJR[IM-1]+DJR[IP-1]*DJR[IM+5];HN[2]=HN[2]-DJR[IP-1]*DJR[IM+2]+DJR[IP+2]*DJR[IM-1];
			  IP=1;IM=2;HN[0]=HN[0]-DJR[IP+2]*DJR[IM+5]+DJR[IP+5]*DJR[IM+2];
			  HN[1]=HN[1]-DJR[IP+5]*DJR[IM-1]+DJR[IP-1]*DJR[IM+5];HN[2]=HN[2]-DJR[IP-1]*DJR[IM+2]+DJR[IP+2]*DJR[IM-1];
			  *DJD=HN[0]*HN[0]+HN[1]*HN[1]+HN[2]*HN[2];
			  if(*DJD<PMSURF*PMSURF){
//  ::MessageBeep(-1);MessageBox("STFISO4 zero surf.det.");
												 *DJD=1.E-6f;
												}
			  else *DJD=sqrt(*DJD);
			  HN[0]=HN[0]/ *DJD;HN[1]=HN[1]/ *DJD;HN[2]=HN[2]/ *DJD;
			  HN[3]=DJR[2]-DJR[0];HN[4]=DJR[5]-DJR[3];HN[5]=DJR[8]-DJR[6];
			  GGSQ=1.f/sqrt(HN[3]*HN[3]+HN[4]*HN[4]+HN[5]*HN[5]);
			  HN[3]=GGSQ*HN[3];HN[4]=GGSQ*HN[4];HN[5]=GGSQ*HN[5];
			  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
			  HN[8]=HN[0]*HN[4]-HN[1]*HN[3]; *DJD= *DJD/2.f;
			 }
		  else
			 {if(fabs(G)<PRMT){IP=2;IM=3;}
			  else if(fabs(E)<PRMT){IP=3;IM=1;}
			  else if(fabs(T)<PRMT){IP=1;IM=2;}
			  else{exit(0);}
			  GG11=DJR[IP-1]*DJR[IP-1]+DJR[IP+2]*DJR[IP+2]+DJR[IP+5]*DJR[IP+5];
			  GG22=DJR[IM-1]*DJR[IM-1]+DJR[IM+2]*DJR[IM+2]+DJR[IM+5]*DJR[IM+5];
			  GG12=DJR[IP-1]*DJR[IM-1]+DJR[IP+2]*DJR[IM+2]+DJR[IP+5]*DJR[IM+5];
			  *DJD=GG11*GG22-GG12*GG12;
			  if(*DJD<PMSURF*PMSURF){
//  ::MessageBeep(-1);MessageBox("STFISO4 zero surf.det.");
												 *DJD=1.E-6f;
												}
			  else *DJD=sqrt(*DJD);
			  GGSQ=1.f/sqrt(GG11);
			  HN[0]=(DJR[IP+2]*DJR[IM+5]-DJR[IP+5]*DJR[IM+2])/ *DJD;
			  HN[1]=(DJR[IP+5]*DJR[IM-1]-DJR[IP-1]*DJR[IM+5])/ *DJD;
			  HN[2]=(DJR[IP-1]*DJR[IM+2]-DJR[IP+2]*DJR[IM-1])/ *DJD;
			  HN[3]=GGSQ*DJR[IP-1];HN[4]=GGSQ*DJR[IP+2];HN[5]=GGSQ*DJR[IP+5];
			  HN[6]=HN[1]*HN[5]-HN[2]*HN[4];HN[7]=HN[2]*HN[3]-HN[0]*HN[5];
			  HN[8]=HN[0]*HN[4]-HN[1]*HN[3]; *DJD= *DJD/2.f;
			 }
		 }
	 }
  else{
//  ::MessageBeep(-1);MessageBox("STFISO4 unsupported ISL");
		 exit(0);
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Form1Paint(TObject *Sender)
// Convention: iPaintyesno=ab
//   where   a=0 ->Painted       and   b=0 ->Find all GID facets
//             1 ->Unpainted             1 ->Surface facets only
//
// iplotType=2      Use element-by-element plot authorization (base.arELEM)
//          =other  Use GID authorization                     (base.arGID, but actually arGID is not in base.XXX now)
//
// iplot_flag=other ->Banner
//    The following require  iPaintyesno=10 (ie. exterior facets known)
// iplot_flag=1     ->Margin_plot ->iCircleplot=0                                      ->Wireframe
//                                ->iCircleplot=other ->iCullyesno=0     ->Cull facets ->Paint
//                                                    ->iCullyesno=other ->Use facets  ->Paint
// iplot_flag=2     ->Scale_plot  ->iCircleplot=0                                      ->Wireframe
//                                ->iCircleplot=other ->iCullyesno=0     ->Cull facets ->Paint
//                                                    ->iCullyesno=other ->Use facets  ->Paint
{
 TPoint ptDraw[MAXPOINTS];
 long ntranche=8,i=0;
 float factor=10.f,VcapArr[2*7]={7.f,0.f, 11.f,14.f, 15.f,14.f, 19.f,0.f, 16.f,0.f, 13.f,10.f, 10.f,0.f},
	   FcapArr[2*10]={21.f,0.f, 21.f,14.f, 24.f,14.f, 24.f,8.f, 29.f,8.f, 29.f,6.f, 24.f,6.f, 24.f,2.f, 29.f,2.f, 29.f,0.f},
	   TcapArr[2*8]={30.f,0.f, 30.f,2.f, 33.5f,2.f, 33.5f,14.f, 37.f,14.f, 37.f,2.f, 40.5f,2.f, 40.5f,0.f},
	   TlowArr[2*8]={41.f,0.f, 41.f,0.25f, 41.625f,0.25f, 41.625f,2.f, 41.875f,2.f, 41.875f,0.25f, 42.5f,0.25f, 42.5f,0.f},
	   MlowArr[2*10]={42.75f,0.f, 42.75f,2.f, 43.f,2.f, 43.f,0.5f, 43.625f,1.8f, 44.25f,0.5f, 44.25f,2.f, 44.5f,2.f, 44.5f,0.f, 43.625f,1.4f};
///////////////////////////////////////
if     (iBackroundColor==1){Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clBlack;
							tBitmap->Canvas->Brush->Color=clWhite;tBitmap->Canvas->Pen->Color=clBlack;
						   }
else if(iBackroundColor==2){Canvas->Brush->Color=clRed;Canvas->Pen->Color=clWhite;
							tBitmap->Canvas->Brush->Color=clRed;tBitmap->Canvas->Pen->Color=clWhite;
						   }
else if(iBackroundColor==3){Canvas->Brush->Color=clGreen;Canvas->Pen->Color=clWhite;
							tBitmap->Canvas->Brush->Color=clGreen;tBitmap->Canvas->Pen->Color=clWhite;
						   }
else if(iBackroundColor==4){Canvas->Brush->Color=clBlue;Canvas->Pen->Color=clWhite;
							tBitmap->Canvas->Brush->Color=clBlue;tBitmap->Canvas->Pen->Color=clWhite;
						   }
else                       {Canvas->Brush->Color=clBlack;Canvas->Pen->Color=clWhite;
							tBitmap->Canvas->Brush->Color=clBlack;tBitmap->Canvas->Pen->Color=clWhite;
						   }
///////////////////////////////////////
 Canvas->FillRect(Rect(0,0,Width,Height));
 tBitmap->Width = Width;tBitmap->Height = Height;
 tBitmap->Canvas->FillRect(Rect(0,0,tBitmap->Width,tBitmap->Height));

 if(iplotflag==1)
   {if(iCircleplot != 0 && iPaintyesno/10==0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must paint first->View/Paint",L"Halt",MB_OK);iCircleplot=0;}
	Image1->Visible=false;
	FDpmargin(indat.npoin,indat.nelt,indat.nop1,indat.matno,indat.c1,trans_op);
	if(iCircleplot==0)FDwireplot(indat.nelt,indat.nop1,indat.matno,indat.c1);
	else if(iCircleplot==1)
	  {if(iCullyesno==0){
/////////////////////////////////
						 if(iplotType==2){//nFacets=FDcullfacet_arE_mem(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM);
											 arbFacet=new long[4*base.nelt]; //Use approx storage instead of computing nFacets exactly EFP 1/02/2012
											 rbTemp=new long[4*base.nelt];
											 nFacets=FDcullfacet_arEc(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM,arbFacet,rbTemp);
											 FDsort_low1(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,ntranche);
//										  if(!ShowFEMesh->Checked)FDculledge(indat.nop1,indat.matno,indat.bc1,nFacets,arbFacet,edgFacet);
iCullyesno=1;
										 }
						 else {
							arbFacet=new long[4*base.nelt]; //Use approx storage instead of computing nFacets exactly EFP 1/02/2012
							rbTemp=new long[4*base.nelt];
							nFacets=FDcullfacetc(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet,rbTemp);
							FDsort_low1(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,ntranche);
//						 if(!ShowFEMesh->Checked)FDculledge(indat.nop1,indat.matno,indat.bc1,nFacets,arbFacet,edgFacet);
iCullyesno=1;
							  }
						}
										   FDpaintplot(1,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if((iPersistVFT-100*(iPersistVFT/100))/10 && wp.nWeldPass)persistVFT1a(wp.nWeldPass,wp.util_arr); //EFP 1/27/2011
	   if(iPersistVFT/100 && wp.nWeldPass)persistVFT2(wp.nWeldPass,wp.util_arr);
	  }
	axis_plotXYZ(ClientWidth,ClientHeight);
//xxxxxxxxx
 Form1->Canvas->Draw(0,0,tBitmap);
//xxxxxxxxx
   }
 else if(iplotflag==2)
   {if(iCircleplot != 0 && iPaintyesno/10==0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must paint first->View/Paint",L"Halt",MB_OK);iCircleplot=0;}
	Image1->Visible=false;
	FDpscale(indat.npoin,indat.c1,prod_op);
	if(iCircleplot==0)FDwireplot(indat.nelt,indat.nop1,indat.matno,indat.c1);
	else if(iCircleplot==1)
	  {if(iCullyesno==0){
/////////////////////////////////
						 if(iplotType==2){//nFacets=FDcullfacet_arE_mem(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM);
//										  GlobalMemoryStatus(&ms);
//										  if(4*(ms.dwAvailPhys/20)>unsigned((ntranche+5)*nFacets+3*ntranche+2)) // 80% of available long word memory
//											{
//											 arbFacet=new long[nFacets];rbTemp=new long[nFacets];edgFacet=new int[nFacets];mutFacet=new float[nFacets];
											 arbFacet=new long[4*base.nelt]; //Use approx storage instead of computing nFacets exactly EFP 1/02/2012
											 rbTemp=new long[4*base.nelt];
//											 FDcullfacet_arE(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM,arbFacet);
											 nFacets=FDcullfacet_arEc(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM,arbFacet,rbTemp);
//											 FDorderfacet(indat.nop1,indat.matno,indat.c1,nFacets,arbFacet,rbTemp);
											 FDsort_low1(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,ntranche);
//											}
//										  else if(4*(ms.dwAvailPhys/20)>unsigned(6*nFacets))
//											{
//											 arbFacet=new long[nFacets];rbTemp=new long[nFacets];edgFacet=new int[nFacets];mutFacet=new float[nFacets];
//											 FDcullfacet_arE(indat.nelt,indat.nop1,indat.matno,indat.c1,base.arELEM,arbFacet);
//											 FDorderfacet(indat.nop1,indat.matno,indat.c1,nFacets,arbFacet,rbTemp);
//											 arbFace1=new long[nFacets];rbTem1=new long[nFacets];edgFace1=new int[nFacets];
//											 FDsort_low(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,edgFacet,arbFace1,rbTem1,edgFace1);
//											 delete [] arbFace1;delete [] rbTem1;delete [] edgFace1;
//											}
//										  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory to render.",L"Terminate",MB_OK);exit(0);}
//										  if(!ShowFEMesh->Checked)FDculledge(indat.nop1,indat.matno,indat.bc1,nFacets,arbFacet,edgFacet);
iCullyesno=1;
										 }
						 else {
/////////////////////////////////

//						 nFacets=FDcullfacet_mem(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID);
//						 GlobalMemoryStatus(&ms);
//						 if(4*(ms.dwAvailPhys/20)>unsigned((ntranche+5)*nFacets+3*ntranche+2)) // 80% of available long word memory
//						   {
//							arbFacet=new long[nFacets];rbTemp=new long[nFacets];edgFacet=new int[nFacets];mutFacet=new float[nFacets];
							arbFacet=new long[4*base.nelt]; //Use approx storage instead of computing nFacets exactly EFP 1/02/2012
							rbTemp=new long[4*base.nelt];
//							FDcullfacet(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet);
							nFacets=FDcullfacetc(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet,rbTemp);
//							FDorderfacet(indat.nop1,indat.matno,indat.c1,nFacets,arbFacet,rbTemp);
							FDsort_low1(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,ntranche);
//						   }
//						 else if(4*(ms.dwAvailPhys/20)>unsigned(6*nFacets))
//						   {
//							arbFacet=new long[nFacets];rbTemp=new long[nFacets];edgFacet=new int[nFacets];mutFacet=new float[nFacets];
//							FDcullfacet(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet);
//							FDorderfacet(indat.nop1,indat.matno,indat.c1,nFacets,arbFacet,rbTemp);
//							arbFace1=new long[nFacets];rbTem1=new long[nFacets];edgFace1=new int[nFacets];
//							FDsort_low(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,edgFacet,arbFace1,rbTem1,edgFace1);
//							delete [] arbFace1;delete [] rbTem1;delete [] edgFace1;
//						   }
//						 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory to render.",L"Terminate",MB_OK);exit(0);}

//                         try {arbFacet=new long[nFacets];rbTemp=new long[nFacets];edgFacet=new int[nFacets];}
//                         catch (xalloc){extern PACKAGE void __fastcall Beep(void);Application->MessageBox("Insufficient cull dynamic memory","Halt",MB_OK);exit(0);}
//                         FDcullfacet(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet);
//                         FDorderfacet(indat.nop1,indat.matno,indat.c1,nFacets,arbFacet,rbTemp);
//                         try {arbFace1=new long[nFacets];rbTem1=new long[nFacets];edgFace1=new int[nFacets];
//                              FDsort_low(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,edgFacet,arbFace1,rbTem1,edgFace1);
//                              delete [] arbFace1;delete [] rbTem1;delete [] edgFace1;
//                             }
//                         catch (xalloc){extern PACKAGE void __fastcall Beep(void);Application->MessageBox("Insufficient FDsort_low dynamic memory","Halt",MB_OK);exit(0);}

//						 if(!ShowFEMesh->Checked)FDculledge(indat.nop1,indat.matno,indat.bc1,nFacets,arbFacet,edgFacet);
iCullyesno=1;
////////////////////////////////
							  }
////////////////////////////////


						}
										  FDpaintplot(1,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if((iPersistVFT-100*(iPersistVFT/100))/10 && wp.nWeldPass)persistVFT1a(wp.nWeldPass,wp.util_arr); //EFP 1/27/2011
	   if(iPersistVFT/100 && wp.nWeldPass)persistVFT2(wp.nWeldPass,wp.util_arr);
//	   if(wp.nWeldPass)persistVFT2(wp.nWeldPass,wp.util_arr);
	  }
	axis_plotXYZ(ClientWidth,ClientHeight);
//xxxxxxxxx
 Form1->Canvas->Draw(0,0,tBitmap);
//xxxxxxxxx
   }
 else {

	   if(!base.nop1){
//Image1->Visible=true; //EFP 12/04/2009 This image is VFT logo
						ptDraw[0].x=0;ptDraw[0].y=ClientHeight;
						ptDraw[1].x=(ClientWidth-Panel1->Width)/2;ptDraw[1].y=ClientHeight/2;
						ptDraw[2].x=ClientWidth-Panel1->Width;ptDraw[2].y=ClientHeight;
//						tBitmap->Canvas->Brush->Color=clYellow;tBitmap->Canvas->Pen->Color=clYellow;
//						tBitmap->Canvas->Polygon(ptDraw,3-1);
						Canvas->Brush->Color=clYellow;Canvas->Pen->Color=clYellow;
						Canvas->Polygon(ptDraw,3-1);
//XXXXXXXX
						Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
for(i=0;i< 7;i++){ptDraw[i].x=int(factor*VcapArr[2*i+0]+0.5f);ptDraw[i].y=int(factor*VcapArr[2*i+1]+0.5f);}
						Canvas->Polygon(ptDraw,7-1);
for(i=0;i<10;i++){ptDraw[i].x=int(factor*FcapArr[2*i+0]+0.5f);ptDraw[i].y=int(factor*FcapArr[2*i+1]+0.5f);}
						Canvas->Polygon(ptDraw,10-1);
for(i=0;i< 8;i++){ptDraw[i].x=int(factor*TcapArr[2*i+0]+0.5f);ptDraw[i].y=int(factor*TcapArr[2*i+1]+0.5f);}
						Canvas->Polygon(ptDraw,8-1);
for(i=0;i< 8;i++){ptDraw[i].x=int(factor*TlowArr[2*i+0]+0.5f);ptDraw[i].y=int(factor*TlowArr[2*i+1]+0.5f);}
						Canvas->Polygon(ptDraw,8-1);
for(i=0;i<10;i++){ptDraw[i].x=int(factor*MlowArr[2*i+0]+0.5f);ptDraw[i].y=int(factor*MlowArr[2*i+1]+0.5f);}
						Canvas->Polygon(ptDraw,10-1);
//XXXXXXXX
//Image1->Visible=true; //EFP 12/04/2009 This image is VFT logo
					 }
	  }
// Form1->Canvas->Draw(0,0,tBitmap);
 cxWidth0=ClientWidth;cyHeight0=ClientHeight;
}
//---------------------------------------------------------------------------
//void TForm1::persistVFT1a(long nWP,int util_arr[])
void TForm1::persistVFT1a(long nWP,long util_arr[])
// Plots weld pass outline on tBitmap canvas
// startWP & endWP begin with 0
{int vertplot[15]={1,5,6,2,3,7,4,0,3,7,6,2,1,5,4};
 long inw=0,ie=0,ct_curr_sttEl=0,in=0,ipp=0,ie1=0,iside1=0,ip1=0,icount=0,eltype1=0,t7=10000000;
 float xave=0.f,yave=0.f,zave=0.f,xc=0.f,yc=0.f,zc=0.f,DJD=0.f,HN[9],SN[8],SG[24],DJR[9+1];
 TPoint ptDraw[MAXPOINTS];
// tBitmap->Canvas->Pen->Width=1;
 for(inw=0;inw<nWP;inw++)
  {if(util_arr[inw]/10)
   {
//	ct_curr_sttEl=base.nelt;
	ct_curr_sttEl=wp.memWGa; //Correction EFP 3/23/2012
	for(ie=0;ie<wp.memWGa;ie++)if(wp.sttEles[wp.memWGa*inw+ie]<0){ct_curr_sttEl=ie;break;}
//	icount=base.nelt;
	icount=wp.memWGa; //Correction EFP 3/23/2012
	for(ie=0;ie<wp.memWGa;ie++)if(wp.eles[wp.memWGa*inw+ie]<0){icount=ie;break;}
	if(icount){tBitmap->Canvas->Brush->Color=wp.WeldColor[inw];tBitmap->Canvas->Pen->Color=wp.WeldColor[inw];
			   tBitmap->Canvas->Pen->Width=1;
			   for(in=0;in<ct_curr_sttEl;in++)
				 {for(ipp=0;ipp<icount/ct_curr_sttEl;ipp++){
ie1=wp.eles[wp.memWGa*inw+ct_curr_sttEl*ipp+in]/10;eltype1=indat.matno[ie1]/t7;
if(eltype1==8){tBitmap->Canvas->MoveTo(int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+0]  ]+0.5f),
									   ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+0]+1]+0.5f));
			   for(ip1=0;ip1<15;ip1++)
tBitmap->Canvas->LineTo(int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+vertplot[ip1]]  ]+0.5f),
						ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+vertplot[ip1]]+1]+0.5f));
			  }
	else {honk<<(ie1+1)<<" persistVFT1a() miscreantNonHex "<<eltype1<<"\n"; //Modification
//	  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex-element found in this weld pass.",L"Warning",MB_OK);
		 }
														   }
				 }
////////////////// Insert weld direction arrow here  EFP 3/22/2012
///////////////////
////////////////////
xave=yave=zave=0.f;
for(ip1=0;ip1<wp.n_curr_sttEl[inw];ip1++)
  {for(in=0;in<8;in++){xave=xave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+0];
					   yave=yave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+1];
					   zave=zave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+2];
					  }
  }
//xave=xave/double(8*wp.n_curr_sttEl[inw]);yave=yave/double(8*wp.n_curr_sttEl[inw]);zave=zave/double(8*wp.n_curr_sttEl[inw]);
xave=xave/float(8*wp.n_curr_sttEl[inw]);yave=yave/float(8*wp.n_curr_sttEl[inw]);zave=zave/float(8*wp.n_curr_sttEl[inw]);
tBitmap->Canvas->Pen->Width=3;tBitmap->Canvas->Brush->Color=clWhite;tBitmap->Canvas->Pen->Color=clWhite;
tBitmap->Canvas->Ellipse(int(xave)-5,ClientHeight-(int(yave)-5),int(xave)+5,ClientHeight-(int(yave)+5));
tBitmap->Canvas->Brush->Color=clBlue;tBitmap->Canvas->Pen->Color=clBlue;
tBitmap->Canvas->MoveTo(int(xave),ClientHeight-int(yave));
ie1=wp.sttEles[wp.memWGa*inw+0]/10;iside1=wp.sttEles[wp.memWGa*inw+0]-10*ie1; // Use first start element face as normal
if(iside1==0){xc=  0.f;yc= -1.f;zc=  0.f;}else if(iside1==2){xc=  0.f;yc=  1.f;zc=  0.f;}else if(iside1==1){xc=  1.f;yc=  0.f;zc=  0.f;}else if(iside1==3){xc= -1.f;yc=  0.f;zc=  0.f;}else if(iside1==4){xc=  0.f;yc=  0.f;zc= -1.f;}else {xc=  0.f;yc=  0.f;zc=  1.f;}
STFISO8(3,xc,yc,zc,&DJD,HN,SN,SG,DJR,indat.nop1+MXNPEL*ie1,indat.c1); //EFP 3/24/2012
tBitmap->Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1]))); //GIANTS
tBitmap->Canvas->MoveTo(int(xave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.*HN[1])));
tBitmap->Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1])));
tBitmap->Canvas->LineTo(int(xave+25.f*(HN[0]-HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[1]-HN[0]))+(min(ClientWidth,ClientHeight)/200)*int(-50.*HN[1])));
////////////////////
///////////////////
//////////////////
			  }
   }
  }
}
//---------------------------------------------------------------------------
//void TForm1::persistVFT2(long nWP,int util_arr[])
void TForm1::persistVFT2(long nWP,long util_arr[])
// Plots weld pass start elements on tBitmap canvas
//{int gdata8[24]={0,1,5,4,
//				 1,2,6,5,
//				 3,2,6,7,
//				 0,3,7,4,
//				 0,1,2,3,
//				 4,5,6,7};
{int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
 TPoint ptDraw[MAXPOINTS];
 long inw=0,ir=0,in=0,ie1=0,iside1=0,ip=0,ir1=0,ip1=0,ieGID=0
 //,eltype=0,bscode=0,node=0,t3=1000,t5=100000,t7=10000000
 ;
 float xave=0.f,yave=0.f,zave=0.f,xc=0.f,yc=0.f,zc=0.f,DJD=0.f,HN[9],SN[8],SG[24],DJR[9+1];
 for(inw=0;inw<nWP;inw++)
   {ir=util_arr[inw]/10;
	if(util_arr[inw]-10*ir){tBitmap->Canvas->Brush->Color=wp.WeldColor[inw];
							for(in=0;in<wp.memWGa;in++)
							  {if(wp.sttEles[wp.memWGa*inw+in]>=0)
								 {ie1=wp.sttEles[wp.memWGa*inw+in]/10;
								  iside1=wp.sttEles[wp.memWGa*inw+in]-10*ie1;
ieGID=indat.arrELSET[ie1];
////////////// but this does not use indat.xxx ?
////////////
		  ir1=ieGID-9*(ieGID/9);
		  if     (ir1==0)tBitmap->Canvas->Pen->Color=clMaroon; // Coding for double color contrast
		  else if(ir1==1)tBitmap->Canvas->Pen->Color=clRed;
		  else if(ir1==2)tBitmap->Canvas->Pen->Color=clLtGray;
		  else if(ir1==3)tBitmap->Canvas->Pen->Color=clBlue;
		  else if(ir1==4)tBitmap->Canvas->Pen->Color=clGreen;
		  else if(ir1==5)tBitmap->Canvas->Pen->Color=clLime;
		  else if(ir1==6)tBitmap->Canvas->Pen->Color=clNavy;
		  else if(ir1==7)tBitmap->Canvas->Pen->Color=clYellow;
		  else           tBitmap->Canvas->Pen->Color=clOlive;
		  tBitmap->Canvas->Pen->Width=2;
//
								  for(ip=0;ip<4;ip++){ptDraw[ip].x=int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+ip]]]+0.5f);
													  ptDraw[ip].y=ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+ip]]+1]+0.5f);
													 }
								  tBitmap->Canvas->Polygon(ptDraw,4-1);
								 }
							   else break;
							  }
////////////////// Insert weld direction arrow here  EFP 3/22/2012
///////////////////
////////////////////
xave=yave=zave=0.f;
for(ip1=0;ip1<wp.n_curr_sttEl[inw];ip1++)
  {for(in=0;in<8;in++){xave=xave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+0];
					   yave=yave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+1];
					   zave=zave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*inw+ip1]/10)+in]+2];
					  }
  }
//xave=xave/double(8*wp.n_curr_sttEl[inw]);yave=yave/double(8*wp.n_curr_sttEl[inw]);zave=zave/double(8*wp.n_curr_sttEl[inw]);
xave=xave/float(8*wp.n_curr_sttEl[inw]);yave=yave/float(8*wp.n_curr_sttEl[inw]);zave=zave/float(8*wp.n_curr_sttEl[inw]);
tBitmap->Canvas->Pen->Width=3;tBitmap->Canvas->Brush->Color=clWhite;tBitmap->Canvas->Pen->Color=clWhite;
tBitmap->Canvas->Ellipse(int(xave)-5,ClientHeight-(int(yave)-5),int(xave)+5,ClientHeight-(int(yave)+5));
tBitmap->Canvas->Brush->Color=clBlue;tBitmap->Canvas->Pen->Color=clBlue;
tBitmap->Canvas->MoveTo(int(xave),ClientHeight-int(yave));
ie1=wp.sttEles[wp.memWGa*inw+0]/10;iside1=wp.sttEles[wp.memWGa*inw+0]-10*ie1; // Use first start element face as normal
if(iside1==0){xc=  0.f;yc= -1.f;zc=  0.f;}else if(iside1==2){xc=  0.f;yc=  1.f;zc=  0.f;}else if(iside1==1){xc=  1.f;yc=  0.f;zc=  0.f;}else if(iside1==3){xc= -1.f;yc=  0.f;zc=  0.f;}else if(iside1==4){xc=  0.f;yc=  0.f;zc= -1.f;}else {xc=  0.f;yc=  0.f;zc=  1.f;}
STFISO8(3,xc,yc,zc,&DJD,HN,SN,SG,DJR,indat.nop1+MXNPEL*ie1,indat.c1); //EFP 3/24/2012
tBitmap->Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1]))); //GIANTS
tBitmap->Canvas->MoveTo(int(xave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.*HN[1])));
tBitmap->Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1])));
tBitmap->Canvas->LineTo(int(xave+25.f*(HN[0]-HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[1]-HN[0]))+(min(ClientWidth,ClientHeight)/200)*int(-50.*HN[1])));
////////////////////
///////////////////
//////////////////
						   }
   }
}
//---------------------------------------------------------------------------
void TForm1::parse_cdm3ff(char descript[],long ncol,int* nic,int* nrc,long iparse[],float parse[])
// This routine reads comma-delimited coords, driven by #numbers on line==ncol.
// It accommodates spaces before/after commas. It can handle an extra comma at end.
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,nline=0,ist=0,irflag=0;
 long dii=0,lse=0;float div=0.f,rse=0.f;char stchar[19],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d';stchar[17]=',';stchar[18]=' '; *nic=0;*nrc=0;isel= -1;
 for(nline=0;nline<ncol;nline++)
  {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;
   if( *nic)irflag=1;
   else irflag=0;
   for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
   for(ist=0;ist<25;ist++)
	  {isel++;
	   if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
	   else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
	   else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] || descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
	   else if(descript[isel] == stchar[10]){continue;}
	   else if(descript[isel] == stchar[0])
		   {inel=0;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[1])
		   {inel=1;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[2])
		   {inel=2;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[3])
		   {inel=3;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[4])
		   {inel=4;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[5])
		   {inel=5;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[6])
		   {inel=6;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[7])
		   {inel=7;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[8])
		   {inel=8;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[9])
		   {inel=9;
				   if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[17]){break;}
	   else if(descript[isel] == stchar[18]){if(nline==ncol-1)break;else continue;}
	   else break;
	  }
   if(irflag)
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}}}
	   div=1.f;for(ileft=0;ileft<nright;ileft++){div=div*10.f;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
	   if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;parse[*nrc]=rse;*nrc=*nrc+1;
	  }
   else
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}}}
	   dii=1;for(ileft=0;ileft<nright;ileft++){dii=dii*10;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
	   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;iparse[*nic]=lse;*nic=*nic+1;
	  }
  }
}
//---------------------------------------------------------------------------
void TForm1::parse_cdmQ(char descript[],int* nic,int* nrc,long iparse[],float parse[])
// Integers and/or float(E)/double(D) numbers delimited by ' ' or ','
// Read up to ncol "integer + real" or end of record (siza=record length without \n), whichever comes first.
// Delimited by comma, except at end. There may or may not be a space before the comma. (i.e. 7543 , 7544 , etc)
// CORRECTION: EFP 5/11/2010 for possible space before comma
// BIG siza BUG HERE
// Note: This routine can accommodate spaces befor/after commas. It can accommodate an extra comma at end.
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,ist=0,irflag=0,isw=0;
 long dii=0,lse=0;float div=0.f,rse=0.f;char stchar[18],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d';stchar[17]=','; *nic=0;*nrc=0;isel= -1;
//siza=sizeof(descript);
//honk<<sizeof(descript)<<" SSSSSSS "<<sizb<<"\n";

//for(ist=0;ist<100;ist++){//if(descript[isel]== \n)honk<<" LF\n";
//						 honk<<ist<<" "<<descript[ist]<<" descrit\n";
//						}
 isw=1;
 do {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
	 for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
////	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17] || descript[isel]=='\n'){isel--;break;}
////							 } //comma
//	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17]){isel--;break;}
//							 } //comma
	 for(ist=0;ist<25;ist++)
		{isel++;
		 if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
		 else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
		 else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] ||
				 descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
		 else if(descript[isel] == stchar[10])continue;
		 else if(descript[isel] == stchar[0])
			 {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[1])
			 {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[2])
			 {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[3])
			 {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[4])
			 {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[5])
			 {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[6])
			 {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[7])
			 {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[8])
			 {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[9])
			 {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
//		 else break;
		 else if(descript[isel] ==' ')continue;
		 else if(descript[isel] == stchar[17])break;
		 else {isw=0;break;}
		}
//if(isw){
	 if(irflag){for(ileft=0;ileft<nleft;ileft++)
				 {for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}
				 }
				div=1.f;for(ileft=0;ileft<nright;ileft++)
						 {div=div*10.f;
						  for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}
						 }
				vexp=0;for(ileft=0;ileft<nexp;ileft++)
						 {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
						 }
				if(vexp>37)vexp=37; // Revision
				div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
				if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;
				parse[*nrc]=rse;*nrc=*nrc+1;
			   }
	 else {for(ileft=0;ileft<nleft;ileft++)
			{for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}
			}
		   dii=1;for(ileft=0;ileft<nright;ileft++)
				   {dii=dii*10;
					for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}
				   }
		   vexp=0;for(ileft=0;ileft<nexp;ileft++)
				   {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
				   }
		   if(vexp>37)vexp=37; // Revision
		   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
		   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;
		   iparse[*nic]=lse;*nic=*nic+1;
		  }
//	   }
	}
// while (isel+1<siza && (*nic+ *nrc)<ncol);
 while (isw);
}
//---------------------------------------------------------------------------
void TForm1::parse_cdm(char descript[],long ncol,int* nic,int* nrc,long iparse[],float parse[])
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,nline=0,ist=0,irflag=0;
 long dii=0,lse=0;float div=0.f,rse=0.f;char stchar[17],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d'; *nic=0;*nrc=0;isel= -1;
 for(nline=0;nline<ncol;nline++)
  {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
   for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
   for(ist=0;ist<25;ist++)
	  {isel++;
	   if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
	   else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
	   else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] || descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
	   else if(descript[isel] == stchar[10])continue;
	   else if(descript[isel] == stchar[0])
		   {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[1])
		   {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[2])
		   {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[3])
		   {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[4])
		   {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[5])
		   {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[6])
		   {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[7])
		   {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[8])
		   {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[9])
		   {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else break;
	  }
   if(irflag)
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}}}
	   div=1.f;for(ileft=0;ileft<nright;ileft++){div=div*10.f;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
	   if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;parse[*nrc]=rse;*nrc=*nrc+1;
	  }
   else
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}}}
	   dii=1;for(ileft=0;ileft<nright;ileft++){dii=dii*10;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
	   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;iparse[*nic]=lse;*nic=*nic+1;
	  }
  }
}
//---------------------------------------------------------------------------
void TForm1::parse_cdmQn(char descript[],long ncol,int* nic,int* nrc,long iparse[],float parse[])
// Integers and/or float(E)/double(D) numbers delimited by ' ' or ','
// Read up to ncol "integer + real" or end of record (siza=record length without \n), whichever comes first.
// Delimited by comma, except at end. There may or may not be a space before the comma. (i.e. 7543 , 7544 , etc)
// CORRECTION: EFP 5/11/2010 for possible space before comma
// BIG siza BUG HERE
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,ist=0,irflag=0,isw=0;
 long dii=0,lse=0,ctcomma=0;float div=0.f,rse=0.f;char stchar[18],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d';stchar[17]=','; *nic=0;*nrc=0;isel= -1;
//siza=sizeof(descript);
//honk<<sizeof(descript)<<" SSSSSSS "<<sizb<<"\n";

//for(ist=0;ist<100;ist++){//if(descript[isel]== \n)honk<<" LF\n";
//						 honk<<ist<<" "<<descript[ist]<<" descrit\n";
//						}
 isw=1;
 do {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
	 for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
////	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17] || descript[isel]=='\n'){isel--;break;}
////							 } //comma
//	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17]){isel--;break;}
//							 } //comma
	 for(ist=0;ist<25;ist++)
		{isel++;
		 if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
		 else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
		 else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] ||
				 descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
		 else if(descript[isel] == stchar[10])continue;
		 else if(descript[isel] == stchar[0])
			 {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[1])
			 {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[2])
			 {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[3])
			 {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[4])
			 {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[5])
			 {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[6])
			 {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[7])
			 {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[8])
			 {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[9])
			 {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
//		 else break;
		 else if(descript[isel] ==' ')continue;
		 else if(descript[isel] == stchar[17]){
											   ctcomma++;if(ctcomma>=ncol)isw=0;
											   break;
											  }
		 else {isw=0;break;}
		}
//if(isw){
	 if(irflag){for(ileft=0;ileft<nleft;ileft++)
				 {for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}
				 }
				div=1.f;for(ileft=0;ileft<nright;ileft++)
						 {div=div*10.f;
						  for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}
						 }
				vexp=0;for(ileft=0;ileft<nexp;ileft++)
						 {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
						 }
				if(vexp>37)vexp=37; // Revision
				div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
				if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;
				parse[*nrc]=rse;*nrc=*nrc+1;
			   }
	 else {for(ileft=0;ileft<nleft;ileft++)
			{for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}
			}
		   dii=1;for(ileft=0;ileft<nright;ileft++)
				   {dii=dii*10;
					for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}
				   }
		   vexp=0;for(ileft=0;ileft<nexp;ileft++)
				   {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
				   }
		   if(vexp>37)vexp=37; // Revision
		   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
		   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;
		   iparse[*nic]=lse;*nic=*nic+1;
		  }
//	   }

//honk<<isel+1<<" "<<siza<<" FFFFFFFFF "<< *nic<<" "<< *nrc<<" "<<ncol<<"\n";


	}
// while (isel+1<siza && (*nic+ *nrc)<ncol);
 while (isw);
}
//---------------------------------------------------------------------------
void TForm1::parse_cdmQb(char descript[],int* nic,int* nrc,long iparse[],float parse[],long siza)
// Integers and/or float(E)/double(D) numbers delimited by ' '
// Read up to ncol "integer + real" or end of record (siza=record length without \n), whichever comes first.
// CORRECTION: EFP 5/11/2010 for possible space before comma
// BIG siza BUG HERE
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,ist=0,irflag=0,isw=0;
 long dii=0,lse=0;float div=0.f,rse=0.f;char stchar[18],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d';stchar[17]=','; *nic=0;*nrc=0;isel= -1;
//siza=sizeof(descript);
//honk<<sizeof(descript)<<" SSSSSSS "<<sizb<<"\n";

//for(ist=0;ist<100;ist++){//if(descript[isel]== \n)honk<<" LF\n";
//						 honk<<ist<<" "<<descript[ist]<<" descrit\n";
//						}
 isw=1;
 do {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
//	 for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
	 for(ist=0;ist<siza;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
////	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17] || descript[isel]=='\n'){isel--;break;}
////							 } //comma
//	 for(ist=0;ist<100;ist++){isel++;if(descript[isel]==stchar[17]){isel--;break;}
//							 } //comma
	 for(ist=0;ist<25;ist++)
		{isel++;

if(isel>siza)break;

		 if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
		 else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
		 else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] ||
				 descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
		 else if(descript[isel] == stchar[10])continue;
		 else if(descript[isel] == stchar[0])
			 {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[1])
			 {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[2])
			 {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[3])
			 {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[4])
			 {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[5])
			 {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[6])
			 {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[7])
			 {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[8])
			 {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
		 else if(descript[isel] == stchar[9])
			 {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
					 else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
					 else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}
					 else exit(0);
			 }
////		 else break;
//		 else if(descript[isel] ==' ')continue;
		 else if(descript[isel] ==' ')break;
//		 else if(descript[isel] == stchar[17])break;
		 else {isw=0;break;}
		}
//if(isw){
	 if(irflag){for(ileft=0;ileft<nleft;ileft++)
				 {for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}
				 }
				div=1.f;for(ileft=0;ileft<nright;ileft++)
						 {div=div*10.f;
						  for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}
						 }
				vexp=0;for(ileft=0;ileft<nexp;ileft++)
						 {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
						 }
				if(vexp>37)vexp=37; // Revision
				div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
				if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;
				parse[*nrc]=rse;*nrc=*nrc+1;
			   }
	 else {for(ileft=0;ileft<nleft;ileft++)
			{for(inel=0;inel<10;inel++)if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}
			}
		   dii=1;for(ileft=0;ileft<nright;ileft++)
				   {dii=dii*10;
					for(inel=1;inel<10;inel++)if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}
				   }
		   vexp=0;for(ileft=0;ileft<nexp;ileft++)
				   {for(inel=0;inel<10;inel++)if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}
				   }
		   if(vexp>37)vexp=37; // Revision
		   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
		   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;
		   iparse[*nic]=lse;*nic=*nic+1;
		  }
//	   }

//honk<<isel+1<<" "<<siza<<" FFFFFFFFF "<< *nic<<" "<< *nrc<<" "<<ncol<<"\n";


	}
// while (isel+1<siza && (*nic+ *nrc)<ncol);
 while (isw);
}
//---------------------------------------------------------------------------
int TForm1::parse_cdmV(char descript[],long ncol,int* nic,int* nrc,long iparse[],float parse[])
// Verify integer/float in first place (vflag integer-> -1, float->1)     EFP 8/26/2012
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,nline=0,ist=0,irflag=0,vflag=0;
 long dii=0,lse=0;float div=0.f,rse=0.f;char stchar[17],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d'; *nic=0;*nrc=0;isel= -1;
 for(nline=0;nline<ncol;nline++)
  {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
   for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
   for(ist=0;ist<25;ist++)
	  {isel++;
	   if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
	   else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
	   else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] || descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
	   else if(descript[isel] == stchar[10])continue;
	   else if(descript[isel] == stchar[0])
		   {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[1])
		   {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[2])
		   {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[3])
		   {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[4])
		   {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[5])
		   {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[6])
		   {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[7])
		   {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[8])
		   {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[9])
		   {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else break;
	  }
   if(irflag)
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}}}
	   div=1.f;for(ileft=0;ileft<nright;ileft++){div=div*10.f;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
	   if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;
	   parse[*nrc]=rse;*nrc=*nrc+1;
	   if(!vflag)vflag=1;
	  }
   else
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}}}
	   dii=1;for(ileft=0;ileft<nright;ileft++){dii=dii*10;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
	   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;
	   iparse[*nic]=lse;*nic=*nic+1;
	   if(!vflag)vflag= -1;
	  }
  }
 return vflag;
}
//---------------------------------------------------------------------------
int TForm1::parse_cdmVc(char descript[],long ncol,int* nic,int* nrc,long iparse[],float parse[])
// Verify integer/float in first place, comma delimited (vflag integer-> -1, float->1)     EFP 11/24/2012
// This is a questionable blend of parse_cdmV() & parse_cdmQn().
{int isel=0,inel=0,pre=0,ileft=0,nleft=0,nright=0,nexp=0,sign=0,esign=0,vexp=0,nline=0,ist=0,irflag=0,vflag=0//,isw=0
;
 long dii=0,lse=0,ctcomma=0;float div=0.f,rse=0.f;char stchar[18],cleft[25],cright[25],cexp[25];
 stchar[0]='0';stchar[1]='1';stchar[2]='2';stchar[3]='3';stchar[4]='4';
 stchar[5]='5';stchar[6]='6';stchar[7]='7';stchar[8]='8';stchar[9]='9';
 stchar[10]='+';stchar[11]='-';stchar[12]='.';stchar[13]='E';stchar[14]='e';
 stchar[15]='D';stchar[16]='d';stchar[17]=','; *nic=0;*nrc=0;isel= -1;//isw=1;
 for(nline=0;nline<ncol;nline++)
  {rse=0.f;lse=0;pre=nleft=nright=nexp=0;esign=1;sign=1;irflag=0;
   for(ist=0;ist<100;ist++){isel++;if(descript[isel] != ' '){isel--;break;}}
   for(ist=0;ist<25;ist++)
	  {isel++;
	   if(descript[isel] == stchar[11]){if(pre == 0)sign= -1;else esign= -1;}
	   else if(descript[isel] == stchar[12]){pre=1;irflag=1;}
	   else if(descript[isel] == stchar[13] || descript[isel] == stchar[14] || descript[isel] == stchar[15] || descript[isel] == stchar[16]){pre=2;irflag=1;}
	   else if(descript[isel] == stchar[10])continue;
	   else if(descript[isel] == stchar[0])
		   {inel=0;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[1])
		   {inel=1;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[2])
		   {inel=2;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[3])
		   {inel=3;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[4])
		   {inel=4;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[5])
		   {inel=5;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[6])
		   {inel=6;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[7])
		   {inel=7;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[8])
		   {inel=8;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
	   else if(descript[isel] == stchar[9])
		   {inel=9;if(pre == 0){nleft=nleft+1;cleft[nleft-1]=stchar[inel];}
				   else if(pre == 1){nright=nright+1;cright[nright-1]=stchar[inel];}
				   else if(pre == 2){nexp=nexp+1;cexp[nexp-1]=stchar[inel];}else exit(0);
		   }
//	   else break;
	   else if(descript[isel] ==' ')continue;
	   else if(descript[isel] == stchar[17]){ctcomma++;//if(ctcomma>=ncol)isw=0;
											 break;
											}
	   else {//isw=0;
			 break;}
	  }
   if(irflag)
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){rse=rse*10+float(inel);break;}}}
	   div=1.f;for(ileft=0;ileft<nright;ileft++){div=div*10.f;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){rse=rse+float(inel)/div;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   div=1.f;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)div=div*10.f;}
	   if(esign<0)rse=float(sign)*rse/div;else rse=float(sign)*rse*div;
	   parse[*nrc]=rse;*nrc=*nrc+1;
	   if(!vflag)vflag=1;
	  }
   else
	  {for(ileft=0;ileft<nleft;ileft++){for(inel=0;inel<10;inel++){if(cleft[ileft] == stchar[inel]){lse=lse*10+long(inel);break;}}}
	   dii=1;for(ileft=0;ileft<nright;ileft++){dii=dii*10;for(inel=1;inel<10;inel++){if(cright[ileft] == stchar[inel]){lse=lse+long(inel)/dii;break;}}}
	   vexp=0;for(ileft=0;ileft<nexp;ileft++){for(inel=0;inel<10;inel++){if(cexp[ileft] == stchar[inel]){vexp=vexp*10+inel;break;}}}
//
if(vexp>37)vexp=37;
//
	   dii=1;if(vexp>0){for(ileft=0;ileft<vexp;ileft++)dii=dii*10;}
	   if(esign<0)lse=long(sign)*lse/dii;else lse=long(sign)*lse*dii;
	   iparse[*nic]=lse;*nic=*nic+1;
	   if(!vflag)vflag= -1;
	  }
//   if(!isw)break;
  }
 return vflag;
}
//---------------------------------------------------------------------------
void TForm1::FDbase_indat(int isel,float shapeprod,int iplotflag,long nColRes)
// base_XXX & indat_XXX variables/arrays NOT passed in argument list (i.e. global) due to memory-allocation (i.e. treat as __fastcall)
// Global NDF,MXNPEL,MXNPEI,MAX_GID
{long i=0,ic=0,ie=0,dum=0;
 if(isel==0 || isel==1)
   {
//    if(Defshapeundefmesh1->Checked){
//////
//honk<<" Unsupported FDbase_indat Defshapeundefmesh1->Checked \n";exit(0);
//			   }
//else          {
	indat.nelt=base.nelt;indat.npoin=base.npoin;indat.nfsect=0;MXNPEI=MXNPEL;indat.nvfix=base.nvfix;indat.nedge=base.nedge;
	if(iplotflag){FDdynmem_manage(-2,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
				 }
	FDdynmem_manage(2,dum,dum,indat.npoin,indat.nelt,dum,dum,dum,dum,dum,dum,dum,indat.nfsect,MXNPEI);
//	if(!Undeformedmesh1->Checked)
//	  {for(i=0;i<base.npoin;i++){for(ic=0;ic<NDF;ic++)indat.c1[NDF*i+ic]=base.c1[NDF*i+ic]+shapeprod*base.res1[nColRes*i+ic];}
//	  }
//	else
for(i=0;i<NDF*base.npoin;i++)indat.c1[i]=base.c1[i];
	for(i=0;i<NDF*indat.npoin;i++)indat.bc1[i]=indat.c1[i];
	for(i=0;i<base.nelt;i++)indat.matno[i]=base.matno[i];for(i=0;i<MXNPEL*base.nelt;i++)indat.nop1[i]=base.nop1[i];
//  SuperEl
	for(i=0;i<base.nelt;i++)indat.nlp[i]=base.nlp[i];for(i=0;i<NDF*base.nelt;i++)indat.beta[i]=base.beta[i];
//
//			   }
//
   }
///////////////////// EFP 8/11/2010
 else if(isel==3)
   {
//	if(Defshapeundefmesh1->Checked){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"FDbase_indat unsupported ISEL",L"Halt",MB_OK);return;
//								   }
//	else       {
	indat.nelt=base.nelt;indat.npoin=base.npoin;indat.nfsect=0;MXNPEI=MXNPEL;indat.nvfix=base.nvfix;indat.nedge=base.nedge;
	if(iplotflag)FDdynmem_manage(-2,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	FDdynmem_manage(2,dum,dum,indat.npoin,indat.nelt,dum,dum,dum,dum,dum,dum,dum,indat.nfsect,MXNPEI);
//	if(!Undeformedmesh1->Checked)
//	  {for(i=0;i<base.npoin;i++){for(ic=0;ic<NDF;ic++)indat.c1[NDF*i+ic]=base.c1[NDF*i+ic]+shapeprod*base.res1[nColRes*i+ic];}
//	  }
//	else
	for(i=0;i<NDF*base.npoin;i++)indat.c1[i]=base.c1[i];
	for(i=0;i<NDF*indat.npoin;i++)indat.bc1[i]=indat.c1[i];
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	for(i=0;i<base.nelt;i++)indat.matno[i]=base.matno[i]; // This simple code used in place of below (EFP 9/10/2010)
	for(i=0;i<MXNPEL*base.nelt;i++)indat.nop1[i]=base.nop1[i];

//			   }
   }
//////////////////////////


 if(isel==2){for(i=0;i<nColRes*base.npoin;i++)indat.res1[i]=base.res1[i];
//			 if(Defshapeundefmesh1->Checked)for(i=0;i<nColRes*base.npoin;i++)indat.res1[i+nColRes*base.npoin]=base.res1[i];
			}
 if(isel<0 || isel>3){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"FDbase_indat unsupported ISEL",L"Halt",MB_OK);return;}
////////////////////
//for(i=0;i<indat.npoin;i++)honk<<(i+1)<<" "<<base.c1[NDF*i+0]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]
//									<<" bCiC "<<indat.c1[NDF*i+0]<<" "<<indat.c1[NDF*i+1]<<" "<<indat.c1[NDF*i+2]<<"\n";
////////////////////
}
//---------------------------------------------------------------------------
void TForm1::CmZoomIn2(long indat_npoin,float indat_c1[],float trans_op[],float prod_op[][3],float trans_zoom[],float prod_zoom[])
// Global NDF,iplotflag,zoomRect/etc
{long i=0;float zoomcenx=0.f,zoomceny=0.f,aspectr=0.f,aspectz=0.f,delx=0.f,dely=0.f,gdiff=0.f;
 zoomcenx=float(zoomRect.left+zoomRect.right)/2.f;zoomceny=float(2*ClientHeight-zoomRect.top-zoomRect.bottom)/2.f;
 delx=float(abs(zoomRect.left-zoomRect.right));dely=float(abs(zoomRect.top-zoomRect.bottom));
 aspectr=float(ClientHeight)/float(ClientWidth);
// Zoom area must exceed 2.5% of frame height or width
 if(dely< 0.025f*float(ClientHeight) || delx< 0.025f*float(ClientWidth))
   {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Zoom area too small.",L"Halt",MB_OK);return;
   }
 else aspectz=dely/delx;
 if(aspectz>aspectr)delx=dely/aspectr;else dely=delx*aspectr;
 gdiff=float(ClientWidth)/delx;
float xmx=0.f,xmn=0.f,ymx=0.f,ymn=0.f,zmx=0.f,zmn=0.f;
xmx= -1.E20f;xmn=1.E20f;ymx= -1.E20f;ymn=1.E20f;zmx= -1.E20f;zmn=1.E20f;
 for(i=0;i<indat_npoin;i++)
   {if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
   }
 for(i=0;i<indat_npoin;i++){indat_c1[NDF*i  ]=(indat_c1[NDF*i  ]-zoomcenx+delx/2.f)*gdiff;
							indat_c1[NDF*i+1]=(indat_c1[NDF*i+1]-zoomceny+dely/2.f)*gdiff;
							indat_c1[NDF*i+2]=indat_c1[NDF*i+2]*gdiff;
						   }
xmx= -1.E20f;xmn=1.E20f;ymx= -1.E20f;ymn=1.E20f;zmx= -1.E20f;zmn=1.E20f;
 for(i=0;i<indat_npoin;i++)
   {if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
   }
gl_indatMedian[0]=gl_indatMedian[0]*gdiff-float(zoomRect.left);
gl_indatMedian[1]=gl_indatMedian[1]*gdiff-float(zoomRect.top);
gl_indatMedian[2]=gl_indatMedian[2]*gdiff;
glGdiff=glGdiff*gdiff;
 trans_op[0]=trans_op[0]+(zoomcenx-delx/2.f)*prod_op[0][0]+(zoomceny-dely/2.f)*prod_op[0][1];
 trans_op[1]=trans_op[1]+(zoomcenx-delx/2.f)*prod_op[1][0]+(zoomceny-dely/2.f)*prod_op[1][1];
 trans_op[2]=trans_op[2]+(zoomcenx-delx/2.f)*prod_op[2][0]+(zoomceny-dely/2.f)*prod_op[2][1];
 prod_op[0][0]=prod_op[0][0]/gdiff; prod_op[0][1]=prod_op[0][1]/gdiff;
 prod_op[0][2]=prod_op[0][2]/gdiff; prod_op[1][0]=prod_op[1][0]/gdiff;
 prod_op[1][1]=prod_op[1][1]/gdiff; prod_op[1][2]=prod_op[1][2]/gdiff;
 prod_op[2][0]=prod_op[2][0]/gdiff; prod_op[2][1]=prod_op[2][1]/gdiff;prod_op[2][2]=prod_op[2][2]/gdiff;
 trans_zoom[0]=trans_zoom[0]+prod_zoom[0]*(zoomcenx-delx/2.f);trans_zoom[1]=trans_zoom[1]+prod_zoom[1]*(zoomceny-dely/2.f);
 prod_zoom[0]=prod_zoom[0]/gdiff;prod_zoom[1]=prod_zoom[1]/gdiff;prod_zoom[2]=prod_zoom[2]/gdiff;iplotflag=2;Invalidate();
}
//---------------------------------------------------------------------------
void TForm1::FDrotate(int transpose,long indat_npoin,float indat_c1[],float rangle[])
// Global NDF
{long i=0;float sumx=0.f,sumy=0.f,sumz=0.f,cenx=0.f,ceny=0.f,cenz=0.f,xmx= -1.E20f,xmn=1.E20f,ymx= -1.E20f,ymn=1.E20f,zmx= -1.E20f,zmn=1.E20f,
  r11=0.f,r12=0.f,r13=0.f,r21=0.f,r22=0.f,r23=0.f,r31=0.f,r32=0.f,r33=0.f,pix=4.f*atan(1.f)/180.f;
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
 cenx=gl_indatMedian[0];ceny=gl_indatMedian[1];cenz=gl_indatMedian[2];
 r11=  cos(pix*rangle[1])*cos(pix*rangle[2]);
 r12= -cos(pix*rangle[1])*sin(pix*rangle[2]);r13=  sin(pix*rangle[1]);
 r21=  cos(pix*rangle[0])*sin(pix*rangle[2])+sin(pix*rangle[0])*sin(pix*rangle[1])*cos(pix*rangle[2]);
 r22=  cos(pix*rangle[0])*cos(pix*rangle[2])-sin(pix*rangle[0])*sin(pix*rangle[1])*sin(pix*rangle[2]);
 r23= -sin(pix*rangle[0])*cos(pix*rangle[1]);
 r31=  sin(pix*rangle[0])*sin(pix*rangle[2])-cos(pix*rangle[0])*sin(pix*rangle[1])*cos(pix*rangle[2]);
 r32=  sin(pix*rangle[0])*cos(pix*rangle[2])+cos(pix*rangle[0])*sin(pix*rangle[1])*sin(pix*rangle[2]);
 r33=  cos(pix*rangle[0])*cos(pix*rangle[1]);
 if(transpose){float temp=r12;r12=r21;r21=temp;temp=r13;r13=r31;r31=temp;temp=r23;r23=r32;r32=temp;}
 xmx= -1.E20f;xmn=1.E20f;ymx= -1.E20f;ymn=1.E20f;zmx= -1.E20f;zmn=1.E20f;
 for(i=0;i<indat_npoin;i++)
   {if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
   }
 xmx= -1.E20f;xmn=1.E20f;ymx= -1.E20f;ymn=1.E20f;zmx= -1.E20f;zmn=1.E20f;
 for(i=0;i<indat_npoin;i++)
   {sumx=r11*(indat_c1[NDF*i]-cenx)+r12*(indat_c1[NDF*i+1]-ceny)+r13*(indat_c1[NDF*i+2]-cenz);
	sumy=r21*(indat_c1[NDF*i]-cenx)+r22*(indat_c1[NDF*i+1]-ceny)+r23*(indat_c1[NDF*i+2]-cenz);
	sumz=r31*(indat_c1[NDF*i]-cenx)+r32*(indat_c1[NDF*i+1]-ceny)+r33*(indat_c1[NDF*i+2]-cenz);
	indat_c1[NDF*i]=sumx;indat_c1[NDF*i+1]=sumy;indat_c1[NDF*i+2]=sumz;
	if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
   }
 for(i=0;i<indat_npoin;i++)
   {indat_c1[NDF*i  ]=indat_c1[NDF*i  ]+gl_indatMedian[0];
	indat_c1[NDF*i+1]=indat_c1[NDF*i+1]+gl_indatMedian[1];
	indat_c1[NDF*i+2]=indat_c1[NDF*i+2]+gl_indatMedian[2];
   }
 xmx= -1.E20f;xmn=1.E20f;ymx= -1.E20f;ymn=1.E20f;zmx= -1.E20f;zmn=1.E20f;
 for(i=0;i<indat_npoin;i++)
   {if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
   }
Screen->Cursor=Save_Cursor;
}
//---------------------------------------------------------------------------
void TForm1::FDrotate_axisplot(int transpose,float rangle[])
// Global NDF
{long i=0;float sumx=0.f,sumy=0.f,sumz=0.f,cenx=0.f,ceny=0.f,cenz=0.f,
  r11=0.f,r12=0.f,r13=0.f,r21=0.f,r22=0.f,r23=0.f,r31=0.f,r32=0.f,r33=0.f,pix=4.f*atan(1.f)/180.f;
 cenx=ceny=cenz=0.f;
 r11=  cos(pix*rangle[1])*cos(pix*rangle[2]);
 r12= -cos(pix*rangle[1])*sin(pix*rangle[2]);r13=  sin(pix*rangle[1]);
 r21=  cos(pix*rangle[0])*sin(pix*rangle[2])+sin(pix*rangle[0])*sin(pix*rangle[1])*cos(pix*rangle[2]);
 r22=  cos(pix*rangle[0])*cos(pix*rangle[2])-sin(pix*rangle[0])*sin(pix*rangle[1])*sin(pix*rangle[2]);
 r23= -sin(pix*rangle[0])*cos(pix*rangle[1]);
 r31=  sin(pix*rangle[0])*sin(pix*rangle[2])-cos(pix*rangle[0])*sin(pix*rangle[1])*cos(pix*rangle[2]);
 r32=  sin(pix*rangle[0])*cos(pix*rangle[2])+cos(pix*rangle[0])*sin(pix*rangle[1])*sin(pix*rangle[2]);
 r33=  cos(pix*rangle[0])*cos(pix*rangle[1]);
 if(transpose){float temp=r12;r12=r21;r21=temp;temp=r13;r13=r31;r31=temp;temp=r23;r23=r32;r32=temp;}
 for(i=0;i<7;i++)
   {sumx=cenx+r11*ap.x[i]+r12*ap.y[i]+r13*ap.z[i];
	sumy=ceny+r21*ap.x[i]+r22*ap.y[i]+r23*ap.z[i];
	sumz=cenz+r31*ap.x[i]+r32*ap.y[i]+r33*ap.z[i];
	ap.x[i]=sumx;ap.y[i]=sumy;ap.z[i]=sumz;
   }
}
//---------------------------------------------------------------------------
void TForm1::maskelemPolyhea(int flag)
// Subroutine to hide polygon-surrounded-elements from mesh visibility (flag != 0), or to show only them (flag=0)
// This does NOT section the mesh.
{int *polytri=NULL,tricount=0,pc=0;
 long in=0,ie=0,eltype=0,bscode=0,node=0,t3=1000,t5=100000,t7=10000000;
 float xavg=0.f,yavg=0.f;
 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor= crHourGlass;
////////////////////
//for(in=0;in<indat.npoin;in++)honk<<(in+1)<<" "<<base.c1[NDF*in+0]<<" "<<base.c1[NDF*in+1]<<" "<<base.c1[NDF*in+2]
//                                    <<" bCiC "<<indat.c1[NDF*in+0]<<" "<<indat.c1[NDF*in+1]<<" "<<indat.c1[NDF*in+2]<<"\n";
////////////////////
	  polytri=new int[2*3*polypts*polypts];if(polytri==NULL){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient dynamic memory for polygon",L"Return",MB_OK);return;}
	  tricount=0;est_polytri(polytri,&tricount);
	  for(ie=0;ie<indat.nelt;ie++)
		{eltype=indat.matno[ie]/t7;bscode=(indat.matno[ie]-eltype*t7)/t5;node=(indat.matno[ie]-eltype*t7-bscode*t5)/t3;
		 xavg=yavg=0.f;for(in=0;in<node;in++){xavg=xavg+float(indat.c1[NDF*indat.nop1[MXNPEL*ie+in]  ]);
											  yavg=yavg+float(indat.c1[NDF*indat.nop1[MXNPEL*ie+in]+1]);
											 }
		 xavg=xavg/float(node);yavg=yavg/float(node);
		 pc=poly_contain(polytri,tricount,xavg,yavg);
		 if((flag && !pc) || (!flag && pc))base.arELEM[ie]=0;
		}
	  delete [] polytri;
if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
else {
	  FDcomp_nGID(indat.nelt,&nGID,arGID);
	  FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);
	 }
////////////////////
FD_LButtonstatus=11;stateVFT=2;
////////////////////
	  iPaintyesno=iPaintyesno-10*(iPaintyesno/10) +10;iCullyesno=0;iCircleplot=1;Invalidate();
Screen->Cursor=Save_Cursor;
}
//---------------------------------------------------------------------------
void TForm1::est_polytri(int polytri[],int *tricount)
{int ip=0,ipp=0,ippp=0,ipc=0,signp=0,signm=0,signpd=0,signmd=0,indicsign=0,iter=0,lolim=0,tri_idic[MAX_POLYPOINTS],ntri_idic[MAX_POLYPOINTS],psearch=polypts,pscount=0,ipstart=0;
 float dx1=0.f,dy1=0.f,dx3=0.f,dy3=0.f,norm=0.f,xave=0.f,yave=0.f;
 for(ip=0;ip<polypts;ip++)tri_idic[ip]=ip;
 for(iter=0;iter<polypts;iter++)
   {signp=0;signm=0;
	for(ip=0;ip<psearch;ip++)
	  {ipp=ip+1;if(ipp>=psearch)ipp=0;ippp=ipp+1;if(ippp>=psearch)ippp=0;
	   dx1=float(polycord[2*tri_idic[ipp]]-polycord[2*tri_idic[ip]]);dy1=float(polycord[2*tri_idic[ipp]+1]-polycord[2*tri_idic[ip]+1]);
	   norm=sqrt(dx1*dx1+dy1*dy1);dx1=dx1/norm;dy1=dy1/norm;
	   dx3=float(polycord[2*tri_idic[ippp]]-polycord[2*tri_idic[ip]]);dy3=float(polycord[2*tri_idic[ippp]+1]-polycord[2*tri_idic[ip]+1]);
	   norm=sqrt(dx3*dx3+dy3*dy3);dx3=dx3/norm;dy3=dy3/norm;
	   if(dx1*dy3-dx3*dy1 >=0.)signp++;else signm++;
	  }
	if(signp>signm)indicsign=1;else if(signp<signm)indicsign= -1;
	else
	  {xave=yave=0.f;
	   for(ip=0;ip<psearch;ip++){xave=xave+float(polycord[2*tri_idic[ip]]);yave=yave+float(polycord[2*tri_idic[ip]+1]);}
	   xave=xave/float(psearch);yave=yave/float(psearch);signpd=signmd=0;
	   for(ip=0;ip<psearch;ip++)
		 {ipp=ip+1;if(ipp>=psearch)ipp=0;
		  dx1=float(polycord[2*tri_idic[ipp]]-polycord[2*tri_idic[ip]]);dy1=float(polycord[2*tri_idic[ipp]+1]-polycord[2*tri_idic[ip]+1]);
		  norm=sqrt(dx1*dx1+dy1*dy1);dx1=dx1/norm;dy1=dy1/norm;
		  dx3=xave-float(polycord[2*tri_idic[ip]]);dy3=yave-float(polycord[2*tri_idic[ip]+1]);
		  norm=sqrt(dx3*dx3+dy3*dy3);dx3=dx3/norm;dy3=dy3/norm;
		  if(dx1*dy3-dx3*dy1>=0.f)signpd++;else signmd++;
		 }
	   if(signpd>signmd)indicsign=1;else indicsign= -1;
	  }
//
	for(ip=0;ip<psearch;ip++)
	  {ipp=ip+1;if(ipp>=psearch)ipp=0;ippp=ipp+1;if(ippp>=psearch)ippp=0;
	   dx1=float(polycord[2*tri_idic[ipp]]-polycord[2*tri_idic[ip]]);dy1=float(polycord[2*tri_idic[ipp]+1]-polycord[2*tri_idic[ip]+1]);
	   norm=sqrt(dx1*dx1+dy1*dy1);dx1=dx1/norm;dy1=dy1/norm;
	   dx3=float(polycord[2*tri_idic[ippp]]-polycord[2*tri_idic[ip]]);dy3=float(polycord[2*tri_idic[ippp]+1]-polycord[2*tri_idic[ip]+1]);
	   norm=sqrt(dx3*dx3+dy3*dy3);dx3=dx3/norm;dy3=dy3/norm;
	   if(dx1*dy3-dx3*dy1>=0.f && indicsign<0)break;if(dx1*dy3-dx3*dy1<0. && indicsign>0)break;
	  }
	ipstart=ipp;pscount=0;lolim=1000;ip=0;
	for(ipc=0;ipc<psearch;ipc++)
	   {if(ip>=psearch)break;
		ipp=ipstart+1;if(ipp>=psearch)ipp=0;ippp=ipp+1;if(ippp>=psearch)ippp=0;
		dx1=float(polycord[2*tri_idic[ipp]]-polycord[2*tri_idic[ipstart]]);dy1=float(polycord[2*tri_idic[ipp]+1]-polycord[2*tri_idic[ipstart]+1]);
		norm=sqrt(dx1*dx1+dy1*dy1);dx1=dx1/norm;dy1=dy1/norm;
		dx3=float(polycord[2*tri_idic[ippp]]-polycord[2*tri_idic[ipstart]]);dy3=float(polycord[2*tri_idic[ippp]+1]-polycord[2*tri_idic[ipstart]+1]);
		norm=sqrt(dx3*dx3+dy3*dy3);dx3=dx3/norm;dy3=dy3/norm;
		if(dx1*dy3-dx3*dy1 >=0.f && indicsign>0 && ippp!=lolim)
		 {polytri[6* *tricount  ]=polycord[2*tri_idic[ipstart]];polytri[6* *tricount+1]=polycord[2*tri_idic[ipstart]+1];
		  polytri[6* *tricount+2]=polycord[2*tri_idic[ipp]];polytri[6* *tricount+3]=polycord[2*tri_idic[ipp]+1];
		  polytri[6* *tricount+4]=polycord[2*tri_idic[ippp]];polytri[6* *tricount+5]=polycord[2*tri_idic[ippp]+1];
		  ntri_idic[pscount]=tri_idic[ippp];pscount++;if(lolim>ipp)lolim=ipp;
		  *tricount= *tricount+1;ip=ip+2;ipstart=ipstart+2;
		  if(ipstart==psearch)ipstart=0;else if(ipstart==psearch+1)ipstart=1;
		 }
		else if(dx1*dy3-dx3*dy1 <0.f && indicsign<0 && ippp!=lolim)
		 {polytri[6* *tricount  ]=polycord[2*tri_idic[ipstart]];polytri[6* *tricount+1]=polycord[2*tri_idic[ipstart]+1];
		  polytri[6* *tricount+2]=polycord[2*tri_idic[ipp]];polytri[6* *tricount+3]=polycord[2*tri_idic[ipp]+1];
		  polytri[6* *tricount+4]=polycord[2*tri_idic[ippp]];polytri[6* *tricount+5]=polycord[2*tri_idic[ippp]+1];
		  ntri_idic[pscount]=tri_idic[ippp];pscount++;if(lolim>ipp)lolim=ipp;
		  *tricount= *tricount+1;ip=ip+2;ipstart=ipstart+2;
		  if(ipstart==psearch)ipstart=0;else if(ipstart==psearch+1)ipstart=1;
		 }
		else {ntri_idic[pscount]=tri_idic[ipp];pscount++;ip++;ipstart=ipstart+1;if(ipstart>=psearch)ipstart=0;}
	   }
	if(pscount<3)break;psearch=pscount;for(ipc=0;ipc<psearch;ipc++)tri_idic[ipc]=ntri_idic[ipc];
   }
}
//---------------------------------------------------------------------------
int TForm1::poly_contain(int polytri[],int tricount,float xnor,float ynor)
{int ic=0,ip=0,icp=0,ipolypts=0;float dx1=0.f,dy1=0.f,dx2=0.f,dy2=0.f,dx3=0.f,dy3=0.f,norm=0.f,xave=0.f,yave=0.f;
 for(ip=0;ip<tricount;ip++)
   {xave=yave=0.f;for(ic=0;ic<3;ic++){xave=xave+float(polytri[6*ip+2*ic]);yave=yave+float(polytri[6*ip+2*ic+1]);}xave=xave/3.f;yave=yave/3.f;ipolypts=0;
	for(ic=0;ic<3;ic++)
	  {icp=ic+1;if(icp>=3)icp=0;
	   dx1=float(polytri[6*ip+2*icp]-polytri[6*ip+2*ic]);dy1=float(polytri[6*ip+2*icp+1]-polytri[6*ip+2*ic+1]);
	   norm=sqrt(dx1*dx1+dy1*dy1);if(norm<0.001f)norm=0.001f;dx1=dx1/norm;dy1=dy1/norm;
	   dx2=xave-float(polytri[6*ip+2*ic]);dy2=yave-float(polytri[6*ip+2*ic+1]);
	   norm=sqrt(dx2*dx2+dy2*dy2);if(norm<0.001f)norm=0.001f;dx2=dx2/norm;dy2=dy2/norm;
	   dx3=xnor-float(polytri[6*ip+2*ic]);dy3=ynor-float(polytri[6*ip+2*ic+1]);
	   norm=sqrt(dx3*dx3+dy3*dy3);if(norm<0.001f)norm=0.001f;dx3=dx3/norm;dy3=dy3/norm;
	   if((dx1*dy2-dx2*dy1)*(dx1*dy3-dx3*dy1)>=0.f)ipolypts++;
	  }
	if(ipolypts>=3)return ip+1;
   }
 return 0;
}
//---------------------------------------------------------------------------
long TForm1::FDnode_interrog1(int X,int Y,long indat_nop1[],long indat_matno[],float indat_c1[],long nFace,long arbFace[])
// Global NDF,MXNPEI
// Policy: Consider only nodes on visible facets nFace (i.e. hidden nodes will never be selected).
//         Use highest-z node (iprez) within TOL graphical distance of (X,Y). If unavailable, use nearest node found (iprec).
{int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
 int gdata6[20]={0,1,4,3,
				 1,2,5,4,
				 2,0,3,5,
				 0,2,1,0,
				 3,4,5,0};
 int gdata4[12]={0,2,1,
				 1,2,3,
				 2,0,3,
				 3,0,1};
 int is=0,npts=0;long iprec=0,iprez=0,ip=0,ic=0,ie=0,fType=0,iside=0,eltype=0,t3=1000,t7=10000000;
 float diff=0.f,diffmin=0.f,zmax=0.f,TOL=5.f,TOLSQ=TOL*TOL;diffmin=1.E28f;zmax= -1.e28f;iprez= -1; // Note that TOL measures graphical units (i.e. screen)
 for(ic=0;ic<nFace;ic++)
   {ie=arbFace[ic]/t3;fType=(arbFace[ic]-ie*t3)/10;iside=arbFace[ic]-ie*t3-fType*10;eltype=indat_matno[ie]/t7;
	if(eltype==5)for(is=0;is<3;is++){ip=indat_nop1[MXNPEI*ie+gdata4[3*iside+is]];
									 diff=(indat_c1[NDF*ip  ]-float(X))*(indat_c1[NDF*ip  ]-float(X))+(indat_c1[NDF*ip+1]-float(ClientHeight-Y))*(indat_c1[NDF*ip+1]-float(ClientHeight-Y));
									 if(diff<TOLSQ && zmax<indat_c1[NDF*ip+2]){iprez=ip;zmax=indat_c1[NDF*iprez+2];}else if(diffmin>diff){diffmin=diff;iprec=ip;}
									}
	else if(eltype==7){if(iside>2)npts=3;else npts=4;
					   for(is=0;is<npts;is++){ip=indat_nop1[MXNPEI*ie+gdata6[4*iside+is]];
											  diff=(indat_c1[NDF*ip  ]-float(X))*(indat_c1[NDF*ip  ]-float(X))+(indat_c1[NDF*ip+1]-float(ClientHeight-Y))*(indat_c1[NDF*ip+1]-float(ClientHeight-Y));
											  if(diff<TOLSQ && zmax<indat_c1[NDF*ip+2]){iprez=ip;zmax=indat_c1[NDF*iprez+2];}else if(diffmin>diff){diffmin=diff;iprec=ip;}
											 }
					  }
	else for(is=0;is<4;is++){ip=indat_nop1[MXNPEI*ie+gdata8[4*iside+is]];
							 diff=(indat_c1[NDF*ip  ]-float(X))*(indat_c1[NDF*ip  ]-float(X))+(indat_c1[NDF*ip+1]-float(ClientHeight-Y))*(indat_c1[NDF*ip+1]-float(ClientHeight-Y));
							 if(diff<TOLSQ && zmax<indat_c1[NDF*ip+2]){iprez=ip;zmax=indat_c1[NDF*iprez+2];}else if(diffmin>diff){diffmin=diff;iprec=ip;}
							}
   }
 if(iprez>=0)return iprez;else return iprec;
}
//---------------------------------------------------------------------------
void TForm1::FDelem_interrog1(int X,int Y,long *eprec,long *sprec,float *RN1,float *RN2,float *RN3,
  long indat_nop1[],long indat_matno[],float indat_c1[],long nFace,long arbFace[])
// Global NDF,MXNPEI
//{int gdata8[24]={0,1,5,4,
//				 1,2,6,5,
//				 3,2,6,7,
//				 0,3,7,4,
//				 0,1,2,3,
//				 4,5,6,7};
{int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
 int gdata6[20]={0,1,4,3,
				 1,2,5,4,
				 2,0,3,5,
				 0,2,1,0,
				 3,4,5,0};
 int gdata4[12]={0,2,1,
				 1,2,3,
				 2,0,3,
				 3,0,1};
 int is=0,isp=0,npts=0,icount=0;long ic=0,ie=0,fType=0,iside=0,eltype=0,bscode=0,node=0,t3=1000,t5=100000,t7=10000000;
 float xave=0.f,yave=0.f,zave=0.f,xmin=0.f,xmax=0.f,ymin=0.f,ymax=0.f,zmax=0.f,cave=0.f,cs=0.f,cpt=0.f,ZP=0.f,RN1s=0.f,RN2s=0.f,RN3s=0.f,
   dx=0.f,dy=0.f,TOLSUB=0.0001f;zmax= -1.E28f; *eprec= -1;
 for(ic=0;ic<nFace;ic++)
   {ie=arbFace[ic]/t3;fType=(arbFace[ic]-ie*t3)/10;iside=arbFace[ic]-ie*t3-fType*10;
	eltype=indat_matno[ie]/t7;bscode=(indat_matno[ie]-eltype*t7)/t5;node=(indat_matno[ie]-eltype*t7-bscode*t5)/t3;
	xmin=ymin=1.E28f;xmax=ymax= -1.E28f;
	if(eltype==5){npts=3;for(is=0;is<npts;is++){if(xmin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ])xmin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ];
												if(xmax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ])xmax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ];
												if(ymin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1])ymin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1];
												if(ymax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1])ymax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1];
											   }
				 }
	else if(eltype==7){if(iside>2)npts=3;else npts=4;
					   for(is=0;is<npts;is++){if(xmin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ])xmin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ];
											  if(xmax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ])xmax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ];
											  if(ymin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1])ymin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1];
											  if(ymax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1])ymax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1];
											 }
					  }
	else {npts=4;for(is=0;is<npts;is++){if(xmin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ])xmin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ];
										if(xmax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ])xmax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ];
										if(ymin>indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1])ymin=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1];
										if(ymax<indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1])ymax=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1];
									   }
		 }
	if(float(X)>=xmin)
	  {if(float(X)<=xmax)
		 {if(float(ClientHeight-Y)>=ymin)
			{if(float(ClientHeight-Y)<=ymax)
			   {xave=yave=zave=0.f;
				if(eltype==5)for(is=0;is<npts;is++){xave=xave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ];
													yave=yave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1];
													zave=zave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+2];
												   }
				else if(eltype==7)for(is=0;is<npts;is++){xave=xave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ];
														 yave=yave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1];
														 zave=zave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+2];
														}
				else for(is=0;is<npts;is++){xave=xave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ];
											yave=yave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1];
											zave=zave+indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+2];
										   }
				xave=xave/float(npts);yave=yave/float(npts);zave=zave/float(npts);icount=0;
				for(is=0;is<npts;is++)
				  {isp=is+1;if(isp>=npts)isp=0;
				   if(eltype==5){dx=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+isp]]  ]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ];
								 dy=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+isp]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1];
								 if(fabs(dx)>1.e-10f){cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ]*dy/dx;
													 cave=yave-xave*dy/dx;cpt=float(ClientHeight-Y)-float(X)*dy/dx;
													}
								 else {cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata4[3*iside+is]]  ];cave=xave;cpt=float(X);}
								}
				   else if(eltype==7){dx=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+isp]]  ]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ];
									  dy=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+isp]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1];
									  if(fabs(dx)>1.e-10f){cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ]*dy/dx;
														  cave=yave-xave*dy/dx;cpt=float(ClientHeight-Y)-float(X)*dy/dx;
														 }
									  else {cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata6[4*iside+is]]  ];cave=xave;cpt=float(X);}
									 }
				   else {dx=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+isp]]  ]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ];
						 dy=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+isp]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1];
						 if(fabs(dx)>1.e-10f){cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]+1]-indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ]*dy/dx;
											 cave=yave-xave*dy/dx;cpt=float(ClientHeight-Y)-float(X)*dy/dx;
											}
						 else {cs=indat_c1[NDF*indat_nop1[MXNPEI*ie+gdata8[4*iside+is]]  ];cave=xave;cpt=float(X);}
						}
				   if((cave-cs)*(cpt-cs)>0.f)icount++;else break;
				  }
				if(icount==npts){homsubm(ie,iside,float(X),float(ClientHeight-Y),&ZP,&RN1s,&RN2s,&RN3s,xave,yave,zave,node,eltype,indat_nop1+MXNPEI*ie,indat_c1,TOLSUB);
								 if(zmax<ZP){zmax=ZP; *eprec=ie; *sprec=iside; *RN1=RN1s; *RN2=RN2s; *RN3=RN3s;}
								}
			   }
			}
		 }
	  }
   }
}
//---------------------------------------------------------------------------
void TForm1::FDelem_interrogpt1(long ie,long iside,long eltype,long node,float xnat,float ynat,float znat,
  float *xave,float *yave,float *zave,float *xnor,float *ynor,float *znor,float *rave,float *area,
  long base_nop1[],long indat_nop1[],float base_c1[],float indat_result[])
// Global NDF,MXNPEL,MXNPEI
{int is=0;
 float xn=0.f,yn=0.f,zn=0.f,DJD=0.f,HN[9],SN[MXNPELS],SG[NDF*MXNPELS],DJR[9],PRMT=1.E-5f;
 *xave= *yave= *zave= *rave=0.f;
	if(eltype==5)
	  {if(node==4)STFISO4(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO10(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   for(is=0;is<node;is++){*xave= *xave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]  ];
							  *yave= *yave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+1];
							  *zave= *zave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+2];
							  if(fl_feres)*rave= *rave+SN[is]*indat_result[indat_nop1[MXNPEI*ie+is]];
							 }
	   if(iside==0){if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat; // STFISO4(ISL,G,E,T...) where S=1-G-E-T=0
					if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
					if(znat<PRMT)zn=2.f*PRMT;else zn=znat;
					zn=1.f-xn-yn;
				   }
	   else if(iside==1){xn=0.f; // G=0
						 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
						 if(znat<PRMT)zn=2.f*PRMT;else zn=znat;
						}
	   else if(iside==2){if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat; // E=0
						 yn=0.f;
						 if(znat<PRMT)zn=2.f*PRMT;else zn=znat;
						}
	   else {if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat; // T=0
			 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
			 zn=0.f;
			}
	   if(node==4)STFISO4(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO10(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   *area=DJD; //Check this tetra element facet area
	  }
	else if(eltype==7)
	  {if(node==6)STFISO6(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO15(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   for(is=0;is<node;is++){*xave= *xave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]  ];
							  *yave= *yave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+1];
							  *zave= *zave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+2];
							  if(fl_feres)*rave= *rave+SN[is]*indat_result[indat_nop1[MXNPEI*ie+is]];
							 }
	   if     (iside==0){if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat;
						 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
						 if(znat< -1.f+2.f*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						 yn=1.f-xn;
						}
	   else if(iside==1){xn=0.f;
						 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
						 if(znat< -1.f+2.f*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						}
	   else if(iside==2){if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat;
						 yn=0.f;
						 if(znat< -1.f+2.f*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						}
	   else if(iside==3){if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat;
						 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
						 zn= -1.f;
						}
	   else {if(xnat<PRMT)xn=2.f*PRMT;else xn=xnat;
			 if(ynat<PRMT)yn=2.f*PRMT;else yn=ynat;
			 zn=1.f;
			}
	   if(node==6)STFISO6(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO15(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   if(iside<3) *area=DJD;else *area=DJD/2.f; //Check this wedge element facet area
	  }
	else
	  {if(node==8)STFISO8(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO20(1,xnat,ynat,znat,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   for(is=0;is<node;is++){*xave= *xave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]  ];
							  *yave= *yave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+1];
							  *zave= *zave+SN[is]*base_c1[NDF*indat_nop1[MXNPEI*ie+is]+2];
							  if(fl_feres)*rave= *rave+SN[is]*indat_result[indat_nop1[MXNPEI*ie+is]];
							 }
	   if(iside==0){if(xnat< -1.f+2.f*PRMT)xn= -1.f+2.f*PRMT;else if(xnat> 1.f-2.f*PRMT)xn=1.f-2.f*PRMT;else xn=xnat;
					yn= -1.f;
					if(znat< -1.f+2.f*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
				   }
	   else if(iside==1){xn=1.f;
						 if(ynat< -1.f+2.*PRMT)yn= -1.f+2.f*PRMT;else if(ynat> 1.f-2.f*PRMT)yn=1.f-2.f*PRMT;else yn=ynat;
						 if(znat< -1.f+2.*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						}
	   else if(iside==2){if(xnat< -1.f+2.*PRMT)xn= -1.f+2.f*PRMT;else if(xnat> 1.f-2.f*PRMT)xn=1.f-2.f*PRMT;else xn=xnat;
						 yn=1.f;
						 if(znat< -1.f+2.*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						}
	   else if(iside==3){xn= -1.f;
						 if(ynat< -1.f+2.*PRMT)yn= -1.f+2.f*PRMT;else if(ynat> 1.f-2.f*PRMT)yn=1.f-2.f*PRMT;else yn=ynat;
						 if(znat< -1.f+2.*PRMT)zn= -1.f+2.f*PRMT;else if(znat> 1.f-2.f*PRMT)zn=1.f-2.f*PRMT;else zn=znat;
						}
	   else if(iside==4){if(xnat< -1.f+2.*PRMT)xn= -1.f+2.f*PRMT;else if(xnat> 1.f-2.f*PRMT)xn=1.f-2.f*PRMT;else xn=xnat;
						 if(ynat< -1.f+2.*PRMT)yn= -1.f+2.f*PRMT;else if(ynat> 1.f-2.f*PRMT)yn=1.f-2.f*PRMT;else yn=ynat;
						 zn= -1.f;
						}
	   else {if(xnat< -1.f+2.f*PRMT)xn= -1.f+2.f*PRMT;else if(xnat> 1.f-2.f*PRMT)xn=1.f-2.f*PRMT;else xn=xnat;
			 if(ynat< -1.f+2.f*PRMT)yn= -1.f+2.f*PRMT;else if(ynat> 1.f-2.f*PRMT)yn=1.f-2.f*PRMT;else yn=ynat;
			 zn=1.f;
			}
	   if(node==8)STFISO8(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   else       STFISO20(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,base_nop1+MXNPEL*ie,base_c1);
	   *area=4.f*DJD;
	  }
 *xnor=HN[0]; *ynor=HN[1]; *znor=HN[2];
}
//---------------------------------------------------------------------------
void TForm1::homsubm(long ie,long kside,float XP,float YP,float *ZP,float *RN1,float *RN2,float *RN3,
  float xcens,float ycens,float zcens,long node,long eltype,long nop1[],float c1[],float TOL)
// TBD: n20/15/10 elements  (It may be necessary to increase MNITER for quads)
// eleng1/2/3 not necessary but used for stability of invdet()
// Face-centered natural coords (xpn,ypn,zpn) used for eltype==8 (per side)
// (xcens,ycens,zcens) = centroid of side
// (xcen,ycen,zcen) = centroid of element
// TBD: Limitation--> Code returns centroid of side for wedge/tetras (i.e. no search)
{long is=0,it=0,MNITER=2,trash[3];
 float DJD=0.f,HN[9],SN[MXNPELS],SG[NDF*MXNPELS],DJR[9],xtip1=0.f,ytip1=0.f,ztip1=0.f,xtip2=0.f,ytip2=0.f,ztip2=0.f,xtip3=0.f,ytip3=0.f,ztip3=0.f,
   eleng1=0.f,eleng2=0.f,eleng3=0.f,xpn=0.f,ypn=0.f,zpn=0.f,xnp=0.f,ynp=0.f,znp=0.f,radius=0.f,pnTOL=1.001f;
 if(eltype==8)
   {if(kside==0){
	   xtip1=0.5f*(c1[NDF*nop1[1]  ]+c1[NDF*nop1[5]  ]);ytip1=0.5f*(c1[NDF*nop1[1]+1]+c1[NDF*nop1[5]+1]);ztip1=0.5f*(c1[NDF*nop1[1]+2]+c1[NDF*nop1[5]+2]);
	   xtip2=0.5f*(0.25f*(c1[NDF*nop1[2]  ]+c1[NDF*nop1[3]  ]+c1[NDF*nop1[6]  ]+c1[NDF*nop1[7]  ])-xcens)+xcens;
	   ytip2=0.5f*(0.25f*(c1[NDF*nop1[2]+1]+c1[NDF*nop1[3]+1]+c1[NDF*nop1[6]+1]+c1[NDF*nop1[7]+1])-ycens)+ycens;
	   ztip2=0.5f*(0.25f*(c1[NDF*nop1[2]+2]+c1[NDF*nop1[3]+2]+c1[NDF*nop1[6]+2]+c1[NDF*nop1[7]+2])-zcens)+zcens;
	   xtip3=0.5f*(c1[NDF*nop1[4]  ]+c1[NDF*nop1[5]  ]);ytip3=0.5f*(c1[NDF*nop1[4]+1]+c1[NDF*nop1[5]+1]);ztip3=0.5f*(c1[NDF*nop1[4]+2]+c1[NDF*nop1[5]+2]);
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]=(xtip1-xcens)/eleng1;DJR[1]=(xtip2-xcens)/eleng2;DJR[2]=(xtip3-xcens)/eleng3;
	   DJR[3]=(ytip1-ycens)/eleng1;DJR[4]=(ytip2-ycens)/eleng2;DJR[5]=(ytip3-ycens)/eleng3;
	   DJR[6]=(ztip1-zcens)/eleng1;DJR[7]=(ztip2-zcens)/eleng2;DJR[8]=(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   ypn=0.f;if(fabs(DJR[5])>1.e-10f) *ZP=(ypn*eleng2-DJR[3]*(XP-xcens)-DJR[4]*(YP-ycens))/DJR[5] +zcens;else *ZP=zcens;
	   xpn=(DJR[0]*(XP-xcens)+DJR[1]*(YP-ycens)+DJR[2]*( *ZP-zcens))/eleng1;zpn=(DJR[6]*(XP-xcens)+DJR[7]*(YP-ycens)+DJR[8]*( *ZP-zcens))/eleng3;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn,ypn-1.f,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn,ypn-1.f,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[5])>1.e-10) *ZP= *ZP+(ypn-DJR[3]*(XP-xnp)-DJR[4]*(YP-ynp))/DJR[5] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[5])>1.e-10f) *ZP= *ZP+(ypn-DJR[3]*(XP-xnp)-DJR[4]*(YP-ynp))/DJR[5];
				xpn=xpn+DJR[0]*(XP-xnp)+DJR[1]*(YP-ynp)+DJR[2]*( *ZP-znp);zpn=zpn+DJR[6]*(XP-xnp)+DJR[7]*(YP-ynp)+DJR[8]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn;*RN2=ypn-1.f;*RN3=zpn;
				}
	else if(kside==1){
	   xtip1=0.5f*(0.25f*(c1[NDF*nop1[0]  ]+c1[NDF*nop1[3]  ]+c1[NDF*nop1[4]  ]+c1[NDF*nop1[7]  ])-xcens)+xcens;
	   ytip1=0.5f*(0.25f*(c1[NDF*nop1[0]+1]+c1[NDF*nop1[3]+1]+c1[NDF*nop1[4]+1]+c1[NDF*nop1[7]+1])-ycens)+ycens;
	   ztip1=0.5f*(0.25f*(c1[NDF*nop1[0]+2]+c1[NDF*nop1[3]+2]+c1[NDF*nop1[4]+2]+c1[NDF*nop1[7]+2])-zcens)+zcens;
	   xtip2=0.5f*(c1[NDF*nop1[2]  ]+c1[NDF*nop1[6]  ]);ytip2=0.5f*(c1[NDF*nop1[2]+1]+c1[NDF*nop1[6]+1]);ztip2=0.5f*(c1[NDF*nop1[2]+2]+c1[NDF*nop1[6]+2]);
	   xtip3=0.5f*(c1[NDF*nop1[5]  ]+c1[NDF*nop1[6]  ]);ytip3=0.5f*(c1[NDF*nop1[5]+1]+c1[NDF*nop1[6]+1]);ztip3=0.5f*(c1[NDF*nop1[5]+2]+c1[NDF*nop1[6]+2]);
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]= -(xtip1-xcens)/eleng1;DJR[1]=(xtip2-xcens)/eleng2;DJR[2]=(xtip3-xcens)/eleng3;
	   DJR[3]= -(ytip1-ycens)/eleng1;DJR[4]=(ytip2-ycens)/eleng2;DJR[5]=(ytip3-ycens)/eleng3;
	   DJR[6]= -(ztip1-zcens)/eleng1;DJR[7]=(ztip2-zcens)/eleng2;DJR[8]=(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   xpn=0.f;if(fabs(DJR[2])>1.e-10f)*ZP=(xpn*eleng1-DJR[0]*(XP-xcens)-DJR[1]*(YP-ycens))/DJR[2] +zcens;else *ZP=zcens;
	   ypn=(DJR[3]*(XP-xcens)+DJR[4]*(YP-ycens)+DJR[5]*( *ZP-zcens))/eleng2;zpn=(DJR[6]*(XP-xcens)+DJR[7]*(YP-ycens)+DJR[8]*( *ZP-zcens))/eleng3;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn+1.f,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn+1.f,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[2])>1.e-10)*ZP= *ZP+(xpn-DJR[0]*(XP-xnp)-DJR[1]*(YP-ynp))/DJR[2] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[2])>1.e-10f)*ZP= *ZP+(xpn-DJR[0]*(XP-xnp)-DJR[1]*(YP-ynp))/DJR[2];
				ypn=ypn+DJR[3]*(XP-xnp)+DJR[4]*(YP-ynp)+DJR[5]*( *ZP-znp);zpn=zpn+DJR[6]*(XP-xnp)+DJR[7]*(YP-ynp)+DJR[8]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn+1.f;*RN2=ypn;*RN3=zpn;
					 }
	else if(kside==2){
	   xtip1=0.5f*(c1[NDF*nop1[2]  ]+c1[NDF*nop1[6]  ]);ytip1=0.5f*(c1[NDF*nop1[2]+1]+c1[NDF*nop1[6]+1]);ztip1=0.5f*(c1[NDF*nop1[2]+2]+c1[NDF*nop1[6]+2]);
	   xtip2=0.5f*(0.25f*(c1[NDF*nop1[0]  ]+c1[NDF*nop1[1]  ]+c1[NDF*nop1[4]  ]+c1[NDF*nop1[5]  ])-xcens)+xcens;
	   ytip2=0.5f*(0.25f*(c1[NDF*nop1[0]+1]+c1[NDF*nop1[1]+1]+c1[NDF*nop1[4]+1]+c1[NDF*nop1[5]+1])-ycens)+ycens;
	   ztip2=0.5f*(0.25f*(c1[NDF*nop1[0]+2]+c1[NDF*nop1[1]+2]+c1[NDF*nop1[4]+2]+c1[NDF*nop1[5]+2])-zcens)+zcens;
	   xtip3=0.5f*(c1[NDF*nop1[6]  ]+c1[NDF*nop1[7]  ]);ytip3=0.5f*(c1[NDF*nop1[6]+1]+c1[NDF*nop1[7]+1]);ztip3=0.5f*(c1[NDF*nop1[6]+2]+c1[NDF*nop1[7]+2]);
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]=(xtip1-xcens)/eleng1;DJR[1]= -(xtip2-xcens)/eleng2;DJR[2]=(xtip3-xcens)/eleng3;
	   DJR[3]=(ytip1-ycens)/eleng1;DJR[4]= -(ytip2-ycens)/eleng2;DJR[5]=(ytip3-ycens)/eleng3;
	   DJR[6]=(ztip1-zcens)/eleng1;DJR[7]= -(ztip2-zcens)/eleng2;DJR[8]=(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   ypn=0.f;if(fabs(DJR[5])>1.e-10f) *ZP=(ypn*eleng2-DJR[3]*(XP-xcens)-DJR[4]*(YP-ycens))/DJR[5] +zcens;else *ZP=zcens;
	   xpn=(DJR[0]*(XP-xcens)+DJR[1]*(YP-ycens)+DJR[2]*( *ZP-zcens))/eleng1;zpn=(DJR[6]*(XP-xcens)+DJR[7]*(YP-ycens)+DJR[8]*( *ZP-zcens))/eleng3;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn,ypn+1.f,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn,ypn+1.f,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[5])>1.e-10) *ZP= *ZP+(ypn-DJR[3]*(XP-xnp)-DJR[4]*(YP-ynp))/DJR[5] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[5])>1.e-10f) *ZP= *ZP+(ypn-DJR[3]*(XP-xnp)-DJR[4]*(YP-ynp))/DJR[5];
				xpn=xpn+DJR[0]*(XP-xnp)+DJR[1]*(YP-ynp)+DJR[2]*( *ZP-znp);zpn=zpn+DJR[6]*(XP-xnp)+DJR[7]*(YP-ynp)+DJR[8]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn;*RN2=ypn+1.f;*RN3=zpn;
				}
	else if(kside==3){
	   xtip1=0.5f*(0.25f*(c1[NDF*nop1[1]  ]+c1[NDF*nop1[2]  ]+c1[NDF*nop1[5]  ]+c1[NDF*nop1[6]  ])-xcens)+xcens;
	   ytip1=0.5f*(0.25f*(c1[NDF*nop1[1]+1]+c1[NDF*nop1[2]+1]+c1[NDF*nop1[5]+1]+c1[NDF*nop1[6]+1])-ycens)+ycens;
	   ztip1=0.5f*(0.25f*(c1[NDF*nop1[1]+2]+c1[NDF*nop1[2]+2]+c1[NDF*nop1[5]+2]+c1[NDF*nop1[6]+2])-zcens)+zcens;
	   xtip2=0.5f*(c1[NDF*nop1[3]  ]+c1[NDF*nop1[7]  ]);ytip2=0.5f*(c1[NDF*nop1[3]+1]+c1[NDF*nop1[7]+1]);ztip2=0.5f*(c1[NDF*nop1[3]+2]+c1[NDF*nop1[7]+2]);
	   xtip3=0.5f*(c1[NDF*nop1[4]  ]+c1[NDF*nop1[7]  ]);ytip3=0.5f*(c1[NDF*nop1[4]+1]+c1[NDF*nop1[7]+1]);ztip3=0.5f*(c1[NDF*nop1[4]+2]+c1[NDF*nop1[7]+2]);
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]=(xtip1-xcens)/eleng1;DJR[1]=(xtip2-xcens)/eleng2;DJR[2]=(xtip3-xcens)/eleng3;
	   DJR[3]=(ytip1-ycens)/eleng1;DJR[4]=(ytip2-ycens)/eleng2;DJR[5]=(ytip3-ycens)/eleng3;
	   DJR[6]=(ztip1-zcens)/eleng1;DJR[7]=(ztip2-zcens)/eleng2;DJR[8]=(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   xpn=0.f;if(fabs(DJR[2])>1.e-10f)*ZP=(xpn*eleng1-DJR[0]*(XP-xcens)-DJR[1]*(YP-ycens))/DJR[2] +zcens;else *ZP=zcens;
	   ypn=(DJR[3]*(XP-xcens)+DJR[4]*(YP-ycens)+DJR[5]*( *ZP-zcens))/eleng2;zpn=(DJR[6]*(XP-xcens)+DJR[7]*(YP-ycens)+DJR[8]*( *ZP-zcens))/eleng3;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn-1.f,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn-1.f,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[2])>1.e-10)*ZP= *ZP+(xpn-DJR[0]*(XP-xnp)-DJR[1]*(YP-ynp))/DJR[2] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[2])>1.e-10f)*ZP= *ZP+(xpn-DJR[0]*(XP-xnp)-DJR[1]*(YP-ynp))/DJR[2];
				ypn=ypn+DJR[3]*(XP-xnp)+DJR[4]*(YP-ynp)+DJR[5]*( *ZP-znp);zpn=zpn+DJR[6]*(XP-xnp)+DJR[7]*(YP-ynp)+DJR[8]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn-1.f;*RN2=ypn;*RN3=zpn;
					 }
	else if(kside==4){
	   xtip1=0.5f*(c1[NDF*nop1[1]  ]+c1[NDF*nop1[2]  ]);ytip1=0.5f*(c1[NDF*nop1[1]+1]+c1[NDF*nop1[2]+1]);ztip1=0.5f*(c1[NDF*nop1[1]+2]+c1[NDF*nop1[2]+2]);
	   xtip2=0.5f*(c1[NDF*nop1[2]  ]+c1[NDF*nop1[3]  ]);ytip2=0.5f*(c1[NDF*nop1[2]+1]+c1[NDF*nop1[3]+1]);ztip2=0.5f*(c1[NDF*nop1[2]+2]+c1[NDF*nop1[3]+2]);
	   xtip3=0.5f*(0.25f*(c1[NDF*nop1[4]  ]+c1[NDF*nop1[5]  ]+c1[NDF*nop1[6]  ]+c1[NDF*nop1[7]  ])-xcens)+xcens;
	   ytip3=0.5f*(0.25f*(c1[NDF*nop1[4]+1]+c1[NDF*nop1[5]+1]+c1[NDF*nop1[6]+1]+c1[NDF*nop1[7]+1])-ycens)+ycens;
	   ztip3=0.5f*(0.25f*(c1[NDF*nop1[4]+2]+c1[NDF*nop1[5]+2]+c1[NDF*nop1[6]+2]+c1[NDF*nop1[7]+2])-zcens)+zcens;
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]=(xtip1-xcens)/eleng1;DJR[1]=(xtip2-xcens)/eleng2;DJR[2]=(xtip3-xcens)/eleng3;
	   DJR[3]=(ytip1-ycens)/eleng1;DJR[4]=(ytip2-ycens)/eleng2;DJR[5]=(ytip3-ycens)/eleng3;
	   DJR[6]=(ztip1-zcens)/eleng1;DJR[7]=(ztip2-zcens)/eleng2;DJR[8]=(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   zpn=0.f;if(fabs(DJR[8])>1.e-10f)*ZP=(zpn*eleng3-DJR[6]*(XP-xcens)-DJR[7]*(YP-ycens))/DJR[8] +zcens;else *ZP=zcens;
	   xpn=(DJR[0]*(XP-xcens)+DJR[1]*(YP-ycens)+DJR[2]*( *ZP-zcens))/eleng1;ypn=(DJR[3]*(XP-xcens)+DJR[4]*(YP-ycens)+DJR[5]*( *ZP-zcens))/eleng2;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn,ypn,zpn-1.f,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn,ypn,zpn-1.f,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[8])>1.e-10)*ZP= *ZP+(zpn-DJR[6]*(XP-xnp)-DJR[7]*(YP-ynp))/DJR[8] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[8])>1.e-10f)*ZP= *ZP+(zpn-DJR[6]*(XP-xnp)-DJR[7]*(YP-ynp))/DJR[8];
				xpn=xpn+DJR[0]*(XP-xnp)+DJR[1]*(YP-ynp)+DJR[2]*( *ZP-znp);ypn=ypn+DJR[3]*(XP-xnp)+DJR[4]*(YP-ynp)+DJR[5]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn;*RN2=ypn;*RN3=zpn-1.f;
					 }
	else {
	   xtip1=0.5f*(c1[NDF*nop1[5]  ]+c1[NDF*nop1[6]  ]);ytip1=0.5f*(c1[NDF*nop1[5]+1]+c1[NDF*nop1[6]+1]);ztip1=0.5f*(c1[NDF*nop1[5]+2]+c1[NDF*nop1[6]+2]);
	   xtip2=0.5f*(c1[NDF*nop1[6]  ]+c1[NDF*nop1[7]  ]);ytip2=0.5f*(c1[NDF*nop1[6]+1]+c1[NDF*nop1[7]+1]);ztip2=0.5f*(c1[NDF*nop1[6]+2]+c1[NDF*nop1[7]+2]);
	   xtip3=0.5f*(0.25f*(c1[NDF*nop1[0]  ]+c1[NDF*nop1[1]  ]+c1[NDF*nop1[2]  ]+c1[NDF*nop1[3]  ])-xcens)+xcens;
	   ytip3=0.5f*(0.25f*(c1[NDF*nop1[0]+1]+c1[NDF*nop1[1]+1]+c1[NDF*nop1[2]+1]+c1[NDF*nop1[3]+1])-ycens)+ycens;
	   ztip3=0.5f*(0.25f*(c1[NDF*nop1[0]+2]+c1[NDF*nop1[1]+2]+c1[NDF*nop1[2]+2]+c1[NDF*nop1[3]+2])-zcens)+zcens;
	   eleng1=sqrt((xtip1-xcens)*(xtip1-xcens)+(ytip1-ycens)*(ytip1-ycens)+(ztip1-zcens)*(ztip1-zcens));
	   eleng2=sqrt((xtip2-xcens)*(xtip2-xcens)+(ytip2-ycens)*(ytip2-ycens)+(ztip2-zcens)*(ztip2-zcens));
	   eleng3=sqrt((xtip3-xcens)*(xtip3-xcens)+(ytip3-ycens)*(ytip3-ycens)+(ztip3-zcens)*(ztip3-zcens));
	   DJR[0]=(xtip1-xcens)/eleng1;DJR[1]=(xtip2-xcens)/eleng2;DJR[2]= -(xtip3-xcens)/eleng3;
	   DJR[3]=(ytip1-ycens)/eleng1;DJR[4]=(ytip2-ycens)/eleng2;DJR[5]= -(ytip3-ycens)/eleng3;
	   DJR[6]=(ztip1-zcens)/eleng1;DJR[7]=(ztip2-zcens)/eleng2;DJR[8]= -(ztip3-zcens)/eleng3;invdet(3,trash,DJR);
	   zpn=0.f;if(fabs(DJR[8])>1.e-10f) *ZP=(zpn*eleng3-DJR[6]*(XP-xcens)-DJR[7]*(YP-ycens))/DJR[8] +zcens;else *ZP=zcens;
	   xpn=(DJR[0]*(XP-xcens)+DJR[1]*(YP-ycens)+DJR[2]*( *ZP-zcens))/eleng1;ypn=(DJR[3]*(XP-xcens)+DJR[4]*(YP-ycens)+DJR[5]*( *ZP-zcens))/eleng2;
	   for(it=0;it<MNITER;it++)
		 {STFISO8(1,xpn,ypn,zpn+1.f,&DJD,HN,SN,SG,DJR,nop1,c1);
		  xnp=ynp=znp=0.f;for(is=0;is<node;is++){xnp=xnp+SN[is]*c1[NDF*nop1[is]  ];ynp=ynp+SN[is]*c1[NDF*nop1[is]+1];znp=znp+SN[is]*c1[NDF*nop1[is]+2];}
		  radius=sqrt((XP-xnp)*(XP-xnp)+(YP-ynp)*(YP-ynp)+( *ZP-znp)*( *ZP-znp));
		  if(radius<TOL){if(fabs(xpn)<=pnTOL && fabs(ypn)<=pnTOL && fabs(zpn)<=pnTOL)break;}
		  else {STFISO8(2,xpn,ypn,zpn+1.f,&DJD,HN,SN,SG,DJR,nop1,c1);invdet(3,trash,DJR);
//                if(fabs(DJR[8])>1.e-10)*ZP= *ZP+(zpn-DJR[6]*(XP-xnp)-DJR[7]*(YP-ynp))/DJR[8] +znp;else *ZP= *ZP+znp;
				if(fabs(DJR[8])>1.e-10f)*ZP= *ZP+(zpn-DJR[6]*(XP-xnp)-DJR[7]*(YP-ynp))/DJR[8];
				xpn=xpn+DJR[0]*(XP-xnp)+DJR[1]*(YP-ynp)+DJR[2]*( *ZP-znp);ypn=ypn+DJR[3]*(XP-xnp)+DJR[4]*(YP-ynp)+DJR[5]*( *ZP-znp);
//                if(it==MNITER-1)honk<<ie<<" homsub: No elem-search convergence "<<XP<<" "<<YP<<" "<<ZP<<"\n";
			   }
		 }
	   *RN1=xpn;*RN2=ypn;*RN3=zpn+1.f;
		 }
   }
 else if(eltype==7)
   {
	if(kside==0){xpn=ypn=0.5f;zpn=0.f;}else if(kside==1){xpn=0.f;ypn=0.5f;zpn=0.f;}else if(kside==2){xpn=0.5f;ypn=0.f;zpn=0.f;}else if(kside==3){xpn=ypn=1.f/3.f;zpn= -1.f;}else {xpn=ypn=1.f/3.f;zpn=1.f;}
	STFISO6(1,xpn,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1); *ZP=0.f;for(is=0;is<node;is++) *ZP= *ZP+SN[is]*c1[NDF*nop1[is]+2];
	*RN1= xpn;*RN2= ypn;*RN3= zpn;
   }
 else if(eltype==5)
   {
	if(kside==0){xpn=ypn=zpn=1.f/3.f;}else if(kside==1){xpn=0.f;ypn=zpn=1.f/3.f;}else if(kside==2){ypn=0.f;xpn=zpn=1.f/3.f;}else {xpn=ypn=1.f/3.f;zpn=0.f;}
	STFISO4(1,xpn,ypn,zpn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1); *ZP=0.f;for(is=0;is<node;is++) *ZP= *ZP+SN[is]*c1[NDF*nop1[is]+2];
	*RN1= xpn;*RN2= ypn;*RN3= zpn;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"homsub: unsupported eltype",L"Terminate",MB_OK);exit(0);}
/////////////////////////////////
}
//---------------------------------------------------------------------------
void TForm1::invdet(long n,long j[],float c[])
// Storage cij   c11,c12,c13... rowwise;    j[n], c[n*n] ----------> NOTE: c[] IS DESTROYED...
{long l=0,k=0,m=0;float cc=0.f,s=0.f,TOL=0.0001f;for(l=0;l<n;l++)j[l]=l;
 for(l=0;l<n;l++){cc=0.f;m=l;for(k=l;k<n;k++){if(fabs(cc)-fabs(c[n*l+k])<0.f){m=k;cc=c[n*l+k];}}
				  if(l!=m){k=j[m];j[m]=j[l];j[l]=k;for(k=0;k<n;k++){s=c[n*k+l];c[n*k+l]=c[n*k+m];c[n*k+m]=s;}}
				  c[n*l+l]=1.f;
if(fabs(cc)<1.e-15f){honk<<cc<<"  Noninvertible invdet: CC\n";exit(0);}
				  for(m=0;m<n;m++)c[n*l+m]=c[n*l+m]/cc;
				  for(m=0;m<n;m++){if(l!=m){cc=c[n*m+l];if(fabs(cc)>TOL){c[n*m+l]=0.f;for(k=0;k<n;k++)c[n*m+k]=c[n*m+k]-cc*c[n*l+k];}}}
				 }
 for(l=0;l<n;l++){if(j[l]!=l){for(m=l+1;m<n;m++){if(j[m]==l || m==n-1)break;}j[m]=j[l];for(k=0;k<n;k++){cc=c[n*l+k];c[n*l+k]=c[n*m+k];c[n*m+k]=cc;}j[l]=l;}}
}
//---------------------------------------------------------------------------
void TForm1::FDrestore()
// Global everything (arrays NOT passed in argument list)
{//int *edgFace1=NULL;long *arbFace1=NULL,*rbTem1=NULL;
 long i=0,in=0,dummy=0,ntranche=8;
 base.GIDcol=2; //Bugfix EFP 3/21/2012 Always prioritize WP when RESTOREing
	FDbase_indat(1,shapecombo,iplotflag,nColRes);
	FDrot_status(indat.npoin,indat.c1,rot_op);
////////////////////
//for(i=0;i<indat.npoin;i++)honk<<(i+1)<<" "<<base.c1[NDF*i+0]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]
//									<<" bCiC "<<indat.c1[NDF*i+0]<<" "<<indat.c1[NDF*i+1]<<" "<<indat.c1[NDF*i+2]<<"\n";
////////////////////

//for(in=0;in<base.npoin;in++)honk<<(in+1)<<" indatCCC "<<indat.c1[NDF*in+0]<<" "<<indat.c1[NDF*in+1]<<" "<<indat.c1[NDF*in+2]<<"\n";
// for(in=0;in<indat.nelt;in++)indat.arrELSET[in]=1;

// FDcomp_nGID(base.nelt,&nGID,arGID);  //BUT THIS USES indat.arrELSET internally....
honk<<iplotType<<" "<<iPaintyesno<<" "<<iplotflag<<" "<<shapecombo<<"\n";
//	FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);iPaintyesno=1;
  if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
  else
							   {if(iPaintyesno-10*(iPaintyesno/10)){
//honk<<iplotType<<" hereeeeee "<<iPaintyesno<<" "<<iplotflag<<" "<<shapecombo<<"\n";
																	if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
																	else FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);

																	iPaintyesno=10+1;
																   }
								else {
									  if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
									  else {
 FDcomp_nGID(base.nelt,&nGID,arGID);  //BUT THIS USES indat.arrELSET internally....
											FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);
										   }
									  iPaintyesno=10;
									 }
							   }
	  arbFacet=new long[6*base.nelt];rbTemp=new long[6*base.nelt];
//// base.GIDcol=2; // Always prioritize WP when RESTOREing
// FDcomp_nGID(base.nelt,&nGID,arGID);  //BUT THIS USES indat.arrELSET internally....
 nFacets=FDcullfacetc(indat.nelt,indat.nop1,indat.matno,indat.c1,arGID,arbFacet,rbTemp);
 FDsort_low1(indat.nop1,indat.c1,nFacets,arbFacet,rbTemp,ntranche);
// if(!ShowFEMesh->Checked)FDculledge(indat.nop1,indat.matno,indat.bc1,nFacets,arbFacet,edgFacet);
 iplotflag=1;iCullyesno=1;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 axis_plot_init();
 axis_plotXYZ(ClientWidth,ClientHeight);
iCircleplot=1;Invalidate();
}
//---------------------------------------------------------------------------
void TForm1::FDrot_status(long indat_npoin,float indat_c1[],float rot_op[][3])
// Global NDF
{long i=0;float sumx=0.f,sumy=0.f,sumz=0.f,cenx=0.f,ceny=0.f,cenz=0.f,xmx= -1.E20f,xmn=1.E20f,ymx= -1.E20f,ymn=1.E20f,zmx= -1.E20f,zmn=1.E20f,r11=0.f,r12=0.f,r13=0.f,r21=0.f,r22=0.f,r23=0.f,r31=0.f,r32=0.f,r33=0.f;
 for(i=0;i<indat_npoin;i++)
   {if(xmx<indat_c1[NDF*i  ])xmx=indat_c1[NDF*i  ];if(xmn>indat_c1[NDF*i  ])xmn=indat_c1[NDF*i  ];
	if(ymx<indat_c1[NDF*i+1])ymx=indat_c1[NDF*i+1];if(ymn>indat_c1[NDF*i+1])ymn=indat_c1[NDF*i+1];
	if(zmx<indat_c1[NDF*i+2])zmx=indat_c1[NDF*i+2];if(zmn>indat_c1[NDF*i+2])zmn=indat_c1[NDF*i+2];
	}
 cenx=(xmx-xmn)/2.f;ceny=(ymx-ymn)/2.f;cenz=(zmx-zmn)/2.f;
 r11=rot_op[0][0];r21=rot_op[1][0];r31=rot_op[2][0];r12=rot_op[0][1];r22=rot_op[1][1];r32=rot_op[2][1];r13=rot_op[0][2];r23=rot_op[1][2];r33=rot_op[2][2];
 for(i=0;i<indat_npoin;i++)
   {sumx=cenx+r11*(indat_c1[NDF*i]-cenx)+r12*(indat_c1[NDF*i+1]-ceny)+r13*(indat_c1[NDF*i+2]-cenz);
	sumy=ceny+r21*(indat_c1[NDF*i]-cenx)+r22*(indat_c1[NDF*i+1]-ceny)+r23*(indat_c1[NDF*i+2]-cenz);
	sumz=cenz+r31*(indat_c1[NDF*i]-cenx)+r32*(indat_c1[NDF*i+1]-ceny)+r33*(indat_c1[NDF*i+2]-cenz);
	indat_c1[NDF*i]=sumx;indat_c1[NDF*i+1]=sumy;indat_c1[NDF*i+2]=sumz;
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveAs2Execute(TObject *Sender)
{if(base.nop1)
//VFT_SaveAs(); //Note that this does NOT use gWsiAlias   EFP 1/07/2011
			   VFT_SaveAs1(1); //User-input filename *.VFTr  EFP 11/23/2012
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
////void __fastcall TForm1::SaveAsExecute(TObject *Sender)
//void TForm1::VFT_SaveAs(char gVFTnameStem[])
void TForm1::VFT_SaveAs1(int ksw)
//Convention: isw=0 default name Model.VFTr; isw= other user chosen filename  EFP 11/23/2012
// *.VFTr is NOT COMMA-DELIMITED currently
// TBD: Currently using & writing highest node number but only writing active nodes. Switch to memory-saving "true node number" scheme
//      This is a cat's ass scheme. Switch to pre-read/reserve memory/read again
{long i=0,j=0,jsw=0,control[10],eltype=0,bscode=0,node=0,ieGID=0,t7=10000000,t5=100000,t3=1000,accum=0;
 char *fnNeed=NULL;
 if(base.nop1)
//   {if(ksw)
   {

//	if(ksw==1)
//	  {SaveDialog1->Filter= "VFTr format (*.VFTr,*.vftr)|*.VFTr;*.vftr";
//	   if(SaveDialog1->Execute())
//		 {jsw=1;fnNeed=new char[strlen(SaveDialog1->FileName.t_str())+1+5];
//		  StringCchCopy(fnNeed,strlen(SaveDialog1->FileName.t_str())+1+5,SaveDialog1->FileName.t_str());
//		  if(ExtractFileExt(SaveDialog1->FileName)==L"")StringCchCat(fnNeed,strlen(SaveDialog1->FileName.t_str())+1+5,".VFTr");
//		 }
//	   else {jsw=0;extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open SaveAs dialog",L"Failure",MB_OK);
//			}
//	  }
//	else if(ksw==2){jsw=1;
//					fnNeed=new char[11];fnNeed[0]='M';fnNeed[1]='o';fnNeed[2]='d';fnNeed[3]='e';fnNeed[4]='l';fnNeed[5]='.';fnNeed[6]='V';fnNeed[7]='F';fnNeed[8]='T';fnNeed[9]='a';fnNeed[10]='\0';
//				   }
//	else {
		  jsw=1;
		  fnNeed=new char[11];fnNeed[0]='M';fnNeed[1]='o';fnNeed[2]='d';fnNeed[3]='e';fnNeed[4]='l';fnNeed[5]='.';fnNeed[6]='V';fnNeed[7]='F';fnNeed[8]='T';fnNeed[9]='r';fnNeed[10]='\0';
//		 }


	if(jsw)
	  {
//	   StringCchCopy(VFTr_name,strlen(fnNeed),fnNeed);
	   ofstream ntape(fnNeed);delete [] fnNeed;
//honk<<VFTr_name<<" VFTr_name VFTSAVEASsolid\n";
//honk<<fnNeed<<" VFTr_name VFTSAVEASsolid\n";
	   if(ntape){ntape.setf(ios::scientific);


//aaaaaaaaaaaaaaaaaaaaa
if(ksw!=2){
//bbbbbbbbbbbbbbbbbbbbb
				 for(i=0;i<10;i++)control[i]=0;
//honk<<base.npoin<<" enter VFTsaveAs "<<base.nelt<<"\n";
////				 control[0]=1; //Flag for old *.VFT
//				 control[0]=2; //Flag for VFT version3.x
				 control[0]=3; //Flag for VFT version3.x for 2013, with wp.lstart==weld length
				 control[1]=base.npoin; //#nodes (Currently, highest node-NOT #active nodes)
				 control[2]=base.nelt; //#elem   (Currently, #active elements, not highest element)
//				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
				 ntape<<control[0]<<" "<<control[1]<<" "<<control[2]<<" "<<control[3]<<" "<<control[4]<<" "<<
						control[5]<<" "<<control[6]<<" "<<control[7]<<" "<<control[8]<<" "<<control[9]<<"\n";
//				 for(i=0;i<base.npoin;i++)ntape<<i+1<<" "<<base.c1[NDF*i  ]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n"; //EFP 11/24/2010
//				 for(i=0;i<base.npoin;i++)if(base.node_map[i]>0)ntape<<i+1<<" "<<base.c1[NDF*i  ]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n"; //EFP 11/24/2010
				 for(i=0;i<base.npoin;i++)ntape<<(base.node_map[i]+1)<<" "<<base.c1[NDF*i  ]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n"; //EFP 11/24/2010
//				 for(i=0;i<base.npoin;i++){if(base.node_map[i]>0)ntape<<i+1<<" "<<base.c1[NDF*i  ]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n";
//										   else ntape<<0<<" "<<base.c1[NDF*i  ]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n";
//										  }
//// Policy: Write out 8 "corners" regardless of element type... Zero for nonexistent corners
//////				 for(i=0;i<base.nelt;i++)ntape<<i+1<<" "<<base.nop1[MXNPEL*i  ]+1<<" "<<base.nop1[MXNPEL*i+1]+1<<" "<<base.nop1[MXNPEL*i+2]+1<<" "<<base.nop1[MXNPEL*i+3]+1<<" "<<
//////														  base.nop1[MXNPEL*i+4]+1<<" "<<base.nop1[MXNPEL*i+5]+1<<" "<<base.nop1[MXNPEL*i+6]+1<<" "<<base.nop1[MXNPEL*i+7]+1<<"\n";
////				 for(i=0;i<base.nelt;i++)ntape<<base.el_map[i]+1<<" "<<base.nop1[MXNPEL*i  ]+1<<" "<<base.nop1[MXNPEL*i+1]+1<<" "<<base.nop1[MXNPEL*i+2]+1<<" "<<base.nop1[MXNPEL*i+3]+1<<" "<<
////																	   base.nop1[MXNPEL*i+4]+1<<" "<<base.nop1[MXNPEL*i+5]+1<<" "<<base.nop1[MXNPEL*i+6]+1<<" "<<base.nop1[MXNPEL*i+7]+1<<"\n";
//				 for(i=0;i<base.nelt;i++)ntape<<base.el_map[i]+1<<" "<<base.node_map[base.nop1[MXNPEL*i  ]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+1]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+2]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+3]]+1<<" "<<
//																	   base.node_map[base.nop1[MXNPEL*i+4]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+5]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+6]]+1<<" "<<base.node_map[base.nop1[MXNPEL*i+7]]+1<<"\n";
				 for(i=0;i<base.nelt;i++){eltype=base.matno[i]/t7;bscode=(base.matno[i]-eltype*t7)/t5;node=(base.matno[i]-eltype*t7-bscode*t5)/t3;
										  if(eltype==8){if(node==8) //EFP 6/12/2014
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<"\n";
														else
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+10]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+11]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+12]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+13]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+14]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+15]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+16]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+17]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+18]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+19]]+1)<<"\n";
													   }
										  else if(eltype==7){if(node==6)
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<"\n";
															 else
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+10]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+11]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+12]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+13]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+14]]+1)<<"\n";
															}
										  else if(eltype==5){if(node==4)
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<"\n";
															 else
ntape<<(base.el_map[i]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<" "<<
								(base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<" "<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<"\n";
															}
										  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"VFT_SaveAs() unsupported element type",L"Terminate",MB_OK);exit(0);}
										 }
//honk<<wp.nWeldGroup<<" SaveAs wp.nWeldGroup\n";


//ntape<<base.nelt<<"\n";
//if(1==1)exit(0);
accum=0;ntape<<accum<<"\n"; //Just in case  EFP 4/03/2011
accum=0;ntape<<accum<<"\n";// Omit JavaVFT group[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsall[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupA[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsallname[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsallindex[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groups[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupscount[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupname[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsname[]
//wwwwwwwwwwwwwww
//wwwwwwwwwwwwwww
//wwwwwwwwwwwwwww
//wp.nWeldGroup=0;
//extern PACKAGE void __fastcall Beep(void);
//Application->MessageBox(L"wp.nWeldGroup set to 0",L"Frantic note",MB_OK);
//wwwwwwwwwwwwwww
//wwwwwwwwwwwwwww
//wwwwwwwwwwwwwww
				 ntape<<wp.nWeldGroup<<"\n";
//if(1==1)exit(0);

				 if(wp.nWeldGroup){for(j=0;j<wp.nWeldGroup;j++)
								   {
/////////////  Include just in case  EFP 4/03/2011
//////////////
//////////////
accum=0;ntape<<accum<<"\n"; //Just in case  EFP 4/03/2011
accum=0;ntape<<accum<<"\n";// Omit JavaVFT group[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsall[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupA[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsallname[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupsallindex[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groups[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupscount[]
accum=0;ntape<<accum<<"\n";// Omit JavaVFT groupname[]
//////////////
/////////////
////////////
									accum=0;
//									for(i=0;i<base.nelt;i++){
//eltype=base.matno[i]/t7;bscode=(base.matno[i]-eltype*t7)/t5;node=(base.matno[i]-eltype*t7-bscode*t5)/t3;ieGID=base.matno[i]-eltype*t7-bscode*t5-node*t3;
//															 if(ieGID==j+1)accum++;
//															}
									for(i=0;i<base.nelt;i++){
////eltype=base.orig_matno[i]/t7;bscode=(base.orig_matno[i]-eltype*t7)/t5;node=(base.orig_matno[i]-eltype*t7-bscode*t5)/t3;ieGID=base.orig_matno[i]-eltype*t7-bscode*t5-node*t3;

////	 if(base.trackELSET[i+1]-base.trackELSET[i]==1)ieGID=0;
////	 else ieGID=base.arrELSET[ base.trackELSET[i]+1 ];
//////////////// EFP 2/20/2012
//						if(base.trackELSET[i+1]-base.trackELSET[i]==1)ieGID=0;
//						else if(base.arrELSET[ base.trackELSET[i+1]-1 ]<0)ieGID=base.arrELSET[ base.trackELSET[i+1]-2 ];
//						else {if(base.GIDcol==1)ieGID=base.arrELSET[ base.trackELSET[i]+base.GIDcol ]; //Prioritize WG
//							  else ieGID=base.arrELSET[ base.trackELSET[i+1]-1 ];                      //Prioritize WP
//							 }
ieGID=base.arrELSET[i];
////////////// but this does not use indat.xxx ?

															 if(ieGID==j+1)accum++;
															}
									ntape<<accum<<"\n";
////////									ntape<<"WeldGroup "<<j+1<<"\n";
//////									ntape<<base.groupsname[j].c_str()<<"\n";// EFP 3/15/2011
////									ntape<<base.groupsname[j].t_str()<<"\n";// EFP 3/15/2011 then 3/27/2011
//									ntape<<base.groupsname[j].w_str()<<"\n";// EFP 3/15/2011 then 3/27/2011
//xxxxxxxxxx
 int bufferSize1=WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,NULL,0,NULL,NULL);
 char* m1=new char[bufferSize1];WideCharToMultiByte(CP_UTF8,0,base.groupsname[j].w_str(), -1,m1,bufferSize1,NULL,NULL);
 ntape<<m1<<"\n";// EFP 12/10/2014
 delete[] m1;
//xxxxxxxxxx
									for(i=0;i<base.nelt;i++){
////eltype=base.orig_matno[i]/t7;bscode=(base.orig_matno[i]-eltype*t7)/t5;node=(base.orig_matno[i]-eltype*t7-bscode*t5)/t3;ieGID=base.orig_matno[i]-eltype*t7-bscode*t5-node*t3;

////	 if(base.trackELSET[i+1]-base.trackELSET[i]==1)ieGID=0;
////	 else ieGID=base.arrELSET[ base.trackELSET[i]+1 ];
//////////////// EFP 2/20/2012
//						if(base.trackELSET[i+1]-base.trackELSET[i]==1)ieGID=0;
//						else if(base.arrELSET[ base.trackELSET[i+1]-1 ]<0)ieGID=base.arrELSET[ base.trackELSET[i+1]-2 ];
//						else {if(base.GIDcol==1)ieGID=base.arrELSET[ base.trackELSET[i]+base.GIDcol ]; //Prioritize WG
//							  else ieGID=base.arrELSET[ base.trackELSET[i+1]-1 ];                      //Prioritize WP
//							 }
ieGID=base.arrELSET[i];
////////////// but this does not use indat.xxx ?

															 if(ieGID==j+1)ntape<<(i+1)<<"\n";
															}
								   }
								  }

////				 for(i=0;i<base.npoin;i++)ntape<<i+1<<" "<<i+1<<"\n"; // Nodal hashtable
//				 for(i=0;i<base.npoin;i++)ntape<<i+1<<" "<<base.node_map[i]+1<<"\n"; // Nodal hashtable
//				 for(i=0;i<base.nelt;i++)ntape<<i+1<<" "<<base.el_map[i]+1<<"\n"; // Element hashtable
//


//aaaaaaaaaaaaaaaaaaaaa
		  }
//bbbbbbbbbbbbbbbbbbbbb
//honk<<wp.nWeldPass<<" pear0\n";
				 ntape<<wp.nWeldPass<<"\n";
//if(1==1)exit(0);
				 if(wp.nWeldPass){for(i=0;i<wp.nWeldPass;i++){
//ntape<<wp.name[i].w_str()<<"\n";
//xxxxxxxxxx
 int bufferSize2=WideCharToMultiByte(CP_UTF8,0,wp.name[i].w_str(), -1,NULL,0,NULL,NULL);
 char* m2=new char[bufferSize2];WideCharToMultiByte(CP_UTF8,0,wp.name[i].w_str(), -1,m2,bufferSize2,NULL,NULL);
 ntape<<m2<<"\n";// EFP 12/10/2014
 delete[] m2;
//xxxxxxxxxx
//ntape<<wp.matName[i].w_str()<<"\n";
//xxxxxxxxxx
 int bufferSize3=WideCharToMultiByte(CP_UTF8,0,wp.matName[i].w_str(), -1,NULL,0,NULL,NULL);
 char* m3=new char[bufferSize3];WideCharToMultiByte(CP_UTF8,0,wp.matName[i].w_str(), -1,m3,bufferSize3,NULL,NULL);
 ntape<<m3<<"\n";// EFP 12/10/2014
 delete[] m3;
//xxxxxxxxxx
		ntape<<wp.seqNum[i]<<"\n";
		ntape<<wp.boolFlags[i]<<"\n";
		ntape<<wp.type[i]<<"\n";
		ntape<<wp.shape[i]<<"\n";
		ntape<<wp.stepInterval[i]<<"\n";
		ntape<<wp.hp[i]<<"\n";
		ntape<<wp.nsegs[i]<<"\n";
		ntape<<wp.iselect[i]<<"\n"; //Remember wp.eles[]=10*ie+iside in the following (ditto wp.sttEles[])
//		for(j=0;j<wp.memWGa;j++)ntape<<wp.eles[wp.memWGa*i+j]<<"\n";  // Elements in this weld pass
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.eles[wp.memWGa*i+j]> -1)accum++;  // Elements in this weld pass
										 else break;
										}
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.eles[wp.memWGa*i+j]<<"\n";  // Elements in this weld pass
////		wp.sttEles=new long[wp.nWeldPass+1*bnelt*4];  // No need for *4 ????
//		for(j=0;j<wp.memWGa;j++)ntape<<wp.sttEles[wp.memWGa*i+j]<<"\n";  // No need for *4 ????
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.sttEles[wp.memWGa*i+j]> -1)accum++;
										 else break;
										}
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.sttEles[wp.memWGa*i+j]<<"\n";  // No need for *4 ????
//		wp.stpEle=new long[wp.nWeldPass+1*bnelt];  // No need for *bnelt
		ntape<<wp.stpEle[i]<<"\n";  // No need for *bnelt
//		for(j=0;j<wp.memWGa;j++)ntape<<wp.firstEle[wp.memWGa*i+j]<<"\n";
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.firstEle[wp.memWGa*i+j]> -1)accum++;
										 else break;
										}
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.firstEle[wp.memWGa*i+j]<<"\n";
//		for(j=0;j<wp.memWGa;j++)ntape<<wp.nextEle[wp.memWGa*i+j]<<"\n";
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.nextEle[wp.memWGa*i+j]> -1)accum++;
										 else break;
										}
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.nextEle[wp.memWGa*i+j]<<"\n";
		for(j=0;j<4;j++)ntape<<wp.snorm1[i*4+j]<<"\n";
		for(j=0;j<4;j++)ntape<<wp.snorm2[i*4+j]<<"\n";
		for(j=0;j<3;j++)ntape<<wp.circEles[i*3+j]<<"\n";

////////////////////
////////////////////
//////////////////// Attempt to accommodate uncertain edgeEles/edgeNodes  EFP 4/09/2011
//		for(j=0;j<wp.memWGa;j++)ntape<<wp.edgeEles[wp.memWGa*i+j]<<"\n";
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.edgeEles[wp.memWGa*i+j]> -1)accum++;
										 else break;
										}
		ntape<<accum<<"\n";
//		for(j=0;j<accum;j++)ntape<<wp.edgeEles[wp.memWGa*i+j]<<"\n";
//		for(j=0;j<4;j++)ntape<<wp.edgeNodes[i*4+j]<<"\n"; // correct???
		if(accum){
				  for(j=0;j<accum;j++)ntape<<wp.edgeEles[wp.memWGa*i+j]<<"\n";
				  for(j=0;j<4;j++)ntape<<wp.edgeNodes[i*4+j]<<"\n"; // correct???
				 }
////////////////////
////////////////////
////////////////////

//		for(j=0;j<4*wp.memWGa;j++)ntape<<wp.sttEleNodes[i*wp.memWGa*4+j]<<"\n";
		accum=0;for(j=0;j<4*wp.memWGa;j++){if(wp.sttEleNodes[i*wp.memWGa*4+j]> -1)accum++;
										   else break;
										  }
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.sttEleNodes[i*wp.memWGa*4+j]<<"\n";
//		ntape<<wp.curr[i]<<"\n";
//		ntape<<wp.volt[i]<<"\n";
//		ntape<<wp.eff[i]<<"\n";
//		ntape<<wp.speed[i]<<"\n";
//		ntape<<wp.troom[i]<<"\n";
//		ntape<<wp.tmelt[i]<<"\n";
//		ntape<<wp.tcutl[i]<<"\n";
//		ntape<<wp.tcuth[i]<<"\n";
//		ntape<<wp.timeInterval[i]<<"\n";
//		ntape<<wp.maxiHeatStep[i]<<"\n";
//		ntape<<wp.miniHeatStep[i]<<"\n";
//		ntape<<wp.thk1[i]<<"\n";
//		ntape<<wp.thk2[i]<<"\n";
//		ntape<<wp.lstart[i]<<"\n";
//		ntape<<wp.lend[i]<<"\n";

		ntape<<wp.curr[i]<<"\n"<<wp.volt[i]<<"\n"<<wp.eff[i]<<"\n"<<wp.speed[i]<<"\n";
		ntape<<wp.troom[i]<<"\n"<<wp.tmelt[i]<<"\n"<<wp.tcutl[i]<<"\n"<<wp.tcuth[i]<<"\n";
		ntape<<wp.timeInterval[i]<<"\n"<<wp.maxiHeatStep[i]<<"\n"<<wp.miniHeatStep[i]<<"\n";
		ntape<<wp.thk1[i]<<"\n"<<wp.thk2[i]<<"\n";
		ntape<<wp.lstart[i]<<"\n"<<wp.lend[i]<<"\n";

//		for(j=0;j<wp.memWGa;j++)ntape<<wp.hlightel[wp.memWGa*i+j]<<"\n"; //Convention: 1st col= elem side#
		accum=0;for(j=0;j<wp.memWGa;j++){if(wp.hlightel[wp.memWGa*i+j]> -1)accum++; //Convention: 1st col= elem side#
										 else break;
										}
		ntape<<accum<<"\n";
		for(j=0;j<accum;j++)ntape<<wp.hlightel[wp.memWGa*i+j]<<"\n"; //Convention: 1st col= elem side#
		for(j=0;j<3*NDF;j++)ntape<<wp.arrows[3*NDF*i+j]<<"\n"; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		ntape<<wp.n_curr_sttEl[i]<<"\n";
		ntape<<wp.wpTimes[i]<<"\n";
		ntape<<wp.source[i]<<"\n";
		ntape<<wp.mcr[i]<<"\n"; // Not part of original VFT
///////////////////////////
////if(1==1)exit(0);
//honk<<(long)wp.WeldColor[i]<<" Weldcolor "<<i<<"\n";
///////////////////////////
		ntape<<(long)wp.WeldColor[i]<<"\n";
		accum=0;ntape<<accum<<"\n";//Just in case  EFP 4/03/2011
															 }
								 }
//aaaaaaaaaaaaaaaaaaaaa
if(ksw!=2){
//bbbbbbbbbbbbbbbbbbbbb
//honk<<wps.nWeldParamSet<<" pear1\n";
////if(1==1)exit(0);
				 ntape<<wps.nWeldParamSet<<"\n";
				 if(wps.nWeldParamSet){for(i=0;i<wps.nWeldParamSet;i++){
//		ntape<<wps.name[i].t_str()<<"\n";
//		ntape<<wps.curr[i]<<"\n";
//		ntape<<wps.volt[i]<<"\n";
//		ntape<<wps.eff[i]<<"\n";
//		ntape<<wps.speed[i]<<"\n";

//		ntape<<wps.name[i].w_str()<<"\n";
//xxxxxxxxxx
 int bufferSize4=WideCharToMultiByte(CP_UTF8,0,wps.name[i].w_str(), -1,NULL,0,NULL,NULL);
 char* m4=new char[bufferSize4];WideCharToMultiByte(CP_UTF8,0,wps.name[i].w_str(), -1,m4,bufferSize4,NULL,NULL);
 ntape<<m4<<"\n";// EFP 12/10/2014
 delete[] m4;
//xxxxxxxxxx
		ntape<<wps.curr[i]<<"\n"<<wps.volt[i]<<"\n"<<wps.eff[i]<<"\n"<<wps.speed[i]<<"\n";
		accum=0;ntape<<accum<<"\n";  //Just in case of future expansion  EFP 4/03/2011
																	   }
									  }
//honk<<wms.nMatPropSet<<" pear2\n";

//if(1==1)exit(0);

				 ntape<<wms.nMatPropSet<<"\n";
				 if(wms.nMatPropSet){for(i=0;i<wms.nMatPropSet;i++){
//		ntape<<wms.name[i].w_str()<<"\n";
//xxxxxxxxxx
 int bufferSize5=WideCharToMultiByte(CP_UTF8,0,wms.name[i].w_str(), -1,NULL,0,NULL,NULL);
 char* m5=new char[bufferSize5];WideCharToMultiByte(CP_UTF8,0,wms.name[i].w_str(), -1,m5,bufferSize5,NULL,NULL);
 ntape<<m5<<"\n";// EFP 12/10/2014
 delete[] m5;
//xxxxxxxxxx
//		ntape<<wms.matFileName[i].w_str()<<"\n";
//xxxxxxxxxx
 int bufferSize6=WideCharToMultiByte(CP_UTF8,0,wms.matFileName[i].w_str(), -1,NULL,0,NULL,NULL);
 char* m6=new char[bufferSize6];WideCharToMultiByte(CP_UTF8,0,wms.matFileName[i].w_str(), -1,m6,bufferSize6,NULL,NULL);
 ntape<<m6<<"\n";// EFP 12/10/2014
 delete[] m6;
//xxxxxxxxxx
//		ntape<<wms.Steps[i].t_str()<<"\n"; //Emergency EFP 11/11/2010
		ntape<<"0\n";

//		ntape<<wms.mcr[i]<<"\n";
//		ntape<<wms.annjd[i]<<"\n";
//		ntape<<wms.ved[i]<<"\n";
//		ntape<<wms.hetjd[i]<<"\n";
//		ntape<<wms.nprops[i]<<"\n";
//		ntape<<wms.cond[i]<<"\n";
//		ntape<<wms.heat[i]<<"\n";
//		ntape<<wms.den[i]<<"\n";
//		ntape<<wms.Ti[i]<<"\n";
//		ntape<<wms.Ta[i]<<"\n";
//		ntape<<wms.Tm[i]<<"\n";
//		ntape<<int(wms.switc[i])<<"\n";

		ntape<<wms.mcr[i]<<"\n"<<wms.annjd[i]<<"\n"<<wms.ved[i]<<"\n"<<wms.hetjd[i]<<"\n"<<wms.nprops[i]<<"\n";
		ntape<<wms.cond[i]<<"\n"<<wms.heat[i]<<"\n"<<wms.den[i]<<"\n"<<wms.Ti[i]<<"\n"<<wms.Ta[i]<<"\n"<<wms.Tm[i]<<"\n";
		ntape<<int(wms.switc[i])<<"\n";
		accum=0;ntape<<accum<<"\n";  //Just in case of future expansion  EFP 4/03/2011
																   }
									}
		accum=0;ntape<<accum<<"\n";  //Just in case of future expansion  EFP 4/03/2011
//aaaaaaaaaaaaaaaaaaaaa
		  }
//bbbbbbbbbbbbbbbbbbbbb
//
//if(1==1)exit(0);



////					 }
////				 __finally {
//				 Screen->Cursor=Save_Cursor;
////				 }
				 ntape.close();

/////////// Cursor EFP 1/21/2011
//Screen->Cursor=crSizeAll;
///////////
//if(1==1)exit(0);

				}
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open file",L"Failure",MB_OK);}
	  }
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CTSPinterpSolidExecute(TObject *Sender){int solidshellsw=0;CTSPinterpolate_prog(solidshellsw);} //Shell not used anymore
//---------------------------------------------------------------------------
//void __fastcall TForm1::CTSPinterpolateExecute(TObject *Sender)
void TForm1::CTSPinterpolate_prog(int solidshellsw)
{
// Note: Only positive temperatures are output, based on tol=0.001  EFP 8/26/2012
// Correctly, tcuth should be obtained & used for this.... but this code should be OK for combination?
 int nic=0,nrc=0,vcheck=0;
// int solidshellsw=0; //Solid only
 long ntime1=0,nndmax1=0,ntime2=0,nndmax2=0,in=0,ic=0,id=0,
	  index1old=0,index2old=0,index1=0,index2=0,inodemin=0,inodemax=0,isum=0,
	  nndv1=0,nndm1=0,nndv2=0,nndm2=0,larr[5], *nstor1=NULL, *nstor2=NULL;
 float t11=0.f,t12=0.f,t13=0.f,t14=0.f,t15=0.f,rkm=0.f,tstorprox1=0.f,tstorprox2=0.f,tvalv1=0.f,tvalm1=0.f,tvalv2=0.f,tvalm2=0.f,
	   tol=0.001f,darr[5], *rstor1=NULL, *rstor2=NULL, *combo=NULL;
 char descript[76];
 OpenDialog1->Filter=L"Out (*.out)|*.out;*.OUT";
 if(OpenDialog1->Execute())
//   {ifstream viewfile1(OpenDialog1->FileName.t_str(),ios::nocreate,0);
   {ifstream viewfile1(OpenDialog1->FileName.w_str(),ios::nocreate,0);
	if(viewfile1)
	  {
///////////////
//	   TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//	   try {
///////////////
	   ntime1=nndmax1=inodemax=0;inodemin=LONG_INT;
	   while(!viewfile1.eof())
		 {viewfile1.getline(descript,76);
		  if(strlen(descript))
			{
//			 parse_cdm(descript,2,&nic,&nrc,larr,darr);
			 vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
//honk<<ntime1<<" "<<vcheck<<" 1v\n";
			 if(!ntime1 && vcheck<0){
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Terminate: possible time.out file?",MB_OK);exit(0);
									}
			 nndv1=larr[0];ntime1++;if(nndmax1<nndv1)nndmax1=nndv1;  //TBD: Check for errant time.out
			 if(nndv1){if(solidshellsw)for(in=0;in<nndv1;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;if(inodemin>id)inodemin=id;if(inodemax<id)inodemax=id;} //setw(5->7) above & below for more space EFP 12/15/2011
					   else for(in=0;in<nndv1;in++){viewfile1>>id>>t11;if(inodemin>id)inodemin=id;if(inodemax<id)inodemax=id;}
					  }
			}
		 }
	   viewfile1.close();
///////////////
//		   }
//	   __finally {Screen->Cursor=Save_Cursor;}
///////////////
	   extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Reading first temp@@@.out file.\nNext, select second file.",L"Status",MB_OK);
	   OpenDialog2->Filter=L"Out (*.out)|*.out;*.OUT";
	   if(OpenDialog2->Execute())
//		{ifstream viewfile2(OpenDialog2->FileName.t_str(),ios::nocreate,0);
		{ifstream viewfile2(OpenDialog2->FileName.w_str(),ios::nocreate,0);
		 if(OpenDialog2->FileName != OpenDialog1->FileName){
		 if(viewfile2)
		   {
///////////////
//	   TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//	   try {
///////////////
			ntime2=nndmax2=0;
			while(!viewfile2.eof())
			  {viewfile2.getline(descript,76);
			   if(strlen(descript))
				 {
//				  parse_cdm(descript,2,&nic,&nrc,larr,darr);
				  vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
//honk<<ntime2<<" "<<vcheck<<" 2v\n";
				  if(!ntime2 && vcheck<0){
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Terminate: possible time.out file?",MB_OK);exit(0);
										 }
				  nndv2=larr[0];ntime2++;if(nndmax2<nndv2)nndmax2=nndv2;  //TBD: Check for errant time.out
				  if(nndv2){if(solidshellsw)for(in=0;in<nndv2;in++){viewfile2>>id>>t11>>t12>>t13>>t14>>t15;if(inodemin>id)inodemin=id;if(inodemax<id)inodemax=id;}
							else for(in=0;in<nndv2;in++){viewfile2>>id>>t11;if(inodemin>id)inodemin=id;if(inodemax<id)inodemax=id;}
						   }
				 }
			  }
			viewfile2.close();
/////////////////////////
			ifstream viewfile1(OpenDialog1->FileName.w_str(),ios::nocreate,0);
			ifstream viewfile2(OpenDialog2->FileName.w_str(),ios::nocreate,0);
			if(viewfile1 && viewfile2)
			  {nstor1=new long[2*nndmax1];nstor2=new long[2*nndmax2];
			   if(solidshellsw){rstor1=new float[2*nndmax1*5];rstor2=new float[2*nndmax2*5];combo=new float[5*(inodemax-inodemin+1)];}
			   else {rstor1=new float[2*nndmax1];rstor2=new float[2*nndmax2];combo=new float[inodemax-inodemin+1];}
///////
			   viewfile1>>tvalv1>>nndv1; //Note: viewfile1.getline/parse does not combine with viewfile1>>id>>t11, in general.
			   if(nndv1){if(solidshellsw)for(in=0;in<nndv1;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
																 nstor1[in+nndmax1]=id-1;
															   rstor1[5*(in+nndmax1)  ]=t11;
															   rstor1[5*(in+nndmax1)+1]=t12;
															   rstor1[5*(in+nndmax1)+2]=t13;
															   rstor1[5*(in+nndmax1)+3]=t14;
															   rstor1[5*(in+nndmax1)+4]=t15;
															  } //setw(5->7) above & below for more space EFP 12/15/2011
						else for(in=0;in<nndv1;in++){viewfile1>>id>>t11;nstor1[in+nndmax1]=id-1;rstor1[in+nndmax1]=t11;}
					   }
			   viewfile2>>tvalv2>>nndv2;
			   if(nndv2){if(solidshellsw)for(in=0;in<nndv2;in++){viewfile2>>id>>t11>>t12>>t13>>t14>>t15;
															   nstor2[in+nndmax2]=id-1;
															   rstor2[5*(in+nndmax2)  ]=t11;
															   rstor2[5*(in+nndmax2)+1]=t12;
															   rstor2[5*(in+nndmax2)+2]=t13;
															   rstor2[5*(in+nndmax2)+3]=t14;
															   rstor2[5*(in+nndmax2)+4]=t15;
															  } //setw(5->7) above & below for more space EFP 12/15/2011
						else for(in=0;in<nndv2;in++){viewfile2>>id>>t11;nstor2[in+nndmax2]=id-1;rstor2[in+nndmax2]=t11;}
					   }
///////
			   index1=index2=0;
			   ofstream outfile1("tempCombine.out");ofstream outfile2("timeCombine.out");
			   for(ic=0;ic<ntime1+ntime2;ic++)
				 {index1old=index1;index2old=index2;
				  if(index1<ntime1)tstorprox1=tvalv1;else tstorprox1=1.e25f;
				  if(index2<ntime2)tstorprox2=tvalv2;else tstorprox2=1.e25f;
				  if(fabs(tstorprox2-tstorprox1)<tol)
					{
/////////////////////// start
					 if(solidshellsw){
						   for(in=0;in<5*(inodemax-inodemin+1);in++)combo[in]=0.f;
						   for(in=0;in<nndv1;in++){combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]=combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]+rstor1[5*(in+nndmax1)  ];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]+rstor1[5*(in+nndmax1)+1];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]+rstor1[5*(in+nndmax1)+2];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]+rstor1[5*(in+nndmax1)+3];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]+rstor1[5*(in+nndmax1)+4];
													   }
						   for(in=0;in<nndv2;in++){combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]=combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]+rstor2[5*(in+nndmax2)  ];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]+rstor2[5*(in+nndmax2)+1];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]+rstor2[5*(in+nndmax2)+2];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]+rstor2[5*(in+nndmax2)+3];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]+rstor2[5*(in+nndmax2)+4];
													   }
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)isum++;
																	   }
									 }
					 else {for(in=0;in<inodemax-inodemin+1;in++)combo[in]=0.f;
						   for(in=0;in<nndv1;in++)combo[nstor1[in+nndmax1]-inodemin+1]=combo[nstor1[in+nndmax1]-inodemin+1]+rstor1[in+nndmax1];
						   for(in=0;in<nndv2;in++)combo[nstor2[in+nndmax2]-inodemin+1]=combo[nstor2[in+nndmax2]-inodemin+1]+rstor2[in+nndmax2];
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)isum++;}
						  }
					 outfile1<<setw(15)<<scientific<<tvalv1<<setw(12)<<isum<<"\n";
					 outfile2<<setw(12)<<(ic+1)<<setw(15)<<scientific<<tvalv1<<"\n";
					 if(isum) //This is cats ass coding but it works. EFP 3/29/2012
					   {if(solidshellsw)for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)
outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[5*in]<<setw(14)<<combo[5*in+1]<<setw(14)<<combo[5*in+2]<<setw(14)<<combo[5*in+3]<<setw(14)<<combo[5*in+4]<<"\n";
																			 } //setw(5->7) above & below for more space EFP 12/15/2011
						else for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[in]<<"\n";}
					   }
/////////////////////// end
					 index1++;index2++;
					}
				  else if(tstorprox2>tstorprox1)
						 {
/////////////////////// start
					 if(solidshellsw){
						   for(in=0;in<5*(inodemax-inodemin+1);in++)combo[in]=0.f;
						   for(in=0;in<nndv1;in++){combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]=combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]+rstor1[5*(in+nndmax1)  ];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]+rstor1[5*(in+nndmax1)+1];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]+rstor1[5*(in+nndmax1)+2];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]+rstor1[5*(in+nndmax1)+3];
														combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]+rstor1[5*(in+nndmax1)+4];
													   }
						   if(index2>0 && index2<ntime2)
							 {rkm=(tvalv1-tvalm2)/(tvalv2-tvalm2);
							  for(in=0;in<nndv2;in++){combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]=combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]+rkm*rstor2[5*(in+nndmax2)  ];
														   combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]+rkm*rstor2[5*(in+nndmax2)+1];
														   combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]+rkm*rstor2[5*(in+nndmax2)+2];
														   combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]+rkm*rstor2[5*(in+nndmax2)+3];
														   combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]+rkm*rstor2[5*(in+nndmax2)+4];
														  }
							  for(in=0;in<nndm2;in++){combo[5*(nstor2[in]-inodemin+1)  ]=combo[5*(nstor2[in]-inodemin+1)  ]+(1.f-rkm)*rstor2[5*in  ];
														   combo[5*(nstor2[in]-inodemin+1)+1]=combo[5*(nstor2[in]-inodemin+1)+1]+(1.f-rkm)*rstor2[5*in+1];
														   combo[5*(nstor2[in]-inodemin+1)+2]=combo[5*(nstor2[in]-inodemin+1)+2]+(1.f-rkm)*rstor2[5*in+2];
														   combo[5*(nstor2[in]-inodemin+1)+3]=combo[5*(nstor2[in]-inodemin+1)+3]+(1.f-rkm)*rstor2[5*in+3];
														   combo[5*(nstor2[in]-inodemin+1)+4]=combo[5*(nstor2[in]-inodemin+1)+4]+(1.f-rkm)*rstor2[5*in+4];
														  }
							 }
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)isum++;
																	   }
									 }
					 else {for(in=0;in<inodemax-inodemin+1;in++)combo[in]=0.f;
						   for(in=0;in<nndv1;in++)combo[nstor1[in+nndmax1]-inodemin+1]=combo[nstor1[in+nndmax1]-inodemin+1]+rstor1[in+nndmax1];
						   if(index2>0 && index2<ntime2){rkm=(tvalv1-tvalm2)/(tvalv2-tvalm2);
for(in=0;in<nndv2;in++)combo[nstor2[in+nndmax2]-inodemin+1]=combo[nstor2[in+nndmax2]-inodemin+1]+rkm*rstor2[in+nndmax2];
for(in=0;in<nndm2;in++)combo[nstor2[in]-inodemin+1]=combo[nstor2[in]-inodemin+1]+(1.f-rkm)*rstor2[in];
														}
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)isum++;}
						  }
					 outfile1<<setw(15)<<scientific<<tvalv1<<setw(12)<<isum<<"\n";
					 outfile2<<setw(12)<<(ic+1)<<setw(15)<<scientific<<tvalv1<<"\n";
					 if(isum) //This is cats ass coding but it works. EFP 3/29/2012
					   {if(solidshellsw)for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)
outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[5*in]<<setw(14)<<combo[5*in+1]<<setw(14)<<combo[5*in+2]<<setw(14)<<combo[5*in+3]<<setw(14)<<combo[5*in+4]<<"\n";
																			 } //setw(5->7) above & below for more space EFP 12/15/2011
						else for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[in]<<"\n";}
					   }
/////////////////////// end
						  index1++;
						 }
				  else {
/////////////////////// start
					 if(solidshellsw){
						   for(in=0;in<5*(inodemax-inodemin+1);in++)combo[in]=0.f;
						   for(in=0;in<nndv2;in++){combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]=combo[5*(nstor2[in+nndmax2]-inodemin+1)  ]+rstor2[5*(in+nndmax2)  ];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+1]+rstor2[5*(in+nndmax2)+1];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+2]+rstor2[5*(in+nndmax2)+2];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+3]+rstor2[5*(in+nndmax2)+3];
														combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]=combo[5*(nstor2[in+nndmax2]-inodemin+1)+4]+rstor2[5*(in+nndmax2)+4];
													   }
						   if(index1>0 && index1<ntime1)
							 {rkm=(tvalv2-tvalm1)/(tvalv1-tvalm1);
							  for(in=0;in<nndv1;in++){combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]=combo[5*(nstor1[in+nndmax1]-inodemin+1)  ]+rkm*rstor1[5*(in+nndmax1)  ];
														   combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+1]+rkm*rstor1[5*(in+nndmax1)+1];
														   combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+2]+rkm*rstor1[5*(in+nndmax1)+2];
														   combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+3]+rkm*rstor1[5*(in+nndmax1)+3];
														   combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]=combo[5*(nstor1[in+nndmax1]-inodemin+1)+4]+rkm*rstor1[5*(in+nndmax1)+4];
														  }
							  for(in=0;in<nndm1;in++){combo[5*(nstor1[in]-inodemin+1)  ]=combo[5*(nstor1[in]-inodemin+1)  ]+(1.f-rkm)*rstor1[5*in  ];
														   combo[5*(nstor1[in]-inodemin+1)+1]=combo[5*(nstor1[in]-inodemin+1)+1]+(1.f-rkm)*rstor1[5*in+1];
														   combo[5*(nstor1[in]-inodemin+1)+2]=combo[5*(nstor1[in]-inodemin+1)+2]+(1.f-rkm)*rstor1[5*in+2];
														   combo[5*(nstor1[in]-inodemin+1)+3]=combo[5*(nstor1[in]-inodemin+1)+3]+(1.f-rkm)*rstor1[5*in+3];
														   combo[5*(nstor1[in]-inodemin+1)+4]=combo[5*(nstor1[in]-inodemin+1)+4]+(1.f-rkm)*rstor1[5*in+4];
														  }
							 }
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)isum++;
																	   }
									 }
					 else {for(in=0;in<inodemax-inodemin+1;in++)combo[in]=0.f;
						   for(in=0;in<nndv2;in++)combo[nstor2[in+nndmax2]-inodemin+1]=combo[nstor2[in+nndmax2]-inodemin+1]+rstor2[in+nndmax2];
						   if(index1>0 && index1<ntime1){rkm=(tvalv2-tvalm1)/(tvalv1-tvalm1);
for(in=0;in<nndv1;in++)combo[nstor1[in+nndmax1]-inodemin+1]=combo[nstor1[in+nndmax1]-inodemin+1]+rkm*rstor1[in+nndmax1];
for(in=0;in<nndm1;in++)combo[nstor1[in]-inodemin+1]=combo[nstor1[in]-inodemin+1]+(1.f-rkm)*rstor1[in];
														}
						   isum=0;for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)isum++;}
						  }
					 outfile1<<setw(15)<<scientific<<tvalv2<<setw(12)<<isum<<"\n";
					 outfile2<<setw(12)<<(ic+1)<<setw(15)<<scientific<<tvalv2<<"\n";
					 if(isum) //This is cats ass coding but it works. EFP 3/29/2012
					   {if(solidshellsw)for(in=0;in<inodemax-inodemin+1;in++){
															 if(combo[5*in  ]>tol ||
																combo[5*in+1]>tol ||
																combo[5*in+2]>tol ||
																combo[5*in+3]>tol ||
																combo[5*in+4]>tol)
outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[5*in]<<setw(14)<<combo[5*in+1]<<setw(14)<<combo[5*in+2]<<setw(14)<<combo[5*in+3]<<setw(14)<<combo[5*in+4]<<"\n";
																			 } //setw(5->7) above & below for more space EFP 12/15/2011
						else for(in=0;in<inodemax-inodemin+1;in++){if(combo[in]>tol)outfile1<<setw(7)<<(in+inodemin)<<setw(14)<<dec<<showpoint<<setprecision(5)<<combo[in]<<"\n";}
					   }
/////////////////////// end
						index2++;
					   }
				  if(index1==ntime1 && index2==ntime2)break;
////
				  if(index1<ntime1 && index1!=index1old){nndm1=nndv1;tvalm1=tvalv1;
if(nndm1){if(solidshellsw)for(in=0;in<nndm1;in++){nstor1[in]=nstor1[in+nndmax1];
															rstor1[5*in  ]=rstor1[5*(in+nndmax1)  ];
															rstor1[5*in+1]=rstor1[5*(in+nndmax1)+1];
															rstor1[5*in+2]=rstor1[5*(in+nndmax1)+2];
															rstor1[5*in+3]=rstor1[5*(in+nndmax1)+3];
															rstor1[5*in+4]=rstor1[5*(in+nndmax1)+4];
														   }
		  else for(in=0;in<nndm1;in++){nstor1[in]=nstor1[in+nndmax1];rstor1[in]=rstor1[in+nndmax1];}
		 }
viewfile1>>tvalv1>>nndv1;
if(nndv1){if(solidshellsw)for(in=0;in<nndv1;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
												nstor1[in+nndmax1]=id-1;
												rstor1[5*(in+nndmax1)  ]=t11;
												rstor1[5*(in+nndmax1)+1]=t12;
												rstor1[5*(in+nndmax1)+2]=t13;
												rstor1[5*(in+nndmax1)+3]=t14;
												rstor1[5*(in+nndmax1)+4]=t15;
											   } //setw(5->7) above & below for more space EFP 12/15/2011
		  else for(in=0;in<nndv1;in++){viewfile1>>id>>t11;nstor1[in+nndmax1]=id-1;rstor1[in+nndmax1]=t11;}
		 }
														}
				  if(index2<ntime2 && index2!=index2old){nndm2=nndv2;tvalm2=tvalv2;
if(nndm2){if(solidshellsw)for(in=0;in<nndm2;in++){nstor2[in]=nstor2[in+nndmax2];
															rstor2[5*in  ]=rstor2[5*(in+nndmax2)  ];
															rstor2[5*in+1]=rstor2[5*(in+nndmax2)+1];
															rstor2[5*in+2]=rstor2[5*(in+nndmax2)+2];
															rstor2[5*in+3]=rstor2[5*(in+nndmax2)+3];
															rstor2[5*in+4]=rstor2[5*(in+nndmax2)+4];
														   }
		  else for(in=0;in<nndm2;in++){nstor2[in]=nstor2[in+nndmax2];rstor2[in]=rstor2[in+nndmax2];}
		 }
viewfile2>>tvalv2>>nndv2;
if(nndv2){if(solidshellsw)for(in=0;in<nndv2;in++){viewfile2>>id>>t11>>t12>>t13>>t14>>t15;
												nstor2[in+nndmax2]=id-1;
												rstor2[5*(in+nndmax2)  ]=t11;
												rstor2[5*(in+nndmax2)+1]=t12;
												rstor2[5*(in+nndmax2)+2]=t13;
												rstor2[5*(in+nndmax2)+3]=t14;
												rstor2[5*(in+nndmax2)+4]=t15;
											   } //setw(5->7) above & below for more space EFP 12/15/2011
		  else for(in=0;in<nndv2;in++){viewfile2>>id>>t11;nstor2[in+nndmax2]=id-1;rstor2[in+nndmax2]=t11;}
		 }
														}
////
				 }
			   outfile1.close();outfile2.close();delete [] nstor1;delete [] nstor2;delete [] rstor1;delete [] rstor2;delete [] combo;
///////////////
//		   }
//	   __finally {Screen->Cursor=Save_Cursor;}
///////////////
			   extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"tempCombine.out & timeCombine.out files written",L"Success",MB_OK);
			  }
			else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not reopen *.out files",L"Failure",MB_OK);}
/////////////////////////
		   }
		 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open second *.out file",L"Failure",MB_OK);}
														   }
		 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User selected first *.out file twice.",L"Failure",MB_OK);}
		}
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User did not choose second *.out",L"Abandon",MB_OK);}
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open first *.out file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{//enum TMouseButton { mbLeft, mbRight, mbMiddle };
 int CRB=0,CRBsection=0,// Coding for FEMAP users EFP 12/20/2010
iPers=iPersistVFT/100,jPers=(iPersistVFT-100*iPers)/10,
// TB1H=0,P1W=0,
 CCB=0,CRB_sel=0,CRB_selx=0,CRB_ckShape=0,circFlag=0,girthFlag=0;
 long ip=0,ipp=0,ippp=0,signp=0,signm=0,isw=0,nipismin=0,nipismax=0,curiside=0,dumrec=0; // unsigned long prod=1,aflag=0;
 long ik=0,ic=0,iss=0,ies=0,isides=0;//Coding for FEMAP users EFP 12/20/2010
 long NodeNum=0,ie=0,
// iGID=0,
 iside=0,eltype=0,bscode=0,node=0,ieGID=0,t3=1000,t5=100000,t7=10000000,eltype5=0,bscode5=0,
   is=0,in=0,ir=0,numdum=0,eltype1=0,ieGID1=0,ip1=0,is1=0,iside1=0,
   ie1=0,icount=0,ieGID2=0,eltype3=0,ieGID3=0,
   *dumarr=NULL,*dummap=NULL,*duminv=NULL,*dumgrp=NULL;
 float rv=0.f,xave=0.f,yave=0.f,zave=0.f,xnor=0.f,ynor=0.f,znor=0.f,rave=0.f,zero=0.f,dx1=0.f,dy1=0.f,dx3=0.f,dy3=0.f,norm=0.f,RN1=0.f,RN2=0.f,RN3=0.f,
//	   DJD=0.f,SN[20],SG[60],DJR[9+1],
	   HN[9],xc=0.f,yc=0.f,zc=0.f,xnor1=0.f,ynor1=0.f,znor1=0.f,dist=0.f,areafac=0.f;
//                            11: Pivot freehand rotate (timer-based, right mouse button)
//                              & Pan (timer-based, left mouse button)
//                            13: ZoomInContinuous (timer-based, left mouse button BUT SHOULD BE MOUSEWHEEL)
//                            16: MaskElem0
///////////////////////
//TStringBuilder *sb=new TStringBuilder();
//sb->Append( System::UnicodeString("ding") );sb->Append( System::UnicodeString(".out") );
//ofstream zonk( sb->ToString().c_str() );zonk<< sb->ToString().c_str() <<" more wisdom\n";zonk.close();
//extern PACKAGE void __fastcall Beep(void);Application->MessageBox(sb->ToString().w_str(),L"ZONK",MB_OK);
//sb->Clear();
//// Note use of: AppendLine()
//delete sb;
///////////////////////

///////////// New VFT coding ////////////////////////////////
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7},
	 opp_arr8[6]={2,3,0,1,5,4};//TBD: Add vertplot[15] coding
// char extensChar[]="Query ELSET= ",extensChar1[]="Weld Pass= ",extensChar2[]="Sequence# ",extensChar3[]=" of "; //EFP 10/03/2011
// String extensCharS[]={L"Query ELSET= "};
// String extensCharS1[]={L"Weld Pass= "};
// String extensCharS2[]={L"Sequence# "};
// String extensCharS3[]={L" of "};
 String extensCharS=UnicodeString(L"Query ELSET= "),extensCharS1=UnicodeString(L"Weld Pass= "),extensCharS2=UnicodeString(L"Sequence# "),extensCharS3=UnicodeString(L" of ");
 TPoint ptDraw[MAXPOINTS];

// TStringBuilder *sb=new TStringBuilder();
// String extensCharS[]={L"Query ELSET= "};
// String extensCharS1[]={L"Weld Pass= "};
// String extensCharS2[]={L"Sequence# "};
// String extensCharS3[]={L" of "};

 //Convention: CRB_sel=CreateLinWeldPass->CheckISEL
//  0-> Create full length
//  1-> Create partial length
//  2-> Edit full length
//  3-> Edit partial length
//  4-> Create full length from entire weld group section
////////////////////////////////////////////
 if(Button==TMouseButton::mbLeft)
   {if(FD_LButtonstatus==1)
	  {zoomDrag=true;zoomRect.left=zoomRect.right=X;zoomRect.top=zoomRect.bottom=Y;
	  }


// Warning: The code does not use "Mouse/focus capture" so there may be a problem with multiple modeless dialogs (e.g. QElem, QNdist)
	else if(FD_LButtonstatus==2) // Convert to FORTRAN format (ie. begin with 1)
	  {if(QElem){delete QElem;QElem=NULL;}if(QNDist){delete QNDist;QNDist=NULL;}
	   NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(!QNode){QNode=new TForm3(base.node_map[NodeNum]+1,indat.bc1[NDF*NodeNum],indat.bc1[NDF*NodeNum+1],indat.bc1[NDF*NodeNum+2],this);
				  QNode->Caption=L"Query node";QNode->Label1->Caption=L"Node#";
				  QNode->Label2->Caption=L"X";QNode->Label3->Caption=L"Y";QNode->Label4->Caption=L"Z";
//	   QNode->ShowModal();
				  QNode->Show();
				 }
	   else {QNode->setNodeQueryNum(base.node_map[NodeNum]+1);QNode->setNodeQueryXCor(indat.bc1[NDF*NodeNum  ]);
			 QNode->setNodeQueryYCor(indat.bc1[NDF*NodeNum+1]);QNode->setNodeQueryZCor(indat.bc1[NDF*NodeNum+2]);//QNode->setNodeQueryValue(iResSel+1,rv);
			}
	  }
	else if(FD_LButtonstatus==3) //This option must have active "Close" Button1  EFP 3/29/2012
// Convert to FORTRAN format (ie. begin with 1)
	  {if(QNode){delete QNode;QNode=NULL;}if(QNDist){delete QNDist;QNDist=NULL;}
	   FDelem_interrog1(X,Y,&ie,&iside,&RN1,&RN2,&RN3,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if(ie>=0 && ie<base.nelt)
				{eltype=indat.matno[ie]/t7;bscode=(indat.matno[ie]-eltype*t7)/t5;node=(indat.matno[ie]-eltype*t7-bscode*t5)/t3;
				 FDelem_interrogpt1(ie,iside,eltype,node,RN1,RN2,RN3,&xave,&yave,&zave,&xnor,&ynor,&znor,&rave,&areafac,base.nop1,indat.nop1,base.c1,indat.result);
//				 if(iResSel+1)rv=rave;else rv=0.f;
				 ieGID1=ieGID=indat.arrELSET[ie];isw=0;
//				 if(!QElem){QElem=new TForm6(base.el_map[ie]+1,iside+1,ieGID+1,xave,yave,zave,xnor,ynor,znor,iResSel+1,rv,this);
				 if(!QElem){QElem=new TForm6(base.el_map[ie]+1,iside+1,ieGID+1,xave,yave,zave,xnor,ynor,znor,1,rv,this);
							QElem->setFacetArea(areafac);
							QElem->Button1->Enabled=true;QElem->Button1->Visible=true; //EFP 3/29/2012
if(ieGID==0){QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
			 QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
			 QElem->Height=115;
			 QElem->Caption=L"Query element: base metal";
			}
else if(!isw){
//			  char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
//			  StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
//			  StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
			  QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
			  QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
			  QElem->Height=115;
//			  QElem->Caption=fnNeed;
//			  delete [] fnNeed;
			  QElem->Caption=(extensCharS+base.groupsname[ieGID-1]).w_str();

////			  sb->Append(System::UnicodeString(extensChar));
//////			  sb->Append(extensCharS);
////			  sb->Append(System::UnicodeString(L"ding"));
////			  sb->Append(System::UnicodeString(base.groupsname[ieGID-1]));
////			  QElem->Caption=sb->ToString();
			 }
else {
//			  char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
//			  StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
//			  StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
//			  QElem->Caption=fnNeed;
//			  delete [] fnNeed;
			  QElem->Caption=(extensCharS+base.groupsname[ieGID-1]).w_str();
	  QElem->Label11->Enabled=true;QElem->Label11->Visible=true;
//	  char *fnNeed1=new char[strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1]; //Correction EFP 12/16/2011
//	  StringCchCopy(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,extensChar1);
//	  StringCchCat(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,wp.name[ieGID1-base.allGrp].t_str());
//	  QElem->Label11->Caption=fnNeed1;
//	  delete [] fnNeed1;
	  QElem->Label11->Caption=(extensCharS1+wp.name[ieGID1-base.allGrp]).w_str();
//
	  QElem->Label12->Enabled=true;QElem->Label12->Visible=true;
//	  char *fnNeed2=new char[strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1]; //EFP 3/29/2012
//	  StringCchCopy(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar2);
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str());
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar3);
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.nWeldPass)).t_str());
//	  QElem->Label12->Caption=fnNeed2;
//	  delete [] fnNeed2;
	  QElem->Label12->Caption= extensCharS2+IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp]))+extensCharS3+IntToStr(__int64(wp.nWeldPass));
//
//	  QElem->Height=130;
	  QElem->Height=169;
	 }
							QElem->Show();
						   }
				 else {QElem->setElemQueryNum(base.el_map[ie]+1);//Correction EFP 12/20/2010
					   QElem->setElemQuerySide(iside+1);QElem->setElemQueryGID(ieGID+1);QElem->setElemQueryXCor(xave);
					   QElem->setElemQueryYCor(yave);QElem->setElemQueryZCor(zave);QElem->setElemQueryNx(xnor);QElem->setElemQueryNy(ynor);
					   QElem->setElemQueryNz(znor);//QElem->setElemQueryValue(iResSel+1,rv);
					   QElem->setFacetArea(areafac);
if(ieGID==0){QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
			 QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
			 QElem->Height=115;
			 QElem->Caption=L"Query element: base metal";
			}
else if(!isw){
//			  char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
//			  StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
//			  StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
//			  QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
//			  QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
//			  QElem->Height=115;
//			  QElem->Caption=fnNeed;
//			  delete [] fnNeed;
			  QElem->Caption=(extensCharS+base.groupsname[ieGID-1]).w_str();
			 }
else {
//			  char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
//			  StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
//			  StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
//			  QElem->Caption=fnNeed;delete [] fnNeed;
			  QElem->Caption=(extensCharS+base.groupsname[ieGID-1]).w_str();
	  QElem->Label11->Enabled=true;QElem->Label11->Visible=true;
//	  char *fnNeed1=new char[strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1]; //Correction EFP 12/16/2011
//	  StringCchCopy(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,extensChar1);
///	  StringCchCat(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,wp.name[ieGID1-base.allGrp].t_str());
//	  QElem->Label11->Caption=fnNeed1;delete [] fnNeed1;
	  QElem->Label11->Caption=(extensCharS1+wp.name[ieGID1-base.allGrp]).w_str();
//
	  QElem->Label12->Enabled=true;QElem->Label12->Visible=true;
//	  char *fnNeed2=new char[strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1]; //EFP 3/29/2012
//	  StringCchCopy(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar2);
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str());
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar3);
//	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
//							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.nWeldPass)).t_str());
//	  QElem->Label12->Caption=fnNeed2;delete [] fnNeed2;
	  QElem->Label12->Caption=extensCharS2+IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp]))+extensCharS3+IntToStr(__int64(wp.nWeldPass));
//
	  QElem->Height=169;
	 }


					  }
				}
//	   else {if(!QElem){QElem=new TForm6(0,0,0,zero,zero,zero,zero,zero,zero,iResSel+1,rv,this);
	   else {if(!QElem){QElem=new TForm6(0,0,0,zero,zero,zero,zero,zero,zero,1,rv,this);
						QElem->setFacetArea(zero);
						QElem->Button1->Enabled=true;QElem->Button1->Visible=true; //EFP 3/29/2012
						QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
						QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
						QElem->Caption=L"Query element: Click again";
						QElem->Show();
					   }
			 else {QElem->setElemQueryNum(0);//Correction EFP 12/20/2010
				   QElem->setElemQuerySide(0);QElem->setElemQueryGID(0);QElem->setElemQueryXCor(zero);
				   QElem->setElemQueryYCor(zero);QElem->setElemQueryZCor(zero);QElem->setElemQueryNx(zero);QElem->setElemQueryNy(zero);
				   QElem->setElemQueryNz(zero);//QElem->setElemQueryValue(iResSel+1,rv);
				   QElem->setFacetArea(zero);
				   QElem->Label11->Enabled=false;QElem->Label11->Visible=false;
				   QElem->Label12->Enabled=false;QElem->Label12->Visible=false;
				   QElem->Caption=L"Query element: Click again";
				  }
			}
	  }

	else if(FD_LButtonstatus==21)
// Convert to FORTRAN format (ie. begin with 1)
	  {if(QElem){delete QElem;QElem=NULL;}if(QNode){delete QNode;QNode=NULL;}
	   if(qndNodeNum1> -1)
		 {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
		  Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
		  Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
		  rave=sqrt((indat.bc1[NDF*NodeNum  ]-indat.bc1[NDF*qndNodeNum1  ])*(indat.bc1[NDF*NodeNum  ]-indat.bc1[NDF*qndNodeNum1  ])+
					(indat.bc1[NDF*NodeNum+1]-indat.bc1[NDF*qndNodeNum1+1])*(indat.bc1[NDF*NodeNum+1]-indat.bc1[NDF*qndNodeNum1+1])+
					(indat.bc1[NDF*NodeNum+2]-indat.bc1[NDF*qndNodeNum1+2])*(indat.bc1[NDF*NodeNum+2]-indat.bc1[NDF*qndNodeNum1+2]));
		  QNDist->setQNDnodeN(2,base.node_map[NodeNum]+1);
		  QNDist->setQNDnodeXYZD(4,indat.bc1[NDF*NodeNum  ]);
		  QNDist->setQNDnodeXYZD(5,indat.bc1[NDF*NodeNum+1]);QNDist->setQNDnodeXYZD(6,indat.bc1[NDF*NodeNum+2]);
		  QNDist->setQNDnodeXYZD(10,rave);
		  if(rave>0.000001f){RN1=(indat.bc1[NDF*NodeNum  ]-indat.bc1[NDF*qndNodeNum1  ])/rave;if(RN1<0.001f && RN1> -0.001f)RN1=0.f;
							 RN2=(indat.bc1[NDF*NodeNum+1]-indat.bc1[NDF*qndNodeNum1+1])/rave;if(RN2<0.001f && RN2> -0.001f)RN2=0.f;
							 RN3=(indat.bc1[NDF*NodeNum+2]-indat.bc1[NDF*qndNodeNum1+2])/rave;if(RN3<0.001f && RN3> -0.001f)RN3=0.f;
							 QNDist->setQNDnodeXYZD(7,RN1);QNDist->setQNDnodeXYZD(8,RN2);QNDist->setQNDnodeXYZD(9,RN3);
							}
		  qndNodeNum1= -1;
		 }
	   else
		 {
		  NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
		  Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
		  Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
		  qndNodeNum1=NodeNum;if(!QNDist){QNDist=new TForm4(this);QNDist->Caption=L"Query nodal distance";
										  QNDist->Label1->Caption=L"N#1";QNDist->Edit1->Text=L"***";
										  QNDist->Label2->Caption=L"X";QNDist->Label3->Caption=L"Y";QNDist->Label4->Caption=L"Z";
										  QNDist->Edit2->Text=L"***";QNDist->Edit3->Text=L"***";QNDist->Edit4->Text=L"***";
										  QNDist->Label5->Caption=L"N#2";QNDist->Edit5->Text=L"***";
										  QNDist->Label6->Caption=L"X";QNDist->Label7->Caption=L"Y";QNDist->Label8->Caption=L"Z";
										  QNDist->Edit6->Text=L"***";QNDist->Edit7->Text=L"***";QNDist->Edit8->Text=L"***";
										  QNDist->Label9->Caption=L"Dist";QNDist->Edit9->Text=L"***";
										  QNDist->Label10->Caption=L"DirX";QNDist->Label11->Caption=L"DirY";QNDist->Label12->Caption=L"DirZ";
										  QNDist->Edit10->Text=L"***";QNDist->Edit11->Text=L"***";QNDist->Edit12->Text=L"***";
										  QNDist->Button1->Caption=L"Close";QNDist->Color=clRed;QNDist->Show();
										 }
		  QNDist->setQNDnodeN(1,base.node_map[NodeNum]+1);
		  QNDist->setQNDnodeXYZD(1,indat.bc1[NDF*NodeNum  ]);
		  QNDist->setQNDnodeXYZD(2,indat.bc1[NDF*NodeNum+1]);QNDist->setQNDnodeXYZD(3,indat.bc1[NDF*NodeNum+2]);
		  QNDist->setQNDnodeN(-2,base.node_map[NodeNum]+1);
		  QNDist->setQNDnodeXYZD(-4,rv);QNDist->setQNDnodeXYZD(-5,rv);
		  QNDist->setQNDnodeXYZD(-6,rv);QNDist->setQNDnodeXYZD(-7,rv);QNDist->setQNDnodeXYZD(-8,rv);
		  QNDist->setQNDnodeXYZD(-9,rv);QNDist->setQNDnodeXYZD(-10,rv);
		 }
	  }


//	else if(FD_LButtonstatus==10){if(ANLINcount<ANNOT_CHAR){AnLINIndex[4*ANLINcount]=X;AnLINIndex[4*ANLINcount+1]=Y;}}
	else if(FD_LButtonstatus==16){if(polypts==0){polycord[0]=X;polycord[1]=ClientHeight-Y;polypts++;Canvas->MoveTo(X,Y);}}
/////////////////////////// Reuse zoomRect.etc   FormMouse
	else if(FD_LButtonstatus==8){zoomRect.left=zoomRect.right=X;zoomRect.top=zoomRect.bottom=Y;}
///////////////////////////
	else if(FD_LButtonstatus==11){

//honk<<X<<" "<<Y<<" FMD mbLeft 11\n";
								  zoomRect.left=zoomRect.right=X;zoomRect.top=zoomRect.bottom=Y;
								  stateVFT=2;iplotflag=2;Timer1->Interval=250;Timer1->Enabled=true;
								 }
	else if(FD_LButtonstatus==13){iplotflag=2;iCullyesno=1;Timer1->Interval=250;Timer1->Enabled=true;}
/*
	else if(FD_LButtonstatus==22) //Pistol-shot weld direction changes  EFP 2/23/2012
	  {if(QNode){delete QNode;QNode=NULL;}if(QNDist){delete QNDist;QNDist=NULL;} //This option must have inactive "Close" Button1  EFP 3/29/2012
	   FDelem_interrog1(X,Y,&ie,&iside,&RN1,&RN2,&RN3,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if(ie>=0 && ie<base.nelt)
				{eltype=indat.matno[ie]/t7;bscode=(indat.matno[ie]-eltype*t7)/t5;node=(indat.matno[ie]-eltype*t7-bscode*t5)/t3;
				 FDelem_interrogpt1(ie,iside,eltype,node,RN1,RN2,RN3,&xave,&yave,&zave,&xnor,&ynor,&znor,&rave,&areafac,base.nop1,indat.nop1,base.c1,indat.result);
				 if(iResSel+1)rv=rave;else rv=0.f;
ieGID1=ieGID=indat.arrELSET[ie];isw=1;

				 if(ieGID==0 || !isw){if(QElem){delete QElem;QElem=NULL;}
									  extern PACKAGE void __fastcall Beep(void);
									  Application->MessageBox(L"Left-click again to seek weldpass or right-click to quit",L"Weldpass not found",MB_OK);
									 }
				 else {if(!QElem){QElem=new TForm6(base.el_map[ie]+1,iside+1,ieGID+1,xave,yave,zave,xnor,ynor,znor,iResSel+1,rv,this);
								  QElem->Button1->Enabled=false;QElem->Button1->Visible=false;//EFP 3/29/2012
								  char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
								  StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
								  StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
								  QElem->Caption=fnNeed;delete [] fnNeed;
								  QElem->Label11->Enabled=true;QElem->Label11->Visible=true;
								  char *fnNeed1=new char[strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1]; //Correction EFP 12/16/2011
								  StringCchCopy(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,extensChar1);
								  StringCchCat(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,wp.name[ieGID1-base.allGrp].t_str());
								  QElem->Label11->Caption=fnNeed1;delete [] fnNeed1;
//
	  QElem->Label12->Enabled=true;QElem->Label12->Visible=true;
	  char *fnNeed2=new char[strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1]; //EFP 3/29/2012
	  StringCchCopy(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar2);
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str());
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar3);
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.nWeldPass)).t_str());
	  QElem->Label12->Caption=fnNeed2;delete [] fnNeed2;
//
								  QElem->Height=114;
								  QElem->Show();
								 }
					   else {QElem->setElemQueryNum(base.el_map[ie]+1);//Correction EFP 12/20/2010
							 QElem->setElemQuerySide(iside+1);QElem->setElemQueryGID(ieGID+1);QElem->setElemQueryXCor(xave);
							 QElem->setElemQueryYCor(yave);QElem->setElemQueryZCor(zave);QElem->setElemQueryNx(xnor);QElem->setElemQueryNy(ynor);
							 QElem->setElemQueryNz(znor);QElem->setElemQueryValue(iResSel+1,rv);
							 char *fnNeed=new char[strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1];
							 StringCchCopy(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,extensChar);
							 StringCchCat(fnNeed,strlen(base.groupsname[ieGID-1].t_str())+strlen(extensChar)+1,base.groupsname[ieGID-1].t_str());
							 QElem->Caption=fnNeed;delete [] fnNeed;
//							 QElem->Label11->Enabled=true;QElem->Label11->Visible=true;//Not necessary?
							 char *fnNeed1=new char[strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1]; //Correction EFP 12/16/2011
							 StringCchCopy(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,extensChar1);
							 StringCchCat(fnNeed1,strlen(wp.name[ieGID1-base.allGrp].t_str())+strlen(extensChar1)+1,wp.name[ieGID1-base.allGrp].t_str());
							 QElem->Label11->Caption=fnNeed1;delete [] fnNeed1;
//
//	  QElem->Label12->Enabled=true;QElem->Label12->Visible=true; //Not necessary???
	  char *fnNeed2=new char[strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1]; //EFP 3/29/2012
	  StringCchCopy(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar2);
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str());
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,extensChar3);
	  StringCchCat(fnNeed2,strlen(extensChar2)+strlen(IntToStr(__int64(wp.seqNum[ieGID1-base.allGrp])).t_str())+
							 strlen(extensChar3)+strlen(IntToStr(__int64(wp.nWeldPass)).t_str())+1,IntToStr(__int64(wp.nWeldPass)).t_str());
	  QElem->Label12->Caption=fnNeed2;delete [] fnNeed2;
//
							 QElem->Height=114;
							}
///////////////////////////
					   iPersistVFT=iPersistVFT+10*(1-jPers); //reset[]= 10*sequence+direction, where seq starts with 1  EFP 3/28/2012
					   RevProg0(ieGID1-base.allGrp);ip=wp.reset[ieGID1-base.allGrp]-10*(wp.reset[ieGID1-base.allGrp]/10);
					   wp.reset[ieGID1-base.allGrp]=10*(wp.reset[ieGID1-base.allGrp]/10)+1-ip;
					   for(ir=0;ir<wp.nWeldPass;ir++){ip=wp.util_arr[ir]/10;
													  wp.util_arr[ir]=wp.util_arr[ir]-10*ip;
													  wp.util_arr[ir]=wp.util_arr[ir]+10; //Activate all
													 }
//	  ShowWP_hide->Checked=false;ShowWP_last->Checked=false;ShowWP_select->Checked=false;ShowWPout_All->Checked=true;
					   Invalidate();
///////////////////////////
					  }

				}
	   else {if(QElem){delete QElem;QElem=NULL;}
			 extern PACKAGE void __fastcall Beep(void);
			 Application->MessageBox(L"Left-click again to seek weldpass or right-click to quit",L"Weldpass not found",MB_OK);
			}
	  }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
*/




	else if(FD_LButtonstatus==17)
// Convert to FORTRAN format (ie. begin with 1)
	  {

	   FDelem_interrog1(X,Y,&ie,&iside,&RN1,&RN2,&RN3,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if(ie>=0){eltype=indat.matno[ie]/t7;bscode=(indat.matno[ie]-eltype*t7)/t5;node=(indat.matno[ie]-eltype*t7-bscode*t5)/t3;
				 eltype3=base.orig_matno[ie]/t7;
wp.pending=0;wp.elStart=ie;ieGID=ieGID3=indat.arrELSET[ie];
				 if(eltype!=8 || eltype3!=8){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex element selected for weld pass processing",L"Terminate",MB_OK);exit(0);}
//////////////////////////////////
				}
	   if(CreateLinWeldPass)
				{
				if(CreateLinWeldPass->PageControl1->TabIndex==0)
				{CRB=CreateLinWeldPass->CheckRadioB;CCB=CreateLinWeldPass->CheckCheckB;
				 CRB_selx=CreateLinWeldPass->CheckISEL;
				 if(CRB_selx==1)     {CRB_sel=1;CRBsection=0;}
////				 else if(CRB_selx==2){CRB_sel=0;CRBsection=0;} //CRB_selx=4/5/6/7 are Edit
////				 else if(CRB_selx==3){CRB_sel=1;CRBsection=1;}    // EFP 9/22/2010  Note: CRB_sel=0 -> Full;1 -> Partial
				 else if(CRB_selx==2){CRB_sel=0;CRBsection=1;}    // EFP 12/21/2010  Note: CRBsection=0 ->User-selected width, 1->Entire section
				 else if(CRB_selx==3){CRB_sel=1;CRBsection=1;}
				 else if(CRB_selx==4){CRB_sel=0;CRBsection=0;}
				 else if(CRB_selx==5){CRB_sel=1;CRBsection=0;}
				 else if(CRB_selx==6){CRB_sel=0;CRBsection=1;}
				 else if(CRB_selx==7){CRB_sel=1;CRBsection=1;}
				 else                {CRB_sel=0;CRBsection=0;}
				 CRB_ckShape=CreateLinWeldPass->CheckShape; // 0=noncircular,1=fullcircle,2=partialcircle,3=fullgirth,4=partialgirth
				 circFlag=(wp.boolFlags[wp.PRECORD]-100*(wp.boolFlags[wp.PRECORD]/100))/10;
				 girthFlag=(wp.boolFlags[wp.PRECORD]-10000*(wp.boolFlags[wp.PRECORD]/100000))/10000;
				 if(CRB_ckShape==1 || CRB_ckShape==2){wp.boolFlags[wp.PRECORD]=wp.boolFlags[wp.PRECORD]+10*(1-circFlag);
													  circFlag=1;girthFlag=0;
													 }
				 else if(CRB_ckShape==3 || CRB_ckShape==4){wp.boolFlags[wp.PRECORD]=wp.boolFlags[wp.PRECORD]+10000*(1-girthFlag);
														   circFlag=0;girthFlag=1;
														  }
				 if(CRB==1) // All Start Elements
				   {if(ieGID && (node==8 || node==20))
					  {
					   if(wp.count_curr_sttEl==0)
						 {wp.avis=10*(wp.avis/10); //EFP 6/26/2011 Set first col to zero
						  if(wp.CreateWPassMode)wp.GIDwp=10*ieGID3+iside; // The GID/iside of the first facet to be clicked becomes the definitive GID.
						  else wp.GIDwp=10*(1+wp.nWeldGroup+wp.PRECORD)+wp.temp_eles[wp.memWGa*wp.PRECORD+0]-10*(wp.temp_eles[wp.memWGa*wp.PRECORD+0]/10);
						 }
					   else if(!wp.CreateWPassMode){wp.count_curr_sttEl=0; // EFP 3/30/2012
													wp.GIDwp=10*(1+wp.nWeldGroup+wp.PRECORD)+wp.temp_eles[wp.memWGa*wp.PRECORD+0]-10*(wp.temp_eles[wp.memWGa*wp.PRECORD+0]/10);
												   }
					   if((wp.CreateWPassMode && ieGID==wp.GIDwp/10) || //Remove iside check EFP 4/10/2011
						  (!wp.CreateWPassMode && (ieGID==wp.GIDwp/10 || ieGID==wp.prevGID[wp.PRECORD]))) //Revision: Generalize iside EFP 5/02/2010
										  {
		  ir=ieGID-9*(ieGID/9);
		  if     (ir==0)Canvas->Brush->Color=clRed;
		  else if(ir==1)Canvas->Brush->Color=clLtGray; // Coding for color contrast
		  else if(ir==2)Canvas->Brush->Color=clBlue;
		  else if(ir==3)Canvas->Brush->Color=clGreen;
		  else if(ir==4)Canvas->Brush->Color=clLime;
		  else if(ir==5)Canvas->Brush->Color=clNavy;
		  else if(ir==6)Canvas->Brush->Color=clYellow;
		  else if(ir==7)Canvas->Brush->Color=clOlive;
		  else          Canvas->Brush->Color=clMaroon;
		  if     (ir==0)Canvas->Pen->Color=clMaroon; // Coding for double color contrast
		  else if(ir==1)Canvas->Pen->Color=clRed;
		  else if(ir==2)Canvas->Pen->Color=clLtGray;
		  else if(ir==3)Canvas->Pen->Color=clBlue;
		  else if(ir==4)Canvas->Pen->Color=clGreen;
		  else if(ir==5)Canvas->Pen->Color=clLime;
		  else if(ir==6)Canvas->Pen->Color=clNavy;
		  else if(ir==7)Canvas->Pen->Color=clYellow;
		  else          Canvas->Pen->Color=clOlive;
		  Canvas->Pen->Width=4;
Canvas->Brush->Color=CreateLinWeldPass->CheckWeldColor;
Canvas->Pen->Color=clBlack;  //Source of our "All Black" problem ???
wp.WeldColor[wp.PRECORD]=CreateLinWeldPass->CheckWeldColor;
for(ip=0;ip<4;ip++){ptDraw[ip].x=int(indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]]+0.5f);
					ptDraw[ip].y=ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]+1]+0.5f);
				   }
Canvas->Polygon(ptDraw,4-1);
		  Canvas->Pen->Width=1;
										   if(wp.CreateWPassMode){ //Creating
										   isw=1;
										   if(wp.nWeldPass){for(ip=0;ip<wp.nWeldPass;ip++)
															  {for(in=0;in<wp.memWGa;in++)
																 {if(wp.eles[wp.memWGa*ip+in]>=0)
																	{if(wp.eles[wp.memWGa*ip+in]/10==ie)
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Element already in another weld pass",L"Ignore",MB_OK);isw=0;}
																	}
																  else break;
																 }
															  }
														   }
										   if(wp.count_curr_sttEl){for(in=0;in<wp.count_curr_sttEl;in++)
																	if(wp.sttEles[wp.memWGa*wp.nWeldPass+in]/10==ie)
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Duplicate element in weld pass",L"Ignore",MB_OK);isw=0;}
																  }
																 }
										   else {                         //Editing
										   isw=1;
										   if(wp.count_curr_sttEl){for(in=0;in<wp.count_curr_sttEl;in++)if(wp.sttEles[wp.memWGa*wp.PRECORD+in]/10==ie)
{extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Duplicate element in weld pass",L"Ignore",MB_OK);isw=0;}
																  }
												}
										   if(isw)
											 {wp.sttEles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl]=10*ie+iside;

//honk<<wp.count_curr_sttEl<<" STARTelDetailsURU "<<ie<<" "<<iside<<" "<<wp.memWGa<<" "<<wp.PRECORD<<"\n";
											  for(ip=0;ip<4;ip++)wp.sttEleNodes[wp.memWGa*4*wp.PRECORD+4*wp.count_curr_sttEl+ip]=base.nop1[MXNPEL*ie+gdata8[4*iside+ip]];
											  wp.count_curr_sttEl=wp.count_curr_sttEl+1;
											 }
										  }
					   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not in current weld group or not current start face.",L"Ignore",MB_OK);}
					  }
					else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not hex-element or not in a weld group.",L"Ignore",MB_OK);}
				   }
				 else if(CRB==2)  // One Stop Element
				   {
					wp.pending=0;//EFP 9/01/2011  (Actually no longer needed because of wp.elStart below)
					wp.elStart= -1;//EFP 9/02/2011
///////////////////////////////// start EFP 4/17/2012
if(!wp.CreateWPassMode){
				 CRB_selx=CreateLinWeldPass->CheckFunction;
				 if(CRB_selx==1)     {CRB_sel=1;CRBsection=0;}
////				 else if(CRB_selx==2){CRB_sel=0;CRBsection=0;} //CRB_selx=4/5/6/7 are Edit
////				 else if(CRB_selx==3){CRB_sel=1;CRBsection=1;}    // EFP 9/22/2010  Note: CRB_sel=0 -> Full;1 -> Partial
				 else if(CRB_selx==2){CRB_sel=0;CRBsection=1;}    // EFP 12/21/2010  Note: CRBsection=0 ->User-selected width, 1->Entire section
				 else if(CRB_selx==3){CRB_sel=1;CRBsection=1;}
				 else if(CRB_selx==4){CRB_sel=0;CRBsection=0;}
				 else if(CRB_selx==5){CRB_sel=1;CRBsection=0;}
				 else if(CRB_selx==6){CRB_sel=0;CRBsection=1;}
				 else if(CRB_selx==7){CRB_sel=1;CRBsection=1;}
				 else                {CRB_sel=0;CRBsection=0;}
					   }
					if((CRB_sel==0 || (CRB_sel==1 &&
						 ((!wp.CreateWPassMode && ieGID3==wp.prevGID[wp.PRECORD]) ||
						  (wp.CreateWPassMode && ieGID3==wp.GIDwp/10)))) &&  //EFP 11/23/2010
					   (node==8 || node==20) && wp.count_curr_sttEl) //EFP 1/30/2011   Remember to test for start-stop coincidence...

// DIRE WARNING: When selecting a stop element in EDIT mode, the user must chose one in line with the chosen start elements.
// The software does not test for this currently.
					  {

TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
		  ir=ieGID-9*(ieGID/9);
		  if     (ir==0)Canvas->Brush->Color=clOlive; // Coding for triple color contrast
		  else if(ir==1)Canvas->Brush->Color=clMaroon;
		  else if(ir==2)Canvas->Brush->Color=clRed;
		  else if(ir==3)Canvas->Brush->Color=clLtGray;
		  else if(ir==4)Canvas->Brush->Color=clBlue;
		  else if(ir==5)Canvas->Brush->Color=clGreen;
		  else if(ir==6)Canvas->Brush->Color=clLime;
		  else if(ir==7)Canvas->Brush->Color=clNavy;
		  else          Canvas->Brush->Color=clYellow;
		  if     (ir==0)Canvas->Pen->Color=clMaroon; // Coding for double color contrast
		  else if(ir==1)Canvas->Pen->Color=clRed;
		  else if(ir==2)Canvas->Pen->Color=clLtGray;
		  else if(ir==3)Canvas->Pen->Color=clBlue;
		  else if(ir==4)Canvas->Pen->Color=clGreen;
		  else if(ir==5)Canvas->Pen->Color=clLime;
		  else if(ir==6)Canvas->Pen->Color=clNavy;
		  else if(ir==7)Canvas->Pen->Color=clYellow;
		  else          Canvas->Pen->Color=clOlive;
		  Canvas->Pen->Width=4;
xnor=ynor=znor=0.f;
for(ip=0;ip<4;ip++){ptDraw[ip].x=int(indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]]+0.5f);
					ptDraw[ip].y=ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]+1]+0.5f);
					xnor=xnor+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]  ];
					ynor=ynor+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]+1];
					znor=znor+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*iside+ip]]+2];
				   }
xnor=xnor/4.f;ynor=ynor/4.f;znor=znor/4.f;
Canvas->Polygon(ptDraw,4-1);
		  Canvas->Pen->Width=1;
					   wp.stpEle[wp.PRECORD]=10*ie+iside;
					   dumarr=new long[base.nelt];duminv=new long[base.nelt];dumgrp=new long[base.nelt];// Coding for FEMAP users EFP 12/20/2010
					   for(ip=0;ip<base.nelt;ip++)duminv[ip]= -1;
					   is=0;for(ip=0;ip<base.nelt;ip++){
//														eltype2=base.orig_matno[ip]/t7;bscode2=(base.orig_matno[ip]-eltype2*t7)/t5;
//														node2=(base.orig_matno[ip]-eltype2*t7-bscode2*t5)/t3;

//														eltype=indat.matno[ip]/t7;bscode=(indat.matno[ip]-eltype*t7)/t5;
//														node=(indat.matno[ip]-eltype*t7-bscode*t5)/t3;

														dumgrp[ip]=0;//EFP 8/11/2011
////														for(ipx=base.trackELSET[ip]+1;ipx<base.trackELSET[ip+1];ipx++)
////														  {ieGID= base.orig_arrELSET[ipx];//EFP 8/13/2011
////														   ieGID2=base.orig_arrELSET[ipx];
////														   if((wp.CreateWPassMode && ieGID==wp.GIDwp/10) ||
////															  (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))dumgrp[ip]= -10;//EFP 8/11/2011
////														   if((wp.CreateWPassMode && wp.GIDwp/10 ==ieGID) ||
////															  (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))
////															 {dumarr[is]=ip;duminv[ip]=is;is++;
////															  break;
////															 }
////														  }
//////														ipx=base.trackELSET[ip]+1; //VFTsolid only  EFP 3/05/2012
//////														ieGID= base.orig_arrELSET[ipx];//EFP 8/13/2011
//////														ieGID2=base.orig_arrELSET[ipx];
//////														if((wp.CreateWPassMode && ieGID==wp.GIDwp/10) ||
////// 														   (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))dumgrp[ip]= -10;//EFP 8/11/2011
//////														if((wp.CreateWPassMode && wp.GIDwp/10 ==ieGID) ||
//////														   (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))
//////														   {dumarr[is]=ip;duminv[ip]=is;is++;
//////															break;
//////														   }

//														if(indat.trackELSET[ip]+1<indat.trackELSET[ip+1]) //EFP 4/20/2012
//														  {ipx=indat.trackELSET[ip]+1;
//														   ieGID= indat.orig_arrELSET[ipx];//EFP 8/13/2011
//														   ieGID2=indat.orig_arrELSET[ipx];
//														   if((wp.CreateWPassMode && ieGID==wp.GIDwp/10 && indat.arrELSET[ipx+1]<0) ||
//															  (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))
//															 {dumgrp[ip]= -10;dumarr[is]=ip;duminv[ip]=is;is++;
//															 }
//														  }

ieGID=ieGID2= base.arrELSET[ip];//EFP 8/13/2011
if((wp.CreateWPassMode && ieGID==wp.GIDwp/10) ||
  (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))dumgrp[ip]= -10;//EFP 8/11/2011
if((wp.CreateWPassMode && wp.GIDwp/10 ==ieGID) ||
  (!wp.CreateWPassMode && wp.prevGID[wp.PRECORD]==ieGID2))
   {dumarr[is]=ip;duminv[ip]=is;is++;
//	break;
   }

													   } // dumarr Memory could be reduced here (nelt-->is)
///////////////// start EFP 5/08/2011
if(CRBsection)dumgrp[wp.sttEles[wp.memWGa*wp.PRECORD+0]/10]=wp.sttEles[wp.memWGa*wp.PRECORD+0]-10*(wp.sttEles[wp.memWGa*wp.PRECORD+0]/10)+1;
else {for(ip=0;ip<wp.count_curr_sttEl;ip++) //TBD: Integrity test needed
dumgrp[wp.sttEles[wp.memWGa*wp.PRECORD+ip]/10]=wp.sttEles[wp.memWGa*wp.PRECORD+ip]-10*(wp.sttEles[wp.memWGa*wp.PRECORD+ip]/10)+1;
	 }
///////////////// end
					   numdum=is;
					   for(ip=0;ip<numdum;ip++){eltype=base.orig_matno[dumarr[ip]]/t7;
if(eltype!=8){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex element found in candidate weld group",L"Terminate: Mesh must have hex in WG",MB_OK);exit(0);}
											   }
					   dummap=new long[6*numdum];

honk<<numdum<<" NUMDUMmmmmmmm\n";//if(1==1)exit(0);
/////////////// Test integrity of weld group mesh  EFP 3/23/2013
if(CRBsection && numdum != wp.count_curr_sttEl*(numdum/wp.count_curr_sttEl)){extern PACKAGE void __fastcall Beep(void);
	 Application->MessageBox(L"This full length/full section WG mesh does not have same #elements per slice",L"Fatal mesh defect",MB_OK);}
///////////////
					   for(ip=0;ip<6*numdum;ip++)dummap[ip]= -1;
					   for(ip=0;ip<numdum-1;ip++){for(is=0;is<6;is++) // Assumes contiguous nodal numbering
												   {if(dummap[6*ip+is]<0) //Search forward for facet coincidence by max/min opposite corners
													  {nipismin=min(base.nop1[MXNPEL*dumarr[ip]+gdata8[4*is+0]],base.nop1[MXNPEL*dumarr[ip]+gdata8[4*is+2]]);
													   nipismax=max(base.nop1[MXNPEL*dumarr[ip]+gdata8[4*is+0]],base.nop1[MXNPEL*dumarr[ip]+gdata8[4*is+2]]);
													   isw=0;
													   for(ip1=ip+1;ip1<numdum;ip1++){for(is1=0;is1<6;is1++)
																						if(dummap[6*ip1+is1]<0)
																						  {
///////////////// start EFP 1/28/2010
if((nipismin==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+0]] || nipismin==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+1]] ||
	nipismin==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+2]] || nipismin==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+3]]) &&
   (nipismax==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+0]] || nipismax==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+1]] ||
	nipismax==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+2]] || nipismax==base.nop1[MXNPEL*dumarr[ip1]+gdata8[4*is1+3]]))
  {dummap[6*ip+is]=10*dumarr[ip1]+is1;dummap[6*ip1+is1]=10*dumarr[ip]+is;isw=1;break;}
///////////////// end 1/28/2010
																						  }
																					  if(isw)break;
																					 }
													  }
												   }
												 }
for(ir=0;ir<numdum;ir++)
  {isw=0;
   for(ic=0;ic<numdum;ic++)
	 {ip=dumarr[ic];
	  if(dumgrp[ip]>0)
		{for(is=0;is<6;is++)
		   {if(is!=dumgrp[ip]-1 && is!=opp_arr8[dumgrp[ip]-1])
			  {if(dummap[6*ic+is]> -1)
				 {ies=dummap[6*ic+is]/10;isides=dummap[6*ic+is]-10*ies;
				  if(dumgrp[ies]< -6)
					{for(iss=0;iss<6;iss++)
					   {if(iss!=isides && iss!=opp_arr8[isides])
						  {icount=0;
						   for(ik=0;ik<4;ik++)
							 {
if(base.nop1[MXNPEL*ies+gdata8[4*iss+ik]]==base.nop1[MXNPEL*ip+gdata8[4*(dumgrp[ip]-1)+0]])icount++;
if(base.nop1[MXNPEL*ies+gdata8[4*iss+ik]]==base.nop1[MXNPEL*ip+gdata8[4*(dumgrp[ip]-1)+1]])icount++;
if(base.nop1[MXNPEL*ies+gdata8[4*iss+ik]]==base.nop1[MXNPEL*ip+gdata8[4*(dumgrp[ip]-1)+2]])icount++;
if(base.nop1[MXNPEL*ies+gdata8[4*iss+ik]]==base.nop1[MXNPEL*ip+gdata8[4*(dumgrp[ip]-1)+3]])icount++;
							 }
						   if(icount==2){dumgrp[ies]=iss+1;isw=1;break;}
						  }
					   }
					}
				 }
			  }
		   }
		 dumgrp[ip]= -dumgrp[ip];
		}
	 }
   if(isw==0)break;
  }
is=0;
for(ip=0;ip<base.nelt;ip++)
  {if(dumgrp[ip]<0 && dumgrp[ip]>= -6){
									   dumgrp[is]=10*ip-dumgrp[ip]-1;is++;
									  }
  }
for(ip=is;ip<base.nelt;ip++)dumgrp[ip]= -1;
if(CRBsection){wp.count_curr_sttEl=is;
			   for(ir=0;ir<wp.count_curr_sttEl;ir++)
				 {ik=dumgrp[ir]-10*(dumgrp[ir]/10);
				  wp.sttEles[wp.memWGa*wp.PRECORD+ir]=dumgrp[ir];
for(ip=0;ip<4;ip++)wp.sttEleNodes[wp.memWGa*4*wp.PRECORD+4*ir+ip]=base.nop1[MXNPEL*(dumgrp[ir]/10) +gdata8[4*ik+ip]];
				 }
			  }
					   if(CRB_sel){  //Partial weld. Assumption: All elements in weld group have same orientation/side numbering (Partial length weld ONLY. Fix this...)					   isw=0;
					   for(ipp=0;ipp<wp.count_curr_sttEl;ipp++)
						 {icount=0;is1=ip1=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]/10; //ip1 correction EFP 1/05/2012
						  curiside=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]-10*is1;icount++;
						  if(is1==wp.stpEle[wp.PRECORD]/10){isw=1;break;}
						  else {while(dummap[6*duminv[is1]+opp_arr8[curiside]]> -1)
									 {dumrec=dummap[6*duminv[is1]+opp_arr8[curiside]];
									  if(ip1==dumrec/10)break;
									  else {is1=dumrec/10;curiside=dumrec-10*(dumrec/10);icount++;
											if(is1==wp.stpEle[wp.PRECORD]/10){isw=1;break;}
										   }
									 }
							   }
						  if(isw==1)break;
						 }
					   ir=icount;icount=0;
					   for(ipp=0;ipp<wp.count_curr_sttEl;ipp++)
						 {wp.eles[wp.memWGa*wp.PRECORD+icount]=wp.sttEles[wp.memWGa*wp.PRECORD+ipp];icount++;
						  is1=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]/10;
						  curiside=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]-10*is1;
						  if(ir>1)
							{for(in=1;in<ir;in++)
							   {if(dummap[6*duminv[is1]+opp_arr8[curiside]]> -1)
								  {dumrec=dummap[6*duminv[is1]+opp_arr8[curiside]];
								   wp.eles[wp.memWGa*wp.PRECORD+icount]=dumrec;icount++;
								   is1=dumrec/10;curiside=dumrec-10*(dumrec/10);
								  }
								else break;
							   }
							}
						 }
								  }
					   else { //Full length. Not necessary to have same element orientation here...
					   icount=0;
					   for(ipp=0;ipp<wp.count_curr_sttEl;ipp++)
						 {wp.eles[wp.memWGa*wp.PRECORD+icount]=wp.sttEles[wp.memWGa*wp.PRECORD+ipp];icount++;
						  is1=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]/10;
						  curiside=wp.sttEles[wp.memWGa*wp.PRECORD+ipp]-10*is1;ip1=is1;

						  for(in=0;in<numdum;in++)
							{
							 if(dummap[6*duminv[is1]+opp_arr8[curiside]]> -1)
							   {dumrec=dummap[6*duminv[is1]+opp_arr8[curiside]];
								if(ip1==dumrec/10)break;
								else {wp.eles[wp.memWGa*wp.PRECORD+icount]=dumrec;
								   icount++;
								   is1=dumrec/10;
								   curiside=dumrec-10*(dumrec/10);
									 }
							   }
							 else break;
							}
						 }
							}
					   delete [] dumgrp;// Coding for FEMAP users    Moved EFP 5/05/2011
					   for(in=0;in<icount;in++)dumarr[in]=wp.eles[wp.memWGa*wp.PRECORD+in];
if(wp.CreateWPassMode){dist=0.f;
					   for(in=0;in<wp.count_curr_sttEl;in++)
						 {for(ipp=0;ipp<icount/wp.count_curr_sttEl;ipp++){
wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*ipp+in]=dumarr[(icount/wp.count_curr_sttEl)*in+ipp];
ie1=dumarr[(icount/wp.count_curr_sttEl)*in+ipp]/10;eltype1=indat.matno[ie1]/t7;
ieGID1=indat.arrELSET[ie1];

//indat.matno[ie1]=indat.matno[ie1]-ieGID1+wp.PRECORD+wp.nWeldGroup+1; //Now obsolete
//base.matno[ie1]=base.matno[ie1]-ieGID1+wp.PRECORD+wp.nWeldGroup+1; //Now obsolete

if(ieGID1<=wp.nWeldGroup)wp.prevGID[wp.PRECORD]=ieGID1; // This might not allow for repeated edits..... FIX THIS
arGID[wp.PRECORD+wp.nWeldGroup+1]=1; //Corrected EFP 11/12/2010
//////for(ipx=base.trackELSET[ie1]+1;ipx<base.trackELSET[ie1+1];ipx++)
//////  {if(ieGID1==base.orig_arrELSET[ipx])
//////	 {base.arrELSET[ipx]=wp.PRECORD+wp.nWeldGroup+1;break;
//////	 }
//////  }
////base.arrELSET[base.trackELSET[ie1]+2]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
////indat.arrELSET[indat.trackELSET[ie1]+2]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
//base.arrELSET[base.trackELSET[ie1]+1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
//indat.arrELSET[indat.trackELSET[ie1]+1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
base.arrELSET[ie1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
indat.arrELSET[ie1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012


//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
xc=yc=zc=0.f;iside1=dumarr[(icount/wp.count_curr_sttEl)*in+ipp]-10*ie1;
for(is1=0;is1<4;is1++){xc=xc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+0]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+0];
					   yc=yc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+1]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+1];
					   zc=zc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+2]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+2];
					  }
xc=xc/4.f;yc=yc/4.f;zc=zc/4.f;dist=dist+sqrt(xc*xc+yc*yc+zc*zc);
//cccccccccccccccc end
																		 }
						 }
//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
wp.lend[wp.PRECORD]=dist/float(wp.count_curr_sttEl);
//cccccccccccccccc end
					  }
else {
///////////////////////// start EFP 4/17/2012
					   for(in=0;in<wp.memWGa;in++){
if(wp.eles[wp.memWGa*wp.PRECORD+in]/10 >=0){
//base.arrELSET[base.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+2]= -1;
//indat.arrELSET[indat.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+2]= -1;
base.arrELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]= -1;
indat.arrELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]= -1;
////indat.matno[wp.eles[wp.memWGa*wp.PRECORD+in]/10]=
////   t3*(indat.matno[wp.eles[wp.memWGa*wp.PRECORD+in]/10]/t3)+wp.prevGID[wp.PRECORD]; //Now obsolete
////base.matno[wp.eles[wp.memWGa*wp.PRECORD+in]/10]=
////   t3*(base.matno[wp.eles[wp.memWGa*wp.PRECORD+in]/10]/t3)+wp.prevGID[wp.PRECORD]; //Now obsolete
										   }

////base.arrELSET[base.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+2]=wp.prevGID[wp.PRECORD];
////indat.arrELSET[indat.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+2]=wp.prevGID[wp.PRECORD];
//base.arrELSET[base.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+1]=wp.prevGID[wp.PRECORD];
//indat.arrELSET[indat.trackELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]+1]=wp.prevGID[wp.PRECORD];
base.arrELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]=wp.prevGID[wp.PRECORD];
indat.arrELSET[wp.eles[wp.memWGa*wp.PRECORD+in]/10]=wp.prevGID[wp.PRECORD];
												   wp.eles[wp.memWGa*wp.PRECORD+in]= -1;
												  }
///////////////////////// end
					   dist=0.f;
					   for(in=0;in<wp.count_curr_sttEl;in++)
						 {for(ipp=0;ipp<icount/wp.count_curr_sttEl;ipp++){
wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*ipp+in]=dumarr[(icount/wp.count_curr_sttEl)*in+ipp];
ie1=dumarr[(icount/wp.count_curr_sttEl)*in+ipp]/10;eltype1=indat.matno[ie1]/t7;
//bscode1=(indat.matno[ie1]-eltype1*t7)/t5;node1=(indat.matno[ie1]-eltype1*t7-bscode1*t5)/t3;
//ieGID1=indat.matno[ie1]-eltype1*t7-bscode1*t5-node1*t3;

//indat.matno[ie1]=indat.matno[ie1]-ieGID1+glWPRECORD+wp.nWeldGroup+1; //Special purpose coding
//base.matno[ie1]=base.matno[ie1]-ieGID1+glWPRECORD+wp.nWeldGroup+1; //Special purpose coding
////if(ieGID1<=wp.nWeldGroup)wp.prevGID[glWPRECORD]=ieGID1; // This might not allow for repeated edits..... FIX THIS
////arGID[glWPRECORD+wp.nWeldGroup+1]=1; //Corrected EFP 11/12/2010

/////////////// start EFP 1/29/2011 (Coding bypassed as simplification EFP 4/18/2012)
//if(wp.nWeldPass>1)
//  {for(inw=0;inw<wp.nWeldPass;inw++)
//	 {if(inw!=wp.PRECORD && wp.n_curr_sttEl[inw])
//						 {ie2=wp.eles[wp.memWGa*inw+0]/10;eltype2=indat.matno[ie2]/t7;bscode2=(indat.matno[ie2]-eltype2*t7)/t5;
//						  node2=(indat.matno[ie2]-eltype2*t7-bscode2*t5)/t3;ieGID2=indat.matno[ie2]-eltype2*t7-bscode2*t5-node2*t3;
//						  if(ieGID2==ieGID1){isw3=icount3=0;for(ik=0;ik<base.nelt;ik++){if(wp.eles[wp.memWGa*inw+ik]>=0)icount3++;else break;}
//											 for(in3=0;in3<wp.n_curr_sttEl[inw];in3++)
//											   {for(ipp3=0;ipp3<icount3/wp.n_curr_sttEl[inw];ipp3++)
//												  {ie3=wp.eles[wp.memWGa*inw+wp.n_curr_sttEl[inw]*ipp3+in3]/10;
//												   if(ie3==ie1){
//icount3b=0;
//for(ipp3a=0;ipp3a<icount3/wp.n_curr_sttEl[inw];ipp3a++)
//  {icount3a=0;
//   for(in3a=0;in3a<wp.n_curr_sttEl[inw];in3a++)
//	 {if(in3a!=in3){wp.eles[wp.memWGa*inw+(wp.n_curr_sttEl[inw]-1)*ipp3a+icount3a]=
//					wp.eles[wp.memWGa*inw+wp.n_curr_sttEl[inw]*ipp3a+in3a];
//					icount3a++;icount3b++;
//				   }
//	 }
//  }
//for(in3a=icount3b;in3a<base.nelt;in3a++)wp.eles[wp.memWGa*inw+in3a]= -1;
//icount3a=0;
//for(in3a=0;in3a<wp.n_curr_sttEl[inw];in3a++)if(in3a!=in3)
//											  {wp.sttEles[wp.memWGa*inw+icount3a]=wp.sttEles[wp.memWGa*inw+in3a];
//											   icount3a++;
//											  }
//wp.n_curr_sttEl[inw]=wp.n_curr_sttEl[inw]-1;isw3=1;break;
//															   }
//												  }
//												if(isw3)break;
//											   }
//											 if(!wp.n_curr_sttEl[inw]){extern PACKAGE void __fastcall Beep(void); // EFP 2/02/2011
//																	   Application->MessageBox(L"This action eliminates a weld pass.",L"Notice",MB_OK);
//																	  }
//											}
//						 }
//	 }
//  }
///////////////// end 1/29/2011

//indat.matno[ie1]=indat.matno[ie1]-ieGID1+wp.PRECORD+wp.nWeldGroup+1; //Now obsolete
//base.matno[ie1]=base.matno[ie1]-ieGID1+wp.PRECORD+wp.nWeldGroup+1; //Now obsolete

//////////// EFP 2/09/2012
//for(ipx=base.trackELSET[ie1]+1;ipx<base.trackELSET[ie1+1];ipx++) //EFP did not attend to Edit yet
//  {if(ieGID1==base.orig_arrELSET[ipx])
//	 {base.arrELSET[ipx]=wp.PRECORD+wp.nWeldGroup+1;break;
//	 }
//  }

////base.arrELSET[base.trackELSET[ie1]+2]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
////indat.arrELSET[indat.trackELSET[ie1]+2]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
//base.arrELSET[base.trackELSET[ie1]+1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
//indat.arrELSET[indat.trackELSET[ie1]+1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
base.arrELSET[ie1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012
indat.arrELSET[ie1]=wp.PRECORD+wp.nWeldGroup+1; //Presumes VFTsolid (only one GID per el) EFP 2/18/2012

//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
xc=yc=zc=0.f;iside1=dumarr[(icount/wp.count_curr_sttEl)*in+ipp]-10*ie1;
for(is1=0;is1<4;is1++){xc=xc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+0]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+0];
					   yc=yc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+1]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+1];
					   zc=zc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+2]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+2];
					  }
xc=xc/4.f;yc=yc/4.f;zc=zc/4.f;dist=dist+sqrt(xc*xc+yc*yc+zc*zc);
//cccccccccccccccc end
//////////
//if(ieGID1==base.orig_arrELSET[base.trackELSET[ie1]+2])
//  base.arrELSET[base.trackELSET[ie1]+2]=wp.PRECORD+wp.nWeldGroup+1; //Edit??? VFTsolid only EFP 3/05/2012
																		 }
						 }
//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
wp.lend[wp.PRECORD]=dist/float(wp.count_curr_sttEl);
//cccccccccccccccc end
	 }
					   wp.n_curr_sttEl[wp.PRECORD]=wp.count_curr_sttEl;
					   delete [] dummap;delete [] dumarr;delete [] duminv;

//vvvvvvvvvvvvvvvvv start Coding for circEles NODES  EFP 10/14/2010
// Ensure that icount is unchanged from above
if(circFlag || girthFlag)
  {xave=yave=zave=0.f;
   for(in=0;in<wp.count_curr_sttEl;in++)
	 {is1=wp.eles[wp.memWGa*wp.PRECORD+in]/10;curiside=wp.eles[wp.memWGa*wp.PRECORD+in]-10*is1; //Assume 8n hex only
	  node=4;xnor1=ynor1=znor1=0.f;
	  for(ip=0;ip<node;ip++)
		{xnor1=xnor1+base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]  ];
		 ynor1=ynor1+base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+1];
		 znor1=znor1+base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+2];
		}
	  xave=xave+xnor1/float(node);yave=yave+ynor1/float(node);zave=zave+znor1/float(node);
	 }
   xave=xave/float(wp.count_curr_sttEl);yave=yave/float(wp.count_curr_sttEl);zave=zave/float(wp.count_curr_sttEl);
   node=4;rave=1.e20f;  //Assume 8n hex only
   for(in=0;in<wp.count_curr_sttEl;in++)
	 {is1=wp.eles[wp.memWGa*wp.PRECORD+in]/10;curiside=wp.eles[wp.memWGa*wp.PRECORD+in]-10*is1; //Assume 8n hex only
	  for(ip=0;ip<node;ip++)
		{rv=(xave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]  ])*(xave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]  ])+
			(yave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+1])*(yave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+1])+
			(zave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+2])*(zave-base.c1[NDF*base.nop1[MXNPEL*is1+gdata8[4*curiside+ip]]+2]);//Correction xave->zave EFP 1/07/2011
		 if(rave>rv){rave=rv;ip1=in;ippp=ip;}
		}
	 }
   is1=wp.eles[wp.memWGa*wp.PRECORD+ip1]/10;curiside=wp.eles[wp.memWGa*wp.PRECORD+ip1]-10*is1; //Assume 8n hex only
   wp.circEles[3*wp.PRECORD+0]=base.nop1[MXNPEL*is1+gdata8[4*curiside+ippp]];
   if(CRB_sel) //Partial
	 {
	  ip=wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*((icount/wp.count_curr_sttEl)/2)+ip1]/10;
	  wp.circEles[3*wp.PRECORD+1]=base.nop1[MXNPEL*ip +gdata8[4*curiside+ippp]];
	  ip=wp.eles[wp.memWGa*wp.PRECORD+icount-wp.count_curr_sttEl +ip1]/10;
	  wp.circEles[3*wp.PRECORD+2]=base.nop1[MXNPEL*ip +gdata8[4*opp_arr8[curiside]+ippp]]; //ippp not entirely accurate here
	 }
   else // Full
	 {
	  ip=wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*((icount/wp.count_curr_sttEl)/3)+ip1]/10;
	  wp.circEles[3*wp.PRECORD+1]=base.nop1[MXNPEL*ip +gdata8[4*curiside+ippp]];
	  ip=wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*(2*(icount/wp.count_curr_sttEl)/3)+ip1]/10;
	  wp.circEles[3*wp.PRECORD+2]=base.nop1[MXNPEL*ip +gdata8[4*curiside+ippp]];
	 }
  }
/////////////////////// Dotted outline of weld pass elements (always 8n hex in current version)
		  in=wp.PRECORD%9;
		  if     (in==0){Canvas->Brush->Color=clYellow;Canvas->Pen->Color=clYellow;}
		  else if(in==1){Canvas->Brush->Color=clOlive;Canvas->Pen->Color=clOlive;} // Coding for quad color contrast
		  else if(in==2){Canvas->Brush->Color=clMaroon;Canvas->Pen->Color=clMaroon;}
		  else if(in==3){Canvas->Brush->Color=clRed;Canvas->Pen->Color=clRed;}
		  else if(in==4){Canvas->Brush->Color=clLtGray;Canvas->Pen->Color=clLtGray;}
		  else if(in==5){Canvas->Brush->Color=clBlue;Canvas->Pen->Color=clBlue;}
		  else if(in==6){Canvas->Brush->Color=clBlue;Canvas->Pen->Color=clBlue;}
		  else if(in==7){Canvas->Brush->Color=clLime;Canvas->Pen->Color=clLime;}
		  else if(in==8){Canvas->Brush->Color=clNavy;Canvas->Pen->Color=clNavy;}
Canvas->Brush->Color=wp.WeldColor[wp.PRECORD];Canvas->Pen->Color=wp.WeldColor[wp.PRECORD];
					   Canvas->Pen->Width=1;
					   for(in=0;in<wp.count_curr_sttEl;in++){
															 for(ipp=0;ipp<icount/wp.count_curr_sttEl;ipp++){
ie1=wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*ipp+in]/10;
eltype1=indat.matno[ie1]/t7;
is1=0;
if(eltype1==8)for(is1=0;is1<6;is1++) //Correction EFP 1/20/2011
			   {Canvas->MoveTo(int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*is1+0]]  ]+0.5f),
							   ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*is1+0]]+1]+0.5f));
				for(ip1=1;ip1<4;ip1++)Canvas->LineTo(int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*is1+ip1]]  ]+0.5f),
													 ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*is1+ip1]]+1]+0.5f));
			   }
else {wp.avis=10*(wp.avis/10)+1;
	  honk<<(ie1+1)<<" miscreantNonHex "<<eltype1<<"\n"; // Modification EFP 5/04/2011
//	  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex-element found in this weld pass.",L"Warning",MB_OK);
	 }
																											}
															}
/////////////////////// EFP 5/04/2011
//if(wp.avis){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex-element found in this weld pass.",L"Warning",MB_OK);}
if(wp.avis-10*(wp.avis/10)){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Non-hex-element found in this weld pass.",L"Warning",MB_OK);}
///////////////////////
// Corrected version of above, allowing for curved WP  EFP 3/22/2012
				 if(CCB!=1){  //Reverse directions
////TBD: The following discriminant logic might just be for circular welds(Full length==Circular???)
//// i.e. CRB_sel is the wrong parameter to use, since crack shape is the determinant???
//   if(CRB_sel) //Partial length
//	 {ippp=wp.circEles[3*wp.PRECORD+0];
//	  wp.circEles[3*wp.PRECORD+0]=wp.circEles[3*wp.PRECORD+2];
//	  wp.circEles[3*wp.PRECORD+2]=ippp;
//	 }
//   else // Full length
//	 {ippp=wp.circEles[3*wp.PRECORD+1];
//	  wp.circEles[3*wp.PRECORD+1]=wp.circEles[3*wp.PRECORD+2];
//	  wp.circEles[3*wp.PRECORD+2]=ippp;
//	 }
// EFP thinks that the coding should be as follows (3/22/2012) TEST THIS!!!
// CRB_ckShape=CreateLinWeldPass->CheckShape;
// 0=noncircular,1=fullcircle,2=partialcircle,3=fullgirth,4=partialgirth
   if(CRB_ckShape==1 || CRB_ckShape==3) //fullcircle or fullgirth
	 {ippp=wp.circEles[3*wp.PRECORD+1];
	  wp.circEles[3*wp.PRECORD+1]=wp.circEles[3*wp.PRECORD+2];
	  wp.circEles[3*wp.PRECORD+2]=ippp;
	 }
   else
	 {ippp=wp.circEles[3*wp.PRECORD+0];
	  wp.circEles[3*wp.PRECORD+0]=wp.circEles[3*wp.PRECORD+2];
	  wp.circEles[3*wp.PRECORD+2]=ippp;
	 }
for(in=0;in<wp.memWGa;in++)if(wp.eles[wp.memWGa*wp.PRECORD+in]<0)break;
icount=in;dumarr=new long[base.nelt];
for(in=0;in<icount;in++)dumarr[in]=wp.eles[wp.memWGa*wp.PRECORD+in];
for(ipp=0;ipp<icount/wp.count_curr_sttEl;ipp++)
  {for(in=0;in<wp.count_curr_sttEl;in++){ //Correction for missing pair of brackets  EFP 3/2012
ip1=    dumarr[wp.count_curr_sttEl*((icount/wp.count_curr_sttEl)-ipp-1)+in]-
	10*(dumarr[wp.count_curr_sttEl*((icount/wp.count_curr_sttEl)-ipp-1)+in]/10);
wp.eles[wp.memWGa*wp.PRECORD+wp.count_curr_sttEl*ipp+in]=
dumarr[wp.count_curr_sttEl*((icount/wp.count_curr_sttEl)-ipp-1)+in]-ip1+opp_arr8[ip1];  //Correction  EFP 3/22/2012
										}
  }
delete [] dumarr;
for(in=0;in<wp.count_curr_sttEl;in++)wp.sttEles[wp.memWGa*wp.PRECORD+in]=wp.eles[wp.memWGa*wp.PRECORD+in];
						   }
xave=yave=zave=0.f;
for(ip=0;ip<wp.count_curr_sttEl;ip++)
  {for(in=0;in<8;in++){xave=xave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*wp.PRECORD+ip]/10)+in]+0];
					   yave=yave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*wp.PRECORD+ip]/10)+in]+1];
					   zave=zave+indat.c1[NDF*indat.nop1[MXNPEL*(wp.sttEles[wp.memWGa*wp.PRECORD+ip]/10)+in]+2];
					  }
  }
xave=xave/float(8*wp.count_curr_sttEl);yave=yave/float(8*wp.count_curr_sttEl);zave=zave/float(8*wp.count_curr_sttEl);
ie=wp.sttEles[wp.memWGa*wp.PRECORD+0]/10; // Use first start element face as normal
iside=wp.sttEles[wp.memWGa*wp.PRECORD+0]-10*ie;
STFISO8_ncalc(ie,iside,HN,base.nop1,base.c1); //First calc to establish global arrows EFP 3/23/2012
wp.arrows[NDF*3*wp.PRECORD+NDF*0+0]= -HN[0];wp.arrows[NDF*3*wp.PRECORD+NDF*0+1]= -HN[1];wp.arrows[NDF*3*wp.PRECORD+NDF*0+2]= -HN[2];

//honk<<wp.PRECORD<<" "<<ie<<" "<<iside<<" "<<wp.arrows[NDF*3*wp.PRECORD+NDF*0+0]<<" "<<wp.arrows[NDF*3*wp.PRECORD+NDF*0+1]<<" "<<wp.arrows[NDF*3*wp.PRECORD+NDF*0+2]<<" wpARROW_weldDir\n";
STFISO8_ncalc(ie,iside,HN,indat.nop1,indat.c1); //second calc to establish screen frame directions EFP 3/23/2012
Canvas->Pen->Width=3;
Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
Canvas->Ellipse(int(xave)-5,ClientHeight-(int(yave)-5),int(xave)+5,ClientHeight-(int(yave)+5));
Canvas->Brush->Color=clBlue;Canvas->Pen->Color=clBlue;
Canvas->MoveTo(int(xave),ClientHeight-int(yave));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1]))); //GIANTS
Canvas->MoveTo(int(xave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[0]+HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1])));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1])));
Canvas->LineTo(int(xave+25.f*(HN[0]-HN[1]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[0]),ClientHeight-(int(yave+25.f*(HN[1]-HN[0]))+(min(ClientWidth,ClientHeight)/200)*int(-50.f*HN[1])));

Screen->Cursor=Save_Cursor;

					  }
					else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not hex-element or not in current weld group or no start elements yet.",L"Ignore",MB_OK);}
				   }

				 else if(CRB==3)  // Plate1 normal to element/side
				   {
///////////// 1st start Coding to ensure that normal is on plate EFP 1/08/2011
					eltype=base.matno[ie]/t7;bscode=(base.matno[ie]-eltype*t7)/t5;node=(base.matno[ie]-eltype*t7-bscode*t5)/t3;
//					ieGID=base.matno[ie]-eltype*t7-bscode*t5-node*t3; //Use ieGID from above
					if(ieGID==0){

///////////// start normal & complementary coding EFP 6/26/2011
if((wp.avis-100*(wp.avis/100))/10){isw=1;eltype5=indat.matno[ie]/t7;bscode5=(indat.matno[ie]-eltype5*t7)/t5;
								   if(bscode5>=32){if(iside!=5){isides=5;isw=0;}
												   else bscode5=bscode5-32;
												  }
								   if(bscode5>=16 && isw){if(iside!=4){isides=4;isw=0;}
														  else bscode5=bscode5-16;
														 }
								   if(bscode5>=8 && isw){if(iside!=3){isides=3;isw=0;}
														 else bscode5=bscode5-8;
														}
								   if(bscode5>=4 && isw){if(iside!=2){isides=2;isw=0;}
														 else bscode5=bscode5-4;
														}
								   if(bscode5>=2 && isw){if(iside!=1){isides=1;isw=0;}
														 else bscode5=bscode5-2;
														}
								   if(bscode5>=1 && isw){if(iside!=0)isides=0;
														} //TBD: Add warning if isides not found
								  }
else isides=iside;
///////////// 1st end
					Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;xave=yave=0.f;  // Assumes 8/20n elements
					for(ip=0;ip<4;ip++){xave=xave+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]]  ];yave=yave+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]]+1];}
					xave=xave/float(4);yave=yave/float(4);
					Canvas->Ellipse(int(xave)-2,ClientHeight-(int(yave)-2),int(xave)+2,ClientHeight-(int(yave)+2));
STFISO8_ncalc(ie,isides,HN,indat.nop1,indat.c1); //first calc to establish screen frame directions EFP 3/23/2012
					Canvas->Pen->Color=clGreen;Canvas->Pen->Width=3;Canvas->MoveTo(int(xave),ClientHeight-int(yave));
					Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])));
Canvas->Pen->Width=2;
Canvas->MoveTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[0],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[1]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[1],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])-25.f*HN[0]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])-25.f*HN[1],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[0]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[0],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[1]));
					for(ip=0;ip<4;ip++){wp.snorm1[4*wp.PRECORD+ip]=indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]];
									   }
///////////
dist=1.e+24;
for(ip=0;ip<4;ip++) //Enforce CTSP normals convention (1st node at fusion line at start elements)  EFP 10/30/2012
  {for(is=0;is<wp.count_curr_sttEl;is++)
	 {ie1=wp.sttEles[wp.memWGa*wp.PRECORD+is]/10;iside1=wp.sttEles[wp.memWGa*wp.PRECORD+is]-10*ie1;
	  for(in=0;in<4;in++){
if(dist >(base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+0])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+0])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+1])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+1])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+2])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+2]))
  {dist =(base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+0])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+0])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+1])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+1])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+2])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm1[4*wp.PRECORD+ip]+2]);
   ip1=ip;
  }
						 }
	 }
  }
ip1--;if(ip1<0)ip1=3;
for(ip=0;ip<4;ip++){wp.snorm1[4*wp.PRECORD+ip]=indat.nop1[MXNPEL*ie+gdata8[4*isides+ip1]];
					ip1++;if(ip1>3)ip1=0;
				   }
STFISO8_ncalc(ie,iside,HN,base.nop1,base.c1); //second calc to establish global arrows EFP 3/23/2012
					wp.arrows[NDF*3*wp.PRECORD+NDF*1+0]=HN[0];wp.arrows[NDF*3*wp.PRECORD+NDF*1+1]=HN[1];wp.arrows[NDF*3*wp.PRECORD+NDF*1+2]=HN[2];
					CreateLinWeldPass->CheckNorm1=wp.snorm1[4*wp.PRECORD+0]; //EFP 5/31/2011
								}
					else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Normal must be on plate material",L"Failure",MB_OK);}
				   }
				 else if(CRB==4)  // Plate2 normal to element/side
				   {
					eltype=base.matno[ie]/t7;bscode=(base.matno[ie]-eltype*t7)/t5;node=(base.matno[ie]-eltype*t7-bscode*t5)/t3;
					if(ieGID==0){

///////////// start normal & complementary coding EFP 6/26/2011
if((wp.avis-100*(wp.avis/100))/10){isw=1;eltype5=indat.matno[ie]/t7;bscode5=(indat.matno[ie]-eltype5*t7)/t5;
								   if(bscode5>=32){if(iside!=5){isides=5;isw=0;}
												   else bscode5=bscode5-32;
												  }
								   if(bscode5>=16 && isw){if(iside!=4){isides=4;isw=0;}
														  else bscode5=bscode5-16;
														 }
								   if(bscode5>=8 && isw){if(iside!=3){isides=3;isw=0;}
														 else bscode5=bscode5-8;
														}
								   if(bscode5>=4 && isw){if(iside!=2){isides=2;isw=0;}
														 else bscode5=bscode5-4;
														}
								   if(bscode5>=2 && isw){if(iside!=1){isides=1;isw=0;}
														 else bscode5=bscode5-2;
														}
								   if(bscode5>=1 && isw){if(iside!=0)isides=0;
														} //TBD: Add warning if isides not found
								  }
else isides=iside;
					Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;xave=yave=0.f;    // Assumes 8/20n elements
					for(ip=0;ip<4;ip++){xave=xave+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]]  ];yave=yave+indat.c1[NDF*indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]]+1];}
					xave=xave/float(4);yave=yave/float(4);
					Canvas->Ellipse(int(xave)-2,ClientHeight-(int(yave)-2),int(xave)+2,ClientHeight-(int(yave)+2));
STFISO8_ncalc(ie,isides,HN,indat.nop1,indat.c1); //first calc to establish screen frame directions EFP 3/23/2012
					Canvas->Pen->Color=clLime;Canvas->Pen->Width=3;Canvas->MoveTo(int(xave),ClientHeight-int(yave));
					Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0]),ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])));
Canvas->Pen->Width=2;
Canvas->MoveTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[0],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[1]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[1],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])-25.f*HN[0]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])-25.f*HN[1],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[0]));
Canvas->LineTo(int(xave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[0])+25.f*HN[0],ClientHeight-(int(yave)+(min(ClientWidth,ClientHeight)/200)*int(100.f*HN[1])+25.f*HN[1]));
					for(ip=0;ip<4;ip++){wp.snorm2[4*wp.PRECORD+ip]=indat.nop1[MXNPEL*ie+gdata8[4*isides+ip]];
									   }
dist=1.e+24f;
for(ip=0;ip<4;ip++) //Enforce CTSP normals convention (1st node at fusion line at start elements)  EFP 10/30/2012
  {for(is=0;is<wp.count_curr_sttEl;is++)
	 {ie1=wp.sttEles[wp.memWGa*wp.PRECORD+is]/10;iside1=wp.sttEles[wp.memWGa*wp.PRECORD+is]-10*ie1;
	  for(in=0;in<4;in++){
if(dist >(base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+0])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+0])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+1])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+1])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+2])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+2]))
  {dist =(base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+0])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+0]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+0])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+1])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+1]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+1])+
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+2])*
		 (base.c1[NDF*indat.nop1[MXNPEL*ie1+gdata8[4*iside1+in]]+2]-base.c1[NDF*wp.snorm2[4*wp.PRECORD+ip]+2]);
   ip1=ip;
  }
						 }
	 }
  }
ip1--;if(ip1<0)ip1=3;
for(ip=0;ip<4;ip++){wp.snorm2[4*wp.PRECORD+ip]=indat.nop1[MXNPEL*ie+gdata8[4*isides+ip1]];
					ip1++;if(ip1>3)ip1=0;
				   }
STFISO8_ncalc(ie,iside,HN,base.nop1,base.c1); //second calc to establish global arrows EFP 3/23/2012
					wp.arrows[NDF*3*wp.PRECORD+NDF*2+0]=HN[0];wp.arrows[NDF*3*wp.PRECORD+NDF*2+1]=HN[1];wp.arrows[NDF*3*wp.PRECORD+NDF*2+2]=HN[2];
					CreateLinWeldPass->CheckNorm2=wp.snorm2[4*wp.PRECORD+0]; //EFP 5/31/2011
								}
					else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Normal must be on plate material",L"Failure",MB_OK);}
				   }
				 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported VFT WeldPass radioButton option.",L"Terminate",MB_OK);exit(0);}
				}
			 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must select Weld Pass Location tab",L"Failure",MB_OK);}
				}
	   else {
			extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get CreateWeld dialog box.",L"Failure",MB_OK);
		   }
/////////////// New VFT coding //////////////
/////////////////////////////
	  }






/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
	else if(FD_LButtonstatus==30) // Add Ux=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=1;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==31) // Uy=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=2;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==32) // Uz=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=3;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==33) // Ux=Uy=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=4;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==34) // Ux=Uz=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=5;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==35) // Uy=Uz=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=6;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==36) // Ux=Uy=Uz=0  EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Overwriting existing BC node",L"Notice",MB_OK);}
	   base.nofix[2*NodeNum+1]=7;FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==37) // Remove BC EFP 4/10/2012
	  {NodeNum=FDnode_interrog1(X,Y,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   Canvas->Brush->Color=clRed;Canvas->Pen->Color=clRed;
	   Canvas->Ellipse(int(indat.c1[NDF*NodeNum])-4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])-4),int(indat.c1[NDF*NodeNum])+4,ClientHeight-(int(indat.c1[NDF*NodeNum+1])+4));
	   if(!base.nofix[2*NodeNum+1]){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Not a BC node",L"Failure",MB_OK);}
	   else base.nofix[2*NodeNum+1]=0;
	   FD_LButtonstatus=FD_LBrec;
	  }
	else if(FD_LButtonstatus==23) // Element facet area EFP 5/01/2013
	  {FDelem_interrog1(X,Y,&ie,&iside,&RN1,&RN2,&RN3,indat.nop1,indat.matno,indat.c1,nFacets,arbFacet);
	   if(ie>=0 && ie<base.nelt){Canvas->Pen->Width=3;Canvas->Brush->Color=clWhite;Canvas->Pen->Color=clWhite;
								 Canvas->Ellipse(X-5,Y-5,X+5,Y+5);rave=ElemFacetArea(ie,iside,base.nop1,base.matno,base.c1);
								 Application->MessageBox(FloatToStr(rave).w_str(),IntToStr(__int64(base.el_map[ie]+1)).w_str(),MB_OK);
								}
	   Screen->Cursor=crSize;FD_LButtonstatus=FD_LBrec;
	  }
   }
//
//
//
//
//
 else if(Button==mbRight)
   {
	if(FD_LButtonstatus==16)
	  {if(polypts){Canvas->LineTo(polycord[0],ClientHeight-polycord[1]);
				   if(polypts<3){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Insufficient polygon points.",L"Abandon",MB_OK);
								 FD_LButtonstatus=0;polypts=0;return;
								}
// TBD: Add a polygon integrity test here
	   signp=signm=0;
	   for(ip=0;ip<polypts;ip++)
		 {ipp=ip+1;if(ipp>=polypts)ipp=0;ippp=ipp+1;if(ippp>=polypts)ippp=0;
		  dx1=float(polycord[2*ipp]-polycord[2*ip]);dy1=float(polycord[2*ipp+1]-polycord[2*ip+1]);
		  norm=sqrt(dx1*dx1+dy1*dy1);if(norm<0.001f){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Indistinct polygon points.",L"Try again",MB_OK);
													FD_LButtonstatus=0;polypts=0;return;
												   }
		  dx1=dx1/norm;dy1=dy1/norm;
		  dx3=float(polycord[2*ippp]-polycord[2*ip]);dy3=float(polycord[2*ippp+1]-polycord[2*ip+1]);
		  norm=sqrt(dx3*dx3+dy3*dy3);if(norm<0.001f){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Indistinct polygon points.",L"Try again",MB_OK);
													FD_LButtonstatus=0;polypts=0;return;
												   }
		  dx3=dx3/norm;dy3=dy3/norm;if(dx1*dy3-dx3*dy1 >=0.)signp++;else signm++;
		 }
	   if(signp==signm){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Possibly invalid criss-cross polygon.",L"Warning",MB_OK);
					   }
												maskelemPolyhea(0);
										 FD_LButtonstatus=11;stateVFT=2;
////////////////// Cursors EFP 1/21/2011
Screen->Cursor=crSizeAll;
//////////////////
				   polypts=0;
				  }
	  }
	else if(FD_LButtonstatus==11){stateVFT=3;Xpan0=X;Ypan0=Y;panMouseM=0;
								  iplotflag=2;iCullyesno=1;Timer1->Interval=250;Timer1->Enabled=true;
								 }
/*
	else if(FD_LButtonstatus==22){if(QElem){delete QElem;QElem=NULL;}
								  FD_LButtonstatus=11;stateVFT=2;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
								 }
*/
   }
// else //Middle button
//   {if(FD_LButtonstatus==11){zoomRect.left=zoomRect.right=X;zoomRect.top=zoomRect.bottom=Y;
//                             stateVFT=2;iplotflag=2;Timer5->Interval=250;Timer5->Enabled=true;
//                            }
//   }

// delete sb;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
// "Highlight" points/lines are plotted directly to Form1->Canvas, not buffer tBitmap->Canvas
{panMouseM=1;
 if(FD_LButtonstatus==1)
	 {if(zoomDrag){zoomRect.right=X;zoomRect.bottom=Y;
				   Canvas->MoveTo(zoomRect.left,zoomRect.top);Canvas->LineTo(zoomRect.left,(zoomRect.bottom+3*zoomRect.top)/4);
				   Canvas->MoveTo(zoomRect.left,(3*zoomRect.bottom+zoomRect.top)/4);Canvas->LineTo(zoomRect.left,zoomRect.bottom);
				   Canvas->LineTo((3*zoomRect.left+zoomRect.right)/4,zoomRect.bottom);
				   Canvas->MoveTo((zoomRect.left+3*zoomRect.right)/4,zoomRect.bottom);Canvas->LineTo(zoomRect.right,zoomRect.bottom);
				   Canvas->LineTo(zoomRect.right,(3*zoomRect.bottom+zoomRect.top)/4);
				   Canvas->MoveTo(zoomRect.right,(zoomRect.bottom+3*zoomRect.top)/4);Canvas->LineTo(zoomRect.right,zoomRect.top);
				   Canvas->LineTo((zoomRect.left+3*zoomRect.right)/4,zoomRect.top);
				   Canvas->MoveTo((3*zoomRect.left+zoomRect.right)/4,zoomRect.top);Canvas->LineTo(zoomRect.left,zoomRect.top);
				  }
	 }
 else if(FD_LButtonstatus==11){if(Shift.Contains(ssLeft)){zoomRect.right=X;zoomRect.bottom=Y;}
							   if(Shift.Contains(ssRight)){Xpan1=X;Ypan1=Y;}
							  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{//enum TMouseButton { mbLeft, mbRight, mbMiddle }; // Draw directly on Canvas
 float rad=0.f;int TB1H=0,P1W=0;
 if(Button==TMouseButton::mbLeft)
   {if(FD_LButtonstatus==1){if(zoomDrag){zoomDrag=false;zoomRect.right=X;zoomRect.bottom=Y;FD_LButtonstatus=0;
										 CmZoomIn2(indat.npoin,indat.c1,trans_op,prod_op,trans_zoom,prod_zoom);
										 FD_LButtonstatus=FD_LBrec;
										 stateVFT=stateVFTrec;
										}
						   }
	else if(FD_LButtonstatus==16)
	  {if(polypts){if(polypts<MAX_POLYPOINTS)
					 {
					  rad=sqrt(float(X-polycord[2*polypts-2])*float(X-polycord[2*polypts-2])+float(ClientHeight-Y-polycord[2*polypts-1])*float(ClientHeight-Y-polycord[2*polypts-1]));
					  if(rad>0.00001f){polycord[2*polypts]=X;polycord[2*polypts+1]=ClientHeight-Y;polypts++;Canvas->LineTo(X,Y);}
					  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must left-button-click & drag.",L"Halt",MB_OK);}
					 }
				   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must close polygon now-->right-button.",L"Halt",MB_OK);}
				  }
	  }

/////////////////////////// Reuse zoomRect.etc
////	else if(FD_LButtonstatus==5)
	else if(FD_LButtonstatus==8)// Changed from 5->8 to match VFTgen
	  {zoomRect.right=X;zoomRect.bottom=Y;FD_LButtonstatus=0;
	   rangle[0]=180.f*float(-zoomRect.top+zoomRect.bottom)/float(ClientHeight- TB1H);rangle0[0]=rangle0[0]+rangle[0];
	   rangle[1]=180.f*float(zoomRect.right-zoomRect.left)/float(ClientWidth- P1W);rangle0[1]=rangle0[1]+rangle[1];
	   rangle[2]=0.f;rangle0[2]=rangle0[2]+rangle[2];
	   FDrotate(0,indat.npoin,indat.c1,rangle);
	   FDrotate_axisplot(0,rangle); //EFP 9/17/2010
	   iCullyesno=0;Invalidate();
	  }
	else if(FD_LButtonstatus==11){

//honk<<X<<" "<<Y<<" FMU mbLeft 11\n";
								  Timer1->Enabled=false;
								 }
	else if(FD_LButtonstatus==13){
								  Timer1->Enabled=false;
										 FD_LButtonstatus=FD_LBrec;
										 stateVFT=stateVFTrec;
								 }
   }
 else if(Button==TMouseButton::mbRight)
   {if(FD_LButtonstatus==11){
							 stateVFT=2;
							 Timer1->Enabled=false;
							}
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{FD_LBrec=FD_LButtonstatus;stateVFTrec=stateVFT;
 if(WheelDelta>0){zoomRect.left=int(float(ClientWidth)*0.05f);zoomRect.top=int(float(ClientHeight)*0.05f);
				  zoomRect.right=int(float(ClientWidth)*0.95f);zoomRect.bottom=int(float(ClientHeight)*0.95f);
				  zoomDrag=false;FD_LButtonstatus=0;
				  CmZoomIn2(indat.npoin,indat.c1,trans_op,prod_op,trans_zoom,prod_zoom);
				 }
 else if(WheelDelta<0){zoomRect.left= -int(float(ClientWidth)*0.05f);zoomRect.top= -int(float(ClientHeight)*0.05f);
					   zoomRect.right=int(float(ClientWidth)*1.05f);zoomRect.bottom=int(float(ClientHeight)*1.05f);
					   zoomDrag=false;FD_LButtonstatus=0;
					   CmZoomIn2(indat.npoin,indat.c1,trans_op,prod_op,trans_zoom,prod_zoom);
					  }
 FD_LButtonstatus=FD_LBrec;stateVFT=stateVFTrec;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{long i=0,iq=0,ic=0,ie=0,iWP=0,iSeq=0,icount3=0,iup=0,icdiv=0//, *arbFace1=NULL, *rbTem1=NULL
;
 int TB1H=0,P1W=0//, *edgFace1=NULL
 ;
 bool dir=true;
 int gdedges8[12*2]={0,1,//First 9 edges are continuous
					 1,2,
					 2,3,
					 3,0,
					 0,4,
					 4,5,
					 5,6,
					 6,7,
					 7,4,
					 1,5,
					 2,6,
					 3,7};
 MEMORYSTATUS ms;ms.dwLength=sizeof(MEMORYSTATUS);

// honk<< Timer1->Interval <<" Timer1intv "<<FD_LButtonstatus<<" "<<stateVFT<<"\n";
 if(FD_LButtonstatus==11 && stateVFT==2){

// honk<<zoomRect.left<<" "<<zoomRect.right<<" ZZZ "<<zoomRect.top<<" "<<zoomRect.bottom<<"\n";
   if(zoomRect.left-zoomRect.right && zoomRect.top-zoomRect.bottom){
	   rangle[0]=180.f*float(-zoomRect.top+zoomRect.bottom)/float(ClientHeight- TB1H);rangle0[0]=rangle0[0]+rangle[0];
	   rangle[1]=180.f*float(zoomRect.right-zoomRect.left)/float(ClientWidth- P1W);rangle0[1]=rangle0[1]+rangle[1];
	   rangle[2]=0.f;rangle0[2]=rangle0[2]+rangle[2];
rangle[0]=rangle[0]/glGdiff;
rangle[1]=rangle[1]/glGdiff;
rangle[2]=rangle[2]/glGdiff;

//honk<<rangle[0]<<" "<<rangle[1]<<" "<<rangle[2]<<" "<<glGdiff<<" Timer1Timer 11-2\n";
	   FDrotate(0,indat.npoin,indat.c1,rangle);
	   FDrotate_axisplot(0,rangle); //EFP 9/17/2010
	   zoomRect.left=zoomRect.right;zoomRect.top=zoomRect.bottom;iCullyesno=0;Invalidate();
																   }
										}
 else if(FD_LButtonstatus==11 && stateVFT==3){
   if(panMouseM){for(i=0;i<indat.npoin;i++){indat.c1[NDF*i  ]=indat.c1[NDF*i  ]+float(Xpan1-Xpan0);
											indat.c1[NDF*i+1]=indat.c1[NDF*i+1]-float(Ypan1-Ypan0);
										   }
				 gl_indatMedian[0]=gl_indatMedian[0]+float(Xpan1-Xpan0);
				 gl_indatMedian[1]=gl_indatMedian[1]-float(Ypan1-Ypan0);
				 Xpan0=Xpan1;Ypan0=Ypan1;panMouseM=0;
//				 int ipltF=iplotflag;iplotflag=0;Invalidate();
//				 iplotflag=ipltF;

//honk<<Xpan0<<" "<<Ypan0<<" "<<gl_indatMedian[0]<<" "<<gl_indatMedian[1]<<" Timer1Timer 11-3\n";
				 Invalidate();
				}
										 }
 else if(FD_LButtonstatus==13){
zoomRect.left=int(0.05*double(ClientWidth- P1W));
zoomRect.right=int(0.95*double(ClientWidth- P1W));
zoomRect.top=int(0.05*double(ClientHeight- TB1H));
zoomRect.bottom=int(0.95*double(ClientHeight- TB1H));
							   CmZoomIn2(indat.npoin,indat.c1,trans_op,prod_op,trans_zoom,prod_zoom);
							  }
/*
 else if(FD_LButtonstatus==19){iSeq=icTimer5-1000*(icTimer5/1000); //Plot by WP cross-sectional slice  EFP 1/01/0212
							   if(iSeq<wp.nWeldPass) //icTimer5=a+1000b, where a=current WP sequence & b=current slice
								 {for(iWP=0;iWP<wp.nWeldPass;iWP++){WeldPassEditSeqn->CheckSeq=iWP;iup=WeldPassEditSeqn->CheckSeq;
																	if(iup==iSeq+1)break;
																   }
								  dir=WeldPassEditSeqn->CheckDir;
								  icount3=0;for(i=0;i<wp.memWGa;i++){if(wp.eles[wp.memWGa*iWP+i]>=0)icount3++;else break;}
								  if(icTimer5/1000<icount3/wp.n_curr_sttEl[iWP]){
Canvas->Pen->Width=2;Canvas->Pen->Color=wp.WeldColor[iWP];
for(icdiv=0;icdiv<wp.n_curr_sttEl[iWP];icdiv++)
  {if(dir)ie=wp.eles[wp.memWGa*iWP+wp.n_curr_sttEl[iWP]*icTimer5/1000 +icdiv]/10;
   else   ie=wp.eles[wp.memWGa*iWP+wp.n_curr_sttEl[iWP]*(icount3/wp.n_curr_sttEl[iWP]-1-icTimer5/1000)+icdiv]/10;
   iq=0;Canvas->MoveTo(int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+0]]  ]+0.5f),
					   ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+0]]+1]+0.5f));
   for(iq=0;iq<9;iq++)Canvas->LineTo(int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+1]]  ]+0.5f),
									 ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+1]]+1]+0.5f));
   for(iq=9;iq<12;iq++){Canvas->MoveTo(int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+0]]  ]+0.5f),
									   ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+0]]+1]+0.5f));
						Canvas->LineTo(int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+1]]  ]+0.5f),
									   ClientHeight-int(indat.c1[NDF*indat.nop1[MXNPEI*ie+gdedges8[2*iq+1]]+1]+0.5f));
					   }
  }
icTimer5=icTimer5+1000;
																				}
								  else icTimer5=iSeq+1;
								 }
							   else {Timer2->Enabled=false;FD_LButtonstatus=FD_LBrec;stateVFT=stateVFTrec;
									 for(i=0;i<base.nelt;i++)base.arELEM[i]=1;
									 FDrestore();Screen->Cursor=crSizeAll;
									}
							  }
*/
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ElFacetAreaExecute(TObject *Sender)
{if(base.nop1)
   {Screen->Cursor=crDefault;
	FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=23; //Element facet area
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
float TForm1::ElemFacetArea(long ie,long iside,long nop1[],long matno[],float c1[])
{int ngaus8=2,ig=0,jg=0;long eltype=0,bscode=0,node=0,t3=1000,t5=100000,t7=10000000;
 float area=0.f,xn=0.f,yn=0.f,zn=0.f,DJD=0.f,HN[9],SN[MXNPELS],SG[NDF*MXNPELS],DJR[9],POSGP8[2],WEIGP8[2];
 POSGP8[0]= -1.f/sqrt(3.f);POSGP8[1]=1.f/sqrt(3.f);WEIGP8[0]=WEIGP8[1]=1.00000f;
 area=0.f;eltype=matno[ie]/t7;bscode=(matno[ie]-eltype*t7)/t5;node=(matno[ie]-eltype*t7-bscode*t5)/t3;
 if(eltype==5)
	  {
//	   if(iside==0){if(xnat<PRMT)xn=2.*PRMT;else xn=xnat; // STFISO4(ISL,G,E,T...) where S=1-G-E-T=0
//					if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//					if(znat<PRMT)zn=2.*PRMT;else zn=znat;
//					zn=1.-xn-yn;
//				   }
//	   else if(iside==1){xn=0.; // G=0
//						 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//						 if(znat<PRMT)zn=2.*PRMT;else zn=znat;
//						}
//	   else if(iside==2){if(xnat<PRMT)xn=2.*PRMT;else xn=xnat; // E=0
//						 yn=0.;
//						 if(znat<PRMT)zn=2.*PRMT;else zn=znat;
//						}
//	   else {if(xnat<PRMT)xn=2.*PRMT;else xn=xnat; // T=0
//			 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//			 zn=0.;
//			}
//	   if(node==4)STFISO4(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
//	   else       STFISO10(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported element type 5",L"Failure",MB_OK);
	  }
 else if(eltype==7)
	  {
//	   if     (iside==0){if(xnat<PRMT)xn=2.*PRMT;else xn=xnat;
//						 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//						 if(znat< -1.+2.*PRMT)zn= -1.+2.*PRMT;else if(znat> 1.-2.*PRMT)zn=1.-2.*PRMT;else zn=znat;
//						 yn=1.-xn;
//						}
//	   else if(iside==1){xn=0.;
//						 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//						 if(znat< -1.+2.*PRMT)zn= -1.+2.*PRMT;else if(znat> 1.-2.*PRMT)zn=1.-2.*PRMT;else zn=znat;
//						}
//	   else if(iside==2){if(xnat<PRMT)xn=2.*PRMT;else xn=xnat;
//						 yn=0.;
//						 if(znat< -1.+2.*PRMT)zn= -1.+2.*PRMT;else if(znat> 1.-2.*PRMT)zn=1.-2.*PRMT;else zn=znat;
//						}
//	   else if(iside==3){if(xnat<PRMT)xn=2.*PRMT;else xn=xnat;
//						 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//						 zn= -1.;
// 						}
//	   else {if(xnat<PRMT)xn=2.*PRMT;else xn=xnat;
//			 if(ynat<PRMT)yn=2.*PRMT;else yn=ynat;
//			 zn=1.;
//			}
//	   if(node==6)STFISO6(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
//	   else       STFISO15(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported element type 7",L"Failure",MB_OK);
	  }
 else
   {for(ig=0;ig<ngaus8;ig++)
	{for(jg=0;jg<ngaus8;jg++)
	  {if     (iside==0){xn=POSGP8[ig];yn= -1.f;zn=POSGP8[jg];}
	   else if(iside==1){xn=  1.f;yn=POSGP8[ig];zn=POSGP8[jg];}
	   else if(iside==2){xn=POSGP8[ig];yn=  1.f;zn=POSGP8[jg];}
	   else if(iside==3){xn= -1.f;yn=POSGP8[ig];zn=POSGP8[jg];}
	   else if(iside==4){xn=POSGP8[ig];yn=POSGP8[jg];zn= -1.f;}
	   else             {xn=POSGP8[ig];yn=POSGP8[jg];zn=1.f;}
	   if(node==8)STFISO8(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
	   else       STFISO20(3,xn,yn,zn,&DJD,HN,SN,SG,DJR,nop1+MXNPEL*ie,c1);
	   area=area+DJD*WEIGP8[ig]*WEIGP8[jg];
	  }
	}
   }
 return area;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FEstatsExecute(TObject *Sender)
{if(base.nop1){
			   FEstatForm=new TForm2(base.npoin,base.nelt,wp.nWeldGroup,wp.nWeldPass,this);
			   FEstatForm->Caption=L"FE mesh stats";
			   FEstatForm->Label1->Caption=L"Hi-Node";
			   FEstatForm->Label2->Caption=L"Tot-elem";
			   FEstatForm->Label3->Caption=L"#WeldGroup";
			   FEstatForm->Label4->Caption=L"#WeldPass";
			   FEstatForm->ShowModal();
			   delete FEstatForm;FEstatForm=NULL;
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::QueryNodeExecute(TObject *Sender)
{if(base.nop1){if(iPaintyesno/10)
	{FD_LButtonstatus=2;
	 Screen->Cursor=crCross;
	}
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Use VIEW/PAINT to paint",L"Halt",MB_OK);}
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NodalDistanceExecute(TObject *Sender)
{if(base.nop1){if(iPaintyesno/10)
	{FD_LButtonstatus=21;
	 Screen->Cursor=crCross;
	}
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Use VIEW/PAINT to paint",L"Halt",MB_OK);}
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::QNodeTRASH_public()
{delete QNode;QNode=NULL;
FD_LButtonstatus=11; //EFP 8/29/2011
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
} //Nodal distance (Modeless)
//---------------------------------------------------------------------------
void TForm1::QNDistTRASH_public()
{delete QNDist;QNDist=NULL;
FD_LButtonstatus=11; //EFP 8/29/2011
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
} //Nodal distance (Modeless)
//---------------------------------------------------------------------------
void __fastcall TForm1::QueryElementExecute(TObject *Sender)
{if(base.nop1){
//			   extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported",L"Halt",MB_OK);
			   if(iPaintyesno/10)
	{FD_LButtonstatus=3;
	 Screen->Cursor=crCross;
	}
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Use VIEW/PAINT to paint",L"Halt",MB_OK);}
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::QElemTRASH_public(){if(QElem){// delete QElem;
										   QElem=NULL;}
FD_LButtonstatus=11; //EFP 8/29/2011
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
								}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn1Click(TObject *Sender)// Selection mode of graphics
{if(base.nop1){
			   if(CreateLinWeldPass){FD_LButtonstatus=17;stateVFT=1;
///////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
/////////
									}
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No dialog box open.",L"Halt",MB_OK);}
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn2Click(TObject *Sender)// View mode
{if(base.nop1){FD_LButtonstatus=11; // stateVFT=2 for pivot, 3 for pan
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn3Click(TObject *Sender)// 3DMasterSuiteHelp (not used)
//{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"3DMasterSuiteHelp (not used)",L"Halt",MB_OK);}
{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"(unfinished) Graphics Help",L"Halt",MB_OK);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn4Click(TObject *Sender)// Set current view as home view
{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"(unfinished) Set current view as home view",L"Halt",MB_OK);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn5Click(TObject *Sender)// Return to this home view
//{if(base.nop1){iplotflag=1;iCullyesno=1;Invalidate();}
{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"(unfinished) Return to this home view",L"Halt",MB_OK);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn6Click(TObject *Sender)// View all regions of model AKA Center
//{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"(unfinished) Return to this home view",L"Halt",MB_OK);}
{if(base.nop1){
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
			   iplotflag=1;iCullyesno=1;Invalidate();}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn7Click(TObject *Sender)// Target zoom
{if(base.nop1){FD_LBrec=FD_LButtonstatus;stateVFTrec=stateVFT;
			   FD_LButtonstatus=13;stateVFT=4;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
///////////
			  }
//{if(base.nop1){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"(unfinished) Target zoom",L"Halt",MB_OK);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn8Click(TObject *Sender)// ZOOM BOX
{if(base.nop1){FD_LBrec=FD_LButtonstatus;
			   stateVFTrec=stateVFT;
			   FD_LButtonstatus=1;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
///////////
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn9Click(TObject *Sender)
{long in=0;
 if(base.nop1){
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
			   for(in=0;in<base.nelt;in++)base.arELEM[in]=1;//Correction EFP 2/04/2011 (Not used when iplotType != 2 but it is necessary to reset for next polygon)
			   iplotType=0;FDrestore();
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn10Click(TObject *Sender)
{if(base.nop1){iplotType=2;FD_LButtonstatus=16;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
///////////
			  }  // Include elements within poly
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Normal_direction1Execute(TObject *Sender)
{if(base.nop1){int i=(wp.avis-100*(wp.avis/100))/10;
			   wp.avis=wp.avis-10*i;
			   Normal_direction1->Checked=true;
			   Complementary_direction1->Checked=false;
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Complementary_direction1Execute(TObject *Sender)
{if(base.nop1){int i=(wp.avis-100*(wp.avis/100))/10;
			   wp.avis=wp.avis-10*i+10;
			   Normal_direction1->Checked=false;
			   Complementary_direction1->Checked=true;
			  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BackroundColorF0Execute(TObject *Sender) //black
{BackroundColorF0->Checked=true;BackroundColorF1->Checked=false;BackroundColorF2->Checked=false;
 BackroundColorF3->Checked=false;BackroundColorF4->Checked=false;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iBackroundColor=0;iCullyesno=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BackroundColorF1Execute(TObject *Sender) //white
{BackroundColorF0->Checked=false;BackroundColorF1->Checked=true;BackroundColorF2->Checked=false;
 BackroundColorF3->Checked=false;BackroundColorF4->Checked=false;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iBackroundColor=1;iCullyesno=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BackroundColorF2Execute(TObject *Sender) //red
{BackroundColorF0->Checked=false;BackroundColorF1->Checked=false;BackroundColorF2->Checked=true;
 BackroundColorF3->Checked=false;BackroundColorF4->Checked=false;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iBackroundColor=2;iCullyesno=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BackroundColorF3Execute(TObject *Sender) //green
{BackroundColorF0->Checked=false;BackroundColorF1->Checked=false;BackroundColorF2->Checked=false;
 BackroundColorF3->Checked=true;BackroundColorF4->Checked=false;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iBackroundColor=3;iCullyesno=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BackroundColorF4Execute(TObject *Sender) //blue
{BackroundColorF0->Checked=false;BackroundColorF1->Checked=false;BackroundColorF2->Checked=false;
 BackroundColorF3->Checked=false;BackroundColorF4->Checked=true;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iBackroundColor=4;iCullyesno=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::XY_rotExecute(TObject *Sender)
{float dum=0.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(0,dum,dum,dum);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::YZ_rotExecute(TObject *Sender)
{float dum=0.f,val=90.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(1,dum,val,dum);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZX_rotExecute(TObject *Sender)
{float dum=0.f,val=90.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(1,val,dum,dum);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::XYm_rotExecute(TObject *Sender)
{float dum=0.f,val=180.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(1,dum,dum,val);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::YZm_rotExecute(TObject *Sender)
{float dum=0.f,val= -90.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(1,dum,val,dum);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZXm_rotExecute(TObject *Sender)
{float dum=0.f,val= -90.f;
 if(base.nop1){stateVFT=2;FD_LButtonstatus=11;Rot_program(1,val,dum,dum);}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::Rot_program(int isw,float ang0,float ang1,float ang2)
{long ie=0;
 FDbase_indat(1,shapecombo,iplotflag,nColRes);
 if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
//	else {FDcomp_nGID();FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);}
 else {
//								      FDcomp_nGID();
	   FDcomp_nGID(indat.nelt,&nGID,arGID);
	   FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);
	  }
/////////////// EFP 3/19/2012
for(ie=0;ie<base.nelt;ie++)indat.arrELSET[ie]=base.arrELSET[ie];
/////////////
 iPaintyesno=10+1;iCullyesno=0;
//	rangle[0]=rangle0[0]=0.;rangle[1]=rangle0[1]=0.;rangle[2]=rangle0[2]=180.;
 rangle[0]=rangle0[0]=ang0;rangle[1]=rangle0[1]=ang1;rangle[2]=rangle0[2]=ang2;
 axis_plot_init();  //EFP 9/17/2010
 if(isw){FDrotate(0,indat.npoin,indat.c1,rangle);
		 FDrotate_axisplot(0,rangle); //EFP 9/17/2010
		}
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 iplotflag=1;iCircleplot=1;Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WeldParamExecute(TObject *Sender)
{long dum=0;
 wchar_t curMess0[]=L"WeldParam",string1[15];
 if(base.nop1)
//   {if(wps.nWeldParamSet<MAX_WELDPARSET)
   {if(!CreateWeldingParamSet)
	  {
//honk<<wps.nWeldParamSet<<" Enter WeldParamExecute()\n";
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
	   FDdynmem_manage(8,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
//SSSSSSSSSSSSSSSSSSSSSSSSSSS
	   if(wps.nWeldParamSet>0){
			 wps.curr[wps.nWeldParamSet]=wps.curr[wps.nWeldParamSet-1];
			 wps.volt[wps.nWeldParamSet]=wps.volt[wps.nWeldParamSet-1];
			 wps.eff[wps.nWeldParamSet]=wps.eff[wps.nWeldParamSet-1];
			 wps.speed[wps.nWeldParamSet]=wps.speed[wps.nWeldParamSet-1];
							  }
	   else {
// CreateLinWeldPass->CheckEdit1=curMess0;
			 wps.curr[wps.nWeldParamSet]=350.f;
			 wps.volt[wps.nWeldParamSet]=25.f;
			 wps.eff[wps.nWeldParamSet]=0.75f;
			 wps.speed[wps.nWeldParamSet]=5.f;
			}
// _ltow(wps.nWeldParamSet+1,string1,10);
 _ltow(wps.highest+1,string1,10);
// honk<<string1[].t_str()<<" string1\n";
// honk<<curMess0.t_str()<<" curMess0\n";
 StringCchCatW(curMess0,39+1,string1); //Length of destination curMess0 is provided.
 wps.name[wps.nWeldParamSet]=curMess0;
	   CreateWeldingParamSet=new TForm22(0,this);



	   CreateWeldingParamSet->Caption=L"Create a Welding Parameter Set";
	   CreateWeldingParamSet->Label1->Caption=L"Welding Parameter Set Name";
	   CreateWeldingParamSet->Label2->Caption=L"Welding Current (Amp):";
	   CreateWeldingParamSet->Label3->Caption=L"Welding Voltage (Volt):";
	   CreateWeldingParamSet->Label4->Caption=L"Arc Efficiency:";
	   CreateWeldingParamSet->Label5->Caption=L"Torch Traveling Speed:";
	   CreateWeldingParamSet->Button1->Caption=L"Create";
	   CreateWeldingParamSet->Button2->Caption=L"Cancel";

////	   CreateWeldingParamSet->CheckEdit1=L"****";
//	   CreateWeldingParamSet->CheckEdit1=wps.name[wps.nWeldParamSet];
//	   CreateWeldingParamSet->CheckEdit1=wps.name[wps.nWeldParamSet].c_str();
	   CreateWeldingParamSet->CheckEdit1=wps.name[wps.nWeldParamSet].w_str();
	   CreateWeldingParamSet->CheckEdit2=wps.curr[wps.nWeldParamSet];
	   CreateWeldingParamSet->CheckEdit3=wps.volt[wps.nWeldParamSet];
	   CreateWeldingParamSet->CheckEdit4=wps.eff[wps.nWeldParamSet];
	   CreateWeldingParamSet->CheckEdit5=wps.speed[wps.nWeldParamSet];
	   CreateWeldingParamSet->ShowModal();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Close existing Create/Edit box first",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::WeldParam_public() //TBD Test these
{int missing=0,isel=CreateWeldingParamSet->CheckISEL;long i=wp.PRECORD;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
//FDrestore();
///////////

 if(isel){wps.name[i]=CreateWeldingParamSet->CheckEdit1;wps.curr[i]=CreateWeldingParamSet->CheckEdit2;
		  wps.volt[i]=CreateWeldingParamSet->CheckEdit3;wps.eff[i]=CreateWeldingParamSet->CheckEdit4;
		  wps.speed[i]=CreateWeldingParamSet->CheckEdit5;
		  FD_LButtonstatus=11;stateVFT=2;

//honk<<isel<<" WeldParam_public0\n";//if(1==1)exit(0);

		  delete CreateWeldingParamSet; //Created with Show()
//	CreateWeldingParamSet=NULL;// because it was created with Show()
		 }
 else {if(CreateWeldingParamSet->CheckEdit1==L"****")
		 {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter weld parameter name",L"Repeat",MB_OK);
		  missing++;
		 }
	   else wps.name[wps.nWeldParamSet]=CreateWeldingParamSet->CheckEdit1;
	   wps.curr[wps.nWeldParamSet]=CreateWeldingParamSet->CheckEdit2;
	   wps.volt[wps.nWeldParamSet]=CreateWeldingParamSet->CheckEdit3;
	   wps.eff[wps.nWeldParamSet]=CreateWeldingParamSet->CheckEdit4;
	   wps.speed[wps.nWeldParamSet]=CreateWeldingParamSet->CheckEdit5;

//honk<<isel<<" WeldParam_public1\n";//if(1==1)exit(0);

	   if(!missing){
					wps.nWeldParamSet=wps.nWeldParamSet+1;
					wps.highest=wps.highest+1;
				   }
// delete CreateWeldingParamSet;
	   CreateWeldingParamSet->Close(); //Created with ShowModal()
	  }
 CreateWeldingParamSet=NULL;
/////////// Cursor EFP 1/21/2011
//Screen->Cursor=crSizeAll;
//FDrestore();
///////////
}
//---------------------------------------------------------------------------
void TForm1::EditWeldParamProg(int VFTitemindex)
{
 wp.PRECORD=VFTitemindex;
//honk<<wp.PRECORD<<" Enter EditWeldParam()\n";
 CreateWeldingParamSet=new TForm22(1,this); // No test: i.e. This presumes that TForm22 is always created by ShowModal()
 CreateWeldingParamSet->Caption=L"Edit a Welding Parameter Set";
 CreateWeldingParamSet->Label1->Caption=L"Welding Parameter Set Name";
 CreateWeldingParamSet->Label2->Caption=L"Welding Current (Amp):";
 CreateWeldingParamSet->Label3->Caption=L"Welding Voltage (Volt):";
 CreateWeldingParamSet->Label4->Caption=L"Arc Efficiency:";
 CreateWeldingParamSet->Label5->Caption=L"Torch Traveling Speed:";
 CreateWeldingParamSet->Button1->Caption=L"Enter";
 CreateWeldingParamSet->Button2->Caption=L"Restore";
 wps.temp_name=wps.name[wp.PRECORD];
 wps.temp_curr=wps.curr[wp.PRECORD];
 wps.temp_volt=wps.volt[wp.PRECORD];
 wps.temp_eff=wps.eff[wp.PRECORD];
 wps.temp_speed=wps.speed[wp.PRECORD];
 CreateWeldingParamSet->CheckEdit1=wps.name[wp.PRECORD].w_str();
 CreateWeldingParamSet->CheckEdit2=wps.curr[wp.PRECORD];
 CreateWeldingParamSet->CheckEdit3=wps.volt[wp.PRECORD];
 CreateWeldingParamSet->CheckEdit4=wps.eff[wp.PRECORD];
 CreateWeldingParamSet->CheckEdit5=wps.speed[wp.PRECORD];
// CreateWeldingParamSet->ShowModal();
 CreateWeldingParamSet->Show();// Note that you can not follow a VFTlistbox->ShowModal() with a CreateWeldingParamSet->ShowModal()
}
//---------------------------------------------------------------------------
void TForm1::EditWeldParam_public()
{int isel=CreateWeldingParamSet->CheckISEL;long i=wp.PRECORD;
 if(isel)
   {wps.name[i]=wps.temp_name;wps.curr[i]=wps.temp_curr;wps.volt[i]=wps.temp_volt;wps.eff[i]=wps.temp_eff;wps.speed[i]=wps.temp_speed;
	FD_LButtonstatus=11;stateVFT=2;
	delete CreateWeldingParamSet;
//	CreateWeldingParamSet=NULL;// because it was created with Show()
   }
 else CreateWeldingParamSet->Close();
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 CreateWeldingParamSet=NULL;
}
//---------------------------------------------------------------------------
void TForm1::VFTlistbox_public()
{int VFTitemindex= VFTlistbox->CheckItemIndex,VFTsel= VFTlistbox->CheckISEL;
 VFTlistbox->Close();
 VFTlistbox=NULL; // Necessary to set to NULL but why?


 if(VFTsel==1)DelWeldPass(VFTitemindex,wp.memWGa); //Correction EFP 3/27/2011
 else if(VFTsel==2)DelWeldParam(VFTitemindex);
 else if(VFTsel==3)EditWeldParamProg(VFTitemindex);
 else if(VFTsel==4)EditMatPropProg(VFTitemindex);
 else if(VFTsel==5)DelMatPropProg(VFTitemindex);
 else if(VFTsel==6 || VFTsel==7)EditWeldPassProg(VFTitemindex);
// else if(VFTsel==8)ShowWPoutlineProg(VFTitemindex);
// else if(VFTsel==9)ShowWGselectorProg(VFTitemindex);
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported VFTsel option",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::DelWeldPass(int VFTitemindex,long bnelt)
{
//int i=0,j=0;
// TBD: Adjust memory
 int i=0,*SWTseqNum=NULL,*SWTboolFlags=NULL,*SWTtype=NULL,*SWTshape=NULL,*SWTstepInterval=NULL,
	 *SWThp=NULL,*SWTnsegs=NULL,*SWTiselect=NULL,*SWTmcr=NULL,*SWTsource=NULL,*SWTutil_arr=NULL
//	 ,*SWTedgeFlag,*SWTcircFlag,*SWTmerge
	 ;
// GIDwp: 1st col= current iside, remainder=GID
 long ii=0,in=0,dum=0,ieGID1=0,//eltype1=0,bscode1=0,node1=0,t3=1000,t5=100000,t7=10000000,
	  *SWTeles=NULL,*SWTsttEles=NULL,*SWTstpEle=NULL,*SWTfirstEle=NULL,
	  *SWTnextEle=NULL,*SWTsnorm1=NULL,*SWTsnorm2=NULL,
	  *SWTcircEles=NULL,*SWTedgeEles=NULL,*SWTedgeNodes=NULL,*SWTsttEleNodes=NULL,
	  *SWThlightel=NULL,*SWTn_curr_sttEl=NULL,*SWTprevGID=NULL,*SWTreset=NULL;
 float *SWTcurr=NULL,*SWTvolt=NULL,*SWTeff=NULL,*SWTspeed=NULL,*SWTtroom=NULL,*SWTtmelt=NULL,
	   *SWTtcutl=NULL,*SWTtcuth=NULL,*SWTtimeInterval=NULL,
	   *SWTmaxiHeatStep=NULL,*SWTminiHeatStep=NULL,*SWTthk1=NULL,*SWTthk2=NULL,*SWTwpTimes=NULL;
 double *SWTlstart=NULL,*SWTlend=NULL,*SWTarrows=NULL;
 String *SWTname=NULL,*SWTmatName=NULL;
 TColor *SWTWeldColor=NULL;


for(in=0;in<base.nelt;in++)
  {
ieGID1=indat.arrELSET[in];

   if(ieGID1==VFTitemindex+wp.nWeldGroup+1)
	 {
	  base.arrELSET[in]= -1;
	  indat.arrELSET[in]= -1;
	 }
////////////////////
   else if(ieGID1>VFTitemindex+wp.nWeldGroup+1){
base.arrELSET[in]=base.arrELSET[in]-1;
indat.arrELSET[in]=indat.arrELSET[in]-1;
											   }
////////////////////
  }
////////////////////////// start Correction: Code to adjust seqNum[]  EFP 3/26/2011
 i=wp.seqNum[VFTitemindex];
 for(in=0;in<wp.nWeldPass;in++){if(wp.seqNum[in]>i)wp.seqNum[in]=wp.seqNum[in]-1;
// honk<<in<<" "<<wp.seqNum[in]<<" DEL seqn checkAFT\n";
							   }
////////////////////////// end

//for(in=0;in<wp.nWeldPass;in++)honk<<in<<" "<<(long)wp.WeldColor[in]<<" EarlyCkTk\n";
//////////////////////////////
//////////////////////////////
//////////////////////////////

 if(wp.nWeldPass>1)
	   {SWTname=new String[wp.nWeldPass-1];
		SWTmatName=new String[wp.nWeldPass-1];
		SWTseqNum=new int[wp.nWeldPass-1];
		SWTboolFlags=new int[wp.nWeldPass-1];
		SWTtype=new int[wp.nWeldPass-1];
		SWTshape=new int[wp.nWeldPass-1];
		SWTstepInterval=new int[wp.nWeldPass-1];
		SWThp=new int[wp.nWeldPass-1];
		SWTnsegs=new int[wp.nWeldPass-1];
		SWTiselect=new int[wp.nWeldPass-1];
		SWTeles=new long[(wp.nWeldPass-1)*bnelt];  // Elements in this weld pass
		SWTsttEles=new long[(wp.nWeldPass-1)*bnelt];  // No need for *4 ????
		SWTstpEle=new long[wp.nWeldPass-1];  // No need for *bnelt
		SWTfirstEle=new long[(wp.nWeldPass-1)*bnelt];
		SWTnextEle=new long[(wp.nWeldPass-1)*bnelt];
		SWTsnorm1=new long[(wp.nWeldPass-1)*4];
		SWTsnorm2=new long[(wp.nWeldPass-1)*4];
		SWTcircEles=new long[(wp.nWeldPass-1)*3];
		SWTedgeEles=new long[(wp.nWeldPass-1)*bnelt];
		SWTedgeNodes=new long[(wp.nWeldPass-1)*4]; // correct???
		SWTsttEleNodes=new long[(wp.nWeldPass-1)*bnelt*4];
		SWTcurr=new float[wp.nWeldPass-1];
		SWTvolt=new float[wp.nWeldPass-1];
		SWTeff=new float[wp.nWeldPass-1];
		SWTspeed=new float[wp.nWeldPass-1];
		SWTtroom=new float[wp.nWeldPass-1];
		SWTtmelt=new float[wp.nWeldPass-1];
		SWTtcutl=new float[wp.nWeldPass-1];
		SWTtcuth=new float[wp.nWeldPass-1];
		SWTtimeInterval=new float[wp.nWeldPass-1];
		SWTmaxiHeatStep=new float[wp.nWeldPass-1];
		SWTminiHeatStep=new float[wp.nWeldPass-1];
		SWTthk1=new float[wp.nWeldPass-1];
		SWTthk2=new float[wp.nWeldPass-1];
		SWTlstart=new double[wp.nWeldPass-1];
		SWTlend=new double[wp.nWeldPass-1];
		SWThlightel=new long[(wp.nWeldPass-1)*bnelt]; //Convention: 1st col= elem side#
		SWTarrows=new double[3*NDF*(wp.nWeldPass-1)]; //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		SWTn_curr_sttEl=new long[wp.nWeldPass-1];
		SWTreset=new long[wp.nWeldPass-1];
		SWTwpTimes=new float[wp.nWeldPass-1];
		SWTsource=new int[wp.nWeldPass-1];
		SWTmcr=new int[wp.nWeldPass-1]; // Not part of original VFT
		SWTWeldColor=new TColor[wp.nWeldPass-1];
		SWTprevGID=new long[wp.nWeldPass-1];
		SWTutil_arr=new int[wp.nWeldPass-1];

 if(VFTitemindex)
   {
		for(in=0;in<VFTitemindex;in++) *(SWTname+in)= *(wp.name+in);
		for(in=0;in<VFTitemindex;in++) *(SWTmatName+in)= *(wp.matName+in);
		for(in=0;in<VFTitemindex;in++) *(SWTseqNum+in)= *(wp.seqNum+in);
		for(in=0;in<VFTitemindex;in++) *(SWTboolFlags+in)= *(wp.boolFlags+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtype+in)= *(wp.type+in);
		for(in=0;in<VFTitemindex;in++) *(SWTshape+in)= *(wp.shape+in);
		for(in=0;in<VFTitemindex;in++) *(SWTstepInterval+in)= *(wp.stepInterval+in);
		for(in=0;in<VFTitemindex;in++) *(SWThp+in)= *(wp.hp+in);
		for(in=0;in<VFTitemindex;in++) *(SWTnsegs+in)= *(wp.nsegs+in);
		for(in=0;in<VFTitemindex;in++) *(SWTiselect+in)= *(wp.iselect+in);
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWTeles+in)= *(wp.eles+in);  // Elements in this weld pass
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWTsttEles+in)= *(wp.sttEles+in);  // No need for *4 ????
		for(in=0;in<VFTitemindex;in++) *(SWTstpEle+in)= *(wp.stpEle+in);  // No need for *bnelt
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWTfirstEle+in)= *(wp.firstEle+in);
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWTnextEle+in)= *(wp.nextEle+in);
		for(in=0;in<VFTitemindex*4;in++) *(SWTsnorm1+in)= *(wp.snorm1+in);
		for(in=0;in<VFTitemindex*4;in++) *(SWTsnorm2+in)= *(wp.snorm2+in);
		for(in=0;in<VFTitemindex*3;in++) *(SWTcircEles+in)= *(wp.circEles+in);
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWTedgeEles+in)= *(wp.edgeEles+in);
		for(in=0;in<VFTitemindex*4;in++) *(SWTedgeNodes+in)= *(wp.edgeNodes+in); // correct???
		for(in=0;in<VFTitemindex*bnelt*4;in++) *(SWTsttEleNodes+in)= *(wp.sttEleNodes+in);
		for(in=0;in<VFTitemindex;in++) *(SWTcurr+in)= *(wp.curr+in);
		for(in=0;in<VFTitemindex;in++) *(SWTvolt+in)= *(wp.volt+in);
		for(in=0;in<VFTitemindex;in++) *(SWTeff+in)= *(wp.eff+in);
		for(in=0;in<VFTitemindex;in++) *(SWTspeed+in)= *(wp.speed+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtroom+in)= *(wp.troom+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtmelt+in)= *(wp.tmelt+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtcutl+in)= *(wp.tcutl+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtcuth+in)= *(wp.tcuth+in);
		for(in=0;in<VFTitemindex;in++) *(SWTtimeInterval+in)= *(wp.timeInterval+in);
		for(in=0;in<VFTitemindex;in++) *(SWTmaxiHeatStep+in)= *(wp.maxiHeatStep+in);
		for(in=0;in<VFTitemindex;in++) *(SWTminiHeatStep+in)= *(wp.miniHeatStep+in);
		for(in=0;in<VFTitemindex;in++) *(SWTthk1+in)= *(wp.thk1+in);
		for(in=0;in<VFTitemindex;in++) *(SWTthk2+in)= *(wp.thk2+in);
		for(in=0;in<VFTitemindex;in++) *(SWTlstart+in)= *(wp.lstart+in);
		for(in=0;in<VFTitemindex;in++) *(SWTlend+in)= *(wp.lend+in);
		for(in=0;in<VFTitemindex*bnelt;in++) *(SWThlightel+in)= *(wp.hlightel+in); //Convention: 1st col= elem side#
		for(in=0;in<VFTitemindex*3*NDF;in++) *(SWTarrows+in)= *(wp.arrows+in); //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(in=0;in<VFTitemindex;in++) *(SWTn_curr_sttEl+in)= *(wp.n_curr_sttEl+in);
		for(in=0;in<VFTitemindex;in++) *(SWTreset+in)= *(wp.reset+in);
		for(in=0;in<VFTitemindex;in++) *(SWTwpTimes+in)= *(wp.wpTimes+in);
		for(in=0;in<VFTitemindex;in++) *(SWTsource+in)= *(wp.source+in);
		for(in=0;in<VFTitemindex;in++) *(SWTmcr+in)= *(wp.mcr+in); // Not part of original VFT
		for(in=0;in<VFTitemindex;in++) *(SWTWeldColor+in)= *(wp.WeldColor+in);
		for(in=0;in<VFTitemindex;in++) *(SWTprevGID+in)= *(wp.prevGID+in);
		for(in=0;in<VFTitemindex;in++) *(SWTutil_arr+in)= *(wp.util_arr+in);
   }
 if(VFTitemindex+1<wp.nWeldPass)
   {
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTname+in-1)= *(wp.name+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTmatName+in-1)= *(wp.matName+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTseqNum+in-1)= *(wp.seqNum+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTboolFlags+in-1)= *(wp.boolFlags+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtype+in-1)= *(wp.type+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTshape+in-1)= *(wp.shape+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTstepInterval+in-1)= *(wp.stepInterval+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWThp+in-1)= *(wp.hp+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTnsegs+in-1)= *(wp.nsegs+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTiselect+in-1)= *(wp.iselect+in);
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWTeles+in-bnelt)= *(wp.eles+in);  // Elements in this weld pass
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWTsttEles+in-bnelt)= *(wp.sttEles+in);  // No need for *4 ????
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTstpEle+in-1)= *(wp.stpEle+in);  // No need for *bnelt
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWTfirstEle+in-bnelt)= *(wp.firstEle+in);
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWTnextEle+in-bnelt)= *(wp.nextEle+in);
		for(in=(VFTitemindex+1)*4;in<wp.nWeldPass*4;in++) *(SWTsnorm1+in-4)= *(wp.snorm1+in);
		for(in=(VFTitemindex+1)*4;in<wp.nWeldPass*4;in++) *(SWTsnorm2+in-4)= *(wp.snorm2+in);
		for(in=(VFTitemindex+1)*3;in<wp.nWeldPass*3;in++) *(SWTcircEles+in-3)= *(wp.circEles+in);
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWTedgeEles+in-bnelt)= *(wp.edgeEles+in);
		for(in=(VFTitemindex+1)*4;in<wp.nWeldPass*4;in++) *(SWTedgeNodes+in-4)= *(wp.edgeNodes+in); // correct???
		for(in=(VFTitemindex+1)*bnelt*4;in<wp.nWeldPass*bnelt*4;in++) *(SWTsttEleNodes+in-bnelt*4)= *(wp.sttEleNodes+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTcurr+in-1)= *(wp.curr+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTvolt+in-1)= *(wp.volt+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTeff+in-1)= *(wp.eff+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTspeed+in-1)= *(wp.speed+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtroom+in-1)= *(wp.troom+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtmelt+in-1)= *(wp.tmelt+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtcutl+in-1)= *(wp.tcutl+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtcuth+in-1)= *(wp.tcuth+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTtimeInterval+in-1)= *(wp.timeInterval+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTmaxiHeatStep+in-1)= *(wp.maxiHeatStep+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTminiHeatStep+in-1)= *(wp.miniHeatStep+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTthk1+in-1)= *(wp.thk1+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTthk2+in-1)= *(wp.thk2+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTlstart+in-1)= *(wp.lstart+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTlend+in-1)= *(wp.lend+in);
		for(in=(VFTitemindex+1)*bnelt;in<wp.nWeldPass*bnelt;in++) *(SWThlightel+in-bnelt)= *(wp.hlightel+in); //Convention: 1st col= elem side#
		for(in=(VFTitemindex+1)*3*NDF;in<wp.nWeldPass*3*NDF;in++) *(SWTarrows+in-3*NDF)= *(wp.arrows+in); //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTn_curr_sttEl+in-1)= *(wp.n_curr_sttEl+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTreset+in-1)= *(wp.reset+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTwpTimes+in-1)= *(wp.wpTimes+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTsource+in-1)= *(wp.source+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTmcr+in-1)= *(wp.mcr+in); // Not part of original VFT
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTWeldColor+in-1)= *(wp.WeldColor+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTprevGID+in-1)= *(wp.prevGID+in);
		for(in=VFTitemindex+1;in<wp.nWeldPass;in++) *(SWTutil_arr+in-1)= *(wp.util_arr+in);
   }
		FDdynmem_manage(-9,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
		wp.nWeldPass=wp.nWeldPass-1;

//honk<<VFTitemindex<<" SWT DelWeldPassAFT "<<bnelt<<" "<<base.nelt<<" "<<wp.nWeldPass<<"\n";
//for(ii=0;ii<wp.nWeldPass;ii++)
//  {for(in=0;in<bnelt;in++)honk<<(ii+1)<<" "<<(in+1)<<" "<< *(SWTsttEles+bnelt*ii+in) <<" SWTsttEles\n";
//  }

		FDdynmem_manage( 9,dum,bnelt,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
		for(in=0;in<wp.nWeldPass;in++) *(wp.name+in)= *(SWTname+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.matName+in)= *(SWTmatName+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.seqNum+in)= *(SWTseqNum+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.boolFlags+in)= *(SWTboolFlags+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.type+in)= *(SWTtype+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.shape+in)= *(SWTshape+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.stepInterval+in)= *(SWTstepInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.hp+in)= *(SWThp+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.nsegs+in)= *(SWTnsegs+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.iselect+in)= *(SWTiselect+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.eles+in)= *(SWTeles+in);  // Elements in this weld pass
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.sttEles+in)= *(SWTsttEles+in);  // No need for *4 ????
		for(in=0;in<wp.nWeldPass;in++) *(wp.stpEle+in)= *(SWTstpEle+in);  // No need for *bnelt
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.firstEle+in)= *(SWTfirstEle+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.nextEle+in)= *(SWTnextEle+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.snorm1+in)= *(SWTsnorm1+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.snorm2+in)= *(SWTsnorm2+in);
		for(in=0;in<wp.nWeldPass*3;in++) *(wp.circEles+in)= *(SWTcircEles+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.edgeEles+in)= *(SWTedgeEles+in);
		for(in=0;in<wp.nWeldPass*4;in++) *(wp.edgeNodes+in)= *(SWTedgeNodes+in); // correct???
		for(in=0;in<wp.nWeldPass*bnelt*4;in++) *(wp.sttEleNodes+in)= *(SWTsttEleNodes+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.curr+in)= *(SWTcurr+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.volt+in)= *(SWTvolt+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.eff+in)= *(SWTeff+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.speed+in)= *(SWTspeed+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.troom+in)= *(SWTtroom+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tmelt+in)= *(SWTtmelt+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tcutl+in)= *(SWTtcutl+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.tcuth+in)= *(SWTtcuth+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.timeInterval+in)= *(SWTtimeInterval+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.maxiHeatStep+in)= *(SWTmaxiHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.miniHeatStep+in)= *(SWTminiHeatStep+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.thk1+in)= *(SWTthk1+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.thk2+in)= *(SWTthk2+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.lstart+in)= *(SWTlstart+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.lend+in)= *(SWTlend+in);
		for(in=0;in<wp.nWeldPass*bnelt;in++) *(wp.hlightel+in)= *(SWThlightel+in); //Convention: 1st col= elem side#
		for(in=0;in<wp.nWeldPass*3*NDF;in++) *(wp.arrows+in)= *(SWTarrows+in); //Convention: 1st arrow=weld direction, 2nd & 3rd=plate normals
		for(in=0;in<wp.nWeldPass;in++) *(wp.n_curr_sttEl+in)= *(SWTn_curr_sttEl+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.reset+in)= *(SWTreset+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.wpTimes+in)= *(SWTwpTimes+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.source+in)= *(SWTsource+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.mcr+in)= *(SWTmcr+in); // Not part of original VFT
		for(in=0;in<wp.nWeldPass;in++) *(wp.WeldColor+in)= *(SWTWeldColor+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.prevGID+in)= *(SWTprevGID+in);
		for(in=0;in<wp.nWeldPass;in++) *(wp.util_arr+in)= *(SWTutil_arr+in);


//for(in=0;in<wp.nWeldPass;in++)honk<<in<<" "<<(long)wp.WeldColor[in]<<" LateCkTk\n";
//////////////////////////////
//////////////////////////////
//////////////////////////////
					delete [] SWTname;SWTname=NULL;
					delete [] SWTmatName;SWTmatName=NULL;
					delete [] SWTseqNum;SWTseqNum=NULL;
					delete [] SWTboolFlags;SWTboolFlags=NULL;
					delete [] SWTtype;SWTtype=NULL;
					delete [] SWTshape;SWTshape=NULL;
					delete [] SWTstepInterval;SWTstepInterval=NULL;
					delete [] SWThp;SWThp=NULL;
					delete [] SWTnsegs;SWTnsegs=NULL;
					delete [] SWTiselect;SWTiselect=NULL;
					delete [] SWTeles;SWTeles=NULL;
					delete [] SWTsttEles;SWTsttEles=NULL;
					delete [] SWTstpEle;SWTstpEle=NULL;
					delete [] SWTfirstEle;SWTfirstEle=NULL;
					delete [] SWTnextEle;SWTnextEle=NULL;
					delete [] SWTsnorm1;SWTsnorm1=NULL;
					delete [] SWTsnorm2;SWTsnorm2=NULL;
					delete [] SWTcircEles;SWTcircEles=NULL;
					delete [] SWTedgeEles;SWTedgeEles=NULL;
					delete [] SWTedgeNodes;SWTedgeNodes=NULL;
					delete [] SWTsttEleNodes;SWTsttEleNodes=NULL;
					delete [] SWTcurr;SWTcurr=NULL;
					delete [] SWTvolt;SWTvolt=NULL;
					delete [] SWTeff;SWTeff=NULL;
					delete [] SWTspeed;SWTspeed=NULL;
					delete [] SWTtroom;SWTtroom=NULL;
					delete [] SWTtmelt;SWTtmelt=NULL;
					delete [] SWTtcutl;SWTtcutl=NULL;
					delete [] SWTtcuth;SWTtcuth=NULL;
					delete [] SWTtimeInterval;SWTtimeInterval=NULL;
					delete [] SWTmaxiHeatStep;SWTmaxiHeatStep=NULL;
					delete [] SWTminiHeatStep;SWTminiHeatStep=NULL;
					delete [] SWTthk1;SWTthk1=NULL;
					delete [] SWTthk2;SWTthk2=NULL;
					delete [] SWTlstart;SWTlstart=NULL;
					delete [] SWTlend;SWTlend=NULL;
					delete [] SWThlightel;SWThlightel=NULL;
					delete [] SWTarrows;SWTarrows=NULL;
					delete [] SWTn_curr_sttEl;SWTn_curr_sttEl=NULL;
					delete [] SWTreset;SWTreset=NULL;
					delete [] SWTwpTimes;SWTwpTimes=NULL;
					delete [] SWTsource;SWTsource=NULL;
					delete [] SWTmcr;SWTmcr=NULL;// Not part of original VFT
					delete [] SWTWeldColor;SWTWeldColor=NULL;
					delete [] SWTprevGID;SWTprevGID=NULL;
					delete [] SWTutil_arr;SWTutil_arr=NULL;
	   }
 else {
		FDdynmem_manage(-9,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
		wp.nWeldPass=wp.nWeldPass-1;
//        extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld passes in model",L"Warning",MB_OK);
	  }
 if(wp.nWeldPass==0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"There will be no weld passes in model",L"Warning",MB_OK);}
//iplotflag=1;iCircleplot=1;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
//							   iCullyesno=0;

////////////////// start EFP 4/04/2011
								if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
//								else {FDcomp_nGID();FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);}
//								iPaintyesno=10;iCullyesno=0;

								else {
//								FDcomp_nGID();
FDcomp_nGID(indat.nelt,&nGID,arGID);
								FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);}
								iPaintyesno=10+1;iCullyesno=0;
////////////////// end
//honk<<iplotflag<<" "<<iCircleplot<<" "<<iPaintyesno<<" "<<iplotType<<" "<<iCullyesno<<" DELgraphValCHECK\n";

							   Invalidate();//EFP 1/08/2011
}
//---------------------------------------------------------------------------
void TForm1::DelWeldParam(int VFTitemindex)
{long i=0,dum=0;
 float *SWTcurr=NULL,*SWTvolt=NULL,*SWTeff=NULL,*SWTspeed=NULL;
 String *SWTname=NULL;
//honk<<VFTitemindex<<" DelWeldParamSet\n";
 if(wps.nWeldParamSet>1)
   {
	SWTname=new String[wps.nWeldParamSet-1];
	SWTcurr=new float[wps.nWeldParamSet-1];
	SWTvolt=new float[wps.nWeldParamSet-1];
	SWTeff=new float[wps.nWeldParamSet-1];
	SWTspeed=new float[wps.nWeldParamSet-1];
	if(VFTitemindex){for(i=0;i<VFTitemindex;i++)SWTname[i]=wps.name[i];
					 for(i=0;i<VFTitemindex;i++)SWTcurr[i]=wps.curr[i];
					 for(i=0;i<VFTitemindex;i++)SWTvolt[i]=wps.volt[i];
					 for(i=0;i<VFTitemindex;i++)SWTeff[i]=wps.eff[i];
					 for(i=0;i<VFTitemindex;i++)SWTspeed[i]=wps.speed[i];
					}
	if(VFTitemindex+1<wps.nWeldParamSet){for(i=VFTitemindex+1;i<wps.nWeldParamSet;i++)SWTname[i-1]=wps.name[i];
										 for(i=VFTitemindex+1;i<wps.nWeldParamSet;i++)SWTcurr[i-1]=wps.curr[i];
										 for(i=VFTitemindex+1;i<wps.nWeldParamSet;i++)SWTvolt[i-1]=wps.volt[i];
										 for(i=VFTitemindex+1;i<wps.nWeldParamSet;i++)SWTeff[i-1]=wps.eff[i];
										 for(i=VFTitemindex+1;i<wps.nWeldParamSet;i++)SWTspeed[i-1]=wps.speed[i];
										}
	FDdynmem_manage(-11,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	wps.nWeldParamSet=wps.nWeldParamSet-1;
	FDdynmem_manage( 11,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	for(i=0;i<wps.nWeldParamSet;i++)wps.name[i]=SWTname[i];
	for(i=0;i<wps.nWeldParamSet;i++)wps.curr[i]=SWTcurr[i];
	for(i=0;i<wps.nWeldParamSet;i++)wps.volt[i]=SWTvolt[i];
	for(i=0;i<wps.nWeldParamSet;i++)wps.eff[i]=SWTeff[i];
	for(i=0;i<wps.nWeldParamSet;i++)wps.speed[i]=SWTspeed[i];
	delete [] SWTname;delete [] SWTcurr;delete [] SWTvolt;delete [] SWTeff;delete [] SWTspeed;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
   }
 else if(wps.nWeldParamSet==1)
   {FDdynmem_manage(-11,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	wps.nWeldParamSet=0;
	extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Weld parameter sets no longer in the model",L"Warning",MB_OK);
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Inadmissible nWeldParamSet in DelWeldParam()",L"Terminate",MB_OK);exit(0);}
}
//---------------------------------------------------------------------------
void TForm1::MatProperties_public()
{int missing=0,ip=Form21->CheckRadioGroup1,isel=Form21->CheckISEL;
 long i=wp.PRECORD;
//////////////////////////////// Convention  EFP 3/13/2012
//wms.mcr  CheckRadioGroup1  Model        RadioButton  ip
//0           0          Iso(multilin)        1        0
//1           3          CombineH(lin)        4        3
//2           1          CombineH(multilin)   2        1
//3           4          EPC(Iso,multilin)    5        4
//other(6)    5          CompletePhT          6        5
//5           2          SinglePhTransf       3        2
////////////////////////////////

//honk<<isel<<" "<<ip<<" IPPPPPP "<<Form21->CheckCheckBox1<<"\n";

 if(isel){wms.cond[i]=Form21->CheckEdit2;
		  wms.heat[i]=Form21->CheckEdit3;
		  wms.den[i]=Form21->CheckEdit4;
		  if(ip==0){wms.mcr[i]=0;
					if(Form21->CheckCheckBox1){wms.hetjd[i]= -1;wms.switc[i]=true;
//														 wms.nprops[i]=wms.nprops[i]+1;
														 wms.Steps[i]=Form21->CheckEdit5a;
														}
					else {wms.hetjd[i]=0;
						  wms.switc[i]=false;
						 }
				   }
		  else if(ip==1){wms.mcr[i]=2;wms.hetjd[i]=0;wms.switc[i]=false;}
		  else if(ip==2){wms.mcr[i]=5;wms.hetjd[i]=0;wms.switc[i]=false;}
		  else if(ip==3){wms.mcr[i]=1;wms.hetjd[i]=0;wms.switc[i]=false;}
		  else if(ip==4){wms.mcr[i]=3;  //Creep added to EPC  EFP 3/14/2012
//						 wms.hetjd[i]=0;wms.switc[i]=false;
					if(Form21->CheckCheckBox1){wms.hetjd[i]= -1;wms.switc[i]=true;
//														 wms.nprops[i]=wms.nprops[i]+1;
														 wms.Steps[i]=Form21->CheckEdit5a;
														}
					else {wms.hetjd[i]=0;
						  wms.switc[i]=false;
						 }
						}//Elas=Plas-Creep
		  else          {wms.mcr[i]=6;wms.hetjd[i]=0;wms.switc[i]=false;} // Correction  BBrust 3/25/2011 & EFP 3/27/2011

//VFT convention: wms.annjd[]= -1 is ON (read & use Ti/Ta/Tm);=0 is OFF (use pre-loaded 750/1500/1500)
		  if(Form21->CheckGroupBox3)wms.annjd[i]= -1; //Correction#2 EFP 3/15/2012
//		  if(!Form21->CheckGroupBox3)wms.annjd[i]= -1;//Correction  EFP 9/09/2011
		  else {wms.annjd[i]=0;
//				Form21->CheckEdit5=750.; //Correction 9/09/2011
//				Form21->CheckEdit6=1500.;
//				Form21->CheckEdit7=1500.;
			   }
		  wms.Ti[i]=Form21->CheckEdit5; //This assumes that 750/1500/1500 have been loaded into Form21
		  wms.Ta[i]=Form21->CheckEdit6;
		  wms.Tm[i]=Form21->CheckEdit7;

//VFT convention: wms.ved= -1 is ON;=0 is OFF
		  if(Form21->CheckGroupBox4)wms.ved[i]= -1; //Correction#2 EFP 3/15/2012
//		  if(!Form21->CheckGroupBox4)wms.ved[i]= -1;//Correction  EFP 9/09/2011
		  else wms.ved[i]=0;

// No coding yet for bool wms.switc/wms.hetjd/wms.Steps EFP 4/01/2010
		  wms.name[i]=Form21->CheckEdit1;
/////////// trash
//honk<<wms.name[i].c_str()<<" MatProperties_public\n";
/////////////////
		  wms.matFileName[i]=Form21->CheckEdit8;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
		  delete Form21; //Created with Show()
		 }
 else {
 wms.cond[wms.nMatPropSet]=Form21->CheckEdit2;
 wms.heat[wms.nMatPropSet]=Form21->CheckEdit3;
 wms.den[wms.nMatPropSet]=Form21->CheckEdit4;
 if(ip==0){wms.mcr[wms.nMatPropSet]=0;
		   if(Form21->CheckCheckBox1){wms.hetjd[wms.nMatPropSet]= -1;wms.switc[wms.nMatPropSet]=true;
//												wms.nprops[wms.nMatPropSet]=wms.nprops[wms.nMatPropSet]+1;
												wms.Steps[wms.nMatPropSet]=Form21->CheckEdit5a;
											   }
		   else {wms.hetjd[wms.nMatPropSet]=0;
				 wms.switc[wms.nMatPropSet]=false;
				}
		  }
 else if(ip==1){wms.mcr[wms.nMatPropSet]=2;wms.hetjd[wms.nMatPropSet]=0;wms.switc[wms.nMatPropSet]=false;}
 else if(ip==2){wms.mcr[wms.nMatPropSet]=5;wms.hetjd[wms.nMatPropSet]=0;wms.switc[wms.nMatPropSet]=false;}
 else if(ip==3){wms.mcr[wms.nMatPropSet]=1;wms.hetjd[wms.nMatPropSet]=0;wms.switc[wms.nMatPropSet]=false;}
 else if(ip==4){wms.mcr[wms.nMatPropSet]=3;
//                wms.hetjd[wms.nMatPropSet]=0;wms.switc[wms.nMatPropSet]=false;
		   if(Form21->CheckCheckBox1){wms.hetjd[wms.nMatPropSet]= -1;wms.switc[wms.nMatPropSet]=true;
//												wms.nprops[wms.nMatPropSet]=wms.nprops[wms.nMatPropSet]+1;
												wms.Steps[wms.nMatPropSet]=Form21->CheckEdit5a;
											   }
		   else {wms.hetjd[wms.nMatPropSet]=0;
				 wms.switc[wms.nMatPropSet]=false;
				}
			   }
 else          {wms.mcr[wms.nMatPropSet]=6;wms.hetjd[wms.nMatPropSet]=0;wms.switc[wms.nMatPropSet]=false;} // Correction BBrust 3/25/2011

//VFT convention: wms.annjd[]= -1 is ON (read & use Ti/Ta/Tm);=0 is OFF (use pre-loaded 750/1500/1500)
 if(Form21->CheckGroupBox3)wms.annjd[wms.nMatPropSet]= -1;
// if(!Form21->CheckGroupBox3)wms.annjd[wms.nMatPropSet]= -1;//Correction  EFP 9/09/2011
 else {wms.annjd[wms.nMatPropSet]=0;      //Correction#2 EFP 3/15/2012
//	   Form21->CheckEdit5=750.; //Correction  EFP 9/09/2011
//	   Form21->CheckEdit6=1500.;
//	   Form21->CheckEdit7=1500.;
	  }
 wms.Ti[wms.nMatPropSet]=Form21->CheckEdit5;
 wms.Ta[wms.nMatPropSet]=Form21->CheckEdit6;
 wms.Tm[wms.nMatPropSet]=Form21->CheckEdit7;

//VFT convention: wms.ved= -1 is ON;=0 is OFF
 if(Form21->CheckGroupBox4)wms.ved[wms.nMatPropSet]= -1;   //Correction#2 EFP 3/15/2012
// if(!Form21->CheckGroupBox4)wms.ved[wms.nMatPropSet]= -1;//Correction  EFP 9/09/2011
 else wms.ved[wms.nMatPropSet]=0;
// No coding yet for bool wms.switc/wms.hetjd/wms.Steps EFP 4/01/2010
//honk<<Form21->CheckEdit1.c_str()<<" MatPropSet name\n";
//honk<<Form21->CheckEdit8.c_str()<<" MechPropSet name\n";

if(Form21->CheckEdit1==L"****")
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter material property set name",L"Repeat",MB_OK);
   missing++;
  }
else wms.name[wms.nMatPropSet]=Form21->CheckEdit1;
if(Form21->CheckEdit8==L"****")
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter mechanical property file name",L"Repeat",MB_OK);
   missing++;
  }
else wms.matFileName[wms.nMatPropSet]=Form21->CheckEdit8;
 if(!missing){
//CreateaNewFulllinWeldPass1->Enabled=true;
//CreateaNewLinearWeldPass1->Enabled=true;
////WeldPassEditingandSequencing1=0;
////ShowWeldSequence1=0;
			  wms.nMatPropSet=wms.nMatPropSet+1;
			  wms.highest=wms.highest+1;
			 }
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
	   Form21->Close(); //Created with ShowModal()
	  }
 Form21=NULL;
}
//---------------------------------------------------------------------------
void TForm1::EditMatPropProg(int VFTitemindex)
{
 wp.PRECORD=VFTitemindex;
//honk<<wp.PRECORD<<" Enter EditMatProp()\n";
// TBD: Fix issue with wms.hetjd/nprops/Steps/switc
 Form21=new TForm21(1,this);
 Form21->Caption="Edit a Material Property Set";
 Form21->Label1->Caption="Material Property Set Name";
 Form21->Label2->Caption="Thermal Conductivity:";
 Form21->Label3->Caption="Specific Heat:";
 Form21->Label4->Caption="Density:";
 Form21->Label6->Caption="Steps for Creep Input (comma separated)";
 Form21->CheckBox1->Caption="Switch between Isotropic/Creep";
 Form21->Label7->Caption="Annealing Initiation Temperature";
 Form21->Label8->Caption="Annealing Temperature";
 Form21->Label9->Caption="Material Melting Temperature";
 Form21->Label10->Caption="Mechanical Property File Path/Name:";
 Form21->RadioButton1->Caption="Isotropic (multi-linear)";
 Form21->RadioButton2->Caption="Combined Hardening (multi-linear)";
 Form21->RadioButton3->Caption="Simple Phase Transformation";
 Form21->RadioButton4->Caption="Combined Hardening (linear)";
 Form21->RadioButton5->Caption="Elastic-Plastic-Creep(Isotropic,multi-linear)";
 Form21->RadioButton6->Caption="Complete Phase Transformation";
 Form21->RadioButton9->Caption="No";
 Form21->RadioButton10->Caption="Yes";
 Form21->RadioButton7->Caption="No";
 Form21->RadioButton8->Caption="Yes";
 Form21->Button1->Caption="Choose Mechanical Property File...";
 Form21->Button2->Caption="Enter";
 Form21->Button3->Caption="Restore";
 Form21->GroupBox1->Caption="Physical Properties";
 Form21->RadioGroup1->Caption="Material Constitutive Relation (Double-click Isotropic to get Iso/creep)";
 Form21->Label5->Caption="ABAQUS Material Inputs";
 Form21->GroupBox2->Caption="Annealing";
 Form21->GroupBox3->Caption="VED";
 Form21->GroupBox4->Caption="Mechanical Properties";
// Form21->CheckCheckBox1=false;
// Form21->CheckEdit5a=L"0";

 wms.temp_name=wms.name[wp.PRECORD];
 wms.temp_cond=wms.cond[wp.PRECORD];
 wms.temp_heat=wms.heat[wp.PRECORD];
 wms.temp_den=wms.den[wp.PRECORD];
 wms.temp_Ti=wms.Ti[wp.PRECORD];
 wms.temp_Ti=wms.Ta[wp.PRECORD];
 wms.temp_Ti=wms.Tm[wp.PRECORD];
 wms.temp_matFileName=wms.matFileName[wp.PRECORD];
 wms.temp_mcr=wms.mcr[wp.PRECORD];
 wms.temp_annjd=wms.annjd[wp.PRECORD];
 wms.temp_ved=wms.ved[wp.PRECORD];
///////////////// EFP 9/23/2011
 wms.temp_hetjd=wms.hetjd[wp.PRECORD];
 wms.temp_nprops=wms.nprops[wp.PRECORD];
 wms.temp_Steps=wms.Steps[wp.PRECORD];
 wms.temp_switc=wms.switc[wp.PRECORD];
/////////////////

// Form21->CheckEdit1=wms.name[wp.PRECORD];
// Form21->CheckEdit1=wms.name[wp.PRECORD].c_str();
 Form21->CheckEdit1=wms.name[wp.PRECORD].w_str();
 Form21->CheckEdit2=wms.cond[wp.PRECORD];
 Form21->CheckEdit3=wms.heat[wp.PRECORD];
 Form21->CheckEdit4=wms.den[wp.PRECORD];
 Form21->CheckEdit5=wms.Ti[wp.PRECORD];
 Form21->CheckEdit6=wms.Ta[wp.PRECORD];
 Form21->CheckEdit7=wms.Tm[wp.PRECORD];

// if(wms.nMatPropSet>0)Form21->CheckEdit8=wms.matFileName[wp.PRECORD].c_str();
 if(wms.nMatPropSet>0)Form21->CheckEdit8=wms.matFileName[wp.PRECORD].w_str();
 else Form21->CheckEdit8=L"****";

 if(wms.mcr[wp.PRECORD]==0)Form21->CheckRadioGroup1=0;
// else if(wms.mcr[wp.PRECORD]==1)Form21->CheckRadioGroup1=1;
// else if(wms.mcr[wp.PRECORD]==2)Form21->CheckRadioGroup1=2;
// else if(wms.mcr[wp.PRECORD]==3)Form21->CheckRadioGroup1=3;
// else if(wms.mcr[wp.PRECORD]==5)Form21->CheckRadioGroup1=4;
 else if(wms.mcr[wp.PRECORD]==1)Form21->CheckRadioGroup1=3; //Correction BBrust 3/25/2011
 else if(wms.mcr[wp.PRECORD]==2)Form21->CheckRadioGroup1=1;
 else if(wms.mcr[wp.PRECORD]==3)Form21->CheckRadioGroup1=4;
 else if(wms.mcr[wp.PRECORD]==5)Form21->CheckRadioGroup1=2;
 else Form21->CheckRadioGroup1=5;

// if(wms.annjd[wp.PRECORD]==0)Form21->CheckGroupBox4=false;
// else Form21->CheckGroupBox4=true;
// if(wms.ved[wp.PRECORD]==0)Form21->CheckGroupBox3=false;
// else Form21->CheckGroupBox3=true;

// if(wms.annjd[wp.PRECORD]==0)Form21->CheckGroupBox3=true;//Correction  EFP 9/09/2011
// else Form21->CheckGroupBox3=false;
// if(wms.ved[wp.PRECORD]==0)Form21->CheckGroupBox4=true;
// else Form21->CheckGroupBox4=false;
 if(wms.annjd[wp.PRECORD]==0)Form21->CheckGroupBox3=false;//This seems to be correct  EFP 8/31/2012
 else Form21->CheckGroupBox3=true;
 if(wms.ved[wp.PRECORD]==0)Form21->CheckGroupBox4=false;
 else Form21->CheckGroupBox4=true;

 Form21->CheckCheckBox1=wms.switc[wp.PRECORD];
// Form21->CheckEdit5a=wms.Steps[wp.PRECORD]; //Should be as below???
 Form21->CheckEdit5a=wms.Steps[wp.PRECORD].w_str();

// Form21->Label6->Enabled=true;
// Form21->CheckBox1->Enabled=false;
// Form21->Edit5->Enabled=false;
// Form21->Label6->Visible=false;
// Form21->CheckBox1->Visible=false;
// Form21->Edit5->Visible=false;

	   if(wms.switc[wp.PRECORD]){//EFP 9/23/2011
	   Form21->Label6->Enabled=true;
	   Form21->CheckBox1->Enabled=true;
	   Form21->Edit5->Enabled=true;
	   Form21->Label6->Visible=true;
	   Form21->CheckBox1->Visible=true;
	   Form21->Edit5->Visible=true;
	   Form21->Button4->Enabled=true;
	   Form21->Button4->Visible=true;
 Form21->RadioButton2->Enabled=false;
 Form21->RadioButton3->Enabled=false;
 Form21->RadioButton4->Enabled=false;
 Form21->RadioButton5->Enabled=false;
 Form21->RadioButton6->Enabled=false;
 Form21->RadioButton2->Visible=false;
 Form21->RadioButton3->Visible=false;
 Form21->RadioButton4->Visible=false;
 Form21->RadioButton5->Visible=false;
 Form21->RadioButton6->Visible=false;
									 }
	   else {
	   Form21->Label6->Enabled=true;
	   Form21->CheckBox1->Enabled=true;
	   Form21->Edit5->Enabled=false;
	   Form21->Label6->Visible=false;
	   Form21->CheckBox1->Visible=false;
	   Form21->Edit5->Visible=false;
	   Form21->Button4->Enabled=false;
	   Form21->Button4->Visible=false;
 Form21->RadioButton2->Enabled=true;
 Form21->RadioButton3->Enabled=true;
 Form21->RadioButton4->Enabled=true;
 Form21->RadioButton5->Enabled=true;
 Form21->RadioButton6->Enabled=true;
 Form21->RadioButton2->Visible=true;
 Form21->RadioButton3->Visible=true;
 Form21->RadioButton4->Visible=true;
 Form21->RadioButton5->Visible=true;
 Form21->RadioButton6->Visible=true;
			}

 Form21->Show();
}
//---------------------------------------------------------------------------
void TForm1::DelMatPropProg(int VFTitemindex)
{long i=0,dum=0;
// float *SWTcurr=NULL,*SWTvolt=NULL,*SWTeff=NULL,*SWTspeed=NULL;
// String *SWTname=NULL;

 int isw=0,*SWMTmcr=NULL,*SWMTannjd=NULL,*SWMTved=NULL,*SWMThetjd=NULL,*SWMTnprops=NULL;
 float *SWMTcond=NULL,*SWMTheat=NULL,*SWMTden=NULL,*SWMTTi=NULL,*SWMTTa=NULL,*SWMTTm=NULL;
 String *SWMTname=NULL,*SWMTmatFileName=NULL,*SWMTSteps=NULL;
 bool *SWMTswitc=NULL;
//honk<<VFTitemindex<<" DelMatPropSet\n";

 isw=1; // MaterialPropertyName test  EFP 4/15/2011
 if(wp.nWeldPass)
   {for(i=0;i<wp.nWeldPass;i++)
	  {if(wp.matName[i]==wms.name[VFTitemindex]){
////												 honk<<i<<" "<<VFTitemindex<<" "<<wms.name[VFTitemindex].t_str()<<" Inadmissible MatPropNames\n";
//												 honk<<i<<" "<<VFTitemindex<<" "<<wms.name[VFTitemindex].c_str()<<" Inadmissible MatPropNames\n";
//xxxxxxxxxx
 int bufferSize=WideCharToMultiByte(CP_UTF8,0,wms.name[VFTitemindex].w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,wms.name[VFTitemindex].w_str(), -1,m,bufferSize,NULL,NULL);
 honk<<i<<" "<<VFTitemindex<<" "<<m<<" Inadmissible MatPropNames\n";
 delete[] m;
//xxxxxxxxxx
												 isw=0;break;
												}
	  }
   }
 if(isw){
 if(wms.nMatPropSet>1)
   {
	SWMTmcr=new int[wms.nMatPropSet-1];
	SWMTannjd=new int[wms.nMatPropSet-1];
	SWMTved=new int[wms.nMatPropSet-1];
	SWMThetjd=new int[wms.nMatPropSet-1];
	SWMTnprops=new int[wms.nMatPropSet-1];
	SWMTcond=new float[wms.nMatPropSet-1];
	SWMTheat=new float[wms.nMatPropSet-1];
	SWMTden=new float[wms.nMatPropSet-1];
	SWMTTi=new float[wms.nMatPropSet-1];
	SWMTTa=new float[wms.nMatPropSet-1];
	SWMTTm=new float[wms.nMatPropSet-1];
	SWMTname=new String[wms.nMatPropSet-1];
	SWMTmatFileName=new String[wms.nMatPropSet-1];
	SWMTSteps=new String[wms.nMatPropSet-1];
	SWMTswitc=new bool[wms.nMatPropSet-1];

	if(VFTitemindex){
					 for(i=0;i<VFTitemindex;i++)SWMTmcr[i]=wms.mcr[i];
					 for(i=0;i<VFTitemindex;i++)SWMTannjd[i]=wms.annjd[i];
					 for(i=0;i<VFTitemindex;i++)SWMTved[i]=wms.ved[i];
					 for(i=0;i<VFTitemindex;i++)SWMThetjd[i]=wms.hetjd[i];
					 for(i=0;i<VFTitemindex;i++)SWMTnprops[i]=wms.nprops[i];
					 for(i=0;i<VFTitemindex;i++)SWMTcond[i]=wms.cond[i];
					 for(i=0;i<VFTitemindex;i++)SWMTheat[i]=wms.heat[i];
					 for(i=0;i<VFTitemindex;i++)SWMTden[i]=wms.den[i];
					 for(i=0;i<VFTitemindex;i++)SWMTTi[i]=wms.Ti[i];
					 for(i=0;i<VFTitemindex;i++)SWMTTa[i]=wms.Ta[i];
					 for(i=0;i<VFTitemindex;i++)SWMTTm[i]=wms.Tm[i];
					 for(i=0;i<VFTitemindex;i++)SWMTname[i]=wms.name[i];
					 for(i=0;i<VFTitemindex;i++)SWMTmatFileName[i]=wms.matFileName[i];
					 for(i=0;i<VFTitemindex;i++)SWMTSteps[i]=wms.Steps[i];
					 for(i=0;i<VFTitemindex;i++)SWMTswitc[i]=wms.switc[i];
					}
	if(VFTitemindex+1<wms.nMatPropSet){
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTmcr[i-1]=wms.mcr[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTannjd[i-1]=wms.annjd[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTved[i-1]=wms.ved[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMThetjd[i-1]=wms.hetjd[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTnprops[i-1]=wms.nprops[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTcond[i-1]=wms.cond[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTheat[i-1]=wms.heat[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTden[i-1]=wms.den[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTTi[i-1]=wms.Ti[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTTa[i-1]=wms.Ta[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTTm[i-1]=wms.Tm[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTname[i-1]=wms.name[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTmatFileName[i-1]=wms.matFileName[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTSteps[i-1]=wms.Steps[i];
	for(i=VFTitemindex+1;i<wms.nMatPropSet;i++)SWMTswitc[i-1]=wms.switc[i];
									  }
	FDdynmem_manage(-10,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	wms.nMatPropSet=wms.nMatPropSet-1;
	FDdynmem_manage( 10,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	for(i=0;i<wms.nMatPropSet;i++)wms.mcr[i]=SWMTmcr[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.annjd[i]=SWMTannjd[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.ved[i]=SWMTved[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.hetjd[i]=SWMThetjd[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.nprops[i]=SWMTnprops[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.cond[i]=SWMTcond[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.heat[i]=SWMTheat[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.den[i]=SWMTden[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.Ti[i]=SWMTTi[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.Ta[i]=SWMTTa[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.Tm[i]=SWMTTm[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.name[i]=SWMTname[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.matFileName[i]=SWMTmatFileName[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.Steps[i]=SWMTSteps[i];
	for(i=0;i<wms.nMatPropSet;i++)wms.switc[i]=SWMTswitc[i];

	delete [] SWMTmcr;delete [] SWMTannjd;delete [] SWMTved;delete [] SWMThetjd;delete [] SWMTnprops;
	delete [] SWMTcond;delete [] SWMTheat;delete [] SWMTden;
	delete [] SWMTTi;delete [] SWMTTa;delete [] SWMTTm;
	delete [] SWMTname;delete [] SWMTmatFileName;delete [] SWMTSteps;delete [] SWMTswitc;
   }
 else if(wms.nMatPropSet==1)
   {FDdynmem_manage(-10,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	wms.nMatPropSet=0;
	extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Material property sets no longer in the model",L"Warning",MB_OK);
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Inadmissible nMatPropSet in DelWeldParam()",L"Terminate",MB_OK);exit(0);}
		}
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"A weld pass uses this Material Property Set ",L"Halt: Can not be deleted",MB_OK);}
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
}
//---------------------------------------------------------------------------
void TForm1::EditMatProp_public()
{int isel=Form21->CheckISEL;long i=wp.PRECORD;
 if(isel)
   {wms.mcr[i]=wms.temp_mcr;
	wms.annjd[i]=wms.temp_annjd;
	wms.ved[i]=wms.temp_ved;
	wms.hetjd[i]=wms.temp_hetjd;
	wms.nprops[i]=wms.temp_nprops;
	wms.cond[i]=wms.temp_cond;
	wms.heat[i]=wms.temp_heat;
	wms.den[i]=wms.temp_den;
	wms.Ti[i]=wms.temp_Ti;
	wms.Ta[i]=wms.temp_Ta;
	wms.Tm[i]=wms.temp_Tm;
	wms.name[i]=wms.temp_name;
	wms.matFileName[i]=wms.temp_matFileName;
	wms.Steps[i]=wms.temp_Steps;
	wms.switc[i]=wms.temp_switc;
	FD_LButtonstatus=11;stateVFT=2;
	delete Form21;
//	Form21=NULL;// because it was created with Show()
   }
 else Form21->Close();
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 Form21=NULL;
}
//---------------------------------------------------------------------------
void TForm1::EditWeldPassProg(int VFTitemindex)
{long j=0,
//i=0,is=0,isw=0,
ir=0
//cur=0,prod=1,aflag=0,nwid=0,Sverige[MAX_GID],GIDspac=1+(wp.nWeldPass-1)/16,
//val[(MAX_GID+1)/16],
//	  ie=0,
//	  ,eltype=0,bscode=0,node=0,ieGID=0,t3=1000
	  ;
// wchar_t string0[40],curMess0[]=L"weld",string1[11];  //TEST FOR EXISTING FORM
		  if(CreateLinWeldPass==NULL)
			  {if(iPaintyesno/10)
												 {
 wp.CreateWPassMode=0;

		 wp.PRECORD=VFTitemindex;
//		 isw=wp.EDIT_SEL; //Shameful but not used?????  EFP 11/06/2011




///////////
wp.temp_name=wp.name[wp.PRECORD];
wp.temp_matName=wp.matName[wp.PRECORD];
wp.temp_seqNum=wp.seqNum[wp.PRECORD];
wp.temp_boolFlags=wp.boolFlags[wp.PRECORD];
wp.temp_type=wp.type[wp.PRECORD];
wp.temp_shape=wp.shape[wp.PRECORD];
wp.temp_stepInterval=wp.stepInterval[wp.PRECORD];
wp.temp_hp=wp.hp[wp.PRECORD];
wp.temp_nsegs=wp.nsegs[wp.PRECORD];
wp.temp_iselect=wp.iselect[wp.PRECORD];
wp.temp_stpEle=wp.stpEle[wp.PRECORD];
wp.temp_curr=wp.curr[wp.PRECORD];
wp.temp_volt=wp.volt[wp.PRECORD];
wp.temp_eff=wp.eff[wp.PRECORD];
wp.temp_speed=wp.speed[wp.PRECORD];
wp.temp_troom=wp.troom[wp.PRECORD];
wp.temp_tmelt=wp.tmelt[wp.PRECORD];
wp.temp_tcutl=wp.tcutl[wp.PRECORD];
wp.temp_tcuth=wp.tcuth[wp.PRECORD];
wp.temp_timeInterval=wp.timeInterval[wp.PRECORD];
wp.temp_maxiHeatStep=wp.maxiHeatStep[wp.PRECORD];
wp.temp_miniHeatStep=wp.miniHeatStep[wp.PRECORD];
wp.temp_thk1=wp.thk1[wp.PRECORD];
wp.temp_thk2=wp.thk2[wp.PRECORD];
wp.temp_lstart=wp.lstart[wp.PRECORD];
wp.temp_lend=wp.lend[wp.PRECORD];
wp.temp_n_curr_sttEl=wp.n_curr_sttEl[wp.PRECORD];
wp.temp_reset=wp.reset[wp.PRECORD];
wp.temp_wpTimes=wp.wpTimes[wp.PRECORD];
wp.temp_source=wp.source[wp.PRECORD];
wp.temp_mcr=wp.mcr[wp.PRECORD];
wp.temp_WeldColor=wp.WeldColor[wp.PRECORD]; // Policy: Reserve one extra nWeldPass and copy "to-be-edited" data there.
wp.temp_prevGID=wp.prevGID[wp.PRECORD];
wp.temp_util_arr=wp.util_arr[wp.PRECORD];
//  CAUTION: Not all variables are backed-up... TBD: Comprehensive coding  EFP 4/03/2011
wp.temp_eles=new long[wp.nWeldPass*wp.memWGa];wp.temp_sttEles=new long[wp.nWeldPass*wp.memWGa];wp.temp_sttEleNodes=new long[wp.nWeldPass*wp.memWGa*4];
for(j=0;j<wp.nWeldPass*wp.memWGa;j++)wp.temp_eles[j]=wp.eles[j];
for(j=0;j<wp.nWeldPass*wp.memWGa;j++)wp.temp_sttEles[j]=wp.sttEles[j];
for(j=0;j<wp.nWeldPass*wp.memWGa*4;j++)wp.temp_sttEleNodes[j]=wp.sttEleNodes[j];
//
// The following 2 lines are no longer necessary
for(j=0;j<wp.memWGa;j++)wp.eles[wp.nWeldPass*wp.memWGa+j]=wp.eles[wp.PRECORD*wp.memWGa+j];
for(j=0;j<wp.memWGa;j++)wp.sttEles[wp.nWeldPass*wp.memWGa+j]=wp.sttEles[wp.PRECORD*wp.memWGa+j];

for(j=0;j<wp.memWGa;j++)wp.firstEle[wp.nWeldPass*wp.memWGa+j]=wp.firstEle[wp.PRECORD*wp.memWGa+j];
for(j=0;j<wp.memWGa;j++)wp.nextEle[wp.nWeldPass*wp.memWGa+j]=wp.nextEle[wp.PRECORD*wp.memWGa+j];
for(j=0;j<4;j++)wp.snorm1[wp.nWeldPass*4+j]=wp.snorm1[wp.PRECORD*4+j];
for(j=0;j<4;j++)wp.snorm2[wp.nWeldPass*4+j]=wp.snorm2[wp.PRECORD*4+j];
for(j=0;j<3;j++)wp.circEles[wp.nWeldPass*3+j]=wp.circEles[wp.PRECORD*3+j]; //Policy???
for(j=0;j<wp.memWGa;j++)wp.edgeEles[wp.nWeldPass*wp.memWGa+j]=wp.edgeEles[wp.PRECORD*wp.memWGa+j];
for(j=0;j<4;j++)wp.edgeNodes[wp.nWeldPass*4+j]=wp.edgeNodes[wp.PRECORD*4+j]; // correct???
// The following line is no longer necessary
for(j=0;j<4*wp.memWGa;j++)wp.sttEleNodes[wp.nWeldPass*wp.memWGa*4+j]=wp.sttEleNodes[wp.PRECORD*wp.memWGa*4+j];

for(j=0;j<wp.memWGa;j++)wp.hlightel[wp.nWeldPass*wp.memWGa+j]=wp.hlightel[wp.PRECORD*wp.memWGa+j];
for(j=0;j<3*NDF;j++)wp.arrows[3*NDF*wp.nWeldPass+j]=wp.arrows[3*NDF*wp.PRECORD+j];





// Policy: Do erase any data at this point (NEW POLICY: DO NOT ERASE  EFP 3/30/2012)
												  VFTon=1;
												  wp.count_curr_sttEl=wp.n_curr_sttEl[wp.PRECORD];//Correction EFP 3/30/2012
///////////////////////////////////////////////////////////////////////
												  FD_LButtonstatus=17;
//												  iPersistVFT=1;
												  ir=iPersistVFT/10;
												  iPersistVFT=10*ir+1;

/////////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
///////////

												  CreateLinWeldPass=new TForm15(4,wp.PRECORD+1,wp.seqNum[wp.PRECORD],0,0,
																				wp.snorm1[wp.PRECORD*4+0],wp.snorm2[wp.PRECORD*4+0],
																				wms.nMatPropSet,wms.name,
																				wps.nWeldParamSet,wps.name,
																				this); // (weldpass#,weldseq#,...)

CreateLinWeldPass->CheckBox1->Enabled=false;//Disconnect weld direction CheckBox1 EFP 4/03/2011

//CreateLinWeldPass->CheckEdit1=wp.name[wp.PRECORD];
//CreateLinWeldPass->CheckEdit1=wp.name[wp.PRECORD].c_str();
CreateLinWeldPass->CheckEdit1=wp.name[wp.PRECORD].w_str();


CreateLinWeldPass->Caption=L"Edit weld pass";
CreateLinWeldPass->Label3->Caption=L"All start elements";
CreateLinWeldPass->Label4->Caption=L"One stop element";



CreateLinWeldPass->Label7->Caption=L"Plate1 normal";
CreateLinWeldPass->Label8->Caption=L"Plate2 normal";
CreateLinWeldPass->Button1->Caption=L"Enter";
CreateLinWeldPass->Button2->Caption=L"Restore";
CreateLinWeldPass->Button3->Caption=L"Cancel";
// _ltow(wp.nWeldPass+1,string1,10);StringCchCatW(curMess0,11,string1);
// CreateLinWeldPass->CheckEdit1=curMess0;

///////////////////// EFP 1/03/2011
CreateLinWeldPass->CheckFunction=4; // Note that 4 will be replaced by user-selected option 4/5/6/7
CreateLinWeldPass->RadioGroup1->Caption=L"Change weld pass to";
CreateLinWeldPass->RadioGroup1->Enabled=True;
CreateLinWeldPass->RadioGroup1->Visible=True;
CreateLinWeldPass->PageControl1->TabIndex=2;
/////////////////////


//CreateLinWeldPass->CheckEdit9=0.;
//CreateLinWeldPass->CheckEdit10=0.;
//CreateLinWeldPass->CheckEdit11=0.;
//CreateLinWeldPass->CheckEdit12=0.;
// NEW POLICY: DO NOT ERASE EDITED WELD DATA BEFOREHAND  EFP 3/30/2012
CreateLinWeldPass->CheckEdit9=wp.curr[wp.PRECORD]; // Need to test this
CreateLinWeldPass->CheckEdit10=wp.volt[wp.PRECORD];
CreateLinWeldPass->CheckEdit11=wp.eff[wp.PRECORD];
CreateLinWeldPass->CheckEdit12=wp.speed[wp.PRECORD];



/////////////// i has no meaning in the following so change to glWPRECORD  EFP 1/07/2011
//CreateLinWeldPass->CheckEdit13=wp.troom[i];
//CreateLinWeldPass->CheckEdit14=wp.tmelt[i];
//CreateLinWeldPass->CheckEdit15=wp.tcutl[i];
//CreateLinWeldPass->CheckEdit16=wp.tcuth[i];
//CreateLinWeldPass->CheckEdit17=wp.timeInterval[i]; // Interpass cooling time???
//CreateLinWeldPass->CheckEdit19=wp.stepInterval[i]; // Interpass cooling steps???
//CreateLinWeldPass->CheckEdit18=wp.maxiHeatStep[i]; // Max heating time(output)???
//CreateLinWeldPass->CheckEdit20=wp.miniHeatStep[i]; // Min heating time(output)???
//CreateLinWeldPass->CheckEdit21=wp.thk1[i]; // Plate1 thickness???
//CreateLinWeldPass->CheckEdit22=wp.thk2[i]; // Plate2 thickness???
////CreateLinWeldPass->CheckEditXXX=wp.hp[i]; // Moving Arc only (hp=0) at present (and this must be selected by TForm15 RadioButton5&6)
//CreateLinWeldPass->CheckEdit23=wp.nsegs[i]; // Lumped pass heating procedure: Number of lumped segments
//CreateLinWeldPass->CheckEdit24=1; // Near-edge effect: Number of required edges???
////CreateLinWeldPass->ComboBox1->ItemIndex = 0;
////CreateLinWeldPass->ComboBox2->ItemIndex = 0;
//CreateLinWeldPass->CheckType=wp.type[i];
//CreateLinWeldPass->CheckShape=wp.shape[i];
//CreateLinWeldPass->CheckWeldColor=wp.WeldColor[i];

CreateLinWeldPass->CheckEdit13=wp.troom[wp.PRECORD];
CreateLinWeldPass->CheckEdit14=wp.tmelt[wp.PRECORD];
CreateLinWeldPass->CheckEdit15=wp.tcutl[wp.PRECORD];
CreateLinWeldPass->CheckEdit16=wp.tcuth[wp.PRECORD];
CreateLinWeldPass->CheckEdit17=wp.timeInterval[wp.PRECORD]; // Interpass cooling time???
CreateLinWeldPass->CheckEdit19=wp.stepInterval[wp.PRECORD]; // Interpass cooling steps???
CreateLinWeldPass->CheckEdit18=wp.maxiHeatStep[wp.PRECORD]; // Max heating time(output)???
CreateLinWeldPass->CheckEdit20=wp.miniHeatStep[wp.PRECORD]; // Min heating time(output)???
CreateLinWeldPass->CheckEdit21=wp.thk1[wp.PRECORD]; // Plate1 thickness???
CreateLinWeldPass->CheckEdit22=wp.thk2[wp.PRECORD]; // Plate2 thickness???
//CreateLinWeldPass->CheckEditXXX=wp.hp[wp.PRECORD]; // Moving Arc only (hp=0) at present (and this must be selected by TForm15 RadioButton5&6)
CreateLinWeldPass->CheckEdit23=wp.nsegs[wp.PRECORD]; // Lumped pass heating procedure: Number of lumped segments
CreateLinWeldPass->CheckEdit24=1; // Near-edge effect: Number of required edges???
//CreateLinWeldPass->ComboBox1->ItemIndex = 0;
//CreateLinWeldPass->ComboBox2->ItemIndex = 0;
CreateLinWeldPass->CheckType=wp.type[wp.PRECORD];
CreateLinWeldPass->CheckShape=wp.shape[wp.PRECORD];
/////////////
//		  ir=wp.nWeldPass-9*(wp.nWeldPass/9);
//		  if     (ir==0)CreateLinWeldPass->CheckWeldColor=clRed;
//		  else if(ir==1)CreateLinWeldPass->CheckWeldColor=clLtGray; // Coding for color contrast
//		  else if(ir==2)CreateLinWeldPass->CheckWeldColor=clBlue;
//		  else if(ir==3)CreateLinWeldPass->CheckWeldColor=clGreen;
//		  else if(ir==4)CreateLinWeldPass->CheckWeldColor=clLime;
//		  else if(ir==5)CreateLinWeldPass->CheckWeldColor=clNavy;
//		  else if(ir==6)CreateLinWeldPass->CheckWeldColor=clYellow;
//		  else if(ir==7)CreateLinWeldPass->CheckWeldColor=clOlive;
//		  else          CreateLinWeldPass->CheckWeldColor=clMaroon;
CreateLinWeldPass->CheckWeldColor=wp.WeldColor[wp.PRECORD];

CreateLinWeldPass->Edit2->Enabled=false;






/////////////
//CreateLinWeldPass->PageControl1->ActivePage=0;
//												  CreateLinWeldPass->ShowModal();
												  CreateLinWeldPass->Show();
//												  delete CreateLinWeldPass;CreateLinWeldPass=NULL;
												 }
//				  else {
//		//				ltoa(MAX_WELDPASSES,string0,10);
//		//				strcat(string0,currMess);
//		//				extern PACKAGE void __fastcall Beep(void);
//		//				Application->MessageBox(string0,L"Halt",MB_OK);
//
//						extern PACKAGE void __fastcall Beep(void);
//						Application->MessageBox(_ltow(MAX_WELDPASSES,string0,10),L"Halt: MAX_WELDPASSES exceeded",MB_OK);
//					   }
//				 }
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Use VIEW/PAINT to paint",L"Halt",MB_OK);}
			  }
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Weld pass creation/edit dialog already exists",L"Notice",MB_OK);}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//		}
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must check at least one weld pass to edit.",L"Repeat",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::wpCreate_public(){
int missing=0;//long j=0;
if(CreateLinWeldPass->CheckEdit1==L"****")
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter weld pass name",L"Repeat",MB_OK);
   missing++;
  }
else wp.name[wp.nWeldPass]=CreateLinWeldPass->CheckEdit1;
wp.seqNum[wp.nWeldPass]=CreateLinWeldPass->CheckEdit2; // User-input disabled
wp.troom[wp.nWeldPass]=CreateLinWeldPass->CheckEdit13;
wp.tmelt[wp.nWeldPass]=CreateLinWeldPass->CheckEdit14;
wp.tcutl[wp.nWeldPass]=CreateLinWeldPass->CheckEdit15;
wp.tcuth[wp.nWeldPass]=CreateLinWeldPass->CheckEdit16;
wp.type[wp.nWeldPass]=CreateLinWeldPass->CheckType;
wp.shape[wp.nWeldPass]=CreateLinWeldPass->CheckShape;
wp.timeInterval[wp.nWeldPass]=CreateLinWeldPass->CheckEdit17; // Interpass cooling time???
wp.stepInterval[wp.nWeldPass]=CreateLinWeldPass->CheckEdit19; // Interpass cooling steps???
wp.maxiHeatStep[wp.nWeldPass]=CreateLinWeldPass->CheckEdit18; // Max heating time(output)???
wp.miniHeatStep[wp.nWeldPass]=CreateLinWeldPass->CheckEdit20; // Min heating time(output)???
if(CreateLinWeldPass->CheckEdit21>0.0001f && CreateLinWeldPass->CheckEdit22>0.0001f)
  {wp.thk1[wp.nWeldPass]=CreateLinWeldPass->CheckEdit21; // Plate1 thickness???
   wp.thk2[wp.nWeldPass]=CreateLinWeldPass->CheckEdit22; // Plate2 thickness???
  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter plate thicknesses",L"Repeat",MB_OK);
	  missing++;
	 }
wp.hp[wp.nWeldPass]=0;// Moving arc only at present (selected by TForm15 RadioButton5&6)
wp.nsegs[wp.nWeldPass]=CreateLinWeldPass->CheckEdit23; // Lumped pass heating procedure: Number of lumped segments
if(CreateLinWeldPass->CheckMatName>=0){wp.matName[wp.nWeldPass]=wms.name[CreateLinWeldPass->CheckMatName];
									   wp.mcr[wp.nWeldPass]=wms.mcr[CreateLinWeldPass->CheckMatName];
									  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Material Property Set Name",L"Repeat",MB_OK);
	  missing++;
	 }
if(CreateLinWeldPass->CheckWeldParName>=0)
  {wp.curr[wp.nWeldPass]=wps.curr[CreateLinWeldPass->CheckWeldParName]; // Need to test this
   wp.volt[wp.nWeldPass]=wps.volt[CreateLinWeldPass->CheckWeldParName];
   wp.eff[wp.nWeldPass]=wps.eff[CreateLinWeldPass->CheckWeldParName];
   wp.speed[wp.nWeldPass]=wps.speed[CreateLinWeldPass->CheckWeldParName];
  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Weld Parameter Set Name",L"Repeat",MB_OK);
	  missing++;
	 }
							   if(!missing){
//WeldPassEditingandSequencing1->Enabled=true; //RETURN TO THIS
											wp.nWeldPass=wp.nWeldPass+1;
											wp.highest=wp.highest+1;
										   }
FD_LButtonstatus=11;stateVFT=2;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////

							   delete CreateLinWeldPass;CreateLinWeldPass=NULL;// because it was created with Show()?
							   iCullyesno=0;Invalidate();
							  }
//---------------------------------------------------------------------------
void TForm1::wpCreate1_public()
{CreateLinWeldPass->CheckEdit9=wps.curr[CreateLinWeldPass->CheckWeldParName];
 CreateLinWeldPass->CheckEdit10=wps.volt[CreateLinWeldPass->CheckWeldParName];
 CreateLinWeldPass->CheckEdit11=wps.eff[CreateLinWeldPass->CheckWeldParName];
 CreateLinWeldPass->CheckEdit12=wps.speed[CreateLinWeldPass->CheckWeldParName];
}
//---------------------------------------------------------------------------
void TForm1::wpCreate2_public()
{FD_LButtonstatus=11;stateVFT=2;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 delete CreateLinWeldPass;CreateLinWeldPass=NULL;// because it was created with Show()?
}
//---------------------------------------------------------------------------
void TForm1::wpEdit_public(){
int missing=0;long j=0,i=wp.PRECORD; //Shameful

if(CreateLinWeldPass->CheckEdit1==L"****")
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter weld pass name",L"Repeat",MB_OK);
   missing++;
  }
else wp.name[i]=CreateLinWeldPass->CheckEdit1;
delete [] wp.temp_eles;delete [] wp.temp_sttEles;delete [] wp.temp_sttEleNodes;
wp.seqNum[i]=CreateLinWeldPass->CheckEdit2;
wp.troom[i]=CreateLinWeldPass->CheckEdit13;
wp.tmelt[i]=CreateLinWeldPass->CheckEdit14;
wp.tcutl[i]=CreateLinWeldPass->CheckEdit15;
wp.tcuth[i]=CreateLinWeldPass->CheckEdit16;
wp.type[i]=CreateLinWeldPass->CheckType;
wp.shape[i]=CreateLinWeldPass->CheckShape;
wp.timeInterval[i]=CreateLinWeldPass->CheckEdit17; // Interpass cooling time???
wp.stepInterval[i]=CreateLinWeldPass->CheckEdit19; // Interpass cooling steps???
wp.maxiHeatStep[i]=CreateLinWeldPass->CheckEdit18; // Max heating time(output)???
wp.miniHeatStep[i]=CreateLinWeldPass->CheckEdit20; // Min heating time(output)???
if(CreateLinWeldPass->CheckEdit21>0.0001f && CreateLinWeldPass->CheckEdit22>0.0001f)
  {wp.thk1[i]=CreateLinWeldPass->CheckEdit21; // Plate1 thickness???
   wp.thk2[i]=CreateLinWeldPass->CheckEdit22; // Plate2 thickness???
  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to enter plate thicknesses",L"Repeat",MB_OK);
	  missing++;
	 }
wp.hp[i]=0;// Moving arc only at present (selected by TForm15 RadioButton5&6)
wp.nsegs[i]=CreateLinWeldPass->CheckEdit23; // Lumped pass heating procedure: Number of lumped segments
//wp.XXX[wp.nWeldPass]=CreateLinWeldPass->CheckEdit24; // Near-edge effect: Number of required edges??? (Which array should this be???)

//if(CreateLinWeldPass->CheckMatName>=0)wp.matName[wp.nWeldPass]=wms.matFileName[CreateLinWeldPass->CheckMatName]; // Need to test this INCORRECT
//else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Material Property Set Name",L"Repeat",MB_OK);
//	  missing++;
//	 }
if(CreateLinWeldPass->CheckMatName>=0){wp.matName[i]=wms.name[CreateLinWeldPass->CheckMatName];
									   wp.mcr[i]=wms.mcr[CreateLinWeldPass->CheckMatName];
									  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Material Property Set Name",L"Repeat",MB_OK);
	  missing++;
	 }
if(CreateLinWeldPass->CheckWeldParName>=0)
  {wp.curr[i]=wps.curr[CreateLinWeldPass->CheckWeldParName]; // Need to test this
   wp.volt[i]=wps.volt[CreateLinWeldPass->CheckWeldParName];
   wp.eff[i]=wps.eff[CreateLinWeldPass->CheckWeldParName];
   wp.speed[i]=wps.speed[CreateLinWeldPass->CheckWeldParName];
  }
else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Weld Parameter Set Name",L"Repeat",MB_OK);
	  missing++;
	 }



//////////////////////  EFP 5/24/2011
if(wp.snorm1[4*i+0]<0 && wp.snorm2[4*i+0]<0)
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Plate#1 & Plate#2 normals",L"Repeat",MB_OK);
   missing++;
  }
else if(wp.snorm1[4*i+0]<0)
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Plate#1 normal",L"Repeat",MB_OK);
   missing++;
  }
else if(wp.snorm2[4*i+0]<0)
  {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Forgot to select Plate#2 normal",L"Repeat",MB_OK);
   missing++;
  }
//////////////////////



//honk<<wp.matName[i].c_str()<<" edited WeldPassMatFileName\n";
/////////////////////////////////////////////
							   if(!missing){
//WeldPassEditingandSequencing1->Enabled=true;  //RETURN TO THIS
 for(j=0;j<wp.nWeldPass;j++)if(!wp.n_curr_sttEl[j])DelWeldPass(j,wp.memWGa);
										   }
//honk<<wp.nWeldPass<<" WeldPassEdited\n";

FD_LButtonstatus=11;stateVFT=2;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
							   delete CreateLinWeldPass;CreateLinWeldPass=NULL;// because it was created with Show()?
//iplotflag=1;iCircleplot=1;
//							   iCullyesno=0;

////////////////// start EFP 4/04/2011
								if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
//								else {FDcomp_nGID();FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);}
//								iPaintyesno=10;iCullyesno=0;

								else {
//								FDcomp_nGID();
FDcomp_nGID(indat.nelt,&nGID,arGID);
								FDelemfacets3a(indat.npoin,indat.nelt,indat.nop1,indat.matno);}
								iPaintyesno=10+1;iCullyesno=0;
////////////////// end
//for(j=0;j<nGID;j++)honk<<j<<" "<<nGID<<" "<<arGID[j]<<" arGID\n";
//honk<<iplotflag<<" "<<iCircleplot<<" "<<iPaintyesno<<" "<<iplotType<<" "<<iCullyesno<<" graphValCHECK\n";
//for(j=0;j<base.nelt;j++){
//	 if(base.trackELSET[j+1]-base.trackELSET[j]==1)honk<<indat.matno[j]<<" BaseM\n";
//	 else honk<<indat.matno[j]<<" WGggg "<<base.arrELSET[ base.trackELSET[j+1]-2 ]<<" "<<base.arrELSET[ base.trackELSET[j+1]-1 ]<<"\n";
//						}
//honk<<wp.nWeldGroup<<" "<<wp.nWeldPass<<" Crash dive\n";
//if(1==1)exit(0);
							   Invalidate();
//honk<<" Arachis\n";
							  }
//---------------------------------------------------------------------------
void TForm1::wpEdit2_public()
{long j=0,ie=0//,eltype=0,bscode=0,node=0,ieGID=0,t3=1000,t5=100000,t7=10000000
;
 wp.name[wp.PRECORD]=wp.temp_name;
 wp.matName[wp.PRECORD]=wp.temp_matName;
 wp.seqNum[wp.PRECORD]=wp.temp_seqNum;
 wp.boolFlags[wp.PRECORD]=wp.temp_boolFlags;
 wp.type[wp.PRECORD]=wp.temp_type;
 wp.shape[wp.PRECORD]=wp.temp_shape;
 wp.stepInterval[wp.PRECORD]=wp.temp_stepInterval;
 wp.hp[wp.PRECORD]=wp.temp_hp;
 wp.nsegs[wp.PRECORD]=wp.temp_nsegs;
 wp.iselect[wp.PRECORD]=wp.temp_iselect;
 wp.stpEle[wp.PRECORD]=wp.temp_stpEle;
 wp.curr[wp.PRECORD]=wp.temp_curr;
 wp.volt[wp.PRECORD]=wp.temp_volt;
 wp.eff[wp.PRECORD]=wp.temp_eff;
 wp.speed[wp.PRECORD]=wp.temp_speed;
 wp.troom[wp.PRECORD]=wp.temp_troom;
 wp.tmelt[wp.PRECORD]=wp.temp_tmelt;
 wp.tcutl[wp.PRECORD]=wp.temp_tcutl;
 wp.tcuth[wp.PRECORD]=wp.temp_tcuth;
 wp.timeInterval[wp.PRECORD]=wp.temp_timeInterval;
 wp.maxiHeatStep[wp.PRECORD]=wp.temp_maxiHeatStep;
 wp.miniHeatStep[wp.PRECORD]=wp.temp_miniHeatStep;
 wp.thk1[wp.PRECORD]=wp.temp_thk1;
 wp.thk2[wp.PRECORD]=wp.temp_thk2;
 wp.lstart[wp.PRECORD]=wp.temp_lstart;
 wp.lend[wp.PRECORD]=wp.temp_lend;
 wp.n_curr_sttEl[wp.PRECORD]=wp.temp_n_curr_sttEl;
 wp.reset[wp.PRECORD]=wp.temp_reset;
 wp.wpTimes[wp.PRECORD]=wp.temp_wpTimes;
 wp.source[wp.PRECORD]=wp.temp_source;
 wp.mcr[wp.PRECORD]=wp.temp_mcr;
 wp.WeldColor[wp.PRECORD]=wp.temp_WeldColor; // Policy: Reserve one extra nWeldPass and copy "to-be-edited" data there.
 wp.prevGID[wp.PRECORD]=wp.temp_prevGID;
 wp.util_arr[wp.PRECORD]=wp.temp_util_arr;
// for(j=0;j<base.nelt;j++)wp.eles[wp.PRECORD*base.nelt+j]=wp.eles[wp.nWeldPass*base.nelt+j];
// for(j=0;j<base.nelt;j++)wp.sttEles[wp.PRECORD*base.nelt+j]=wp.sttEles[wp.nWeldPass*base.nelt+j]; for(j=0;j<base.nelt;j++)wp.firstEle[wp.PRECORD*base.nelt+j]=wp.firstEle[wp.nWeldPass*base.nelt+j];
for(j=0;j<wp.nWeldPass*wp.memWGa;j++)wp.eles[j]=wp.temp_eles[j];
for(j=0;j<wp.nWeldPass*wp.memWGa;j++)wp.sttEles[j]=wp.temp_sttEles[j];

 for(j=0;j<wp.memWGa;j++)wp.nextEle[wp.PRECORD*wp.memWGa+j]=wp.nextEle[wp.nWeldPass*wp.memWGa+j];
 for(j=0;j<4;j++)wp.snorm1[wp.PRECORD*4+j]=wp.snorm1[wp.nWeldPass*4+j];
 for(j=0;j<4;j++)wp.snorm2[wp.PRECORD*4+j]=wp.snorm2[wp.nWeldPass*4+j];
 for(j=0;j<3;j++)wp.circEles[wp.PRECORD*3+j]=wp.circEles[wp.nWeldPass*3+j];
 for(j=0;j<wp.memWGa;j++)wp.edgeEles[wp.PRECORD*wp.memWGa+j]=wp.edgeEles[wp.nWeldPass*wp.memWGa+j];
 for(j=0;j<4;j++)wp.edgeNodes[wp.PRECORD*4+j]=wp.edgeNodes[wp.nWeldPass*4+j]; // correct???
// for(j=0;j<4*base.nelt;j++)wp.sttEleNodes[wp.PRECORD*base.nelt*4+j]=wp.sttEleNodes[wp.nWeldPass*base.nelt*4+j];
for(j=0;j<wp.nWeldPass*wp.memWGa*4;j++)wp.sttEleNodes[j]=wp.temp_sttEleNodes[j];

 for(j=0;j<wp.memWGa;j++)wp.hlightel[wp.PRECORD*wp.memWGa+j]=wp.hlightel[wp.nWeldPass*wp.memWGa+j];
 for(j=0;j<3*NDF;j++)wp.arrows[3*NDF*wp.PRECORD+j]=wp.arrows[3*NDF*wp.nWeldPass+j];
//
for(j=0;j<wp.memWGa;j++)
  {if(wp.eles[wp.nWeldPass*wp.memWGa+j]>=0){ie=wp.eles[wp.nWeldPass*wp.memWGa+j]/10;
											base.arrELSET[ie]=wp.nWeldGroup+wp.PRECORD+1;
											indat.arrELSET[ie]=wp.nWeldGroup+wp.PRECORD+1;
										   }
   else break;
  }


 FD_LButtonstatus=11;stateVFT=2;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 delete [] wp.temp_eles;delete [] wp.temp_sttEles;delete [] wp.temp_sttEleNodes;
 delete CreateLinWeldPass;CreateLinWeldPass=NULL;// because it was created with Show()?
//iplotflag=1;iCircleplot=1;
							   iCullyesno=0;Invalidate();// EFP 1/08/2011
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditWeldParamExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
  {if(wps.nWeldParamSet)
	 {if(!CreateWeldingParamSet)// Test here because a "Modal" MessageBox() can not be used after a ShowModal()
		{if(!VFTlistbox)
		   {VFTlistbox=new TForm26(3,this);
			VFTlistbox->Caption=L"Edit weld param set";
//			for(ip=0;ip<wps.nWeldParamSet;ip++)VFTlistbox->ListBox1->Items->Add(wps.name[ip].c_str());
			for(ip=0;ip<wps.nWeldParamSet;ip++)VFTlistbox->ListBox1->Items->Add(wps.name[ip].w_str());
			VFTlistbox->ListBox1->ItemIndex=wps.nWeldParamSet-1;
			VFTlistbox->ShowModal();
		   }
		 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
		}
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Close existing Create/Edit box first",L"Halt",MB_OK);}
	 }
   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld parameter sets found",L"Halt",MB_OK);}
  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteWeldParamExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
  {if(wps.nWeldParamSet)
	 {if(!VFTlistbox)
		{VFTlistbox=new TForm26(2,this);
		 VFTlistbox->Caption=L"Del weld param set";
//		 for(ip=0;ip<wps.nWeldParamSet;ip++)VFTlistbox->ListBox1->Items->Add(wps.name[ip].c_str());
		 for(ip=0;ip<wps.nWeldParamSet;ip++)VFTlistbox->ListBox1->Items->Add(wps.name[ip].w_str());
		 VFTlistbox->ListBox1->ItemIndex=wps.nWeldParamSet-1;
		 VFTlistbox->ShowModal();
		}
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
	 }
   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld parameter sets found",L"Halt",MB_OK);}
  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MatPropertiesExecute(TObject *Sender)
{long dum=0;
 wchar_t curMess0[]=L"MatProp",string1[15];
 if(base.nop1)
   {if(wps.nWeldParamSet)
	  {FDdynmem_manage(7,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
	   if(wms.nMatPropSet>0){
			 wms.cond[wms.nMatPropSet]=wms.cond[wms.nMatPropSet-1];
			 wms.heat[wms.nMatPropSet]=wms.heat[wms.nMatPropSet-1];
			 wms.den[wms.nMatPropSet]=wms.den[wms.nMatPropSet-1];
			 wms.mcr[wms.nMatPropSet]=wms.mcr[wms.nMatPropSet-1];
//			 wms.mcr[wms.nMatPropSet]=1;
//			 wms.mcr[wms.nMatPropSet]=2;
			 wms.ved[wms.nMatPropSet]=wms.ved[wms.nMatPropSet-1];
			 wms.annjd[wms.nMatPropSet]=wms.annjd[wms.nMatPropSet-1];
			 wms.Ti[wms.nMatPropSet]=wms.Ti[wms.nMatPropSet-1];
			 wms.Ta[wms.nMatPropSet]=wms.Ta[wms.nMatPropSet-1];
			 wms.Tm[wms.nMatPropSet]=wms.Tm[wms.nMatPropSet-1];
			 wms.switc[wms.nMatPropSet]=wms.switc[wms.nMatPropSet-1];
			 wms.hetjd[wms.nMatPropSet]=wms.hetjd[wms.nMatPropSet-1];
			 wms.nprops[wms.nMatPropSet]=wms.nprops[wms.nMatPropSet-1];
//			 wms.matFileName[wms.nMatPropSet]=wms.matFileName[wms.nMatPropSet-1];
			 wms.Steps[wms.nMatPropSet]=wms.Steps[wms.nMatPropSet-1];
							}
	   else {
			 wms.cond[wms.nMatPropSet]=0.045f;
			 wms.heat[wms.nMatPropSet]=580.f;
			 wms.den[wms.nMatPropSet]=0.00000785f;
			 wms.mcr[wms.nMatPropSet]=0;
////			 wms.mcr[wms.nMatPropSet]=1;
////			 wms.mcr[wms.nMatPropSet]=2;
////			 wms.ved[wms.nMatPropSet]= -1; //Default Annealing=OFF
////			 wms.annjd[wms.nMatPropSet]= -1; //Default VED=OFF
//			 wms.ved[wms.nMatPropSet]=0; //Default Annealing=ON (per William Ulrich request 2/27/2012)
//			 wms.annjd[wms.nMatPropSet]=0; //Default VED=ON (per William Ulrich request 2/27/2012)
			 wms.annjd[wms.nMatPropSet]= -1; //Default Anneal=ON (per William Ulrich request 2/27/2012)
			 wms.ved[wms.nMatPropSet]= -1; //Default VED=ON (per William Ulrich request 2/27/2012)
			 wms.Ti[wms.nMatPropSet]=750.f;
			 wms.Ta[wms.nMatPropSet]=1500.f;
			 wms.Tm[wms.nMatPropSet]=1500.f;
//			 wms.Steps[wms.nMatPropSet]="Steps".c_str();
			 wms.nprops[wms.nMatPropSet]=8;
			 wms.switc[wms.nMatPropSet]=false;
//			 if(wms.switc[wms.nMatPropSet]){wms.hetjd[wms.nMatPropSet]= -1;
//											wms.nprops[wms.nMatPropSet]=wms.nprops[wms.nMatPropSet]+1;
//										   }
//			 else wms.hetjd[wms.nMatPropSet]=0;
			 wms.hetjd[wms.nMatPropSet]=0;
//			 wms.Steps[wms.nMatPropSet]=L"0";
			 wms.Steps[wms.nMatPropSet]=L"****";//EFP 9/23/2011
			}
// _ltow(wms.nMatPropSet+1,string1,10);
 _ltow(wms.highest+1,string1,10);
 StringCchCatW(curMess0,39+1,string1);
 wms.name[wms.nMatPropSet]=curMess0;
 //	   CreateMatPropSet=new TForm16(this);
	   Form21=new TForm21(0,this);

//honk<<" LaVidaLoco\n";if(1==1)exit(0);

	   Form21->Caption="Create a Material Property Set";
	   Form21->Label1->Caption="Material Property Set Name";
	   Form21->Label2->Caption="Thermal Conductivity:";
	   Form21->Label3->Caption="Specific Heat:";
	   Form21->Label4->Caption="Density:";
	   Form21->Label6->Caption="Steps for Creep Input (comma separated)";
	   Form21->CheckBox1->Caption="Switch between Isotropic/Creep";
	   Form21->Label7->Caption="Annealing Initiation Temperature";
	   Form21->Label8->Caption="Annealing Temperature";
	   Form21->Label9->Caption="Material Melting Temperature";
	   Form21->Label10->Caption="Mechanical Property File Path/Name:";
	   Form21->RadioButton1->Caption="Isotropic (multi-linear)";
	   Form21->RadioButton2->Caption="Combined Hardening (multi-linear)";
	   Form21->RadioButton3->Caption="Simple Phase Transformation";
	   Form21->RadioButton4->Caption="Combined Hardening (linear)";
	   Form21->RadioButton5->Caption="Elastic-Plastic-Creep(Isotropic,multi-linear)";
	   Form21->RadioButton6->Caption="Complete Phase Transformation";
	   Form21->RadioButton9->Caption="No";
	   Form21->RadioButton10->Caption="Yes";
	   Form21->RadioButton7->Caption="No";
	   Form21->RadioButton8->Caption="Yes";
//	   Form21->Button1->Caption="Choose Mechanical Property File...";
	   Form21->Button2->Caption="Create";
	   Form21->Button3->Caption="Cancel";
	   Form21->GroupBox1->Caption="Physical Properties";
	   Form21->RadioGroup1->Caption="Material Constitutive Relation (Double-click Isotropic or EPC to get Creep)";
//// TBD: Find out why the following causes duplicate RadioButtons on RadioGroup1??? EFP 3/30/2010
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton1->Caption);
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton2->Caption);
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton3->Caption);
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton4->Caption);
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton5->Caption);
//	Form21->RadioGroup1->Items->Add(Form21->RadioButton6->Caption);
////	   Form21->GroupBox2->Caption="ABAQUS Material Inputs";
	   Form21->Label5->Caption="ABAQUS Material Inputs";
//	   Form21->GroupBox3->Caption="Annealing";
//	   Form21->GroupBox2->Caption="VED";
	   Form21->GroupBox2->Caption="Annealing";
	   Form21->GroupBox3->Caption="VED";
	   Form21->GroupBox4->Caption="Mechanical Properties";
//	   Form21->CheckCheckBox1=false;
	   Form21->CheckCheckBox1=wms.switc[wms.nMatPropSet];//EFP 9/23/2011
//	   Form21->CheckEdit5a=L"****";//	   Form21->CheckEdit5a=L"0";
	   Form21->CheckEdit5a=wms.Steps[wms.nMatPropSet].w_str(); //Correction??? EFP 10/03/2011
//
////	   Form21->CheckEdit1=L"****";
//	   Form21->CheckEdit1=wms.name[wms.nMatPropSet];
//	   Form21->CheckEdit1=wms.name[wms.nMatPropSet].c_str();
	   Form21->CheckEdit1=wms.name[wms.nMatPropSet].w_str();
	   Form21->CheckEdit2=wms.cond[wms.nMatPropSet];
	   Form21->CheckEdit3=wms.heat[wms.nMatPropSet];
	   Form21->CheckEdit4=wms.den[wms.nMatPropSet];
	   Form21->CheckEdit5=wms.Ti[wms.nMatPropSet];
	   Form21->CheckEdit6=wms.Ta[wms.nMatPropSet];
	   Form21->CheckEdit7=wms.Tm[wms.nMatPropSet];

//honk<<wms.name[wms.nMatPropSet].c_str()<<" NPS1\n";
//honk<<wms.matFileName[wms.nMatPropSet].c_str()<<" NPS2\n";
//if(wms.nMatPropSet>0){
//honk<<wms.name[wms.nMatPropSet-1].c_str()<<" TNPS1\n";
//honk<<wms.matFileName[wms.nMatPropSet-1].c_str()<<" TNPS2\n";
//					 }

//	   if(wms.nMatPropSet>0)Form21->CheckEdit8=wms.matFileName[wms.nMatPropSet-1].c_str();
	   if(wms.nMatPropSet>0)Form21->CheckEdit8=wms.matFileName[wms.nMatPropSet-1].w_str();
	   else Form21->CheckEdit8=L"****";
//	   Form21->CheckEdit8= *p;

	   if(wms.mcr[wms.nMatPropSet]==0)Form21->CheckRadioGroup1=0;
//	   else if(wms.mcr[wms.nMatPropSet]==1)Form21->CheckRadioGroup1=1;
//	   else if(wms.mcr[wms.nMatPropSet]==2)Form21->CheckRadioGroup1=2;
//	   else if(wms.mcr[wms.nMatPropSet]==3)Form21->CheckRadioGroup1=3;
//	   else if(wms.mcr[wms.nMatPropSet]==5)Form21->CheckRadioGroup1=4;
	   else if(wms.mcr[wms.nMatPropSet]==1)Form21->CheckRadioGroup1=3; //Correction BBrust 3/25/2011
	   else if(wms.mcr[wms.nMatPropSet]==2)Form21->CheckRadioGroup1=1;
	   else if(wms.mcr[wms.nMatPropSet]==3)Form21->CheckRadioGroup1=4;
	   else if(wms.mcr[wms.nMatPropSet]==5)Form21->CheckRadioGroup1=2;
	   else Form21->CheckRadioGroup1=5;

//	   if(wms.annjd[wms.nMatPropSet]==0)Form21->CheckGroupBox3=false;
//	   else Form21->CheckGroupBox3=true;
//	   if(wms.ved[wms.nMatPropSet]==0)Form21->CheckGroupBox4=false;
//	   else Form21->CheckGroupBox4=true;

//	   if(wms.annjd[wms.nMatPropSet]==0)Form21->CheckGroupBox4=false;
//	   else Form21->CheckGroupBox4=true;
//	   if(wms.ved[wms.nMatPropSet]==0)Form21->CheckGroupBox3=false;
//	   else Form21->CheckGroupBox3=true;

//	   if(wms.annjd[wms.nMatPropSet]==0)Form21->CheckGroupBox3=true;//Correction EFP 9/09/2011
//	   else Form21->CheckGroupBox3=false;
//	   if(wms.ved[wms.nMatPropSet]==0)Form21->CheckGroupBox4=true;
//	   else Form21->CheckGroupBox4=false;
	   if(wms.annjd[wms.nMatPropSet]==0)Form21->CheckGroupBox3=false;//This seems to be correct EFP 8/31/2012
	   else Form21->CheckGroupBox3=true;
	   if(wms.ved[wms.nMatPropSet]==0)Form21->CheckGroupBox4=false;
	   else Form21->CheckGroupBox4=true;

//	   Form21->Label6->Enabled=true;
//	   Form21->CheckBox1->Enabled=false;
//	   Form21->Edit5->Enabled=false;
//	   Form21->Label6->Visible=false;
//	   Form21->CheckBox1->Visible=false;
//	   Form21->Edit5->Visible=false;

	   if(wms.switc[wms.nMatPropSet]){//EFP 9/23/2011
	   Form21->Label6->Enabled=true;
	   Form21->CheckBox1->Enabled=true;
	   Form21->Edit5->Enabled=true;
	   Form21->Label6->Visible=true;
	   Form21->CheckBox1->Visible=true;
	   Form21->Edit5->Visible=true;
	   Form21->Button4->Enabled=true;
	   Form21->Button4->Visible=true;
 Form21->RadioButton2->Enabled=false;
 Form21->RadioButton3->Enabled=false;
 Form21->RadioButton4->Enabled=false;
 Form21->RadioButton5->Enabled=false;
 Form21->RadioButton6->Enabled=false;
 Form21->RadioButton2->Visible=false;
 Form21->RadioButton3->Visible=false;
 Form21->RadioButton4->Visible=false;
 Form21->RadioButton5->Visible=false;
 Form21->RadioButton6->Visible=false;
									 }
	   else {
	   Form21->Label6->Enabled=true;
	   Form21->CheckBox1->Enabled=true;
	   Form21->Edit5->Enabled=false;
	   Form21->Label6->Visible=false;
	   Form21->CheckBox1->Visible=false;
	   Form21->Edit5->Visible=false;
	   Form21->Button4->Enabled=false;
	   Form21->Button4->Visible=false;
 Form21->RadioButton2->Enabled=true;
 Form21->RadioButton3->Enabled=true;
 Form21->RadioButton4->Enabled=true;
 Form21->RadioButton5->Enabled=true;
 Form21->RadioButton6->Enabled=true;
 Form21->RadioButton2->Visible=true;
 Form21->RadioButton3->Visible=true;
 Form21->RadioButton4->Visible=true;
 Form21->RadioButton5->Visible=true;
 Form21->RadioButton6->Visible=true;
			}
	   Form21->ShowModal();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must enter Weld Parameter Set first.",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditMatPropExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
  {if(wps.nWeldParamSet)
   {if(wms.nMatPropSet)
	 {if(!Form21)// Test here because a "Modal" MessageBox() can not be used after a ShowModal()
		{if(!VFTlistbox)
		   {VFTlistbox=new TForm26(4,this);
			VFTlistbox->Caption=L"Edit mat prop set";
//			for(ip=0;ip<wms.nMatPropSet;ip++)VFTlistbox->ListBox1->Items->Add(wms.name[ip].c_str());
			for(ip=0;ip<wms.nMatPropSet;ip++)VFTlistbox->ListBox1->Items->Add(wms.name[ip].w_str());
			VFTlistbox->ListBox1->ItemIndex=wms.nMatPropSet-1;
			VFTlistbox->ShowModal();
		   }
		 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
		}
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Close existing Create/Edit box first",L"Halt",MB_OK);}
	 }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No material property sets found",L"Halt",MB_OK);}
   }
   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First enter weld parameter set",L"Halt",MB_OK);}
  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteMatPropExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
  {if(wms.nMatPropSet)
	 {if(!VFTlistbox)
		{VFTlistbox=new TForm26(5,this);
		 VFTlistbox->Caption=L"Del mat prop set";
//		 for(ip=0;ip<wms.nMatPropSet;ip++)VFTlistbox->ListBox1->Items->Add(wms.name[ip].c_str());
		 for(ip=0;ip<wms.nMatPropSet;ip++)VFTlistbox->ListBox1->Items->Add(wms.name[ip].w_str());
		 VFTlistbox->ListBox1->ItemIndex=wms.nMatPropSet-1;
		 VFTlistbox->ShowModal();
		}
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
	 }
   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No material property sets found",L"Halt",MB_OK);}
  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateWPFullLengthFullWidthExecute(TObject *Sender){create_new_weld(2);}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateWPFullLengthPartWidthExecute(TObject *Sender){create_new_weld(0);}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateWPPartLengthFullWidthExecute(TObject *Sender){create_new_weld(3);}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateWPPartLengthPartWidthExecute(TObject *Sender){create_new_weld(1);}
//---------------------------------------------------------------------------
void TForm1::create_new_weld(int isw)//TBD: Move base.nop1/etc tests to above calling routines  EFP 3/22/2012
{long dum=0,ir=0;
// char string0[40],currMess[]=" MAX_WELDPASSES exceeded.";
 wchar_t curMess0[]=L"weldpass",string1[11];  //TEST FOR EXISTING FORM
 wp.CreateWPassMode=1;
 wp.PRECORD=wp.nWeldPass;
/////////////
//honk<<iPersistVFT<<" "<<wp.memWGa<<" iPersistVFT\n";//if(1==1)exit(0);
/////////////
 if(base.nop1)
   {if(wps.nWeldParamSet)
	  {if(wms.nMatPropSet)
		 {
		  if(CreateLinWeldPass==NULL)
			  {if(iPaintyesno/10)
//				 {if(wp.nWeldPass<MAX_WELDPASSES)
												 {
												  VFTon=1;wp.count_curr_sttEl=0;
////												  FDdynmem_manage(6,dum,base.nelt,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
//												  FDdynmem_manage(6,dum,wp.memWGa,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum);
												  FDdynmem_manage(6,dum,wp.memWGa,dum,dum,dum,dum,dum,dum,dum,dum,dum,dum,1);
										 wp.seqNum[wp.nWeldPass]=wp.nWeldPass+1;
										 if(wp.nWeldPass){
												  wp.tcuth[wp.nWeldPass]=wp.tcuth[wp.nWeldPass-1];
												  wp.wpTimes[wp.nWeldPass]=wp.wpTimes[wp.nWeldPass-1];
												  wp.timeInterval[wp.nWeldPass]=wp.timeInterval[wp.nWeldPass-1];
												  wp.stepInterval[wp.nWeldPass]=wp.stepInterval[wp.nWeldPass-1];
										  wp.troom[wp.nWeldPass]=wp.troom[wp.nWeldPass-1];
										  wp.tmelt[wp.nWeldPass]=wp.tmelt[wp.nWeldPass-1];
										  wp.tcutl[wp.nWeldPass]=wp.tcutl[wp.nWeldPass-1];
										  wp.source[wp.nWeldPass]=wp.source[wp.nWeldPass-1];
										  wp.maxiHeatStep[wp.nWeldPass]=wp.maxiHeatStep[wp.nWeldPass-1];
										  wp.miniHeatStep[wp.nWeldPass]=wp.miniHeatStep[wp.nWeldPass-1];
												  wp.type[wp.nWeldPass]=wp.type[wp.nWeldPass-1];
												  wp.shape[wp.nWeldPass]=wp.shape[wp.nWeldPass-1];
												  wp.thk1[wp.nWeldPass]=wp.thk1[wp.nWeldPass-1];
												  wp.thk2[wp.nWeldPass]=wp.thk2[wp.nWeldPass-1];
//												  wp.thk1[wp.nWeldPass]=0.;
//												  wp.thk2[wp.nWeldPass]=0.;
//										  wp.curr[wp.nWeldPass]=wp.curr[wp.nWeldPass-1]; // Unknown
//										  wp.volt[wp.nWeldPass]=wp.volt[wp.nWeldPass-1]; // Unknown but curr*volt=8750
//										  wp.eff[wp.nWeldPass]=wp.eff[wp.nWeldPass-1];
//										  wp.speed[wp.nWeldPass]=wp.speed[wp.nWeldPass-1];
										  wp.boolFlags[wp.nWeldPass]=wp.boolFlags[wp.nWeldPass-1];
										  wp.hp[wp.nWeldPass]=wp.hp[wp.nWeldPass-1];// Moving Arc
										  wp.nsegs[wp.nWeldPass]=wp.nsegs[wp.nWeldPass-1];
										  wp.mcr[wp.nWeldPass]=wp.mcr[wp.nWeldPass-1];
///////////////////// Start coding to repeat Plate#1/2 normals  EFP 5/04/2011
for(ir=0;ir<4;ir++)wp.snorm1[4*wp.nWeldPass+ir]=wp.snorm1[4*(wp.nWeldPass-1)+ir];
for(ir=0;ir<4;ir++)wp.snorm2[4*wp.nWeldPass+ir]=wp.snorm2[4*(wp.nWeldPass-1)+ir];
for(ir=0;ir<NDF;ir++)wp.arrows[NDF*3*wp.nWeldPass+NDF*1+ir]=wp.arrows[NDF*3*(wp.nWeldPass-1)+NDF*1+ir];
for(ir=0;ir<NDF;ir++)wp.arrows[NDF*3*wp.nWeldPass+NDF*2+ir]=wp.arrows[NDF*3*(wp.nWeldPass-1)+NDF*2+ir];
///////////////////// End
												  wp.lstart[wp.nWeldPass]=wp.lstart[wp.nWeldPass-1]; //EFP9/02/2012
												  wp.lend[wp.nWeldPass]=wp.lend[wp.nWeldPass-1];
														 }
										 else            {
												  wp.tcuth[wp.nWeldPass]=25.f;
												  wp.wpTimes[wp.nWeldPass]=457.20f;
//												  wp.timeInterval[wp.nWeldPass]=600.0;
												  wp.timeInterval[wp.nWeldPass]=450.0f;
												  wp.stepInterval[wp.nWeldPass]=5;
										  wp.troom[wp.nWeldPass]=25.f;
										  wp.tmelt[wp.nWeldPass]=1500.f;
										  wp.tcutl[wp.nWeldPass]=50.f;
										  wp.source[wp.nWeldPass]=11;//Must be odd
										  wp.maxiHeatStep[wp.nWeldPass]=1000.f;
										  wp.miniHeatStep[wp.nWeldPass]=0.001f;
												  wp.type[wp.nWeldPass]=0;
												  wp.shape[wp.nWeldPass]=0;
//												  wp.thk1[wp.nWeldPass]=12.7;
//												  wp.thk2[wp.nWeldPass]=12.7;
												  wp.thk1[wp.nWeldPass]=0.f;
												  wp.thk2[wp.nWeldPass]=0.f;
//										  wp.curr[wp.nWeldPass]=350.; // Unknown
//										  wp.volt[wp.nWeldPass]=25.; // Unknown but curr*volt=8750
//										  wp.eff[wp.nWeldPass]=0.75;
//										  wp.speed[wp.nWeldPass]=5.;
										  wp.boolFlags[wp.nWeldPass]=0;
										  wp.hp[wp.nWeldPass]=0;// Moving Arc
										  wp.nsegs[wp.nWeldPass]=1;
										  wp.mcr[wp.nWeldPass]=0;
for(ir=0;ir<4;ir++)wp.snorm1[4*wp.nWeldPass+ir]= -1;
for(ir=0;ir<4;ir++)wp.snorm2[4*wp.nWeldPass+ir]= -1;
//
												  wp.lstart[wp.nWeldPass]=double(60.f*60.f); //seconds We will just use wp.lstart[0] EFP9/02/2012
												  wp.lend[wp.nWeldPass]=0.;
														  }
												  wp.reset[wp.nWeldPass]=0; //EFP 4/08/2013
///////////////////////////////////////////////////////////////////////
												  FD_LButtonstatus=17;
//												  iPersistVFT=1;
												  ir=iPersistVFT/10;
												  iPersistVFT=10*ir+1;
// NOTE: Test for wms existence...
/////////////
//honk<<iPersistVFT<<" iPersVFT\n";//if(1==1)exit(0);
/////////////
												  CreateLinWeldPass=new TForm15(isw,wp.nWeldPass+1,wp.seqNum[wp.nWeldPass],0,0,
																				wp.snorm1[4*wp.nWeldPass+0],wp.snorm2[4*wp.nWeldPass+0],
																				wms.nMatPropSet,wms.name,
																				wps.nWeldParamSet,wps.name,
																				this); // (weldpass#,weldseq#,...)

CreateLinWeldPass->CheckBox1->Enabled=false;//Disconnect weld direction CheckBox1 EFP 4/03/2011

if(isw==1)
  {
CreateLinWeldPass->Caption=L"Create partial length/partial section weld pass";
CreateLinWeldPass->Label3->Caption=L"All start elements";
CreateLinWeldPass->Label4->Caption=L"One stop element";
  }
else if(isw==2)
  {
CreateLinWeldPass->Caption=L"Create weld pass to end run/full section";
CreateLinWeldPass->Label3->Caption=L"One start element";
CreateLinWeldPass->Label4->Caption=L"Auto-gen to end run (click for direction)";
  }
else if(isw==3)
  {
CreateLinWeldPass->Caption=L"Create partial length/full section weld pass";
CreateLinWeldPass->Label3->Caption=L"One start element";
CreateLinWeldPass->Label4->Caption=L"One stop element";
  }
else { //isw==0
CreateLinWeldPass->Caption=L"Create weld pass to end run/partial section";
CreateLinWeldPass->Label3->Caption=L"All start elements";
CreateLinWeldPass->Label4->Caption=L"Auto-gen to end run (click for direction)";
	 }
CreateLinWeldPass->Label7->Caption=L"Plate1 normal";
CreateLinWeldPass->Label8->Caption=L"Plate2 normal";
//CreateLinWeldPass->CheckEdit1=L"****";
// _ltow(wp.nWeldPass+1,string1,10);
 _ltow(wp.highest+1,string1,10);
StringCchCatW(curMess0,11,string1);CreateLinWeldPass->CheckEdit1=curMess0;

CreateLinWeldPass->PageControl1->TabIndex=2;
/////////////////////// EFP 1/03/2011
CreateLinWeldPass->RadioGroup1->Enabled=False;
CreateLinWeldPass->RadioGroup1->Visible=False;
///////////////////////
CreateLinWeldPass->CheckEdit9=0.f;
CreateLinWeldPass->CheckEdit10=0.f;
CreateLinWeldPass->CheckEdit11=0.f;
CreateLinWeldPass->CheckEdit12=0.f;

CreateLinWeldPass->CheckEdit13=wp.troom[wp.nWeldPass];
CreateLinWeldPass->CheckEdit14=wp.tmelt[wp.nWeldPass];
CreateLinWeldPass->CheckEdit15=wp.tcutl[wp.nWeldPass];
CreateLinWeldPass->CheckEdit16=wp.tcuth[wp.nWeldPass];
CreateLinWeldPass->CheckEdit17=wp.timeInterval[wp.nWeldPass]; // Interpass cooling time???
CreateLinWeldPass->CheckEdit19=wp.stepInterval[wp.nWeldPass]; // Interpass cooling steps???
CreateLinWeldPass->CheckEdit18=wp.maxiHeatStep[wp.nWeldPass]; // Max heating time(output)???
CreateLinWeldPass->CheckEdit20=wp.miniHeatStep[wp.nWeldPass]; // Min heating time(output)???
CreateLinWeldPass->CheckEdit21=wp.thk1[wp.nWeldPass]; // Plate1 thickness???
CreateLinWeldPass->CheckEdit22=wp.thk2[wp.nWeldPass]; // Plate2 thickness???
//CreateLinWeldPass->CheckEditXXX=wp.hp[wp.nWeldPass]; // Moving Arc only (hp=0) at present (and this must be selected by TForm15 RadioButton5&6)
CreateLinWeldPass->CheckEdit23=wp.nsegs[wp.nWeldPass]; // Lumped pass heating procedure: Number of lumped segments
CreateLinWeldPass->CheckEdit24=1; // Near-edge effect: Number of required edges???
//CreateLinWeldPass->ComboBox1->ItemIndex = 0;
//CreateLinWeldPass->ComboBox2->ItemIndex = 0;
CreateLinWeldPass->CheckType=wp.type[wp.nWeldPass];
CreateLinWeldPass->CheckShape=wp.shape[wp.nWeldPass];

CreateLinWeldPass->CheckWeldColor=clOlive;

CreateLinWeldPass->Edit2->Enabled=false;

/////////// Cursor EFP 1/21/2011
Screen->Cursor=crDefault;
///////////
/////////////
//CreateLinWeldPass->PageControl1->ActivePage=0;
												  CreateLinWeldPass->Show();

//honk<<CreateLinWeldPass->CheckEdit9<<" FullLinCk0\n";
												 }

//				  else {
//		//				ltoa(MAX_WELDPASSES,string0,10);
//		//				strcat(string0,currMess);
//		//				extern PACKAGE void __fastcall Beep(void);
//		//				Application->MessageBox(string0,L"Halt",MB_OK);
//
//						extern PACKAGE void __fastcall Beep(void);
//						Application->MessageBox(_ltow(MAX_WELDPASSES,string0,10),L"Halt: MAX_WELDPASSES exceeded",MB_OK);
//					   }
//				 }
			   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Use VIEW/PAINT to paint",L"Halt",MB_OK);}
			  }
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Weld creation dialog already exists",L"Notice",MB_OK);}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First create material property set.",L"Halt",MB_OK);}
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First create weld parameter set.",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditWeldPassExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
   {if(wps.nWeldParamSet)
	  {if(wms.nMatPropSet)
		{if(wp.nWeldPass)
		   {if(!VFTlistbox)
			  {VFTlistbox=new TForm26(6,this); // The value 6 here has no meaning anymore EFP 1/03/2011
			   VFTlistbox->Caption=L"EditWeldPass (Full)";
			   wp.EDIT_SEL=0;
//			   for(ip=0;ip<wp.nWeldPass;ip++)VFTlistbox->ListBox1->Items->Add(wp.name[ip].c_str());
			   for(ip=0;ip<wp.nWeldPass;ip++)VFTlistbox->ListBox1->Items->Add(wp.name[ip].w_str());
			   VFTlistbox->ListBox1->ItemIndex=wp.nWeldPass-1;
			   VFTlistbox->ShowModal();
			  }
			else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
		   }
		 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld passes found",L"Halt",MB_OK);}
		}
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First create a material property set",L"Halt",MB_OK);}
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"First create a weld parameter set",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteWeldPassExecute(TObject *Sender)
{long ip=0;
 if(base.nop1)
  {if(wp.nWeldPass)
	 {if(!VFTlistbox)
		{VFTlistbox=new TForm26(1,this);
		 VFTlistbox->Caption=L"Delete weldpass";
//		 for(ip=0;ip<wp.nWeldPass;ip++)VFTlistbox->ListBox1->Items->Add(wp.name[ip].c_str());
		 for(ip=0;ip<wp.nWeldPass;ip++)VFTlistbox->ListBox1->Items->Add(wp.name[ip].w_str());
		 VFTlistbox->ListBox1->ItemIndex=wp.nWeldPass-1;
		 VFTlistbox->ShowModal();
		}
	  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"List box already exists",L"Halt",MB_OK);}
	 }
   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No weld passes found",L"Halt",MB_OK);}
  }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Import or generate geometry file->File/Misc.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WeldPassEditingandSequencing1Execute(TObject *Sender)
{if(base.nop1)
   {if(wp.nWeldPass>0)
//	  {WeldPassEditSeqn=new TForm18(wp.nWeldPass,wp.name,this);
	  {Screen->Cursor=crDefault; //EFP 4/27/2013
	   WeldPassEditSeqn=new TForm30(wp.nWeldPass,wp.name,wp.WeldColor,wp.seqNum,wp.reset,this);
	   WeldPassEditSeqn->Caption=L"Weld Pass Sequencing";
	   WeldPassEditSeqn->Label1->Caption=L"Current direction/seq.";
	   WeldPassEditSeqn->Label2->Caption=L"Proposed direction/seq.";
	   WeldPassEditSeqn->Label3->Caption=L"Check to reverse direction";
	   WeldPassEditSeqn->Label4->Caption=L"Animation time";
	   WeldPassEditSeqn->Button1->Caption=L"Enter";
	   WeldPassEditSeqn->Button2->Caption=L"Animate seq.";
	   WeldPassEditSeqn->Button3->Caption=L"Move up";
	   WeldPassEditSeqn->Button4->Caption=L"Move down";
	   WeldPassEditSeqn->Button5->Caption=L"Restore current dir.";
	   WeldPassEditSeqn->Button6->Caption=L"Restore curr.seq+dir"; //EFP 3/29/2012
	   WeldPassEditSeqn->Button7->Caption=L"Reverse direct. all";
	   WeldPassEditSeqn->Button8->Caption=L"Cancel";
	   WeldPassEditSeqn->CheckEdit1=1000;
	   WeldPassEditSeqn->Button2->Enabled=false; //Temporary EFP 3/29/2012
	   WeldPassEditSeqn->Show();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must create weld pass first.",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::WeldPassEditSeqn1_public() //Animate weld seq+dir  EFP 12/31/2011
{long i=0,ip=0,ieGID=0//,in=0,ie=0,ic=0,eltype=0,bscode=0,node=0,t3=1000,t5=100000,t7=10000000
;
 Screen->Cursor=crHourGlass; //Arrow is restored in PanGo1Execute() animation termination
 ip=iPersistVFT/10;iPersistVFT=10*ip+0;
 for(i=0;i<base.nelt;i++)base.arELEM[i]=1;
 iplotType=2;iplotflag=2;iPaintyesno=0;
 if(iPaintyesno/10==0){if(iplotType==2)FDelemfacets_arE3(indat.npoin,indat.nelt,indat.nop1,indat.matno,base.arELEM);
					   else {FDcomp_nGID(indat.nelt,&nGID,arGID);
							 FDelemfacets3(indat.npoin,indat.nelt,indat.nop1,indat.matno,arGID);
							}
					   iPaintyesno=10;iCullyesno=0;
					  }
 for(i=0;i<base.nelt;i++){//eltype=base.matno[i]/t7;bscode=(base.matno[i]-eltype*t7)/t5;node=(base.matno[i]-eltype*t7-bscode*t5)/t3;
ieGID=base.arrELSET[i];
////////////
						  if(ieGID>0)base.arELEM[i]=0;
						 }
 iCircleplot=1;Invalidate();
/////////// Cursor EFP 1/21/2011
//Screen->Cursor=crSizeAll;
///////////
////Convention: icTimer5=a+1000b, where a=current WP sequence & b=current slice  EFP 1/01/2012
// icTimer5=0;FD_LBrec=FD_LButtonstatus;stateVFTrec=stateVFT;FD_LButtonstatus=19;
// Timer2->Interval=WeldPassEditSeqn->CheckEdit1;
///////////////////////
//honk<<Timer2->Interval<<" Timer2->Interval\n";
///////////////////////
// Timer2->Enabled=true;
}
//---------------------------------------------------------------------------
void TForm1::WeldPassEditSeqn2_public() //Enter
{long ip=0,ipp=0;
 for(ip=0;ip<wp.nWeldPass;ip++){WeldPassEditSeqn->CheckSeq=ip;
								wp.seqNum[ip]=WeldPassEditSeqn->CheckSeq;
//								if(!WeldPassEditSeqn->CheckDir)// Change direction of weld pass
								if(WeldPassEditSeqn->CheckDir != wp.reset[ip]-10*(wp.reset[ip]/10))//EFP 3/28/2012 Change direction of weld pass
								  {RevProg0(ip);
								   ipp=wp.reset[ip]-10*(wp.reset[ip]/10);//EFP 3/28/2012
								   wp.reset[ip]=10*(wp.reset[ip]/10)+1-ipp;
								  }
							   }
 delete WeldPassEditSeqn;WeldPassEditSeqn=NULL;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
 Invalidate();
}
//---------------------------------------------------------------------------
void TForm1::WeldPassEditSeqn3_public(){Screen->Cursor=crSizeAll;delete WeldPassEditSeqn;WeldPassEditSeqn=NULL;}//Cancel EFP 3/29/2012
//---------------------------------------------------------------------------
void TForm1::RevProg0(long iWP)
{int opp_arr8[6]={2,3,0,1,5,4};
 long ie=0,iside=0,ipp=0,ippp=0,ip1=0,in=0,icount=0,is=0,ip=0, *dumarr=NULL;
 float xc=0.f,yc=0.f,zc=0.f,DJD=0.f,HN[9],SN[MXNPELS],SG[NDF*MXNPELS],DJR[9];
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7};
//// This might be wrong???
//   if(CRB_sel) //Partial
//	 {ippp=wp.circEles[3*iWP+0];
//	  wp.circEles[3*iWP+0]=wp.circEles[3*iWP+2];
//	  wp.circEles[3*iWP+2]=ippp;
//	 }
//   else // Full
//	 {ippp=wp.circEles[3*iWP+1];
//	  wp.circEles[3*iWP+1]=wp.circEles[3*iWP+2];
//	  wp.circEles[3*iWP+2]=ippp;
//	 }
//// EFP thinks that the coding should be as follows (3/22/2012) TEST THIS!!!
//// CRB_ckShape=CreateLinWeldPass->CheckShape;
//// 0=noncircular,1=fullcircle,2=partialcircle,3=fullgirth,4=partialgirth
   if(wp.shape[iWP]==1 || wp.shape[iWP]==3) //fullcircle or fullgirth
	 {ippp=wp.circEles[3*iWP+1];
	  wp.circEles[3*iWP+1]=wp.circEles[3*iWP+2];
	  wp.circEles[3*iWP+2]=ippp;
	 }
   else
	 {ippp=wp.circEles[3*iWP+0];
	  wp.circEles[3*iWP+0]=wp.circEles[3*iWP+2];
	  wp.circEles[3*iWP+2]=ippp;
	 }
for(in=0;in<wp.memWGa;in++)if(wp.eles[wp.memWGa*iWP+in]<0)break;
icount=in;
//for(in=0;in<icount;in++)honk<<iWP<<" "<<in<<" wpELES bef "<<wp.eles[wp.memWGa*iWP+in]<<"\n";
dumarr=new long[base.nelt];

//honk<<(iWP+1)<<" REVERSING WP\n";

for(in=0;in<icount;in++){dumarr[in]=wp.eles[wp.memWGa*iWP+in];
//honk<<(in+1)<<" "<<dumarr[in]<<" elesBEFORE\n";
						}
// Rewrite above to allow for mixed faces on sttEles   EFP 12/02/2011
for(ipp=0;ipp<icount/wp.n_curr_sttEl[iWP];ipp++)
  {for(in=0;in<wp.n_curr_sttEl[iWP];in++){ //Correction for missing pair of brackets (also in FormMouseDown) EFP 3/22/2012
ip1=    dumarr[wp.n_curr_sttEl[iWP]*((icount/wp.n_curr_sttEl[iWP])-ipp-1)+in]-
	10*(dumarr[wp.n_curr_sttEl[iWP]*((icount/wp.n_curr_sttEl[iWP])-ipp-1)+in]/10);

//honk<<iWP<<" "<<ipp<<" "<<in<<" "<<ip1<<" InPrrroc "<<wp.eles[wp.memWGa*iWP+wp.n_curr_sttEl[iWP]*ipp+in]
//		 <<" "<<dumarr[wp.n_curr_sttEl[iWP]*((icount/wp.n_curr_sttEl[iWP])-ipp-1)+in]<<"\n";

//wp.eles[wp.memWGa*iWP+wp.n_curr_sttEl[iWP]*ipp+in]=
//10*(dumarr[wp.n_curr_sttEl[iWP]*((icount/wp.n_curr_sttEl[iWP])-ipp-1)+in]/10)-ip1+opp_arr8[ip1];  //Correction  EFP 12/02/2011
wp.eles[wp.memWGa*iWP+wp.n_curr_sttEl[iWP]*ipp+in]=
dumarr[wp.n_curr_sttEl[iWP]*((icount/wp.n_curr_sttEl[iWP])-ipp-1)+in]-ip1+opp_arr8[ip1];  //Correction  EFP 3/22/2012
										 }
  }
 delete [] dumarr;
//// Figure out the following....
//for(in=0;in<wp.count_curr_sttEl;in++)wp.sttEles[wp.memWGa*wp.PRECORD+in]=wp.eles[wp.memWGa*wp.PRECORD+in]/10;//Emergency
// The following corrected  EFP 12/02/2011
 for(in=0;in<wp.n_curr_sttEl[iWP];in++){
//									   ip1=wp.eles[wp.memWGa*iWP+in];
//honk<<iWP<<" "<<in<<" Bef/After sttEles "<<wp.sttEles[wp.memWGa*iWP+in]<<" "<<ip1<<"\n";
										wp.sttEles[wp.memWGa*iWP+in]=wp.eles[wp.memWGa*iWP+in];
									   is=wp.sttEles[wp.memWGa*iWP+in]-10*(wp.sttEles[wp.memWGa*iWP+in]/10);

//honk<<(in+1)<<" "<<wp.sttEles[wp.memWGa*iWP+in]<<" "<<(is+1)<<" NEW sttEL\n";
									   for(ip=0;ip<4;ip++){  //Added as correction  EFP 4/08/2013
wp.sttEleNodes[wp.memWGa*4*iWP+4*in+ip]=
   base.nop1[MXNPEL*(wp.sttEles[wp.memWGa*iWP+in]/10)+gdata8[4*is+ip]]; //Not necessary here since this is done in export_CTSP()  EFP 3/22/2012

//honk<<(ip+1)<<" "<<wp.sttEleNodes[wp.memWGa*4*iWP+4*in+ip]<<" sttElNodes\n";
														  }
									   }

 ie=wp.sttEles[wp.memWGa*iWP+0]/10;iside=wp.sttEles[wp.memWGa*iWP+0]-10*ie; // Use first start element face as normal
 if(iside==0){xc=0.f;yc= -1.f;zc=0.f;}else if(iside==2){xc=0.f;yc=1.f;zc=0.f;}else if(iside==1){xc=1.f;yc=0.f;zc=0.f;}else if(iside==3){xc= -1.f;yc=0.f;zc=0.f;}else if(iside==4){xc=0.f;yc=0.f;zc= -1.f;}else {xc=0.f;yc=0.f;zc=1.f;}
 STFISO8(3,xc,yc,zc,&DJD,HN,SN,SG,DJR,indat.nop1+MXNPEL*ie,indat.c1);
 wp.arrows[NDF*3*iWP+NDF*0+0]= -HN[0];wp.arrows[NDF*3*iWP+NDF*0+1]= -HN[1];wp.arrows[NDF*3*iWP+NDF*0+2]= -HN[2];
 return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::exportCTSPExecute(TObject *Sender)
{long in=0;float iptmax=0.f;
// long isw=0;
// long ie=0,ies=0,i=0,j=0,k=0,ir=0,istart=0,iELSETtype=0,iELSETactive=0,t3=1000,ibrsw=0, *iELSETarr=NULL;
 if(base.nop1)
   {if(wp.nWeldPass)
	  {

	   CTSPnames=new TForm29(0,wp.nWeldPass,this);
	   CTSPnames->Caption="Enter file name for CTSP(TM) input deck.";
	   CTSPnames->Label1->Caption="CTSP main file";
	   CTSPnames->Label2->Caption="CTSP node file";
	   CTSPnames->Label3->Caption="CTSP element file";
	   CTSPnames->Label8->Caption="CTSP param file";
	   CTSPnames->Label4->Caption="Subdivide CTSP into # multicores:";
	   CTSPnames->Label5->Caption="_CTSP_input.in";
	   CTSPnames->Label6->Caption="_CTSP_node.in";
	   CTSPnames->Label7->Caption="_CTSP_element.in";
	   CTSPnames->Label9->Caption="_CTSP_param.in";
	   CTSPnames->Label10->Caption="Suggested core-to-core overlap time";
	   CTSPnames->Button1->Caption="OK";
	   CTSPnames->Button2->Caption="Cancel";
	   CTSPnames->Label2->Enabled=false;
	   CTSPnames->Label3->Enabled=false;
	   CTSPnames->Label6->Enabled=false;
	   CTSPnames->Label7->Enabled=false;
	   CTSPnames->Label8->Enabled=false;
	   CTSPnames->Label9->Enabled=false;
	   CTSPnames->Label10->Enabled=false;
	   CTSPnames->Label2->Visible=false;
	   CTSPnames->Label3->Visible=false;
	   CTSPnames->Label6->Visible=false;
	   CTSPnames->Label7->Visible=false;
	   CTSPnames->Label8->Visible=false;
	   CTSPnames->Label9->Visible=false;
	   CTSPnames->Label10->Visible=false;

	   CTSPnames->Edit1->Enabled=true;
	   CTSPnames->Edit2->Enabled=false;
	   CTSPnames->Edit3->Enabled=false;
	   CTSPnames->Edit5->Enabled=false;
	   CTSPnames->Edit4->Enabled=true;
	   CTSPnames->Edit6->Enabled=false;
	   CTSPnames->Edit1->Visible=true;
	   CTSPnames->Edit2->Visible=false;
	   CTSPnames->Edit3->Visible=false;
	   CTSPnames->Edit5->Visible=false;
	   CTSPnames->Edit4->Visible=true;
	   CTSPnames->Edit6->Visible=false;
	   CTSPnames->CheckEdit1=gWsiAlias;
	   CTSPnames->CheckEdit4=1;
	   if(wp.nWeldPass>1){iptmax=0.f;for(in=0;in<wp.nWeldPass-1;in++)if(iptmax<wp.timeInterval[in])iptmax=wp.timeInterval[in];
						  CTSPnames->CheckEdit6=3.5f*iptmax; // Policy: core-to-core default= 3.5x max IPtime interval
						 }
	   else CTSPnames->CheckEdit6=float(3600);
	   CTSPnames->ShowModal();

/*
	   for(i=0;i<wp.nWeldPass;i++)
		 {for(ies=0;ies<wp.memWGa;ies++)
			{ie=wp.eles[wp.memWGa*i+ies]/10;
			 if(ie>=0){k=base.matno[ie]-t3*(base.matno[ie]/t3);
					   base.matno[ie]= -(base.matno[ie]-k+i+1);
					  }
			 else break;
			}
		 }
	   istart=0;j=1;
	   iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);
	   do {ibrsw=0;
		   for(i=istart+1;i<base.nelt;i++)
			 {k= labs(base.matno[i])-t3*(labs(base.matno[i])/t3);
			  if(iELSETtype != k){if(base.matno[istart]> -1)
								  j++;ibrsw=1;istart=i;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){if(base.matno[istart]> -1)
							j++;
						   }
	   iELSETactive=j;iELSETarr=new long[iELSETactive];for(j=0;j<iELSETactive;j++)iELSETarr[j]= -1;
//aaaaaaaaaaa
	   istart=ies=0;
	   iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);
	   do {ibrsw=0;
		   for(i=istart+1;i<base.nelt;i++)
			 {k= labs(base.matno[i])-t3*(labs(base.matno[i])/t3);
			  if(iELSETtype != k){if(base.matno[istart]> -1){isw=1;if(ies){for(ir=0;ir<ies;ir++)if(iELSETarr[ir]==iELSETtype){isw=0;break;}}
															 if(isw){iELSETarr[ies]=iELSETtype;ies++;}
															}
								  ibrsw=1;istart=i;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){if(base.matno[istart]> -1){isw=1;if(ies){for(ir=0;ir<ies;ir++)if(iELSETarr[ir]==iELSETtype){isw=0;break;}}
													   if(isw){iELSETarr[ies]=iELSETtype;ies++;}
													  }
						   }
//bbbbbbbbbbb
//honk<<ies<<" IES "<<iELSETactive<<"\n";
//for(ir=0;ir<ies;ir++)honk<<(ir+1)<<" ELSETarr "<<iELSETarr[ir]<<"\n";
//	   Form7=new TForm7(base.allGrp,this);
	   Form7=new TForm7(ies,this);
	   Form7->Caption=L"Assign material files to non-WPs";
	   Form7->Label1->Caption=L"Entity";
	   Form7->Label2->Caption=L"Chosen material";
	   Form7->Label3->Caption=L"Available material";
	   Form7->Label4->Caption=L"Usage: First click on Entity; then click Available material";
	   Form7->Button1->Caption=L"OK";
	   for(ir=0;ir<ies;ir++)Form7->ListBox1->AddItem(base.ELSETinputnames[iELSETarr[ir]],this);
	   delete [] iELSETarr;
	   Form7->ListBox1->ItemIndex=0;
	   for(i=0;i<wms.nMatPropSet;i++)Form7->ListBox3->AddItem(wms.matFileName[i],this);
	   if(wms.nMatPropSet==1)for(i=0;i<ies;i++)Form7->ListBox2->AddItem(wms.matFileName[0],this);
	   else                  for(i=0;i<ies;i++)Form7->ListBox2->AddItem(L"****",this);
	   Form7->ShowModal();
	   delete Form7;// *Form7=NULL; (not in Unit1, remember, but perhaps we should not "delete"?)
*/
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must create weld passes->Weld",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::exportCTSP2_public() //Version with load balancing  EFP 11/24/2012
// FRANTIC NOTICE: ic, mcmlo.... must be int (not long, or HUGE PROBLEMS) so set maxCore=128
// For some reason, Delete *.* does not work so EFP has to delete files individually
//  (but this does not include the CTSPxxx.exe executable, which will hopefully be elsewhere.)
////{wchar_t curMess0[]=L"_CTSP_input.in\n",curMess1[]=L"_CTSP_node.in\n",curMess2[]=L"_CTSP_element.in",string0[90];
//{int ic=0,mcmlo=0,mcmup=0,mcminc=0,maxCore=128,mcm=CTSPnames->CheckEdit4;
{int ic=0,mcmlo=0,mcmup=0,mcminc=0,maxCore=999,mcm=CTSPnames->CheckEdit4; //EFP 4/29/2013
 long i=0,ip=0,iseq=0,icount=0,itotal=0,stepsum=0, *loadBal=NULL;
//char *temps;
 char extensChar1a[]="CTSPsubd00",extensChar1b[]="CTSPsubd0",extensChar1c[]="CTSPsubd",
//	  extensChar2[]="_CTSP_input.in",extensChar3[]="_CTSP_element.in",extensChar4[]="_CTSP_node.in",extensChar5[]="_CTSP_param.in",
	  *fnNeed=NULL
//	  ,*fnNeed1=NULL,*fnNeed2=NULL,*fnNeed3=NULL,*fnNeed4=NULL,*fnNeed5=NULL
;
 UnicodeString fnNeedS,extensCharS1a=UnicodeString(L"CTSPsubd00"),extensCharS1b=UnicodeString(L"CTSPsubd0"),extensCharS1c=UnicodeString(L"CTSPsubd");
 char buf[3+1];
 wchar_t curMess0[]=L"_CTSP_input.in\n",curMess1[]=L"_CTSP_node.in\n",curMess2[]=L"_CTSP_element.in\n",curMess3[]=L"_CTSP_param.in",string0[160];
//{wchar_t curMess0[]=L"_CTSP_input.txt\n",curMess1[]=L"_CTSP_node.txt\n",curMess2[]=L"_CTSP_element.txt",string0[90];
////		  Application->MessageBox(PtrToStringChars(gWsiAlias),L"Warning",MB_OK); // Visual C++ function
TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;

 gWsiAlias=CTSPnames->CheckEdit1;
 StringCchCopyW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess0);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess1);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess2);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess3);
 if(mcm==1){
//honk<<"\n"<<" Writing CTSP(TM) files...\n";
			extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Writing CTSP files exportCTSP2_public()",MB_OK);



			for(ic=0;ic<maxCore;ic++){ //Empty & remove any preexisting subdirectories  EFP 11/06/2011
// Allow for input.in, element.in, node.in, param.in, temp.out, time.out, etc but not *.exe
									  if(!DelSubd0(ic,gWsiAlias))break;
									 }



//			export_CTSP_INPUTp(gWsiAlias.t_str());export_CTSP_NODE(gWsiAlias.t_str());export_CTSP_ELEMENT8(gWsiAlias.t_str());
		   }
 else {itotal=0;loadBal=new long[wp.nWeldPass];
	   for(ip=0;ip<wp.nWeldPass;ip++)
		{icount=1;itotal++;for(i=0;i<wp.memWGa;i++)if(wp.eles[wp.memWGa*ip+i]<0)break;
		 icount=icount+i/wp.n_curr_sttEl[ip] +wp.stepInterval[ip];
		 itotal=itotal+i/wp.n_curr_sttEl[ip] +wp.stepInterval[ip];loadBal[ip]=icount;
		}
//	   mcminc=wp.nWeldPass/mcm;
	   mcminc=1+ itotal/mcm;mcmlo=1;
	   for(ip=0;ip<wp.nWeldPass;ip++)wp.lstart[ip]=double(CTSPnames->CheckEdit6);

//honk<<"\n"<<wp.lstart[0]<<" "<<mcm<<" "<<wp.nWeldPass<<" "<<mcminc<<" Writing multi-core CTSP(TM) directories...\n";
	   extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Writing multi-core CTSP directories",MB_OK);
	   for(ic=0;ic<mcm;ic++){stepsum=0;
//							 for(ip=0;ip<wp.nWeldPass;ip++){stepsum=stepsum+loadBal[ip];
//															if(stepsum>=mcminc*(ic+1))break;
//														   }
//							 if(stepsum-mcminc*(ic+1)>mcminc*(ic+1)+loadBal[ip]-stepsum)ip=ip-1;
//							 if(ip>wp.nWeldPass-mcm+ic)ip=wp.nWeldPass-mcm+ic;
//							 if(ip<ic)ip=ic;  //Correction: EFP 11/25/2012
//							 if(ic==mcm-1)mcmup=wp.nWeldPass;
////							 else mcmup=mcmlo+mcminc-1;
//							 else mcmup=ip+1;

							 for(iseq=0;iseq<wp.nWeldPass;iseq++)//This should be done by sequence Correction  EFP 9/12/2013
							   {for(ip=0;ip<wp.nWeldPass;ip++)if(iseq+1==wp.seqNum[ip])break;
								stepsum=stepsum+loadBal[ip];
								if(stepsum>=mcminc*(ic+1))break;
							   }
							 if(stepsum-mcminc*(ic+1)>mcminc*(ic+1)+loadBal[ip]-stepsum)iseq=iseq-1;
							 if(iseq>wp.nWeldPass-mcm+ic)iseq=wp.nWeldPass-mcm+ic;
							 if(iseq<ic)iseq=ic;  //Correction: EFP 11/25/2012
							 if(ic==mcm-1)mcmup=wp.nWeldPass;
							 else mcmup=iseq+1;

//							 if(ic<10-1){fnNeed=new char[strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1];
							 if(ic<10-1){
//										 gcvt(double(ic+1),1,buf);
//										 fnNeed=new char[strlen(extensChar1a)+strlen(buf)+1];
////										 StringCchCopy(fnNeed,strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1,extensChar1a);
//										 StringCchCopy(fnNeed,strlen(extensChar1a)+strlen(buf)+1,extensChar1a);
////										 StringCchCat(fnNeed,strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//										 StringCchCat(fnNeed,strlen(extensChar1a)+strlen(buf)+1,buf);
										 fnNeedS=extensCharS1a+IntToStr(ic+1);
//										 if(!DirectoryExists(extensCharS1a+IntToStr(ic+1)),true)CreateDirectoryW((extensCharS1a+IntToStr(ic+1)).w_str(),NULL);
//										 SetCurrentDirectory((extensCharS1a+IntToStr(ic+1)).w_str());
										}
							 else if(ic<100-1){
//							                   gcvt(double(ic+1),2,buf);
////											   fnNeed=new char[strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1];
////											   StringCchCopy(fnNeed,strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1,extensChar1b);
////											   StringCchCat(fnNeed,strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//											   fnNeed=new char[strlen(extensChar1b)+strlen(buf)+1];
//											   StringCchCopy(fnNeed,strlen(extensChar1b)+strlen(buf)+1,extensChar1b);
//											   StringCchCat(fnNeed,strlen(extensChar1b)+strlen(buf)+1,buf);
											   fnNeedS=extensCharS1b+IntToStr(ic+1);
//											   if(!DirectoryExists(extensCharS1b+IntToStr(ic+1)),true)CreateDirectoryW((extensCharS1b+IntToStr(ic+1)).w_str(),NULL);
//											   SetCurrentDirectory((extensCharS1b+IntToStr(ic+1)).w_str());
											  }
							 else {
//							       gcvt(double(ic+1),3,buf);
////								   fnNeed=new char[strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1];
////								   StringCchCopy(fnNeed,strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1,extensChar1c);
////								   StringCchCat(fnNeed,strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//								   fnNeed=new char[strlen(extensChar1c)+strlen(buf)+1];
//								   StringCchCopy(fnNeed,strlen(extensChar1c)+strlen(buf)+1,extensChar1c);
//								   StringCchCat(fnNeed,strlen(extensChar1c)+strlen(buf)+1,buf);
								   fnNeedS=extensCharS1c+IntToStr(ic+1);
//								   if(!DirectoryExists(extensCharS1c+IntToStr(ic+1)),true)CreateDirectoryW((extensCharS1c+IntToStr(ic+1)).w_str(),NULL);
//								   SetCurrentDirectory((extensCharS1c+IntToStr(ic+1)).w_str());
								  }


//							 if(!DirectoryExists(fnNeed))CreateDirectoryA(fnNeed,NULL);
//							 SetCurrentDirectory(fnNeed);delete [] fnNeed;
							 if(!DirectoryExists(fnNeedS),true)CreateDirectoryW(fnNeedS.w_str(),NULL);
							 SetCurrentDirectory(fnNeedS.w_str());


//							 if(ic==mcm-1)export_CTSP_INPUTp_mcm(mcmlo,mcmup,0,float(wp.lstart[0]),gWsiAlias.t_str());//Extra cooling OFF
//							 else         export_CTSP_INPUTp_mcm(mcmlo,mcmup,1,float(wp.lstart[0]),gWsiAlias.t_str());//Extra cooling ON
							 if(ic==mcm-1)export_CTSP_INPUTp_mcm(mcmlo,mcmup,0,float(wp.lstart[0]));//Extra cooling OFF
							 else         export_CTSP_INPUTp_mcm(mcmlo,mcmup,1,float(wp.lstart[0]));//Extra cooling ON
//							 export_CTSP_NODE(gWsiAlias.t_str());
							 export_CTSP_NODE();
//							 export_CTSP_ELEMENT8(gWsiAlias.t_str());
							 export_CTSP_ELEMENT8();
							 mcmlo=mcmup+1;SetCurrentDirectory(L"..");
							}
////////////////
	   delete [] loadBal;
////////////////



	   for(ic=mcm;ic<maxCore;ic++){ //Empty & remove any preexisting excess subdirectories  EFP 11/01/2011
// Allow for input.in, element.in, node.in, param.in, temp.out, time.out, etc but not *.exe
								   if(!DelSubd0(ic,gWsiAlias))break;
								  }



	  }
//////Policy: write input.in (all WP), element.in, node.in to model directory regardless of multi-core  EFP 11/06/2011
//// export_CTSP_INPUTp(gWsiAlias.t_str());
// export_CTSP_INPUTp1(gWsiAlias.t_str(),float(wp.lstart[0])); //Version which writes preemptive time.out  EFP 6/26/2012
 export_CTSP_INPUTp1(float(wp.lstart[0])); //Version which writes preemptive time.out  EFP 6/26/2012
// export_CTSP_NODE(gWsiAlias.t_str());
 export_CTSP_NODE();
// export_CTSP_ELEMENT8(gWsiAlias.t_str());
 export_CTSP_ELEMENT8();
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////
}
//---------------------------------------------------------------------------
void TForm1::exportCTSP3_public()
{gWsiAlias=CTSPnames->CheckEdit1;
	   CTSPnames->CheckIsel=1;
	   CTSPnames->Edit2->Enabled=true;CTSPnames->Edit3->Enabled=true;CTSPnames->Edit5->Enabled=true;CTSPnames->Edit6->Enabled=true;
	   CTSPnames->Edit2->Visible=true;CTSPnames->Edit3->Visible=true;CTSPnames->Edit5->Visible=true;CTSPnames->Edit6->Visible=true;
	   CTSPnames->Label1->Enabled=false;
	   CTSPnames->Label2->Enabled=true;
	   CTSPnames->Label3->Enabled=true;
	   CTSPnames->Label8->Enabled=true;
	   CTSPnames->Label4->Enabled=false;
	   CTSPnames->Label6->Enabled=true;
	   CTSPnames->Label7->Enabled=true;
	   CTSPnames->Label9->Enabled=true;
	   CTSPnames->Label10->Enabled=true;
	   CTSPnames->Label2->Visible=true;
	   CTSPnames->Label3->Visible=true;
	   CTSPnames->Label5->Visible=true;
	   CTSPnames->Label6->Visible=true;
	   CTSPnames->Label7->Visible=true;
	   CTSPnames->Label8->Visible=true;
	   CTSPnames->Label9->Visible=true;
	   CTSPnames->Label10->Visible=true;
	   CTSPnames->CheckEdit1=gWsiAlias;CTSPnames->CheckEdit2=gWsiAlias;CTSPnames->CheckEdit3=gWsiAlias;CTSPnames->CheckEdit5=gWsiAlias;
	   CTSPnames->Edit1->Enabled=false;

	   if(CTSPnames->CheckEdit4==1)CTSPnames->Button1->Caption=L"Write files";
	   else {CTSPnames->Label5->Caption=L"/CTSPsubdN/..._CTSP_input.in";
			 CTSPnames->Label6->Caption=L"/CTSPsubdN/..._CTSP_node.in";
			 CTSPnames->Label7->Caption=L"/CTSPsubdN/..._CTSP_element.in";
			 CTSPnames->Label9->Caption=L"/CTSPsubdN/..._CTSP_param.in";
			 CTSPnames->Button1->Caption=L"Write multi_core directories";
			}
}
/*
//---------------------------------------------------------------------------
void TForm1::exportCTSP4_public()
{int limlist=5,i=0,isw=0,icount=0,nlist=Form7->CheckNlist;
 String umat=L"_umat", *sArr=NULL;
 char longo1[2],longo2[3],longo3[4];
 long ir=0,j=0,js=0,k=0,ies=0,iesr=0,icycle=0,itype=0,ilast=0,istart=0,iELSETtype=0,t3=1000,ibrsw=0, *iELSETorder=NULL;
 sArr=new String[nlist];
 iELSETorder=new long[nlist+wp.nWeldPass];
 for(i=0;i<nlist;i++){Form7->CheckLB2ItemIndex=i;
					  sArr[i]=Form7->CheckLB2Item;
					 }
for(ir=0;ir<wms.nMatPropSet;ir++){honk<<"material ";
 int bufferSize1=WideCharToMultiByte(CP_UTF8,0,(wms.matFileName[ir]+umat).w_str(), -1,NULL,0,NULL,NULL);
 char* m1=new char[bufferSize1];WideCharToMultiByte(CP_UTF8,0,(wms.matFileName[ir]+umat).w_str(), -1,m1,bufferSize1,NULL,NULL);
honk<<m1<<"\n";// EFP 12/10/2014
honk<<"  properties umat  rho "<<wms.den[ir]<<"  alpha 0.0,\n";
honk<<"       um_1 1 um_2 "<<wms.Ti[ir]<<" um_3 "<<wms.Ta[ir]<<",\n";
honk<<"       um_4 "<<wms.Tm[ir]<<" um_5 -1.0,\n";
honk<<"       um_6 -1.0 um_7 0 um_8 0\n";
 delete[] m1;
								 }
honk<<"number of nodes "<<base.npoin<<" elements "<<base.nelt<<"\n";
honk<<"elements\n";
	   istart=j=js=0;iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);if(base.matno[istart]<0)iELSETtype= -iELSETtype;
	   do {ibrsw=0;
		   for(ir=istart+1;ir<base.nelt;ir++)
			 {k= labs(base.matno[ir])-t3*(labs(base.matno[ir])/t3);if(base.matno[ir]<0)k= -k;
			  if(iELSETtype != k){isw=1;for(ies=0;ies<j;ies++)if(iELSETorder[ies]/t3==iELSETtype){isw=0;iesr=ies;}
								  if(isw){if(iELSETtype> -1)iELSETorder[j]=iELSETtype*t3+js;
										  else              iELSETorder[j]=iELSETtype*t3-js;
										  j++;
										 }
								  else {if(iELSETorder[iesr]> -1)iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) +js;
										else                     iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) -js;
									   }
								  js++;ibrsw=1;istart=ir;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){isw=1;for(ies=0;ies<j;ies++)if(iELSETorder[ies]/t3==iELSETtype){isw=0;iesr=ies;}
							if(isw){if(iELSETtype> -1)iELSETorder[j]=iELSETtype*t3+js;
									else              iELSETorder[j]=iELSETtype*t3-js;
									j++;
								   }
							else {if(iELSETorder[iesr]> -1)iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) +js;
								  else                     iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) -js;
								 }
						   }
 for(icycle=0;icycle< nlist+wp.nWeldPass;icycle++)
   {itype=iELSETorder[icycle]/t3;ilast= labs(iELSETorder[icycle])-t3*(labs(iELSETorder[icycle])/t3);
	istart=j=js=0;iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);if(base.matno[istart]<0)iELSETtype= -iELSETtype;
	do {ibrsw=0;
		for(ir=istart+1;ir<base.nelt;ir++)
		  {k= labs(base.matno[ir])-t3*(labs(base.matno[ir])/t3);if(base.matno[ir]<0)k= -k;
		   if(iELSETtype != k){if(iELSETtype==itype){if(j==0){icount=0;if(itype>=0 && itype <10){ltoa(itype,longo1,10);honk<<"list \"nonWeldPassEntity"<<longo1<<"\" ";}
																	   else if(itype>=10 && itype <100){ltoa(itype,longo2,10);honk<<"list \"nonWeldPassEntity"<<longo2<<"\" ";}
																	   else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);honk<<"list \"nonWeldPassEntity"<<longo3<<"\" ";}
																	   else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);honk<<"list \"WeldPassEntity"<<longo1<<"\" ";}
																	   else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);honk<<"list \"WeldPassEntity"<<longo2<<"\" ";}
																	   else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);honk<<"list \"WeldPassEntity"<<longo3<<"\" ";}
																	   else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
																			}
															 }
													 j++;
													 if(ilast==js)honk<<(istart+1)<<"-"<<ir<<"\n";
													 else {icount++;if(icount<limlist)honk<<(istart+1)<<"-"<<ir<<",";
																	else {icount=0;honk<<(istart+1)<<"-"<<ir<<",\n      ";}
														  }
													}
							   js++;ibrsw=1;istart=ir;iELSETtype=k;break;
							  }
		  }
	   }
	while(ibrsw);
	if(istart<base.nelt){
							   if(iELSETtype==itype){if(j==0){icount=0;
																	   if(itype>=0 && itype <10){ltoa(itype,longo1,10);honk<<"list \"nonWeldPassEntity"<<longo1<<"\" ";}
																	   else if(itype>=10 && itype <100){ltoa(itype,longo2,10);honk<<"list \"nonWeldPassEntity"<<longo2<<"\" ";}
																	   else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);honk<<"list \"nonWeldPassEntity"<<longo3<<"\" ";}
																	   else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);honk<<"list \"WeldPassEntity"<<longo1<<"\" ";}
																	   else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);honk<<"list \"WeldPassEntity"<<longo2<<"\" ";}
																	   else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);honk<<"list \"WeldPassEntity"<<longo3<<"\" ";}
																	   else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
																			}
															 }
													 if(ilast==js)honk<<(istart+1)<<"-"<<ir<<"\n";
													 else {icount++;if(icount<limlist)honk<<(istart+1)<<"-"<<ir<<",";
																	else {icount=0;honk<<(istart+1)<<"-"<<ir<<",\n      ";}
														  }
													}
						}
   }
 for(icycle=0;icycle< nlist+wp.nWeldPass;icycle++)
   {itype=iELSETorder[icycle]/t3;
	if(itype>=0 && itype <10){int bufferSize3=WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
							  char* m1=new char[bufferSize3];WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,m1,bufferSize3,NULL,NULL);
							  ltoa(itype,longo1,10);
							  honk<<"\"nonWeldPassEntity"<<longo1<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
							  delete [] m1;
							 }
	else if(itype>=10 && itype <100){ltoa(itype,longo2,10);
							  int bufferSize4=WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
							  char* m1=new char[bufferSize4];WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,m1,bufferSize4,NULL,NULL);
							  honk<<"\"nonWeldPassEntity"<<longo2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
							  delete [] m1;
									}
	else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);
							  int bufferSize5=WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
							  char* m1=new char[bufferSize5];WideCharToMultiByte(CP_UTF8,0,(sArr[itype-1]+umat).w_str(), -1,m1,bufferSize5,NULL,NULL);
							  honk<<"\"nonWeldPassEntity"<<longo3<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
							  delete [] m1;
									  }
	else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);
									int bufferSize6=WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
									char* m1=new char[bufferSize6];WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,m1,bufferSize6,NULL,NULL);
									honk<<"\"nonWeldPassEntity"<<longo1<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
									delete [] m1;
								   }
	else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);
									int bufferSize7=WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
									char* m1=new char[bufferSize7];WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,m1,bufferSize7,NULL,NULL);
									honk<<"\"nonWeldPassEntity"<<longo2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
									delete [] m1;
									  }
	else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);
									int bufferSize8=WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,NULL,0,NULL,NULL);
									char* m1=new char[bufferSize8];WideCharToMultiByte(CP_UTF8,0,(wp.matName[-itype-1]+umat).w_str(), -1,m1,bufferSize8,NULL,NULL);
									honk<<"\"nonWeldPassEntity"<<longo3<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
									delete [] m1;
										}
	else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
		 }
   }
 delete [] iELSETorder;
 delete [] sArr;
}
*/
//---------------------------------------------------------------------------
int TForm1::DelSubd0(int ic,String gWsiAlias)
//WARNING: This code does not delete any possible executable, or the directory in such case  EFP 6/27/2012
{int flag=0;
 char buf[3+1];
 char extensChar1a[]="CTSPsubd00",extensChar1b[]="CTSPsubd0",extensChar1c[]="CTSPsubd",
	  extensChar2[]="_CTSP_input.in",extensChar3[]="_CTSP_element.in",extensChar4[]="_CTSP_node.in",extensChar5[]="_CTSP_param.in",
//	  *fnNeed=NULL,
	  *fnNeed1=NULL,*fnNeed2=NULL,*fnNeed3=NULL,*fnNeed4=NULL,*fnNeed5=NULL;
 UnicodeString fnNeedS1,fnNeedS2,fnNeedS3,fnNeedS4,fnNeedS5,
	  extensCharS1a=UnicodeString(L"CTSPsubd00"),extensCharS1b=UnicodeString(L"CTSPsubd0"),extensCharS1c=UnicodeString(L"CTSPsubd"),
	  extensCharS2=UnicodeString(L"_CTSP_input.in"),extensCharS3=UnicodeString(L"_CTSP_element.in"),extensCharS4=UnicodeString(L"_CTSP_node.in"),extensCharS5=UnicodeString(L"_CTSP_param.in");
 if(ic<10-1){
//			 gcvt(double(ic+1),1,buf);
////			 fnNeed1=new char[strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1];
////			 StringCchCopy(fnNeed1,strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1,extensChar1a);
////			 StringCchCat(fnNeed1,strlen(extensChar1a)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//			 fnNeed1=new char[strlen(extensChar1a)+strlen(buf)+1];
//			 StringCchCopy(fnNeed1,strlen(extensChar1a)+strlen(buf)+1,extensChar1a);
//			 StringCchCat(fnNeed1,strlen(extensChar1a)+strlen(buf)+1,buf);
			 fnNeedS1=extensCharS1a+IntToStr(ic+1);
			}
 else if(ic<100-1){
//				   gcvt(double(ic+1),2,buf);
////				   fnNeed1=new char[strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1];
////				   StringCchCopy(fnNeed1,strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1,extensChar1b);
////				   StringCchCat(fnNeed1,strlen(extensChar1b)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//				   fnNeed1=new char[strlen(extensChar1b)+strlen(buf)+1];
//				   StringCchCopy(fnNeed1,strlen(extensChar1b)+strlen(buf)+1,extensChar1b);
//				   StringCchCat(fnNeed1,strlen(extensChar1b)+strlen(buf)+1,buf);
				   fnNeedS1=extensCharS1b+IntToStr(ic+1);
				  }
 else {
//	   gcvt(double(ic+1),3,buf);
////	   fnNeed1=new char[strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1];
////	   StringCchCopy(fnNeed1,strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1,extensChar1c);
////	   StringCchCat(fnNeed1,strlen(extensChar1c)+strlen(IntToStr(ic+1).c_str())+1,IntToStr(ic+1).c_str());
//	   fnNeed1=new char[strlen(extensChar1c)+strlen(buf)+1];
//	   StringCchCopy(fnNeed1,strlen(extensChar1c)+strlen(buf)+1,extensChar1c);
//	   StringCchCat(fnNeed1,strlen(extensChar1c)+strlen(buf)+1,buf);
	   fnNeedS1=extensCharS1c+IntToStr(ic+1);
	  }
 if(DirectoryExists(fnNeedS1,true)){
 SetCurrentDirectory(fnNeedS1.w_str());
// fnNeed2=new char[strlen(gWsiAlias.t_str())+strlen(extensChar2)+1];
// StringCchCopy(fnNeed2,strlen(gWsiAlias.t_str())+strlen(extensChar2)+1,gWsiAlias.t_str());
// StringCchCat(fnNeed2,strlen(gWsiAlias.t_str())+strlen(extensChar2)+1,extensChar2);
// DeleteFile(fnNeed2);
 fnNeedS2=gWsiAlias+extensCharS2;
 DeleteFile(fnNeedS2);
 DeleteFile("input.in");
// delete [] fnNeed2;

// fnNeed3=new char[strlen(gWsiAlias.t_str())+strlen(extensChar3)+1];
// StringCchCopy(fnNeed3,strlen(gWsiAlias.t_str())+strlen(extensChar3)+1,gWsiAlias.t_str());
// StringCchCat(fnNeed3,strlen(gWsiAlias.t_str())+strlen(extensChar3)+1,extensChar3);
// DeleteFile(fnNeed3);
 fnNeedS3=gWsiAlias+extensCharS3;
 DeleteFile(fnNeedS3);
 DeleteFile("element.in");
// delete [] fnNeed3;

// fnNeed4=new char[strlen(gWsiAlias.t_str())+strlen(extensChar4)+1];
// StringCchCopy(fnNeed4,strlen(gWsiAlias.t_str())+strlen(extensChar4)+1,gWsiAlias.t_str());
// StringCchCat(fnNeed4,strlen(gWsiAlias.t_str())+strlen(extensChar4)+1,extensChar4);
// DeleteFile(fnNeed4);
 fnNeedS4=gWsiAlias+extensCharS4;
 DeleteFile(fnNeedS4);
 DeleteFile("node.in");
// delete [] fnNeed4;

// fnNeed5=new char[strlen(gWsiAlias.t_str())+strlen(extensChar5)+1];
// StringCchCopy(fnNeed5,strlen(gWsiAlias.t_str())+strlen(extensChar5)+1,gWsiAlias.t_str());
// StringCchCat(fnNeed5,strlen(gWsiAlias.t_str())+strlen(extensChar5)+1,extensChar5);
// DeleteFile(fnNeed5);
 fnNeedS5=gWsiAlias+extensCharS5;
 DeleteFile(fnNeedS5);
 DeleteFile(L"param.in");
// delete [] fnNeed5;

 DeleteFile(L"temp.out");DeleteFile(L"time.out");DeleteFile(L"*.exe"); //Which might or might not exist
 DeleteFile(L"tempRevise.out"); //EFP 6/27/2012
 SetCurrentDirectory(L"..");
 RemoveDirectory(fnNeedS1.w_str());
 flag=1;
								  }
 else flag=0;
// delete [] fnNeed1;
 return flag;
}
//---------------------------------------------------------------------------
//void TForm1::export_CTSP_NODE(char gVFTnameStem[]) // Identical to ABAQUS-format ASCII nodal geometry file *.inp (or *.in), delimited by ","
void TForm1::export_CTSP_NODE() // Identical to ABAQUS-format ASCII nodal geometry file *.inp (or *.in), delimited by ","
{long i=0; //Version with mirror file  EFP 3/26/2011
// char extensChar[]="_CTSP_node.in";char *fnNeed=new char[strlen(gVFTnameStem)+strlen(extensChar)+1];
// StringCchCopy(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,gVFTnameStem);StringCchCat(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,extensChar);
// ofstream viewfile(fnNeed);delete [] fnNeed;
 ofstream mirrorfile(L"node.in");
// viewfile.setf(ios::scientific);
 mirrorfile.setf(ios::scientific);

 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
 //try {
 //viewfile.precision(6);
	  mirrorfile.precision(6);
//	  for(i=0;i<base.npoin;i++)  viewfile<<base.node_map[i]+1<<","<<base.c1[NDF*i]<<","<<base.c1[NDF*i+1]<<","<<base.c1[NDF*i+2]<<"\n";
	  for(i=0;i<base.npoin;i++)mirrorfile<<(base.node_map[i]+1)<<","<<base.c1[NDF*i]<<","<<base.c1[NDF*i+1]<<","<<base.c1[NDF*i+2]<<"\n";
//	 }
//__finally {
Screen->Cursor=Save_Cursor;
//}
// viewfile.close();
 mirrorfile.close();
}
//---------------------------------------------------------------------------
//void TForm1::export_CTSP_ELEMENT8(char gVFTnameStem[]) // Identical to ABAQUS format ASCII element (8n only, with element mapping) connectivity file *.inp (or *.in), delimited by ","
void TForm1::export_CTSP_ELEMENT8() // Identical to ABAQUS format ASCII element (8n only, with element mapping) connectivity file *.inp (or *.in), delimited by ","
{long i=0,eltype=0,bscode=0,node=0,t7=10000000,t5=100000,t3=1000;
// char extensChar[]="_CTSP_element.in";char *fnNeed=new char[strlen(gVFTnameStem)+strlen(extensChar)+1];
// StringCchCopy(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,gVFTnameStem);StringCchCat(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,extensChar);
// ofstream viewfile(fnNeed);delete [] fnNeed;
 ofstream mirrorfile(L"element.in"); //Version with mirror file  EFP 3/26/2011
 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//// try {
////      for(i=0;i<base.nelt;i++)viewfile<<base.el_map[i]+1<<","
////				<<base.node_map[base.nop1[MXNPEL*i+0]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+1]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+2]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+3]]+1<<","
////				<<base.node_map[base.nop1[MXNPEL*i+4]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+5]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+6]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+7]]+1<<"\n";
//	  for(i=0;i<base.nelt;i++)mirrorfile<<base.el_map[i]+1<<","
//				<<base.node_map[base.nop1[MXNPEL*i+0]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+1]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+2]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+3]]+1<<","
//				<<base.node_map[base.nop1[MXNPEL*i+4]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+5]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+6]]+1<<","<<base.node_map[base.nop1[MXNPEL*i+7]]+1<<"\n";


				 for(i=0;i<base.nelt;i++){eltype=base.matno[i]/t7;bscode=(base.matno[i]-eltype*t7)/t5;node=(base.matno[i]-eltype*t7-bscode*t5)/t3;
										  if(eltype==8){if(node==8) //EFP 6/12/2014
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<"\n";
														else
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+10]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+11]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+12]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+13]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+14]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+15]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+16]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+17]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+18]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+19]]+1)<<"\n";
													   }
										  else if(eltype==7){if(node==6)
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<"\n";
															 else
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+10]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+11]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+12]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+13]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+14]]+1)<<"\n";
															}
										  else if(eltype==5){if(node==4)
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<"\n";
															 else
mirrorfile<<(base.el_map[i]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i   ]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 1]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 2]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 3]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 4]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 5]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 6]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 7]]+1)<<","<<
									 (base.node_map[base.nop1[MXNPEL*i+ 8]]+1)<<","<<(base.node_map[base.nop1[MXNPEL*i+ 9]]+1)<<"\n";
															}
										  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"export_CTSP_ELEMENT8() unsupported element type",L"Terminate",MB_OK);exit(0);}
										 }


//	 }
//__finally {
Screen->Cursor=Save_Cursor;
//}
// viewfile.close();
 mirrorfile.close();
}
//---------------------------------------------------------------------------
//void TForm1::export_CTSP_INPUTp1(char gVFTnameStem[],float overlap) //Version with mirror file  EFP 3/26/2011
void TForm1::export_CTSP_INPUTp1(float overlap) //Version with mirror file  EFP 3/26/2011
// Version for use with mcm only, which writes a preemptive time.out  EFP 6/26/2012
{long i=0,k=0,icount=0,steptotal=0,sttEles_size=0,eles_size=0,iseq=0,mstep=0,medge=0,mseg=0,maxelp=0,
	  ipp=0,in=0,ie1=0,iside1=0,is1=0,j=0,max1=0,lastStepIntv=0; //This line junk EFP 6/16/2012
 float timeWeld=0.f,lasttimerec=0.f,highestTmelt=0.f;int solidshellsw=0; //0=solid, 1=shell  EFP 8/19/2012
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7},
	  opp_arr8[6]={2,3,0,1,5,4};
 float dist=0.f,xc=0.f,yc=0.f,zc=0.f;
 double heritageTime=0.f,delt=0.f,deltc=0.f,sumdeltc=0.f,echo=0.f;
// char extensChar[]="_CTSP_input.in";char *fnNeed=new char[strlen(gVFTnameStem)+strlen(extensChar)+1];
// StringCchCopy(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,gVFTnameStem);StringCchCat(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,extensChar);
// ofstream viewfile(fnNeed);delete [] fnNeed;
 ofstream mirrorfile(L"input.in");
 ofstream outfile2(L"time.out");
 ofstream outfile3(L"scratch0.txt");
// viewfile.setf(ios::scientific);
 mirrorfile.setf(ios::scientific);

 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
// try {
	  medge=mseg=icount=maxelp=0; //Corrected EFP 5/12/2010
		 icount++; //Bud's correction  EFP 12/13/2012
	  highestTmelt=0.f;
	  for(k=0;k<wp.nWeldPass;k++)
		{
		 if(highestTmelt<wp.tmelt[k])highestTmelt=wp.tmelt[k]; //EFP 12/25/2014
//		 icount++;
		 for(i=0;i<wp.memWGa;i++)if(wp.eles[wp.memWGa*k+i]<0)break;
		 icount=icount+i/wp.n_curr_sttEl[k] +wp.stepInterval[k]; //Corrected EFP 5/12/2010
		 if(maxelp<i)maxelp=i; //EFP 3/08/2012
		}
	  if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #time step data",MB_OK);exit(0);}
	  mstep=icount;
//	  viewfile<<"1, "<<icount<<"\n";
	  mirrorfile<<"1, "<<icount<<"\n";
//	  viewfile<<wms.cond[0]<<", "<<wms.heat[0]<<", "<<wms.den[0]<<", "<<wp.eff[0]<<", "<<wp.nWeldPass<<", 0.0\n";//Extra 0 at end (i.e. new format) BB 4/16/2010
	  mirrorfile<<wms.cond[0]<<", "<<wms.heat[0]<<", "<<wms.den[0]<<", "<<wp.eff[0]<<", "<<wp.nWeldPass<<", 0.0\n";//Extra 0 at end (i.e. new format) BB 4/16/2010

// No Preheat Temperature Version
// Next Line otherwise delete
//          sb.append(wp.troom + ", " + wp.tmelt  + ", " + wp.tcutl + ", " + wp.source + ", 1, " + wp.maxiHeatStep + ", " + wp.miniHeatStep + "\n");
// tcuth is used for Preheat Temperature
// Activate the next line for writing Preheat Temperature
//          sb.append(wp.tcuth + ", "+ wp.troom + ", " + wp.tmelt  + ", " + wp.tcutl + ", " + wp.source + ", 1, " + wp.maxiHeatStep + ", " + wp.miniHeatStep + "\n");

////	  viewfile<<wp.tcuth[0]<<", "<<wp.troom[0]<<", "<<wp.tmelt[0]<<", "<<wp.tcutl[0]<<", "<<wp.source[0]<<", 1, "<<wp.maxiHeatStep[0]<<", "<<wp.miniHeatStep[0]<<"\n";
//	  mirrorfile<<wp.tcuth[0]<<", "<<wp.troom[0]<<", "<<wp.tmelt[0]<<", "<<wp.tcutl[0]<<", "<<wp.source[0]<<", 1, "<<wp.maxiHeatStep[0]<<", "<<wp.miniHeatStep[0]<<"\n";
	  mirrorfile<<wp.tcuth[0]<<", "<<wp.troom[0]<<", "<<highestTmelt<<", "<<wp.tcutl[0]<<", "<<wp.source[0]<<", 1, "<<wp.maxiHeatStep[0]<<", "<<wp.miniHeatStep[0]<<"\n";
	  steptotal=0;timeWeld=0.f;
// boolFlags[]: showFlag=1st digit,circFlag=2nd,edgeFlag=3rd,merge=4th,girthFlag=5th

/////////////// start Junk EFP 6/16/2012
max1=1;heritageTime=0.f;
outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
outfile3<<0.<<" "<<1<<"\n";
/////////////// end

	  for(iseq=0;iseq<wp.nWeldPass;iseq++)
		{for(k=0;k<wp.nWeldPass;k++)if(iseq+1==wp.seqNum[k])break;
////		 viewfile<<"***** Weld Pass "<<wp.name[k].t_str()<<" *****\n";
//		 mirrorfile<<"***** Weld Pass "<<wp.name[k].t_str()<<" *****\n";
//		 mirrorfile<<"***** Weld Pass "<<wp.name[k].w_str()<<" *****\n";
//xxxxxxxxxx
 int bufferSize=WideCharToMultiByte(CP_UTF8,0,wp.name[k].w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,wp.name[k].w_str(), -1,m,bufferSize,NULL,NULL);
 mirrorfile<<"***** Weld Pass "<<m<<" *****\n";
 delete[] m;
//xxxxxxxxxx

////		 int showFlag=wp.boolFlags[k]-10*(wp.boolFlags[k]/10);
		 int circFlag=(wp.boolFlags[k]-100*(wp.boolFlags[k]/100))/10;
		 int edgeFlag=(wp.boolFlags[k]-1000*(wp.boolFlags[k]/1000))/100;
		 int merge=(wp.boolFlags[k]-10000*(wp.boolFlags[k]/10000))/1000;
		 int girthFlag=(wp.boolFlags[k]-100000*(wp.boolFlags[k]/100000))/10000;
		 icount=0;for(i=0;i<4*wp.nWeldPass;i++)if(wp.edgeNodes[i]> -1)icount++;else break;
		 int numEdge = edgeFlag ? icount : 0;
///////////////////////// start EFP 10/12/2011
		 if(numEdge)medge++; //EFP 10/12/2011
///////////////////////// end
		 int cirWeld = circFlag ? wp.shape[k] : 0;
		 int girthWeld=0;
		 if(wp.shape[k]==3){girthWeld=1;cirWeld=0;}
		 if(wp.shape[k]==4){girthWeld=2;cirWeld=0;}
		 int lumpWeld=wp.nsegs[k]; //Lump Pass (1) or Moving Arc (0) Heating Procedure
		 if(wp.hp[k]==0)lumpWeld=0;
//         if(k==0)wp.merge[k]=false;
		 wp.boolFlags[k]=wp.boolFlags[k]-merge*1000; //Zero merge component
		 if(k==0)merge=0;
		 if(merge>0)wp.boolFlags[k]=wp.boolFlags[k]+merge*1000;
		 int mcond=0;if(merge)mcond=1;
//		 viewfile<<"1, "<<wp.type[k] + 1<<", "<<wp.thk1[k]<<", "<<wp.thk2[k]<<", "<<numEdge<<", "<<cirWeld<<", "<<lumpWeld<<", "<<mcond<<", "<<girthWeld<<"\n";
		 mirrorfile<<"1, "<<(wp.type[k] + 1)<<", "<<wp.thk1[k]<<", "<<wp.thk2[k]<<", "<<numEdge<<", "<<cirWeld<<", "<<lumpWeld<<", "<<mcond<<", "<<girthWeld<<"\n";
//		 viewfile  <<base.node_map[wp.snorm1[4*k+1]]+1<<", "<<base.node_map[wp.snorm1[4*k+2]]+1<<", "<<base.node_map[wp.snorm1[4*k+3]]+1<<", "; // Note that these are the 2/3/4 nodes, not 1
		 mirrorfile<<(base.node_map[wp.snorm1[4*k+1]]+1)<<", "<<(base.node_map[wp.snorm1[4*k+2]]+1)<<", "<<(base.node_map[wp.snorm1[4*k+3]]+1)<<", "; // Note that these are the 2/3/4 nodes, not 1
//		 viewfile  <<base.node_map[wp.snorm2[4*k+1]]+1<<", "<<base.node_map[wp.snorm2[4*k+2]]+1<<", "<<base.node_map[wp.snorm2[4*k+3]]+1<<"\n";
		 mirrorfile<<(base.node_map[wp.snorm2[4*k+1]]+1)<<", "<<(base.node_map[wp.snorm2[4*k+2]]+1)<<", "<<(base.node_map[wp.snorm2[4*k+3]]+1)<<"\n";
//		 viewfile<<wp.timeInterval[k]<<", "<<wp.stepInterval[k]<<", "<<wp.speed[k]<<", "<<wp.curr[k] * wp.volt[k]<<"\n";
		 mirrorfile<<wp.timeInterval[k]<<", "<<wp.stepInterval[k]<<", "<<wp.speed[k]<<", "<<(wp.curr[k] * wp.volt[k])<<"\n";
		 icount=0;for(i=wp.memWGa*k;i<wp.memWGa*(k+1);i++){if(wp.sttEles[i]> -1)icount++;else break;}
		 if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #weld start elements",MB_OK);exit(0);}
		 sttEles_size=icount;
//		 viewfile<<icount<<"\n";
		 mirrorfile<<icount<<"\n";
//		 for(i=0;i<icount;i++)viewfile  <<base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+0]]+1<<", "<<base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+1]]+1<<", "<<base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+2]]+1<<", "<<base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+3]]+1<<"\n";
		 for(i=0;i<icount;i++)mirrorfile<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+0]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+1]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+2]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+3]]+1)<<"\n";
		 icount=0;for(i=wp.memWGa*k;i<wp.memWGa*(k+1);i++){if(wp.eles[i]> -1)icount++;else break;}
		 if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #weld group elements",MB_OK);exit(0);}
		 eles_size=icount;
/////////////////////////// EFP 10/12/2011
		 if(mseg<icount/wp.n_curr_sttEl[k])mseg=icount/wp.n_curr_sttEl[k];
///////////////////////////
//		 viewfile<<icount;
		 mirrorfile<<icount;
		 for(i=0;i<icount;i++){if(i%10 == 0){//viewfile<<"\n";
											 mirrorfile<<"\n";}
//							   viewfile<<base.el_map[wp.eles[wp.memWGa*k+i]/10] +1; //Corrected EFP 9/10/2010
							   mirrorfile<<(base.el_map[wp.eles[wp.memWGa*k+i]/10] +1); //Corrected EFP 9/10/2010
							   if(i%10 != 9 && i != icount-1){//viewfile<<", ";
															  mirrorfile<<", ";}
							  }
//		 viewfile<<"\n";
		 mirrorfile<<"\n";
		 if(wp.hp[k]==0)steptotal += wp.stepInterval[k]+eles_size/sttEles_size;
		 else if(wp.hp[k]==1)steptotal += wp.stepInterval[k]+(wp.nsegs[k]-1)*5+ (wp.nsegs[k])*20 ;

//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
		 timeWeld=timeWeld+wp.timeInterval[k]+wp.lend[k]/wp.speed[k];
//cccccccccccccccc end

//		 for(i=0;i<numEdge;i++)viewfile  <<base.node_map[wp.edgeNodes[4*k+i]]+1<<"\n"; //Edge Effect (Correction EFP 4/09/2011
		 for(i=0;i<numEdge;i++)mirrorfile<<(base.node_map[wp.edgeNodes[4*k+i]]+1)<<"\n"; //Edge Effect (Correction EFP 4/09/2011
//Circular Weld
		 if(circFlag){for(i=0;i<3;i++){
//									   viewfile  <<base.node_map[wp.circEles[3*k+i]]+1; // Corrected EFP 9/10/2010
									   mirrorfile<<(base.node_map[wp.circEles[3*k+i]]+1); // Corrected EFP 9/10/2010
									   if(i<2){//viewfile<<", ";
											   mirrorfile<<", ";}
									  }
//					  viewfile<<"\n";
					  mirrorfile<<"\n";
					 }
//Girth Weld
		 if(girthFlag){for(i=0;i<3;i++){
//										viewfile  <<base.node_map[wp.circEles[3*k+i]]+1; // Corrected EFP 9/10/2010
										mirrorfile<<(base.node_map[wp.circEles[3*k+i]]+1); // Corrected EFP 9/10/2010
										if(i<2){//viewfile<<", ";
												mirrorfile<<", ";}
									   }
//					   viewfile<<"\n";
					   mirrorfile<<"\n";
					  }

////////////////// start Junk EFP 6/16/2012
						  for(ipp=0;ipp<eles_size/wp.n_curr_sttEl[k];ipp++){dist=0.f;
					   for(in=0;in<wp.n_curr_sttEl[k];in++){
ie1=wp.eles[wp.memWGa*k+wp.n_curr_sttEl[k]*ipp+in]/10;iside1=wp.eles[wp.memWGa*k+wp.n_curr_sttEl[k]*ipp+in]-10*ie1;
xc=yc=zc=0.f;
for(is1=0;is1<4;is1++){xc=xc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+0]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+0];
					   yc=yc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+1]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+1];
					   zc=zc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+2]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+2];
					  }
xc=xc/4.f;yc=yc/4.f;zc=zc/4.f;dist=dist+sqrt(xc*xc+yc*yc+zc*zc);//EFP dislikes this non-area-weighted centroid
														   }
delt=dist/(float(wp.n_curr_sttEl[k])*wp.speed[k]);
heritageTime=heritageTime+delt;
//viewfile1<<heritageTime<<"\n";
outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
//if(ipp<eles_size/wp.n_curr_sttEl[k] -1)outfile3<<delt<<" "<<3<<"\n";else outfile3<<delt<<" "<<2<<"\n";
if(ipp<eles_size/wp.n_curr_sttEl[k] -1)outfile3<<heritageTime<<" "<<3<<"\n";else outfile3<<heritageTime<<" "<<2<<"\n";
																		   }
////////////
if(wp.stepInterval[k]==1){heritageTime=heritageTime+wp.timeInterval[k];
//viewfile1<<heritageTime<<"\n";
outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
//if(iseq<wp.nWeldPass-1)outfile3<<wp.timeInterval[k]<<" "<<1<<"\n";else outfile3<<wp.timeInterval[k]<<" "<<3<<"\n";
if(iseq<wp.nWeldPass-1)outfile3<<heritageTime<<" "<<1<<"\n";else outfile3<<heritageTime<<" "<<3<<"\n";
						 }
else {
	  echo=heritageTime;
////	  ExpoStep(delt,double(wp.timeInterval[k]),wp.stepInterval[k],&T0,&CM);
////	  for(j=0;j<wp.stepInterval[k];j++){
////heritageTime=echo+T0*(exp(CM*float(j+1))-1.);
////outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
////									   }
//
//	  aval=1.;sum=aval; //EFP 11/24/2012
//	  for(j=0;j<wp.stepInterval[k]-1;j++){aval=aval*1.2;sum=sum+aval;}
//	  a0=wp.timeInterval[k]/sum;
//	  for(j=0;j<wp.stepInterval[k]-1;j++){
//heritageTime=heritageTime+a0;a0=a0*1.2;
//outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
//										 }
	  sumdeltc=0.f;
	  for(j=0;j<wp.stepInterval[k]-1;j++){ //EFP 1/18/2013
deltc=wp.timeInterval[k]*
  log((wp.stepInterval[k]-float(j)+float(j)*0.0353f)/(wp.stepInterval[k]-float(j)-1.f+float(j+1)*0.0353f))/
  log(1./0.0353f);
sumdeltc=sumdeltc+deltc;
heritageTime=heritageTime+wp.timeInterval[k]*
  log((wp.stepInterval[k]-float(j)+float(j)*0.0353f)/(wp.stepInterval[k]-float(j)-1.f+float(j+1)*0.0353f))/
  log(1./0.0353f);
outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
//outfile3<<deltc<<" "<<3<<"\n";
outfile3<<heritageTime<<" "<<3<<"\n";
										 }

heritageTime=echo+wp.timeInterval[k];
outfile2<<setw(12)<<max1<<setw(15)<<scientific<<heritageTime<<"\n";max1++;
//if(iseq<wp.nWeldPass-1)outfile3<<wp.timeInterval[k]-sumdeltc<<" "<<1<<"\n";else outfile3<<wp.timeInterval[k]-sumdeltc<<" "<<3<<"\n";
if(iseq<wp.nWeldPass-1)outfile3<<heritageTime<<" "<<1<<"\n";else outfile3<<heritageTime<<" "<<3<<"\n";
	 }
////////////////// end
		 lasttimerec=wp.timeInterval[k]; //EFP 8/192012
		 lastStepIntv=wp.stepInterval[k]; //EFP 8/192012
		}
//	  viewfile<<"-1\n";
	  mirrorfile<<"-1\n";
//	 }
//__finally {
Screen->Cursor=Save_Cursor;
//}
// viewfile.close();
 mirrorfile.close();outfile2.close();outfile3.close();
/////////////////////////////////////////////////////////////
 sleep(5); //Allow real time 5 seconds for file to close... Needs <dos.h>
 ifstream infile3("scratch0.txt");
 ofstream outfile3a("preWARP.txt");
 int hcflag1=0,hcrec=3,hc_stor[500];float timesum=0.f,time1=0.f,time_stor[500];
 infile3>>time1>>hcflag1;
//honk<<hcflag1<<" "<<time1<<" first\n";
//if(1==1)exit(0);
 hc_stor[0]=hcflag1;time_stor[0]=time1; //Presume time_stor[0]=0.
 for(i=0;i<wp.nWeldPass;i++){for(j=0;j<100;j++){infile3>>time1>>hcflag1;
//honk<<hcflag1<<" "<<time1<<" secnd\n";
//if(j>13)exit(0);
													hc_stor[j+1]=hcflag1;time_stor[j+1]=time1;
													if(hcflag1 != hcrec){timesum=time1;
//honk<<"BREAKING0\n";
																		 break;}
//if(j==11){honk<<"BREAKING1\n";timesum=time1;break;}
												   }
							 outfile3a<<time_stor[0]<<" "<<hc_stor[0]<<" "<<(timesum-time_stor[0])<<"\n";
//							 for(k=1;k<j;k++)outfile3a<<time_stor[k]<<" "<<hc_stor[k]<<" "<<timesum-time_stor[k]<<"\n";
							 for(k=1;k<j+1;k++)outfile3a<<time_stor[k]<<" "<<hc_stor[k]<<" "<<(timesum-time_stor[k])<<"\n";
							 hc_stor[0]=hcflag1;time_stor[0]=time1;timesum=timesum+wp.timeInterval[i];
							 for(j=0;j<wp.stepInterval[i];j++){infile3>>time1>>hcflag1;
//honk<<hcflag1<<" "<<time1<<" third\n";
															   hc_stor[j+1]=hcflag1;time_stor[j+1]=time1;
															  }
//if(1==1)exit(0);
							 outfile3a<<time_stor[0]<<" "<<hc_stor[0]<<" "<<(timesum-time_stor[0])<<"\n";
//							 for(k=1;k<j;k++)outfile3a<<time_stor[k]<<" "<<hc_stor[k]<<" "<<timesum-time_stor[k]<<"\n";
							 for(k=1;k<wp.stepInterval[i];k++)outfile3a<<time_stor[k]<<" "<<hc_stor[k]<<" "<<(timesum-time_stor[k])<<"\n";
							 if(i==wp.nWeldPass-1)outfile3a<<time_stor[wp.stepInterval[i]]<<" "<<hc_stor[wp.stepInterval[i]]<<" "<<0.<<"\n";
							 hc_stor[0]=hcflag1;time_stor[0]=time1;
							}
 outfile3a.close();infile3.close();DeleteFile(L"scratch0.txt");

// ofstream outfile4("uexternal_data_file.inp");
// outfile4<<"!  Three non-comment lines with file names required\n";
// outfile4<<"!   1 - name of material.dat file for VFT\n";
// outfile4<<"!   2 - name of VED.dat file\n";
// outfile4<<"!   3 - root of file names for thermal profiles\n";
// outfile4<<"!       There must be file names with extensions\n";
// outfile4<<"!       *.txt & *.bin\n";
// outfile4<<"!       Omit extensions here.\n";
// outfile4<<"!\n";
// outfile4<<"!  File names may have ~/ to denote user home directory.\n";
// outfile4<<"!  WARP3D will resolve to full path name.\n";
// outfile4<<"!\n";
// outfile4<<"./material.dat\n";
// outfile4<<"./VED.dat\n";
// outfile4<<"./warp_temp_2_files\n";
// outfile4<<"!\n";
// outfile4<<"!  Stop when analysis for this thermal profile completed.\n";
// outfile4<<"!  If this number of profiles is not defined, WARP3D will\n";
// outfile4<<"!  write output files, a restart file, and execute normal termination.\n";
// outfile4<<"!\n";
//// outfile4<<"  10\n";
// outfile4<<"  10000\n";
// outfile4<<"!\n";
// outfile4<<"!  Values to control output:\n";
// outfile4<<"!  - number of thermal profiles between saving restart file\n";
// outfile4<<"!  - number of thermal profiles between generation of output file\n";
// outfile4<<"!  - file of WARP3D output commands to be executed\n";
// outfile4<<"!\n";
//// outfile4<<"  500, 500\n";
// outfile4<<"  10000, 10\n";
// outfile4<<"output_commands.inp\n";
// outfile4<<"!\n";
// outfile4<<"!  Values to control solution when:\n";
// outfile4<<"!  o - a torch or torches comes on\n";
// outfile4<<"!  o - cooling starts\n";
// outfile4<<"!  o - analysis startup and on restarts\n";
// outfile4<<"!\n";
// outfile4<<"! - (N1) number of sequential thermal profiles over which\n";
// outfile4<<"!        to use a larger number of WARP3D load steps\n";
// outfile4<<"! - (N2) number of increased load steps to use (>=1) for\n";
// outfile4<<"!        solution over these profiles\n";
// outfile4<<"!\n";
//// outfile4<<"  2, 5\n";
// outfile4<<"  1, 2\n";
// outfile4<<"!  Value to control solution when:\n";
// outfile4<<"!  o - heating is occurring and has continued beyond N1 above\n";
// outfile4<<"!  o - cooling is occurring and has continued beyond N1 above\n";
// outfile4<<"! - (N1) number of sequential thermal profiles over which\n";
// outfile4<<"!\n";
// outfile4<<"!  N3 =1 is the most common value.\n";
// outfile4<<"!\n";
//// outfile4<<"  1\n";
// outfile4<<"  2\n";
// outfile4.close();
// ofstream outfile5("output_commands.inp");
//// outfile5<<"!  Put WARP3D output commands here. These will be executed after\n";
//// outfile5<<"!  solution for a profile completes at the profile frequency \n";
//// outfile5<<"!  specified in\n";
//// outfile5<<"!  uexternal_data_file.inp\n";
//// outfile5<<"!\n";
//// outfile5<<"   output displacements nodes 10000-10300\n";
//// outfile5<<"   output displacements elements 6000-6050 by 2\n";
//// outfile5<<"   output totals only reactions nodes all\n";
//// outfile5<<"   output wide eformat noheader strains 91240-91250,\n";
//// outfile5<<"       156320-157000 by 3\n";
//// outfile5<<"   output wide stresses 156320-157000 by 3\n";
//// outfile5<<"   output patran binary displacements stresses strains\n";
//// outfile5<<"   output patran binary element stresses strains\n";
// outfile5<<"!  DEFAULT FOR WSO, IF YOU NEED STRESS/STRAIN, COMMENT Line 3,\n";
// outfile5<<"!  UNCOMMENT OTHER LINES AS APPROPRIATE\n";
// outfile5<<"!\n";
// outfile5<<"!    Use patran files for old-version pat2exii until Python\n";
// outfile5<<"!    version becomes faster. Then switch to output flat ..\n";
// outfile5<<"!    commands.\n";
// outfile5<<"!\n";
// outfile5<<"  output patran formatted displacements\n";
// outfile5<<"  output patran formatted stresses\n";
// outfile5<<"  output patran formatted temperatures\n";
// outfile5<<"!\n";
// outfile5<<"!  output patran binary element strains stresses\n";
// outfile5<<"!  output totals only reactions nodes all\n";
// outfile5<<"!\n";
// outfile5<<"!  output flat text nodal stresses\n";
// outfile5<<"!  output flat text nodal temperatures\n";
// outfile5.close();

/////////////////////////////////////////////////////////////

// char extensChar1[]="_CTSP_param.in";char *fnNeed1=new char[strlen(gVFTnameStem)+strlen(extensChar1)+1];
// StringCchCopy(fnNeed1,strlen(gVFTnameStem)+strlen(extensChar1)+1,gVFTnameStem);StringCchCat(fnNeed1,strlen(gVFTnameStem)+strlen(extensChar1)+1,extensChar1);
// ofstream view1file(fnNeed1);delete [] fnNeed1;
 ofstream mirror1file("param.in");

 k=0;for(i=0;i<wp.nWeldPass;i++)if(k<wp.n_curr_sttEl[i])k=wp.n_curr_sttEl[i]; //Moved up here per EK format  EFP 6/26/2012
// view1file<<mstep<<"\n";
 mirror1file<<mstep<<"\n"; //mstep= #timesteps
 if(mseg<2)mseg=2; //EK directive  12/22/2011
// view1file<<mseg<<"\n";
 mirror1file<<mseg<<"\n"; //mseg= Max #segments along weldpass (same for moving arc & lumped mass)
// view1file<<4<<"\n";
 mirror1file<<4<<"\n"; //maxps= #passes (min 4 EK directive 12/22/2011, BB thinks that this is a weird unused CTSP parameter)
// view1file<<max(long(11),k)<<"\n";
 mirror1file<<max(long(11),k)<<"\n"; //maxsc= #heat sources = 11 (i.e. Policy: new EK format)
// view1file<<wp.nWeldPass<<"\n";
 mirror1file<<wp.nWeldPass<<"\n"; //maxwd= #weldpasses
// view1file<<maxelp+1<<"\n";
 mirror1file<<(maxelp+1)<<"\n"; //Correction 3/08/2012 //maxelp= Max #elements in a weldpass (+1 EK directive 12/22/2011)
// view1file<<k<<"\n";
 mirror1file<<k<<"\n"; //maxelcs= Max #elements a weldpass cross-section
 if(medge<1)medge=1; //Policy: default to min 1  EFP 10/12/2011
// view1file<<medge<<"\n";
 mirror1file<<medge<<"\n"; //medge= #edges with edge effects
 icount=0;for(i=0;i<base.npoin;i++)if(icount<base.node_map[i])icount=base.node_map[i];
// view1file<<icount+1<<"\n";
 mirror1file<<(icount+1)<<"\n"; //Highest occurring node number EFP 1/06/2012
////cccccccccccccccc start Time taken for this weld set incl. cooling [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
// view1file<<timeWeld<<"\n";
 mirror1file.precision(8);
 mirror1file<<timeWeld<<"\n";
 mirror1file.precision(6);
// view1file<<1<<"\n";
 mirror1file<<1<<"\n"; //No merging needed but include for consistency  EFP 5/20/2012
// view1file<<wp.tcutl[0]<<"\n";
 mirror1file.precision(8);
 mirror1file<<wp.tcutl[0]<<"\n"; //Correction: Low-cut temperature for 1st WP  EFP 5/22/2012
 mirror1file.precision(6);
////cccccccccccccccc end
////cccccccccccccccc start 8 extra data cards EFP 8/19/2012
// view1file<<solidshellsw<<"\n";
 mirror1file<<solidshellsw<<"\n"; //solidshellsw  EFP 5/20/2012
// view1file<<overlap<<"\n";
 mirror1file.precision(8);
 mirror1file<<overlap<<"\n"; //Core-to-core overlap time (sec) EFP 5/20/2012
// view1file<<wp.tcuth[0]<<"\n";
 mirror1file<<wp.tcuth[0]<<"\n"; //Pre-heat temperature for 1st WP  EFP 5/22/2012
 mirror1file.precision(6);
// view1file<<base.el_map[base.nelt-1]+1<<"\n";
 mirror1file<<(base.el_map[base.nelt-1]+1)<<"\n"; //Global max element#  EFP 9/16/2012
// view1file<<0<<"\n";
 mirror1file<<0<<"\n"; //(should be delt for multicore)extra  EFP 5/20/2012
// view1file<<lasttimerec<<"\n";
 mirror1file.precision(8);
 mirror1file<<lasttimerec<<"\n"; //IP cooling time on last WP per core (sec) EFP 9/23/2012
 mirror1file.precision(6);
// view1file<<lastStepIntv<<"\n";
 mirror1file<<lastStepIntv<<"\n"; //IP cooling steps on last WP per core EFP 5/20/2012
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
////10 more
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file<<0<<"\n";
 mirror1file<<0<<"\n";
// view1file.close();
 mirror1file.close();
}
//---------------------------------------------------------------------------
// void export_CTSP_INPUT_mcm(int,int,int,char []);
// export_CTSP_INPUT_mcm(itranche,1,wp.nWeldPass,gWsiAlias.t_str());
//void TForm1::export_CTSP_INPUTp_mcm(long mcmlo,long mcmup,int isw,float lasttimerec,char gVFTnameStem[]) //Version with mirror file  EFP 3/26/2011
void TForm1::export_CTSP_INPUTp_mcm(long mcmlo,long mcmup,int isw,float lasttimerec) //Version with mirror file  EFP 3/26/2011
//mcm= multi-core manual;mcmlo= lower limit (min 1);mcmup=upper limit (max wp.nWeldPass)  EFP 10/28/2011
// isw=0 ->Extra cooling OFF; 1->Extra cooling ON (e.g. last WP per core, except last core)
{long i=0,k=0,icount=0,//steptotal=0,
	  eles_size=0,iseq=0,mstep=0,medge=0,mseg=0,mWPcs=0,maxelp=0,laststep=0,
	  ipp=0,in=0,ie1=0,iside1=0,is1=0,laplim=0,LIMOVER=5; //This line junk EFP 6/16/2012
//	  ipp=0,in=0,ie1=0,iside1=0,is1=0,j=0,laplim=0,LIMOVER=500; //This line junk EFP 6/16/2012
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7},
	 opp_arr8[6]={2,3,0,1,5,4},solidshellsw=0; //0=solid, 1=shell  EFP 8/19/2012
 float timeWeld=0.f,highestTmelt=0.f,lasttime=0.f,dist=0.f,xc=0.f,yc=0.f,zc=0.f,astart=0.f,lasttimecom=0.f,biasintv=1.5f;
 double delt=0.f;

 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
 ofstream mirrorfile("input.in");mirrorfile.setf(ios::scientific);

 highestTmelt=0.f; //Policy: use globally highest Tmelt on each core, regardless of the WPs on that core
 for(iseq=mcmlo-1;iseq<mcmup;iseq++)// Correction  EFP 10/30/2011
   {for(k=0;k<wp.nWeldPass;k++)if(iseq+1==wp.seqNum[k])break;
	if(highestTmelt<wp.tmelt[k])highestTmelt=wp.tmelt[k]; //EFP 12/25/2014
   }
// TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
	  medge=mseg=icount=maxelp=0; //Corrected EFP 5/12/2010
	  icount++; //Bud's correction  EFP 12/13/2012
	  for(iseq=mcmlo-1;iseq<mcmup;iseq++)// Correction  EFP 10/30/2011
		{for(k=0;k<wp.nWeldPass;k++)if(iseq+1==wp.seqNum[k])break;
		 for(i=0;i<wp.memWGa;i++)if(wp.eles[wp.memWGa*k+i]<0)break;
		 icount=icount+i/wp.n_curr_sttEl[k] +wp.stepInterval[k]; //Corrected EFP 5/12/2010
		 if(maxelp<i)maxelp=i; //EFP 3/30/2012
		 laststep=wp.stepInterval[k];lasttime=wp.timeInterval[k];
////		 astart=exp(log(lasttime)-float(laststep-1)*log(1.2));  //EFP 1/25/2013
////		 LIMOVER=1+long(log(lasttimerec/astart)/log(1.2));
////		 lasttimecom=exp(log(astart)+float(LIMOVER-1)*log(1.2));
//		 astart=exp(log(lasttime)-float(laststep-1)*log(biasintv));  //EFP 1/25/2013
//		 LIMOVER=1+long(log(lasttimerec/astart)/log(biasintv));
//		 lasttimecom=exp(log(astart)+float(LIMOVER-1)*log(biasintv));

//		 astart=lasttime*((biasintv-1.f)/(std::pow(biasintv,float(laststep))-1.f));  //EFP 1/29/2013
//		 LIMOVER=long(0.001f+log(1.f+(biasintv-1.f)*(lasttimerec/astart))/log(biasintv));
//		 lasttimecom=astart*((std::pow(biasintv,float(LIMOVER))-1.f)/(biasintv-1.f));
		 astart=lasttime*((biasintv-1.f)/(std::pow((double)biasintv,double(laststep))-1.f));  //EFP 1/29/2013
		 LIMOVER=long(0.001f+log(1.f+(biasintv-1.f)*(lasttimerec/astart))/log(biasintv));
		 lasttimecom=astart*((std::pow((double)biasintv,double(LIMOVER))-1.f)/(biasintv-1.f));
		}
/////////////////////////
//honk<<k<<" "<<laststep<<" "<<lasttime<<" kkkkkk "<<biasintv<<"\n";
//honk<<astart<<" "<<LIMOVER<<" "<<lasttimecom<<" astart,LIMOVER,lasttimecom\n";
/////////////////////////
	  if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #time step data",MB_OK);exit(0);}
//////////////////
//cccccccccccccccc start Extra cooling steps for multi-core  EFP 5/20/2012
	  laplim=icount-laststep;if(isw)icount=icount+max(LIMOVER,laststep)-laststep;
//////////////////
	  mstep=icount;
// Note on multi-core option:      EFP 9/03/2012
// Core mstep is not the actual number of times in core time.out.
// Core mstep= SummationOverWPcore(1+ i/wp.n_curr_sttEl[k] +wp.stepInterval[k])
// Actual #times in core time.out= (mstep-#WPcore) +1
// Accordingly, parameter core laplim (below)= mstep-#WPcore +1 -lastStepIntv
////////////////////////////////////////////
	  mirrorfile<<"1, "<<icount<<"\n";
	  mirrorfile<<wms.cond[0]<<", "<<wms.heat[0]<<", "<<wms.den[0]<<", "<<wp.eff[0]<<", "<<(mcmup-mcmlo+1)<<", 0.0\n";//Extra 0 at end (i.e. new format) BB 4/16/2010
//// No Preheat Temperature Version
//// Next Line otherwise delete
////          sb.append(wp.troom + ", " + wp.tmelt  + ", " + wp.tcutl + ", " + wp.source + ", 1, " + wp.maxiHeatStep + ", " + wp.miniHeatStep + "\n");
//// tcuth is used for Preheat Temperature
//// Activate the next line for writing Preheat Temperature
////          sb.append(wp.tcuth + ", "+ wp.troom + ", " + wp.tmelt  + ", " + wp.tcutl + ", " + wp.source + ", 1, " + wp.maxiHeatStep + ", " + wp.miniHeatStep + "\n");
//	  mirrorfile<<wp.tcuth[0]<<", "<<wp.troom[0]<<", "<<wp.tmelt[0]<<", "<<wp.tcutl[0]<<", "<<wp.source[0]<<", 1, "<<wp.maxiHeatStep[0]<<", "<<wp.miniHeatStep[0]<<"\n";
	  mirrorfile<<wp.tcuth[0]<<", "<<wp.troom[0]<<", "<<highestTmelt<<", "<<wp.tcutl[0]<<", "<<wp.source[0]<<", 1, "<<wp.maxiHeatStep[0]<<", "<<wp.miniHeatStep[0]<<"\n";
//	  steptotal=0;
	  mWPcs=0;timeWeld=0.f;
// boolFlags[]: showFlag=1st digit,circFlag=2nd,edgeFlag=3rd,merge=4th,girthFlag=5th
	  for(iseq=mcmlo-1;iseq<mcmup;iseq++)
		{for(k=0;k<wp.nWeldPass;k++)if(iseq+1==wp.seqNum[k])break;
//		 mirrorfile<<"***** Weld Pass "<<wp.name[k].t_str()<<" *****\n";
//		 mirrorfile<<"***** Weld Pass "<<wp.name[k].w_str()<<" *****\n";
//xxxxxxxxxx
 int bufferSize=WideCharToMultiByte(CP_UTF8,0,wp.name[k].w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,wp.name[k].w_str(), -1,m,bufferSize,NULL,NULL);
 mirrorfile<<"***** Weld Pass "<<m<<" *****\n";
 delete[] m;
//xxxxxxxxxx
		 if(mWPcs<wp.n_curr_sttEl[k])mWPcs=wp.n_curr_sttEl[k];
		 int circFlag=(wp.boolFlags[k]-100*(wp.boolFlags[k]/100))/10;
		 int edgeFlag=(wp.boolFlags[k]-1000*(wp.boolFlags[k]/1000))/100;
		 int merge=(wp.boolFlags[k]-10000*(wp.boolFlags[k]/10000))/1000;
		 int girthFlag=(wp.boolFlags[k]-100000*(wp.boolFlags[k]/100000))/10000;
		 icount=0;
		 for(i=4*(mcmlo-1);i<4*mcmup;i++)if(wp.edgeNodes[i]> -1)icount++;else break;
		 int numEdge = edgeFlag ? icount : 0;
///////////////////////// start EFP 10/12/2011
		 if(numEdge)medge++; //EFP 10/12/2011
///////////////////////// end
		 int cirWeld = circFlag ? wp.shape[k] : 0;
		 int girthWeld=0;
		 if(wp.shape[k]==3){girthWeld=1;cirWeld=0;}
		 if(wp.shape[k]==4){girthWeld=2;cirWeld=0;}
		 int lumpWeld=wp.nsegs[k]; //Lump Pass (1) or Moving Arc (0) Heating Procedure
		 if(wp.hp[k]==0)lumpWeld=0;
		 wp.boolFlags[k]=wp.boolFlags[k]-merge*1000; //Zero merge component
		 if(k==0)merge=0;
		 if(merge>0)wp.boolFlags[k]=wp.boolFlags[k]+merge*1000;
		 int mcond=0;if(merge)mcond=1;
		 mirrorfile<<"1, "<<(wp.type[k] + 1)<<", "<<wp.thk1[k]<<", "<<wp.thk2[k]<<", "<<numEdge<<", "<<cirWeld<<", "<<lumpWeld<<", "<<mcond<<", "<<girthWeld<<"\n";
		 mirrorfile<<(base.node_map[wp.snorm1[4*k+1]]+1)<<", "<<(base.node_map[wp.snorm1[4*k+2]]+1)<<", "<<(base.node_map[wp.snorm1[4*k+3]]+1)<<", "; // Note that these are the 2/3/4 nodes, not 1
		 mirrorfile<<(base.node_map[wp.snorm2[4*k+1]]+1)<<", "<<(base.node_map[wp.snorm2[4*k+2]]+1)<<", "<<(base.node_map[wp.snorm2[4*k+3]]+1)<<"\n";
		 if(iseq==mcmup-1 && isw){ //EFP 6/07/2012
//		 mirrorfile<<lasttimerec<<", "<<max(LIMOVER,laststep)<<", "<<wp.speed[k]<<", "<<wp.curr[k] * wp.volt[k]<<"\n";
//honk<<lasttimerec<<", "<<max(LIMOVER,laststep)<<", "<<wp.speed[k]<<", "<<wp.curr[k] * wp.volt[k]<<" mcmCTSP overlapON "<<iseq<<" "<<mcmlo-1<<" "<<mcmup<<" "<<k<<"\n";
		 mirrorfile<<lasttimecom<<", "<<max(LIMOVER,laststep)<<", "<<wp.speed[k]<<", "<<(wp.curr[k] * wp.volt[k])<<"\n";
//honk<<lasttimecom<<", "<<max(LIMOVER,laststep)<<", "<<wp.speed[k]<<", "<<wp.curr[k] * wp.volt[k]<<" mcmCTSP overlapON "<<iseq<<" "<<mcmlo-1<<" "<<mcmup<<" "<<k<<"\n";
								 }
		 else {
		 mirrorfile<<wp.timeInterval[k]<<", "<<wp.stepInterval[k]<<", "<<wp.speed[k]<<", "<<(wp.curr[k] * wp.volt[k])<<"\n";
//honk<<wp.timeInterval[k]<<", "<<wp.stepInterval[k]<<" "<<wp.speed[k]<<", "<<wp.curr[k] * wp.volt[k]<<" mcmCTSP overlapOFF "<<iseq<<" "<<mcmlo-1<<" "<<mcmup<<" "<<k<<"\n";
			  }
		 icount=0;for(i=wp.memWGa*k;i<wp.memWGa*(k+1);i++){if(wp.sttEles[i]> -1)icount++;else break;}
		 if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #weld start elements",MB_OK);exit(0);}
//		 sttEles_size=icount;
		 mirrorfile<<icount<<"\n"; //sttEles_size=icount
		 for(i=0;i<icount;i++)mirrorfile<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+0]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+1]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+2]]+1)<<", "<<(base.node_map[wp.sttEleNodes[4*wp.memWGa*k+4*i+3]]+1)<<"\n";
		 icount=0;for(i=wp.memWGa*k;i<wp.memWGa*(k+1);i++){if(wp.eles[i]> -1)icount++;else break;}
		 if(!icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Terminate: Corrupt data",L"Writing zero #weld group elements",MB_OK);exit(0);}
		 eles_size=icount;
/////////////////////////// EFP 10/12/2011
		 if(mseg<icount/wp.n_curr_sttEl[k])mseg=icount/wp.n_curr_sttEl[k];
///////////////////////////
		 mirrorfile<<icount;
		 for(i=0;i<icount;i++){if(i%10 == 0)mirrorfile<<"\n";
							   mirrorfile<<(base.el_map[wp.eles[wp.memWGa*k+i]/10] +1); //Corrected EFP 9/10/2010
							   if(i%10 != 9 && i != icount-1)mirrorfile<<", ";
							  }
		 mirrorfile<<"\n";

//		 if(iseq==mcmup-1 && isw){          //EFP 6/07/2012
//		 if(wp.hp[k]==0)steptotal += laststep+eles_size/sttEles_size;
//		 else if(wp.hp[k]==1)steptotal += laststep+(wp.nsegs[k]-1)*5+ (wp.nsegs[k])*20 ;
//								 }
//		 else {
//		 if(wp.hp[k]==0)steptotal += wp.stepInterval[k]+eles_size/sttEles_size;
//		 else if(wp.hp[k]==1)steptotal += wp.stepInterval[k]+(wp.nsegs[k]-1)*5+ (wp.nsegs[k])*20 ;
//			  }

//cccccccccccccccc start [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
		 timeWeld=timeWeld+wp.timeInterval[k]+wp.lend[k]/wp.speed[k];
//cccccccccccccccc end
		 for(i=0;i<numEdge;i++)mirrorfile<<(base.node_map[wp.edgeNodes[4*k+i]]+1)<<"\n"; //Edge Effect (Correction EFP 4/09/2011
//Circular Weld
		 if(circFlag){for(i=0;i<3;i++){mirrorfile<<(base.node_map[wp.circEles[3*k+i]]+1); // Corrected EFP 9/10/2010
									   if(i<2)mirrorfile<<", ";
									  }
					  mirrorfile<<"\n";
					 }
//Girth Weld
		 if(girthFlag){for(i=0;i<3;i++){mirrorfile<<(base.node_map[wp.circEles[3*k+i]]+1); // Corrected EFP 9/10/2010
										if(i<2)mirrorfile<<", ";
									   }
					   mirrorfile<<"\n";
					  }
///////////// start NEW CODING EFP 6/19/2012  reactivated 9/23/2012 EFP
						  ipp=eles_size/wp.n_curr_sttEl[k]-1;dist=0.f; //Last slice only
					   for(in=0;in<wp.n_curr_sttEl[k];in++){
ie1=wp.eles[wp.memWGa*k+wp.n_curr_sttEl[k]*ipp+in]/10;iside1=wp.eles[wp.memWGa*k+wp.n_curr_sttEl[k]*ipp+in]-10*ie1;
xc=yc=zc=0.f;
for(is1=0;is1<4;is1++){xc=xc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+0]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+0];
					   yc=yc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+1]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+1];
					   zc=zc+base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*iside1+is1]]+2]-base.c1[NDF*base.nop1[MXNPEL*ie1+gdata8[4*opp_arr8[iside1]+is1]]+2];
					  }
xc=xc/4.f;yc=yc/4.f;zc=zc/4.f;dist=dist+sqrt(xc*xc+yc*yc+zc*zc);//EFP dislikes this non-area-weighted centroid
														   }
delt=dist/(float(wp.n_curr_sttEl[k])*wp.speed[k]); //We require the last delt of last WP only
		}
	  mirrorfile<<"-1\n";
 mirrorfile.close();
 ofstream mirror1file("param.in");

 mirror1file<<mstep<<"\n"; //mstep= #timesteps (but not #times in time.out)
 if(mseg<2)mseg=2; //EK directive  12/22/2011 THIS WAS MISSING
 mirror1file<<mseg<<"\n"; //mseg= Max #segments along weldpass (same for moving arc & lumped mass)
 mirror1file<<4<<"\n"; // THIS WAS ONLY 1. maxps= #passes (min 4 EK directive 12/22/2011, BB thinks that this is a weird unused CTSP parameter)
 mirror1file<<max(long(11),mWPcs)<<"\n"; //maxsc= #heat sources = 11 (i.e. Policy: new EK format 6/26/2012)
 mirror1file<<(mcmup-mcmlo+1)<<"\n"; //maxwd= #weldpasses  EFP 10/30/2011
 mirror1file<<(maxelp+1)<<"\n"; //Correction 3/30/2012 //maxelp= Max #elements in a weldpass (+1 EK directive 12/22/2011)
 mirror1file<<mWPcs<<"\n"; //maxelcs= Max #elements a weldpass cross-section
 if(medge<1)medge=1; //Policy: default to min 1  EFP 10/12/2011
 mirror1file<<medge<<"\n"; //medge= #edges with edge effects
 icount=0;for(i=0;i<base.npoin;i++)if(icount<base.node_map[i])icount=base.node_map[i];
 mirror1file<<(icount+1)<<"\n"; //Highest occurring node number EFP 1/06/2012
//cccccccccccccccc start Time taken for this weld-set incl. user-specified cooling, not overlap [EFP does not approve of this non-element-area-based algorithm] EFP 5/18/2012
 mirror1file.precision(8);
 mirror1file<<timeWeld<<"\n";
 mirror1file.precision(6);
 mirror1file<<laplim<<"\n"; //"laplim" needed for overlap & merging  EFP 9/03/2012 Correction Bud 12/13/2012
 mirror1file.precision(8);
 mirror1file<<wp.tcutl[0]<<"\n"; //Correction: Low-cut temperature for 1st WP  EFP 5/22/2012
 mirror1file.precision(6);
////cccccccccccccccc end
////cccccccccccccccc start 8 extra data cards EFP 8/19/2012
 mirror1file<<solidshellsw<<"\n"; //solidshellsw  EFP 5/20/2012
// mirror1file<<lasttimerec<<"\n"; //Core-to-core overlap  EFP 5/20/2012
 mirror1file.precision(8);
 mirror1file<<lasttimecom<<"\n"; //Core-to-core overlap  EFP 1/25/2013
 mirror1file<<wp.tcuth[0]<<"\n"; //Pre-heat temperature for 1st WP  EFP 5/22/2012
 mirror1file.precision(6);
 mirror1file<<(base.el_map[base.nelt-1]+1)<<"\n"; //Global max element#  EFP 9/16/2012
 mirror1file.precision(8);
 mirror1file<<delt<<"\n"; //Data for standalone mergeCTSPcore  EFP 9/23/2012
 mirror1file<<lasttime<<"\n"; //user-spec IP cooling time on last WP per core (sec) EFP 9/23/2012
 mirror1file.precision(6);
 mirror1file<<laststep<<"\n"; //user-spec IP cooling steps on last WP per core  EFP 9/23/2012
 mirror1file<<0<<"\n";
////10 more
 mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";
 mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";mirror1file<<0<<"\n";
 mirror1file.close();
//	 }
Screen->Cursor=Save_Cursor;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::exportWARP3DExecute(TObject *Sender)
{if(base.nop1)
   {if(wp.nWeldPass)
	  {

	   ABAQnames=new TForm25(2,this);

//honk<<" WARPing0 here\n";
	   ABAQnames->Caption=L"Enter file names for WARP3D input deck.";
	   ABAQnames->Label1->Caption=L"WARP3D main file";
	   ABAQnames->Label2->Caption=L"WARP3D node file";
	   ABAQnames->Label3->Caption=L"WARP3D element file";
	   ABAQnames->Label4->Caption=L"WARP3D VED file";
	   ABAQnames->Label7->Caption=L".wrp";
	   ABAQnames->Label8->Caption=L".coordinates";
	   ABAQnames->Label9->Caption=L".incid";
	   ABAQnames->Label10->Caption=L"_VED.inp";
	   ABAQnames->Button1->Caption=L"OK";
	   ABAQnames->Button2->Caption=L"Cancel";
	   ABAQnames->Label2->Enabled=false;ABAQnames->Label3->Enabled=false;ABAQnames->Label4->Enabled=false;
	   ABAQnames->Label5->Enabled=false;ABAQnames->Label6->Enabled=false;ABAQnames->Label8->Enabled=false;
	   ABAQnames->Label9->Enabled=false;ABAQnames->Label10->Enabled=false;ABAQnames->Label11->Enabled=false;
	   ABAQnames->Edit2->Visible=false;ABAQnames->Edit3->Visible=false;ABAQnames->Edit4->Visible=false;
	   ABAQnames->Edit5->Visible=false;ABAQnames->Edit6->Visible=false;
//	   ABAQnames->CheckEdit1=gWsiAlias.t_str();
	   ABAQnames->CheckEdit1=gWsiAlias.w_str();
	   ABAQnames->ShowModal();

	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Must create weld passes->Weld",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::exportWARP3D_public()
{

	   gWsiAlias=ABAQnames->CheckEdit1;
//honk<<" WARPing0a\n";
////xxxxxxxxxx
// int bufferSize1=WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,NULL,0,NULL,NULL);
// char* m1=new char[bufferSize1];WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,m1,bufferSize1,NULL,NULL);
// honk<<m1<<"\n";// EFP 12/10/2014
// delete[] m1;
////xxxxxxxxxx
	   ABAQnames->CheckIsel=3;ABAQnames->Edit2->Visible=true;ABAQnames->Edit3->Visible=true;
	   ABAQnames->Edit4->Visible=true;
	   ABAQnames->Edit5->Visible=false;
	   ABAQnames->Edit6->Visible=false;
	   ABAQnames->Label1->Enabled=false;
	   ABAQnames->Label2->Enabled=true;ABAQnames->Label3->Enabled=true;ABAQnames->Label4->Enabled=true;
	   ABAQnames->Label5->Enabled=false;
	   ABAQnames->Label6->Enabled=false;
	   ABAQnames->Label8->Enabled=true;
	   ABAQnames->Label9->Enabled=true;ABAQnames->Label10->Enabled=true;
	   ABAQnames->Label11->Enabled=false;
	   ABAQnames->CheckEdit1=gWsiAlias;ABAQnames->CheckEdit2=gWsiAlias;ABAQnames->CheckEdit3=gWsiAlias;
	   ABAQnames->CheckEdit4=gWsiAlias;
	   ABAQnames->Edit1->Enabled=false;ABAQnames->Button1->Caption=L"Write files";

}
//---------------------------------------------------------------------------
void TForm1::exportWARP3D1a_public()
{long i=0,eltype=0,bscode=0,node=0,t7=10000000,t5=100000,t3=1000;
 long isw=0;
 long ie=0,ies=0,j=0,k=0,ir=0,istart=0,iELSETtype=0,iELSETactive=0,ibrsw=0, *iELSETarr=NULL;
 float timesave2=0.f;
// long epStepsPerT=long(tdeltCTSP->Angle0 +0.5); //To reverse, comment this
 UnicodeString fnNeedS1,fnNeedS2,extensCharS1=UnicodeString(L".coordinates"),extensCharS2=UnicodeString(L".incid");
 wchar_t curMess0[]=L".wrp\n",    //curMess0[]=L"_ABA_input.inp\n",
		 curMess1[]=L"_VED.dat\n",curMess2[]=L".coordinates\n",
		 curMess3[]=L".incid\n",string0[160]; //Correction EFP 11/12/2012
 StringCchCopyW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess0);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess1);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess2);
 StringCchCatW(string0,160,gWsiAlias.w_str());StringCchCatW(string0,160,curMess3);
// char extensChar1[]=".coordinates";char *fnNeed1=new char[strlen(gWsiAlias.t_str())+strlen(extensChar1)+1];
// StringCchCopy(fnNeed1,strlen(gWsiAlias.t_str())+strlen(extensChar1)+1,gWsiAlias.t_str());StringCchCat(fnNeed1,strlen(gWsiAlias.t_str())+strlen(extensChar1)+1,extensChar1);
// char extensChar2[]=".incid";char *fnNeed2=new char[strlen(gWsiAlias.t_str())+strlen(extensChar2)+1];
// StringCchCopy(fnNeed2,strlen(gWsiAlias.t_str())+strlen(extensChar2)+1,gWsiAlias.t_str());StringCchCat(fnNeed2,strlen(gWsiAlias.t_str())+strlen(extensChar2)+1,extensChar2);

//honk<<" WARPing1\n";
 fnNeedS1=gWsiAlias+extensCharS1;
////xxxxxxxxxx
// int bufferSize1=WideCharToMultiByte(CP_UTF8,0,fnNeedS1.w_str(), -1,NULL,0,NULL,NULL);
// char* m1=new char[bufferSize1];WideCharToMultiByte(CP_UTF8,0,fnNeedS1.w_str(), -1,m1,bufferSize1,NULL,NULL);
// honk<<m1<<"\n";// EFP 12/10/2014
// delete[] m1;
////xxxxxxxxxx
 fnNeedS2=gWsiAlias+extensCharS2;
////xxxxxxxxxx
// int bufferSize2=WideCharToMultiByte(CP_UTF8,0,fnNeedS2.w_str(), -1,NULL,0,NULL,NULL);
// char* m2=new char[bufferSize2];WideCharToMultiByte(CP_UTF8,0,fnNeedS2.w_str(), -1,m2,bufferSize2,NULL,NULL);
// honk<<m2<<"\n";// EFP 12/10/2014
// delete[] m2;
////xxxxxxxxxx
 if(base.nop1)
   {
////	ofstream viewfile1("default.coordinates");
//	ofstream viewfile1(fnNeed1);
	ofstream viewfile1(fnNeedS1.w_str());
//	delete [] fnNeed1;
	if(viewfile1)
	  {TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
	   viewfile1<<"c\ncoordinates\n*echo off\n";
	   viewfile1.setf(ios::scientific);viewfile1.precision(6); //Correction BB 1/07/2013
	   for(i=0;i<base.npoin;i++)viewfile1<<(base.node_map[i]+1)<<" "<<base.c1[NDF*i]<<" "<<base.c1[NDF*i+1]<<" "<<base.c1[NDF*i+2]<<"\n";
	   viewfile1<<"*echo on\nc\n";viewfile1.close();
	   Screen->Cursor=Save_Cursor;
////	   ofstream viewfile2("default.incid_and_blocking");
//	   ofstream viewfile2(fnNeed2);
	   ofstream viewfile2(fnNeedS2.w_str());
//	   delete [] fnNeed2;
	   if(viewfile2) //8-n elements throughout
		 {TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
		  viewfile2<<"c\nincidences\n*echo off\n";

//		  for(i=0;i<base.nelt;i++)viewfile2<<base.el_map[i]+1<<" "<<
//base.nop1[MXNPEL*i+0]+1<<" "<<base.nop1[MXNPEL*i+1]+1<<" "<<base.nop1[MXNPEL*i+2]+1<<" "<<base.nop1[MXNPEL*i+3]+1<<" "<<
//base.nop1[MXNPEL*i+4]+1<<" "<<base.nop1[MXNPEL*i+5]+1<<" "<<base.nop1[MXNPEL*i+6]+1<<" "<<base.nop1[MXNPEL*i+7]+1<<"\n";

// Why no base.node_map[]?
				 for(i=0;i<base.nelt;i++){eltype=base.matno[i]/t7;bscode=(base.matno[i]-eltype*t7)/t5;node=(base.matno[i]-eltype*t7-bscode*t5)/t3;
										  if(eltype==8){if(node==8) //EFP 6/12/2014
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 4]+1)<<" "<<(base.nop1[MXNPEL*i+ 5]+1)<<" "<<(base.nop1[MXNPEL*i+ 6]+1)<<" "<<(base.nop1[MXNPEL*i+ 7]+1)<<"\n";
														else
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 4]+1)<<" "<<(base.nop1[MXNPEL*i+ 5]+1)<<" "<<(base.nop1[MXNPEL*i+ 6]+1)<<" "<<(base.nop1[MXNPEL*i+ 7]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 8]+1)<<" "<<(base.nop1[MXNPEL*i+ 9]+1)<<" "<<(base.nop1[MXNPEL*i+10]+1)<<" "<<(base.nop1[MXNPEL*i+11]+1)<<" "<<
									(base.nop1[MXNPEL*i+12]+1)<<" "<<(base.nop1[MXNPEL*i+13]+1)<<" "<<(base.nop1[MXNPEL*i+14]+1)<<" "<<(base.nop1[MXNPEL*i+15]+1)<<" "<<
									(base.nop1[MXNPEL*i+16]+1)<<" "<<(base.nop1[MXNPEL*i+17]+1)<<" "<<(base.nop1[MXNPEL*i+18]+1)<<" "<<(base.nop1[MXNPEL*i+19]+1)<<"\n";
													   }
										  else if(eltype==7){if(node==6)
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 4]+1)<<" "<<(base.nop1[MXNPEL*i+ 5]+1)<<"\n";
															 else
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 4]+1)<<" "<<(base.nop1[MXNPEL*i+ 5]+1)<<" "<<(base.nop1[MXNPEL*i+ 6]+1)<<" "<<(base.nop1[MXNPEL*i+ 7]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 8]+1)<<" "<<(base.nop1[MXNPEL*i+ 9]+1)<<" "<<(base.nop1[MXNPEL*i+10]+1)<<" "<<(base.nop1[MXNPEL*i+11]+1)<<" "<<
									(base.nop1[MXNPEL*i+12]+1)<<" "<<(base.nop1[MXNPEL*i+13]+1)<<" "<<(base.nop1[MXNPEL*i+14]+1)<<"\n";
															}
										  else if(eltype==5){if(node==4)
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<"\n";
															 else
viewfile2<<(base.el_map[i]+1)<<" "<<(base.nop1[MXNPEL*i   ]+1)<<" "<<(base.nop1[MXNPEL*i+ 1]+1)<<" "<<(base.nop1[MXNPEL*i+ 2]+1)<<" "<<(base.nop1[MXNPEL*i+ 3]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 4]+1)<<" "<<(base.nop1[MXNPEL*i+ 5]+1)<<" "<<(base.nop1[MXNPEL*i+ 6]+1)<<" "<<(base.nop1[MXNPEL*i+ 7]+1)<<" "<<
									(base.nop1[MXNPEL*i+ 8]+1)<<" "<<(base.nop1[MXNPEL*i+ 9]+1)<<"\n";
															}
										  else {
honk<<(i+1)<<" Unsupp in exportWARP3D1a_public() "<<eltype<<" "<<bscode<<" "<<node<<"\n";
												extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"exportWARP3D1a_public() unsupported element type",L"Terminate",MB_OK);
												exit(0);}
										 }


//		  viewfile2<<"c\n*echo on\nc\nc\nc\n*echo off\nblocking automatic\n"; //BobD request to remove BLOCKING 9/04/2014
		  viewfile2<<"c\n*echo on\nc\nc\nc\n*echo off\n";
		  viewfile2<<"c\nc   *** MPI + threads version: sparse and pcg solvers allowed\n";
		  viewfile2<<"c\nc   *** Threads version: sparse solver only\nc\n*echo on\n";
		  viewfile2.close();
////////		  export_VED("default",&timesave2); // VED= virtual element detection
//////		  export_VED(gWsiAlias.t_str(),&timesave2); // VED= virtual element detection
////		  export_VED(gWsiAlias.c_str(),&timesave2); // VED= virtual element detection
//		  export_VED(gWsiAlias,&timesave2); // VED= virtual element detection
		  export_VED(gWsiAlias,&timesave2,1); // VED= virtual element detection (Abaqus 0,WARP3D 1)
////		  WARP3DepBlock();
//		  WARP3DepBlock(gWsiAlias);
		  Screen->Cursor=Save_Cursor;
//		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"default_coordinates.COORDINATES & default_INCIDandBLOCK.INCIDandBLOCK\nwritten but user must use PATGO to create default_constraints.CONSTRAINTS\nBlocking info must be added to default_INCIDandBLOCK.INCIDandBLOCK",L"Successful first step",MB_OK);
honk<<"\n"<<" Writing four WARP3D datafiles...\n";
		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Writing 4 WARP3D files",MB_OK);
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

	   for(i=0;i<wp.nWeldPass;i++)
		 {for(ies=0;ies<wp.memWGa;ies++)
			{ie=wp.eles[wp.memWGa*i+ies]/10;
			 if(ie>=0){k=base.matno[ie]-t3*(base.matno[ie]/t3);
					   base.matno[ie]= -(base.matno[ie]-k+i+1);
					  }
			 else break;
			}
		 }
	   istart=0;j=1;
	   iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);
	   do {ibrsw=0;
		   for(i=istart+1;i<base.nelt;i++)
			 {k= labs(base.matno[i])-t3*(labs(base.matno[i])/t3);
			  if(iELSETtype != k){if(base.matno[istart]> -1)
								  j++;ibrsw=1;istart=i;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){if(base.matno[istart]> -1)
							j++;
						   }
	   iELSETactive=j;iELSETarr=new long[iELSETactive];for(j=0;j<iELSETactive;j++)iELSETarr[j]= -1;
	   istart=ies=0;
	   iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);
	   do {ibrsw=0;
		   for(i=istart+1;i<base.nelt;i++)
			 {k= labs(base.matno[i])-t3*(labs(base.matno[i])/t3);
			  if(iELSETtype != k){if(base.matno[istart]> -1){isw=1;if(ies){for(ir=0;ir<ies;ir++)if(iELSETarr[ir]==iELSETtype){isw=0;break;}}
															 if(isw){iELSETarr[ies]=iELSETtype;ies++;}
															}
								  ibrsw=1;istart=i;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){if(base.matno[istart]> -1){isw=1;if(ies){for(ir=0;ir<ies;ir++)if(iELSETarr[ir]==iELSETtype){isw=0;break;}}
													   if(isw){iELSETarr[ies]=iELSETtype;ies++;}
													  }
						   }
//	   Form7=new TForm7(base.allGrp,this);
	   Form7=new TForm7(ies,this);
	   Form7->Caption=L"Assign material files to non-WPs";
	   Form7->Label1->Caption=L"Entity";
	   Form7->Label2->Caption=L"Chosen material";
	   Form7->Label3->Caption=L"Available material";
	   Form7->Label4->Caption=L"Usage: First click on Entity; then click Available material";
	   Form7->Button1->Caption=L"OK";
	   for(ir=0;ir<ies;ir++)Form7->ListBox1->AddItem(base.ELSETinputnames[iELSETarr[ir]],this);
	   delete [] iELSETarr;
	   Form7->ListBox1->ItemIndex=0;
	   for(i=0;i<wms.nMatPropSet;i++)Form7->ListBox3->AddItem(wms.matFileName[i],this);
	   if(wms.nMatPropSet==1)for(i=0;i<ies;i++)Form7->ListBox2->AddItem(wms.matFileName[0],this);
	   else                  for(i=0;i<ies;i++)Form7->ListBox2->AddItem(L"****",this);
	   Form7->ShowModal();
	   delete Form7;// *Form7=NULL; (not in Unit1, remember, but perhaps we should not "delete"?)

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
//		  Screen->Cursor=Save_Cursor;
////		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"default_coordinates.COORDINATES & default_INCIDandBLOCK.INCIDandBLOCK\nwritten but user must use PATGO to create default_constraints.CONSTRAINTS\nBlocking info must be added to default_INCIDandBLOCK.INCIDandBLOCK",L"Successful first step",MB_OK);
//honk<<"\n"<<" Writing four WARP3D datafiles...\n";
//		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(string0,L"Writing 4 WARP3D files",MB_OK);
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unable to create default_INCID.INCID",L"Failure",MB_OK);}
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unable to create default_coordinates.COORDINATES",L"Failure",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
			   ABAQnames=NULL;
/////////// Cursor EFP 1/21/2011
Screen->Cursor=crSizeAll;
///////////

}
//---------------------------------------------------------------------------
//void TForm1::export_VED(char gVFTnameStem[],float *timesave2) // VED= virtual element detection  (based on WSIExportABA.java export_VED)
void TForm1::export_VED(String gVFTnameStem,float *timesave2,int isw) // VED= virtual element detection  (based on WSIExportABA.java export_VED)
// Note that the last number in the output file will still have a trailing comma.
// Export with element mapping
// Centroidal distance/speed coded & Correction for non-conforming faces EFP 5/24/2011
// Version with CTSP-compliant "element-based "centroid & weld length  EFP 3/16/2012
// (EFP recommends an "element-area-based" centroid)
// isw=1 WARP3D naming; 0 Abaqus
{//int n1=0,n2=0,n3=0,n4=0,n5=0,n6=0,n7=0,n8=0;
 long i=0,j=0,k=0,ja=0,ist=0,tranche=0,ieq=0,isideq=0,iseq=0;
//////////////////
// int gdata8[24]={0,1,5,4,
//				 1,2,6,5,
//				 3,2,6,7,
//				 0,3,7,4,
//				 0,1,2,3,
//				 4,5,6,7},
 int gdata8[24]={0,1,5,4, //Revised to get counterclock faces
				 1,2,6,5,
				 2,3,7,6,
				 3,0,4,7,
				 3,2,1,0,
				 4,5,6,7},
	 opp_arr8[6]={2,3,0,1,5,4};//EFP 3/16/2012
//////////////////
 float xfavg=0.f,yfavg=0.f,zfavg=0.f,xpatch=0.f,ypatch=0.f,zpatch=0.f,//dist2patch=1.e20f,dist2=0.f,
	   xup=0.f,yup=0.f,zup=0.f,time=0.f,ttime=0.f,wpTime=0.f,dist=0.f;

 UnicodeString fnNeedS,extensCharS=UnicodeString(L"_VED.dat");
// char extensChar[]="_VED.dat";
// char *fnNeed=new char[strlen(gVFTnameStem)+strlen(extensChar)+1];
//// char extensChar[]="_VEDdat.txt";char *fnNeed=new char[strlen(gVFTnameStem)+strlen(extensChar)+1];
// StringCchCopy(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,gVFTnameStem);StringCchCat(fnNeed,strlen(gVFTnameStem)+strlen(extensChar)+1,extensChar);
// ofstream viewfile(fnNeed);

// fnNeedS=gVFTnameStem+extensCharS;
//// if(isw)ofstream viewfile("VED.dat");
//// else
 if(isw)fnNeedS=L"VED.dat";else fnNeedS=gVFTnameStem+extensCharS;
 ofstream viewfile(fnNeedS.w_str());
// delete [] fnNeed;

  viewfile.setf(ios::scientific);

 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
// try {
	  time=0.f;
//	  for(i=0;i<wp.nWeldPass;i++)
	  for(iseq=0;iseq<wp.nWeldPass;iseq++) //Correction EFP 8/15/2011
		{for(i=0;i<wp.nWeldPass;i++)if(iseq+1==wp.seqNum[i])break;
//		{ist=0;for(j=base.nelt*i;j<base.nelt*(i+1);j++){if(wp.eles[j]> -1)ist++;else break;}
		 ist=0;for(j=wp.memWGa*i;j<wp.memWGa*(i+1);j++){if(wp.eles[j]> -1)ist++;else break;}
		 tranche=ist/wp.n_curr_sttEl[i];
//		 ttime=wpTime=0.;
		 wpTime=0.f;ttime=time;
/////////////////// trash
//ttimeA=ttimeE=time;
//honk<<"ElCenTime ArCenTime CenStTime for SEQ "<<(iseq+1)<<" and WP "<<(i+1)<<"\n";
//POSGP8[0]= -1.f/sqrt(3.f);POSGP8[1]=1.f/sqrt(3.f);WEIGP8[0]=WEIGP8[1]=  1.00000f;
///////////////////
//		 ie=wp.sttEles[wp.memWGa*i+0]/10;iside=wp.sttEles[wp.memWGa*i+0]-10*ie;
//		 if(iside==0 || iside==2){n1=0;n2=3;}
//		 else if(iside==1 || iside==3){n1=1;n2=0;}
//		 else {n1=4;n2=0;}

/////////////
/////////////
/////////////
xpatch=ypatch=zpatch=0.f;
for(j=0;j<wp.n_curr_sttEl[i];j++)
  {xfavg=yfavg=zfavg=0.f;ieq=wp.sttEles[wp.memWGa*i+j]/10;isideq=wp.sttEles[wp.memWGa*i+j]-10*ieq;
   for(k=0;k<4;k++){xfavg=xfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+0];
					yfavg=yfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+1];
					zfavg=zfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+2];
				   }
   xpatch=xpatch+xfavg/4.f;ypatch=ypatch+yfavg/4.f;zpatch=zpatch+zfavg/4.f;
  }
xpatch=xpatch/float(wp.n_curr_sttEl[i]);ypatch=ypatch/float(wp.n_curr_sttEl[i]);zpatch=zpatch/float(wp.n_curr_sttEl[i]);
//for(j=0;j<wp.n_curr_sttEl[i];j++)
//  {xfavg=yfavg=zfavg=0.f;ieq=wp.sttEles[wp.memWGa*i+j]/10;isideq=wp.sttEles[wp.memWGa*i+j]-10*ieq;
//   for(k=0;k<4;k++){xfavg=xfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+0];
//					yfavg=yfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+1];
//					zfavg=zfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+2];
// 				   }
//   dist2=(xpatch-xfavg/4.f)*(xpatch-xfavg/4.f)+(ypatch-yfavg/4.f)*(ypatch-yfavg/4.f)+(zpatch-zfavg/4.f)*(zpatch-zfavg/4.f);
//   if(dist2patch>dist2){cen_stt=j;dist2patch=dist2;} //Not used anymore
//  }



////////////// trash
//xpatchA=ypatchA=zpatchA=area=0.;
//for(j=0;j<wp.n_curr_sttEl[i];j++)
//  {xfavg=yfavg=zfavg=0.;ieq=wp.sttEles[wp.memWGa*i+j]/10;isideq=wp.sttEles[wp.memWGa*i+j]-10*ieq;
//   for(k2=0;k2<ngaus;k2++){
//	 for(k1=0;k1<ngaus;k1++){
//					if(isideq==0)     {xc=POSGP8[k1];yc= -1.;zc=POSGP8[k2];}
//					else if(isideq==1){xc=  1.;yc=POSGP8[k1];zc=POSGP8[k2];}
//					else if(isideq==2){xc=POSGP8[k1];yc=  1.;zc=POSGP8[k2];}
//					else if(isideq==3){xc= -1.;yc=POSGP8[k1];zc=POSGP8[k2];}
//					else if(isideq==4){xc=POSGP8[k1];yc=POSGP8[k2];zc= -1.;}
//					else              {xc=POSGP8[k1];yc=POSGP8[k2];zc=  1.;}
//					STFISO8(1,xc,yc,zc,&DJD,HN,SN,SG,DJR,base.nop1+MXNPEL*ieq,base.c1);
//					STFISO8(3,xc,yc,zc,&DJD,HN,SN1,SG,DJR,base.nop1+MXNPEL*ieq,base.c1);
//					area=area+DJD*WEIGP8[k1]*WEIGP8[k2];
//					for(k=0;k<8;k++){
//xfavg=xfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+0]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
//yfavg=yfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+1]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
//zfavg=zfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+2]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
////honk<<j<<" "<<k2<<" "<<k1<<" "<<k<<" "<<SN[k]<<" rapido "<<xfavg<<" "<<yfavg<<" "<<zfavg<<"\n";
//									}
//							}
//						  }
//   xpatchA=xpatchA+xfavg;ypatchA=ypatchA+yfavg;zpatchA=zpatchA+zfavg;
//  }
//xpatchA=xpatchA/area;ypatchA=ypatchA/area;zpatchA=zpatchA/area;
//honk<<xpatch<<" "<<ypatch<<" "<<zpatch<<" aaaxxx\n";
//honk<<xpatchA<<" "<<ypatchA<<" "<<zpatchA<<" "<<area<<" XAYAZA area\n";
//if(1==1)exit(0);
//////////////
		 for(j=0;j<tranche;j++)
		   {viewfile<<ttime<<", "<<wp.n_curr_sttEl[i]<<"\n";
////////////// trash
////honk<<ttime*wp.speed[i]<<"   "<<ttimeA*wp.speed[i]<<"   "<<ttimeE*wp.speed[i]<<"\n";
//honk<<ttime<<"   "<<wp.n_curr_sttEl[i]<<" Tranche "<<(j+1)<<"\n";
//////////////
			for(k=0;k<wp.n_curr_sttEl[i];k++)viewfile<<(base.el_map[wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+k]/10]+1)<<", ";
			viewfile<<"\n";
/////////////////// trash
//			ie=wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+cen_stt]/10;
//			iside=wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+cen_stt]-10*ie;
//		 if     (iside==0 || iside==2){n1=0;n2=1;n3=5;n4=4;n5=2;n6=3;n7=7;n8=6;}
//		 else if(iside==1 || iside==3){n1=1;n2=2;n3=6;n4=5;n5=3;n6=0;n7=4;n8=7;}
//		 else                         {n1=0;n2=1;n3=2;n4=3;n5=4;n6=5;n7=6;n8=7;}
//		 xlo=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n1]+0]+base.c1[NDF*base.nop1[MXNPEL*ie+n2]+0]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n3]+0]+base.c1[NDF*base.nop1[MXNPEL*ie+n4]+0]);
//		 ylo=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n1]+1]+base.c1[NDF*base.nop1[MXNPEL*ie+n2]+1]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n3]+1]+base.c1[NDF*base.nop1[MXNPEL*ie+n4]+1]);
//		 zlo=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n1]+2]+base.c1[NDF*base.nop1[MXNPEL*ie+n2]+2]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n3]+2]+base.c1[NDF*base.nop1[MXNPEL*ie+n4]+2]);
//		 xup=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n5]+0]+base.c1[NDF*base.nop1[MXNPEL*ie+n6]+0]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n7]+0]+base.c1[NDF*base.nop1[MXNPEL*ie+n8]+0]);
//		 yup=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n5]+1]+base.c1[NDF*base.nop1[MXNPEL*ie+n6]+1]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n7]+1]+base.c1[NDF*base.nop1[MXNPEL*ie+n8]+1]);
//		 zup=0.25*(base.c1[NDF*base.nop1[MXNPEL*ie+n5]+2]+base.c1[NDF*base.nop1[MXNPEL*ie+n6]+2]+
//				   base.c1[NDF*base.nop1[MXNPEL*ie+n7]+2]+base.c1[NDF*base.nop1[MXNPEL*ie+n8]+2]);
//		 distE=sqrt((xup-xlo)*(xup-xlo)+(yup-ylo)*(yup-ylo)+(zup-zlo)*(zup-zlo));
///////////////////
			xup=yup=zup=0.f;
			for(ja=0;ja<wp.n_curr_sttEl[i];ja++)
			  {xfavg=yfavg=zfavg=0.f;
			   ieq=wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+ja]/10;
			   isideq=opp_arr8[wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+ja]-10*ieq];
			   for(k=0;k<4;k++){xfavg=xfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+0];
								yfavg=yfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+1];
								zfavg=zfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+gdata8[4*isideq+k]]+2];
							   }
			   xup=xup+xfavg/4.f;yup=yup+yfavg/4.f;zup=zup+zfavg/4.f;
			  }
			xup=xup/float(wp.n_curr_sttEl[i]);yup=yup/float(wp.n_curr_sttEl[i]);zup=zup/float(wp.n_curr_sttEl[i]);

			dist=sqrt((xup-xpatch)*(xup-xpatch)+(yup-ypatch)*(yup-ypatch)+(zup-zpatch)*(zup-zpatch));
//honk<<xup<<" "<<yup<<" "<<zup<<" UPpointDOWN "<<xpatch<<" "<<ypatch<<" "<<zpatch<<"\n";
			xpatch=xup;ypatch=yup;zpatch=zup;
/////////////// trash
//xupA=yupA=zupA=area=0.;
//for(ja=0;ja<wp.n_curr_sttEl[i];ja++)
//  {xfavg=yfavg=zfavg=0.;ieq=wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+ja]/10;isideq=opp_arr8[wp.eles[wp.memWGa*i+wp.n_curr_sttEl[i]*j+ja]-10*ieq];
//   for(k2=0;k2<ngaus;k2++){
//	 for(k1=0;k1<ngaus;k1++){
//					if(isideq==0)     {xc=POSGP8[k1];yc= -1.;zc=POSGP8[k2];}
//					else if(isideq==1){xc=  1.;yc=POSGP8[k1];zc=POSGP8[k2];}
//					else if(isideq==2){xc=POSGP8[k1];yc=  1.;zc=POSGP8[k2];}
//					else if(isideq==3){xc= -1.;yc=POSGP8[k1];zc=POSGP8[k2];}
//					else if(isideq==4){xc=POSGP8[k1];yc=POSGP8[k2];zc= -1.;}
//					else              {xc=POSGP8[k1];yc=POSGP8[k2];zc=  1.;}
//					STFISO8(1,xc,yc,zc,&DJD,HN,SN,SG,DJR,base.nop1+MXNPEL*ieq,base.c1);
//					STFISO8(3,xc,yc,zc,&DJD,HN,SN1,SG,DJR,base.nop1+MXNPEL*ieq,base.c1);
//					area=area+DJD*WEIGP8[k1]*WEIGP8[k2];
//					for(k=0;k<8;k++){xfavg=xfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+0]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
//									 yfavg=yfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+1]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
//									 zfavg=zfavg+base.c1[NDF*base.nop1[MXNPEL*ieq+k]+2]*SN[k]*DJD*WEIGP8[k1]*WEIGP8[k2];
//				                    }
//							}
//						  }
//   xupA=xupA+xfavg;yupA=yupA+yfavg;zupA=zupA+zfavg;
//  }
//xupA=xupA/area;yupA=yupA/area;zupA=zupA/area;
//distA=sqrt((xupA-xpatchA)*(xupA-xpatchA)+(yupA-ypatchA)*(yupA-ypatchA)+(zupA-zpatchA)*(zupA-zpatchA));
////honk<<xpatchA<<" "<<ypatchA<<" "<<zpatchA<<" "<<area<<" XAYAZA areaoo "<<distA<<"\n";
//xpatchA=xupA;ypatchA=yupA;zpatchA=zupA;
//ttimeA=ttimeA+distA/wp.speed[i];
//ttimeE=ttimeE+distE/wp.speed[i];
///////////////
			ttime=ttime+dist/wp.speed[i];
			wpTime=wpTime+dist/wp.speed[i];
//honk<<ttime<<" "<<wpTime<<" Updated local/global time using dist & speed "<<dist<<" "<<wp.speed[i]<<"\n";
		   }
//		 wp.wpTimes[i]=wpTime+wp.timeInterval[i];
		 wp.wpTimes[i]=wpTime; // Correction EFP 8-25-2010
		 time=time+wpTime+wp.timeInterval[i];
		}
	  *timesave2=time;
//	 }
//__finally {
Screen->Cursor=Save_Cursor;
//}
 viewfile.close();


}
//---------------------------------------------------------------------------
//void TForm1::WARP3DepBlock(String gWsiAlias) //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
void TForm1::exportWARP4_public()
//Routine too write *.wrp, compute_commands_all_profiles.inp, uexternal_data_file.inp, output_commands.inp
//limlist= number of "a-b" pairs in WARP3D list output format, before writing next line
{int solidshellsw=0,limlist=5,i=0,isw=0,mtype=0,icount=0,buffersize=0,nlist=Form7->CheckNlist, *rollcall=NULL;
 long ic=0,hinode=0,hielem=0,lolim=0,uplim=0,mdummy=0,ir=0,j=0,js=0,k=0,ies=0,iesr=0,icycle=0,irec=0,itype=0,ilast=0,istart=0,iELSETtype=0,t3=1000,ibrsw=0, *iELSETorder=NULL;
 float tcuth=0.f,tdummy=0.f;
 String umat=L"_umat", *sArr=NULL;
 UnicodeString fnNeedS1,extensCharS1=UnicodeString(L".wrp");
// char longo1[2],longo2[3],longo3[4];
//

//String s11=L"W:\\Thequick\\fox brown\\foxjumps_iso_file.dat",s22=L".",s33=L"\\";
//wchar_t *Dest=NULL;
//typedef System::Set<System_Sysutils__75, System_Sysutils__75::rfReplaceAll, System_Sysutils__75::rfIgnoreCase> TReplaceFlags;
////typedef System::Set<System_Sysutils__75, System_Sysutils__75, System_Sysutils__75::rfIgnoreCase> TReplaceFlags;
////typedef System::Set<System_Sysutils__75, System_Sysutils__75::rfIgnoreCase> TReplaceFlags;
//TReplaceFlags Flags;// Note: Flags would be set by WHAT???
//if(ContainsStr(s11,s33))honk<<" YES\n";
//else honk<<" NO\n";//if(1==1)exit(0);
////i=umat->LastIndexOf(L"a");honk<<i<<"\n";if(1==1)exit(0);
////i=s11->LastDelimiter(&L"x");
//i=LastDelimiter(s33,s11); // Begins at 1; 0 if not found
//icount=LastDelimiter(s22,s11); // Begins at 1; 0 if not found
//honk<<i<<" LD "<<icount<<"\n";
////wchar_t *s44=NULL;
////TCharArray *s44=NULL;
////TCharArray s44;
////s44.SetLength[icount-i+2];
////wchar_t *s55=L"abcdefghijklmnopqrstuvwxyz";
////		const _DCHAR* p = s55;
////		std::auto_ptr<TStringBuilder> builder(new TStringBuilder());
////		assert(builder->ToString() == System::String());
////		System::String str1(p);
////		while (*p)
////		{builder->Append(*p++);
////		}
//////s44=new TCharArray[icount-i+2];
//////s55->CopyTo(i,s44,1,icount-i+1);
////builder->CopyTo(i,s44,1,icount-i+1);
//// int bufferSize=WideCharToMultiByte(CP_UTF8,0,s44.w_str(), -1,NULL,0,NULL,NULL);
//// char* m=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,s44.w_str(), -1,m,bufferSize,NULL,NULL);
////honk<<m<<" mmmmm\n";
////honk<<s44[1]<<"\n";
////delete [] s44;
////TStringDynArray DynStrings=SplitString(s11,L".\\"); //How to delete DynStrings after this?
////buffersize=WideCharToMultiByte(CP_UTF8,0,DynStrings[0].w_str(), -1,NULL,0,NULL,NULL);
////char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,DynStrings[0].w_str(), -1,m1,buffersize,NULL,NULL);
////honk<<m1<<"\n";// EFP 12/10/2014
////delete[] m1;
////if(wms.matFileName[0].IndexOf(L"\\")> -1);
////s11->Replace(L"\\",L"Q",s11->IndexOf(L"\\"),1);
////TStringDynArray DynStrings=SplitString(s11,L"."); //How to delete DynStrings after this?
////buffersize=WideCharToMultiByte(CP_UTF8,0,DynStrings[0].w_str(), -1,NULL,0,NULL,NULL);
////char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,DynStrings[0].w_str(), -1,m1,buffersize,NULL,NULL);
////outfile<<m1<<"\n";// EFP 12/10/2014
////delete[] m1;
//Dest=new wchar_t[wms.matFileName[0].Length()+1];
//StringCchCopyW(Dest,wms.matFileName[0].Length()+1,wms.matFileName[0].w_str());
////Dest->Replace(L"C",L"Q",1,3);
//WideReplaceStr(Dest,L"C",L"Q");
// buffersize=WideCharToMultiByte(CP_UTF8,0,Dest, -1,NULL,0,NULL,NULL);
// char* m=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,Dest, -1,m,buffersize,NULL,NULL);
//honk<<m<<"\n";// EFP 12/10/2014
//delete[] m;
////s55=new String[wms.matFileName[0].Length()+1];
//String s55=StringReplace(wms.matFileName[0],s33,s22,Flags);
////if(ContainsStr(wms.matFileName[0],s33))honk<<" wmsYES\n";
//if(ContainsStr(s55,s33))honk<<" wmsYES\n";
//else honk<<" NO\n";//if(1==1)exit(0);
//// buffersize=WideCharToMultiByte(CP_UTF8,0,wms.matFileName[0].w_str(), -1,NULL,0,NULL,NULL);
//// char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,wms.matFileName[0].w_str(), -1,m1,buffersize,NULL,NULL);
// buffersize=WideCharToMultiByte(CP_UTF8,0,s55.w_str(), -1,NULL,0,NULL,NULL);
// char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,s55.w_str(), -1,m1,buffersize,NULL,NULL);
//honk<<m1<<"\n";// EFP 12/10/2014
//delete[] m1;
//if(1==1)exit(0);

				  ifstream viewfile4("param.in",ios::nocreate,0); //Global param.in, in case of multi-core
				  if(viewfile4){ //Read 11-parameter param.in
viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;
viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>hinode;viewfile4>>tdummy;
viewfile4>>mdummy;viewfile4>>tdummy;viewfile4>>solidshellsw;viewfile4>>tdummy;viewfile4>>tcuth;
viewfile4>>hielem;viewfile4.close();
								if(!solidshellsw){
 fnNeedS1=gWsiAlias+extensCharS1;
 ofstream outfile(fnNeedS1.w_str());
//xxxxxxxxxx
 buffersize=WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,m,buffersize,NULL,NULL);
//xxxxxxxxxx
outfile<<"c\nc\nc *.wrp file\n";
outfile<<"structure "<<m<<"\n";
outfile<<"c\nc\nc\n";
outfile<<"c Material VFT UMAT commands\n";

 sArr=new String[nlist];iELSETorder=new long[nlist+wp.nWeldPass];
 for(i=0;i<nlist;i++){Form7->CheckLB2ItemIndex=i;sArr[i]=Form7->CheckLB2Item;}

////outfile<<"material steel_1e650_umat\n";  //Make name=defined material name & note that it is umat
//outfile<<"material MATERIAL_umat\n";  //Make name=defined material name & note that it is umat
//outfile<<"  properties umat  rho 0.0  alpha 0.0,\n";
//outfile<<"       um_1 1 um_2 7.5E+02 um_3 1.5E+03,\n";
//outfile<<"       um_4 1.5E+03 um_5 -1.0,\n";
//outfile<<"       um_6 -1.0  um_7 0 um_8 0\n";
for(ir=0;ir<wms.nMatPropSet;ir++){outfile<<"material ";
TStringDynArray DynStrings=SplitString(wms.matFileName[ir],L"."); //How to delete DynStrings after this?
buffersize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,buffersize,NULL,NULL);
outfile<<m1<<"\n";// EFP 12/10/2014
if     (ContainsStr(DynStrings[0],L"_iso_"))mtype=0; //Isotropic
else if(ContainsStr(DynStrings[0],L"_kin_"))mtype=1; //Linear kinematic (not multi-linear)
else if(ContainsStr(DynStrings[0],L"_mln_"))mtype=2; //Multi-inear kinematic
else if(ContainsStr(DynStrings[0],L"_mix_"))mtype=3; //Post-weld mixed
// No mtype=4 value
else if(ContainsStr(DynStrings[0],L"_phs_"))mtype=5; //Simple phase-transformation (not full)
else if(ContainsStr(DynStrings[0],L"_fph_"))mtype=6; //Full phase-transformation
else {mtype=0;
extern PACKAGE void __fastcall Beep(void);Application->MessageBox((L"material# "+IntToStr(__int64(ir+1))).w_str(),L"Warning: user must edit *.wrp um_7 for material type",MB_OK);
	 }
outfile<<"  properties umat  rho "<<wms.den[ir]<<"  alpha 0.0,\n";
outfile<<"       um_1 "<<(ir+1)<<" um_2 "<<wms.Ti[ir]<<" um_3 "<<wms.Ta[ir]<<",\n";
outfile<<"       um_4 "<<wms.Tm[ir]<<" um_5 -1.0,\n";
outfile<<"       um_6 -1.0 um_7 "<<mtype<<" um_8 0\n";  //um_7=0 for isotropic
 delete[] m1;
								 }
//
outfile<<"c     ***************************************\n";
outfile<<"c     *          end of materials           *\n";
outfile<<"c     ***************************************\n";
outfile<<"c\nc\n";
outfile<<"number of nodes "<<hinode<<" elements "<<hielem<<"\n";
outfile<<"c\n";
outfile<<"*input from \'"<<m<<".coordinates\'\n"; //Properly name (e.g.) 'Tee2.coordinates'
outfile<<"c\nc\n";
outfile<<"elements\n";
outfile<<"c   for config number   0\n";
////outfile<<"            1 -   "<<hielem<<" type l3disop    linear material steel_1e650_umat,\n"; //Proper name
//outfile<<"            1 -   "<<hielem<<" type l3disop    linear material MATERIAL_umat,\n"; //Proper name
//outfile<<"					   order 2x2x2 bbar center_output short\n";

	   istart=j=js=0;iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);if(base.matno[istart]<0)iELSETtype= -iELSETtype;
	   do {ibrsw=0;
		   for(ir=istart+1;ir<base.nelt;ir++)
			 {k= labs(base.matno[ir])-t3*(labs(base.matno[ir])/t3);if(base.matno[ir]<0)k= -k;
			  if(iELSETtype != k){isw=1;for(ies=0;ies<j;ies++)if(iELSETorder[ies]/t3==iELSETtype){isw=0;iesr=ies;}
								  if(isw){if(iELSETtype> -1)iELSETorder[j]=iELSETtype*t3+js;
										  else              iELSETorder[j]=iELSETtype*t3-js;
										  j++;
										 }
								  else {if(iELSETorder[iesr]> -1)iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) +js;
										else                     iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) -js;
									   }
								  js++;ibrsw=1;istart=ir;iELSETtype=k;break;
								 }
			 }
		  }
	   while(ibrsw);
	   if(istart<base.nelt){isw=1;for(ies=0;ies<j;ies++)if(iELSETorder[ies]/t3==iELSETtype){isw=0;iesr=ies;}
							if(isw){if(iELSETtype> -1)iELSETorder[j]=iELSETtype*t3+js;
									else              iELSETorder[j]=iELSETtype*t3-js;
									j++;
								   }
							else {if(iELSETorder[iesr]> -1)iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) +js;
								  else                     iELSETorder[iesr]=t3*(iELSETorder[iesr]/t3) -js;
								 }
						   }
 for(icycle=0;icycle< nlist+wp.nWeldPass;icycle++)
   {itype=iELSETorder[icycle]/t3;ilast= labs(iELSETorder[icycle])-t3*(labs(iELSETorder[icycle])/t3);
	istart=j=js=0;iELSETtype= labs(base.matno[istart])-t3*(labs(base.matno[istart])/t3);if(base.matno[istart]<0)iELSETtype= -iELSETtype;
	do {ibrsw=0;
		for(ir=istart+1;ir<base.nelt;ir++)
		  {k= labs(base.matno[ir])-t3*(labs(base.matno[ir])/t3);if(base.matno[ir]<0)k= -k;
		   if(iELSETtype != k){if(iELSETtype==itype){if(j==0){icount=0;
//																	   if(itype>=0 && itype <10){ltoa(itype,longo1,10);outfile<<"list \"nonWeldPassEntity"<<longo1<<"\" ";}
//																	   else if(itype>=10 && itype <100){ltoa(itype,longo2,10);outfile<<"list \"nonWeldPassEntity"<<longo2<<"\" ";}
//																	   else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);outfile<<"list \"nonWeldPassEntity"<<longo3<<"\" ";}
if(itype>=0 && itype <1000){
buffersize=WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,m1,buffersize,NULL,NULL);
outfile<<"list \""<<m1<<"\" ";
delete [] m1;
						   }
//																	   else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);outfile<<"list \"WeldPassEntity"<<longo1<<"\" ";}
//																	   else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);outfile<<"list \"WeldPassEntity"<<longo2<<"\" ";}
//																	   else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);outfile<<"list \"WeldPassEntity"<<longo3<<"\" ";}
else if(itype>= -999 && itype <0){
buffersize=WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,m1,buffersize,NULL,NULL);
outfile<<"list \""<<m1<<"\" ";
delete [] m1;
								 }
																	   else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
																			}
															 }
													 j++;
													 if(ilast==js)outfile<<(istart+1)<<"-"<<ir<<"\n";
													 else {icount++;if(icount<limlist)outfile<<(istart+1)<<"-"<<ir<<",";
																	else {icount=0;outfile<<(istart+1)<<"-"<<ir<<",\n      ";}
														  }
													}
							   js++;ibrsw=1;istart=ir;iELSETtype=k;break;
							  }
		  }
	   }
	while(ibrsw);
	if(istart<base.nelt){
							   if(iELSETtype==itype){if(j==0){icount=0;
//																	   if(itype>=0 && itype <10){ltoa(itype,longo1,10);outfile<<"list \"nonWeldPassEntity"<<longo1<<"\" ";}
//																	   else if(itype>=10 && itype <100){ltoa(itype,longo2,10);outfile<<"list \"nonWeldPassEntity"<<longo2<<"\" ";}
//																	   else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);outfile<<"list \"nonWeldPassEntity"<<longo3<<"\" ";}
if(itype>=0 && itype <1000){
buffersize=WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,m1,buffersize,NULL,NULL);
outfile<<"list \""<<m1<<"\" ";
delete [] m1;
						   }
//																	   else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);outfile<<"list \"weldpass"<<longo1<<"\" ";}
//																	   else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);outfile<<"list \"weldpass"<<longo2<<"\" ";}
//																	   else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);outfile<<"list \"weldpass"<<longo3<<"\" ";}
else if(itype>= -999 && itype <0){
buffersize=WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,m1,buffersize,NULL,NULL);
outfile<<"list \""<<m1<<"\" ";
delete [] m1;
								 }
																	   else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
																			}
															 }
													 if(ilast==js)outfile<<(istart+1)<<"-"<<ir<<"\n";
													 else {icount++;if(icount<limlist)outfile<<(istart+1)<<"-"<<ir<<",";
																	else {icount=0;outfile<<(istart+1)<<"-"<<ir<<",\n      ";}
														  }
													}
						}
   }
 for(icycle=0;icycle< nlist+wp.nWeldPass;icycle++)
   {itype=iELSETorder[icycle]/t3;
//	if(itype>=0 && itype <10){ltoa(itype,longo1,10);
//							  TStringDynArray DynStrings=SplitString(sArr[itype-1],L"."); //How to delete DynStrings after this?
//							  bufferSize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//							  char* m1=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize,NULL,NULL);
//							  outfile<<"\"nonWeldPassEntity"<<longo1<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//							  delete [] m1;
//							 }
//	else if(itype>=10 && itype <100){ltoa(itype,longo2,10);
//									 TStringDynArray DynStrings=SplitString(sArr[itype-1],L"."); //How to delete DynStrings after this?
//									 bufferSize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//									 char* m1=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize,NULL,NULL);
//									 outfile<<"\"nonWeldPassEntity"<<longo2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//									 delete [] m1;
//									}
//	else if(itype>=100 && itype <1000){ltoa(itype,longo3,10);
//									   TStringDynArray DynStrings=SplitString(sArr[itype-1],L"."); //How to delete DynStrings after this?
//									   bufferSize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//									   char* m1=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize,NULL,NULL);
//									   outfile<<"\"nonWeldPassEntity"<<longo3<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//									   delete [] m1;
//									  }
	if(itype>=0 && itype <1000){
TStringDynArray DynStrings=SplitString(sArr[itype-1],L"."); //How to delete DynStrings after this?
buffersize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,buffersize,NULL,NULL);
buffersize=WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,NULL,0,NULL,NULL);
char* m2=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,base.ELSETinputnames[itype].w_str(), -1,m2,buffersize,NULL,NULL);
outfile<<"\""<<m2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
delete [] m2;delete [] m1;
							   }
//	else if(itype>= -9 && itype <0){ltoa(-itype,longo1,10);for(ic=0;ic<wms.nMatPropSet;ic++)if(wms.name[ic]==wp.matName[-itype-1]){irec=ic;break;}
//									TStringDynArray DynStrings=SplitString(wms.matFileName[irec],L"."); //How to delete DynStrings after this?
//									bufferSize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//									char* m1=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize,NULL,NULL);
//									outfile<<"\"weldpass"<<longo1<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//									delete [] m1;
//								   }
//	else if(itype>= -99 && itype < -9){ltoa(-itype,longo2,10);for(ic=0;ic<wms.nMatPropSet;ic++)if(wms.name[ic]==wp.matName[-itype-1]){irec=ic;break;}
//									   TStringDynArray DynStrings=SplitString(wms.matFileName[irec],L"."); //How to delete DynStrings after this?
//									   int bufferSize7=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//									   char* m1=new char[bufferSize7];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize7,NULL,NULL);
// 									   outfile<<"\"weldpass"<<longo2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//									   delete [] m1;
//									  }
//	else if(itype>= -999 && itype < -99){ltoa(-itype,longo3,10);for(ic=0;ic<wms.nMatPropSet;ic++)if(wms.name[ic]==wp.matName[-itype-1]){irec=ic;break;}
//										 TStringDynArray DynStrings=SplitString(wms.matFileName[irec],L"."); //How to delete DynStrings after this?
//										 bufferSize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
//										 char* m1=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,bufferSize,NULL,NULL);
//										 outfile<<"\"weldpass"<<longo3<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
//										 delete [] m1;
//										}
	else if(itype>= -999 && itype <0){
//ltoa(-itype,longo3,10);
for(ic=0;ic<wms.nMatPropSet;ic++)if(wms.name[ic]==wp.matName[-itype-1]){irec=ic;break;}
TStringDynArray DynStrings=SplitString(wms.matFileName[irec],L"."); //How to delete DynStrings after this?
buffersize=WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,(DynStrings[0]+umat).w_str(), -1,m1,buffersize,NULL,NULL);
buffersize=WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,NULL,0,NULL,NULL);
char* m2=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,wp.name[-itype-1].w_str(), -1,m2,buffersize,NULL,NULL);
outfile<<"\""<<m2<<"\" type l3disop material "<<m1<<" order 2x2x2 center output short\n";
delete [] m2;delete [] m1;
									 }
	else {honk<<itype<<" Terminate: Too many nonWeld/Weld entities in exportCTSP4_public()\n";
		  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Too many nonWeld/Weld entities in exportCTSP4_public()",L"Terminate",MB_OK);exit(0);
		 }
   }
// delete [] iELSETorder;delete [] sArr;

outfile<<"c\n";
outfile<<"*input from \'"<<m<<".incid\'\n";
outfile<<"c default blocking is 128 elem/blk\n";
outfile<<"blocking automatic\n";
outfile<<"c\nc\n";
outfile<<"*input from \'"<<m<<".constraints\'\n";
outfile<<"c\n";
outfile<<"c  Define initial temperatures\n";
outfile<<"initial conditions\n";
outfile<<"   temperature\n";
outfile<<"	 nodes 1-"<<hinode<<" temperature "<<tcuth<<"\n";
outfile<<"c\n";
outfile<<"output patran neutral \'"<<m<<".neut\'\n";
outfile<<"c\n";
outfile<<"c  From the template here are the load definitions.\n";
outfile<<"c  Total weld times are also included for convenience, not necessity\n";
outfile<<"c\n";
				  outfile<<"*echo off\n";
				  outfile<<"loading weld_temps\n";
				  outfile<<" nodal loads\n";
				  outfile<<"  user_routine\n";
outfile<<"c\nc The loading steps\nc\n";
outfile<<"c This name must be used in the following: weld_sim\n";
				  outfile<<"loading weld_sim\n";
				  outfile<<" nonlinear\n";
				  lolim=1;uplim=10000;
				  outfile<<"  step "<<lolim<<"-"<<uplim<<" weld_temps 1.0\n";
outfile<<"c Always define 5000 or more steps\n";
outfile<<"c Actual number of steps solved is determined dynamically by the\n";
outfile<<"c user_solution routine in response to features of thermal profiles.\n";
				  outfile<<"c\nc\nc Solution parameters.\nc\n";
outfile<<" nonlinear analysis parameters\n";
outfile<<" user_routine on\n";
outfile<<" umat serial off\n";
outfile<<" solution technique direct sparse\n";
outfile<<" convergence test maximum residual tolerance 0.5\n";
outfile<<" nonconvergent solutions stop\n";
outfile<<" divergence check on\n";
outfile<<" batch messages on\n";
outfile<<" cpu time limit off\n";
outfile<<" material messages off\n";
outfile<<" bbar stabilization factor 0.05\n";
outfile<<"c The following is only used for large displ analysis\n";
outfile<<" consistent q-matrix off\n";
outfile<<" trace solution on\n";
outfile<<"c The following values ignored because controlled by user_routine\n";
outfile<<"   time step 0.09436531\n";
outfile<<"   maximum iterations 7\n";
outfile<<"   minimum iterations 1\n";
outfile<<"   adaptive solution on\n";
outfile<<"   linear stiffness for iteration one off\n";
outfile<<"   extrapolate off\n";
				  outfile<<"*input 'compute_commands_all_profiles.inp'\n";
				  outfile<<"stop\n";
				  outfile.close();
//xxxxxxxxxx
 delete[] m;
//xxxxxxxxxx
/////////////////////////////				  delete [] timeSeries;
 ofstream outfile1("compute_commands_all_profiles.inp");
// for(ipp=0;ipp<wp.stepInterval[k];ipp++)
 for(ic=0;ic<10000;ic++) // Allow for max 10000 "on the fly" steps per BobD's request  EFP 10/09/2014
   {outfile1<<" compute displacements loading weld_sim step "<<(ic+1)<<"\n"; //Name weld_sim ESSENTIAL
	outfile1<<"   *input from 'vft_solution_cmds.inp'\n"; //vft_solution_cmds.inp is created by WARP3D on the fly, not by us
   }
 outfile1<<"stop\n";
 outfile1.close();

//////////////////////////////////
 ofstream outfile4("uexternal_data_file.inp");
 outfile4<<"!  Three non-comment lines with file names required\n";
 outfile4<<"!   1 - name of material.dat file for VFT\n";
 outfile4<<"!   2 - name of VED.dat file\n";
 outfile4<<"!   3 - root of file names for thermal profiles\n";
 outfile4<<"!       There must be file names with extensions\n";
 outfile4<<"!       *.txt & *.bin\n";
 outfile4<<"!       Omit extensions here.\n";
 outfile4<<"!\n";
 outfile4<<"!  File names may have ~/ to denote user home directory.\n";
 outfile4<<"!  WARP3D will resolve to full path name.\n";
 outfile4<<"!\n";
// outfile4<<"./material.dat\n"; //original WARP3D uexternal_data_file.inp for a single material
 rollcall=new int[wms.nMatPropSet];for(ir=0;ir<wms.nMatPropSet;ir++)rollcall[ir]=0;
 for(ic=0;ic<nlist;ic++)
   {for(ir=0;ir<wms.nMatPropSet;ir++)if(wms.matFileName[ir]==sArr[ic]){rollcall[ir]=1;break;}
   }
 for(ic=0;ic<wp.nWeldPass;ic++)
   {for(ir=0;ir<wms.nMatPropSet;ir++)if(wms.name[ir]==wp.matName[ic]){rollcall[ir]=1;break;}
   }
 irec=0;for(ir=0;ir<wms.nMatPropSet;ir++)if(rollcall[ir])irec++;
 outfile4<<irec<<"\n";
 for(ir=0;ir<wms.nMatPropSet;ir++)if(rollcall[ir]){
buffersize=WideCharToMultiByte(CP_UTF8,0,wms.matFileName[ir].w_str(), -1,NULL,0,NULL,NULL);
char* m1=new char[buffersize];WideCharToMultiByte(CP_UTF8,0,wms.matFileName[ir].w_str(), -1,m1,buffersize,NULL,NULL);
outfile4<<"./"<<m1<<"\n";// EFP 12/10/2014
delete[] m1;
												  }
 delete [] rollcall;
 outfile4<<"./VED.dat\n";
 outfile4<<"./warp_temp_2_files\n";
 outfile4<<"!\n";
 outfile4<<"!  Stop when analysis for this thermal profile completed.\n";
 outfile4<<"!  If this number of profiles is not defined, WARP3D will\n";
 outfile4<<"!  write output files, a restart file, and execute normal termination.\n";
 outfile4<<"!\n";
// outfile4<<"  10\n";
 outfile4<<"  10000\n";
 outfile4<<"!\n";
 outfile4<<"!  Values to control output:\n";
 outfile4<<"!  - number of thermal profiles between saving restart file\n";
 outfile4<<"!  - number of thermal profiles between generation of output file\n";
 outfile4<<"!  - file of WARP3D output commands to be executed\n";
 outfile4<<"!\n";
// outfile4<<"  500, 500\n";
 outfile4<<"  10000, 10\n";
 outfile4<<"output_commands.inp\n";
 outfile4<<"!\n";
 outfile4<<"!  Values to control solution when:\n";
 outfile4<<"!  o - a torch or torches comes on\n";
 outfile4<<"!  o - cooling starts\n";
 outfile4<<"!  o - analysis startup and on restarts\n";
 outfile4<<"!\n";
 outfile4<<"! - (N1) number of sequential thermal profiles over which\n";
 outfile4<<"!        to use a larger number of WARP3D load steps\n";
 outfile4<<"! - (N2) number of increased load steps to use (>=1) for\n";
 outfile4<<"!        solution over these profiles\n";
 outfile4<<"!\n";
// outfile4<<"  2, 5\n";
 outfile4<<"  1, 2\n";
 outfile4<<"!  Value to control solution when:\n";
 outfile4<<"!  o - heating is occurring and has continued beyond N1 above\n";
 outfile4<<"!  o - cooling is occurring and has continued beyond N1 above\n";
 outfile4<<"! - (N1) number of sequential thermal profiles over which\n";
 outfile4<<"!\n";
 outfile4<<"!  N3 =1 is the most common value.\n";
 outfile4<<"!\n";
// outfile4<<"  1\n";
 outfile4<<"  2\n";
 outfile4.close();
 delete [] iELSETorder;delete [] sArr;
// ofstream outfile5("output_commands.inp");
//// outfile5<<"!  Put WARP3D output commands here. These will be executed after\n";
//// outfile5<<"!  solution for a profile completes at the profile frequency \n";
//// outfile5<<"!  specified in\n";
//// outfile5<<"!  uexternal_data_file.inp\n";
//// outfile5<<"!\n";
//// outfile5<<"   output displacements nodes 10000-10300\n";
//// outfile5<<"   output displacements elements 6000-6050 by 2\n";
//// outfile5<<"   output totals only reactions nodes all\n";
//// outfile5<<"   output wide eformat noheader strains 91240-91250,\n";
//// outfile5<<"       156320-157000 by 3\n";
//// outfile5<<"   output wide stresses 156320-157000 by 3\n";
//// outfile5<<"   output patran binary displacements stresses strains\n";
//// outfile5<<"   output patran binary element stresses strains\n";
// outfile5<<"!  DEFAULT FOR WSO, IF YOU NEED STRESS/STRAIN, COMMENT Line 3,\n";
// outfile5<<"!  UNCOMMENT OTHER LINES AS APPROPRIATE\n";
// outfile5<<"!\n";
// outfile5<<"!    Use patran files for old-version pat2exii until Python\n";
// outfile5<<"!    version becomes faster. Then switch to output flat ..\n";
// outfile5<<"!    commands.\n";
// outfile5<<"!\n";
// outfile5<<"!  output patran formatted displacements\n";
// outfile5<<"!  output patran formatted stresses\n";
// outfile5<<"!  output patran formatted temperatures\n";
// outfile5<<"!\n";
// outfile5<<"!  output patran binary element strains stresses\n";
// outfile5<<"!  output totals only reactions nodes all\n";
// outfile5<<"!\n";
// outfile5<<"  output flat text nodal stresses\n";  //BB 1/15/2015
// outfile5<<"  output flat text nodal displacements\n";
// outfile5<<"  output flat text nodal temperatures\n";
// outfile5.close();
/////////////////////////////////////////////////////////////
//				  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"*.wrp & compute_commands_all_profiles.inp written",L"Success",MB_OK);
//xxxxxxxxxx
//// delete[] m;
//xxxxxxxxxx
//	   ir=11; //MUST COUNT ENTITIES BELOW
	   Form9=new TForm9(this);
	   Form9->Caption=L"WARP3D output options";
	   Form9->Button1->Caption=L"OK";
	   i=  0;Form9->CheckListBox1->AddItem(L"Stresses",this);
	   Form9->CheckListBox1->Checked[i]=true;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Displacements",this);
	   Form9->CheckListBox1->Checked[i]=true;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Temperatures",this);
	   Form9->CheckListBox1->Checked[i]=true;
//
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe11",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe22",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe33",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe12",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe13",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Pl.strain pe23",this);
	   Form9->CheckListBox1->Checked[i]=false;
//
	   i=i+1;Form9->CheckListBox1->AddItem(L"Temp at end incr.",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   i=i+1;Form9->CheckListBox1->AddItem(L"Max temp.occurring",this);
	   Form9->CheckListBox1->Checked[i]=false;
	   Form9->CheckListBox1->ItemIndex=0;
	   Form9->ShowModal();
	   delete Form9;// *Form9=NULL; (not in Unit1, remember, but perhaps we should not "delete"?)


												 }
								else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"WARP3D is for solid models only",L"Failure: Shell model detected",MB_OK);}
							   }
				  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Missing param.in file",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::exportWARP3D5_public()
//Routine to write "output_commands.inp"  EFP 1/16/2015
{int icount=0;
 ofstream outfile5("output_commands.inp");
 outfile5<<"!  DEFAULT FOR WSO, IF YOU NEED STRESS/STRAIN, COMMENT Line 3,\n";
 outfile5<<"!  UNCOMMENT OTHER LINES AS APPROPRIATE\n";
 outfile5<<"!\n";
 outfile5<<"!    Use patran files for old-version pat2exii until Python\n";
 outfile5<<"!    version becomes faster. Then switch to output flat ..\n";
 outfile5<<"!    commands.\n";
 outfile5<<"!\n";
 outfile5<<"!  output patran formatted displacements\n";
 outfile5<<"!  output patran formatted stresses\n";
 outfile5<<"!  output patran formatted temperatures\n";
 outfile5<<"!\n";
 outfile5<<"!  output patran binary element strains stresses\n";
 outfile5<<"!  output totals only reactions nodes all\n";
 outfile5<<"!\n";
// outfile5<<"  output flat text nodal stresses\n";  //BB 1/15/2015
// outfile5<<"  output flat text nodal displacements\n";
// outfile5<<"  output flat text nodal temperatures\n";
 icount=0;
 if(Form9->CheckListBox1->Checked[ 0]){icount++;outfile5<<"  output flat text nodal stresses\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 1]){icount++;outfile5<<"  output flat text nodal displacements\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 2]){icount++;outfile5<<"  output flat text nodal temperatures\n";}  //BB 1/15/2015
//
 if(Form9->CheckListBox1->Checked[ 3]){icount++;outfile5<<"!  output flat text nodal pe11\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 4]){icount++;outfile5<<"!  output flat text nodal pe22\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 5]){icount++;outfile5<<"!  output flat text nodal pe33\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 6]){icount++;outfile5<<"!  output flat text nodal pe12\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 7]){icount++;outfile5<<"!  output flat text nodal pe13\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[ 8]){icount++;outfile5<<"!  output flat text nodal pe23\n";}  //BB 1/15/2015
//
 if(Form9->CheckListBox1->Checked[ 9]){icount++;outfile5<<"!  output flat text nodal temp.end.incr\n";}  //BB 1/15/2015
 if(Form9->CheckListBox1->Checked[10]){icount++;outfile5<<"!  output flat text nodal max.temp.occurring\n";}  //BB 1/15/2015
 outfile5.close();
///////////////////////////////////////////////////////////
 if(icount){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"*.wrp & compute_commands_all_profiles.inp written",L"Success",MB_OK);}
 else      {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No WARP3D output entities checked",L"Failure",MB_OK);}
}
//---------------------------------------------------------------------------
////void TForm1::WARP3DepBlock(long epStepsPerT) //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
//void TForm1::WARP3DepBlock() //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
void TForm1::WARP3DepBlock(String gWsiAlias) //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
//Routine to create (e.g.) tee2-warp-finalPARTIAL.inp from time.out & param.in  EFP 9/16/2012
{int solidshellsw=0;
 long ic=0,hinode=0,hielem=0,lolim=0,uplim=0,mdummy=0//,ir=0,in=0,ntimeSer=0
 ;
 float tcuth=0.f,tdummy=0.f//,epval=0.f, *timeSeries=NULL
 ;
 UnicodeString fnNeedS1,extensCharS1=UnicodeString(L".wrp");
// char buf[3+1];
// char extensChar1a[]="*input from \'temp000",extensChar1b[]="*input from \'temp00",extensChar1c[]="*input from \'temp0",extensChar1d[]="*input from \'temp",
//	  outChar1[]=".out\'",descript[76], *otNeed=NULL;
// char descript[76];
///// EFP 12/15/2012
//extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User to choose time.out, not temp.out",L"Notice",MB_OK);
/////
// OpenDialog1->Filter= "Out (*.out)|*.out;*.OUT"; //TBD: Add check on time.out vs temp.out
// if(OpenDialog1->Execute())
//   {ifstream viewfile3(OpenDialog1->FileName.c_str(),ios::nocreate,0);
//	if(viewfile3){ntimeSer=0;epval=1./float(epStepsPerT);
//				  while(!viewfile3.eof()) //EFP 7/02/2012
//					{viewfile3.getline(descript,76);
//					 if(strlen(descript))ntimeSer++;
//					}
//				  viewfile3.close();
//				  ifstream viewfile3(OpenDialog1->FileName.c_str(),ios::nocreate);
//				  timeSeries=new float[ntimeSer];
//				  for(ic=0;ic<ntimeSer;ic++)viewfile3>>in>>timeSeries[ic];
//				  viewfile3.close();
//honk<<ntimeSer<<" ntimeSer\n";
//////////////
//////////////
				  ifstream viewfile4("param.in",ios::nocreate,0); //Global param.in, in case of multi-core
				  if(viewfile4){ //Read 11-parameter param.in
viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;
viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>mdummy;viewfile4>>hinode;viewfile4>>tdummy;
viewfile4>>mdummy;viewfile4>>tdummy;viewfile4>>solidshellsw;viewfile4>>tdummy;viewfile4>>tcuth;
viewfile4>>hielem;viewfile4.close();
								if(!solidshellsw){
////////////////
////////////////
//////				  ofstream outfile("default.wrp");
////				  ofstream outfile("warp-finalPARTIAL.inp");
//				  ofstream outfile("warp-finalFULL.inp");

// char extensChar1[]=".wrp";char *fnNeed1=new char[strlen(gWsiAlias.t_str())+strlen(extensChar1)+1];
// StringCchCopy(fnNeed1,strlen(gWsiAlias.t_str())+strlen(extensChar1)+1,gWsiAlias.t_str());StringCchCat(fnNeed1,strlen(gWsiAlias.t_str())+strlen(extensChar1)+1,extensChar1);
// ofstream outfile(fnNeed1);
// delete [] fnNeed1;

 fnNeedS1=gWsiAlias+extensCharS1;
 ofstream outfile(fnNeedS1.w_str());
//xxxxxxxxxx
 int bufferSize=WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,NULL,0,NULL,NULL);
 char* m=new char[bufferSize];WideCharToMultiByte(CP_UTF8,0,gWsiAlias.w_str(), -1,m,bufferSize,NULL,NULL);
//xxxxxxxxxx

//outfile<<"c\nc\ncFEMAP 8.20\nc\nc\nc\n";
outfile<<"c\nc\nc *.wrp file\n";
////outfile<<"structure warp3d_model\nc\nc\nc\n"; //Make this the same name as *.VFTr
//outfile<<"structure "<<gWsiAlias.t_str()<<"\n";
outfile<<"structure "<<m<<"\n";
outfile<<"c\nc\nc\n";
outfile<<"c Material VFT UMAT commands\n";
//outfile<<"material steel_1e650_umat\n";  //Make name=defined material name & note that it is umat
outfile<<"material MATERIAL_umat\n";  //Make name=defined material name & note that it is umat
outfile<<"  properties umat  rho 0.0  alpha 0.0,\n";
outfile<<"       um_1 1 um_2 7.5E+02 um_3 1.5E+03,\n";
outfile<<"       um_4 1.5E+03 um_5 -1.0,\n";
outfile<<"       um_6 -1.0  um_7 0 um_8 0\n";
outfile<<"c     ***************************************\n";
outfile<<"c     *       end of sample materials       *\n";
outfile<<"c     ***************************************\n";
outfile<<"c\nc\n";
//outfile<<"number of nodes "<<hinode<<"\n";
//outfile<<"number of elements "<<hielem<<"\n";
outfile<<"number of nodes "<<hinode<<" elements "<<hielem<<"\n";
outfile<<"c\n";
//////outfile<<"*input from \'Tee2.coordinates\'\n"; //Properly name (e.g.) 'Tee2.coordinates'
////outfile<<"*input from \'default.coordinates\'\n"; //Properly name (e.g.) 'Tee2.coordinates'
//outfile<<"*input from \'"<<gWsiAlias.t_str()<<".coordinates\'\n"; //Properly name (e.g.) 'Tee2.coordinates'
outfile<<"*input from \'"<<m<<".coordinates\'\n"; //Properly name (e.g.) 'Tee2.coordinates'
outfile<<"c\nc\n";
outfile<<"elements\n";
outfile<<"c   for config number   0\n";
//outfile<<"            1 -   "<<hielem<<" type l3disop    linear material steel_1e650_umat,\n"; //Proper name
outfile<<"            1 -   "<<hielem<<" type l3disop    linear material MATERIAL_umat,\n"; //Proper name
outfile<<"					   order 2x2x2 bbar center_output short\n";
outfile<<"c\n";
//////outfile<<"*input from \'Tee2.incid\'\n";
////outfile<<"*input from \'default.incid\'\n";
//outfile<<"*input from \'"<<gWsiAlias.t_str()<<".incid\'\n";
outfile<<"*input from \'"<<m<<".incid\'\n";
outfile<<"c default blocking is 128 elem/blk\n";
outfile<<"blocking automatic\n";
outfile<<"c\nc\n";
//////outfile<<"*input from \'Tee2.constraints\'\n";
////outfile<<"*input from \'default.constraints\'\n";
//outfile<<"*input from \'"<<gWsiAlias.t_str()<<".constraints\'\n";
outfile<<"*input from \'"<<m<<".constraints\'\n";
outfile<<"c\n";
outfile<<"c  Define initial temperatures\n";
outfile<<"initial conditions\n";
outfile<<"   temperature\n";
outfile<<"	 nodes 1-"<<hinode<<" temperature "<<tcuth<<"\n";
outfile<<"c\n";
//////outfile<<"output patran neutral \"Tee2.neut\"\n";
////outfile<<"output patran neutral \'default.neut\'\n";
//outfile<<"output patran neutral \'"<<gWsiAlias.t_str()<<".neut\'\n";
outfile<<"output patran neutral \'"<<m<<".neut\'\n";
outfile<<"c\n";
outfile<<"c  From the template here are the load definitions.\n";
outfile<<"c  Total weld times are also included for convenience, not necessity\n";
outfile<<"c\n";
				  outfile<<"*echo off\n";
//
//				  lolim=1;uplim=epStepsPerT;
//				  for(ir=1;ir<ntimeSer;ir++)
//					{
//if(ir<10){gcvt(double(ir),1,buf);
//		  otNeed=new char[strlen(extensChar1a)+strlen(buf)+strlen(outChar1)+1];
//		  StringCchCopy(otNeed,strlen(extensChar1a)+strlen(buf)+strlen(outChar1)+1,extensChar1a);
//		  StringCchCat(otNeed,strlen(extensChar1a)+strlen(buf)+strlen(outChar1)+1,buf);
//		  StringCchCat(otNeed,strlen(extensChar1a)+strlen(buf)+strlen(outChar1)+1,outChar1);
//		 }
//else if(ir<100){gcvt(double(ir),2,buf);
//				otNeed=new char[strlen(extensChar1b)+strlen(buf)+strlen(outChar1)+1];
//				StringCchCopy(otNeed,strlen(extensChar1b)+strlen(buf)+strlen(outChar1)+1,extensChar1b);
//				StringCchCat(otNeed,strlen(extensChar1b)+strlen(buf)+strlen(outChar1)+1,buf);
//				StringCchCat(otNeed,strlen(extensChar1b)+strlen(buf)+strlen(outChar1)+1,outChar1);
//			   }
//else if(ir<1000){gcvt(double(ir),3,buf);
//				 otNeed=new char[strlen(extensChar1c)+strlen(buf)+strlen(outChar1)+1];
//				 StringCchCopy(otNeed,strlen(extensChar1c)+strlen(buf)+strlen(outChar1)+1,extensChar1c);
//				 StringCchCat(otNeed,strlen(extensChar1c)+strlen(buf)+strlen(outChar1)+1,buf);
//				 StringCchCat(otNeed,strlen(extensChar1c)+strlen(buf)+strlen(outChar1)+1,outChar1);
//				}
//else {gcvt(double(ir),4,buf);
//	  otNeed=new char[strlen(extensChar1d)+strlen(buf)+strlen(outChar1)+1];
//	  StringCchCopy(otNeed,strlen(extensChar1d)+strlen(buf)+strlen(outChar1)+1,extensChar1d);
//	  StringCchCat(otNeed,strlen(extensChar1d)+strlen(buf)+strlen(outChar1)+1,buf);
//	  StringCchCat(otNeed,strlen(extensChar1d)+strlen(buf)+strlen(outChar1)+1,outChar1);
//	 }
////c  step 1-20 t=12.7
//					 outfile<<"c  step "<<lolim<<"-"<<uplim<<" t="<<timeSeries[ir]<<"\n";
//					 outfile<<"loading t"<<ir<<"\n";
//					 outfile<<"nodal loads\n";
//					 outfile<<otNeed<<"\n";
//					 outfile<<"c\n";
//					 delete [] otNeed;
//					 lolim=lolim+epStepsPerT;uplim=uplim+epStepsPerT;
//					}
				  outfile<<"loading weld_temps\n";
				  outfile<<" nodal loads\n";
				  outfile<<"  user_routine\n";

outfile<<"c\nc The loading steps\nc\n";
outfile<<"c This name must be used in the following: weld_sim\n";
//				  outfile<<"loading null\n";
				  outfile<<"loading weld_sim\n";
				  outfile<<" nonlinear\n";
//				  lolim=1;uplim=epStepsPerT;
//				  for(ic=1;ic<ntimeSer;ic++)
//					{
//					 outfile<<"  step "<<lolim<<"-"<<uplim<<" t"<<ic<<" "<<epval<<" constraints "<<epval<<"\n";
//					 lolim=lolim+epStepsPerT;uplim=uplim+epStepsPerT;
//					}
				  lolim=1;uplim=10000;
				  outfile<<"  step "<<lolim<<"-"<<uplim<<" weld_temps 1.0\n";
outfile<<"c Always define 5000 or more steps\n";
outfile<<"c Actual number of steps solved is determined dynamically by the\n";
outfile<<"c user_solution routine in response to features of thermal profiles.\n";

				  outfile<<"c\nc\nc Solution parameters.\nc\n";
outfile<<" nonlinear analysis parameters\n";

//outfile<<"   solution technique direct sparse\n";
//outfile<<"c   solution technique lnpcg\n";
//outfile<<"c   solution technique direct sparse\n";
//outfile<<"c   solution technique direct sparse hp\n";
//outfile<<"c   solution technique direct sparse bcs\n";
//outfile<<"c   solution technique direct sparse sgi\n";
//outfile<<"c   preconditioner type diagonal\n";
//outfile<<"c   preconditioner type hughes-winget\n";
////outfile<<"   lnr_pcg conv test res tol 0.01\n";  //Disconnected at BBB request 12/15/2012
////outfile<<"   maximum linear iterations 20000\n"; //Disconnected at BBB request 12/15/2012
////outfile<<"   maximum iterations 5\n";
//outfile<<"   maximum iterations 10\n";
////outfile<<"   minimum iterations 2\n";
//outfile<<"   minimum iterations 1\n";
//outfile<<"   threads for warp3d 8\n";  //Added EFP 12/15/2012
//outfile<<"   threads for umat 1\n";    //Added EFP 12/15/2012
////outfile<<"   convergence test norm res tol 0.01\n";
//outfile<<"   convergence test norm res tol 0.05\n";
//outfile<<"   nonconvergent solutions stop\n";
//outfile<<"   adaptive on\n";
////outfile<<"   linear stiffness for iteration one on\n";
//outfile<<"   linear stiffness for iteration one off\n";
//outfile<<"   batch messages off\n";
//outfile<<"   cpu time limit off\n";
//outfile<<"   material messages off\n";
//outfile<<"   bbar stabilization factor 0.05\n";
//outfile<<"   consistent q-matrix on\n";
//outfile<<"   time step 0.09436531\n"; //NOTE: Time for first slice of first WP???
//outfile<<"   trace solution on lpcg_solution off\n";
//outfile<<"   extrapolate on\n";
//outfile<<"   display tied mesh mpcs off\n";

//////////////////// TBD  EFP 9/23/2012
////outfile<<" nonlinear analysis parameters\n";
////outfile<<"   solution technique direct sparse\n";
////c Next command specifies 8 threads for solver
////outfile<<"   threads for mkl 8\n";
////outfile<<"   maximum iterations 10\n";
////outfile<<"   minimum iterations 1\n";
////outfile<<"   convergence test norm res tol 0.05\n";
////outfile<<"   nonconvergent solutions stop\n";
////outfile<<"   adaptive on\n";
////outfile<<"   linear stiffness for iteration one off\n";
////outfile<<"   batch messages off\n";
////outfile<<"   cpu time limit off\n";
////outfile<<"   material messages off\n";
////outfile<<"   bbar stabilization factor 0.05\n";
////outfile<<"   time step 0.0466683\n"; //NOTE: This must be time for first slice of first WP
////outfile<<"   trace solution on\n";
////outfile<<"   extrapolate on\n";
////////////////////
outfile<<" user_routine on\n";
outfile<<" umat serial off\n";
outfile<<" solution technique direct sparse\n";
outfile<<" convergence test maximum residual tolerance 0.5\n";
outfile<<" nonconvergent solutions stop\n";
outfile<<" divergence check on\n";
outfile<<" batch messages on\n";
outfile<<" cpu time limit off\n";
outfile<<" material messages off\n";
outfile<<" bbar stabilization factor 0.05\n";
outfile<<"c The following is only used for large displ analysis\n";
outfile<<" consistent q-matrix off\n";
outfile<<" trace solution on\n";
outfile<<"c The following values ignored because controlled by user_routine\n";
outfile<<"   time step 0.09436531\n";
outfile<<"   maximum iterations 7\n";
outfile<<"   minimum iterations 1\n";
outfile<<"   adaptive solution on\n";
outfile<<"   linear stiffness for iteration one off\n";
outfile<<"   extrapolate off\n";

//outfile<<"c\nc\nc Compute steps. Makes sure delta times are correct so WARP3D can\nc obtain accurate total times.\nc\n";
//				  lolim=1;uplim=epStepsPerT;
//				  for(ic=1;ic<ntimeSer;ic++)
//					{outfile<<"c t"<<ic<<"\n";
//					 outfile<<" compute displacements for loading null step "<<lolim<<"-"<<uplim<<"\n";
//					 outfile<<" nonlinear analysis parameters\n";
//					 outfile<<" time step "<<(timeSeries[ic]-timeSeries[ic-1])/float(epStepsPerT)<<"\n";
//					 outfile<<" output patran format displacements\n";
//					 outfile<<" output patran format element strains\n";
//					 outfile<<" output patran format element stresses\n";
////					 outfile<<" output patran format stresses\n";
//					 outfile<<" save to file 'pantleg'\n"; //TBD: Replace pantleg with actual Abaqus model name
//					 lolim=lolim+epStepsPerT;uplim=uplim+epStepsPerT;
//					}
				  outfile<<"*input 'compute_commands_all_profiles.inp'\n";
				  outfile<<"stop\n";
				  outfile.close();
/////////////////////////////				  delete [] timeSeries;
 ofstream outfile1("compute_commands_all_profiles.inp");
// for(ipp=0;ipp<wp.stepInterval[k];ipp++)
 for(ic=0;ic<10000;ic++) // Allow for max 10000 "on the fly" steps per BobD's request  EFP 10/09/2014
   {outfile1<<" compute displacements loading weld_sim step "<<(ic+1)<<"\n"; //Name weld_sim ESSENTIAL
	outfile1<<"   *input from 'vft_solution_cmds.inp'\n"; //vft_solution_cmds.inp is created by WARP3D on the fly, not by us
   }
 outfile1<<"stop\n";
 outfile1.close();
				  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"*.wrp & compute_commands_all_profiles.inp written",L"Success",MB_OK);
////////////////
////////////////
//xxxxxxxxxx
 delete[] m;
//xxxxxxxxxx
												 }
								else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"WARP3D is for solid models only",L"Failure: Shell model detected",MB_OK);}
							   }
				  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Missing param.in file",L"Failure",MB_OK);}
//////////////
//////////////
//				 }
//	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open time.out",L"Failure",MB_OK);}
//   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
// F28_Form->Close();
// delete F28_Form;


}

//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWParam_currExecute(TObject *Sender)
// Usage:
// TForm28(isel,imode,fORint,vl0,vlmin,vlmax,vf0,vfmin,vfmax...)
// imode=0 (modal) & 1 (modeless)
// fORint=0 (long) & 1 (float)
// If long,  vlmin<vl0<vlmax
// If float, vfmin<vf0<vfmax
// Selector isel

//Remember to change CANCEL->CLOSE for Modeless
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wps.nWeldParamSet){F28_Form=new TForm28(1,0,1,vl0,vlmin,vlmax,wps.curr[0],vfmin,vfmax,this);
						  F28_Form->Caption="Change WeldParam current";F28_Form->Button3->Caption="Cancel";
						  F28_Form->ShowModal();
						 }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Parameters",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWParam_voltExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wps.nWeldParamSet){F28_Form=new TForm28(2,0,1,vl0,vlmin,vlmax,wps.volt[0],vfmin,vfmax,this);
						  F28_Form->Caption="Change WeldParam voltage";F28_Form->Button3->Caption="Cancel";
						  F28_Form->ShowModal();
						 }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Parameters",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWParam_effExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.f;
 if(base.nop1)
   {if(wps.nWeldParamSet){F28_Form=new TForm28(3,0,1,vl0,vlmin,vlmax,wps.eff[0],vfmin,vfmax,this);
						  F28_Form->Caption="Change WeldParam efficiency";F28_Form->Button3->Caption="Cancel";
						  F28_Form->ShowModal();
						 }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Parameters",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWParam_speedExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wps.nWeldParamSet){F28_Form=new TForm28(4,0,1,vl0,vlmin,vlmax,wps.speed[0],vfmin,vfmax,this);
						  F28_Form->Caption="Change WeldParam speed";F28_Form->Button3->Caption="Cancel";
						  F28_Form->ShowModal();
						 }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Parameters",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_ThCondExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(5,0,1,vl0,vlmin,vlmax,wms.cond[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Th.conductivity";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_SpHeatExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(6,0,1,vl0,vlmin,vlmax,wms.heat[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Specific Heat";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_DensExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(7,0,1,vl0,vlmin,vlmax,wms.den[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Density";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
//------ TBD: mcr, Steps, nprops, hetjd material choice EFP 9/02/2011 -------
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_AnnealExecute(TObject *Sender)
{long vlmin= -1,vlmax=0;float vf0=0.f,vfmin=0.f,vfmax=0.f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(8,0,0,wms.annjd[0],vlmin,vlmax,vf0,vfmin,vfmax,this);
//						F28_Form->Caption="MatProp anneal (no -1/yes 0)";
						F28_Form->Caption="MatProp anneal (yes -1/no 0)";
						F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_TiExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(9,0,1,vl0,vlmin,vlmax,wms.Ti[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Initiation T";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_TaExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(10,0,1,vl0,vlmin,vlmax,wms.Ta[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Anneal T";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_TmExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(11,0,1,vl0,vlmin,vlmax,wms.Tm[0],vfmin,vfmax,this);
						F28_Form->Caption="Change MatProp Melting T";F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllMatProp_VEDExecute(TObject *Sender)
{long vlmin= -1,vlmax=0;float vf0=0.f,vfmin=0.f,vfmax=0.f;
 if(base.nop1)
   {if(wms.nMatPropSet){F28_Form=new TForm28(12,0,0,wms.ved[0],vlmin,vlmax,vf0,vfmin,vfmax,this);
//						F28_Form->Caption="Change MatProp VED (no -1/yes 0)";
						F28_Form->Caption="Change MatProp VED (yes -1/no 0)";
						F28_Form->Button3->Caption="Cancel";
						F28_Form->ShowModal();
					   }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Material Properties",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_timeIntervExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(13,0,1,vl0,vlmin,vlmax,wp.timeInterval[0],vfmin,vfmax,this);
					 F28_Form->Caption="InterPass Cooling Time";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_stepIntervalExecute(TObject *Sender)
{long vlmin=0,vlmax=1000000;float vf0=0.f,vfmin=0.f,vfmax=0.f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(14,0,0,wp.stepInterval[0],vlmin,vlmax,vf0,vfmin,vfmax,this);
					 F28_Form->Caption="Change InterPass Cooling Steps";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_maxiHeatStepExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(15,0,1,vl0,vlmin,vlmax,wp.maxiHeatStep[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change Max Heating Time";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_miniHeatStepExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(16,0,1,vl0,vlmin,vlmax,wp.miniHeatStep[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change Min Heating Time";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_thk1Execute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(17,0,1,vl0,vlmin,vlmax,wp.thk1[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change Plate#1 thickness";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_thk2Execute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin=0.000001f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(18,0,1,vl0,vlmin,vlmax,wp.thk2[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change Plate#2 thickness";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_troomExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin= -273.1f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(20,0,1,vl0,vlmin,vlmax,wp.troom[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change room temperature";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_tmeltExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin= -273.1f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(21,0,1,vl0,vlmin,vlmax,wp.tmelt[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change melting temperature";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_tcutlExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin= -273.1f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(22,0,1,vl0,vlmin,vlmax,wp.tcutl[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change low-cut temperature";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChAllWPass_tcuthExecute(TObject *Sender)
{long vl0=0,vlmin=0,vlmax=0;float vfmin= -273.1f,vfmax=1.e20f;
 if(base.nop1)
   {if(wp.nWeldPass){F28_Form=new TForm28(23,0,1,vl0,vlmin,vlmax,wp.tcuth[0],vfmin,vfmax,this);
					 F28_Form->Caption="Change preheat temperature";F28_Form->Button3->Caption="Cancel";
					 F28_Form->ShowModal();
					}
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Create Weld Pass",L"Halt",MB_OK);}
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Get geometry file->File/Open",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::Form28_public()
{int isel=F28_Form->CheckISEL,imode=F28_Form->CheckMODE,fORint=F28_Form->CheckForI;
 long i=0,ival=0;float fval=0.f;
 if(fORint)fval=F28_Form->CheckFVAL;else ival=F28_Form->CheckIVAL;
 if(!imode){F28_Form->Close();F28_Form=NULL;}
 if(isel==1){for(i=0;i<wps.nWeldParamSet;i++)wps.curr[i]=fval;//WeldParam_curr
			 if(wp.nWeldPass)for(i=0;i<wp.nWeldPass;i++)wp.curr[i]=fval;
			}
 else if(isel==2){for(i=0;i<wps.nWeldParamSet;i++)wps.volt[i]=fval;//WeldParam_volt
				  if(wp.nWeldPass)for(i=0;i<wp.nWeldPass;i++)wp.volt[i]=fval;
				 }
 else if(isel==3){for(i=0;i<wps.nWeldParamSet;i++)wps.eff[i]=fval;//WeldParam_eff
				  if(wp.nWeldPass)for(i=0;i<wp.nWeldPass;i++)wp.eff[i]=fval;
				 }
 else if(isel==4){for(i=0;i<wps.nWeldParamSet;i++)wps.speed[i]=fval;//WeldParam_speed
				  if(wp.nWeldPass)for(i=0;i<wp.nWeldPass;i++)wp.speed[i]=fval;
				 }
//
 else if(isel==5)for(i=0;i<wms.nMatPropSet;i++)wms.cond[i]=fval;//MatProp_ThCond
 else if(isel==6)for(i=0;i<wms.nMatPropSet;i++)wms.heat[i]=fval;//MatProp_SpHeat
 else if(isel==7)for(i=0;i<wms.nMatPropSet;i++)wms.den[i]=fval;//MatProp_Density
//------ TBD: mcr, Steps, nprops, hetjd material choice EFP 9/02/2011 -------
// else if(isel==8)for(i=0;i<wms.nMatPropSet;i++)wms.annjd[i]=ival;//MatProp_annjd(-1 no/0 yes)
 else if(isel==8)for(i=0;i<wms.nMatPropSet;i++)wms.annjd[i]=ival;//MatProp_annjd(-1 yes/0 no)
 else if(isel==9)for(i=0;i<wms.nMatPropSet;i++)wms.Ti[i]=fval;//MatProp_Ti
 else if(isel==10)for(i=0;i<wms.nMatPropSet;i++)wms.Ta[i]=fval;//MatProp_Ta
 else if(isel==11)for(i=0;i<wms.nMatPropSet;i++)wms.Tm[i]=fval;//MatProp_Tm
// else if(isel==12)for(i=0;i<wms.nMatPropSet;i++)wms.ved[i]=ival;//MatProp_VED(-1 no/0 yes)
 else if(isel==12)for(i=0;i<wms.nMatPropSet;i++)wms.ved[i]=ival;//MatProp_VED(-1 yes/0 no)
//------ TBD: joint type, shapes, moving arc EFP 9/02/2011 -------
 else if(isel==13)for(i=0;i<wp.nWeldPass;i++)wp.timeInterval[i]=fval;//WPass_timeInterval
 else if(isel==14)for(i=0;i<wp.nWeldPass;i++)wp.stepInterval[i]=ival;//WPass_stepInterval
 else if(isel==15)for(i=0;i<wp.nWeldPass;i++)wp.maxiHeatStep[i]=fval;//WPass_maxiHeatStep
 else if(isel==16)for(i=0;i<wp.nWeldPass;i++)wp.miniHeatStep[i]=fval;//WPass_miniHeatStep
 else if(isel==17)for(i=0;i<wp.nWeldPass;i++)wp.thk1[i]=fval;//WPass_thk1
 else if(isel==18)for(i=0;i<wp.nWeldPass;i++)wp.thk2[i]=fval;//WPass_thk2
// else if(isel==19)for(i=0;i<wp.nWeldPass;i++)wp.xlay[i]=fval;//WPass_xlay
 else if(isel==20)for(i=0;i<wp.nWeldPass;i++)wp.troom[i]=fval;//WPass_troom
 else if(isel==21)for(i=0;i<wp.nWeldPass;i++)wp.tmelt[i]=fval;//WPass_tmelt
 else if(isel==22)for(i=0;i<wp.nWeldPass;i++)wp.tcutl[i]=fval;//WPass_tcutl
 else if(isel==23)for(i=0;i<wp.nWeldPass;i++)wp.tcuth[i]=fval;//WPass_tcuth
// else if(isel==24){if(NodalCoincidenceTest(base.npoin,base.c1,base.node_map,fval))//NodalCoincidence  EFP 9/07/2011
//					 {long tempnpoin=base.npoin;
//					  NodalCoincidenceElimCompact1(&tempnpoin,base.c1,base.node_map,fval,base.nelt,base.nop1,base.matno); //EFP 11/17/2012
//					  base.npoin=tempnpoin;
//					  extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Coincidence found & eliminated. See VFTlogfile.txt",L"Notice",MB_OK);
//					 }
//				   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"No nodal coincidence found",L"Success",MB_OK);}
//				  }
// else if(isel==25)IntervalT4=ival;//Timer4 interval
// else if(isel==26)WARP3DepBlock();//WARP3D elastic-plastic blocks  EFP 9/11/2012
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unsupported Form28_public",L"Warning",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::Form28Close_public(){F28_Form->Close();
								  if(F28_Form->CheckMODE)delete F28_Form;
								  F28_Form=NULL;
								 }
//---------------------------------------------------------------------------
void __fastcall TForm1::AboutVFTExecute(TObject *Sender)
{
// extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Unavailable",L"Warning",MB_OK);
//honk<< *base.comboCWD<<"\n";if(1==1)exit(0);
//
 About_VFT=new TForm31(base.comboCWD,this);About_VFT->Left=315;About_VFT->Top=20;
 About_VFT->Caption=L"VFT weld simulation software";
// About_VFT->Label1->Caption=L"VFTsolid version 3.2.56i 2014";
 About_VFT->Label1->Caption=VFTversion;
 About_VFT->Label2->Caption=L"VFT + CTSP are proprietary to Caterpillar Corporation.";
 About_VFT->Label3->Caption=L"Simulia/Abaqus is proprietary to Dassault Systemes.";
 About_VFT->Label4->Caption=L"WARP3D is public domain software from U.of Illinois (Urbana-Champaign)";
 About_VFT->Label5->Caption=L"VFT was developed using CodeGear C++ Builder XE7 and is available through";
 About_VFT->Label6->Caption=L"    Engineering Mechanics Corporation of Columbus (www.emc-sq.com) and";
 About_VFT->Label7->Caption=L"    Ohio Supercomputer Center (www.osc.edu).";
 About_VFT->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimeshiftCTSP0Execute(TObject *Sender) //EFP 7/05/2012
{float val=0.f;tshiftCTSP=new TForm8(2,val,this);tshiftCTSP->Caption="Timeshift CTSP";
 tshiftCTSP->Button2->Caption="Reset";tshiftCTSP->Button3->Caption="Inactive";tshiftCTSP->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimeshiftVED0Execute(TObject *Sender) //EFP 11/24/2012
{float val=0.f;tshiftCTSP=new TForm8(4,val,this);tshiftCTSP->Caption="Timeshift VED";
 tshiftCTSP->Button2->Caption="Reset";tshiftCTSP->Button3->Caption="Inactive";tshiftCTSP->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimeshiftCTSPandVED0Execute(TObject *Sender) //EFP 11/24/2012
{float val=0.f;tshiftCTSP=new TForm8(5,val,this);tshiftCTSP->Caption="Timeshift CTSP+VED";
 tshiftCTSP->Button2->Caption="Reset";tshiftCTSP->Button3->Caption="Inactive";tshiftCTSP->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MergeTimeshiftCTSP0Execute(TObject *Sender){int isw=7;tshiftCTSP2(isw);} //EFP 5/09/2013
void __fastcall TForm1::MergeTimeshiftVED0Execute(TObject *Sender){int isw=8;tshiftCTSP2(isw);}
void __fastcall TForm1::MergeTimeshiftCTSPandVED01Execute(TObject *Sender){int isw=9;tshiftCTSP2(isw);}
//---------------------------------------------------------------------------
void TForm1::tshiftCTSP_public()
//TBD: Write code here to discriminate between solid/shell & set solidshellsw  EFP 7/05/2012
{int solidshellsw=0,nic=0,nrc=0,icheck=0,vcheck=0,isw=tshiftCTSP->checkISW;
 long nnd=0,id=0,in=0,max1=0,vcount=0,larr[5];
 float tshift=tshiftCTSP->Angle0,tstep=0.f,t11=0.f,t12=0.f,t13=0.f,t14=0.f,t15=0.f,darr[5];
 char descript[76],descript1[8*1024];  //Allow for 1000 elements per VED line
//honk<<isw<<" "<<tshift<<" tshiftCTSP_public() but solid models only\n";
/////////
 icheck=0;
 if(isw==2 || isw==5){
// Typical temp.out results format (but may start at non-zero time)
//  0.0000000E+00           0
//   2.540000              41
//     45        167.27
//     52        241.83
//     53        357.34
//     55        309.45
//      .
//      .
/////////
 OpenDialog1->Filter=L"CTSP temp result (*.out)|*.out;*.OUT";
 if(OpenDialog1->Execute())
   {ifstream viewfile1(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile1)
	  {vcount=0;viewfile1.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Failure: possible time.out file?",MB_OK);}
		  else {nnd=larr[0];tstep=darr[0];vcount++;  //Code allows for non-zero nnd on first card
//honk<<tstep<<" "<<nnd<<" First card of temp.out\n";
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
			 ofstream outfile1("tempShift.out");ofstream outfile2("timeShift.out");max1=1;
			 outfile1<<setw(15)<<scientific<<(tstep+tshift)<<setw(12)<<nnd<<"\n";
			 outfile2<<setw(12)<<max1<<setw(15)<<scientific<<(tstep+tshift)<<"\n";max1++;icheck=1;
			 if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
															outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
														   } //setw(5->7) above & below for more space EFP 12/15/2011
									 }
					 else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
												 outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
												}
						  }
					}
			 while(!viewfile1.eof())
			   {viewfile1.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];
				   outfile1<<setw(15)<<scientific<<(tstep+tshift)<<setw(12)<<nnd<<"\n";
				   outfile2<<setw(12)<<max1<<setw(15)<<scientific<<(tstep+tshift)<<"\n";max1++;
				   if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
																  outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
																 } //setw(5->7) above & below for more space EFP 12/15/2011
										   }
						   else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
													   outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
													  }
								}
						  }
				  }
			   }
			 outfile2.close();outfile1.close();
//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);}
	   viewfile1.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *.out file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
///////////
					 }
 if(isw==4 || isw==5){
 // Typical ...VED.dat format
//0.000000e+00, 12
//33352, 33351, 33350, 33349, 33360, 33359, 33358, 33357, 33368, 33367, 33366, 33365,
//7.511511e-01, 12
//33348, 33347, 33346, 33345, 33356, 33355, 33354, 33353, 33364, 33363, 33362, 33361,
//1.502439e+00, 12
// .
// .
//2.627592e+04, 12
//32884, 32888, 32932, 32936, 32876, 32880, 32924, 32928, 32868, 32872, 32916, 32920,
//2.987915e+04, 8
//94425, 94433, 94441, 94449, 94427, 94435, 94443, 94451,
//2.988155e+04, 8
//94426, 94434, 94442, 94450, 94428, 94436, 94444, 94452,
 OpenDialog1->Filter=L"VED (*.dat)|*.dat;*.DAT";
 if(OpenDialog1->Execute())
   {ifstream viewfile2(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile2)
	  {vcount=0;viewfile2.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmVc(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Failure: possible MatProp.dat file?",MB_OK);}
		  else
			{nnd=larr[0];tstep=darr[0];vcount++;
//honk<<tstep<<" "<<nnd<<" First card of VED.dat\n";
		  if(nnd)
			{
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
			 ofstream outfile3("VEDshift.dat");icheck=icheck+10;
			 outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
			 viewfile2.getline(descript1,8*1024);outfile3<<descript1<<"\n";
			 while(!viewfile2.eof())
			   {viewfile2.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];
				   outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
				   viewfile2.getline(descript1,8*1024);outfile3<<descript1<<"\n";
				  }
			   }
			 outfile3.close();
//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt *VED.dat file. No elements listed.",L"Failure",MB_OK);}
			}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);}
	   viewfile2.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *VED.dat file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
					 }
///////////
 if(isw==5 && icheck==11)     {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out & VEDshift.dat",L"Success",MB_OK);}
 else if(isw==5 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out",L"Partial success with CTSP",MB_OK);}
 else if(isw==5 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDshift.dat",L"Partial success with VED",MB_OK);}
 else if(isw==2 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out",L"Success",MB_OK);}
 else if(isw==4 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDshift.dat",L"Success",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::tshiftCTSP2(int isw)
//TBD: Write code here to discriminate between solid/shell & set solidshellsw  EFP 7/05/2012
{int solidshellsw=0,nic=0,nrc=0,icheck=0,vcheck=0;
 long nnd=0,id=0,in=0,max1=0,vcount=0,larr[5];
// float tshift=tshiftCTSP->Angle0,tstep=0.,t11=0.,t12=0.,t13=0.,t14=0.,t15=0.,darr[5];
 float tshift=0.f,tstep=0.f,t11=0.f,t12=0.f,t13=0.f,t14=0.f,t15=0.f,val=0.f,peakTime=0.f,darr[5];
 char descript[76],descript1[8*1024];  //Allow for 1000 elements per VED line
//honk<<isw<<" "<<tshift<<" MergeTshiftCTSP_public() but solid models only\n";
/////////
 icheck=0;
 if(isw==7 || isw==9){
// Typical temp.out results format (but may start at non-zero time)
//  0.0000000E+00           0
//   2.540000              41
//     45        167.27
//     52        241.83
//     53        357.34
//     55        309.45
//      .
//      .
/////////
 OpenDialog1->Filter=L"CTSP temp result (*.out)|*.out;*.OUT";
 if(OpenDialog1->Execute())
   {ifstream viewfile1(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile1)
	  {vcount=0;viewfile1.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);
								  Application->MessageBox(L"Floating point not found in first number",L"Failure: possible time.out file?",MB_OK);
								  viewfile1.close();return;
								 }
		  else {nnd=larr[0];tstep=darr[0];vcount++;  //Code allows for non-zero nnd on first card
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
			 peakTime=tstep+tshift;
			 ofstream outfile1("tempMerge.out");ofstream outfile2("timeMerge.out");max1=1;
//honk<<tstep<<" "<<nnd<<" First card of Merge 1st temp.out "<<max1<<"\n";
			 outfile1<<setw(15)<<scientific<<(tstep+tshift)<<setw(12)<<nnd<<"\n";
			 outfile2<<setw(12)<<max1<<setw(15)<<scientific<<(tstep+tshift)<<"\n";max1++;icheck=1;
			 if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
															outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
														   } //setw(5->7) above & below for more space EFP 12/15/2011
									 }
					 else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
												 outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
												}
						  }
					}
			 while(!viewfile1.eof())
			   {viewfile1.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];
//honk<<tstep<<" "<<nnd<<" Subseq card of Merge 1st temp.out "<<max1<<"\n";
				   peakTime=tstep+tshift;
				   outfile1<<setw(15)<<scientific<<(tstep+tshift)<<setw(12)<<nnd<<"\n";
				   outfile2<<setw(12)<<max1<<setw(15)<<scientific<<(tstep+tshift)<<"\n";max1++;
				   if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
																  outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
																 } //setw(5->7) above & below for more space EFP 12/15/2011
										   }
						   else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
													   outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
													  }
								}
						  }
				  }
			   }
			 outfile2.close();outfile1.close();
//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		  viewfile1.close();
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);
			 viewfile1.close();return;
			}
//	   viewfile1.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Could not open *.out file",L"Failure",MB_OK);return;}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
///////////
					 }
 if(isw==8 || isw==9){
 // Typical ...VED.dat format
//0.000000e+00, 12
//33352, 33351, 33350, 33349, 33360, 33359, 33358, 33357, 33368, 33367, 33366, 33365,
//7.511511e-01, 12
//33348, 33347, 33346, 33345, 33356, 33355, 33354, 33353, 33364, 33363, 33362, 33361,
//1.502439e+00, 12
// .
// .
//2.627592e+04, 12
//32884, 32888, 32932, 32936, 32876, 32880, 32924, 32928, 32868, 32872, 32916, 32920,
//2.987915e+04, 8
//94425, 94433, 94441, 94449, 94427, 94435, 94443, 94451,
//2.988155e+04, 8
//94426, 94434, 94442, 94450, 94428, 94436, 94444, 94452,
 OpenDialog1->Filter=L"VED (*.dat)|*.dat;*.DAT";
 if(OpenDialog1->Execute())
   {ifstream viewfile2(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile2)
	  {vcount=0;viewfile2.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmVc(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Failure: possible MatProp.dat file?",MB_OK);
								  viewfile2.close();return;
								 }
		  else
			{nnd=larr[0];tstep=darr[0];vcount++;max1=1;
//honk<<tstep<<" "<<nnd<<" First card of 1st VED.dat "<<max1<<"\n";
		  if(nnd)
			{
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
			 ofstream outfile3("VEDmerge.dat");icheck=icheck+10;
			 outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
			 viewfile2.getline(descript1,8*1024);outfile3<<descript1<<"\n";
			 while(!viewfile2.eof())
			   {viewfile2.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];max1++;
//honk<<tstep<<" "<<nnd<<" Subseq card of 1st VED.dat "<<max1<<"\n";
				   outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
				   viewfile2.getline(descript1,8*1024);outfile3<<descript1<<"\n";
				  }
			   }
			 outfile3.close();
//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt *VED.dat file. No elements listed.",L"Failure",MB_OK);
				viewfile2.close();return;
			   }
			}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);
			 viewfile2.close();return;
			}
//	   viewfile2.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);
		  Application->MessageBox(L"Could not open *VED.dat file",L"Failure",MB_OK);return;
		 }
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
					 }
 extern PACKAGE void __fastcall Beep(void);Application->MessageBox(FloatToStr(peakTime).w_str(),L"Highest time to date",MB_OK);
 tshiftCTSP=new TForm8(isw,val,this);
 if(isw==7)tshiftCTSP->Caption=L"Merge+Timeshift CTSP";
 else if(isw==8)tshiftCTSP->Caption=L"Merge+Timeshift VED";
 else if(isw==9)tshiftCTSP->Caption=L"Merge+Timeshift CTSP+VED";
 tshiftCTSP->Button2->Caption=L"Reset";tshiftCTSP->Button3->Caption=L"Inactive";tshiftCTSP->ShowModal();
///////////
// if(isw==9 && icheck==11)     {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out & VEDshift.dat",L"Success",MB_OK);}
// else if(isw==9 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out",L"Partial success with CTSP",MB_OK);}
// else if(isw==9 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDshift.dat",L"Partial success with VED",MB_OK);}
// else if(isw==7 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempShift.out & timeShift.out",L"Success",MB_OK);}
// else if(isw==8 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDshift.dat",L"Success",MB_OK);}
}
//---------------------------------------------------------------------------
void TForm1::tshiftCTSP3_public()
//TBD: Write code here to discriminate between solid/shell & set solidshellsw  EFP 7/05/2012
{int solidshellsw=0,nic=0,nrc=0,icheck=0,vcheck=0,isw=tshiftCTSP->checkISW;
 long nnd=0,id=0,in=0,max1=0,m2ax1=0,vcount=0,larr[5];
 float tshift=tshiftCTSP->Angle0,lastTime=0.f,tstep=0.f,t11=0.f,t12=0.f,t13=0.f,t14=0.f,t15=0.f,darr[5];
 char descript[76],descript1[8*1024];  //Allow for 1000 elements per VED line
//honk<<isw<<" "<<tshift<<" Merge2tshiftCTSP_public() but solid models only\n";

//if(1==1)exit(0);
/////////
 icheck=0;
 if(isw==7 || isw==9){
// Typical temp.out results format (but may start at non-zero time)
//  0.0000000E+00           0
//   2.540000              41
//     45        167.27
//     52        241.83
//     53        357.34
//     55        309.45
//      .
//      .
/////////
 OpenDialog1->Filter=L"CTSP temp result (*.out)|*.out;*.OUT";
 if(OpenDialog1->Execute())
   {ifstream viewfile1(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile1)
	  {vcount=0;viewfile1.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Failure: possible time.out file?",MB_OK);}
		  else {nnd=larr[0];tstep=darr[0];vcount++;  //Code allows for non-zero nnd on first card
//honk<<tstep<<" "<<nnd<<" First card of 2nd temp.out\n";
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
//				 try {
			 ifstream viewfile3("timeMerge.out",ios::nocreate);max1=0;
			 while(!viewfile3.eof()){viewfile3.getline(descript,76);
									 if(strlen(descript)){vcheck=parse_cdmV(descript,2,&nic,&nrc,larr,darr);
if(vcheck>=0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Integer not found in first number of time.out",L"Terminate: possible temp.out file?",MB_OK);exit(0);}
														  max1++;lastTime=darr[0];
														 }
									}
			 viewfile3.close();
//honk<<max1<<" #times in imported timeMerge to date & tstep "<<lastTime<<" "<<tstep<<"\n";

//			 if(tstep+tshift>lastTime){
			 tshift=lastTime-tstep;
			 extern PACKAGE void __fastcall Beep(void);Application->MessageBox(FloatToStr(tshift).w_str(),L"Override user input",MB_OK);
			 ofstream outfile1("tempMerge.out",ios::nocreate | ios::app);ofstream outfile2("timeMerge.out",ios::nocreate | ios::app);
m2ax1=1;honk<<tstep<<" "<<nnd<<" First card of 2nd temp.out (not written) "<<m2ax1<<"\n";m2ax1++;
//			 max1=1;
//			 outfile1<<setw(15)<<scientific<<tstep+tshift<<setw(12)<<nnd<<"\n";
//			 outfile2<<setw(12)<<max1<<setw(15)<<scientific<<tstep+tshift<<"\n";
//			 max1++;
			 icheck=1;
			 if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
//															outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
														   } //setw(5->7) above & below for more space EFP 12/15/2011
									 }
					 else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
//												 outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
												}
						  }
					}
			 while(!viewfile1.eof())
			   {viewfile1.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];
honk<<tstep<<" "<<nnd<<" Subseq card of 2nd temp.out "<<m2ax1<<"\n";m2ax1++;
				   outfile1<<setw(15)<<scientific<<(tstep+tshift)<<setw(12)<<nnd<<"\n";
				   max1++;outfile2<<setw(12)<<max1<<setw(15)<<scientific<<(tstep+tshift)<<"\n";
				   if(nnd){if(solidshellsw){for(in=0;in<nnd;in++){viewfile1>>id>>t11>>t12>>t13>>t14>>t15;
																  outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<setw(14)<<t12<<setw(14)<<t13<<setw(14)<<t14<<setw(14)<<t15<<setw(14)<<"\n";
																 } //setw(5->7) above & below for more space EFP 12/15/2011
										   }
						   else {for(in=0;in<nnd;in++){viewfile1>>id>>t11;
													   outfile1<<setw(7)<<id<<setw(14)<<dec<<showpoint<<setprecision(5)<<t11<<"\n";
													  }
								}
						  }
				  }
			   }
			 outfile2.close();outfile1.close();
//									  }
//			 else {DeleteFile("tempMerge.out");DeleteFile("timeMerge.out");if(isw==9)DeleteFile("VEDmerge.dat");
//				   extern PACKAGE void __fastcall Beep(void);
//				   Application->MessageBox(FloatToStr(lastTime-tstep).w_str(),L"Failure (overlap) so increase Timeshift to at least",MB_OK);
//				   return;
//				  }

//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);}
	   viewfile1.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *.out file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
///////////
					 }
 if(isw==8 || isw==9){
 // Typical ...VED.dat format
//0.000000e+00, 12
//33352, 33351, 33350, 33349, 33360, 33359, 33358, 33357, 33368, 33367, 33366, 33365,
//7.511511e-01, 12
//33348, 33347, 33346, 33345, 33356, 33355, 33354, 33353, 33364, 33363, 33362, 33361,
//1.502439e+00, 12
// .
// .
//2.627592e+04, 12
//32884, 32888, 32932, 32936, 32876, 32880, 32924, 32928, 32868, 32872, 32916, 32920,
//2.987915e+04, 8
//94425, 94433, 94441, 94449, 94427, 94435, 94443, 94451,
//2.988155e+04, 8
//94426, 94434, 94442, 94450, 94428, 94436, 94444, 94452,
 OpenDialog1->Filter=L"VED (*.dat)|*.dat;*.DAT";
 if(OpenDialog1->Execute())
   {ifstream viewfile2(OpenDialog1->FileName.w_str(),ios::nocreate);
	if(viewfile2)
	  {vcount=0;viewfile2.getline(descript,76);
	   if(strlen(descript))
		 {vcheck=parse_cdmVc(descript,2,&nic,&nrc,larr,darr);
		  if(!vcount && vcheck<0){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Floating point not found in first number",L"Failure: possible MatProp.dat file?",MB_OK);}
		  else
			{nnd=larr[0];tstep=darr[0];vcount++;
m2ax1=1;honk<<tstep<<" "<<nnd<<" First card of 2nd VED.dat "<<m2ax1<<"\n";m2ax1++;
//TBD: Test for nonoverlapping VED only ==> DeleteFile(VEDmerge.dat)
		  if(nnd)
			{
///////////////////////////
				 TCursor Save_Cursor=Screen->Cursor;Screen->Cursor=crHourGlass;
////				 try {
			 ofstream outfile3("VEDmerge.dat",ios::nocreate | ios::app);icheck=icheck+10;
			 outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
			 viewfile2.getline(descript1,8*1024);
			 outfile3<<descript1<<"\n";
			 while(!viewfile2.eof())
			   {viewfile2.getline(descript,76);
				if(strlen(descript))
				  {parse_cdm(descript,2,&nic,&nrc,larr,darr);nnd=larr[0];tstep=darr[0];
//honk<<tstep<<" "<<nnd<<" Subseq card of 2nd VED.dat "<<m2ax1<<"\n";m2ax1++;
				   outfile3<<setw(12)<<scientific<<(tstep+tshift)<<","<<setw(4)<<nnd<<"\n";
				   viewfile2.getline(descript1,8*1024);outfile3<<descript1<<"\n";
				  }
			   }
			 outfile3.close();
//					 }
//				 __finally {
			 Screen->Cursor=Save_Cursor;
//			 }
///////////////////////////
			}
		  else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Corrupt *VED.dat file. No elements listed.",L"Failure",MB_OK);}
			}
		 }
	   else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Empty file",L"Failure",MB_OK);}
	   viewfile2.close();
	  }
	else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not open *VED.dat file",L"Failure",MB_OK);}
   }
// else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"Could not create FileOpen selector",L"Failure",MB_OK);}
					 }
///////////
 if(isw==9 && icheck==11)     {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempMerge.out & timeMerge.out & VEDMerge.dat",L"Success",MB_OK);}
 else if(isw==9 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempMerge.out & timeMerge.out",L"Partial success with CTSP",MB_OK);}
 else if(isw==9 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDMerge.dat",L"Partial success with VED",MB_OK);}
 else if(isw==7 && icheck==1) {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename tempMerge.out & timeMerge.out",L"Success",MB_OK);}
 else if(isw==8 && icheck==10){extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"User should rename VEDMerge.dat",L"Success",MB_OK);}
}
//---Reset------------------------------------------------------------------
void TForm1::tshiftCTSP1_public(){float val=0.f;tshiftCTSP->setAngle0(val);}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick0Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=30;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"124Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick1Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=31;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"125Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick2Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=32;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"126Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick3Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=33;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"127Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick4Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=34;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"128Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick5Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=35;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"129Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick6Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=36;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"130Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BCsNclick7Execute(TObject *Sender)
{if(base.nop1)
   {FD_LBrec=FD_LButtonstatus;FD_LButtonstatus=37;
   }
 else {extern PACKAGE void __fastcall Beep(void);Application->MessageBox(L"131Get geometry file->File/Open.",L"Halt",MB_OK);}
}
//---------------------------------------------------------------------------

