object Form5: TForm5
  Left = 0
  Top = 0
  Caption = 'Form5'
  ClientHeight = 441
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  TextHeight = 15
  object Label1: TLabel
    Left = 74
    Top = 295
    Width = 73
    Height = 15
    Caption = 'Length Range'
  end
  object Label2: TLabel
    Left = 15
    Top = 359
    Width = 85
    Height = 15
    Caption = 'Number of Data'
  end
  object Label3: TLabel
    Left = 121
    Top = 359
    Width = 74
    Height = 15
    Caption = 'Generate Data'
  end
  object Label4: TLabel
    Left = 285
    Top = 295
    Width = 51
    Height = 15
    Caption = 'Error Rate'
  end
  object Label5: TLabel
    Left = 274
    Top = 359
    Width = 72
    Height = 15
    Caption = 'Transmit Data'
  end
  object Label7: TLabel
    Left = 478
    Top = 295
    Width = 65
    Height = 15
    Caption = 'Modify Data'
  end
  object Label6: TLabel
    Left = 76
    Top = 27
    Width = 69
    Height = 15
    Caption = 'Original Data'
  end
  object Label9: TLabel
    Left = 468
    Top = 27
    Width = 84
    Height = 15
    Caption = 'Error Correction'
  end
  object Label10: TLabel
    Left = 266
    Top = 27
    Width = 89
    Height = 15
    Caption = 'Transmitted Data'
  end
  object Label11: TLabel
    Left = 379
    Top = 336
    Width = 64
    Height = 15
    Caption = 'Initailization'
  end
  object SpinEdit1: TSpinEdit
    Left = 25
    Top = 316
    Width = 75
    Height = 24
    MaxValue = 10000
    MinValue = 0
    TabOrder = 0
    Value = 0
  end
  object ListBox1: TListBox
    Left = 25
    Top = 48
    Width = 161
    Height = 241
    ItemHeight = 15
    TabOrder = 1
  end
  object ListBox3: TListBox
    Left = 230
    Top = 48
    Width = 161
    Height = 241
    Style = lbOwnerDrawFixed
    ItemHeight = 15
    TabOrder = 2
  end
  object SpinEdit2: TSpinEdit
    Left = 121
    Top = 316
    Width = 75
    Height = 24
    MaxValue = 10000
    MinValue = 0
    TabOrder = 3
    Value = 0
  end
  object SpinEdit3: TSpinEdit
    Left = 25
    Top = 380
    Width = 75
    Height = 24
    MaxValue = 10000
    MinValue = 0
    TabOrder = 4
    Value = 0
  end
  object Button1: TButton
    Left = 121
    Top = 380
    Width = 75
    Height = 25
    Caption = 'Generate'
    TabOrder = 5
    OnClick = Button1Click
  end
  object SpinEdit4: TSpinEdit
    Left = 273
    Top = 316
    Width = 75
    Height = 24
    MaxValue = 100
    MinValue = 0
    TabOrder = 6
    Value = 0
  end
  object Button2: TButton
    Left = 273
    Top = 380
    Width = 75
    Height = 25
    Caption = 'Transmit'
    TabOrder = 7
    OnClick = Button2Click
  end
  object Button4: TButton
    Left = 473
    Top = 316
    Width = 75
    Height = 25
    Caption = 'Modify'
    TabOrder = 8
    OnClick = Button4Click
  end
  object ListBox4: TListBox
    Left = 430
    Top = 48
    Width = 161
    Height = 241
    Style = lbOwnerDrawFixed
    ItemHeight = 15
    TabOrder = 9
    OnDrawItem = ListBox4DrawItem
  end
end
