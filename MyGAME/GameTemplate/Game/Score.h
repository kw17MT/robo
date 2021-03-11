#pragma once
class Score : public IGameObject
{
private:
	//最終的に表示するスコア
	//具材の数 * 100
	int Score01 = 0;
	int Score02 = 0;
	//カウンター側で何層のハンバーガーを提供したか。
	//提供した具材を数えておく。
	int BasePoint01 = 0;
	int BasePoint02 = 0;

	RenderContext renderContext = g_graphicsEngine->GetRenderContext();
public:
	Score();
	//カウンター側でnumをあてはめ、こちら側のBasePointに加算する。
	void SetBasePoint01(int num) { BasePoint01 += num; }
	void SetBasePoint02(int num) { BasePoint02 += num; }

	//表示するスコアをこの関数で計算し、その文字列を返す。
	const wchar_t* FinalScore01();
	const wchar_t* FinalScore02();

	void Update();
};

