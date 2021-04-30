#pragma once
class SkinModelRender;
class Guzai;
class PlayerGene;

class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_guzaiPos = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	bool isHavingGuzai = false;
	//Ž©•ª‚ÍWayPoint‚Ì‰½”Ô–Ú‚ÌŽM‚È‚Ì‚©
	int myNumber = 0;
	int time = 0;
public:
	Dish() {}
	~Dish() {}

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }
	void SetMyNumber(int num) { myNumber = num; }

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai;
	PlayerGene* playerGene;
};

