object Form8: TForm8
  Left = 486
  Top = 16
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Deformed shape'
  ClientHeight = 49
  ClientWidth = 155
  Color = clPurple
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Edit1: TEdit
    Left = 8
    Top = 0
    Width = 105
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Button1: TButton
    Left = 8
    Top = 24
    Width = 41
    Height = 17
    Caption = 'Enter'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 56
    Top = 24
    Width = 41
    Height = 17
    Caption = 'Reset'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 104
    Top = 24
    Width = 49
    Height = 17
    Caption = 'Button3'
    TabOrder = 3
    OnClick = Button3Click
  end
end
