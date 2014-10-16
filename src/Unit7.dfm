object Form7: TForm7
  Left = 1
  Top = 478
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Section'
  ClientHeight = 76
  ClientWidth = 244
  Color = clFuchsia
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
    Left = 8
    Top = 0
    Width = 17
    Height = 13
    Caption = 'Min'
  end
  object Label2: TLabel
    Left = 152
    Top = 0
    Width = 20
    Height = 13
    Caption = 'Max'
  end
  object Label3: TLabel
    Left = 128
    Top = 24
    Width = 34
    Height = 13
    Caption = 'Search'
  end
  object ScrollBar1: TScrollBar
    Left = 48
    Top = 0
    Width = 97
    Height = 16
    PageSize = 0
    TabOrder = 0
  end
  object Edit1: TEdit
    Left = 192
    Top = 0
    Width = 49
    Height = 21
    TabOrder = 1
    Text = 'Edit1'
  end
  object Button1: TButton
    Left = 8
    Top = 48
    Width = 41
    Height = 17
    Caption = 'OK'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 112
    Top = 48
    Width = 65
    Height = 17
    Caption = 'Continuous'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 56
    Top = 48
    Width = 49
    Height = 17
    Caption = 'Reset'
    TabOrder = 4
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 208
    Top = 24
    Width = 33
    Height = 17
    Caption = 'Max'
    TabOrder = 5
    OnClick = Button4Click
  end
  object Button7: TButton
    Left = 168
    Top = 24
    Width = 41
    Height = 17
    Caption = 'Min'
    TabOrder = 6
    OnClick = Button7Click
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 24
    Width = 41
    Height = 17
    Caption = 'Left'
    TabOrder = 7
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 56
    Top = 24
    Width = 49
    Height = 17
    Caption = 'Right'
    TabOrder = 8
    OnClick = RadioButton2Click
  end
  object Button5: TButton
    Left = 184
    Top = 48
    Width = 59
    Height = 17
    Caption = 'Close'
    TabOrder = 9
    OnClick = Button5Click
  end
end
