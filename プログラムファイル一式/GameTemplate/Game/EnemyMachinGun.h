#pragma once
class SkinModelRender;

class EnemyMachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//マシンガンの位置
	Vector3 m_targetPos = Vector3::Zero;		//着弾させるターゲット座標
	bool m_canShoot = false;					//現在弾を撃てるかどうか
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

	/**
	 * @brief 自分が発射された位置を設定する
	 * @param pos 発射された位置
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief 発射できるかどうかを設定する
	 * @param canShoot 発射できるかどうか
	*/
	void SetCanShoot(bool canShoot) { m_canShoot = canShoot; }

private:
	SkinModelRender* m_skinModelRender = nullptr;	//マシンガンのモデル
};

