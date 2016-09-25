object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 651
  ClientWidth = 837
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  OnMouseWheel = FormMouseWheel
  OnPaint = Form1Paint
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 8
    Width = 537
    Height = 345
  end
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 803
    Height = 651
    Align = alClient
    OnMouseDown = FormMouseDown
    OnMouseMove = FormMouseMove
    OnMouseUp = FormMouseUp
    OnPaint = Form1Paint
    ExplicitLeft = 8
    ExplicitTop = 8
    ExplicitWidth = 755
    ExplicitHeight = 635
  end
  object Panel1: TPanel
    Left = 803
    Top = 0
    Width = 34
    Height = 651
    Align = alRight
    Alignment = taRightJustify
    Color = clBtnShadow
    ParentBackground = False
    TabOrder = 0
    object BitBtn1: TBitBtn
      Left = 0
      Top = 16
      Width = 33
      Height = 25
      Hint = 'Selection mode of graphics'
      TabOrder = 0
      OnClick = BitBtn1Click
    end
    object BitBtn2: TBitBtn
      Left = 0
      Top = 64
      Width = 33
      Height = 25
      Hint = 'View mode (LB pivot, RB pan)'
      TabOrder = 1
      OnClick = BitBtn2Click
    end
    object BitBtn3: TBitBtn
      Left = 0
      Top = 112
      Width = 33
      Height = 25
      Hint = '(unfinished) Graphics help'
      TabOrder = 2
      OnClick = BitBtn3Click
    end
    object BitBtn4: TBitBtn
      Left = 0
      Top = 160
      Width = 33
      Height = 25
      Hint = '(unfinished) Set current view as home view'
      TabOrder = 3
      OnClick = BitBtn4Click
    end
    object BitBtn5: TBitBtn
      Left = 0
      Top = 208
      Width = 33
      Height = 25
      Hint = '(unfinished) Return to this home view'
      TabOrder = 4
      OnClick = BitBtn5Click
    end
    object BitBtn6: TBitBtn
      Left = 0
      Top = 256
      Width = 33
      Height = 25
      Hint = 'View all regions of mesh'
      TabOrder = 5
      OnClick = BitBtn6Click
    end
    object BitBtn7: TBitBtn
      Left = 0
      Top = 304
      Width = 33
      Height = 25
      Hint = 'Target zoom'
      TabOrder = 6
      OnClick = BitBtn7Click
    end
    object BitBtn8: TBitBtn
      Left = 0
      Top = 352
      Width = 33
      Height = 25
      Hint = 'Box zoom'
      TabOrder = 7
      OnClick = BitBtn8Click
    end
    object BitBtn9: TBitBtn
      Left = 0
      Top = 392
      Width = 33
      Height = 25
      Hint = 'Restore all elements'
      Caption = 'R'
      TabOrder = 8
      OnClick = BitBtn9Click
    end
    object BitBtn10: TBitBtn
      Left = 0
      Top = 432
      Width = 33
      Height = 25
      Hint = 'Hide elements by polygon'
      Caption = 'H'
      TabOrder = 9
      OnClick = BitBtn10Click
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 56
    Top = 392
  end
  object OpenDialog1: TOpenDialog
    Left = 144
    Top = 392
  end
  object OpenDialog2: TOpenDialog
    Left = 232
    Top = 392
  end
  object SaveDialog1: TSaveDialog
    Left = 320
    Top = 392
  end
  object MainMenu1: TMainMenu
    Left = 472
    Top = 392
    object File1: TMenuItem
      Caption = 'File'
      object Open1: TMenuItem
        Caption = 'Open'
        object ImportAba1: TMenuItem
          Action = ImportAba
        end
        object ImportMsh1: TMenuItem
          Action = ImportMsh
        end
        object ImportVFTr1: TMenuItem
          Action = ImportVFTr
        end
      end
      object SaveAs21: TMenuItem
        Action = SaveAs2
        Caption = 'SaveAs Model.VFTr'
      end
      object Export1: TMenuItem
        Caption = 'Export'
        object exportCTSP1: TMenuItem
          Action = exportCTSP
          Caption = 'CTSP files...'
        end
        object exportWARP3D1: TMenuItem
          Action = exportWARP3D
          Caption = 'WARP3D files...'
        end
      end
      object imeshift1: TMenuItem
        Caption = 'Timeshift'
        object imeshiftCTSPandVED01: TMenuItem
          Action = TimeshiftCTSPandVED0
          Caption = 'Timeshift Warp_temp+VED'
        end
        object imeshiftCTSP01: TMenuItem
          Action = TimeshiftCTSP0
          Caption = 'Timeshift Warp_temp'
        end
        object imeshiftVED01: TMenuItem
          Action = TimeshiftVED0
          Caption = 'Timeshift VED'
        end
      end
      object CTSPinterpSolid1: TMenuItem
        Action = CTSPinterpSolid
        Caption = 'Combine two temp.out'
      end
      object FileClose1: TMenuItem
        Action = FileClose
      end
      object Exit1: TMenuItem
        Action = Exit
      end
    end
    object Mesh1: TMenuItem
      Caption = 'Mesh'
      object FEstats1: TMenuItem
        Action = FEstats
        Caption = 'FEstats...'
      end
      object BCs1: TMenuItem
        Caption = 'BCs'
        object Addbynodeclick1: TMenuItem
          Caption = 'Add by node click'
          object BCsNclick01: TMenuItem
            Action = BCsNclick0
            Caption = 'Ux = 0'
          end
          object BCsNclick11: TMenuItem
            Action = BCsNclick1
            Caption = 'Uy = 0'
          end
          object BCsNclick21: TMenuItem
            Action = BCsNclick2
            Caption = 'Uz = 0'
          end
          object BCsNclick31: TMenuItem
            Action = BCsNclick3
            Caption = 'Ux = Uy = 0'
          end
          object BCsNclick41: TMenuItem
            Action = BCsNclick4
            Caption = 'Ux = Uz = 0'
          end
          object BCsNclick51: TMenuItem
            Action = BCsNclick5
            Caption = 'Uy = Uz = 0'
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
    object Query1: TMenuItem
      Caption = 'Query'
      object QueryNode1: TMenuItem
        Action = QueryNode
        Caption = 'QueryNode...'
      end
      object QueryElement1: TMenuItem
        Action = QueryElement
        Caption = 'QueryElement...'
      end
      object NodalDistance1: TMenuItem
        Action = NodalDistance
        Caption = 'NodalDistance...'
      end
      object ElFacetArea1: TMenuItem
        Action = ElFacetArea
      end
    end
    object View1: TMenuItem
      Caption = 'View'
      object Normaldirection1: TMenuItem
        Caption = 'Normals convention'
        object Normaldirection11: TMenuItem
          Action = Normal_direction1
          Caption = 'Normal direction'
        end
        object Complementarydirection11: TMenuItem
          Action = Complementary_direction1
          Caption = 'Complementary dir'
        end
      end
      object Backroundcolor1: TMenuItem
        Caption = 'Backround color'
        object BackroundColorF01: TMenuItem
          Action = BackroundColorF0
          Caption = 'Black'
        end
        object BackroundColorF11: TMenuItem
          Action = BackroundColorF1
          Caption = 'White'
        end
        object BackroundColorF21: TMenuItem
          Action = BackroundColorF2
          Caption = 'Red'
        end
        object BackroundColorF31: TMenuItem
          Action = BackroundColorF3
          Caption = 'Green'
        end
        object BackroundColorF41: TMenuItem
          Action = BackroundColorF4
          Caption = 'Blue'
        end
      end
      object XYrot1: TMenuItem
        Action = XY_rot
        Caption = 'XY rotate'
      end
      object XYmrot1: TMenuItem
        Action = XYm_rot
        Caption = '-XY rotate'
      end
      object YZrot1: TMenuItem
        Action = YZ_rot
        Caption = 'YZ rotate'
      end
      object YZmrot1: TMenuItem
        Action = YZm_rot
        Caption = '-YZ rotate'
      end
      object ZXrot1: TMenuItem
        Action = ZX_rot
        Caption = 'ZX rotate'
      end
      object ZXmrot1: TMenuItem
        Action = ZXm_rot
        Caption = '-ZX rotate'
      end
    end
    object Weld1: TMenuItem
      Caption = 'Weld'
      object Weldparameters1: TMenuItem
        Caption = 'Weld parameters'
        object WeldParam1: TMenuItem
          Action = WeldParam
          Caption = 'Create...'
        end
        object EditWeldParam1: TMenuItem
          Action = EditWeldParam
          Caption = 'Edit...'
        end
        object DeleteWeldParam1: TMenuItem
          Action = DeleteWeldParam
          Caption = 'Delete...'
        end
      end
      object Materialproperties1: TMenuItem
        Caption = 'Material properties'
        object MatProperties1: TMenuItem
          Action = MatProperties
          Caption = 'Create...'
        end
        object EditMatProp1: TMenuItem
          Action = EditMatProp
          Caption = 'Edit...'
        end
        object DeleteMatProp1: TMenuItem
          Action = DeleteMatProp
          Caption = 'Delete...'
        end
      end
      object Weldpasses1: TMenuItem
        Caption = 'Create WP to end run'
        object CreateWPFullLengthFullWidth1: TMenuItem
          Action = CreateWPFullLengthFullWidth
          Caption = 'Full section...'
        end
        object CreateWPFullLengthPartWidth1: TMenuItem
          Action = CreateWPFullLengthPartWidth
          Caption = 'Part section...'
        end
      end
      object Partlengthweldpass1: TMenuItem
        Caption = 'Create part length WP'
        object CreateWPPartLengthFullWidth1: TMenuItem
          Action = CreateWPPartLengthFullWidth
          Caption = 'Full section...'
        end
        object CreateWPPartLengthPartWidth1: TMenuItem
          Action = CreateWPPartLengthPartWidth
          Caption = 'Part section...'
        end
      end
      object EditWeldPass1: TMenuItem
        Action = EditWeldPass
        Caption = 'Edit weld pass...'
      end
      object DeleteWeldPass1: TMenuItem
        Action = DeleteWeldPass
        Caption = 'Delete weld pass...'
      end
      object WeldPassEditingandSequencing11: TMenuItem
        Caption = 'Weld pass sequence'
        object WeldPassEditingandSequencing12: TMenuItem
          Action = WeldPassEditingandSequencing1
          Caption = 'By Move Up/Down...'
        end
        object WeldPassEditingandSequencing21: TMenuItem
          Action = WeldPassEditingandSequencing2
          Caption = 'By CheckListBox...'
        end
      end
      object Changeall1: TMenuItem
        Caption = 'Change all'
        object Weldparameters2: TMenuItem
          Caption = 'Weld parameters'
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
            Caption = 'Arc efficiency'
          end
          object ChAllWParamspeed1: TMenuItem
            Action = ChAllWParam_speed
            Caption = 'Torch speed'
          end
        end
        object Materialproperties2: TMenuItem
          Caption = 'Material properties'
          object ChAllMatPropThCond1: TMenuItem
            Action = ChAllMatProp_ThCond
            Caption = 'Thermal cond'
          end
          object ChAllMatPropSpHeat1: TMenuItem
            Action = ChAllMatProp_SpHeat
            Caption = 'Specific heat'
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
        object Weldpassdata1: TMenuItem
          Caption = 'Weld pass data'
          object CTSPinputs1: TMenuItem
            Caption = 'CTSP inputs'
            object ChAllWPasstimeInterv1: TMenuItem
              Action = ChAllWPass_timeInterv
              Caption = 'IP cooling time'
            end
            object ChAllWPassstepInterval1: TMenuItem
              Action = ChAllWPass_stepInterval
              Caption = 'IP cooling steps'
            end
            object ChAllWPassmaxiHeatStep1: TMenuItem
              Action = ChAllWPass_maxiHeatStep
              Caption = 'Max heat step'
            end
            object ChAllWPassminiHeatStep1: TMenuItem
              Action = ChAllWPass_miniHeatStep
              Caption = 'Min heat step'
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
          object Weldparameters3: TMenuItem
            Caption = 'Weld parameters'
            object ChAllWPasstroom1: TMenuItem
              Action = ChAllWPass_troom
              Caption = 'Room T'
            end
            object ChAllWPasstmelt1: TMenuItem
              Action = ChAllWPass_tmelt
              Caption = 'Melting T'
            end
            object ChAllWPasstcutl1: TMenuItem
              Action = ChAllWPass_tcutl
              Caption = 'Low-cut T'
            end
            object ChAllWPasstcuth1: TMenuItem
              Action = ChAllWPass_tcuth
              Caption = 'Pre-heat T'
            end
          end
        end
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object AboutVFT1: TMenuItem
        Action = AboutVFT
      end
    end
  end
  object ActionList1: TActionList
    Left = 400
    Top = 392
    object ImportAba: TAction
      Category = 'File'
      Caption = 'ImportAba'
      OnExecute = ImportAbaExecute
    end
    object ImportMsh: TAction
      Category = 'File'
      Caption = 'ImportMsh'
      OnExecute = ImportMshExecute
    end
    object FileClose: TAction
      Category = 'File'
      Caption = 'FileClose'
      OnExecute = FileCloseExecute
    end
    object Exit: TAction
      Category = 'File'
      Caption = 'Exit'
      OnExecute = ExitExecute
    end
    object SaveAs2: TAction
      Category = 'File'
      Caption = 'SaveAs2'
      OnExecute = SaveAs2Execute
    end
    object CTSPinterpSolid: TAction
      Category = 'File'
      Caption = 'CTSPinterpSolid'
      OnExecute = CTSPinterpSolidExecute
    end
    object ElFacetArea: TAction
      Category = 'Query'
      Caption = 'ElFacetArea'
      OnExecute = ElFacetAreaExecute
    end
    object FEstats: TAction
      Category = 'Mesh'
      Caption = 'FEstats'
      OnExecute = FEstatsExecute
    end
    object QueryNode: TAction
      Category = 'Query'
      Caption = 'QueryNode'
      OnExecute = QueryNodeExecute
    end
    object NodalDistance: TAction
      Category = 'Query'
      Caption = 'NodalDistance'
      OnExecute = NodalDistanceExecute
    end
    object QueryElement: TAction
      Category = 'Query'
      Caption = 'QueryElement'
      OnExecute = QueryElementExecute
    end
    object Normal_direction1: TAction
      Category = 'View'
      Caption = 'Normal_direction1'
      OnExecute = Normal_direction1Execute
    end
    object Complementary_direction1: TAction
      Category = 'View'
      Caption = 'Complementary_direction1'
      OnExecute = Complementary_direction1Execute
    end
    object BackroundColorF0: TAction
      Category = 'View'
      Caption = 'BackroundColorF0'
      OnExecute = BackroundColorF0Execute
    end
    object BackroundColorF1: TAction
      Category = 'View'
      Caption = 'BackroundColorF1'
      OnExecute = BackroundColorF1Execute
    end
    object BackroundColorF2: TAction
      Category = 'View'
      Caption = 'BackroundColorF2'
      OnExecute = BackroundColorF2Execute
    end
    object BackroundColorF3: TAction
      Category = 'View'
      Caption = 'BackroundColorF3'
      OnExecute = BackroundColorF3Execute
    end
    object BackroundColorF4: TAction
      Category = 'View'
      Caption = 'BackroundColorF4'
      OnExecute = BackroundColorF4Execute
    end
    object XY_rot: TAction
      Category = 'View'
      Caption = 'XY_rot'
      OnExecute = XY_rotExecute
    end
    object XYm_rot: TAction
      Category = 'View'
      Caption = 'XYm_rot'
      OnExecute = XYm_rotExecute
    end
    object YZ_rot: TAction
      Category = 'View'
      Caption = 'YZ_rot'
      OnExecute = YZ_rotExecute
    end
    object YZm_rot: TAction
      Category = 'View'
      Caption = 'YZm_rot'
      OnExecute = YZm_rotExecute
    end
    object ZX_rot: TAction
      Category = 'View'
      Caption = 'ZX_rot'
      OnExecute = ZX_rotExecute
    end
    object ZXm_rot: TAction
      Category = 'View'
      Caption = 'ZXm_rot'
      OnExecute = ZXm_rotExecute
    end
    object WeldParam: TAction
      Category = 'Weld'
      Caption = 'WeldParam'
      OnExecute = WeldParamExecute
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
    object MatProperties: TAction
      Category = 'Weld'
      Caption = 'MatProperties'
      OnExecute = MatPropertiesExecute
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
    object CreateWPFullLengthFullWidth: TAction
      Category = 'Weld'
      Caption = 'CreateWPFullLengthFullWidth'
      OnExecute = CreateWPFullLengthFullWidthExecute
    end
    object CreateWPFullLengthPartWidth: TAction
      Category = 'Weld'
      Caption = 'CreateWPFullLengthPartWidth'
      OnExecute = CreateWPFullLengthPartWidthExecute
    end
    object CreateWPPartLengthFullWidth: TAction
      Category = 'Weld'
      Caption = 'CreateWPPartLengthFullWidth'
      OnExecute = CreateWPPartLengthFullWidthExecute
    end
    object CreateWPPartLengthPartWidth: TAction
      Category = 'Weld'
      Caption = 'CreateWPPartLengthPartWidth'
      OnExecute = CreateWPPartLengthPartWidthExecute
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
    object WeldPassEditingandSequencing1: TAction
      Category = 'Weld'
      Caption = 'WeldPassEditingandSequencing1'
      OnExecute = WeldPassEditingandSequencing1Execute
    end
    object exportCTSP: TAction
      Category = 'File'
      Caption = 'exportCTSP'
      OnExecute = exportCTSPExecute
    end
    object exportWARP3D: TAction
      Category = 'File'
      Caption = 'exportWARP3D'
      OnExecute = exportWARP3DExecute
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
    object AboutVFT: TAction
      Category = 'Help'
      Caption = 'AboutVFT'
      OnExecute = AboutVFTExecute
    end
    object TimeshiftCTSPandVED0: TAction
      Category = 'File'
      Caption = 'TimeshiftCTSPandVED0'
      OnExecute = TimeshiftCTSPandVED0Execute
    end
    object TimeshiftCTSP0: TAction
      Category = 'File'
      Caption = 'TimeshiftCTSP0'
      OnExecute = TimeshiftCTSP0Execute
    end
    object TimeshiftVED0: TAction
      Category = 'File'
      Caption = 'TimeshiftVED0'
      OnExecute = TimeshiftVED0Execute
    end
    object MergeTimeshiftCTSPandVED01: TAction
      Category = 'File'
      Caption = 'MergeTimeshiftCTSPandVED01'
      OnExecute = MergeTimeshiftCTSPandVED01Execute
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
    object ImportVFTr: TAction
      Category = 'File'
      Caption = 'ImportVFTr'
      OnExecute = ImportVFTrExecute
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
    object WeldPassEditingandSequencing2: TAction
      Category = 'Weld'
      Caption = 'WeldPassEditingandSequencing2'
      OnExecute = WeldPassEditingandSequencing2Execute
    end
  end
end
