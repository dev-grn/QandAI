//---------------------------------------------------------------------------

#include <vcl.h>
#include<vector>
#include<iostream>
#include<string>
#include<math.h>
#include<algorithm>
#include <windows.h>
#pragma hdrstop

#include "Unit5.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
int NoD, MaxL, MinL, LoN[10000], result, result2[100000], nx, errored[10000][1000], HowManyError[10000];//data 수, 최장 길이, 최단 길이, 각 수 길이, 틀린 부분 있는지 확인, 현 x 값
String ReadyOut, input[10000]; //해밍 적용 값 저장, 각 수
vector<int> Data[10000], Transmit[10000];


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	Label6->Font->Color = clGray;
	Label10->Font->Color = (TColor) RGB (0xFF, 0xA5, 0x00);
	Label9->Font->Color = clGreen;
}

//---------------------------------------------------------------------------

void Hamming(int x)  //지금 주어진 수가 아니라 맨 앞에 1 붙이고 맨 뒤의 수 버린 값으로 계산 중
{
	//ShowMessage(IntToStr(Data[x][0])+" "+IntToStr(Data[x][1])+IntToStr(Data[x][2])+IntToStr(Data[x][3]));
	int k = LoN[x];
	int parity_num = log(k + 1) / log(2);
	int t = 1, b = 0;
	while(pow(2, parity_num) < parity_num + k + 1)
		parity_num++;
	//ShowMessage(k);
	//ShowMessage(parity_num);
	vector<int> location, square;
	location.push_back(-1);
	for(int i = 1; i<=k+parity_num; i++)
	{
		if(i == t)
		{
			square.push_back(t);
			location.push_back(0);
			b++;
			t *= 2;
			//ShowMessage(0);
		}
		else{
			location.push_back(i - b);
			//ShowMessage(i-b);
		}
	}

	//ShowMessage(k+parity_num);
	//ShowMessage(location[k+parity_num]);
	square.push_back(t);
	t = 1;
	vector<bool> parity_bits;
	for(int j = 1; j<=k+parity_num; j++)
	{
		//ShowMessage(Data[x][location[j + t - 1]]);
	}
	for(int i = 1; i<=parity_num; i++)
	{
		int parity = -1;
		for(int j = 1; j<=k+parity_num; j++)
		{
			if(j % (t * 2) >= 1 && j % (t * 2) <= t)
			{
				//ShowMessage(Data[x][location[j + t - 1]]);
				if(j + t - 1 > k + parity_num)
					break;
				if(parity == -1)
					parity = Data[x][location[j + t - 1]];
				else
					parity = parity ^ Data[x][location[j + t - 1]];
			}
		}
		parity_bits.push_back(parity);
		t *= 2;
	}
	Transmit[x].resize(k + parity_num + 1);
	fill(Transmit[x].begin(), Transmit[x].end(), -1);
	for(int i = 0; i<parity_num; i++) Transmit[x][square[i]] = parity_bits[i];
	for(int i = 1; i<=k + parity_num; i++)
		if(location[i] != 0) Transmit[x][i] = Data[x][location[i]];
}

void Cause_Error(int x, int err_per)
{
	ReadyOut = "";
	int length = Transmit[x].size();
	for(int i = 1; i<length; i++)
	{
		int r = random(100);
		int exstd = Transmit[x][i];
		if(r < err_per)
			Transmit[x][i] = random(2);
		if(Transmit[x][i]!=exstd)
		{
			HowManyError[x]++;
			errored[x][i] = 1;
		}
	}
	for(int i = 1; i<length; i++)
		ReadyOut+=Transmit[x][i];
	Form5->ListBox3->Items->Add(ReadyOut);
	int maxWidth = 0;
	for (int i = 0; i < Form5->ListBox3->Items->Count; i++) {
		int itemWidth = Form5->ListBox3->Canvas->TextWidth(Form5->ListBox3->Items->Strings[i]);
		if (itemWidth > maxWidth) {
			maxWidth = itemWidth;
		}
	}
   SendMessage(Form5->ListBox3->Handle, LB_SETHORIZONTALEXTENT, maxWidth+10, 0);
}

void Reception(int x)
{
	int length = Transmit[x].size() - 1;
	int parity_num = log(length) / log(2);
	while(pow(2, parity_num) < length) parity_num++;

	vector<int> square; int t = 1;
	for(int i = 1; i<=length; i++)
	{
		if(i == t)
		{
			square.push_back(t);
			t *= 2;
		}
	}
	square.push_back(t); t = 1;

	vector<int> examination;
	for(int i = 1; i<=parity_num; i++)
	{
		int parity = -1;
		for(int j = 1; j<=length; j++)
		{
			if(j % (t * 2) >= 1 && j % (t * 2) <= t)
			{
				if(j + t - 1 > length) break;
				if(parity == -1) parity = Transmit[x][j + t - 1];
				else parity = parity ^ Transmit[x][j + t - 1];
			}
		}
		examination.push_back(parity); t *= 2;
	}
	result = 0;
	for(int i = parity_num - 1; i>=0; i--) result += (examination[i] * square[i]);
	ReadyOut = "";
	for(int i = 1; i<Transmit[x].size(); i++)
	{
		if(i==result)
		{
			if(Transmit[x][i]==0)
				ReadyOut+='1';
			if(Transmit[x][i]==1)
				ReadyOut+='0';
			continue;
		}
		ReadyOut+=Transmit[x][i];

	}
	result2[x] = result;
	Form5->ListBox4->Items->Add(ReadyOut);
	int maxWidth = 0;
	for (int i = 0; i < Form5->ListBox4->Items->Count; i++) {
		int itemWidth = Form5->ListBox4->Canvas->TextWidth(Form5->ListBox4->Items->Strings[i]);
		if (itemWidth > maxWidth) {
			maxWidth = itemWidth;
		}
	}
   SendMessage(Form5->ListBox4->Handle, LB_SETHORIZONTALEXTENT, maxWidth+10, 0);
	//ShowMessage("11");
}


