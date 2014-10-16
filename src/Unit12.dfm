object Form12: TForm12
  Left = 19
  Top = 354
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'SectionByRot'
  ClientHeight = 143
  ClientWidth = 239
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 112
    Top = 0
    Width = 21
    Height = 13
    Caption = '-180'
  end
  object Label2: TLabel
    Left = 216
    Top = 0
    Width = 18
    Height = 13
    Caption = '180'
  end
  object Label3: TLabel
    Left = 0
    Top = 72
    Width = 34
    Height = 13
    Caption = 'Search'
  end
  object Label4: TLabel
    Left = 0
    Top = 0
    Width = 58
    Height = 13
    Caption = 'Longit.angle'
  end
  object Label5: TLabel
    Left = 112
    Top = 24
    Width = 21
    Height = 13
    Caption = '-180'
  end
  object Label6: TLabel
    Left = 216
    Top = 24
    Width = 18
    Height = 13
    Caption = '180'
  end
  object Label7: TLabel
    Left = 0
    Top = 24
    Width = 55
    Height = 13
    Caption = 'Latitud.angl'
  end
  object Label8: TLabel
    Left = 160
    Top = 48
    Width = 18
    Height = 13
    Caption = 'Pt x'
  end
  object Label9: TLabel
    Left = 160
    Top = 72
    Width = 18
    Height = 13
    Caption = 'Pt y'
  end
  object Label10: TLabel
    Left = 160
    Top = 96
    Width = 18
    Height = 13
    Caption = 'Pt z'
  end
  object Button1: TButton
    Left = 8
    Top = 96
    Width = 57
    Height = 17
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 104
    Top = 120
    Width = 65
    Height = 17
    Caption = 'Continue'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 80
    Top = 96
    Width = 57
    Height = 17
    Caption = 'Reset'
    TabOrder = 2
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 72
    Top = 72
    Width = 33
    Height = 17
    Caption = 'Max'
    TabOrder = 3
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 40
    Top = 72
    Width = 33
    Height = 17
    Caption = 'Min'
    TabOrder = 4
    OnClick = Button5Click
  end
  object RadioButton1: TRadioButton
    Left = 0
    Top = 48
    Width = 41
    Height = 17
    Caption = 'Left'
    TabOrder = 5
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 48
    Top = 48
    Width = 49
    Height = 17
    Caption = 'Right'
    TabOrder = 6
    OnClick = RadioButton2Click
  end
  object Edit1: TEdit
    Left = 56
    Top = 0
    Width = 41
    Height = 21
    TabOrder = 7
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 56
    Top = 24
    Width = 41
    Height = 21
    TabOrder = 8
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 184
    Top = 48
    Width = 49
    Height = 21
    TabOrder = 9
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 184
    Top = 72
    Width = 49
    Height = 21
    TabOrder = 10
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 184
    Top = 96
    Width = 49
    Height = 21
    TabOrder = 11
    Text = 'Edit5'
  end
  object ScrollBar1: TScrollBar
    Left = 136
    Top = 0
    Width = 81
    Height = 16
    PageSize = 0
    TabOrder = 12
    OnScroll = ScrollBar1Scroll
  end
  object ScrollBar2: TScrollBar
    Left = 136
    Top = 24
    Width = 81
    Height = 16
    PageSize = 0
    TabOrder = 13
    OnScroll = ScrollBar2Scroll
  end
  object Button6: TButton
    Left = 8
    Top = 120
    Width = 75
    Height = 17
    Caption = 'Close'
    TabOrder = 14
    OnClick = Button6Click
  end
end
