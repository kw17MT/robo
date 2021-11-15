#pragma once
class SpriteRender;
class Enemy;

class MissileTargetIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;				//スクリーン上での位置
	Vector3 m_targetedEnemyPos = Vector3::Zero;			//ターゲットされている敵の位置
	Vector3 m_scale = Vector3::Zero;					//拡大率
	Quaternion qRot = Quaternion::Identity;				//回転

	float m_angle = 0.0f;								//アイコンに適用する角度
	bool m_isFirstExpand = false;						//最初の拡大をするかどうか
public:
	/**
	 * @brief コンストラクタ
	*/
	MissileTargetIcon() {}

	/**
	 * @brief デストラクタ
	*/
	~MissileTargetIcon();

	/**
	 * @brief インスタンス生成時に呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief ターゲットとなる敵の位置を設定
	 * @param enemyPos 敵の位置
	*/
	void SetTargetedEnemyPos(Vector3 enemyPos) { m_targetedEnemyPos = enemyPos; }

	/**
	 * @brief ターゲットとなる敵のインスタンスを取得
	 * @param enemy 敵
	*/
	void SetTargetedEnemy(Enemy* enemy) { m_enemy = enemy; }

	/**
	 * @brief ターゲットした最初は拡大した状態で出す
	 * @param state 拡大したか
	*/
	void SetFirstExpandScale(bool state) { m_isFirstExpand = state; }
private:
	SpriteRender* m_spriteRender = nullptr;		//アイコン画像のインスタンス
	Enemy* m_enemy = nullptr;					//アイコンを表示する敵のインスタンス
};

