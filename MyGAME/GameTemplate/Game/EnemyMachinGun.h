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
	 * @brief 着弾地点を設定する。
	 * @param targetPos
	*/
	void SetTargetPos(Vector3 targetPos) { m_targetPos = targetPos; }

	void SetPosition(Vector3 pos) { m_position = pos; }

	void SetCanShoot(bool canShoot) { m_canShoot = canShoot; }

private:
	SkinModelRender* m_skinModelRender = nullptr;	//マシンガンのモデル
	std::vector<Bullet*> m_bullets = { nullptr };	//発射される弾のインスタンス
};

