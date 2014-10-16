object Form17: TForm17
  Left = 3
  Top = 165
  BorderIcons = []
  Caption = 'GID'
  ClientHeight = 219
  ClientWidth = 115
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object CheckListBox1: TCheckListBox
    Left = 0
    Top = 0
    Width = 89
    Height = 161
    ItemHeight = 13
    TabOrder = 0
  end
  object Button1: TButton
    Left = 8
    Top = 168
    Width = 81
    Height = 17
    Caption = 'Redraw'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 192
    Width = 81
    Height = 17
    Caption = 'Close'
    TabOrder = 2
    OnClick = Button2Click
  end
end
