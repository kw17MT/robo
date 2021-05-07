#pragma once
class SkinModelRender;
class Player;
class PlayerGene;

class TrashCan : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_trashcanScale = { 0.2f,0.2f,0.2f };
	Vector3 m_targetScale = Vector3::Zero;
	const float distance = 150.0f;
	int trashcanNo = 0;
public:
	bool Start();
	void Update();

	float CalcDistance(Vector3 v1, Vector3 v2);

	void SetPosition(Vector3 pos) { m_position = pos; }
	Vector3 GetPosition() { return m_position; }
	void SetScale(Vector3 scale) { m_targetScale = scale; }
	void SetTrashCanNum(int num) { trashcanNo = num; }

	Player* player[2];
	SkinModelRender* m_skinModelRender;
	SkinModelRender* m_targeting;
	PlayerGene* m_playerGene;
};

