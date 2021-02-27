#include "stdafx.h"
#include "FontRender.h"


bool FontRender::Start()
{
	return true;
}

wchar_t String2Wcahr(int num) {
	std::wstring str;
	str = std::to_wstring(num);				//ˆê“xA”’l‚ğwstringŒ^‚É•ÏŠ·‚·‚é
	const wchar_t* wcStr = str.c_str();		//wstringŒ^‚ğconst wchar_t*Œ^‚É•ÏŠ·‚·‚é

	return *wcStr;
}

void FontRender::Update()
{
	Font fontTime;
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };
	fontTime.Begin(renderContext);
	fontTime.Draw(L"TIME :", posTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	fontTime.Begin(renderContext);
	fontTime.Draw(L"SCORE :", posScore01, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	fontTime.Begin(renderContext);
	fontTime.Draw(L"SCORE :", posScore02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	/*Counter++;
	if (Counter == 30) {
		time--;

		String2Wcahr(time);
		Counter = 0;
	}

	const wchar_t wcTime = String2Wcahr(time);

	fontTime.Begin(renderContext);
	fontTime.Draw(&wcTime, posScore02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);*/

}