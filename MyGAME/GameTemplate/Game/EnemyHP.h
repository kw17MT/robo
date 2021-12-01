#pragma once
class SpriteRender;

//与えられたダメージの種類
enum EnDamageTypes
{
	enBullet,
	enMissile,
	enRaser
};

class EnemyHP : public IGameObject
{
private:
	Vector3 m_enemyPos = Vector3::Zero;		//敵の位置
	Vector3 m_screenPos = Vector3::Zero;	//スクリーン座標
	Vector3 m_scale = Vector3::One;			//拡大率

	int m_enemyHP = 100;					//敵のHP
	bool m_isTargeted = false;				//ターゲットされている時だけHPバーをだす
	bool m_isEnemyDead = false;					//この敵は倒されたか
public:
	/**
	 * @brief コンストラクタ
	*/
	EnemyHP() {}

	/**
	 * @brief デストラクタ
	*/
	~EnemyHP();

	/**
	 * @brief インスタンスが生成された時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief この敵はターゲットされているか
	 * @param isTargeted 現在ターゲットされているか
	*/
	void IsEnemyTargeted(bool isTargeted) { m_isTargeted = isTargeted; }

	/**
	 * @brief 追従する敵の位置の座標
	 * @param enemyPos 敵の位置
	*/
	void SetEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }

	/**
	 * @brief ダメージを適用する
	 * @param damageType ダメージタイプ
	*/
	void ApplyDamage(EnDamageTypes damageType);

	/**
	 * @brief 倒されたか
	 * @return TRUE＝倒された、FALSE＝生きている
	*/
	bool IsEnemyDead() {return m_isEnemyDead;}
private:
	SpriteRender* m_spriteRender = nullptr;			//HPバーのインスタンス
};

