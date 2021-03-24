#pragma once

class FixedUI : public IGameObject
{
private:
	//表示する残り時間
	int LastTime = 60;
	//フレームカウント用、60になったらLastTimeを１減らす。
	int timer = 0;
	//タイムアップフラグ
	bool isTimeUp = false;

	//それぞれのプレイヤーのスコア
	int score01 = 0;
	int score02 = 0;

	//それぞれのパラメーターの座標
	Vector2 posTime = { -100,350 };
	Vector2 posLastTime = { 50, 350 };
	//タイムアップ状態の表示
	Vector2 posTimeUpText = {-100,300};
	Vector2 posTimeUpState = { 100,300 };

	Vector2 posItem01 = { -600,350 };
	Vector2 posItem02 = { 350, 350 };
	
	Vector2 posScore01 = { -600,-300 };
	Vector2 posScore02 = { 350, -300 };

	RenderContext renderContext = g_graphicsEngine->GetRenderContext();
public:
	

	//ここで毎フレーム文字を表示している。
	void Update();

	//タイムアップフラグの状態で文字列を変更
	const wchar_t* TurnTimeUpState();

	//ゲッター
	//タイムアップ状態を取得(true : ON , false : OFF)
	bool GetIsTimeUp() { return isTimeUp; }
	int GetTime() { return LastTime; }
};


