#pragma once
class Bullet;
class SkinModelRender;

class EnemyMachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//マシンガンの位置
	Vector3 m_targetPos = Vector3::Zero;		//着弾させるターゲット座標
	bool m_canShoot = false;
public:
	/**
	 * @brief コンストラクタ
	*/
	EnemyMachinGun() {};

	/**
	 * @brief デストラクタ
	*/
	~EnemyMachinGun();

	/**
	 * @brief オブジェクト生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 着弾地点と発射地点を設定する。
	 * @param targetPos
	 * @param pos
	*/
	void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos) /*override;*/
	{
		m_position = pos;
		m_targetPos = targetPos;
	}

	void SetCanShoot(bool canShoot) { m_canShoot = canShoot; }

private:
	SkinModelRender* m_skinModelRender = nullptr;	//マシンガンのモデル
	std::vector<Bullet*> m_bullets = { nullptr };	//発射される弾のインスタンス
};

