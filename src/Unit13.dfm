object Form13: TForm13
  Left = 78
  Top = 197
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Section by P1'
  ClientHeight = 81
  ClientWidth = 206
  Color = clGray
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
    Left = 112
    Top = 48
    Width = 38
    Height = 13
    Caption = 'Plane D'
  end
  object Label2: TLabel
    Left = 0
    Top = 0
    Width = 33
    Height = 13
    Caption = 'Node#'
  end
  object Label3: TLabel
    Left = 112
    Top = 0
    Width = 37
    Height = 13
    Caption = 'Plane A'
  end
  object Label4: TLabel
    Left = 112
    Top = 16
    Width = 37
    Height = 13
    Caption = 'Plane B'
  end
  object Label5: TLabel
    Left = 112
    Top = 32
    Width = 37
    Height = 13
    Caption = 'Plane C'
  end
  object Label6: TLabel
    Left = 0
    Top = 16
    Width = 43
    Height = 13
    Caption = 'P1 stress'
  end
  object Button1: TButton
    Left = 8
    Top = 56
    Width = 33
    Height = 17
    Caption = 'OK'
    TabOrder = 0
    OnClick = Button1Click
  end
  object RadioButton1: TRadioButton
    Left = 0
    Top = 32
    Width = 41
    Height = 17
    Caption = 'Left'
    TabOrder = 1
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 48
    Top = 32
    Width = 49
    Height = 17
    Caption = 'Right'
    TabOrder = 2
    OnClick = RadioButton2Click
  end
  object Edit1: TEdit
    Left = 152
    Top = 48
    Width = 49
    Height = 21
    TabOrder = 3
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 48
    Top = 0
    Width = 57
    Height = 21
    TabOrder = 4
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 152
    Top = 0
    Width = 49
    Height = 21
    TabOrder = 5
    Text = 'Edit3'
  end
  object Edit4: TEdit
    Left = 152
    Top = 16
    Width = 49
    Height = 21
    TabOrder = 6
    Text = 'Edit4'
  end
  object Edit5: TEdit
    Left = 152
    Top = 32
    Width = 49
    Height = 21
    TabOrder = 7
    Text = 'Edit5'
  end
  object Edit6: TEdit
    Left = 48
    Top = 16
    Width = 57
    Height = 21
    TabOrder = 8
    Text = 'Edit6'
  end
  object Button2: TButton
    Left = 48
    Top = 56
    Width = 57
    Height = 17
    Caption = 'Close'
    TabOrder = 9
    OnClick = Button2Click
  end
end
