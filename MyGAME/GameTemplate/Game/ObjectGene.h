#pragma once
class Guzai;
class Buff;

class ObjectGene : public IGameObject
{
private:
	//Generator—p‚Ìƒ‚ƒfƒ‹
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

public:
	bool Start();
	void Update();

	int timer = 0;
	int Buffnum = 0;

	Guzai* m_guzai;
	Buff* m_buff;
};
