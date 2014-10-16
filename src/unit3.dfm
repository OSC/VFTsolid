object Form3: TForm3
  Left = 32
  Top = 29
  Hint = 'Legend max/min'
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Custom legend'
  ClientHeight = 67
  ClientWidth = 132
  Color = clTeal
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 43
    Height = 13
    Caption = 'Leg_max'
  end
  object Label2: TLabel
    Left = 0
    Top = 16
    Width = 40
    Height = 13
    Caption = 'Leg_min'
  end
  object Button1: TButton
    Left = 0
    Top = 48
    Width = 65
    Height = 17
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 48
    Top = 0
    Width = 73
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 48
    Top = 16
    Width = 73
    Height = 21
    TabOrder = 2
    Text = 'Edit2'
  end
  object Button2: TButton
    Left = 72
    Top = 48
    Width = 57
    Height = 17
    Caption = 'Close'
    TabOrder = 3
    OnClick = Button2Click
  end
end
