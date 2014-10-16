object Form16: TForm16
  Left = 340
  Top = 48
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'FE stats'
  ClientHeight = 116
  ClientWidth = 124
  Color = clOlive
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
    Left = 0
    Top = 0
    Width = 31
    Height = 13
    Caption = 'Nodes'
  end
  object Label2: TLabel
    Left = 0
    Top = 16
    Width = 43
    Height = 13
    Caption = 'Elements'
  end
  object Label3: TLabel
    Left = 0
    Top = 48
    Width = 32
    Height = 13
    Caption = 'Label3'
  end
  object Label4: TLabel
    Left = 0
    Top = 67
    Width = 32
    Height = 13
    Caption = 'Label4'
  end
  object Edit1: TEdit
    Left = 64
    Top = -3
    Width = 57
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 64
    Top = 16
    Width = 57
    Height = 21
    TabOrder = 1
    Text = 'Edit2'
  end
  object Button1: TButton
    Left = 16
    Top = 93
    Width = 75
    Height = 17
    Caption = 'Close'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Edit3: TEdit
    Left = 64
    Top = 43
    Width = 57
    Height = 21
    TabOrder = 3
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 64
    Top = 66
    Width = 57
    Height = 21
    TabOrder = 4
    Text = 'Edit4'
  end
end
