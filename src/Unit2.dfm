object Form2: TForm2
  Left = 644
  Top = 7
  HorzScrollBar.Visible = False
  BorderIcons = []
  Caption = 'ResultSelector'
  ClientHeight = 255
  ClientWidth = 139
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
  object ListBox1: TListBox
    Left = 0
    Top = 0
    Width = 139
    Height = 255
    Hint = 'ResultSelector'
    Align = alClient
    ItemHeight = 13
    TabOrder = 0
    OnClick = ListBox1ClickExecute
  end
  object ActionList1: TActionList
    Left = 328
    Top = 216
    object ListBox1Click: TAction
      Category = 'Click'
      Caption = 'ListBox1Click'
      Hint = 'ListBox1Click'
      OnExecute = ListBox1ClickExecute
    end
  end
end
