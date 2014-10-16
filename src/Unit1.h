// Note: void __fastcall ResultGo(TObject *Sender) "commented out" in Unit1 & missing from below
// This occurred because it shares Timer2 with PanGo1Execute()
//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>

#include <strsafe.h>
#include <Buttons.hpp>

#include <dir.h>
////////////////////////////////////
#include <dos.h> //Used by sleep(seconds)
////////////////////////////////////
//---------------------------------------------------------------------------
//#define	MXNPELS      8
#define	MXNPELS      20
#define	NDF         3
#define	MAXPOINTS    50      // #nodes in Polygon
////#define	MAX_GID    63
////#define	MAX_GID    31        // Determined by GIDselector coding (long 2**MAX_GID)
//#define	MAX_GID    16*16-1        // Determined by GIDselector coding (long 2**MAX_GID) & off-limits top MAX_GID value
////#define	MAX_GID    13-1        // Determined by GIDselector coding (long 2**MAX_GID) & off-limits top MAX_GID value
#define	MAX_GID   4096
#define	MAX_RESCOL       15
#define	MAX_LABEL       16
#define	MAX_LABELCHAR       14
#define	MAX_SRESULTSTEP  10
#define	MAX_SECTIONSTEP  15
#define	LONG_INT  2147483647
#define MPROP         19
#define	MAX_NSECTION  3
#define	TIMER_INTV  4000/8
//#define	TIMER4_INTV  1048576/10
#define	TIMER4_INTV  1000000
#define ANNOT_CHAR 16
//#define MAX_WELDGROUPS 25
//#define MAX_WELDPASSES 25

// Start from VFTgen
#define	MAX_POLYPOINTS  64
//#define	MAX_WELDGROUPS  25
#define	MAX_WELDGROUPS  500
#define	MAX_WELDPASSES  16*16-2
#define	MAX_MATPROPSET  10
#define	MAX_WELDPARSET  10
//End from VFTgen

#define  ZOOMTIMDELAY   1500  //Cycles in zoom rectangle delay loop
#define  TIMERZOOM_MILSEC    100  //Milliseconds for zoom timer
#define  TIMERROTA_MILSEC    550  //Milliseconds for continuous rota timer
// GIF
#define GIFHSIZE 5003
//#define GIFHSIZE 8199
#define GIF_BITS 12
//
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TActionList *ActionList1;
        TAction *CAPgeomOpen;
        TAction *FileClose;
        TAction *Exit;
        TPaintBox *PaintBox1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Close1;
        TMenuItem *Exit1;
        TRichEdit *RichEdit1;
        TOpenDialog *OpenDialog1;
        TAction *ViewPaint;
        TMenuItem *View1;
        TAction *CAPformatOpen;
        TMenuItem *Post1;
        TOpenDialog *OpenDialog2;
        TAction *Legend_auto;
        TAction *Legend_custom;
        TMenuItem *Legend;
        TMenuItem *Legendauto1;
        TMenuItem *Legendcustom1;
        TAction *Rotate;
        TMenuItem *Rotate1;
        TAction *ZoomIn;
        TAction *ZoomOut;
        TAction *ZoomReset;
        TMenuItem *Zoom;
		TMenuItem *ZoomIn1;
        TMenuItem *ZoomOut1;
        TMenuItem *ZoomReset1;
        TMenuItem *Analysis1;
        TAction *FDstress_analysis;
        TAction *CAPthermal_analysis;
        TMenuItem *Node1;
        TMenuItem *Query1;
        TAction *QueryNode;
        TAction *CrackGrowth;
        TTimer *Timer1;
        TAction *ShowCracks1;
        TAction *QueryElement;
        TMenuItem *QueryElement1;
        TAction *ReadViewFile;
        TAction *WriteViewFile;
        TAction *WriteModel;
        TAction *WriteResult;
        TAction *AnimResult;
        TTimer *Timer2;
        TAction *AnimMultiResult;
        TTimer *Timer3;
        TAction *SuperElOpen;
        TAction *FDmodal_analysis;
        TAction *Cart_frame;
        TAction *Cyl_frame;
        TAction *Spher_frame;
        TMenuItem *CoordFrame1;
        TMenuItem *Cartesian1;
        TMenuItem *Spherframe1;
        TAction *Section_X;
        TAction *Section_Y;
        TAction *Section_Z;
        TTimer *Timer4;
        TMenuItem *Defundeformed1;
        TMenuItem *Undeformedmesh1;
        TMenuItem *Deformedshape1;
        TMenuItem *Defshapeundefmesh1;
        TAction *Undeform_shape;
        TAction *Deform_shape;
        TAction *Def_undef_shape;
        TAction *Section_wedge;
        TMenuItem *Animate1;
        TMenuItem *AnimateMultiResult3;
        TAction *ApperGID;
        TAction *ApperMetal;
        TAction *ApperShade;
        TAction *ContourAll;
        TAction *ContourSection;
        TMenuItem *Contour1;
        TMenuItem *All1;
        TMenuItem *Section2;
        TOpenDialog *OpenDialog3;
        TAction *ShowTiny;
        TAction *ShowElliptic;
        TAction *Cyl_Xframe;
        TAction *Cyl_Yframe;
        TAction *Cyl_Zframe;
        TAction *Cyl_Gframe;
		TMenuItem *Cylindframe1;
        TMenuItem *CylXaxis1;
        TMenuItem *CylYaxis1;
        TMenuItem *CylZaxis1;
        TMenuItem *CylGaxis1;
//        TAction *Result_scale;
        TAction *Result_scale;
        TAction *NEUgeomOpen;
        TAction *NASgeomOpen;
        TAction *Section_3pt;
        TAction *Section_1pt2rot;
        TAction *QueryStressDir;
        TAction *SingleEllipticCrack;
        TAction *NodalDistance;
        TMenuItem *NodalDistance1;
        TAction *FEstats;
        TMenuItem *Analyze1;
        TAction *LEAnalyze;
        TOpenDialog *OpenDialog4;
        TAction *UNVgeomOpen;
        TAction *EPAnalyze;
        TAction *GIFsnapshot;
        TAction *ShowFEMesh;
        TAction *NoShowFEMesh;
        TAction *ThermDynAnalyze;
        TAction *AnimMultiResGIF;
        TMenuItem *AnimMultiResult2;
        TMenuItem *AnimMultiResGIF1;
		TAction *AnimResSnap;
        TAction *EigenAnalyze;
        TAction *CreepAnalyze;
        TAction *ShowEdge;
        TAction *HideEdge1;
        TAction *LEFMFatAnalyze;
//		TAction *Result_LinePlot;
		TAction *Result_LinePlot;
        TAction *RotateFreehand;
        TAction *LEFMFractAnalyze;
        TAction *FDstress_strain_analysis;
        TAction *LEAnalyDirectSol;
        TAction *MeshCharacteristics;
        TAction *GIDselector;
        TMenuItem *GIDselector1;
        TAction *RotateFixed;
        TAction *InitialValueProblem;
        TAction *LEAnalyze_T;
        TAction *WriteUNVmodelGID;
        TAction *LEAnalyze_T_MultiS;
        TAction *Generic_analysis;
        TAction *WriteResultbyCol;
        TAction *WriteUNVmodelAllGID;
        TAction *QueryCoordinate;
        TAction *LEAnalyzeD_T;
        TAction *EPAnalyzeD_T;
