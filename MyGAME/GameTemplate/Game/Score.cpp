#include "stdafx.h"
#include "Score.h"
#include "Counter.h"

void Score::Update()
{
	//ÇOÇPÇÕÇPPÇÃÅAÇOÇQÇÕÇQPÇÃÉXÉRÉA
	Font score01;
	Font score02;
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };

	Score01 = BasePoint01 * 100;
	std::wstring str01;
	str01 = std::to_wstring(Score01);

	Score02 = BasePoint02 * 100;
	std::wstring str02;
	str02 = std::to_wstring(Score02);

	score01.Begin(renderContext);
	score01.Draw(str01.c_str(), { -400.0f,-300.0f }, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	score01.End(renderContext);

	score02.Begin(renderContext);
	score02.Draw(str02.c_str(), { 550.0f,-300.0f }, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	score02.End(renderContext);
}