object Form15: TForm15
  Left = 0
  Top = 102
  BorderIcons = []
  ClientHeight = 591
  ClientWidth = 420
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 85
    Height = 13
    Caption = 'Weld Pass Name:'
  end
  object Label2: TLabel
    Left = 192
    Top = 8
    Width = 98
    Height = 13
    Caption = 'Pass Sequence No.:'
  end
  object Edit1: TEdit
    Left = 104
    Top = 8
    Width = 65
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 296
    Top = 8
    Width = 65
    Height = 21
    TabOrder = 1
    Text = 'Edit2'
  end
  object Button1: TButton
    Left = 8
    Top = 560
    Width = 75
    Height = 25
    Caption = 'Create'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 88
    Top = 560
    Width = 75
    Height = 25
    Caption = 'Inoperative'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 168
    Top = 560
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 4
    OnClick = Button3Click
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 27
    Width = 401
    Height = 521
    ActivePage = TabSheet3
    TabOrder = 5
    object TabSheet1: TTabSheet
      Caption = 'Weld Pass Location'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label7: TLabel
        Left = 143
        Top = 393
        Width = 32
        Height = 13
        Caption = 'Label7'
      end
      object Label8: TLabel
        Left = 143
        Top = 429
        Width = 32
        Height = 13
        Caption = 'Label8'
      end
      object Label3: TLabel
        Left = 143
        Top = 315
        Width = 32
        Height = 13
        Caption = 'Label3'
      end
      object Label4: TLabel
        Left = 143
        Top = 351
        Width = 32
        Height = 13
        Caption = 'Label4'
      end
      object Label5: TLabel
        Left = 3
        Top = 120
        Width = 76
        Height = 13
        Caption = 'Weld pass color'
      end
      object Shape1: TShape
        Left = 93
        Top = 115
        Width = 45
        Height = 34
        Shape = stRoundRect
      end
      object Label6: TLabel
        Left = 0
        Top = 40
        Width = 123
        Height = 13
        Caption = 'Choose Weld Material Set'
      end
      object RadioGroup2: TRadioGroup
        Left = 0
        Top = 295
        Width = 137
        Height = 161
        Caption = 'Weld pass/plate location'
        Items.Strings = (
          'First item'
          'Second'
          'Third'
          'Fourth')
        TabOrder = 0
      end
      object CheckBox1: TCheckBox
        Left = 8
        Top = 8
        Width = 297
        Height = 17
        Caption = 'Check if weld direction is start-to-stop; uncheck for reverse'
        TabOrder = 1
      end
      object Button4: TButton
        Left = 156
        Top = 115
        Width = 89
        Height = 25
        Caption = 'Change color...'
        TabOrder = 2
        OnClick = Button4Click
      end
      object ListBox1: TListBox
        Left = 156
        Top = 40
        Width = 183
        Height = 57
        ItemHeight = 13
        TabOrder = 3
        OnClick = ListBox1Click
      end
      object RadioGroup1: TRadioGroup
        Left = 3
        Top = 155
        Width = 185
        Height = 105
        Caption = 'RadioGroup1'
        Items.Strings = (
          'To End Run/Part Section'
          'Partial Length/Part Section'
          'To End Run/Full Section'
          'Partial Length/Full Section')
        TabOrder = 4
        OnClick = RadioGroup1Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Welding Parameters'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label9: TLabel
        Left = 3
        Top = 8
        Width = 134
        Height = 13
        Caption = 'Choose Weld Parameter Set'
      end
      object Panel1: TPanel
        Left = 3
        Top = 104
        Width = 281
        Height = 113
        TabOrder = 0
        object Label10: TLabel
          Left = 16
          Top = 8
          Width = 109
          Height = 13
          Caption = 'Welding Current (Amp):'
        end
        object Label11: TLabel
          Left = 16
          Top = 32
          Width = 108
          Height = 13
          Caption = 'Welding Voltage (Volt):'
        end
        object Label12: TLabel
          Left = 16
          Top = 56
          Width = 68
          Height = 13
          Caption = 'Arc Efficiency:'
        end
        object Label13: TLabel
          Left = 16
          Top = 80
          Width = 112
          Height = 13
          Caption = 'Torch Traveling Speed:'
        end
        object Edit9: TEdit
          Left = 136
          Top = 8
          Width = 121
          Height = 21
          TabOrder = 0
          Text = 'Edit9'
        end
        object Edit10: TEdit
          Left = 136
          Top = 32
          Width = 121
          Height = 21
          TabOrder = 1
          Text = 'Edit10'
        end
        object Edit11: TEdit
          Left = 136
          Top = 56
          Width = 121
          Height = 21
          TabOrder = 2
          Text = 'Edit11'
        end
        object Edit12: TEdit
          Left = 136
          Top = 80
          Width = 121
          Height = 21
          TabOrder = 3
          Text = 'Edit12'
        end
      end
      object GroupBox1: TGroupBox
        Left = 3
        Top = 241
        Width = 281
        Height = 121
        Caption = 'Temperature Control'
        Color = clYellow
        ParentBackground = False
        ParentColor = False
        TabOrder = 1
        object Label14: TLabel
          Left = 8
          Top = 16
          Width = 94
          Height = 13
          Caption = 'Room Temperature:'
        end
        object Label15: TLabel
          Left = 8
          Top = 40
          Width = 100
          Height = 13
          Caption = 'Melting Temperature:'
        end
        object Label16: TLabel
          Left = 8
          Top = 64
          Width = 105
          Height = 13
          Caption = 'Low-Cut Temperature:'
        end
        object Label17: TLabel
          Left = 8
          Top = 88
          Width = 103
          Height = 13
          Caption = 'Preheat Temperature:'
        end
        object Edit13: TEdit
          Left = 136
          Top = 16
          Width = 121
          Height = 21
          TabOrder = 0
          Text = 'Edit13'
        end
        object Edit14: TEdit
          Left = 136
          Top = 40
          Width = 121
          Height = 21
          TabOrder = 1
          Text = 'Edit14'
        end
        object Edit15: TEdit
          Left = 136
          Top = 64
          Width = 121
          Height = 21
          TabOrder = 2
          Text = 'Edit15'
        end
        object Edit16: TEdit
          Left = 136
          Top = 88
          Width = 121
          Height = 21
          TabOrder = 3
          Text = 'Edit16'
        end
      end
      object ListBox2: TListBox
        Left = 153
        Top = 8
        Width = 167
        Height = 74
        ItemHeight = 13
        TabOrder = 2
        OnClick = ListBox2Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'CTSP Inputs'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox2: TGroupBox
        Left = 0
        Top = 8
        Width = 361
        Height = 73
        Caption = 'Weld Joint Type'
        TabOrder = 0
        object Label18: TLabel
          Left = 8
          Top = 16
          Width = 52
          Height = 13
          Caption = 'Joint Type:'
        end
        object Label19: TLabel
          Left = 184
          Top = 16
          Width = 59
          Height = 13
          Caption = 'Joint Shape:'
        end
        object ComboBox1: TComboBox
          Left = 16
          Top = 40
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 4
          TabOrder = 0
          Items.Strings = (
            'T-Fillet'
            'V-Groove'
            'Lap Joint'
            'Box Type')
        end
        object ComboBox2: TComboBox
          Left = 208
          Top = 40
          Width = 145
          Height = 21
          Style = csDropDownList
          TabOrder = 1
          Items.Strings = (
            'Non-Circular'
            'Full Circle'
            'Partial Circle'
            'Girth (Full Circle)'
            'Girth (Partial Circle)')
        end
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 88
        Width = 393
        Height = 81
        Caption = 'Time Step Control'
        TabOrder = 1
        object Label20: TLabel
          Left = 8
          Top = 16
          Width = 111
          Height = 13
          Caption = 'InterPass Cooling Time:'
        end
        object Label21: TLabel
          Left = 208
          Top = 16
          Width = 115
          Height = 13
          Caption = 'InterPass Cooling Steps:'
        end
        object Label22: TLabel
          Left = 8
          Top = 48
          Width = 130
          Height = 13
          Caption = 'Max Heating Time (Output):'
        end
        object Label23: TLabel
          Left = 208
          Top = 48
          Width = 127
          Height = 13
          Caption = 'Min Heating Time (Output):'
        end
        object Edit17: TEdit
          Left = 144
          Top = 21
          Width = 49
          Height = 21
          TabOrder = 0
          Text = 'Edit17'
        end
        object Edit18: TEdit
          Left = 144
          Top = 48
          Width = 49
          Height = 21
          TabOrder = 1
          Text = 'Edit18'
        end
        object Edit19: TEdit
          Left = 336
          Top = 16
          Width = 41
          Height = 21
          TabOrder = 2
          Text = 'Edit19'
        end
        object Edit20: TEdit
          Left = 336
          Top = 48
          Width = 41
          Height = 21
          TabOrder = 3
          Text = 'Edit20'
        end
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 176
        Width = 113
        Height = 73
        Caption = 'Plate Thickness'
        Color = clYellow
        ParentBackground = False
        ParentColor = False
        TabOrder = 2
        object Label24: TLabel
          Left = 8
          Top = 16
          Width = 36
          Height = 13
          Caption = 'Plate 1:'
        end
        object Label25: TLabel
          Left = 8
          Top = 40
          Width = 36
          Height = 13
          Caption = 'Plate 2:'
        end
        object Edit21: TEdit
          Left = 48
          Top = 16
          Width = 57
          Height = 21
          TabOrder = 0
          Text = 'Edit21'
        end
        object Edit22: TEdit
          Left = 48
          Top = 40
          Width = 57
          Height = 21
          TabOrder = 1
          Text = 'Edit22'
        end
      end
      object GroupBox5: TGroupBox
        Left = 0
        Top = 256
        Width = 393
        Height = 105
        Caption = 'Heating Procedure'
        TabOrder = 3
        object Label26: TLabel
          Left = 160
          Top = 40
          Width = 143
          Height = 13
          Caption = 'Number of Lumped Segments:'
        end
        object RadioButton5: TRadioButton
          Left = 8
          Top = 16
          Width = 113
          Height = 17
          Caption = 'Moving Arc'
          TabOrder = 0
        end
        object RadioButton6: TRadioButton
          Left = 160
          Top = 16
          Width = 113
          Height = 17
          Caption = 'Lumped Pass'
          TabOrder = 1
        end
        object Edit23: TEdit
          Left = 304
          Top = 40
          Width = 57
          Height = 21
          TabOrder = 2
          Text = 'Edit23'
        end
        object CheckBox3: TCheckBox
          Left = 160
          Top = 72
          Width = 185
          Height = 17
          Caption = 'Merge Current Pass with Previous'
          TabOrder = 3
        end
      end
      object GroupBox6: TGroupBox
        Left = 0
        Top = 368
        Width = 233
        Height = 105
        Caption = 'Near-Edge Effect'
        TabOrder = 4
        object Label27: TLabel
          Left = 8
          Top = 48
          Width = 131
          Height = 13
          Caption = 'Number of Required Edges:'
        end
        object CheckBox4: TCheckBox
          Left = 8
          Top = 24
          Width = 145
          Height = 17
          Caption = 'Near-Edge Effect Toggle'
          TabOrder = 0
        end
        object Edit24: TEdit
          Left = 144
          Top = 48
          Width = 49
          Height = 21
          TabOrder = 1
          Text = 'Edit24'
        end
        object RadioButton7: TRadioButton
          Left = 8
          Top = 80
          Width = 137
          Height = 17
          Caption = 'Edge Surface Normal'
          TabOrder = 2
        end
      end
    end
  end
  object ColorDialog1: TColorDialog
    Left = 376
  end
end
