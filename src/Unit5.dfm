object Form5: TForm5
  Left = 76
  Top = 73
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Nodal query'
  ClientHeight = 121
  ClientWidth = 185
  Color = clLime
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
    Width = 33
    Height = 13
    Caption = 'Node#'
  end
  object Label2: TLabel
    Left = 0
    Top = 16
    Width = 37
    Height = 13
    Caption = 'X-coord'
  end
  object Label3: TLabel
    Left = 0
    Top = 32
    Width = 37
    Height = 13
    Caption = 'Y-coord'
  end
  object Label4: TLabel
    Left = 0
    Top = 48
    Width = 37
    Height = 13
    Caption = 'Z-coord'
  end
  object Label5: TLabel
    Left = 0
    Top = 64
    Width = 27
    Height = 13
    Caption = 'Value'
  end
  object Edit1: TEdit
    Left = 40
    Top = 0
    Width = 137
    Height = 21
    ReadOnly = True
    TabOrder = 0
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 40
    Top = 16
    Width = 137
    Height = 21
    ReadOnly = True
    TabOrder = 1
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 40
    Top = 32
    Width = 137
    Height = 21
    ReadOnly = True
    TabOrder = 2
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 40
    Top = 48
    Width = 137
    Height = 21
    ReadOnly = True
    TabOrder = 3
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 40
    Top = 64
    Width = 137
    Height = 21
    ReadOnly = True
    TabOrder = 4
    Text = 'Edit5'
  end
  object Button1: TButton
    Left = 48
    Top = 96
    Width = 75
    Height = 17
    Caption = 'Close'
    TabOrder = 5
    OnClick = Button1Click
  end
end
