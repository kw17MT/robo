#pragma once
class Level;
class FixedUI;
class SpriteRender;
class Guzai;
class GuzaiGene;
class GuzaiOkiba;
class Kitchen;
class Counter;
class Score;
class Result;
class FontRender;
class Level;
class DishGene;
class DishSpeedManeger;
class TrashCan;
class Floor;
class SoundSource;
class Player;
class PlayerGene;

class PostEffectTest;
class ShadowTest;
class Ground;
class CycleDirection;

//class PopUp2D;
class Game : public IGameObject
{
private:
	bool isTimeUp = false;
	int StockPlaceNum = 0;
	bool m_soundFlag00 = false; //0の音が鳴っているか？
	bool m_soundFlag01 = false; //1の音が鳴っているか？
	bool m_soundFlag02 = false; //2の音が鳴っているか？
	bool m_soundFlag03 = false; //3の音が鳴っているか？
public:
	//オブジェクトやUIの表示
	Game();
	~Game();

	bool Start();
	void Update();

	//カウントダウンする。
	void CountDown();

	FixedUI* ui;										//座標の動かないUI（時間、スコア、ミス）
	Counter* counter01;									//カウンタ―１
	Counter* counter02;									//カウンタ―２
	Kitchen* kitchen01;									///キッチン１
	Kitchen* kitchen02;									//キッチン２
	SpriteRender* menu[3];								//メニュー表示３枚分
	Player* player[2];									//両プレイヤー
	PlayerGene* playerGene;								//プレイヤーを生成するもの
	GuzaiGene* guzaiGene;								//具材を生成するもの
	Score* m_score = nullptr;							//プレイヤーのスコアを表示する。
	GuzaiOkiba* guzaiOkiba;								//具材置き場を8個出す
	DishGene* dishGene;									//ウェイポイントに従って皿を出す
	DishSpeedManeger* m_speedManeger;					//皿のスピードを管理する
	TrashCan* m_trashCan[2];							//ゴミ箱
	Floor* floor_r;										//右の床
	Floor* floor_l;										//左の床	
	Result* m_result[3];								//勝敗表示用:引き分けの場合、プレイヤー1、プレイヤー2に用意
	FontRender* m_font;									//フォント
	CSoundSource* m_bgm;									//BGM用
	CycleDirection* m_directionSprite[2] = {nullptr};

	//ここから下はいらないと思う
	PostEffectTest* postTest;
	ShadowTest* shadowTest;
	Ground* ground;

	//PopUp2D* m_popup2D_1 = nullptr;
	//PopUp2D* m_popup2D_2 = nullptr;

	bool GetTimeUp() { return isTimeUp; }
	void SetTimeUp() { isTimeUp = true; }

	//カウントダウン用。
	float m_timer = 4.0f;
	//FontRender* m_font;
	
	//カウントダウン用スプライト
	SpriteRender* m_countSprite = nullptr;
	Vector3 m_ctScale = Vector3::One;
	int changeCount = 3;
	float scaleRate = 2.0f;
	float alpha = 0.0f;
	bool fadeOut = false;

};