//		TAction *ResultVolAverage;
		TAction *ResultVolAverage;
        TAction *Annotate;
        TMenuItem *Annotate1;
		TAction *ClearAnnotate;
        TMenuItem *Annotate2;
        TMenuItem *ClearAnnotate1;
        TAction *AnnotLine;
        TMenuItem *AnnotLine1;
        TAction *ElectStatAnalyze;
        TAction *CreepAnalysisFile;
        TAction *RelaxAnalysisFile;
        TAction *RelaxAnalyze;
        TAction *DynamicTimeInteg;
	TAction *MSHgeomOpen;
	TAction *VFTweld_analysis;
	TMenuItem *VFTweldanalysis1;
	TAction *Import_VED;
	TMenuItem *ImportVED1;
	TMenuItem *ImportSolidMesh1;
	TMenuItem *FromAbaqusInputinp1;
	TMenuItem *FromSIMULIAAbaqabq1;
	TAction *MSHgeomVEDopen;
	TAction *ImportAbaABQ;
	TAction *ImportAbaInp;
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
	TImage *Image1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Weld1;
	TMenuItem *WeldParameters1;
	TMenuItem *MaterialProperties1;
	TMenuItem *CreateNewFullLengthWeldPass1;
	TMenuItem *CreateNewPartialLengthWeldPass1;
	TMenuItem *WeldPassEditSequencing1;
	TAction *WeldParam;
	TAction *MatProperties;
	TAction *WeldPassEditingandSequencing1;
	TAction *CreateNewWeldPass;
	TAction *CreateNewFullLenghtWeldPass;
	TAction *CreateNewPartialLengthWeldPass;
	TMenuItem *Export;
	TMenuItem *exportCTSP1;
	TMenuItem *exportABAQUS1;
	TAction *exportCTSP;
	TAction *exportABAQUS;
	TMenuItem *HideElemByPolygon;
	TAction *MaskElem0;
	TMenuItem *BackGroundColor;
	TMenuItem *BackgroundColor0;
	TMenuItem *BackgroundColor1;
	TMenuItem *BackgroundColor2;
	TMenuItem *BackgroundColor3;
	TMenuItem *BackgroundColor4;
	TMenuItem *BackgroundColor5;
	TAction *BackGroundColorF0;
	TAction *BackGroundColorF1;
	TAction *BackGroundColorF2;
	TAction *BackGroundColorF3;
	TAction *BackGroundColorF4;
	TAction *BackGroundColorF5;
	TMenuItem *XYView1;
	TMenuItem *YZView1;
	TMenuItem *ZXView1;
	TAction *XY_rot;
	TAction *YZ_rot;
	TAction *ZX_rot;
	TAction *PanGo1;
	TAction *Save;
	TMenuItem *SaveAs1;
	TAction *SaveAs2;
	TAction *Restore;
	TMenuItem *Restore1;
	TAction *ImportVFTr;
	TAction *EditWeldPass;
	TMenuItem *EditWeldPass1;
	TAction *DeleteWeldPass;
	TMenuItem *DeleteWeldPass1;
	TBitBtn *BitBtn10;
	TMenuItem *GIFsnap1;
	TMenuItem *FEstats2;
	TMenuItem *Help1;
	TMenuItem *Unfinished1;
	TMenuItem *XY_rot2;
	TAction *XY_rotm;
	TMenuItem *YZ_rotm;
	TAction *YZ_rotm1;
	TMenuItem *ZX_rot1;
	TAction *ZX_rotm;
	TAction *EditWeldPass3;
	TMenuItem *WeldParam1;
	TAction *EditWeldParam;
	TMenuItem *EditWeldParam1;
	TAction *DeleteWeldParam;
	TMenuItem *DeleteWeldParam1;
	TMenuItem *MatProperties1;
	TAction *EditMatProp;
	TAction *DeleteMatProp;
	TMenuItem *EditMatProp1;
	TMenuItem *DeleteMatProp1;
	TAction *ShowWG_red;
	TAction *ShowWG_polychrome;
	TMenuItem *ShowWeldGroup1;
	TMenuItem *ShowWGred1;
	TMenuItem *ShowWGpolychrome1;
	TAction *CreateFullLengthFullSection;
	TAction *CreatPartLengthFullSection;
	TMenuItem *CreateFullLengthFullSection2;
	TMenuItem *CreateNewPartialLengthWeldPass2;
	TMenuItem *CreatPartLengthFullSection2;
	TMenuItem *Showweldpassoutline1;
	TMenuItem *N11;
	TMenuItem *N21;
	TAction *ShowWP_last;
	TAction *ShowWP_hide;
	TMenuItem *ShowWPselect1;
	TAction *ShowWP_select;
	TAction *ShowWPstartelem_list;
	TMenuItem *ShowWPstartelem1;
	TMenuItem *ShowWPstartelemlist1;
	TAction *ShowWPstartelem_Hide;
	TMenuItem *ShowWPstartelemHide1;
	TMenuItem *ImportVFTr2;
	TAction *VFT_AutoSave1Go;
	TMenuItem *AutoSave1;
	TMenuItem *On1;
	TMenuItem *Off1;
	TAction *AutoSaveOn;
	TAction *AutoSaveOff;
	TMenuItem *MSHgeomVEDopen2;
	TMenuItem *Normalsconvention1;
	TMenuItem *Normal_direction;
	TMenuItem *Complem_direction;
	TAction *Normal_direction1;
	TAction *Complem_direction1;
	TAction *NodalCoincidence;
	TMenuItem *NodalCoincidence1;
	TMenuItem *ChangeAll1;
	TMenuItem *WeldParameters2;
	TMenuItem *MaterialProperties2;
	TMenuItem *WeldPassData1;
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
	TMenuItem *ChAllWParamcurr1;
	TMenuItem *ChAllWParamvolt1;
	TMenuItem *ChAllWParameff1;
	TMenuItem *ChAllWParamspeed1;
	TMenuItem *ChAllMatPropThCond1;
	TMenuItem *ChAllMatPropSpHeat1;
	TMenuItem *ChAllMatPropDens1;
	TMenuItem *Anneal1;
	TMenuItem *ChAllMatPropVED1;
	TMenuItem *ChAllMatPropAnneal1;
	TMenuItem *ChAllMatPropTi1;
	TMenuItem *ChAllMatPropTa1;
	TMenuItem *ChAllMatPropTm1;
	TMenuItem *CTSPinputs1;
	TMenuItem *WeldParameters3;
	TMenuItem *ChAllWPasstimeInterv1;
	TMenuItem *ChAllWPassstepInterval1;
	TMenuItem *ChAllWPassmaxiHeatStep1;
	TMenuItem *ChAllWPassminiHeatStep1;
	TMenuItem *ChAllWPassthk11;
	TMenuItem *ChAllWPassthk21;
	TMenuItem *ChAllWPasstroom1;
	TMenuItem *ChAllWPasstmelt1;
	TMenuItem *ChAllWPasstcutl1;
	TMenuItem *ChAllWPasstcuth1;
	TMenuItem *Save1;
	TAction *ChangeTimer4_Interv;
	TMenuItem *ChangeTimer4Interv1;
	TAction *mergeCTSPmcmNoDel;
	TAction *mergeCTSPmcmDel;
	TMenuItem *MergeCTSPmulticore1;
	TMenuItem *mergeCTSPmcmNoDel1;
	TMenuItem *mergeCTSPmcmDel1;
	TAction *AboutVFT;
	TMenuItem *ShowWGonly1;
	TMenuItem *WGselect1;
	TMenuItem *WGall1;
	TAction *WG_selector;
	TAction *WGselAll;
	TAction *ShowWPout_All;
	TMenuItem *ShowWPoutAll1;
	TMenuItem *ReverseWeld1;
	TAction *RevEveryWP;
	TAction *Every2ndWP;
	TMenuItem *RevEveryWP1;
	TMenuItem *Every2ndWP1;
	TAction *EveryOddWP;
	TMenuItem *EveryOddWP1;
	TAction *ResetWPdir;
	TMenuItem *ResetWPdir1;
	TBitBtn *BitBtn11;
	TAction *exportWARP3D;
	TMenuItem *exportWARP3D1;
	TMenuItem *MSHgeomOpen1;
	TAction *TimeshiftCTSP0;
	TMenuItem *imeshiftCTSP01;
	TAction *TempDeltCTSP0;
	TMenuItem *empDeltCTSPtempout1;
	TMenuItem *CTSPinterpolate1;
	TAction *CTSPinterpSolid;
	TAction *CTSPinterpShell;
	TMenuItem *CTSPinterpSolid1;
	TMenuItem *CTSPinterpShell1;
	TAction *WARP3Dconvenience;
	TMenuItem *TempDeltWARP3D;
	TMenuItem *WARP3Dconvenience1;
	TMenuItem *imeshiftCTSP02;
	TAction *TimeshiftVED0;
	TMenuItem *imeshiftVED01;
	TAction *TimeshiftCTSPandVED0;
	TMenuItem *imeshiftCTSPandVED01;
	TMenuItem *BCs1;
	TMenuItem *Addbynodeclick1;
	TMenuItem *Removebyclick1;
	TMenuItem *Ux01;
	TAction *BCsNclick0;
	TAction *BCsNclick1;
	TAction *BCsNclick2;
	TAction *BCsNclick3;
	TAction *BCsNclick4;
	TAction *BCsNclick5;
	TAction *BCsNclick6;
	TAction *BCsNclick7;
	TMenuItem *BCsNclick11;
	TMenuItem *BCsNclick21;
	TMenuItem *BCsNclick31;
	TMenuItem *BCsNclick41;
	TMenuItem *BCsNclick51;
	TMenuItem *BCsNclick61;
	TAction *ElFacetArea;
	TMenuItem *ElFacetArea1;
	TAction *MergeTimeshiftCTSP0;
	TAction *MergeTimeshiftVED0;
	TAction *MergeTimeshiftCTSPandVED0;
	TMenuItem *MergeTimeshifted1;
	TMenuItem *MergeTimeshiftCTSP01;
	TMenuItem *MergeTimeshiftVED01;
	TMenuItem *MergeTimeshiftCTSPandVED01;
	TMenuItem *CreateNewFullLenghtWeldPass2;
	TAction *ImportVFTa;
	TMenuItem *ImportVFTa1;
	TAction *Action1;
	TAction *SaveAs3;
	TMenuItem *SaveAs31;
		void __fastcall ExitExecute(TObject *Sender);
//        void __fastcall ExitExecute();
		void __fastcall FileCloseExecute(TObject *Sender);
//        void __fastcall CAPgeomOpenExecute(TObject *Sender);
        void __fastcall Form1Paint(TObject *Sender);
//        void __fastcall ViewPaintExecute(TObject *Sender);
        void __fastcall Legend_autoExecute(TObject *Sender);
        void __fastcall Legend_customExecute(TObject *Sender);
        void __fastcall RotateExecute(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,TShiftState Shift, int X, int Y);
		void __fastcall FormMouseWheel(TObject *Sender,TShiftState Shift,int WheelDelta,TPoint &MousePos,bool &Handled);
		void __fastcall ZoomOutExecute(TObject *Sender);
        void __fastcall ZoomInExecute(TObject *Sender);
        void __fastcall ZoomResetExecute(TObject *Sender);
//        void __fastcall FDstress_analysisExecute(TObject *Sender);
//        void __fastcall CAPthermal_analysisExecute(TObject *Sender);
        void __fastcall QueryNodeExecute(TObject *Sender);
