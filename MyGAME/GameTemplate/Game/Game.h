#pragma once
class Level;
class ModelRender;
class FixedUI;
class SpriteRender;
class Guzai;
class ObjectGene;
class Kitchen;
class Counter;
class Score;
class Result;
class FontRender;
class Level;
class PostEffectTest;
	
class Game : public IGameObject
{
private:
	bool isTimeUp = false;
public:
	//オブジェクトやUIの表示
	Game();
	~Game();

	bool Start();

	//今は特に役割なし。
	void Update();

	FixedUI* ui;
	Counter* counter01;
	Counter* counter02;
	Kitchen* kitchen01;
	Kitchen* kitchen02;
	SpriteRender* menu[3];
	SpriteRender* Buff[2];
	ModelRender* player[2];
	ObjectGene* generator01;
	ObjectGene* generator02;
	Score* m_score = nullptr;
	PostEffectTest* postTest;
	
	//勝敗表示用:引き分けの場合、プレイヤー1、プレイヤー2に用意
	Result* m_result[3];

	bool GetTimeUp() { return isTimeUp; }
	void SetTimeUp() { isTimeUp = true; }
};