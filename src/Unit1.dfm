object Form1: TForm1
  Left = 53
  Top = 428
  Caption = 'Final Design(TM)'
  ClientHeight = 513
  ClientWidth = 525
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -24
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnMouseWheel = FormMouseWheel
  OnPaint = Form1Paint
  OnResize = Form1Paint
  PixelsPerInch = 96
  TextHeight = 29
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 477
    Height = 513
    Align = alClient
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    ExplicitWidth = 376
    ExplicitHeight = 385
  end
  object Image1: TImage
    Left = 22
    Top = 8
    Width = 449
    Height = 377
  end
  object RichEdit1: TRichEdit
    Left = 8
    Top = 8
    Width = 185
    Height = 89
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'RichEdit1')
    ParentFont = False
    TabOrder = 0
    Visible = False
  end
  object Panel1: TPanel
    Left = 477
    Top = 0
    Width = 48
    Height = 513
    Align = alRight
    Alignment = taRightJustify
    Color = cl3DDkShadow
    ParentBackground = False
    TabOrder = 1
    object BitBtn1: TBitBtn
      Left = 6
      Top = 8
      Width = 35
      Height = 33
      Hint = 'Selection mode of graphics'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 6
      Top = 47
      Width = 35
      Height = 33
      Hint = 'View mode (LB pivot, RB pan)'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = BitBtn2Click
    end
    object BitBtn3: TBitBtn
      Left = 6
      Top = 95
      Width = 35
      Height = 34
      Hint = '(unfinished) Graphics help'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = BitBtn3Click
    end
    object BitBtn4: TBitBtn
      Left = 6
      Top = 144
      Width = 35
      Height = 33
      Hint = '(unfinished) Set current view as home view'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 6
      Top = 183
      Width = 35
      Height = 33
      Hint = '(unfinished) Return to this home view'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = BitBtn5Click
    end
    object BitBtn6: TBitBtn
      Left = 6
      Top = 233
      Width = 35
      Height = 32
      Hint = 'View all regions of mesh'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = BitBtn6Click
    end
    object BitBtn7: TBitBtn
      Left = 6
      Top = 280
      Width = 33
      Height = 33
      Hint = 'Target zoom'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 6
      OnClick = BitBtn7Click
    end
    object BitBtn8: TBitBtn
      Left = 6
      Top = 328
      Width = 35
      Height = 33
      Hint = 'Box zoom'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 7
      OnClick = BitBtn8Click
    end
    object BitBtn9: TBitBtn
      Left = 6
      Top = 376
      Width = 35
      Height = 32
      Hint = 'Restore all elements'
      Caption = 'R'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 8
      OnClick = BitBtn9Click
    end
    object BitBtn10: TBitBtn
      Left = 6
      Top = 424
      Width = 35
      Height = 32
      Hint = 'Hide elements by polygon'
      Caption = 'H'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 9
      OnClick = BitBtn10Click
    end
    object BitBtn11: TBitBtn
      Left = 6
      Top = 462
      Width = 34
      Height = 35
      Hint = 'Reverse weldpass (RBC to quit)'
      Caption = 'Di'
      DoubleBuffered = True
      ParentDoubleBuffered = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 10
      OnClick = BitBtn11Click
    end
  end
  object ActionList1: TActionList
    Left = 216
    Top = 8
    object CAPgeomOpen: TAction
      Category = 'File'
      Caption = 'CAPgeomOpen'
      Hint = 'Open Cap format geom'
    end
    object FileClose: TAction
      Category = 'File'
      Caption = 'FileClose'
      Hint = 'Close file'
      OnExecute = FileCloseExecute
    end
    object Exit: TAction
      Category = 'File'
      Caption = 'Exit'
      Hint = 'Exit FD'
      OnExecute = ExitExecute
    end
    object ViewPaint: TAction
      Category = 'View'
      Caption = 'ViewPaint'
      Hint = 'Paint model'
    end
    object CAPformatOpen: TAction
      Category = 'Post'
      Caption = 'CAPformatOpen'
      Hint = 'Open CAP format result'
    end
    object Legend_auto: TAction
      Category = 'Post'
      Caption = 'Legend_auto'
      Hint = 'Auto legend'
      OnExecute = Legend_autoExecute
    end
    object Legend_custom: TAction
      Category = 'Post'
      Caption = 'Legend_custom'
      Hint = 'Custom legend'
      OnExecute = Legend_customExecute
    end
    object Rotate: TAction
      Category = 'View'
      Caption = 'Rotate'
      OnExecute = RotateExecute
    end
    object ZoomIn: TAction
      Category = 'View'
      Caption = 'ZoomIn'
      OnExecute = ZoomInExecute
    end
    object ZoomOut: TAction
      Category = 'View'
      Caption = 'ZoomOut'
      OnExecute = ZoomOutExecute
    end
    object ZoomReset: TAction
      Category = 'View'
      Caption = 'ZoomReset'
      OnExecute = ZoomResetExecute
    end
    object FDstress_analysis: TAction
      Category = 'Post'
      Caption = 'FDstress_analysis'
    end
    object CAPthermal_analysis: TAction
      Category = 'Post'
      Caption = 'CAPthermal_analysis'
    end
    object QueryNode: TAction
      Category = 'Query'
      Caption = 'QueryNode'
      OnExecute = QueryNodeExecute
    end
    object CrackGrowth: TAction
      Category = 'Animate'
      Caption = 'CrackGrowth'
    end
    object ShowCracks1: TAction
      Category = 'Show'
      Caption = 'ShowCracks'
    end
    object QueryElement: TAction
      Category = 'Query'
      Caption = 'QueryElement'
      OnExecute = QueryElementExecute
    end
    object ReadViewFile: TAction
      Category = 'File'
      Caption = 'ReadViewFile'
    end
    object WriteViewFile: TAction
      Category = 'File'
      Caption = 'WriteViewFile'
    end
    object WriteModel: TAction
      Category = 'File'
      Caption = 'WriteModel'
    end
    object WriteResult: TAction
      Category = 'File'
      Caption = 'WriteResult'
    end
    object AnimResult: TAction
      Category = 'Animate'
      Caption = 'AnimResult'
      OnExecute = AnimResultExecute
    end
    object AnimMultiResult: TAction
      Category = 'Animate'
      Caption = 'AnimMultiResult'
      OnExecute = AnimMultiResultExecute
    end
    object SuperElOpen: TAction
      Category = 'File'
      Caption = 'SuperElOpen'
    end
    object FDmodal_analysis: TAction
      Category = 'Post'
      Caption = 'FDmodal_analysis'
    end
    object Cart_frame: TAction
      Category = 'Post'
      Caption = 'Cart_frame'
      Checked = True
      OnExecute = Cart_frameExecute
    end
    object Cyl_frame: TAction
      Category = 'Post'
      Caption = 'Cyl_frame'
    end
    object Spher_frame: TAction
      Category = 'Post'
      Caption = 'Spher_frame'
      OnExecute = Spher_frameExecute
    end
    object Section_X: TAction
      Category = 'View'
      Caption = 'Section_X'
      OnExecute = Section_XExecute
    end
    object Section_Y: TAction
      Category = 'View'
      Caption = 'Section_Y'
      OnExecute = Section_YExecute
    end
    object Section_Z: TAction
      Category = 'View'
      Caption = 'Section_Z'
      OnExecute = Section_ZExecute
    end
    object Undeform_shape: TAction
      Category = 'Post'
      Caption = 'Undeform_shape'
      OnExecute = Undeform_shapeExecute
    end
    object Deform_shape: TAction
      Category = 'Post'
      Caption = 'Deform_shape'
      OnExecute = Deform_shapeExecute
    end
    object Def_undef_shape: TAction
      Category = 'Post'
      Caption = 'Def_undef_shape'
      OnExecute = Def_undef_shapeExecute
    end
    object Section_wedge: TAction
      Category = 'View'
      Caption = 'Section_wedge'
      OnExecute = Section_wedgeExecute
    end
    object ApperGID: TAction
      Category = 'View'
      Caption = 'ApperGID'
      OnExecute = ApperGIDExecute
    end
    object ApperMetal: TAction
      Category = 'View'
      Caption = 'ApperMetal'
      OnExecute = ApperMetalExecute
    end
    object ApperShade: TAction
      Category = 'View'
      Caption = 'ApperShade'
      OnExecute = ApperShadeExecute
    end
    object ContourAll: TAction
      Category = 'Post'
      Caption = 'ContourAll'
      OnExecute = ContourAllExecute
    end
    object ContourSection: TAction
      Category = 'Post'
      Caption = 'ContourSection'
      OnExecute = ContourSectionExecute
    end
    object ShowTiny: TAction
      Category = 'Show'
      Caption = 'ShowTiny'
    end
    object ShowElliptic: TAction
      Category = 'Show'
      Caption = 'ShowElliptic'
    end
    object Cyl_Xframe: TAction
      Category = 'Post'
      Caption = 'Cyl_Xframe'
      OnExecute = Cyl_XframeExecute
    end
    object Cyl_Yframe: TAction
      Category = 'Post'
      Caption = 'Cyl_Yframe'
      OnExecute = Cyl_YframeExecute
    end
    object Cyl_Zframe: TAction
      Category = 'Post'
      Caption = 'Cyl_Zframe'
      OnExecute = Cyl_ZframeExecute
    end
    object Cyl_Gframe: TAction
      Category = 'Post'
      Caption = 'Cyl_Gframe'
      OnExecute = Cyl_GframeExecute
    end
    object Result_scale: TAction
      Category = 'Post'
      Caption = 'Result_scale'
      OnExecute = Result_scaleExecute
    end
    object NEUgeomOpen: TAction
      Category = 'File'
      Caption = 'NEUgeomOpen'
    end
    object NASgeomOpen: TAction
      Category = 'File'
      Caption = 'NASgeomOpen'
    end
    object Section_3pt: TAction
      Category = 'View'
      Caption = 'Section_3pt'
      OnExecute = Section_3ptExecute
    end
    object Section_1pt2rot: TAction
      Category = 'View'
      Caption = 'Section_1pt2rot'
      OnExecute = Section_1pt2rotExecute
    end
    object QueryStressDir: TAction
      Category = 'Query'
      Caption = 'QueryStressDir'
      OnExecute = QueryStressDirExecute
    end
    object SingleEllipticCrack: TAction
      Category = 'Specify'
      Caption = 'SingleEllipticCrack'
    end
    object NodalDistance: TAction
      Category = 'Query'
      Caption = 'NodalDistance'
      OnExecute = NodalDistanceExecute
    end
    object FEstats: TAction
      Category = 'Mesh'
      Caption = 'FEstats'
      OnExecute = FEstatsExecute
    end
    object LEAnalyze: TAction
      Category = 'Analyze'
      Caption = 'LEAnalyze'
    end
    object UNVgeomOpen: TAction
      Category = 'File'
      Caption = 'UNVgeomOpen'
    end
    object EPAnalyze: TAction
      Category = 'Analyze'
      Caption = 'EPAnalyze'
    end
    object GIFsnapshot: TAction
      Category = 'File'
      Caption = 'GIFsnapshot'
      OnExecute = GIFsnapshotExecute
    end
    object ShowFEMesh: TAction
      Category = 'View'
      Caption = 'ShowFEMesh'
      OnExecute = ShowFEMeshExecute
    end
    object NoShowFEMesh: TAction
      Category = 'View'
      Caption = 'NoShowFEMesh'
      OnExecute = NoShowFEMeshExecute
    end
    object ThermDynAnalyze: TAction
      Category = 'Analyze'
      Caption = 'ThermDynAnalyze'
    end
    object AnimMultiResGIF: TAction
      Category = 'Animate'
      Caption = 'AnimMultiResGIF'
      OnExecute = AnimMultiResGIFExecute
    end
    object AnimResSnap: TAction
      Category = 'Animate'
      Caption = 'AnimResSnap'
      OnExecute = AnimResSnapExecute
    end
    object EigenAnalyze: TAction
      Category = 'Analyze'
      Caption = 'EigenAnalyze'
    end
    object CreepAnalyze: TAction
      Category = 'Analyze'
      Caption = 'CreepAnalyze'
    end
    object ShowEdge: TAction
      Category = 'View'
      Caption = 'ShowEdge'
      OnExecute = ShowEdgeExecute
    end
    object HideEdge1: TAction
      Category = 'View'
      Caption = 'HideEdge'
      OnExecute = HideEdge1Execute
    end
    object LEFMFatAnalyze: TAction
      Category = 'Analyze'
      Caption = 'LEFMFatAnalyze'
    end
    object Result_LinePlot: TAction
      Category = 'Post'
      Caption = 'Result_LinePlot'
      OnExecute = Result_LinePlotExecute
    end
    object RotateFreehand: TAction
      Category = 'View'
      Caption = 'RotateFreehand'
      OnExecute = RotateFreehandExecute
    end
    object LEFMFractAnalyze: TAction
      Category = 'Analyze'
      Caption = 'LEFMFractAnalyze'
    end
    object FDstress_strain_analysis: TAction
      Category = 'Post'
      Caption = 'FDstress_strain_analysis'
    end
    object LEAnalyDirectSol: TAction
      Category = 'Analyze'
      Caption = 'LEAnalyDirectSol'
    end
    object MeshCharacteristics: TAction
      Category = 'Analyze'
      Caption = 'MeshCharacteristics'
      OnExecute = MeshCharacteristicsExecute
    end
    object GIDselector: TAction
      Category = 'Post'
      Caption = 'GIDselector'
      OnExecute = GIDselectorExecute
    end
    object RotateFixed: TAction
      Category = 'View'
      Caption = 'RotateFixed'
      OnExecute = RotateFixedExecute
    end
    object InitialValueProblem: TAction
      Category = 'Analyze'
      Caption = 'InitialValueProblem'
    end
    object LEAnalyze_T: TAction
      Category = 'Analyze'
      Caption = 'LEAnalyze_T'
    end
    object WriteUNVmodelGID: TAction
      Category = 'File'
      Caption = 'WriteUNVmodelGID'
    end
    object LEAnalyze_T_MultiS: TAction
      Category = 'Analyze'
      Caption = 'LEAnalyze_T_MultiS'
    end
    object Generic_analysis: TAction
      Category = 'Post'
    end
    object WriteResultbyCol: TAction
      Category = 'File'
      Caption = 'WriteResultbyCol'
    end
    object WriteUNVmodelAllGID: TAction
      Category = 'File'
      Caption = 'WriteUNVmodelAllGID'
    end
    object QueryCoordinate: TAction
      Category = 'Query'
      Caption = 'QueryCoordinate'
      OnExecute = QueryCoordinateExecute
    end
    object LEAnalyzeD_T: TAction
      Category = 'Analyze'
      Caption = 'LEAnalyzeD_T'
    end
    object EPAnalyzeD_T: TAction
      Category = 'Analyze'
      Caption = 'EPAnalyzeD_T'
    end
    object ResultVolAverage: TAction
      Category = 'Post'
      Caption = 'ResultVolAverage'
      OnExecute = ResultVolAverageExecute
    end
    object Annotate: TAction
      Category = 'Post'
      Caption = 'Annotate'
      OnExecute = AnnotateExecute
    end
    object ClearAnnotate: TAction
      Category = 'Post'
      Caption = 'ClearAnnotate'
      OnExecute = ClearAnnotateExecute
    end
    object AnnotLine: TAction
      Category = 'Post'
      Caption = 'AddAnnotLine'
      OnExecute = AnnotLineExecute
    end
    object ElectStatAnalyze: TAction
      Category = 'Analyze'
      Caption = 'ElectStatAnalyze'
    end
    object CreepAnalysisFile: TAction
      Category = 'Analyze'
      Caption = 'CreepAnalysisFile'
    end
    object RelaxAnalysisFile: TAction
      Category = 'Analyze'
      Caption = 'RelaxAnalysisFile'
    end
    object RelaxAnalyze: TAction
      Category = 'Analyze'
      Caption = 'RelaxAnalyze'
    end
    object DynamicTimeInteg: TAction
      Category = 'Analyze'
      Caption = 'DynamicTimeInteg'
    end
    object MSHgeomOpen: TAction
      Category = 'File'
      Caption = 'MSHgeomOpen'
      OnExecute = MSHgeomOpenExecute
    end
    object VFTweld_analysis: TAction
      Category = 'Post'
      Caption = 'VFTweld_analysis'
      OnExecute = VFTweld_analysisExecute
    end
    object Import_VED: TAction
      Category = 'Post'
      Caption = 'Import_VED'
      OnExecute = Import_VEDExecute
    end
    object MSHgeomVEDopen: TAction
      Category = 'Post'
      Caption = 'MSHgeomVEDopen'
      OnExecute = MSHgeomVEDopenExecute
    end
    object ImportAbaABQ: TAction
      Category = 'File'
      Caption = 'ImportAbaABQ'
      OnExecute = ImportAbaABQExecute
    end
    object ImportAbaInp: TAction
      Category = 'File'
      Caption = 'ImportAbaInp'
      OnExecute = ImportAbaInpExecute
    end
    object WeldParam: TAction
      Category = 'Weld'
      Caption = 'WeldParam'
      OnExecute = WeldParamExecute
    end
    object MatProperties: TAction
      Category = 'Weld'
      Caption = 'MatProperties'
      OnExecute = MatPropertiesExecute
    end
    object WeldPassEditingandSequencing1: TAction
      Category = 'Weld'
      Caption = 'WeldPassEditingandSequencing1'
      OnExecute = WeldPassEditingandSequencing1Execute
    end
    object CreateNewWeldPass: TAction
      Category = 'Weld'
      Caption = 'CreateNewWeldPass'
    end
    object CreateNewFullLenghtWeldPass: TAction
      Category = 'Weld'
      Caption = 'CreateNewFullLenghtWeldPass'
      OnExecute = CreateNewFullLenghtWeldPassExecute
    end
    object CreateNewPartialLengthWeldPass: TAction
      Category = 'Weld'
      Caption = 'CreateNewPartialLengthWeldPass'
      OnExecute = CreateNewPartialLengthWeldPassExecute
    end
    object exportCTSP: TAction
      Category = 'File'
      Caption = 'exportCTSP'
      OnExecute = exportCTSPExecute
    end
    object exportABAQUS: TAction
      Category = 'File'
      Caption = 'exportABAQUS'
      OnExecute = exportABAQUSExecute
    end
    object MaskElem0: TAction
      Category = 'View'
      Caption = 'MaskElem0'
      OnExecute = MaskElem0Execute
    end
    object BackGroundColorF0: TAction
      Category = 'View'
      Caption = 'BackGroundColorF0'
      OnExecute = BackGroundColorF0Execute
    end
    object BackGroundColorF1: TAction
      Category = 'View'
      Caption = 'BackGroundColorF1'
      OnExecute = BackGroundColorF1Execute
    end
    object BackGroundColorF2: TAction
      Category = 'View'
      Caption = 'BackGroundColorF2'
      OnExecute = BackGroundColorF2Execute
    end
    object BackGroundColorF3: TAction
      Category = 'View'
      Caption = 'BackGroundColorF3'
      OnExecute = BackGroundColorF3Execute
    end
    object BackGroundColorF4: TAction
      Category = 'View'
      Caption = 'BackGroundColorF4'
      OnExecute = BackGroundColorF4Execute
    end
    object BackGroundColorF5: TAction
      Category = 'View'
      Caption = 'BackGroundColorF5'
      OnExecute = BackGroundColorF5Execute
    end
    object XY_rot: TAction
      Category = 'View'
      Caption = 'XY_rot'
      OnExecute = XY_rotExecute
    end
    object YZ_rot: TAction
      Category = 'View'
      Caption = 'YZ_rot'
      OnExecute = YZ_rotExecute
    end
    object ZX_rot: TAction
      Category = 'View'
      Caption = 'ZX_rot'
      OnExecute = ZX_rotExecute
    end
    object PanGo1: TAction
      Category = 'View'
      Caption = 'PanGo1'
      OnExecute = PanGo1Execute
    end
    object Save: TAction
      Category = 'File'
      Caption = 'Save'
      OnExecute = SaveExecute
    end
    object SaveAs2: TAction
      Category = 'File'
      Caption = 'SaveAs2'
      OnExecute = SaveAs2Execute
    end
    object Restore: TAction
      Category = 'View'
      Caption = 'Restore'
      OnExecute = RestoreExecute
    end
    object ImportVFTr: TAction
      Category = 'File'
      Caption = 'ImportVFTr'
      OnExecute = ImportVFTrExecute
    end
    object EditWeldPass: TAction
      Category = 'Weld'
      Caption = 'EditWeldPass'
      OnExecute = EditWeldPassExecute
    end
    object DeleteWeldPass: TAction
      Category = 'Weld'
      Caption = 'DeleteWeldPass'
      OnExecute = DeleteWeldPassExecute
    end
    object XY_rotm: TAction
      Category = 'View'
      Caption = 'XY_rotm'
      OnExecute = XY_rotmExecute
    end
    object YZ_rotm1: TAction
      Category = 'View'
      Caption = '-YZ_rot'
      OnExecute = YZ_rotm1Execute
    end
    object ZX_rotm: TAction
      Category = 'View'
      Caption = 'ZX_rotm'
      OnExecute = ZX_rotmExecute
    end
    object EditWeldPass3: TAction
      Category = 'Weld'
      Caption = 'EditWeldPass3'
    end
    object EditWeldParam: TAction
      Category = 'Weld'
      Caption = 'EditWeldParam'
      OnExecute = EditWeldParamExecute
    end
    object DeleteWeldParam: TAction
      Category = 'Weld'
      Caption = 'DeleteWeldParam'
      OnExecute = DeleteWeldParamExecute
    end
    object EditMatProp: TAction
      Category = 'Weld'
      Caption = 'EditMatProp'
      OnExecute = EditMatPropExecute
    end
    object DeleteMatProp: TAction
      Category = 'Weld'
      Caption = 'DeleteMatProp'
      OnExecute = DeleteMatPropExecute
    end
    object ShowWG_red: TAction
      Category = 'View'
      Caption = 'ShowWG_red'
      OnExecute = ShowWG_redExecute
    end
    object ShowWG_polychrome: TAction
      Category = 'View'
      Caption = 'ShowWG_polychrome'
      OnExecute = ShowWG_polychromeExecute
    end
    object CreateFullLengthFullSection: TAction
      Category = 'Weld'
      Caption = 'CreateFullLengthFullSection'
      OnExecute = CreateFullLengthFullSectionExecute
    end
    object CreatPartLengthFullSection: TAction
      Category = 'Weld'
      Caption = 'CreatPartLengthFullSection'
      OnExecute = CreatPartLengthFullSectionExecute
    end
    object ShowWP_last: TAction
      Category = 'View'
      Caption = 'ShowWP_last'
      OnExecute = ShowWP_lastExecute
    end
    object ShowWP_hide: TAction
      Category = 'View'
      Caption = 'ShowWP_hide'
      OnExecute = ShowWP_hideExecute
    end
    object ShowWP_select: TAction
      Category = 'View'
      Caption = 'ShowWP_select'
      OnExecute = ShowWP_selectExecute
    end
    object ShowWPstartelem_list: TAction
      Category = 'View'
      Caption = 'ShowWPstartelem_list'
      OnExecute = ShowWPstartelem_listExecute
    end
    object ShowWPstartelem_Hide: TAction
      Category = 'View'
      Caption = 'ShowWPstartelem_Hide'
      OnExecute = ShowWPstartelem_HideExecute
    end
    object VFT_AutoSave1Go: TAction
      Category = 'File'
      Caption = 'VFT_AutoSave1Go'
      OnExecute = VFT_AutoSave1GoExecute
    end
    object AutoSaveOn: TAction
      Category = 'File'
      Caption = 'AutoSaveOn'
      OnExecute = AutoSaveOnExecute
    end
    object AutoSaveOff: TAction
      Category = 'File'
      Caption = 'AutoSaveOff'
      OnExecute = AutoSaveOffExecute
    end
    object Normal_direction1: TAction
      Category = 'View'
      Caption = 'Normal_direction1'
      OnExecute = Normal_direction1Execute
    end
    object Complem_direction1: TAction
      Category = 'View'
      Caption = 'Complem_direction1'
      OnExecute = Complem_direction1Execute
    end
    object NodalCoincidence: TAction
      Category = 'Query'
      Caption = 'NodalCoincidence'
      OnExecute = NodalCoincidenceExecute
    end
    object ChAllWParam_curr: TAction
      Category = 'Weld'
      Caption = 'ChAllWParam_curr'
      OnExecute = ChAllWParam_currExecute
    end
    object ChAllWParam_volt: TAction
      Category = 'Weld'
      Caption = 'ChAllWParam_volt'
      OnExecute = ChAllWParam_voltExecute
    end
    object ChAllWParam_eff: TAction
      Category = 'Weld'
      Caption = 'ChAllWParam_eff'
      OnExecute = ChAllWParam_effExecute
    end
    object ChAllWParam_speed: TAction
      Category = 'Weld'
      Caption = 'ChAllWParam_speed'
      OnExecute = ChAllWParam_speedExecute
    end
    object ChAllMatProp_ThCond: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_ThCond'
      OnExecute = ChAllMatProp_ThCondExecute
    end
    object ChAllMatProp_SpHeat: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_SpHeat'
      OnExecute = ChAllMatProp_SpHeatExecute
    end
    object ChAllMatProp_Dens: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_Dens'
      OnExecute = ChAllMatProp_DensExecute
    end
    object ChAllMatProp_Anneal: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_Anneal'
      OnExecute = ChAllMatProp_AnnealExecute
    end
    object ChAllMatProp_Ti: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_Ti'
      OnExecute = ChAllMatProp_TiExecute
    end
    object ChAllMatProp_Ta: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_Ta'
      OnExecute = ChAllMatProp_TaExecute
    end
    object ChAllMatProp_Tm: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_Tm'
      OnExecute = ChAllMatProp_TmExecute
    end
    object ChAllMatProp_VED: TAction
      Category = 'Weld'
      Caption = 'ChAllMatProp_VED'
      OnExecute = ChAllMatProp_VEDExecute
    end
    object ChAllWPass_timeInterv: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_timeInterv'
      OnExecute = ChAllWPass_timeIntervExecute
    end
    object ChAllWPass_stepInterval: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_stepInterval'
      OnExecute = ChAllWPass_stepIntervalExecute
    end
    object ChAllWPass_maxiHeatStep: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_maxiHeatStep'
      OnExecute = ChAllWPass_maxiHeatStepExecute
    end
    object ChAllWPass_miniHeatStep: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_miniHeatStep'
      OnExecute = ChAllWPass_miniHeatStepExecute
    end
    object ChAllWPass_thk1: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_thk1'
      OnExecute = ChAllWPass_thk1Execute
    end
    object ChAllWPass_thk2: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_thk2'
      OnExecute = ChAllWPass_thk2Execute
    end
    object ChAllWPass_troom: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_troom'
      OnExecute = ChAllWPass_troomExecute
    end
    object ChAllWPass_tmelt: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_tmelt'
      OnExecute = ChAllWPass_tmeltExecute
    end
    object ChAllWPass_tcutl: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_tcutl'
      OnExecute = ChAllWPass_tcutlExecute
    end
    object ChAllWPass_tcuth: TAction
      Category = 'Weld'
      Caption = 'ChAllWPass_tcuth'
      OnExecute = ChAllWPass_tcuthExecute
    end
    object ChangeTimer4_Interv: TAction
      Category = 'File'
      Caption = 'ChangeTimer4_Interv'
      OnExecute = ChangeTimer4_IntervExecute
    end
    object mergeCTSPmcmNoDel: TAction
      Category = 'File'
      Caption = 'mergeCTSPmcmNoDel'
      OnExecute = mergeCTSPmcmNoDelExecute
    end
    object mergeCTSPmcmDel: TAction
      Category = 'File'
      Caption = 'mergeCTSPmcmDel'
      OnExecute = mergeCTSPmcmDelExecute
    end
    object AboutVFT: TAction
      Category = 'Help'
      Caption = 'AboutVFT'
      OnExecute = AboutVFTExecute
    end
    object WG_selector: TAction
      Category = 'View'
      Caption = 'WG_selector'
      OnExecute = WG_selectorExecute
    end
    object WGselAll: TAction
      Category = 'View'
      Caption = 'WGselAll'
      OnExecute = WGselAllExecute
    end
    object ShowWPout_All: TAction
      Category = 'View'
      Caption = 'ShowWPout_All'
      OnExecute = ShowWPout_AllExecute
    end
    object RevEveryWP: TAction
      Category = 'Weld'
      Caption = 'RevEveryWP'
      OnExecute = RevEveryWPExecute
    end
    object Every2ndWP: TAction
      Category = 'Weld'
      Caption = 'Every2ndWP'
      OnExecute = Every2ndWPExecute
    end
    object EveryOddWP: TAction
      Category = 'Weld'
      Caption = 'EveryOddWP'
      OnExecute = EveryOddWPExecute
    end
    object ResetWPdir: TAction
      Category = 'Weld'
      Caption = 'ResetWPdir'
      OnExecute = ResetWPdirExecute
    end
    object exportWARP3D: TAction
      Category = 'File'
      Caption = 'exportWARP3D'
      OnExecute = exportWARP3DExecute
    end
    object TimeshiftCTSP0: TAction
      Category = 'File'
      Caption = 'TimeshiftCTSP0'
      OnExecute = TimeshiftCTSP0Execute
    end
    object TempDeltCTSP0: TAction
      Category = 'File'
      Caption = 'TempDeltCTSP0'
      OnExecute = TempDeltCTSP0Execute
    end
    object CTSPinterpSolid: TAction
      Category = 'File'
      Caption = 'CTSPinterpSolid'
      OnExecute = CTSPinterpSolidExecute
    end
    object CTSPinterpShell: TAction
      Category = 'File'
      Caption = 'CTSPinterpShell'
      OnExecute = CTSPinterpShellExecute
    end
    object WARP3Dconvenience: TAction
      Category = 'File'
      Caption = 'WARP3Dconvenience'
      OnExecute = WARP3DconvenienceExecute
    end
    object TimeshiftVED0: TAction
      Category = 'File'
      Caption = 'TimeshiftVED0'
      OnExecute = TimeshiftVED0Execute
    end
    object TimeshiftCTSPandVED0: TAction
      Category = 'File'
      Caption = 'TimeshiftCTSPandVED0'
      OnExecute = TimeshiftCTSPandVED0Execute
    end
    object BCsNclick0: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick0'
      OnExecute = BCsNclick0Execute
    end
    object BCsNclick1: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick1'
      OnExecute = BCsNclick1Execute
    end
    object BCsNclick2: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick2'
      OnExecute = BCsNclick2Execute
    end
    object BCsNclick3: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick3'
      OnExecute = BCsNclick3Execute
    end
    object BCsNclick4: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick4'
      OnExecute = BCsNclick4Execute
    end
    object BCsNclick5: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick5'
      OnExecute = BCsNclick5Execute
    end
    object BCsNclick6: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick6'
      OnExecute = BCsNclick6Execute
    end
    object BCsNclick7: TAction
      Category = 'Mesh'
      Caption = 'BCsNclick7'
      OnExecute = BCsNclick7Execute
    end
    object ElFacetArea: TAction
      Category = 'Query'
      Caption = 'ElFacetArea'
      OnExecute = ElFacetAreaExecute
    end
    object MergeTimeshiftCTSP0: TAction
      Category = 'File'
      Caption = 'MergeTimeshiftCTSP0'
      OnExecute = MergeTimeshiftCTSP0Execute
    end
    object MergeTimeshiftVED0: TAction
      Category = 'File'
      Caption = 'MergeTimeshiftVED0'
      OnExecute = MergeTimeshiftVED0Execute
    end
    object MergeTimeshiftCTSPandVED0: TAction
      Category = 'File'
      Caption = 'MergeTimeshiftCTSPandVED0'
      OnExecute = MergeTimeshiftCTSPandVED0Execute
    end
    object ImportVFTa: TAction
      Category = 'File'
      Caption = 'ImportVFTa'
      OnExecute = ImportVFTaExecute
    end
    object Action1: TAction
      Category = 'File'
      Caption = 'Action1'
      OnExecute = Action1Execute
    end
    object SaveAs3: TAction
      Category = 'File'
      Caption = 'SaveAs3'
      OnExecute = SaveAs3Execute
    end
  end
  object MainMenu1: TMainMenu
    Left = 216
    Top = 48
    object File1: TMenuItem
      Caption = 'File'
      object ImportSolidMesh1: TMenuItem
        Caption = 'ImportSolidMesh'
        object FromAbaqusInputinp1: TMenuItem
          Action = ImportAbaInp
        end
        object FromSIMULIAAbaqabq1: TMenuItem
          Action = ImportAbaABQ
        end
        object MSHgeomOpen1: TMenuItem
          Action = MSHgeomOpen
        end
      end
      object ImportVFTr2: TMenuItem
        Action = ImportVFTr
        Caption = 'Import_VFTr'
      end
      object ImportVFTa1: TMenuItem
        Action = ImportVFTa
      end
      object Save1: TMenuItem
        Action = Save
        Caption = 'Save VFTr'
      end
      object SaveAs1: TMenuItem
        Action = SaveAs2
        Caption = 'Save As *.VFTr ...'
      end
      object SaveAs31: TMenuItem
        Action = SaveAs3
        Caption = 'SaveAs*.VFTadd'
      end
      object AutoSave1: TMenuItem
        Caption = 'AutoSave'
        object On1: TMenuItem
          Action = AutoSaveOn
          Caption = 'On'
        end
        object Off1: TMenuItem
          Action = AutoSaveOff
          Caption = 'Off'
        end
        object ChangeTimer4Interv1: TMenuItem
          Action = ChangeTimer4_Interv
          Caption = 'Interval'
        end
      end
      object Export: TMenuItem
        Caption = 'Export'
        object exportCTSP1: TMenuItem
          Action = exportCTSP
        end
        object exportABAQUS1: TMenuItem
          Action = exportABAQUS
        end
        object exportWARP3D1: TMenuItem
          Action = exportWARP3D
        end
        object GIFsnap1: TMenuItem
          Action = GIFsnapshot
        end
      end
      object imeshiftCTSP01: TMenuItem
        Caption = 'Timeshift'
        object imeshiftCTSP02: TMenuItem
          Action = TimeshiftCTSP0
          Caption = 'Timeshift CTSP temp.out'
        end
        object imeshiftVED01: TMenuItem
          Action = TimeshiftVED0
          Caption = 'Timeshift VED'
        end
        object imeshiftCTSPandVED01: TMenuItem
          Action = TimeshiftCTSPandVED0
          Caption = 'Timeshift CTSP+VED'
        end
      end
      object MergeTimeshifted1: TMenuItem
        Caption = 'Merge Timeshifted Nonoverlap'
        object MergeTimeshiftCTSP01: TMenuItem
          Action = MergeTimeshiftCTSP0
          Caption = 'CTSP'
        end
        object MergeTimeshiftVED01: TMenuItem
          Action = MergeTimeshiftVED0
          Caption = 'VED'
        end
        object MergeTimeshiftCTSPandVED01: TMenuItem
          Action = MergeTimeshiftCTSPandVED0
          Caption = 'CTSP and VED'
        end
      end
      object empDeltCTSPtempout1: TMenuItem
        Caption = 'WARP3D options'
        object TempDeltWARP3D: TMenuItem
          Action = TempDeltCTSP0
          Caption = 'TempDeltCTSP from temp.out'
        end
        object WARP3Dconvenience1: TMenuItem
          Action = WARP3Dconvenience
          Caption = 'Create warp-finalFULL from time.out'
        end
      end
      object CTSPinterpolate1: TMenuItem
        Caption = 'CTSPinterpolate temp.out'
        object CTSPinterpSolid1: TMenuItem
          Action = CTSPinterpSolid
          Caption = 'Combine two solid results'
        end
        object CTSPinterpShell1: TMenuItem
          Action = CTSPinterpShell
          Caption = 'Combine two shell results'
        end
      end
      object MergeCTSPmulticore1: TMenuItem
        Caption = 'Merge CTSP multi-core'
        object mergeCTSPmcmNoDel1: TMenuItem
          Action = mergeCTSPmcmNoDel
          Caption = 'Merge CTSP results'
        end
        object mergeCTSPmcmDel1: TMenuItem
          Action = mergeCTSPmcmDel
          Caption = 'Merge+delete subdirectories'
        end
      end
      object Close1: TMenuItem
        Action = FileClose
      end
      object Exit1: TMenuItem
        Action = Exit
      end
    end
    object Analyze1: TMenuItem
      Caption = 'Mesh'
      object FEstats2: TMenuItem
        Action = FEstats
        Caption = 'FEstats...'
      end
      object BCs1: TMenuItem
        Caption = 'BCs'
        object Addbynodeclick1: TMenuItem
          Caption = 'Add by node click'
          object Ux01: TMenuItem
            Action = BCsNclick0
            Caption = 'Ux=0'
          end
          object BCsNclick11: TMenuItem
            Action = BCsNclick1
            Caption = 'Uy=0'
          end
          object BCsNclick21: TMenuItem
            Action = BCsNclick2
            Caption = 'Uz=0'
          end
          object BCsNclick31: TMenuItem
            Action = BCsNclick3
            Caption = 'Ux=Uy=0'
          end
          object BCsNclick41: TMenuItem
            Action = BCsNclick4
            Caption = 'Ux=Uz=0'
          end
          object BCsNclick51: TMenuItem
            Action = BCsNclick5
            Caption = 'Uy=Uz=0'
          end
          object BCsNclick61: TMenuItem
            Action = BCsNclick6
            Caption = 'Ux=Uy=Uz=0'
          end
        end
        object Removebyclick1: TMenuItem
          Action = BCsNclick7
          Caption = 'Remove by click'
        end
      end
    end
    object Node1: TMenuItem
      Caption = 'Query'
      object Query1: TMenuItem
        Action = QueryNode
      end
      object QueryElement1: TMenuItem
        Action = QueryElement
      end
      object NodalDistance1: TMenuItem
        Action = NodalDistance
      end
      object NodalCoincidence1: TMenuItem
        Action = NodalCoincidence
      end
      object ElFacetArea1: TMenuItem
        Action = ElFacetArea
        Caption = 'ElementFacetArea'
      end
    end
    object Animate1: TMenuItem
      Caption = 'Animate'
      object AnimateMultiResult3: TMenuItem
        Caption = 'AnimateWeldDeposit'
        object AnimMultiResult2: TMenuItem
          Action = AnimMultiResult
          Caption = 'AnimWeldDeposit'
        end
        object AnimMultiResGIF1: TMenuItem
          Action = AnimMultiResGIF
          Caption = 'AnimWeldDepSnapshots'
        end
      end
    end
    object Post1: TMenuItem
      Caption = 'Post'
      object MSHgeomVEDopen2: TMenuItem
        Action = MSHgeomVEDopen
        Caption = 'Open MSH+VED'
      end
      object Analysis1: TMenuItem
        Caption = 'AnalysisResult'
        object VFTweldanalysis1: TMenuItem
          Action = VFTweld_analysis
        end
      end
      object Legend: TMenuItem
        Caption = 'Legend'
        object Legendauto1: TMenuItem
          Action = Legend_auto
        end
        object Legendcustom1: TMenuItem
          Action = Legend_custom
        end
      end
      object CoordFrame1: TMenuItem
        Caption = 'Coord.Frame'
        object Cartesian1: TMenuItem
          Action = Cart_frame
        end
        object Cylindframe1: TMenuItem
          Caption = 'Cylind_frame'
          object CylXaxis1: TMenuItem
            Action = Cyl_Xframe
          end
          object CylYaxis1: TMenuItem
            Action = Cyl_Yframe
          end
          object CylZaxis1: TMenuItem
            Action = Cyl_Zframe
          end
          object CylGaxis1: TMenuItem
            Action = Cyl_Gframe
          end
        end
        object Spherframe1: TMenuItem
          Action = Spher_frame
        end
      end
      object Defundeformed1: TMenuItem
        Caption = 'Def/undeformed'
        object Undeformedmesh1: TMenuItem
          Action = Undeform_shape
        end
        object Deformedshape1: TMenuItem
          Action = Deform_shape
        end
        object Defshapeundefmesh1: TMenuItem
          Action = Def_undef_shape
        end
      end
      object Contour1: TMenuItem
        Caption = 'Contour'
        object All1: TMenuItem
          Action = ContourAll
          Caption = 'All'
        end
        object Section2: TMenuItem
          Action = ContourSection
          Caption = 'Section'
        end
      end
      object Annotate1: TMenuItem
        Caption = 'Annotate'
        object AnnotLine1: TMenuItem
          Action = AnnotLine
        end
        object Annotate2: TMenuItem
          Action = Annotate
          Caption = 'AddAnnotate'
        end
        object ClearAnnotate1: TMenuItem
          Action = ClearAnnotate
        end
      end
      object GIDselector1: TMenuItem
        Action = GIDselector
        Caption = 'FE group selector'
      end
      object ImportVED1: TMenuItem
        Action = Import_VED
      end
    end
    object View1: TMenuItem
      Caption = 'View'
      object Rotate1: TMenuItem
        Action = Rotate
        Caption = 'RotateImage...'
      end
      object Zoom: TMenuItem
        Caption = 'Zoom'
        object ZoomIn1: TMenuItem
          Action = ZoomIn
        end
        object ZoomOut1: TMenuItem
          Action = ZoomOut
        end
        object ZoomReset1: TMenuItem
          Action = ZoomReset
        end
      end
      object ShowWeldGroup1: TMenuItem
        Caption = 'WeldGroup Colors'
        object ShowWGred1: TMenuItem
          Action = ShowWG_red
        end
        object ShowWGpolychrome1: TMenuItem
          Action = ShowWG_polychrome
        end
      end
      object ShowWGonly1: TMenuItem
        Caption = 'Show WG only'
        object WGselect1: TMenuItem
          Action = WG_selector
        end
        object WGall1: TMenuItem
          Action = WGselAll
          Caption = 'Show all WG'
        end
      end
      object Showweldpassoutline1: TMenuItem
        Caption = 'Show weld pass outline'
        object ShowWPoutAll1: TMenuItem
          Action = ShowWPout_All
          Caption = 'ShowWP_All'
        end
        object ShowWPselect1: TMenuItem
          Action = ShowWP_select
        end
        object N11: TMenuItem
          Action = ShowWP_last
        end
        object N21: TMenuItem
          Action = ShowWP_hide
          Caption = 'ShowWP_OFF'
        end
      end
      object ShowWPstartelem1: TMenuItem
        Caption = 'ShowWP start elements'
        object ShowWPstartelemlist1: TMenuItem
          Action = ShowWPstartelem_list
          Caption = 'Start elem by list...'
        end
        object ShowWPstartelemHide1: TMenuItem
          Action = ShowWPstartelem_Hide
          Caption = 'ShowWPstartel_OFF'
        end
      end
      object BackGroundColor: TMenuItem
        Caption = 'Background Color'
        object BackgroundColor0: TMenuItem
          Action = BackGroundColorF0
          Caption = 'Black'
        end
        object BackgroundColor1: TMenuItem
          Action = BackGroundColorF1
          Caption = 'White'
        end
        object BackgroundColor2: TMenuItem
          Action = BackGroundColorF2
          Caption = 'Red'
        end
        object BackgroundColor3: TMenuItem
          Action = BackGroundColorF3
          Caption = 'Green'
        end
        object BackgroundColor4: TMenuItem
          Action = BackGroundColorF4
          Caption = 'Blue'
        end
        object BackgroundColor5: TMenuItem
          Action = BackGroundColorF5
          Caption = 'Other colors'
        end
      end
      object Normalsconvention1: TMenuItem
        Caption = 'Normals convention'
        object Normal_direction: TMenuItem
          Action = Normal_direction1
        end
        object Complem_direction: TMenuItem
          Action = Complem_direction1
        end
      end
      object HideElemByPolygon: TMenuItem
        Action = MaskElem0
        Caption = 'Hide elements by polygon'
      end
      object Restore1: TMenuItem
        Action = Restore
        Caption = 'Restore elements'
      end
      object XYView1: TMenuItem
        Action = XY_rot
      end
      object XY_rot2: TMenuItem
        Action = XY_rotm
        Caption = '-XY_rot'
      end
      object YZView1: TMenuItem
        Action = YZ_rot
      end
      object YZ_rotm: TMenuItem
        Action = YZ_rotm1
      end
      object ZXView1: TMenuItem
        Action = ZX_rot
      end
      object ZX_rot1: TMenuItem
        Action = ZX_rotm
        Caption = '-ZX_rot'
      end
    end
    object Weld1: TMenuItem
      Caption = 'Weld'
      object WeldParameters1: TMenuItem
        Caption = 'Weld Parameters'
        object WeldParam1: TMenuItem
          Action = WeldParam
          Caption = 'Create WeldParam...'
        end
        object EditWeldParam1: TMenuItem
          Action = EditWeldParam
          Caption = 'Edit WeldParam...'
        end
        object DeleteWeldParam1: TMenuItem
          Action = DeleteWeldParam
          Caption = 'Delete WeldParam...'
        end
      end
      object MaterialProperties1: TMenuItem
        Caption = 'Material Properties'
        object MatProperties1: TMenuItem
          Action = MatProperties
          Caption = 'Create MatProperties...'
        end
        object EditMatProp1: TMenuItem
          Action = EditMatProp
          Caption = 'Edit MatProperties...'
        end
        object DeleteMatProp1: TMenuItem
          Action = DeleteMatProp
          Caption = 'Delete MatProperties...'
        end
      end
      object CreateNewFullLengthWeldPass1: TMenuItem
        Caption = 'CreateWeldPassToEndRun'
        object CreateFullLengthFullSection2: TMenuItem
          Action = CreateFullLengthFullSection
          Caption = 'Full WG section...'
        end
        object CreateNewFullLenghtWeldPass2: TMenuItem
          Action = CreateNewFullLenghtWeldPass
          Caption = 'Partial WG section...'
        end
      end
      object CreateNewPartialLengthWeldPass1: TMenuItem
        Caption = 'CreatePartLengthWeldPass'
        object CreateNewPartialLengthWeldPass2: TMenuItem
          Action = CreateNewPartialLengthWeldPass
          Caption = 'Partial WG section...'
        end
        object CreatPartLengthFullSection2: TMenuItem
          Action = CreatPartLengthFullSection
          Caption = 'Full WG section...'
        end
      end
      object EditWeldPass1: TMenuItem
        Action = EditWeldPass
        Caption = 'Edit WeldPass...'
      end
      object DeleteWeldPass1: TMenuItem
        Action = DeleteWeldPass
        Caption = 'DeleteWeldPass...'
      end
      object WeldPassEditSequencing1: TMenuItem
        Action = WeldPassEditingandSequencing1
        Caption = 'WeldPassSequencing...'
      end
      object ChangeAll1: TMenuItem
        Caption = 'Change All'
        object WeldParameters2: TMenuItem
          Caption = 'Weld Parameters'
          object ChAllWParamcurr1: TMenuItem
            Action = ChAllWParam_curr
            Caption = 'Current'
          end
          object ChAllWParamvolt1: TMenuItem
            Action = ChAllWParam_volt
            Caption = 'Voltage'
          end
          object ChAllWParameff1: TMenuItem
            Action = ChAllWParam_eff
            Caption = 'Efficiency'
          end
          object ChAllWParamspeed1: TMenuItem
            Action = ChAllWParam_speed
            Caption = 'Speed'
          end
        end
        object MaterialProperties2: TMenuItem
          Caption = 'Material Properties'
          object ChAllMatPropThCond1: TMenuItem
            Action = ChAllMatProp_ThCond
            Caption = 'ThermalCond'
          end
          object ChAllMatPropSpHeat1: TMenuItem
            Action = ChAllMatProp_SpHeat
            Caption = 'SpecificHeat'
          end
          object ChAllMatPropDens1: TMenuItem
            Action = ChAllMatProp_Dens
            Caption = 'Density'
          end
          object Anneal1: TMenuItem
            Caption = 'Anneal'
            object ChAllMatPropAnneal1: TMenuItem
              Action = ChAllMatProp_Anneal
              Caption = 'Yes/no'
            end
            object ChAllMatPropTi1: TMenuItem
              Action = ChAllMatProp_Ti
              Caption = 'Initiation T'
            end
            object ChAllMatPropTa1: TMenuItem
              Action = ChAllMatProp_Ta
              Caption = 'Anneal T'
            end
            object ChAllMatPropTm1: TMenuItem
              Action = ChAllMatProp_Tm
              Caption = 'Melting T'
            end
          end
          object ChAllMatPropVED1: TMenuItem
            Action = ChAllMatProp_VED
            Caption = 'VED yes/no'
          end
        end
        object WeldPassData1: TMenuItem
          Caption = 'Weld Pass Data'
          object CTSPinputs1: TMenuItem
            Caption = 'CTSPinputs'
            object ChAllWPasstimeInterv1: TMenuItem
              Action = ChAllWPass_timeInterv
              Caption = 'InterPassCooling Time'
            end
            object ChAllWPassstepInterval1: TMenuItem
              Action = ChAllWPass_stepInterval
              Caption = 'InterPassCooling Steps'
            end
            object ChAllWPassmaxiHeatStep1: TMenuItem
              Action = ChAllWPass_maxiHeatStep
              Caption = 'Max heating time'
            end
            object ChAllWPassminiHeatStep1: TMenuItem
              Action = ChAllWPass_miniHeatStep
              Caption = 'Min heating time'
            end
            object ChAllWPassthk11: TMenuItem
              Action = ChAllWPass_thk1
              Caption = 'Plate#1 thickness'
            end
            object ChAllWPassthk21: TMenuItem
              Action = ChAllWPass_thk2
              Caption = 'Plate#2 thickness'
            end
          end
          object WeldParameters3: TMenuItem
            Caption = 'WeldParameters'
            object ChAllWPasstroom1: TMenuItem
              Action = ChAllWPass_troom
              Caption = 'Room temperature'
            end
            object ChAllWPasstmelt1: TMenuItem
              Action = ChAllWPass_tmelt
              Caption = 'Melting temperature'
            end
            object ChAllWPasstcutl1: TMenuItem
              Action = ChAllWPass_tcutl
              Caption = 'Low-cut temperature'
            end
            object ChAllWPasstcuth1: TMenuItem
              Action = ChAllWPass_tcuth
              Caption = 'Preheat temperature'
            end
          end
        end
      end
      object ReverseWeld1: TMenuItem
        Caption = 'Reverse Weldpass'
        object RevEveryWP1: TMenuItem
          Action = RevEveryWP
          Caption = 'Every WP'
        end
        object Every2ndWP1: TMenuItem
          Action = Every2ndWP
          Caption = 'Even WPs'
        end
        object EveryOddWP1: TMenuItem
          Action = EveryOddWP
          Caption = 'Odd WPs'
        end
        object ResetWPdir1: TMenuItem
          Action = ResetWPdir
          Caption = 'Reset all WP'
        end
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object Unfinished1: TMenuItem
        Action = AboutVFT
        Caption = 'About VFT'
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '*'
    Filter = 
      'CAP geom + BCs|*.out|CAP geom|*.geo|SuperEl|*.sed|Patran neutral' +
      '|*.neu|Nastran ASCII|*.nas|I-DEAS Universal|*.unv|CAP general|*.' +
      'gcf'
    FilterIndex = 6
    Left = 216
    Top = 88
  end
  object OpenDialog2: TOpenDialog
    DefaultExt = '*'
    Filter = '*'
    Left = 216
    Top = 128
  end
  object Timer1: TTimer
    OnTimer = GitGo
    Left = 8
    Top = 168
  end
  object Timer2: TTimer
    OnTimer = PanGo1Execute
    Left = 48
    Top = 168
  end
  object Timer3: TTimer
    OnTimer = MultiResultGo
    Left = 88
    Top = 168
  end
  object Timer4: TTimer
    OnTimer = VFT_AutoSave1GoExecute
    Left = 128
    Top = 168
  end
  object OpenDialog3: TOpenDialog
    DefaultExt = '*'
    Filter = 'Crack data|*.ckd'
    Left = 216
    Top = 160
  end
  object OpenDialog4: TOpenDialog
    DefaultExt = '*.sol'
    Filter = 'Batch control file|*.sol'
    Left = 168
    Top = 104
  end
  object SaveDialog1: TSaveDialog
    Left = 32
    Top = 232
  end
end
