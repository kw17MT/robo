#pragma once
class SkinModelRender;

class Bullet : public IGameObject
{
private:
	Vector3 m_targetPos = Vector3::Zero;	//着弾する目標地点
	Vector3 m_position = Vector3::Zero;		//発射した位置
	Vector3 m_moveSpeed = Vector3::Zero;	//移動速度
	float count = 0;						//寿命
	bool m_firstCalc = false;				//最初の速度計算ができたか
public:
	/**
	 * @brief コンストラクタ
	*/
	Bullet() {};

	/**
	 * @brief デストラクタ
	*/
	~Bullet();

	/**
	 * @brief オブジェクト生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ぶ関数
	*/
	void Update();

	/**
	 * @brief 発射地点から着弾予定地点のベクトルを求める
	 * @return 発射地点から着弾予定地点の単位ベクトル
	*/
	Vector3 CalcToTargetVec();

	/**
	 * @brief 着弾地点と発射地点を設定する
	 * @param target 着弾地点
	 * @param pos 発射地点
	*/
	void SetTargetAndCurrentPos(Vector3 target, Vector3 pos)
	{
		m_targetPos = target;
		m_position = pos;
	}
private:
	SkinModelRender* m_skinModelRender = nullptr;	//弾モデルのオブジェクト
};

