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
	
class Game : public IGameObject
{
private:
public:
	Game();
	~Game();
	void Update();

	FixedUI* ui;
	Counter* counter01;
	Counter* counter02;
	Kitchen* kitchen01;
	Kitchen* kitchen02;
	SpriteRender* menu[3];
	ModelRender* player[2];
	ObjectGene* generator01;
	ObjectGene* generator02;
	Score* score;
};

