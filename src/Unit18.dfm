object Form18: TForm18
  Left = 3
  Top = 50
  Width = 124
  Height = 218
  BorderIcons = []
  Caption = 'RotateFix'
  Color = clRed
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
    Left = 16
    Top = 0
    Width = 7
    Height = 13
    Caption = 'X'
  end
  object Label2: TLabel
    Left = 40
    Top = 0
    Width = 7
    Height = 13
    Caption = 'Y'
  end
  object Label3: TLabel
    Left = 72
    Top = 0
    Width = 7
    Height = 13
    Caption = 'Z'
  end
  object ListBox1: TListBox
    Left = 0
    Top = 16
    Width = 25
    Height = 113
    ItemHeight = 13
    TabOrder = 0
  end
  object ListBox2: TListBox
    Left = 32
    Top = 16
    Width = 25
    Height = 113
    ItemHeight = 13
    TabOrder = 1
  end
  object ListBox3: TListBox
    Left = 64
    Top = 16
    Width = 25
    Height = 113
    ItemHeight = 13
    TabOrder = 2
  end
  object Button1: TButton
    Left = 8
    Top = 136
    Width = 73
    Height = 17
    Caption = 'Rotate'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 160
    Width = 75
    Height = 17
    Caption = 'Close'
    TabOrder = 4
    OnClick = Button2Click
  end
end
