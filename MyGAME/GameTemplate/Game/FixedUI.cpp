#include "stdafx.h"
#include "FixedUI.h"


bool FixedUI::Start()
{
	return true;
}

wchar_t String2Wcahr(int num) {
	std::wstring str;
	str = std::to_wstring(num);				//àÍìxÅAêîílÇwstringå^Ç…ïœä∑Ç∑ÇÈ
	const wchar_t* wcStr = str.c_str();		//wstringå^Çconst wchar_t*å^Ç…ïœä∑Ç∑ÇÈ

	return *wcStr;
}

void FixedUI::Update()
{
	Font fontTime;
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };
	fontTime.Begin(renderContext);
	fontTime.Draw(L"TIME :", posTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	fontTime.Begin(renderContext);
	fontTime.Draw(L"ITEM", posItem01, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	fontTime.Begin(renderContext);
	fontTime.Draw(L"ITEM", posItem02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
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
		Counter = 0;
	}

	const wchar_t wcTime = String2Wcahr(time);

	fontTime.Begin(renderContext);
	fontTime.Draw(&wcTime, posScore02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);*/

}