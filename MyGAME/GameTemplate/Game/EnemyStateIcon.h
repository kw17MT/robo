#pragma once
class SpriteRender;
class MissileTargetIcon;
class Enemy;
class CrossIcon;
class CapturedSquareIcon;

class Reticle;
class MissileGenerator;

class EnemyStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
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

	void SetReticleInstance(Reticle* reticle) { m_reticle = reticle; }


private:
	std::vector<MissileTargetIcon*> m_missileTargetIcon;

	CrossIcon* m_crossIcon = nullptr;
	CapturedSquareIcon* m_squareIcon = nullptr;

	Reticle* m_reticle = nullptr;					//レティクルのインスタンス保存用
	MissileGenerator* m_missileGenerator = nullptr;	//ミサイル生成器のインスタンス保存用

	Enemy* m_enemy;
};

