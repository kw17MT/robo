#pragma once
class MissileBallistic : public IGameObject
{
private:
	Vector3 m_cameraForward = Vector3::Zero;		
	bool m_return = false;
	float senkaiTime = 1.0f;
	float m_cosVol = 0.0f;
	float m_sinVol = 180.0f;

	enum EnBallisticType
	{
		enStraight,
		enMeandeling,
		enRolling,
		enTypeNum
	};
public:
	/**
	 * @brief ミサイルが生まれた時点でのカメラの前方向を設定する
	 * @param cameraForward カメラの前方向
	*/
	void SetCameraForward(Vector3 cameraForward)
	{
		m_cameraForward = cameraForward;
	}

	/**
	 * @brief ミサイルの弾道タイプを決める
	*/
	void DecideBallisticType();

	/**
	 * @brief 弾道の方向を計算して返す
	 * @return 弾道の方向
	*/
	Vector3 GetBallisticDirection();

	void ProgressDegreeChange();
private:
	EnBallisticType m_type = enStraight;
};

