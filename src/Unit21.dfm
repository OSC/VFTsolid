object Form21: TForm21
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Form21'
  ClientHeight = 520
  ClientWidth = 457
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 31
    Height = 13
    Caption = 'Label1'
  end
  object Label6: TLabel
    Left = 176
    Top = 207
    Width = 31
    Height = 13
    Caption = 'Label6'
  end
  object Edit1: TEdit
    Left = 216
    Top = 5
    Width = 161
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 32
    Width = 441
    Height = 97
    Caption = 'GroupBox1'
    TabOrder = 1
    object Label2: TLabel
      Left = 16
      Top = 26
      Width = 31
      Height = 13
      Caption = 'Label2'
    end
    object Label3: TLabel
      Left = 16
      Top = 45
      Width = 31
      Height = 13
      Caption = 'Label3'
    end
    object Label4: TLabel
      Left = 16
      Top = 64
      Width = 31
      Height = 13
      Caption = 'Label4'
    end
    object Edit2: TEdit
      Left = 208
      Top = 16
      Width = 161
      Height = 21
      TabOrder = 0
      Text = 'Edit2'
    end
    object Edit3: TEdit
      Left = 208
      Top = 40
      Width = 161
      Height = 21
      TabOrder = 1
      Text = 'Edit3'
    end
    object Edit4: TEdit
      Left = 208
      Top = 64
      Width = 161
      Height = 21
      TabOrder = 2
      Text = 'Edit4'
    end
  end
  object RadioGroup1: TRadioGroup
    Left = 8
    Top = 135
    Width = 441
    Height = 106
    Caption = 'RadioGroup1'
    TabOrder = 2
  end
  object RadioButton1: TRadioButton
    Left = 24
    Top = 152
    Width = 153
    Height = 17
    Caption = 'RadioButton1'
    TabOrder = 3
    OnClick = RadioButton1Click
    OnDblClick = Bananas
  end
  object RadioButton2: TRadioButton
    Left = 24
    Top = 176
    Width = 153
    Height = 17
    Caption = 'RadioButton2'
    TabOrder = 4
    OnClick = RadioButton2Click
  end
  object RadioButton3: TRadioButton
    Left = 24
    Top = 200
    Width = 153
    Height = 17
    Caption = 'RadioButton3'
    TabOrder = 5
    OnClick = RadioButton3Click
  end
  object RadioButton4: TRadioButton
    Left = 240
    Top = 152
    Width = 177
    Height = 17
    Caption = 'RadioButton4'
    TabOrder = 6
    OnClick = RadioButton4Click
  end
  object RadioButton5: TRadioButton
    Left = 240
    Top = 175
    Width = 177
    Height = 17
    Caption = 'RadioButton5'
    TabOrder = 7
    OnClick = RadioButton5Click
    OnDblClick = Plantains
  end
  object RadioButton6: TRadioButton
    Left = 240
    Top = 198
    Width = 177
    Height = 17
    Caption = 'RadioButton6'
    TabOrder = 8
    OnClick = RadioButton6Click
  end
  object CheckBox1: TCheckBox
    Left = 168
    Top = 160
    Width = 241
    Height = 17
    Caption = 'CheckBox1'
    TabOrder = 9
    OnClick = CheckBox1Click
  end
  object Edit5: TEdit
    Left = 56
    Top = 207
    Width = 106
    Height = 21
    TabOrder = 10
    Text = 'Edit5'
  end
  object Panel1: TPanel
    Left = 8
    Top = 247
    Width = 441
    Height = 146
    TabOrder = 11
    object Label5: TLabel
      Left = 288
      Top = 8
      Width = 31
      Height = 13
      Caption = 'Label5'
    end
    object Label7: TLabel
      Left = 184
      Top = 56
      Width = 31
      Height = 13
      Caption = 'Label7'
    end
    object Label8: TLabel
      Left = 184
      Top = 83
      Width = 31
      Height = 13
      Caption = 'Label8'
    end
    object Label9: TLabel
      Left = 184
      Top = 112
      Width = 31
      Height = 13
      Caption = 'Label9'
    end
    object GroupBox2: TGroupBox
      Left = 16
      Top = 8
      Width = 138
      Height = 57
      Caption = 'GroupBox2'
      TabOrder = 0
      object RadioButton9: TRadioButton
        Left = 8
        Top = 17
        Width = 113
        Height = 17
        Caption = 'RadioButton9'
        TabOrder = 0
        OnClick = RadioButton9Click
      end
      object RadioButton10: TRadioButton
        Left = 8
        Top = 37
        Width = 113
        Height = 17
        Caption = 'RadioButton10'
        TabOrder = 1
        OnClick = RadioButton10Click
      end
    end
    object GroupBox3: TGroupBox
      Left = 16
      Top = 71
      Width = 138
      Height = 65
      Caption = 'GroupBox3'
      TabOrder = 1
      object RadioButton7: TRadioButton
        Left = 8
        Top = 16
        Width = 113
        Height = 17
        Caption = 'RadioButton7'
        TabOrder = 0
      end
      object RadioButton8: TRadioButton
        Left = 8
        Top = 39
        Width = 113
        Height = 17
        Caption = 'RadioButton8'
        TabOrder = 1
      end
    end
    object Edit6: TEdit
      Left = 352
      Top = 56
      Width = 73
      Height = 21
      TabOrder = 2
      Text = 'Edit6'
    end
    object Edit7: TEdit
      Left = 352
      Top = 83
      Width = 73
      Height = 21
      TabOrder = 3
      Text = 'Edit7'
    end
    object Edit8: TEdit
      Left = 352
      Top = 110
      Width = 73
      Height = 21
      TabOrder = 4
      Text = 'Edit8'
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 399
    Width = 441
    Height = 82
    Caption = 'GroupBox4'
    TabOrder = 12
    object Label10: TLabel
      Left = 16
      Top = 24
      Width = 37
      Height = 13
      Caption = 'Label10'
    end
    object Edit9: TEdit
      Left = 200
      Top = 16
      Width = 225
      Height = 21
      TabOrder = 0
      Text = 'Edit9'
    end
    object Button1: TButton
      Left = 24
      Top = 48
      Width = 385
      Height = 25
      Caption = 'Button1'
      TabOrder = 1
      OnClick = Button1Click
    end
  end
  object Button2: TButton
    Left = 24
    Top = 487
    Width = 113
    Height = 25
    Caption = 'Button2'
    TabOrder = 13
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 304
    Top = 488
    Width = 129
    Height = 25
    Caption = 'Button3'
    TabOrder = 14
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 56
    Top = 176
    Width = 130
    Height = 25
    Caption = 'Cancel Iso/Creep steps'
    TabOrder = 15
    OnClick = Button4Click
  end
  object OpenDialog1: TOpenDialog
    Left = 200
    Top = 488
  end
end
