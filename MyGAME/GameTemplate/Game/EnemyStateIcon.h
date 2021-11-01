#pragma once
class SpriteRender;
class RocketTargetStateIcon;
class Enemy;

class EnemyStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale[2] = { Vector3::One, Vector3::Zero };
	Vector3 m_enemyPos = Vector3::Zero;
	bool m_isFirstExpand = false;
	bool m_isCaptured = false;			//自分が捕捉されたか
	bool m_nextTarget = false;			//プレイヤーが現在ターゲットしている敵が倒されたら次に自動的にターゲットされる敵であるかどうか

	enum EnemyState
	{
		enemyTooFar,					//バツの画像だけ出す
		enemyNear,						//四角の画像も出す
		enemyTargeted,					//ターゲットされているからすべての拡大率をゼロに
	}m_enemyState;

public:
	EnemyStateIcon() {}
	~EnemyStateIcon();
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetEnemyPos(Vector3& pos) { m_enemyPos = pos; }
	bool IsTargeted() {
		if (m_enemyState == enemyTargeted) { return true; }
		else { return false; }
	}

	/**
	 * @brief 敵のインスタンスで距離を引数にいれる
	*/
	void JudgeState(int distance);

	void DisplayIcons();

	void IconBehaviour();

	const void SetEnemyObject(Enemy* enemy) { m_enemy = enemy; }
private:
	std::array<SpriteRender*, 2> m_spriteRender = { nullptr, nullptr };
	std::array<RocketTargetStateIcon*, 10> m_rocketTargetIcon = { nullptr };
	std::vector<int> m_rocketTargetPosNumber;

	Enemy* m_enemy;
};

