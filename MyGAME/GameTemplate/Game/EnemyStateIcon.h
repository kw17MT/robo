#pragma once
class SpriteRender;

class EnemyStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale[2] = { Vector3::One, Vector3::Zero };
	Vector3 m_enemyPos = Vector3::Zero;
	bool m_isFirstExpand = false;
	bool m_isCaptured = false;			//自分が捕捉されたか

	enum EnemyState
	{
		enemyTooFar,					//バツの画像だけ出す
		enemyNear,						//四角の画像も出す
		enemyTargeted					//ターゲットされているからすべての拡大率をゼロに
	}m_enemyState;

public:
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetEnemyPos(Vector3& pos) { m_enemyPos = pos; }

	/**
	 * @brief 敵のインスタンスで距離を引数にいれる
	*/
	void JudgeState(int distance);

	void DisplayIcons();

	void IconBehaviour();

	void CalcPosition();
private:
	SpriteRender* m_spriteRender[2] = { nullptr, nullptr };
};

