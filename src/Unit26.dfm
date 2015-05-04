object Form26: TForm26
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Form26'
  ClientHeight = 206
  ClientWidth = 141
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ListBox1: TListBox
    Left = 8
    Top = 8
    Width = 121
    Height = 153
    ItemHeight = 13
    TabOrder = 0
  end
  object Button1: TButton
    Left = 8
    Top = 176
    Width = 58
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 72
    Top = 176
    Width = 57
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
    OnClick = Button2Click
  end
end
