#pragma once
class FontRender;
class SpriteRender;
class SoundSource;
class PlayerGene;
class PathMove;
//class CycleDirection;

class FixedUI : public IGameObject
{
private:
	//変化させない文字
	//Time,TimeUp,Score,Itemの固定文字
	FontRender* TextTime;
	//FontRender* TextTimeUp;
	FontRender* TextScore[2];
	//FontRender* TextItem[2];
	//変化する文字。
	//制限時間、タイムアップのONOFF
	FontRender* Time;
	//FontRender* isTimeUpState;
	//ミス用の文字
	FontRender* TextMiss[2] = { nullptr };

	//CycleDirection* m_directionSprite[2] = {nullptr};

	////コンベア回転方向表示(スプライト)
	//SpriteRender* convDirText = nullptr;		//コンベア回転方向の状態を示すスプライト
	//SpriteRender* convDirLeft = nullptr;		//p1サイドのコンベア回転方向表示
	//SpriteRender* convDirRight = nullptr;		//p2サイドのコンベア回転方向表示

	//Quaternion rotLeft = Quaternion::Identity;	//p1側のスプライト回転
	//float angleLeft = 0.0f;
	//Quaternion rotRight = Quaternion::Identity;	//p2側のスプライト回転
	//float angleRight = 0.0f;

	//表示する残り時間
	int remainingTime = 999;
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
	////タイムアップ状態の表示
	//Vector2 posTimeUpText = {-100,300};
	//Vector2 posTimeUpState = { 100,300 };

	Vector2 posItem01 = { -600,350 };
	Vector2 posItem02 = { 350, 350 };

	/*Vector2 posScore01 = { -600,-300 };
	Vector2 posScore02 = { 350, -300 };*/
	Vector2 posScore01 = { -600,-250 };
	Vector2 posScore02 = { 250, -250 };

	//ミス表示位置
	Vector2 posMiss01 = { -600.0f,-300.0f };
	Vector2 posMiss02 = { 250.0f,-300.0f };

	////コンベア回転方向表示位置
	//Vector3 posConvDirText = { 0.0f,250.0f,0.0f };
	//Vector3 posConvDirLeft = { 220.0f, 80.0f,0.0f };
	//Vector3 posConvDirRight = { -220.0f, 80.0f,0.0f };

	RenderContext renderContext = g_graphicsEngine->GetRenderContext();
public:
	//デストラクタ
	~FixedUI();
	bool Start();
	//ここで毎フレーム文字を表示している。
	void Update();

	////タイムアップフラグの状態で文字列を変更
	//const wchar_t* TurnTimeUpState();

	//ゲッター
	//タイムアップ状態を取得(true : ON , false : OFF)
	bool GetIsTimeUp() { return isTimeUp; }
	int GetTime() { return remainingTime; }
	//残時間が少ないときの色の変化
	void RemainingTimeColor();
	CSoundSource* timeSound;
};