//        void __fastcall CrackGrowthExecute(TObject *Sender);
        void __fastcall GitGo(TObject *Sender);
//        void __fastcall ShowCracks1Execute(TObject *Sender);
//        void __fastcall Misc0Execute(TObject *Sender);
        void __fastcall QueryElementExecute(TObject *Sender);
//        void __fastcall ReadViewFileExecute(TObject *Sender);
//		void __fastcall WriteViewFileExecute(TObject *Sender);
//        void __fastcall WriteModelExecute(TObject *Sender);
//        void __fastcall WriteResultExecute(TObject *Sender);
        void __fastcall AnimResultExecute(TObject *Sender);
//        void __fastcall PanGo(TObject *Sender);
        void __fastcall AnimMultiResultExecute(TObject *Sender);
		void __fastcall MultiResultGo(TObject *Sender);
//		void __fastcall SuperElOpenExecute(TObject *Sender);
//        void __fastcall FDmodal_analysisExecute(TObject *Sender);
        void __fastcall Cart_frameExecute(TObject *Sender);
        void __fastcall Spher_frameExecute(TObject *Sender);
//        void __fastcall Section_XExecute(TObject *Sender);
//		void __fastcall Section_YExecute(TObject *Sender);
//        void __fastcall Section_ZExecute(TObject *Sender);
        void __fastcall Section_XExecute(TObject *Sender);
		void __fastcall Section_YExecute(TObject *Sender);
        void __fastcall Section_ZExecute(TObject *Sender);
		void __fastcall SectheaGo(TObject *Sender);
        void __fastcall Undeform_shapeExecute(TObject *Sender);
        void __fastcall Deform_shapeExecute(TObject *Sender);
        void __fastcall Def_undef_shapeExecute(TObject *Sender);
//		void __fastcall Section_wedgeExecute(TObject *Sender);
		void __fastcall Section_wedgeExecute(TObject *Sender);
		void __fastcall ApperGIDExecute(TObject *Sender);
        void __fastcall ApperMetalExecute(TObject *Sender);
        void __fastcall ApperShadeExecute(TObject *Sender);
        void __fastcall ContourAllExecute(TObject *Sender);
        void __fastcall ContourSectionExecute(TObject *Sender);
//        void __fastcall ShowTinyExecute(TObject *Sender);
//        void __fastcall ShowEllipticExecute(TObject *Sender);
        void __fastcall Cyl_XframeExecute(TObject *Sender);
        void __fastcall Cyl_YframeExecute(TObject *Sender);
        void __fastcall Cyl_ZframeExecute(TObject *Sender);
        void __fastcall Cyl_GframeExecute(TObject *Sender);
        void __fastcall Result_scaleExecute(TObject *Sender);
////        void __fastcall NEUgeomOpenExecute(TObject *Sender);
////        void __fastcall NASgeomOpenExecute(TObject *Sender);
//		void __fastcall Section_3ptExecute(TObject *Sender);
//		void __fastcall Section_1pt2rotExecute(TObject *Sender);
		void __fastcall Section_3ptExecute(TObject *Sender);
		void __fastcall Section_1pt2rotExecute(TObject *Sender);
		void __fastcall QueryStressDirExecute(TObject *Sender);
//        void __fastcall SingleEllipticCrackExecute(TObject *Sender);
        void __fastcall NodalDistanceExecute(TObject *Sender);
        void __fastcall FEstatsExecute(TObject *Sender);
//        void __fastcall UNVgeomOpenExecute(TObject *Sender);
//        void __fastcall EPAnalyzeExecute(TObject *Sender);
        void __fastcall GIFsnapshotExecute(TObject *Sender);
		void __fastcall ShowFEMeshExecute(TObject *Sender);
        void __fastcall NoShowFEMeshExecute(TObject *Sender);
//        void __fastcall ThermDynAnalyzeExecute(TObject *Sender);
        void __fastcall AnimMultiResGIFExecute(TObject *Sender);
		void __fastcall AnimResSnapExecute(TObject *Sender);
//        void __fastcall EigenAnalyzeExecute(TObject *Sender);
//        void __fastcall CreepAnalyzeExecute(TObject *Sender);
		void __fastcall ShowEdgeExecute(TObject *Sender);
		void __fastcall HideEdge1Execute(TObject *Sender);
//		void __fastcall LEFMFatAnalyzeExecute(TObject *Sender);
		void __fastcall Result_LinePlotExecute(TObject *Sender);
		void __fastcall RotateFreehandExecute(TObject *Sender);
//		void __fastcall LEFMFractAnalyzeExecute(TObject *Sender);
//		void __fastcall FDstress_strain_analysisExecute(TObject *Sender);
		void __fastcall MeshCharacteristicsExecute(TObject *Sender);
		void __fastcall GIDselectorExecute(TObject *Sender);
		void __fastcall RotateFixedExecute(TObject *Sender);
//		void __fastcall InitialValueProblemExecute(TObject *Sender);
//		void __fastcall WriteUNVmodelGIDExecute(TObject *Sender);
//		void __fastcall Generic_analysisExecute(TObject *Sender);
//		void __fastcall WriteResultbyColExecute(TObject *Sender);
//		void __fastcall WriteUNVmodelAllGIDExecute(TObject *Sender);
		void __fastcall QueryCoordinateExecute(TObject *Sender);
//        void __fastcall EPAnalyzeD_TExecute(TObject *Sender);
		void __fastcall ResultVolAverageExecute(TObject *Sender);
		void __fastcall AnnotateExecute(TObject *Sender);
		void __fastcall ClearAnnotateExecute(TObject *Sender);
		void __fastcall AnnotLineExecute(TObject *Sender);
