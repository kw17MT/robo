#pragma once
class FontRender;
class SpriteRender;
class SoundSource;
class PlayerGene;
class PathMove;

class FixedUI : public IGameObject
{
private:
	//変化させない文字
	//Time,TimeUp,Score,Itemの固定文字
	FontRender* TextTime = nullptr;
	FontRender* TextScore[2] = { nullptr, nullptr };
	
	//ミス用の文字
	FontRender* TextMiss[2] = { nullptr, nullptr};
	
	//変化する文字。
	//制限時間、タイムアップのONOFF
	FontRender* Time = nullptr;
	
	//表示する残り時間
	int remainingTime = 1;
	
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

	Vector2 posItem01 = { -600,350 };
	Vector2 posItem02 = { 350, 350 };

	Vector2 posScore01 = { -600,-250 };
	Vector2 posScore02 = { 250, -250 };

	//ミス表示位置
	Vector2 posMiss01 = { -600.0f,-300.0f };
	Vector2 posMiss02 = { 250.0f,-300.0f };

	RenderContext renderContext = g_graphicsEngine->GetRenderContext();
public:
	
	//デストラクタ
	~FixedUI();
	bool Start();
	
	//ここで毎フレーム文字を表示している。
	void Update();

	//ゲッター
	//タイムアップ状態を取得(true : ON , false : OFF)
	bool GetIsTimeUp() { return isTimeUp; }
	int GetTime() { return remainingTime; }
	//残時間が少ないときの色の変化
	void RemainingTimeColor();
	CSoundSource* timeSound = nullptr;
};


