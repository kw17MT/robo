#pragma once
class SkinModelRender;

class MenuTimer : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;											//タイマーの位置
	Vector3 m_scale = { 2.8f,1.0f,1.0f };						//タイマーの拡大率。Xを拡大して横長にしている
	Quaternion m_rot = Quaternion::Identity;					//回転

	bool m_isTimeUp = false;									//タイマーが０になったか
	int m_gaugeNumber = 0;										//０が左、１が右

	bool m_completedMakeYellow = false;
	bool m_completedMakeRed = false;
	bool m_stirringBigger = true;
public:
	/**
	 * @brief タイマーのインスタンスの破棄
	*/
	~MenuTimer();

	/**
	 * @brief タイマーのモデルの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief タイマーの縮小と色の変化
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
	 * @brief 新しい回転の設定
	 * @param rot 新しい回転率
	*/
	void SetRotation(Quaternion rot) { m_rot = rot; }

	/**
	 * @brief タイムアップしたか返す
	 * @return TRUE＝タイムアップした
	*/
	bool GetTimeUpState() const { return m_isTimeUp; }

	/**
	 * @brief タイムアップしたかの設定
	 * @param state TRUE＝タイムアップした
	*/
	void SetTimeUpState(bool state) { m_isTimeUp = state; }

	/**
	 * @brief どちら側にあるゲージかを設定
	 * @param gaugeNumber ０が左、１が右
	*/
	void SetNumber(int gaugeNumber) { m_gaugeNumber = gaugeNumber; }

	/**
	 * @brief タイマーの拡大率、色を初期値に設定する。
	*/
	void ResetTimerParam(int lightNumber);

	/**
	 * @brief ゲージが赤になったら鼓動させる
	*/
	void StirringGauge();

private:
	SkinModelRender* m_skinModelRender = nullptr;		//モデル
};