//		void __fastcall ElectStatAnalyzeExecute(TObject *Sender);
//		void __fastcall CreepAnalysisFileExecute(TObject *Sender);
//		void __fastcall RelaxAnalysisFileExecute(TObject *Sender);
//		void __fastcall RelaxAnalyzeExecute(TObject *Sender);
//        void __fastcall DynamicTimeIntegExecute(TObject *Sender);
//        void __fastcall JunkExecute(TObject *Sender);
	void __fastcall MSHgeomOpenExecute(TObject *Sender);
	void __fastcall VFTweld_analysisExecute(TObject *Sender);
	void __fastcall Import_VEDExecute(TObject *Sender);
	void __fastcall MSHgeomVEDopenExecute(TObject *Sender);
	void __fastcall ImportAbaABQExecute(TObject *Sender);
	void __fastcall ImportAbaInpExecute(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall BitBtn7Click(TObject *Sender);
	void __fastcall BitBtn8Click(TObject *Sender);
	void __fastcall BitBtn9Click(TObject *Sender);
	void __fastcall WeldParamExecute(TObject *Sender);
	void __fastcall MatPropertiesExecute(TObject *Sender);
	void __fastcall WeldPassEditingandSequencing1Execute(TObject *Sender);
	void __fastcall CreateNewFullLenghtWeldPassExecute(TObject *Sender);
	void __fastcall CreateNewPartialLengthWeldPassExecute(TObject *Sender);
	void __fastcall exportCTSPExecute(TObject *Sender);
	void __fastcall exportABAQUSExecute(TObject *Sender);
	void __fastcall MaskElem0Execute(TObject *Sender);
	void __fastcall BackGroundColorF0Execute(TObject *Sender);
	void __fastcall BackGroundColorF1Execute(TObject *Sender);
	void __fastcall BackGroundColorF2Execute(TObject *Sender);
	void __fastcall BackGroundColorF3Execute(TObject *Sender);
	void __fastcall BackGroundColorF4Execute(TObject *Sender);
	void __fastcall BackGroundColorF5Execute(TObject *Sender);
	void __fastcall XY_rotExecute(TObject *Sender);
	void __fastcall YZ_rotExecute(TObject *Sender);
	void __fastcall ZX_rotExecute(TObject *Sender);
	void __fastcall PanGo1Execute(TObject *Sender);
	void __fastcall SaveExecute(TObject *Sender);
	void __fastcall SaveAs2Execute(TObject *Sender);
	void __fastcall RestoreExecute(TObject *Sender);
	void __fastcall ImportVFTrExecute(TObject *Sender);
	void __fastcall EditWeldPassExecute(TObject *Sender);
	void __fastcall DeleteWeldPassExecute(TObject *Sender);
	void __fastcall BitBtn10Click(TObject *Sender);
	void __fastcall XY_rotmExecute(TObject *Sender);
	void __fastcall YZ_rotm1Execute(TObject *Sender);
	void __fastcall ZX_rotmExecute(TObject *Sender);
//	void __fastcall EditWeldPass3Execute(TObject *Sender);
	void __fastcall EditWeldParamExecute(TObject *Sender);
	void __fastcall DeleteWeldParamExecute(TObject *Sender);
	void __fastcall EditMatPropExecute(TObject *Sender);
	void __fastcall DeleteMatPropExecute(TObject *Sender);
	void __fastcall ShowWG_redExecute(TObject *Sender);
	void __fastcall ShowWG_polychromeExecute(TObject *Sender);
	void __fastcall CreateFullLengthFullSectionExecute(TObject *Sender);
	void __fastcall CreatPartLengthFullSectionExecute(TObject *Sender);
	void __fastcall ShowWP_lastExecute(TObject *Sender);
	void __fastcall ShowWP_hideExecute(TObject *Sender);
	void __fastcall ShowWP_selectExecute(TObject *Sender);
	void __fastcall ShowWPstartelem_listExecute(TObject *Sender);
	void __fastcall ShowWPstartelem_HideExecute(TObject *Sender);
	void __fastcall VFT_AutoSave1GoExecute(TObject *Sender);
	void __fastcall AutoSaveOnExecute(TObject *Sender);
	void __fastcall AutoSaveOffExecute(TObject *Sender);
	void __fastcall Normal_direction1Execute(TObject *Sender);
	void __fastcall Complem_direction1Execute(TObject *Sender);
	void __fastcall NodalCoincidenceExecute(TObject *Sender);
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
//	void __fastcall SaveAs_VFTaExecute(TObject *Sender);
//	void __fastcall JunkExecute(TObject *Sender);
	void __fastcall ChangeTimer4_IntervExecute(TObject *Sender);
	void __fastcall mergeCTSPmcmNoDelExecute(TObject *Sender);
	void __fastcall mergeCTSPmcmDelExecute(TObject *Sender);
	void __fastcall AboutVFTExecute(TObject *Sender);
	void __fastcall WG_selectorExecute(TObject *Sender);
	void __fastcall WGselAllExecute(TObject *Sender);
	void __fastcall ShowWPout_AllExecute(TObject *Sender);
	void __fastcall RevEveryWPExecute(TObject *Sender);
	void __fastcall Every2ndWPExecute(TObject *Sender);
	void __fastcall EveryOddWPExecute(TObject *Sender);
	void __fastcall ResetWPdirExecute(TObject *Sender);
	void __fastcall BitBtn11Click(TObject *Sender);
	void __fastcall exportWARP3DExecute(TObject *Sender);
	void __fastcall TimeshiftCTSP0Execute(TObject *Sender);
	void __fastcall TempDeltCTSP0Execute(TObject *Sender);
//	void __fastcall CTSPinterpolateExecute(TObject *Sender);
	void __fastcall CTSPinterpSolidExecute(TObject *Sender);
	void __fastcall CTSPinterpShellExecute(TObject *Sender);
	void __fastcall WARP3DconvenienceExecute(TObject *Sender);
	void __fastcall TimeshiftVED0Execute(TObject *Sender);
	void __fastcall TimeshiftCTSPandVED0Execute(TObject *Sender);
	void __fastcall BCsNclick0Execute(TObject *Sender);
	void __fastcall BCsNclick1Execute(TObject *Sender);
	void __fastcall BCsNclick2Execute(TObject *Sender);
	void __fastcall BCsNclick3Execute(TObject *Sender);
	void __fastcall BCsNclick4Execute(TObject *Sender);
	void __fastcall BCsNclick5Execute(TObject *Sender);
	void __fastcall BCsNclick6Execute(TObject *Sender);
	void __fastcall BCsNclick7Execute(TObject *Sender);
	void __fastcall ElFacetAreaExecute(TObject *Sender);
	void __fastcall MergeTimeshiftCTSP0Execute(TObject *Sender);
	void __fastcall MergeTimeshiftVED0Execute(TObject *Sender);
	void __fastcall MergeTimeshiftCTSPandVED0Execute(TObject *Sender);
	void __fastcall ImportVFTaExecute(TObject *Sender);
	void __fastcall Action1Execute(TObject *Sender);
	void __fastcall SaveAs3Execute(TObject *Sender);
private:	// User declarations
// struct FEdata {long npoin,nelt,nvfix,nedge,pload,mat,nblod,matsteps,ncoorf,
//                  *nop1,*matno,*nlp,*nofix,*nldel,*lodpt,*matyp,*ebody,*nrfix//        ,*lbce,*lbcn
//                ;
//                float *c1,*res1,*beta,*presc,*press,*pointld,*prop,*body,*cangl;
//               } base;
// struct FEdata1{long npoin,nelt,nvfix,nedge,nfsect,*nop1,*matno,*fsect,*nlp//	  ,*nofix,*nldel,*lbce,*lbcn
//                ;
//                float *c1,*bc1,*result,*res1,*beta;
//			   } indat;
 struct FEdataD {int *arELEM;
				 long npoin,nelt,nvfix,nedge,pload,mat,nblod,matsteps,ncoorf,allGrp,ELSETelsum,GIDcol,
				  *nop1,*matno,*nlp,*nofix,*ifpre,*nrfix,*nldel,*lodpt,*matyp,*ebody,*el_map,*node_map//        ,*lbce,*lbcn
				  ,*orig_matno,*arrELSET
//,*orig_arrELSET,*trackELSET
				 ;
				 float *presc,*press,*pointld,*prop,*body,*cangl, *c1,*res1,*beta;
///////////////// EFP 2/25/2012
char *comboCWD;
/////////////////
				 String *groupsname; //EFP 3/15/2011
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
/*
				// Start temporary disconnect VFTmisc coding
 struct WeldPassProp{int *seqNum,*boolFlags,*type,*shape,*stepInterval,*hp,*nsegs,*iselect
//	 ,*edgeFlag,*circFlag,*merge
	 ;
// GIDwp: 1st col= current iside, remainder=GID
					 long nWeldPassEl,count_curr_sttEl,
						  GIDwp,nWeldPass,nWeldGroup,*eles,*sttEles,*stpEle,*firstEle,*nextEle,*snorm1,*snorm2,
						   *circEles,*edgeEles,*edgeNodes,*sttEleNodes,*hlightel,*n_curr_sttEl,*wpTimes
						   ,VEDsteps,VFTresSteps        // Cats ass programming
						   ;
					 float *curr,*volt,*eff,*speed,*troom,*tmelt,*tcutl,*tcuth,*timeInterval,
						   *maxiHeatStep,*miniHeatStep,*thk1,*thk2,*source;
					 double *lstart,*lend,*arrows;
// boolFlags[]: showFlag=1st digit,circFlag=2nd,edgeFlag=3rd,merge=4th,girthFlag=5th
					} wp;
				// End temporary disconnect VFTmisc coding
*/
////////////////////////////////
 struct WeldPassResults{long *wIndex,*wActiven;float *wActiveR
 ,*timeVFTarr,*timeVEDarr
;
					   } wpRes;
////////////////////////////////
// Start from VFTgen
 struct WeldPassProp{int CreateWPassMode,PRECORD,EDIT_SEL,PAINT_SEL,avis,pending,
 temp_seqNum,temp_boolFlags,temp_type,temp_shape,temp_stepInterval,
// temp_hp,temp_nsegs,temp_iselect,temp_mcr,temp_source,temp_util_arr,
 temp_hp,temp_nsegs,temp_iselect,temp_mcr,temp_source,
 *seqNum,*boolFlags,*type,*shape,*stepInterval,*hp,*nsegs,*iselect,*mcr
// ,*source,*util_arr
 ,*source
////	 ,*edgeFlag,*circFlag,*merge
	 ;
 long temp_util_arr,*util_arr;
// CreateWPassMode=0 -> edit; =1 -> Create
// GIDwp: 1st col= current iside, remainder=GID
					 long nWeldPassEl,count_curr_sttEl,GIDwp,
						  nWeldPass,nWeldGroup,memWGa,highest,elStart,
						  temp_stpEle,temp_n_curr_sttEl,temp_prevGID,Record,edit_sel,paint_sel,
						  temp_reset,*prevGID,
						  *eles,*sttEles,*stpEle,*firstEle,*nextEle,*snorm1,*snorm2,
						   *circEles,*edgeEles,*edgeNodes,*sttEleNodes,*hlightel,*n_curr_sttEl,*reset
						   ,VEDsteps,VFTresSteps        // Cats ass programming
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
 int iplotflag,iPaintyesno,iCircleplot,iCullyesno,nGIDmax,fl_feres,nGID,nMultFile,GIFcount,stateVFT,VFTon,
	 CAPres_count,GIFsw,ANNOTcount,AnnotIndex[3*ANNOT_CHAR],ANLINcount,AnLINIndex[4*ANNOT_CHAR],
	 iBackGroundColor,iPersistVFT,icTimer5,iplotSWT,iplotType,flag_VFTrSave,
	 polycord[2*MAX_POLYPOINTS],polypts,Xpan0,Ypan0,Xpan1,Ypan1,panMouseM,
	 stateVFTrec,FD_LBrec;//AnnotIndex[3*ANNOT_CHAR] contains wordlength in col#3 (not used)
// char AnnotArr[(ANNOT_CHAR+2)*8];
// AnsiString AnnotArr[(ANNOT_CHAR+2)*8];
 AnsiString AnnotArr0,AnnotArr1,AnnotArr2,AnnotArr3,AnnotArr4,AnnotArr5,AnnotArr6,AnnotArr7,AnnotArr8,AnnotArr9;
 long nColRes,qndNodeNum1,MXNPEL,MXNPEI,IntervalT4;
//
 long new_nelt,new_npoin,new_nvfix,new_nplod,new_nedge,new_mat,new_ncoorf,new_nbodyf,old_npoin;
//
	 TRect zoomRect;BOOL zoomDrag;int FD_LButtonstatus;TMouseButton Button;
//		float rangle0[3],rangle[3],trans_op[3],prod_op[3][3],trans_zoom[3],prod_zoom[3],rot_op[3][3],*mutFacet,leg_rmax,leg_rmin,resscale,illumDirect[3];
		float rangle0[3],rangle[3],trans_op[3],prod_op[3][3],
		  trans_zoom[3],prod_zoom[3],rot_op[3][3],*mutFacet,leg_rmax,leg_rmin,
		  resscale,illumDirect[3],glGdiff,gl_indatMedian[3];
		int section_frame,isectsw;
        float sect_plane[3],sectdval[2*3],sectmx,sectmn,shapecombo,genaxispts[9];
        long *rbTemp,*arbFacet,nFacets,arGID[MAX_GID];
		int *edgFacet,*iswFace,cxWidth0,cyHeight0,nSize,nColor,lpPolyCount[MAXPOINTS];
        TPoint point,ptClick0,ptClick1,ptClick;int nColContour;float ColContour[13+1];
        void FDdynmem_manage(int,long,long,long,long,long,long,long,long,long,long,long,long,long);
		void FDtrans_results(long,float [],float [],long,int);
//        void FDtrans_frame_results(long,float [],float [],long,int,int,float);
        void FDbase_indat(int,float,int,long);
//		void parse_cdv(char [],float*);
//		void parse_cdr(char [],long,long,float []);
		void parse_cdm(char [],long,int*,int*,long [],float []);
int parse_cdmV(char [],long,int*,int*,long [],float []);
int parse_cdmVc(char [],long,int*,int*,long [],float []);
void parse_cdm3f(char [],long,int*,int*,long [],float []);
void parse_cdm3ff(char [],long,int*,int*,long [],float []);
//		void parse_cdmc(char [],long*,int);
//		void parse_cdmD(char [],long,int*,int*,long [],float []);
//		void parse_cdCAP(char [],long,int*,int*,long [],float []);
//        void parse_cdms(char [],long,int*,int*,long [],float [],int []);
		void parse_cdmQ(char [],long,int*,int*,long [],float [],long);
		void parse_cdmQb(char [],long,int*,int*,long [],float [],long);
		void parse_cdmQn(char [],long,int*,int*,long [],float []);
void parse_cdmQc(char [],long,int*,int*,long [],float []);
//		void parse_nas(int*,long*,long*,long*,long*,float*,float*,float*,float*,int,char [],long [],long*);
//        long parse_long(int,int,char []);float parse_real(int,int,char []);
        void FDpmargin(long,long,long [],long [],float [],long,float [],float [],float [][3]);void FDpscale(long,float [],long,float [],float [][3]);void FDmetal(long,long []);
		void FDwireplot(long,long [],long [],float []);
void FDpaintplot(int,long [],long [],float [],long,long []);
        void FDpaintplotnm(long [],long [],float [],long,long [],int []);void FDpaintplots(long [],long [],float [],long,long [],long,long []);
		void FDcontourplot(long [],long [],float [],float [],long,long [],long [],int []);
		void FDcontourplotC(long [],long [],float [],float [],long,long [],long [],int [],float [],int []);
		void FDcontourplotC1(long [],long [],float [],float [],long,long [],long [],int [],float [],int []);
		void FDprecontour(int *,int);
void FDpreGID(int *,int,long,long [],long []);
void FDcontourplotCa(long [],long [],float [],float [],long,long [],long [],int [],int *);
		void FDcontourplots(long [],long [],float [],float [],long,long [],long [],int [],long,long []);
        void FDshadeplot(long [],long [],float [],long,long []);void FDshadeplotnm(long [],long [],float [],long,long []);
	long FDcullfacet_mem(long,long [],long [],float [],long []);
		 void FDcullfacet(long,long [],long [],float [],long [],long []);
         long FDcullfacetc(long,long [],long [],float [],long [],long [],long []);
		 void FDorderfacet(long [],long [],float [],long,long [],long []);
         void FDsort_low(long *,float *,long,long *,long *,int *,long *,long *,int *);
         void FDsort_low1(long *,float *,long,long *,long *,int *,long);

//void sort_by_partition(long *,float *,long,long *,long *,long [],long [],long [],long [],float [],long,long,long,float,float,long *);
void sort_by_partition(long *,float *,long,long *,long *,long *,long *,long *,long *,float *,long,long,long,float,float,long *);
void sort_by_partition1(long *,float *,long,long *,long *,long [],long [],long [],long [],float [],long,long,long,float,float);
void sort_by_bubble(long *,float *,long *,long *,long *,long,long);
void sort_by_stride(long *,float *,long *,long *,long *,long,long);

         void FDcomp_nGID(long,long [],int*,long []);
//         void FDcomp_nGIDd(unsigned long,int,long []);
//         void FDelemfacets2(long,long,long [],long [],int,long []);void FDlegend();void FDculledge(long [],long [],float [],long,long [],int []);
         void FDelemfacets3(long,long,long [],long [],long []);
         void FDelemfacets3a(long,long,long [],long []);
		 void FDlegend();void FDculledge(long [],long [],float [],long,long [],int []);
       void STFISO20(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
       void STFISO15(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
       void STFISO10(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
       void STFISO8(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
       void STFISO6(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
       void STFISO4(int,float,float,float,float*,float [],float [],float [],float [],long [],float []);
void STFISO8_ncalc(long,long,float [],long [],float []);
		float val_interp(float,float,float);
        void FDsel_res(int,long,float [],float [],long,int);void FD_PSTR(int,long,long,float [],float [],long);void FD_PSTRN(long,float*,float [],float []);
        void FD_PSTRD(float [],float [],float*,float*,float*,float*,float,float);
        void FD_PSTRG(float [],float*,float*,float*);
        void FDrotate(int,long,float [],float [],float [],float [][3],float [][3],long,float []);
        void CmZoomIn2(long,float [],float [],float [][3],float [],float [],long,float []);
        void FDresetzoom(long,float [],float [],float [][3],long,float []);void FDrot_status(long,float [],float [][3],long,float []);
//        int CAP_Tzero_In(float [],long [],long);int CAP_Tzero_In1(float [],long [],long);void binCAPformatOpen(long *,long,long,int,long*);
        long FDnode_interrog1(int,int,long [],long [],float [],long,long []);
		void FDelem_interrog1(int,int,long*,long*,float*,float*,float*,long [],long [],float [],long,long []);
		void FDelem_interrogpt1(long,long,long,long,float,float,float,
		  float*,float*,float*,float*,float*,float*,float*,float*,long [],long [],float [],float []);
// New section routines
        void FDsect_base_indat_mem(int,float,float,float [],long*,long,long*,long,long,long,long [],long [],float [],float [],long,long*,long*,long*);
        void FDsect_base_indat(int,int,float,float,float [],long*,long,long*,long,long,float [],float [],float [],long,float [],long,long [],long [],long [],long*,long*,long*,float [],long [],long []);
        void FDsect_base_indatw_mem(int,float,float,float,float,long*,long,long*,long,long,long,long [],long [],float [],float [],long,long*,long*,long*);
        void FDsect_base_indatw(int,int,float,float,float,float,long*,long,long*,long,long,float [],float [],float [],long,float [],long,long [],long [],long [],long*,long*,long*,float [],long [],long []);
        void FDelem_subd8_mem(float,float,float,float,long*,long*,long*,long,int,float,long [],float [],float [],long);
		void FDelem_subd6_mem(float,float,float,float,long*,long*,long*,long,int,float,long [],float [],float [],long);
        void FDelem_subd4_mem(float,float,float,float,long*,long*,long*,long,int,float,long [],float [],float [],long);
        void FDelem_subd8(float,float,float,float,long*,long*,long*,long,int,int,float,int,long [],long [],float [],float [],long,long [],long [],float [],float [],long []);
        void FDelem_subd6(float,float,float,float,long*,long*,long*,long,int,int,float,int,long [],long [],float [],float [],long,long [],long [],float [],float [],long []);
        void FDelem_subd4(float,float,float,float,long*,long*,long*,long,int,int,float,int,long [],long [],float [],float [],long,long [],long [],float [],float [],long []);
        void FDnode_subd8(float,float,float,float,long,float [],int,float,long [],float [],float [],long);
        void FDnode_subd6(float,float,float,float,long,float [],int,float,long [],float [],float [],long);
        void FDnode_subd4(float,float,float,float,long,float [],int,float,long [],float [],float [],long);
        void FDnode_dir6(float,float,float,float,long,int,float,int *,long [],float [],float [],long);
        void FDnode_cir6(float,float,float,float,long,int,float,int *,long [],float []);
//
        void FDelem_subc8(float,float,float,float,long*,long*,long*,long,int,int,float,int,long,long [],long [],float [],float [],long,long,long []);
        void FDelem_subc6(float,float,float,float,long*,long*,long*,long,int,int,float,int,long,long [],long [],float [],float [],long,long,long []);
        void FDelem_subc4(float,float,float,float,long*,long*,long*,long,int,int,float,int,long,long [],long [],float [],float [],long,long,long []);
        void FDnode_subc8(float,float,float,float,long,float [],int,float,long [],float []);
        void FDnode_subc6(float,float,float,float,long,float [],int,float,long [],float []);
        void FDnode_subc4(float,float,float,float,long,float [],int,float,long [],float []);
//
int nSpecRes,iSpecRes;
long nDefects,iSpecCrack;float *bCrackCor,*bCrackAng,*CrackCor;int *CrackTyp;
//void FDplotdefects(long,float [],float []);
//void Crack_input(long*);
void FDrestore();
void sloan(long,long,long [],long [],long [],float*,float*,long*,long*,long*,long*,long*,int*);
void sloangraph(long,long,long,long,long [],long [],long [],long [],int*);
void sloandiamtr(long,long,long [],long [],long [],long [],long [],long [],long*,long*);
void rootls(long,long,long,long [],long [],long [],long [],long [],long*,long*);
void sloannumber(long,long,long*,long,long [],long [],long [],long [],long []);
void renuminp3g(long,long,long [],long [],long [],float [],float [],double [],long,long,long [],long [],int);
//void renuminp3gR(long,long,long [],long [],long [],float [],float [],double [],long,long,long [],long [],long,float [],int);
//void renuminp3gR(long,long,long [],long [],long [],float [],float [],double [],long,long [],long,float [],int);
//void renuminp3D1(long,long,long [],long [],long [],float [],double [],long,long,long [],long [],float[],long);
void cnsbdwth1(long,long,long [],long [],long [],long [],long*,long*,long*,long);
//int cnsbdwth2(long,long,long [],long [],long [],long [],long [],long*,long*,long*,long);
//void cnsbdwth3(long,long,long [],long [],long [],long*);
void locindx6e1_mem(long,long,long [],long [],long [],long*,long*,long [],long,long [],int*,long,long*,int,long,long,long*,long);
void deanloe(long,long,long [],long [],long [],long [],long*,long*,long,long [],long [],long [],long,long,long,long [],long*,int*,long,long);
void deanloy(long,long,long [],long [],long [],long [],long*,long*,long,long [],long [],long [],long,long,long,long [],int*,long,long [],long,long);
long rangu(long,long,long[],long[],long);void sortminmx(long,long [],float [],long []);
//void formkch7(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],long [],long,float []);
//void formkch7T(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],float [],float [],float,long,long [],long,float []);
void formkch7c(long,float [],long [],long [],float []);
//void formmch7(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],long [],long,float []);
//void formmch7a(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],float [],float [],long [],float []);
//void mformmch7(long,float [],long [],long [],float [],long [],float []);
//void ftrmkch7(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],float [],long [],float [],float);
//void ftrmkch7EM(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],float []);
//void formkch9(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],long [],long,float [],long [],long,long);
//void formkch9T(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],float [],float [],float,long,long [],long,float [],long [],long,long);
//void formmch9(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],long [],long [],long,float [],long [],long,long,long);
//void formmch15(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],double [],float [],float [],long [],long [],long,float [],long [],long,long,long);
//void ftrmkch9(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],long [],long [],float [],float [],float [],float [],long [],long [],long,long,float [],float);
//void formkch13(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],float [],long [],long,long [],long,float []);
//void formmch13(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],double [],float [],long [],long,long [],long,float [],float);
//void formkch13r(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],float [],long [],long,long [],long,float []);
//void formkch13T(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],float [],long [],long,float [],float [],float,long,long [],long,float []);
//void formkch14(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],float [],long [],long [],long,float [],long [],long);
//void formkch14T(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],long [],double [],double [],float [],long [],float [],float [],float,long,long [],long,float [],long [],long);
//void formmch12(long,long,float [],long [],long [],float [],long,long [],float [],long,long [],double [],float [],float [],long [],long,long [],long,float []);
//void estfcomp(long,float [],float*,float [],float);void estfcomp1(long,float [],float*,float);void estfcomp2(long,float [],float*,float);
//void formtem7(long,float [],long [],long [],float [],long [],float [],double [],float [],long,long [],long [],long,float [],float [],float [],int);
void formAVGtem7(long,long,long [],long [],float [],float [],long,float [],long *,float *);
//void nofix_rot(long,long,long,long,long [],long [],float [],long [],float []);
//void ftrp9(long,long,long*,long,float*,long*,long*,float*,long,long*,long*,long,long);
//void forp9(long,long,long,long [],long,float [],long [],long [],float [],long,long [],long [],long,long);
//void forp15(long,long,long,long [],long,double [],long [],float [],long,long);
//void mforp9(float [],long [],float [],long [],long [],long,long,long,long,long);
//void forp14(long,long,long,long [],long,double [],long [],float [],long [],long,long,long);
//void forp14r(long,long,long,long [],long,double [],long [],float [],long [],long,long,long);
//void diagiiz4_icy(long,long,long,long [],float [],float [],float [],float [],float [],long,float,long [],int);
//void diagiiz4_icx(long,long,long,long [],float [],float [],float [],float [],float [],long,float,long [],long [],long,float [],int);
//float full4_ic(long,long*,float*,float*,float*,long*);float full6_ic(long,long [],float [],float [],float [],long [],long [],float []);
//void load1_icgT(float [],double [],long [],long,long [],float[],float,int);
//void load1q_icgT(long,long [],float [],float [],double [],long [],long [],long [],float [],long,long [],long [],long,float [],long,float,int);
//void body_fgT(long,long [],float [],long [],long [],float [],long [],float [],double [],long,long [],long [],long,float [],float,int);
//void net_load(long,long,long [],float [],double [],float*,float*,float*,int);
//void elvol_maxmin(long,long [],long [],float [],float*,float*);
//void creepf_ic(long,float [],long [],long [],float [],long [],float [],float [],long,long [],long [],long,float []);
//void recon_icg(long,long [],float [],double [],float [],long,long [],float [],long [],float [],long,int);
//void recon_icgT(long,long [],float [],double [],float [],long,long [],float [],long,int);
//void recon_icg1(long,long [],float [],double [],float [],long,long [],float [],long [],float [],float,long,int);
//void recon_icD1(long,long [],float [],double [],long,long [],float [],long);
//void recmn_icg1(long,long [],float [],float [],double [],long,long [],float [],long [],float [],long,int);
//void dump_out4cg(long,float [],long [],long [],float [],float [],double [],float [],int);
//void dump_out4Ecg(long,long [],long [],float [],float [],double [],float [],int);
//void dump_out4cgR(long,float [],long [],long [],float [],long,long);
//void dump_out4temg(long,float [],long [],long [],float [],float [],double [],float [],float [],float [],long,int);
//void dump_out4Dsts(long,long [],long [],float [],double [],float []);
//void dump_out4g(long,float [],long [],long [],float [],float [],double [],float [],long,int);
//void dump_out4gEM(long,float [],long [],long [],float [],float [],double [],float [],long,int);
//void dump_out4og(long,float [],long [],long [],float [],float [],double [],long,int);
//void dump_out4temog(long,float [],long [],long [],float [],float [],double [],float [],float [],long,int);
//void str_cal4D(long,float[],long[],long[],float[],double[],double[],double[],double[],double[],double[],double[],double[],float*);
//void str_cal4Dfd(long,float[],long[],long[],float[],double[],double[],double[],double[],double[],double[],double[],double[],double[]);
//void str_cal4Dpl(long,long[],long[],float[],long[],double[],double[],double[]);
//
//void dump_in4Dsts(long,long,long [],long [],float [],float [],long [],long [],long,long,long);
//void dump_in4DstsD(long,long,long [],long [],float [],double [],long [],long [],long,long,long);
//void dump_in4DstsDgp(long,long,long [],long [],float [],double [],long [],long [],long,long,long);
//
//void dump_out4temgT(long,float [],long [],long [],float [],float [],double [],float [],float [],float [],long,long,int);
//void dump_out4temgATcor(long,float [],long [],long [],float [],float [],double [],float [],float [],float [],float,float,long,long,int);
//void interpolf1(long,float,float*,float*,float*,float*,float*,float*,float*);
//void formtem7T(long,float [],long [],long [],float [],long [],float [],double [],float [],long,long [],long [],long,float [],float [],float [],float,float,long,int);
//
//void dump_in4oDsts(long,long,long [],long [],float [],long [],long [],long,long,long);
//void dump_in4csts(long,long,long [],long [],float [],float [],long [],long [],long,long,long);
//int CAPresultOut(long,float [],long);
//int idCAPresultOut(long,float [],long,long []);
//int ResultFileDel(int,int);
void Indig_analysis(long,long,int,float,int,int,float []);
//int chol_decompi(long [],double [],long,long);
//int cholr_decompi(long [],double [],long,long);int chol_decompo(long [],double [],long,long [],long);
//void chol_forelim(long [],double [],long,double []);void chol_forelimo(long [],double [],double [],long [],long);
//void chol_backsub(long [],double [],long,long,double []);void cholr_backsub(double [],long,double []);
//void chol_backsubo(long [],double [],long,double [],long [],long);
//void mult_lowertri_vect(long [],double [],long,double []);
//void mult_uppertri_vect(long [],double [],long,long,double []);
//void oocswap(long*,long [],long [],long,long,long [],long [],long,long);
//void oocswaph(long*,long [],long [],long,long,long [],long [],long,long,long*);
//int gausr_elimi(long [],double [],long,double []);
//int gausr_elimi1(long [],double [],long);
//void mat_reorder1(long [],double [],long);
int WriteGIF();void cl_hash(long,long []);void xvbzero(char*,int);
void output(int,unsigned long*,char [],int*,int,int,int,int,int,int*,int*,int*);
//void triding(long,float [],float [],float []);void tridingD(long,double [],double [],double []);
//void tridingDoc(long,double [],double [],double [],double [],double [],long,long []);
//long nsturml(long,float,float []);long nsturmld(long,double,double []);
//void c_eigval(long,float [],float [],long*,long);void c_eigvalD(long,double [],double [],long*,long);
//int c_eigvnct1(long,float [],float,float [],float [],long);int c_eigvnct1D(long,double [],double,double [],double [],long);
//void eiganpx(long,float [],long);void eiganpxD(long,double [],long);
//void seigvntt1(long,float [],float []);void seigvntt1D(long,double [],double []);
//void seigvntt1Doc(long,double [],double [],long,long[]);
//void r2normaliz1(long,float []);void r2normaliz(long,long,float []);
//void masst_ic(long,long*,float*,float*,long*);
//void fulm4_ic(long,long,long*,float*,float*,float*,long*,long);
//void IVPfcalc_ic(long,long,long [],float [],float [],float [],float [],float [],float [],long []);
//void fulm6_ic(long,long,long [],float [],float [],float [],long [],long [],long,float []);
//void trimat(long,long,float [],float [],float []);
//void addbasis(long,float [],float [],float,float [],float [],long,long []);
//void addbasisOC(long,float [],float [],float,float [],float [],long,long []);
//void orthproc(long,float [],long);void bcprod(long,long,long,float [],float [],float []);
//void meigvntt(long,float [],float [],long,long);void ran_banner();
void elnum35_d8(int,int,int,long [],long [],long);
void elnum44_d8(int*,int [],long [],long [],long,long []);
void elnum44c_d8(int*,int [],long [],long [],long,long [],long,long [],long);
void sfsearch44_d8(int,int,long [],long [],long,long [],int,int);
void sfsearch35_d8(int,long,long [],long [],int,long,long [],int);
void sfsearch_d6(int,long,long [],long [],int,long,long [],int);
void sfsearch_d4(int,long,long [],long [],int,long,long [],int);
void psect_outline(int,long [],long [],float [],float [],long,long []);void psect_compact(long,long*,float,float,float,float,int,long [],long [],float [],long []);
void sectionproc(long,long,long,long,long [],float [],float []);
void sectionprocC(long,long,long,long,long [],float [],float []);
void homsub(long,float,float,float,long*,float*,float*,float*,long,long,long[],float[],float);
void homsubm(long,long,float,float,float*,float*,float*,float*,float,float,float,long,long,long [],float [],float);

//void meshome3(double [],long [],float [],float*,float*,long,long[],long[],float[],float[],long,float);
//void mesh3(double [],double [],double [],double [],double [],double [],double [],double [],double [],int,float,long,double*,long,long,long,long);
//void mesh3a(double [],double [],double [],long [],long [],long [],long,long,long*,long*,long*,long*,long,long[],long[],float[],long,double*,float,long*,float*,float*);
//void fct2(double [],double [],long);void fct3(double [],long);void poly_calc(long [],long*,long,long,long);
//void surface_local(double [],double [],double [],long,long [],long [],float [],long,long [],float,float);
//void surface_local1(long [],long,long [],long [],float [],long,long [],long,long []);
//void fracini4(double [],double [],double [],long,long [],float [],float [],double [],double [],double [],long [],
//            double [],double [],double [],double [],double [],double [],double [],double [],double [],double [],
//            long,long [],long [],float [],float,float,long,long,long,long,long,double,double,long);
//void fracini4b(double [],double [],double [],long [],double [],double [],double [],double [],double [],double [],double [],
//            long [],float,float,long,long,long,long,double,double);
//void hybrd4(double [],double [],double [],long,long [],float [],double [],double [],long [],
//            double [],double [],double [],double [],double [],double [],double [],double [],
//            long,long [],long [],float [],float,float,long,long,long,long);
//void hybrdc(double [],double [],double,long,long [],float [],double [],double [],double [],long,long [],long [],float [],float,long,long,long);
//void elpsco(double,double,double,double,double,double,double,double,double,double,double,double,double,double*,double*,double*);
//void elptin(double,double,double,double,double,double,double,double,double,double,double,double [],long,double [],double [],double [],double []);
//void elp(double,double,double*,double*,int*);
//long NDLT(long,long);double XN(double,long);double DLT(long,long);
//void dlvtvs(double,double,double,double,double,double,double,double,double,double,double,double,double,
//  double,double,double,double [],double [],long [],double [],double [],double [],long,long,long,double [],double []);
//void coeff(double [],double [],double [],long [],double [],double [],double [],double [],double [],long,long,long);
//void coef2(double [],double [],double [],long [],double [],double [],double [],double [],double [],float,long,long,long);
/////////
//void combo_rr(long,long,long [],long [],double [],double [],long [],long [],float [],long,long,long,long[],float[],long,
//   double[],double[],double[],double[],double[],double [],double [],double [],double [],long [],double[],double[],double[],double[],double[],
//  double[],float[],long[],long[],long,long,long,long,double[],double[],double[],float,float,long,long,long,long,float,float,
//  long,long[],float[],float[],float[],long,long,long [],long,float,double[],double,double,
//  long,float[],double[],float,int);
//void combo_rrn(long [],long [],float [],long,long,double[],double[],double[],double[],double[],double [],double [],double [],double [],long [],double[],double[],
// double [],double [],double [],double[],float[],long[],long[],long,long,long,long,
// double[],double[],double[],float,float,long,long,long,long,float,float,long,float,double[],double,double,
// long,float[],double[],float,int);
/////////
//void iteralt5rr(long,long,long [],long [],double [],double [],
//  long [],long [],float [],long,long,long,long[],float[],long
////                double [],double [],double [],long [],double [],double [],double [],double [],double [],float,long,long,long
//   ,double[],double[],double[],double [],double [],double [],double [],long [],double[],double[],double[],
//  float[],long[],long[],long,long,double[],double[],double[],float,float,long,long,long,long,float,float,
//  long,long[],float[],float[],long,long,long [],long,float,double[],
//  long,double[],float,int);
//void iteralt5rrn(long [],long [],float [],long,long,double[],double[],double[],double [],double [],double [],double [],long [],double[],double[],double[],
//  float[],long[],long[],long,long,double[],double[],double[],float,float,long,long,long,long,float,float,long,float,double[],long,double[],float,int);
//void iteralt5rcr(long,long,long [],long [],double [],double [],
//  long [],long [],float [],long,long,long,long[],float[],long,double[],double[],double[],double[],double[],
//  double[],float[],long[],long,long,double[],double[],double[],float,float,long,long,long,long,float,float,
//  long,long[],float[],float[],long,long,long [],long,float,double[],
//  long,double[],float,int);
//void iteralt5rcrn(long [],long [],float [],long,long,double[],double[],double[],double[],double[],
//  double[],float[],long[],long,long,double[],double[],double[],float,float,long,long,long,long,float,float,long,float,double[],
//  long,double[],float,int);
//void pressur(long[],long,double,double,double[]);
//void stransfd(double[],double[],int);void stransf(float[],float[],int);
//void fitrs(long [],long [],float [],long,double [],double [],long [],double[],double[],
//  float[],long[],long[],long,long,double[],double[],double[],float,float,long,long,long,long,float,int);
//void fitrs_shot(double [],double [],long [],double[],double[],
//  float[],long[],long[],long,long,double[],double[],long,long,long,double[],long);
//void fitrsidcr(long [],long [],float [],long,double [],float[],long[],long,long,double[],double[],
//  double,float,float,long,long,long,long,float,int);
//void fitrsidcr_shot(double [],float[],long[],long,long,double[],
//  double,long,long,long,double[],long);
//void sfactor(double,double,double [],double [],double [],long[],long,long,float,double,long,long,float,float,double[],double[],double[]);
//void sfactorc(double,double[],double[],double [],long,long,float,long,float,float,double[],double[],double[]);
//void rsdual5D(long,long [],float [],double[],double[],long[],long [],long [],long,long,long,long);
//double efp(double,long);double chebevf(double,double,double [],long,double);
//void beschb(double,double*,double*);void BESSJY(double,double,double*,double*,double*,double*);
//double BESSJ0(double);double BESSJ1(double);double BESSJ(long,double);void EFPHBES(double,double,double*);
//double RNMLVAL(long,long,long);void COEFFC(double,double [],long);  // MCHEB=4
//void GAULEG(double [],double [],long);  // N<= 240
//void CYLCO(double,double*,double*,double*,double,double,double);
//void CLVTVS4(double [],double [],double,double,double,double,double [],double [],long,long,long,float,double [],double [],long);
//double GAMMLN(double);void GAULAG(double [],double [],long,double);
//void gen_local(double [],double [],long,long [],long [],float [],long,long [],float [],long,float [],long,long [],
//  long [],long [],float [],long [],float [],float [],long [],long [],float);
//void gen_local_mem(double [],double [],long,long [],long [],float [],long,long [],long [],long*,long*,long*,long*,float);
//void subd_determ(double [],double [],double [],long [],long,long [],long [],float [],long,long,long,long,long,float,long,long [],long [],long [],long []);
//void subd_search0(double [],double [],long [],long,long [],long [],float [],long,double,double,double,double,double,double,long,long*,float);
//void subd_search(double [],double [],long [],long,long [],long [],float [],long,long,long,long,double,double,double,double,double,double,long,long*,float,long);
//void subd_search2(double [],double [],long [],long,long [],long [],float [],long,double,double,double,double,double,double,long,long*,float,long []);
//long subA_search(double [],double [],long,long [],long [],float [],long,double,double,double,double,double,long,long,float,double,double);
//void subA_searchx(double [],double [],long,long [],long [],float [],long,double,double,double,double,double,long,long,float,double,double,long*,long*,double*,double*);
//void arr_condens(long,long,long [],long [],float [],long [],long,long*);
//void gen_condens(long,long,long [],long [],float [],long [],float [],long [],long,long,long,long*);
//void stor_condens(long,long,float [],float [],long [],float [],long,float [],float [],long [],float []);
//double dadnrate(double,double,double);
//void inc_crktt(double [],double [],double,double,double,double,double);
//void pop_divarr(long,long,long [],long [],long [],long [],long []);
//void findfacet(long,long,long [],long [],long [],long [],long []);
//long restrfacet(long,long [],long [],long,long [],long,long []);
//void calc_subsurf(long,long,long [],long [],float [],long,long [],long [],double [],double []);
//float SHOTCARB(long,float [],float);void loadSHOTCARB(long,float[]);
//void calepsck(long,long,double[],double[],double[],double[],double[],double[],double[],double,double,double,double*);
//double FT(double[],double,double,double,double);void CAI(double[],double[]);
//double H1CALC(double,double,double);
//void calepsfd(long,long,float,float,double[],double[],double[],double[],double[],double[],double[],double[],float[],double,double,double);
//void INTERE8DFD(long,long,double[],double[],double[],long[],float[],long[],float[],float[]);
//float CHKCONDP(double[],float[],long);
//aaaaaaaaaaaaa
void MultiResultLeg(float*,float*,int,long,long,int,int,float,float [],int,int,int*);
//        void ResultOut(long,float [],long);
//        void ModelOut1(long,long,long,long [],long [],float [],long []);
//        void ModelOut3(long,long,long,long,long,long,long,long,long [],long [],float[],long [],long [],long [],float [],long [],float [],long [],float [],long [],float [],float [],float []);
		void invdet(long,long [],float []);
//		void invdetd(long,long [],double [],double);
//        void gausseq_unsym(long,float [],float []);
// constant DT
//		void trash0();
//		float tem_calc(float,float);
//        float young_mod(long,float);float poiss_rat(long,float);float thexp_coef(long,float);
//        float temper_cyl(float);float temper_cyl0(float);
//void EPAnalyze_D(int);
//		void WriteUNVModelGIDProgram(long*,long*,long*,long*,long*,long*,long,int,long,long,long [],long [],float [],long [],long [],long [],float [],long [],float [],
//							 long [],float [],long [],float [],float [],float [],float [],long ,long []);
		void secthea4();
		void CmContour();void CmContour_Lg(float,float);
//        void annot_render(int,int [],char []);
//        void annot_render(int,int [],AnsiString []);
		void annot_render(int,int [],AnsiString,AnsiString,AnsiString,AnsiString,AnsiString,
									 AnsiString,AnsiString,AnsiString,AnsiString,AnsiString);
		void anLIN_render(int,int []);
//        void CreepAnalysisProgram(long,long,long,long,long,long,long,long [],long [],float [],long [],long [],float [],long [],float [],
//                             long [],float [],long [],float [],float [],float [],long,int);
//        void CreepAnalysisProgram1(long,long,long,long,long [],long [],float [],long [],long [],float [],
//							 float [],float [],long,float [],int);
// From VFTgen
         void FDelemfacets_arE3(long,long,long [],long [],int []);
void FDpaintplot1(int,long,long);
void create_new_weld(int);
//void create_new_weld_partial();void create_new_weld_full();void create_new_weld_full_fullsect();
void export_ABA_NODE(char []); // Write ABAQUS-format ASCII nodal geometry file *.inp (or *.in), delimited by ","
void export_ABA_ELEMENT8(char []); // Write ABAQUS-format ASCII element (8n only, with mapping) connectivity file *.inp (or *.in), delimited by ","
void export_VED(char [],float *); // VED= virtual element detection  (based on WSIExportABA.java export_VED)
void export_ABA(char []); // This corresponds to NG's export_ABA_NEW() i.e. second version
void export_UFIELD(char [],char [],float); // Write ABAQUS3.5-format UFIELD FORTRAN file. EFP 3/2/2010
void export_ABA_BC(char []); // Write ABAQUS-format ASCII nodal BC
void export_CTSP_INPUTp(char []);
void export_CTSP_INPUTp1(char [],float);
void export_CTSP_INPUTp_mcm(long,long,int,float,char []);
void export_CTSP_NODE(char []); // Identical to ABAQUS-format ASCII nodal geometry file *.inp (or *.in), delimited by ","
void export_CTSP_ELEMENT8(char []); // Identical to ABAQUS format ASCII element (8n only, with mapping) connectivity file *.inp (or *.in), delimited by ","
void persistVFT();
void persistVFT1(long,long);

//long FDcullfacet_arE_mem(long,long [],long [],float [],int []);
//void FDcullfacet_arE(long,long [],long [],float [],int [],long []);
long FDcullfacet_arEc(long,long [],long [],float [],int [],long [],long []);
void maskelemPolyhea(int);
void est_polytri(int [],int *);
int poly_contain(int [],int,float,float);
////void VFT_SaveAs(char []);
//void VFT_SaveAs();
//void VFTrSave();  //EFP 10/02/2011
void VFT_SaveAs1(int);
void axis_plotXYZ(int,int);void axis_plot_init();
void FDrotate_axisplot(int,float[]);
void Rot_program(int,float,float,float);
void DelWeldPass(int,long);
void DelWeldParam(int);
void EditWeldParamProg(int);
void DelMatPropProg(int);
void EditMatPropProg(int);
void EditWeldPassProg(int);
void ImportAba_prog(int);
void ShowWPoutlineProg(int);
void ShowWGselectorProg(int);
//void persistVFT1a(long,int []);
void persistVFT1a(long,long []);
//void persistVFT2(long,int []);
void persistVFT2(long,long []);
//int VFT_AutoSaveGo();
void FDside_reorder(long,long,long,long [],long []);
int NodalCoincidenceTest(long,float [],long [],float);
void NodalCoincidenceElimCompact(long*,float [],long [],float,long,long [],long []);
void NodalCoincidenceElimCompact1(long*,float [],long [],float,long,long [],long []);
int DelSubd0(int,String);
//long checkElemDup(long,long [],long [],long [],long []); //EFP 12/19/2011
//long checkElemDup1(long,long [],long [],long [],long [],long,int []); //EFP 12/19/2011
int checkAlphabetic(char);
int merge_CTSPmcm(int,int,int);
void mergeCTSPmcmProg(int,int,int);
//long ProcessAllWG(long,long [],long [],long,String [],int);
//long ProcessAllWG1(long,long [],long [],long,String [],long,int [],int);
long ProcessAllWG2(long,long [],long,String [],long,int [],int);
long WGannihilate(long,long,long,long [],String []);
//	int ParseSteps(String);
void RevProg0(long);
//void gen_left(float [],float [],float,long *,long *,float,int);void gen_rite(float [],float [],float,long *,long *,float,int);
//void gen_mid0(float [],float [],float,long *,long *,float);void gen_mid1(float [],float [],float,long *,long *,float,int);
//void gen_pree(float [],float [],float,long *,long *,float);void gen_post(float [],float [],float,long *,long *,float);
void ExpoStep(double,double,long,double *,double *);
void CALCT3Point(double,double,double,double *,double *,double *,double *,double,long);
void GetCenter(double *,double *,double *,double,double,double,double);
void calctl(double *,double *,double,long,double);
long parse_ch1(long,long *,char [],char []);
void CTSPinterpolate_prog(int);
void WARP3DepBlock(long);void WARP3DepBlock1(long);
float ElemFacetArea(long,long,long [],long [],float []);
//void ModelOut2(long,long,long,long,long,long,long [],long [],double [],long [],long [],float [],long [],float [],long [],float []);
void tshiftCTSP2(int);
void degen8_test(long* eltype,long* n8,long larr[]);
//zzzzzzzzzzzzz
public:		// User declarations
		AnsiString GeomFileName,ResFileName;
		void CmPmContour_public();
		void secthea_public();void secthea1_public();void secthea2_public();void secthea3_public();
		void wahea_public();void defshapehea_public();void defshapehea1_public();
		void rothea_public();void rothea1_public();void rothea_reset_public();void genaxhea_public();
		void res_scalehea_public();void res_scalehea1_public();
//		void Crack_output_public();
		void GIDRestore_public();void rotheaFix_public();void annot_public();
//		void WriteResultbyCol_public();
//		void WriteUNVselGID_public();
		void QNodeTRASH_public();void QElemTRASH_public();void sectheaTRASH_public();
		void SectWedgeTRASH_public();void Sect3ptTRASH_public();void Sect1p2rTRASH_public();
		void SectqsdTRASH_public();void SpecSCkTRASH_public();void QNDistTRASH_public();
		void SelForm17TRASH_public();void rotheaFixTRASH_public();void Form19TRASH_public();
		void Form8TRASH_public();
void WeldParam_public();
void MatProperties_public();
void WeldPassEditSeqn1_public();void WeldPassEditSeqn2_public();void WeldPassEditSeqn3_public();
void wpCreate_public();void wpCreate1_public();void wpCreate2_public();
void exportCTSP_public();void exportCTSP1_public();
void exportCTSP2_public();
void exportCTSP3_public();
void exportABAQUS_public();void exportABAQUS1_public();
//void EditWeldPass_public();
void wpEdit_public();void wpEdit2_public();
void XYrot_public();void XYrotm_public();void YZrot_public();void YZrotm_public();void ZXrot_public();void ZXrotm_public();
void VFTlistbox_public();
void EditWeldParam_public();
void EditMatProp_public();
void SelStrings_public();
void Form28_public();
void Form28Close_public();
void tshiftCTSP_public();void tshiftCTSP1_public();void tshiftCTSP3_public();
void tdeltCTSP_public();void tdeltCTSP1_public();
void exportWARP3D_public();void exportWARP3D1_public();
void exportWARP3D1a_public();//epWARP3D EFP 12/19/2012

		__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