//---------------------------------------------------------------------------

void __fastcall TForm5::Button1Click(TObject *Sender)
{
	Randomize();
	MinL = SpinEdit1->Value;
	MaxL = SpinEdit2->Value;
	NoD = SpinEdit3->Value;
	for(int i = 0;i<NoD;i++)
	{
		LoN[i] = random(MaxL-MinL+1)+MinL;
		input[i] = "1";
		for(int j = 1;j<LoN[i];j++)
		{
			input[i]+=(random(2));
		}
		ListBox1->Items->Add(input[i]);
		int maxWidth = 0;
		for (int i = 0; i < ListBox1->Items->Count; i++) {
			int itemWidth = ListBox1->Canvas->TextWidth(ListBox1->Items->Strings[i]);
			if (itemWidth > maxWidth) {
				maxWidth = itemWidth;
			}
		}
		SendMessage(ListBox1->Handle, LB_SETHORIZONTALEXTENT, maxWidth+10, 0);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm5::Button2Click(TObject *Sender)
{
	int err;
	err = SpinEdit4->Value;
	for(int i = 0;i<NoD;i++)
	{
		Data[i].push_back(-1);
		for(int j = 1; j<=LoN[i]; j++)
		{
			Data[i].push_back(input[i][j]-'0');
			//ShowMessage(Data[i][j+1]);
		}
		Hamming(i);
		Cause_Error(i, err);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm5::Button4Click(TObject *Sender)
{
	for(int i = 0;i<NoD;i++)
	{
		nx = i;
		Reception(i);
		//ShowMessage(i);
	}
	//nx=-1;
}
//---------------------------------------------------------------------------



void __fastcall TForm5::ListBox4DrawItem(TWinControl *Control, int Index, TRect &Rect,
		  TOwnerDrawState State)   //마지막 줄 틀린 글자만 빨간색
{
	String itemText = ListBox4->Items->Strings[Index];

	// 배경 그리기
	ListBox4->Canvas->FillRect(Rect);

	int xPos = Rect.Left + 2;  // x 좌표 시작 위치
	int yPos = Rect.Top;  // y 좌표는 동일

	// 글자마다 반복하며 그리기
	if(HowManyError[Index]>1)
	{
		for (int i = 1; i <= itemText.Length(); i++)
		{
			ListBox4->Canvas->Font->Color = clRed;

			// 한 글자씩 출력
			ListBox4->Canvas->TextOut(xPos, yPos, itemText.SubString(i, 1));

			// 다음 글자 위치로 이동
			xPos += ListBox4->Canvas->TextWidth(itemText.SubString(i, 1));
		}
		return;
	}
	for (int i = 1; i <= itemText.Length(); i++)
	{
		if (i == result2[Index])
		{
			ListBox4->Canvas->Font->Color = clLime;
		}
		else
		{
			// 선택된 항목과 그렇지 않은 항목에 대한 처리
			if (State.Contains(odSelected))
				ListBox4->Canvas->Font->Color = clHighlightText;  // 선택된 글자 색상
			else
				ListBox4->Canvas->Font->Color = clWindowText;     // 선택되지 않은 글자 색상
		}

		// 한 글자씩 출력
		ListBox4->Canvas->TextOut(xPos, yPos, itemText.SubString(i, 1));

		// 다음 글자 위치로 이동
		xPos += ListBox4->Canvas->TextWidth(itemText.SubString(i, 1));
   }

}
//---------------------------------------------------------------------------

void __fastcall TForm5::ListBox3DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State)
{
	String itemText = ListBox3->Items->Strings[Index];

	// 배경 그리기
	ListBox3->Canvas->FillRect(Rect);

	int xPos = Rect.Left + 2;  // x 좌표 시작 위치
	int yPos = Rect.Top;  // y 좌표는 동일

	// 글자마다 반복하며 그리기
	for (int i = 1; i <= itemText.Length(); i++)
	{
		if (errored[Index][i]==1)
		{
			ListBox3->Canvas->Font->Color = clRed;
		}
		else
		{
			// 선택된 항목과 그렇지 않은 항목에 대한 처리
			if (State.Contains(odSelected))
				ListBox3->Canvas->Font->Color = clHighlightText;  // 선택된 글자 색상
			else
				ListBox3->Canvas->Font->Color = clWindowText;     // 선택되지 않은 글자 색상
		}

		// 한 글자씩 출력
		ListBox3->Canvas->TextOut(xPos, yPos, itemText.SubString(i, 1));

		// 다음 글자 위치로 이동
		xPos += ListBox3->Canvas->TextWidth(itemText.SubString(i, 1));
   }
}
//---------------------------------------------------------------------------


