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
class PostEffectTest;
class TrashCan;


class ShadowTest;
class Ground;

class Player;
class PlayerGene;
//class PopUp2D;

class DishGene;
class DishSpeedManeger;

class Floor;
class SoundSource;

	
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

	//今は特に役割なし。
	void Update();

	//カウントダウンする。
	void CountDown();

	FixedUI* ui;
	Counter* counter01;
	Counter* counter02;
	Kitchen* kitchen01;
	Kitchen* kitchen02;
	SpriteRender* menu[3];
	SpriteRender* Buff[2];
	Player* player[2];
	PlayerGene* playerGene;
	GuzaiGene* guzaiGene;
	Score* m_score = nullptr;
	PostEffectTest* postTest;

	ShadowTest* shadowTest;
	Ground* ground;

	GuzaiOkiba* guzaiOkiba;

	DishGene* dishGene;
	DishSpeedManeger* m_speedManeger;
	//PopUp2D* m_popup2D_1 = nullptr;
	//PopUp2D* m_popup2D_2 = nullptr;

	TrashCan* m_trashCan[2];
	Floor* floor_r;
	Floor* floor_l;

	//勝敗表示用:引き分けの場合、プレイヤー1、プレイヤー2に用意
	Result* m_result[3];

	bool GetTimeUp() { return isTimeUp; }
	void SetTimeUp() { isTimeUp = true; }

	//カウントダウン用。
	float m_timer = 4.0f;
	FontRender* m_font;


};