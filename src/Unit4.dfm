object Form4: TForm4
  Left = 75
  Top = 537
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Incremental Rotation (degrees)'
  ClientHeight = 95
  ClientWidth = 307
  Color = clGreen
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
    Left = 68
    Top = 0
    Width = 55
    Height = 13
    Caption = 'X-axis  -180'
  end
  object Label2: TLabel
    Left = 224
    Top = 0
    Width = 18
    Height = 13
    Caption = '180'
  end
  object Label3: TLabel
    Left = 68
    Top = 29
    Width = 55
    Height = 13
    Caption = 'Y-axis  -180'
  end
  object Label4: TLabel
    Left = 224
    Top = 24
    Width = 18
    Height = 13
    Caption = '180'
  end
  object Label5: TLabel
    Left = 68
    Top = 54
    Width = 55
    Height = 13
    Caption = 'Z-axis  -180'
  end
  object Label6: TLabel
    Left = 224
    Top = 53
    Width = 18
    Height = 13
    Caption = '180'
  end
  object ScrollBar1: TScrollBar
    Left = 129
    Top = 2
    Width = 89
    Height = 16
    PageSize = 0
    TabOrder = 0
    OnScroll = ScrollBar1Scroll
  end
  object Edit1: TEdit
    Left = 256
    Top = -3
    Width = 41
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object ScrollBar2: TScrollBar
    Left = 129
    Top = 28
    Width = 89
    Height = 16
    PageSize = 0
    TabOrder = 2
    OnScroll = ScrollBar2Scroll
  end
  object Edit2: TEdit
    Left = 256
    Top = 24
    Width = 41
    Height = 21
    TabOrder = 3
    Text = 'Edit2'
  end
  object ScrollBar3: TScrollBar
    Left = 129
    Top = 50
    Width = 89
    Height = 16
    PageSize = 0
    TabOrder = 4
    OnScroll = ScrollBar3Scroll
  end
  object Edit3: TEdit
    Left = 256
    Top = 51
    Width = 41
    Height = 21
    TabOrder = 5
    Text = 'Edit3'
  end
  object Button1: TButton
    Left = 8
    Top = 72
    Width = 41
    Height = 17
    Caption = 'OK'
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 68
    Top = 73
    Width = 33
    Height = 17
    Caption = 'Undo'
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 112
    Top = 73
    Width = 41
    Height = 17
    Caption = 'Reset'
    TabOrder = 8
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 175
    Top = 72
    Width = 34
    Height = 17
    Caption = 'Close'
    TabOrder = 9
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 8
    Top = 8
    Width = 25
    Height = 17
    Caption = 'XY'
    TabOrder = 10
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 39
    Top = 8
    Width = 25
    Height = 17
    Caption = '-XY'
    TabOrder = 11
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 8
    Top = 31
    Width = 25
    Height = 17
    Caption = 'YZ'
    TabOrder = 12
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 39
    Top = 31
    Width = 26
    Height = 17
    Caption = '-YZ'
    TabOrder = 13
    OnClick = Button8Click
  end
  object Button9: TButton
    Left = 8
    Top = 54
    Width = 25
    Height = 17
    Caption = 'ZX'
    TabOrder = 14
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 39
    Top = 54
    Width = 25
    Height = 17
    Caption = '-ZX'
    TabOrder = 15
    OnClick = Button10Click
  end
end
