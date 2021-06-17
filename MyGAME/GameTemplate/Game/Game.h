#pragma once
class Level;
class CLevel2D;
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

class CountDown;

class Game : public IGameObject
{
private:
	Vector4 m_countDownColor = { 1.0f,1.0f,1.0f,0.0f };			//カウントダウン画像のカラー

	bool m_isTimeUp = false;									//ゲームはタイムアップしたか
	bool m_soundFlag = false;									//現在なんらかの音はなっているか（カウントダウン関係）
	bool m_fadeOut = false;										//フェードアウトを開始するか

	int m_changeCount = 1;										//カウントダウンの
	float m_scaleRate = 2.0f;									//
	float m_alpha = 0.0f;										//
	float m_timer = 4.0f;										//カウントダウン用。
public:
	//オブジェクトやUIの表示
	Game() {};
	~Game();

	bool Start();
	void Update();

	//カウントダウンする。
	//void CountDown();

	bool GetTimeUp() { return m_isTimeUp; }
	void SetTimeUp() { m_isTimeUp = true; }

private:
	CLevel2D* m_level2D = nullptr;
	FixedUI* ui = nullptr;											//座標の動かないUI（時間、スコア、ミス）
	Counter* counter00 = nullptr;									//カウンタ―１
	Counter* counter01 = nullptr;									//カウンタ―２
	Kitchen* kitchen00 = nullptr;									///キッチン１
	Kitchen* kitchen01 = nullptr;									//キッチン２
	Player* player[2] = { nullptr,nullptr };						//両プレイヤー
	PlayerGene* playerGene = nullptr;								//プレイヤーを生成するもの
	GuzaiGene* guzaiGene = nullptr;									//具材を生成するもの
	Score* m_score = nullptr;										//プレイヤーのスコアを表示する。
	GuzaiOkiba* guzaiOkiba = nullptr;								//具材置き場を8個出す
	DishGene* dishGene = nullptr;									//ウェイポイントに従って皿を出す
	DishSpeedManeger* m_speedManeger = nullptr;						//皿のスピードを管理する
	TrashCan* m_trashCan[2] = { nullptr, nullptr};					//ゴミ箱
	Floor* floor_r = nullptr;										//右の床
	Floor* floor_l = nullptr;										//左の床	
	Result* m_result[3] = { nullptr,nullptr,nullptr };				//勝敗表示用:引き分けの場合、プレイヤー1、プレイヤー2に用意
	CSoundSource* m_bgm = nullptr;									//BGM用
	CycleDirection* m_directionSprite[3] = {nullptr,nullptr,nullptr};
	Level* m_level = nullptr;
	CountDown* m_countDown = nullptr;								//カウントダウン
};