#pragma once
class SkinModelRender;
class Meter : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;				//ゲージの位置
	Vector3 m_scale = { 1.4f,1.0f,1.0f };			//ゲージの拡大率
	Quaternion m_rot = Quaternion::Identity;		//ゲージの回転
public:
	Meter() {};
	
	/**
	 * @briefメーターのインスタンスの破棄
	*/
	~Meter();

	/**
	 * @brief ゲージのモデルの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 変更された、位置、拡大率、回転の設定
	*/
	void Update();

	/**
	 * @brief 新しい位置の設定
	 * @param pos 新しい位置
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief 新しい拡大率の設定
	 * @param scale 新しい拡大率
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }

	/**
	 * @brief 現在の拡大率の獲得
	 * @return 現在の拡大率
	*/
	Vector3 GetScale() const{ return m_scale; }

private:
	SkinModelRender* m_skinModelRender = nullptr;				//モデル
};

