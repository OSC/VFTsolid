object Form11: TForm11
  Left = 979
  Top = 117
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Section - 3 point'
  ClientHeight = 129
  ClientWidth = 292
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
    Left = 120
    Top = 0
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 256
    Top = 0
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object Label3: TLabel
    Left = 120
    Top = 80
    Width = 34
    Height = 13
    Caption = 'Search'
  end
  object Label4: TLabel
    Left = 0
    Top = 0
    Width = 38
    Height = 13
    Caption = 'Plane D'
  end
  object Label5: TLabel
    Left = 0
    Top = 24
    Width = 31
    Height = 13
    Caption = 'Pt#1 x'
  end
  object Label6: TLabel
    Left = 0
    Top = 40
    Width = 31
    Height = 13
    Caption = 'Pt#1 y'
  end
  object Label7: TLabel
    Left = 0
    Top = 56
    Width = 31
    Height = 13
    Caption = 'Pt#1 z'
  end
  object Label8: TLabel
    Left = 96
    Top = 24
    Width = 31
    Height = 13
    Caption = 'Pt#2 x'
  end
  object Label9: TLabel
    Left = 96
    Top = 40
    Width = 31
    Height = 13
    Caption = 'Pt#2 y'
  end
  object Label10: TLabel
    Left = 96
    Top = 56
    Width = 31
    Height = 13
    Caption = 'Pt#2 z'
  end
  object Label11: TLabel
    Left = 200
    Top = 24
    Width = 31
    Height = 13
    Caption = 'Pt#3 x'
  end
  object Label12: TLabel
    Left = 200
    Top = 40
    Width = 31
    Height = 13
    Caption = 'Pt#3 y'
  end
  object Label13: TLabel
    Left = 200
    Top = 56
    Width = 31
    Height = 13
    Caption = 'Pt#3 z'
  end
  object Button1: TButton
    Left = 8
    Top = 104
    Width = 41
    Height = 17
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 120
    Top = 104
    Width = 75
    Height = 17
    Caption = 'Continuous'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 64
    Top = 104
    Width = 41
    Height = 17
    Caption = 'Reset'
    TabOrder = 2
  end
  object Button4: TButton
    Left = 200
    Top = 80
    Width = 49
    Height = 17
    Caption = 'Max'
    TabOrder = 3
  end
  object Button5: TButton
    Left = 160
    Top = 80
    Width = 41
    Height = 17
    Caption = 'Min'
    TabOrder = 4
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 80
    Width = 41
    Height = 17
    Caption = 'Left'
    TabOrder = 5
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 56
    Top = 80
    Width = 49
    Height = 17
    Caption = 'Right'
    TabOrder = 6
    OnClick = RadioButton2Click
  end
  object ScrollBar1: TScrollBar
    Left = 152
    Top = 0
    Width = 97
    Height = 16
    PageSize = 0
    TabOrder = 7
  end
  object Edit1: TEdit
    Left = 40
    Top = 0
    Width = 57
    Height = 21
    TabOrder = 8
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 32
    Top = 24
    Width = 57
    Height = 21
    TabOrder = 9
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 32
    Top = 40
    Width = 57
    Height = 21
    TabOrder = 10
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 32
    Top = 56
    Width = 57
    Height = 21
    TabOrder = 11
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 128
    Top = 24
    Width = 65
    Height = 21
    TabOrder = 12
    Text = 'Edit5'
  end
  object Edit6: TEdit
    Left = 128
    Top = 40
    Width = 65
    Height = 21
    TabOrder = 13
    Text = 'Edit6'
  end
  object Edit7: TEdit
    Left = 128
    Top = 56
    Width = 65
    Height = 21
    TabOrder = 14
    Text = 'Edit7'
  end
  object Edit8: TEdit
    Left = 232
    Top = 24
    Width = 57
    Height = 21
    TabOrder = 15
    Text = 'Edit8'
  end
  object Edit9: TEdit
    Left = 232
    Top = 40
    Width = 57
    Height = 21
    TabOrder = 16
    Text = 'Edit9'
  end
  object Edit10: TEdit
    Left = 232
    Top = 56
    Width = 57
    Height = 21
    TabOrder = 17
    Text = 'Edit10'
  end
  object Button6: TButton
    Left = 208
    Top = 104
    Width = 67
    Height = 17
    Caption = 'Close'
    TabOrder = 18
    OnClick = Button6Click
  end
end
