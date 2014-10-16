object Form9: TForm9
  Left = 6
  Top = 453
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Wedge section'
  ClientHeight = 114
  ClientWidth = 279
  Color = clBlue
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
    Width = 32
    Height = 13
    Caption = 'Label1'
  end
  object Label2: TLabel
    Left = 160
    Top = 0
    Width = 32
    Height = 13
    Caption = 'Label2'
  end
  object Label3: TLabel
    Left = 8
    Top = 24
    Width = 32
    Height = 13
    Caption = 'Label3'
  end
  object Label4: TLabel
    Left = 160
    Top = 24
    Width = 32
    Height = 13
    Caption = 'Label4'
  end
  object Label5: TLabel
    Left = 8
    Top = 48
    Width = 32
    Height = 13
    Caption = 'Label5'
  end
  object Label6: TLabel
    Left = 160
    Top = 48
    Width = 32
    Height = 13
    Caption = 'Label6'
  end
  object Label7: TLabel
    Left = 168
    Top = 72
    Width = 34
    Height = 13
    Caption = 'Search'
  end
  object ScrollBar1: TScrollBar
    Left = 48
    Top = 0
    Width = 105
    Height = 16
    PageSize = 0
    TabOrder = 0
  end
  object ScrollBar2: TScrollBar
    Left = 48
    Top = 24
    Width = 105
    Height = 16
    PageSize = 0
    TabOrder = 1
  end
  object ScrollBar3: TScrollBar
    Left = 48
    Top = 48
    Width = 105
    Height = 16
    PageSize = 0
    TabOrder = 2
  end
  object Edit1: TEdit
    Left = 208
    Top = 0
    Width = 65
    Height = 21
    TabOrder = 3
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 208
    Top = 24
    Width = 65
    Height = 21
    TabOrder = 4
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 208
    Top = 48
    Width = 65
    Height = 21
    TabOrder = 5
    Text = 'Edit3'
  end
  object Button1: TButton
    Left = 16
    Top = 72
    Width = 41
    Height = 17
    Caption = 'OK'
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 112
    Top = 96
    Width = 57
    Height = 17
    Caption = 'Continuous'
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 96
    Top = 72
    Width = 41
    Height = 17
    Caption = 'Reset'
    TabOrder = 8
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 208
    Top = 72
    Width = 33
    Height = 17
    Caption = 'Min'
    TabOrder = 9
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 240
    Top = 72
    Width = 33
    Height = 17
    Caption = 'Max'
    TabOrder = 10
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 16
    Top = 96
    Width = 75
    Height = 17
    Caption = 'Close'
    TabOrder = 11
    OnClick = Button6Click
  end
end
