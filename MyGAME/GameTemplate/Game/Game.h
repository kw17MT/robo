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
	
class Game : public IGameObject
{
private:
	bool isTimeUp = false;
public:
	//�I�u�W�F�N�g��UI�̕\��
	Game();
	~Game();
	//���͓��ɖ����Ȃ��B
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
	
	//���s�\���p:���������̏ꍇ�A�v���C���[1�A�v���C���[2�ɗp��
	Result* m_result[3];

	bool GetTimeUp() { return isTimeUp; }
	void SetTimeUp() { isTimeUp = true; }
};
