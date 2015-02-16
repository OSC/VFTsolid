//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <StrUtils.hpp> //SplitString(String,Delimiter)  EFP 1/11/2015
#include <SysUtils.hpp> //TReplaceFlags(,),StringReplace()  EFP 1/15/2015
#include <WideStrUtils.hpp> //  EFP 1/15/2015

#include <strsafe.h>
////////////////////////////////////
#include <dos.h>
//#include <System.Actions.hpp> //Used by sleep(seconds)
////////////////////////////////////

//#define	MXNPELS      8
#define	MXNPELS      20
#define	NDF         3
#define	MAXPOINTS    50      // #nodes in Polygon
#define	MAX_GID   4096
#define	LONG_INT  2147483647
#define MPROP         19
//#define MAX_WELDGROUPS 25
//#define MAX_WELDPASSES 25

#define	MAX_POLYPOINTS  64
//#define	MAX_WELDGROUPS  25
#define	MAX_WELDGROUPS  500
#define	MAX_WELDPASSES  16*16-2
#define	MAX_MATPROPSET  10
#define	MAX_WELDPARSET  10

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPanel *Panel1;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn6;
	TBitBtn *BitBtn7;
	TBitBtn *BitBtn8;
	TBitBtn *BitBtn9;
	TBitBtn *BitBtn10;
	TTimer *Timer1;
	TPaintBox *PaintBox1;
	TOpenDialog *OpenDialog1;
	TOpenDialog *OpenDialog2;
	TSaveDialog *SaveDialog1;
	TMainMenu *MainMenu1;
	TActionList *ActionList1;
	TMenuItem *File1;
	TMenuItem *Open1;
	TAction *ImportAba;
	TAction *ImportMsh;
	TMenuItem *ImportAba1;
	TMenuItem *ImportMsh1;
	TAction *FileClose;
	TAction *Exit;
	TMenuItem *FileClose1;
	TMenuItem *Exit1;
	TAction *SaveAs2;
	TMenuItem *SaveAs21;
	TAction *CTSPinterpSolid;
	TMenuItem *CTSPinterpSolid1;
	TMenuItem *Mesh1;
	TMenuItem *Query1;
	TAction *ElFacetArea;
	TMenuItem *ElFacetArea1;
	TAction *FEstats;
	TMenuItem *FEstats1;
	TAction *QueryNode;
	TMenuItem *QueryNode1;
	TAction *NodalDistance;
	TMenuItem *NodalDistance1;
	TAction *QueryElement;
	TMenuItem *QueryElement1;
	TAction *Normal_direction1;
	TAction *Complementary_direction1;
	TMenuItem *View1;
	TMenuItem *Normaldirection1;
	TMenuItem *Normaldirection11;
	TMenuItem *Complementarydirection11;
	TAction *BackroundColorF0;
	TAction *BackroundColorF1;
	TAction *BackroundColorF2;
	TAction *BackroundColorF3;
	TAction *BackroundColorF4;
	TMenuItem *Backroundcolor1;
	TMenuItem *BackroundColorF01;
	TMenuItem *BackroundColorF11;
	TMenuItem *BackroundColorF21;
	TMenuItem *BackroundColorF31;
	TMenuItem *BackroundColorF41;
	TAction *XY_rot;
	TAction *XYm_rot;
	TAction *YZ_rot;
	TAction *YZm_rot;
	TAction *ZX_rot;
	TAction *ZXm_rot;
	TMenuItem *XYrot1;
	TMenuItem *XYmrot1;
	TMenuItem *YZrot1;
	TMenuItem *YZmrot1;
	TMenuItem *ZXrot1;
	TMenuItem *ZXmrot1;
	TAction *WeldParam;
	TMenuItem *Weld1;
	TMenuItem *Weldparameters1;
	TMenuItem *WeldParam1;
	TAction *EditWeldParam;
	TMenuItem *EditWeldParam1;
	TAction *DeleteWeldParam;
	TMenuItem *DeleteWeldParam1;
	TAction *MatProperties;
	TAction *EditMatProp;
	TAction *DeleteMatProp;
	TMenuItem *Materialproperties1;
	TMenuItem *MatProperties1;
	TMenuItem *EditMatProp1;
	TMenuItem *DeleteMatProp1;
	TAction *CreateWPFullLengthFullWidth;
	TAction *CreateWPFullLengthPartWidth;
	TAction *CreateWPPartLengthFullWidth;
	TAction *CreateWPPartLengthPartWidth;
	TMenuItem *Weldpasses1;
	TMenuItem *CreateWPFullLengthFullWidth1;
	TMenuItem *CreateWPFullLengthPartWidth1;
	TMenuItem *Partlengthweldpass1;
	TMenuItem *CreateWPPartLengthFullWidth1;
	TMenuItem *CreateWPPartLengthPartWidth1;
	TAction *EditWeldPass;
	TAction *DeleteWeldPass;
	TAction *WeldPassEditingandSequencing1;
	TMenuItem *EditWeldPass1;
	TMenuItem *DeleteWeldPass1;
	TMenuItem *WeldPassEditingandSequencing11;
	TAction *exportCTSP;
	TAction *exportWARP3D;
	TMenuItem *Export1;
	TMenuItem *exportCTSP1;
	TMenuItem *exportWARP3D1;
	TAction *ChAllWParam_curr;
	TAction *ChAllWParam_volt;
	TAction *ChAllWParam_eff;
	TAction *ChAllWParam_speed;
	TAction *ChAllMatProp_ThCond;
	TAction *ChAllMatProp_SpHeat;
	TAction *ChAllMatProp_Dens;
	TAction *ChAllMatProp_Anneal;
	TAction *ChAllMatProp_Ti;
	TAction *ChAllMatProp_Ta;
	TAction *ChAllMatProp_Tm;
	TAction *ChAllMatProp_VED;
	TAction *ChAllWPass_timeInterv;
	TAction *ChAllWPass_stepInterval;
	TAction *ChAllWPass_maxiHeatStep;
	TAction *ChAllWPass_miniHeatStep;
	TAction *ChAllWPass_thk1;
	TAction *ChAllWPass_thk2;
	TAction *ChAllWPass_troom;
	TAction *ChAllWPass_tmelt;
	TAction *ChAllWPass_tcutl;
	TAction *ChAllWPass_tcuth;
	TMenuItem *Changeall1;
	TMenuItem *Weldparameters2;
	TMenuItem *ChAllWParamcurr1;
	TMenuItem *ChAllWParamvolt1;
	TMenuItem *ChAllWParameff1;
	TMenuItem *ChAllWParamspeed1;
	TMenuItem *Materialproperties2;
	TMenuItem *ChAllMatPropThCond1;
	TMenuItem *ChAllMatPropSpHeat1;
	TMenuItem *ChAllMatPropDens1;
	TMenuItem *Anneal1;
	TMenuItem *ChAllMatPropAnneal1;
	TMenuItem *ChAllMatPropTi1;
	TMenuItem *ChAllMatPropTa1;
	TMenuItem *ChAllMatPropTm1;
	TMenuItem *ChAllMatPropVED1;
	TMenuItem *Weldpassdata1;
	TMenuItem *CTSPinputs1;
	TMenuItem *ChAllWPasstimeInterv1;
	TMenuItem *ChAllWPassstepInterval1;
	TMenuItem *ChAllWPassmaxiHeatStep1;
	TMenuItem *ChAllWPassminiHeatStep1;
	TMenuItem *ChAllWPassthk11;
	TMenuItem *ChAllWPassthk21;
	TMenuItem *Weldparameters3;
	TMenuItem *ChAllWPasstroom1;
	TMenuItem *ChAllWPasstmelt1;
	TMenuItem *ChAllWPasstcutl1;
	TMenuItem *ChAllWPasstcuth1;
	TAction *AboutVFT;
	TMenuItem *Help1;
	TMenuItem *AboutVFT1;
	TAction *TimeshiftCTSPandVED0;
	TAction *TimeshiftCTSP0;
	TAction *TimeshiftVED0;
	TMenuItem *imeshift1;
	TMenuItem *imeshiftCTSPandVED01;
	TMenuItem *imeshiftCTSP01;
	TMenuItem *imeshiftVED01;
	TAction *MergeTimeshiftCTSPandVED01;
	TAction *MergeTimeshiftCTSP0;
	TAction *MergeTimeshiftVED0;
	TAction *ImportVFTr;
	TMenuItem *ImportVFTr1;
	TAction *BCsNclick0;
	TAction *BCsNclick1;
	TAction *BCsNclick2;
	TAction *BCsNclick3;
	TAction *BCsNclick4;
	TAction *BCsNclick5;
	TAction *BCsNclick6;
	TAction *BCsNclick7;
	TMenuItem *BCs1;
	TMenuItem *Addbynodeclick1;
	TMenuItem *Removebyclick1;
	TMenuItem *BCsNclick01;
	TMenuItem *BCsNclick11;
	TMenuItem *BCsNclick21;
	TMenuItem *BCsNclick31;
	TMenuItem *BCsNclick41;
	TMenuItem *BCsNclick51;
	TMenuItem *BCsNclick61;
	void __fastcall ImportAbaExecute(TObject *Sender);
	void __fastcall ImportMshExecute(TObject *Sender);
	void __fastcall FileCloseExecute(TObject *Sender);
	void __fastcall ExitExecute(TObject *Sender);
	void __fastcall Form1Paint(TObject *Sender);
	void __fastcall SaveAs2Execute(TObject *Sender);
	void __fastcall CTSPinterpSolidExecute(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
          TPoint &MousePos, bool &Handled);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ElFacetAreaExecute(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FEstatsExecute(TObject *Sender);
	void __fastcall QueryNodeExecute(TObject *Sender);
	void __fastcall NodalDistanceExecute(TObject *Sender);
	void __fastcall QueryElementExecute(TObject *Sender);
	void __fastcall BitBtn9Click(TObject *Sender);
	void __fastcall BitBtn10Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall BitBtn7Click(TObject *Sender);
	void __fastcall BitBtn8Click(TObject *Sender);
	void __fastcall Normal_direction1Execute(TObject *Sender);
	void __fastcall Complementary_direction1Execute(TObject *Sender);
	void __fastcall BackroundColorF0Execute(TObject *Sender);
	void __fastcall BackroundColorF1Execute(TObject *Sender);
	void __fastcall BackroundColorF2Execute(TObject *Sender);
	void __fastcall BackroundColorF3Execute(TObject *Sender);
	void __fastcall BackroundColorF4Execute(TObject *Sender);
	void __fastcall XY_rotExecute(TObject *Sender);
	void __fastcall XYm_rotExecute(TObject *Sender);
	void __fastcall YZ_rotExecute(TObject *Sender);
	void __fastcall YZm_rotExecute(TObject *Sender);
	void __fastcall ZX_rotExecute(TObject *Sender);
	void __fastcall ZXm_rotExecute(TObject *Sender);
	void __fastcall WeldParamExecute(TObject *Sender);
	void __fastcall EditWeldParamExecute(TObject *Sender);
	void __fastcall DeleteWeldParamExecute(TObject *Sender);
	void __fastcall MatPropertiesExecute(TObject *Sender);
	void __fastcall EditMatPropExecute(TObject *Sender);
	void __fastcall DeleteMatPropExecute(TObject *Sender);
	void __fastcall CreateWPFullLengthFullWidthExecute(TObject *Sender);
	void __fastcall CreateWPFullLengthPartWidthExecute(TObject *Sender);
	void __fastcall CreateWPPartLengthFullWidthExecute(TObject *Sender);
	void __fastcall CreateWPPartLengthPartWidthExecute(TObject *Sender);
	void __fastcall EditWeldPassExecute(TObject *Sender);
	void __fastcall DeleteWeldPassExecute(TObject *Sender);
	void __fastcall WeldPassEditingandSequencing1Execute(TObject *Sender);
	void __fastcall exportCTSPExecute(TObject *Sender);
	void __fastcall exportWARP3DExecute(TObject *Sender);
	void __fastcall ChAllWParam_currExecute(TObject *Sender);
	void __fastcall ChAllWParam_voltExecute(TObject *Sender);
	void __fastcall ChAllWParam_effExecute(TObject *Sender);
	void __fastcall ChAllWParam_speedExecute(TObject *Sender);
	void __fastcall ChAllMatProp_ThCondExecute(TObject *Sender);
	void __fastcall ChAllMatProp_SpHeatExecute(TObject *Sender);
	void __fastcall ChAllMatProp_DensExecute(TObject *Sender);
	void __fastcall ChAllMatProp_AnnealExecute(TObject *Sender);
	void __fastcall ChAllMatProp_TiExecute(TObject *Sender);
	void __fastcall ChAllMatProp_TaExecute(TObject *Sender);
	void __fastcall ChAllMatProp_TmExecute(TObject *Sender);
	void __fastcall ChAllMatProp_VEDExecute(TObject *Sender);
	void __fastcall ChAllWPass_timeIntervExecute(TObject *Sender);
	void __fastcall ChAllWPass_stepIntervalExecute(TObject *Sender);
	void __fastcall ChAllWPass_maxiHeatStepExecute(TObject *Sender);
	void __fastcall ChAllWPass_miniHeatStepExecute(TObject *Sender);
	void __fastcall ChAllWPass_thk1Execute(TObject *Sender);
	void __fastcall ChAllWPass_thk2Execute(TObject *Sender);
	void __fastcall ChAllWPass_troomExecute(TObject *Sender);
	void __fastcall ChAllWPass_tmeltExecute(TObject *Sender);
	void __fastcall ChAllWPass_tcutlExecute(TObject *Sender);
	void __fastcall ChAllWPass_tcuthExecute(TObject *Sender);
	void __fastcall AboutVFTExecute(TObject *Sender);
	void __fastcall TimeshiftCTSPandVED0Execute(TObject *Sender);
	void __fastcall TimeshiftCTSP0Execute(TObject *Sender);
	void __fastcall TimeshiftVED0Execute(TObject *Sender);
	void __fastcall MergeTimeshiftCTSPandVED01Execute(TObject *Sender);
	void __fastcall MergeTimeshiftCTSP0Execute(TObject *Sender);
	void __fastcall MergeTimeshiftVED0Execute(TObject *Sender);
	void __fastcall ImportVFTrExecute(TObject *Sender);
	void __fastcall BCsNclick0Execute(TObject *Sender);
	void __fastcall BCsNclick1Execute(TObject *Sender);
	void __fastcall BCsNclick2Execute(TObject *Sender);
	void __fastcall BCsNclick3Execute(TObject *Sender);
	void __fastcall BCsNclick4Execute(TObject *Sender);
	void __fastcall BCsNclick5Execute(TObject *Sender);
	void __fastcall BCsNclick6Execute(TObject *Sender);
	void __fastcall BCsNclick7Execute(TObject *Sender);

private:	// User declarations
///////////////////////////////////////////////////////////
 struct FEdataD {int *arELEM;
				 long npoin,nelt,nvfix,nedge,pload,mat,nblod,matsteps,ncoorf,allGrp,ELSETelsum,GIDcol,
				  *nop1,*matno,*nlp,*nofix,*ifpre,*nrfix,*nldel,*lodpt,*matyp,*ebody,*el_map,*node_map// ,*lbce,*lbcn
				  ,*orig_matno,*arrELSET
//,*orig_arrELSET,*trackELSET
				 ;
				 float *presc,*press,*pointld,*prop,*body,*cangl, *c1,*res1,*beta;
///////////////// EFP 2/25/2012
char *comboCWD;
/////////////////
				 String *groupsname, *ELSETinputnames; //EFP 3/15/2011 & 01/01/2015
				} base;
 struct FEdata1D{long npoin,nelt,nvfix,nedge,nfsect,GIDcol,*nop1,*matno,*fsect,*nlp//	  ,*nofix,*nldel,*lbce,*lbcn
				 ,*matnoW,*arrELSET
//,*orig_arrELSET,*trackELSET
				 ;
				 float *c1,*bc1,*result,*res1,*beta;
				} indat;
// Axis convention: isw=0-> "1,2,3" symbol, =1-> "x,y,z"
//x[0],y,z,etc= origin
//x[1]        = x-tip
//x[2]        = y-tip
//x[3]        = z-tip
//x[4]        = x axis symbol
//x[5]        = y axis symbol
//x[6]        = z axis symbol
 struct AxisPlot{int isw;float x[7],y[7],z[7];
				} ap;
// Start from VFTgen
 struct WeldPassProp{int CreateWPassMode,PRECORD,EDIT_SEL,PAINT_SEL,avis,pending,
						 temp_seqNum,temp_boolFlags,temp_type,temp_shape,temp_stepInterval,
						 temp_hp,temp_nsegs,temp_iselect,temp_mcr,temp_source,
						 *seqNum,*boolFlags,*type,*shape,*stepInterval,*hp,*nsegs,*iselect,*mcr
						 ,*source;
					 long temp_util_arr,*util_arr;
// CreateWPassMode=0 -> edit; =1 -> Create
// GIDwp: 1st col= current iside, remainder=GID
					 long nWeldPassEl,count_curr_sttEl,GIDwp,
						  nWeldPass,nWeldGroup,memWGa,highest,elStart,
						  temp_stpEle,temp_n_curr_sttEl,temp_prevGID,Record,edit_sel,paint_sel,
						  temp_reset,*prevGID,
						  *eles,*sttEles,*stpEle,*firstEle,*nextEle,*snorm1,*snorm2,
						   *circEles,*edgeEles,*edgeNodes,*sttEleNodes,*hlightel,*n_curr_sttEl,*reset
						  ,*temp_eles,*temp_sttEles,*temp_sttEleNodes
						   ;
					 float temp_curr,temp_volt,temp_eff,temp_speed,temp_troom,temp_tmelt,temp_tcutl,temp_tcuth,temp_timeInterval,
						   temp_maxiHeatStep,temp_miniHeatStep,temp_thk1,temp_thk2,temp_wpTimes,
						   *curr,*volt,*eff,*speed,*troom,*tmelt,*tcutl,*tcuth,*timeInterval,
						   *maxiHeatStep,*miniHeatStep,*thk1,*thk2,*wpTimes
//						   ,*source
						   ;
					 double temp_lstart,temp_lend,temp_arrows,*lstart,*lend,*arrows;
					 String temp_name,temp_matName,*name,*matName;
					 TColor temp_WeldColor,*WeldColor;
// boolFlags[]: showFlag=1st digit,circFlag=2nd,edgeFlag=3rd,merge=4th,girthFlag=5th
					} wp;
 struct MatPropSet{int temp_mcr,temp_annjd,temp_ved,temp_hetjd,temp_nprops,*mcr,*annjd,*ved,*hetjd,*nprops;
				   long nMatPropSet,highest;
				   float temp_cond,temp_heat,temp_den,temp_Ti,temp_Ta,temp_Tm,*cond,*heat,*den,*Ti,*Ta,*Tm;
				   String temp_name,temp_matFileName,temp_Steps,*name,*matFileName,*Steps;
				   bool temp_switc,*switc;
				  } wms;
 struct WeldParamSet{long nWeldParamSet,highest;
					 float temp_curr,temp_volt,temp_eff,temp_speed,*curr,*volt,*eff,*speed;
					 String temp_name,*name;
					} wps;
 String gWsiAlias;
 char *VFTr_name,modelName_g[260]; //Warning#1: modelName_g[260] does NOT have file extension. See Borland C++ v4.0 Lib Ref p106 fnsplit() & dir.h
// Warning#2: For VFTr file import, both VFTr_name & modelName_g are used.
// End from VFTgen


 Graphics::TBitmap *tBitmap;
 int iplotflag,iPaintyesno,iCircleplot,iCullyesno,nGIDmax,fl_feres,nGID,stateVFT,VFTon,
	 iBackroundColor,iPersistVFT,iplotType,
	 polycord[2*MAX_POLYPOINTS],polypts,Xpan0,Ypan0,Xpan1,Ypan1,panMouseM,
	 stateVFTrec,FD_LBrec,FD_LButtonstatus;
 long new_nelt,new_npoin,new_nvfix,new_nplod,new_nedge,new_mat,new_ncoorf,old_npoin,nColRes,qndNodeNum1,MXNPEL,MXNPEI;
	 TRect zoomRect;BOOL zoomDrag;TMouseButton Button;
  float rangle0[3],rangle[3],trans_op[3],prod_op[3][3],trans_zoom[3],prod_zoom[3],rot_op[3][3],glGdiff,gl_indatMedian[3],shapecombo;
  long *rbTemp,*arbFacet,nFacets,arGID[MAX_GID];
  int cxWidth0,cyHeight0;
///////////////////////////////////////////////////////////
  void FDdynmem_manage(int,long,long,long,long,long,long,long,long,long,long,long,long,long),
	   axis_plot_init(),
	   axis_plotXYZ(int,int),
	   FDpmargin(long,long,long [],long [],float [],float []),
	   FDpscale(long,float [],float [][3]),
	   FDwireplot(long,long [],long [],float []),
	   FDpaintplot(int,long [],long [],float [],long,long[]),
	   FDcomp_nGID(long,int *,long []),
	   FDelemfacets3(long,long,long [],long [],long []),
	   FDelemfacets3a(long,long,long [],long []),
	   FDelemfacets_arE3(long,long,long [],long [],int []);
  long FDcullfacetc(long,long [],long [],float [],long [],long [],long []),
	   FDcullfacet_arEc(long,long [],long [],float [],int [],long [],long []);
  void FDsort_low1(long *,float *,long,long *,long *,long),
	   sort_by_partition(long *,float *,long,long *,long *,long *,long *,long *,long *,float *,long,long,long,float,float,long *),
	   sort_by_stride(long *,float *,long *,long *,long *,long,long),
	   sort_by_bubble(long *,float *,long *,long *,long *,long,long),
	   STFISO20(int,float,float,float,float *,float [],float [],float [],float [],long [],float []),
	   STFISO15(int,float,float,float,float *,float [],float [],float [],float [],long [],float []),
	   STFISO10(int,float,float,float,float *,float [],float [],float [],float [],long [],float []),
	   STFISO8_ncalc(long,long,float [],long [],float []),
	   STFISO8(int,float,float,float,float *,float [],float [],float [],float [],long [],float []),
	   STFISO6(int,float,float,float,float *,float [],float [],float [],float [],long [],float []),
	   STFISO4(int,float,float,float,float *,float [],float [],float [],float [],long [],float []);
  void persistVFT1a(long,long []),
	   persistVFT2(long,long []),
	   ImportAba_prog(int),
	   degen8_test(long*,long*,long []);// Convert degenerate hex to wedge/tetra  EFP 8/30/2014
  void parse_cdm3ff(char [],long,int*,int*,long [],float []),
	   parse_cdmQ(char [],int*,int*,long [],float []),  //This is repeated in Unit21.cpp as ....fixthis()
	   parse_cdm(char [],long,int*,int*,long [],float []),parse_cdmQn(char [],long,int*,int*,long [],float []),
	   parse_cdmQb(char [],int*,int*,long [],float [],long);
  int parse_cdmV(char [],long,int*,int*,long [],float []),parse_cdmVc(char [],long,int*,int*,long [],float []);
  void FDbase_indat(int,float,int,long),
	   CmZoomIn2(long,float [],float [],float [][3],float [],float []),
	   FDrotate(int,long,float [],float []),
	   FDrotate_axisplot(int,float[]),
	   maskelemPolyhea(int),
	   est_polytri(int [],int *);
  int poly_contain(int [],int,float,float);
  long FDnode_interrog1(int,int,long [],long [],float [],long,long []);
  void FDrestore(),
	   FDrot_status(long,float [],float [][3]),
       FDelem_interrog1(int,int,long *,long *,float *,float *,float *,long [],long [],float [],long,long []),
	   FDelem_interrogpt1(long,long,long,long,float,float,float,
		   float *,float *,float *,float *,float *,float *,float *,float *,
		   long [],long [],float [],float []),
	   homsubm(long,long,float,float,float *,float *,float *,float *,float,float,float,long,long,long [],float [],float),
	   invdet(long,long [],float []),
	   VFT_SaveAs1(int),
	   CTSPinterpolate_prog(int);
  float ElemFacetArea(long,long,long [],long [],float []);
  void Rot_program(int,float,float,float),
	   EditWeldParamProg(int),
	   DelWeldPass(int,long),
	   EditMatPropProg(int),
	   DelMatPropProg(int),
	   EditWeldPassProg(int),
	   DelWeldParam(int),
	   create_new_weld(int),
	   RevProg0(long);
	   int DelSubd0(int,String);
	   void export_CTSP_NODE(), // Identical to ABAQUS-format ASCII nodal geometry file *.inp (or *.in), delimited by ","
	        export_CTSP_ELEMENT8(), // Identical to ABAQUS format ASCII element (8n only, with element mapping) connectivity file *.inp (or *.in), delimited by ","
			export_CTSP_INPUTp1(float),
			export_CTSP_INPUTp_mcm(long,long,int,float);
//	   void export_VED(char [],float *); // VED= virtual element detection  (based on WSIExportABA.java export_VED)
	   void export_VED(String,float *,int); // VED= virtual element detection  (based on WSIExportABA.java export_VED)
//	   void WARP3DepBlock(); //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
	   void WARP3DepBlock(String); //THIS IS ORIGINAL OUTPUT FORMAT (pre 12/2012)
	   void tshiftCTSP2(int);
public:		// User declarations
	AnsiString GeomFileName;
	void QNodeTRASH_public(),
		 QNDistTRASH_public(),
		 QElemTRASH_public(),
		 WeldParam_public(), //TBD Test these
		 EditWeldParam_public(),
		 VFTlistbox_public(),
		 MatProperties_public(),
		 EditMatProp_public(),
		 wpCreate_public(),wpCreate1_public(),wpCreate2_public(),
		 wpEdit_public(),wpEdit2_public(),
		 WeldPassEditSeqn1_public(),WeldPassEditSeqn2_public(),WeldPassEditSeqn3_public(),
		 exportCTSP2_public(),exportCTSP3_public(),exportWARP4_public(),
		 exportWARP3D_public(),exportWARP3D1a_public(),exportWARP3D5_public();
	void Form28_public(),Form28Close_public(),
		 tshiftCTSP_public(),tshiftCTSP3_public(),tshiftCTSP1_public();
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
