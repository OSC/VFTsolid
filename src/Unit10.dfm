object Form10: TForm10
  Left = 0
  Top = 198
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'General axis'
  ClientHeight = 134
  ClientWidth = 115
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
    Width = 30
    Height = 13
    Caption = 'Origin:'
  end
  object Label2: TLabel
    Left = 0
    Top = 56
    Width = 32
    Height = 13
    Caption = 'Z* axis'
  end
  object Label3: TLabel
    Left = 0
    Top = 72
    Width = 26
    Height = 13
    Caption = 'point:'
  end
  object Edit1: TEdit
    Left = 40
    Top = 0
    Width = 49
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 40
    Top = 16
    Width = 49
    Height = 21
    TabOrder = 1
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 40
    Top = 32
    Width = 49
    Height = 21
    TabOrder = 2
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 40
    Top = 56
    Width = 49
    Height = 21
    TabOrder = 3
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 40
    Top = 72
    Width = 49
    Height = 21
    TabOrder = 4
    Text = 'Edit5'
  end
  object Edit6: TEdit
    Left = 40
    Top = 88
    Width = 49
    Height = 21
    TabOrder = 5
    Text = 'Edit6'
  end
  object Button1: TButton
    Left = 24
    Top = 112
    Width = 73
    Height = 17
    Caption = 'Enter'
    TabOrder = 6
    OnClick = Button1Click
  end
end
