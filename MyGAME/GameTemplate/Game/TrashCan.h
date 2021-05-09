#pragma once
class SkinModelRender;
class Player;
class PlayerGene;

class TrashCan : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_trashcanScale = { 0.2f,0.2f,0.2f };
	Vector3 m_targetScale = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	const float distance = 150.0f;									//ゴミ箱がプレイヤーを識別する距離
	int trashcanNo = 0;												//ゴミ箱の番号、１が左、２が右
	bool canTrash = false;											//プレイヤーが近いとTRUEにして捨てることができるようにする。

	bool targetUp = true;											//ターゲットアイコンを浮遊させるよう
public:
	bool Start();
	void Update();

	float CalcDistance(Vector3 v1, Vector3 v2);

	void SetPosition(Vector3 pos) { m_position = pos; }
	Vector3 GetPosition() { return m_position; }
	void SetScale(Vector3 scale) { m_targetScale = scale; }
	void SetTrashCanNum(int num) { trashcanNo = num; }
	bool GetCanTrash() { return canTrash; }

	Player* player[2];
	SkinModelRender* m_skinModelRender;
	SkinModelRender* m_targeting;
	PlayerGene* m_playerGene;
};

