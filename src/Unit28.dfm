object Form28: TForm28
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Form28'
  ClientHeight = 68
  ClientWidth = 203
  Color = clLime
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 26
    Height = 13
    Caption = 'Value'
  end
  object Edit1: TEdit
    Left = 71
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 0
    Text = 'Edit1'
  end
  object Button1: TButton
    Left = 8
    Top = 35
    Width = 57
    Height = 25
    Caption = 'Enter'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 71
    Top = 35
    Width = 58
    Height = 25
    Caption = 'Reset'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 135
    Top = 35
    Width = 58
    Height = 25
    Caption = 'Close'
    TabOrder = 3
    OnClick = Button3Click
  end
end
