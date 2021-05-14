#pragma once
class SkinModelRender;
class Player;
class PlayerGene;

class TrashCan : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_trashcanScale = { 1.0f,1.0f,1.0f };
	Vector3 m_targetScale = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	const float distance = 130.0f;									//ゴミ箱がプレイヤーを識別する距離
	int trashcanNo = 0;												//ゴミ箱の番号、１が左、２が右
	bool canTrash = false;											//プレイヤーが近いとTRUEにして捨てることができるようにする。

	bool targetUp = true;											//ターゲットアイコンを浮遊させるよう
	bool isMoving = false;											//ゴミ箱を動かさせるか
	int movingTime = 120;											//ゴミを入れた後どのくらいの時間動くか
public:
	bool Start();
	void Update();

	void SetScale(Vector3 scale) { m_targetScale = scale; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	Vector3 GetPosition() { return m_position; }

	//ゴミ箱の番号の設定　１が左　2が右
	void SetTrashCanNum(int num) { trashcanNo = num; }

	//ごみを捨てられるかどうかの変数を得る
	bool GetCanTrash() { return canTrash; }

	//2地点間の距離を求める
	float CalcDistance(Vector3 v1, Vector3 v2);

	void ChangeMovingState(bool state) { isMoving = state; }
	//ゴミを入れられて動くかどうかを変更する　TRUEにすると動き始める
	void LetStartMoving();

	Player* player[2];
	SkinModelRender* m_skinModelRender;
	SkinModelRender* m_targeting;
	PlayerGene* m_playerGene;
};

