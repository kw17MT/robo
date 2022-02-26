#pragma once
class CameraShake
{
private:
	float m_duration = 0.0f;
	float m_strength = 0.0f;								
	float m_shakingDuration = 0.0f;

	float a = 0.0f;

	Vector3 m_positionToReturn = Vector3::Zero;
	Vector3 m_moveDirection = Vector3::Zero;

	bool m_isFinishShake = true;
	bool m_shouldHarder = false;
	bool m_isAwayFromHome = false;
	bool m_canShake = true;

	enum EnDirection
	{
		enLeftUp,
		enLeftMiddle,
		enLeftBottom,
		enUp,
		enBottom,
		enRightUp,
		enRightMiddle,
		enRightBottom
	};
public:
	/**
	 * @brief 揺れに必要なパラメータを設定する
	 * @param duration 継続時間
	 * @param strength 揺れの強さ
	 * @param shouldHarder 時間によって揺れを段々強くするか
	*/
	void SetParam(float duration, float strength, bool shouldHarder)
	{
		m_duration = duration;
		m_strength = strength;
		m_shouldHarder = shouldHarder;
		m_isFinishShake = false;
	}

	/**
	 * @brief 戻りに行く位置座標を設定する
	 * 揺れを使う場合、毎フレーム設定すること
	 * @param pos 戻りに行く位置
	*/
	void SetHomePosition(Vector3 pos) { m_positionToReturn = pos; }

	/**
	 * @brief 揺れる方向を計算する
	 * @return 揺れの方向
	*/
	Vector3 Shake()
	{
		//揺らすのが終わっているならば、以下は無視
		if (m_isFinishShake)
		{
			return Vector3::Zero;
		}

		//元の位置から遠ざかっていなければ
		if (!m_isAwayFromHome && m_canShake)
		{
			int direction = rand() % 9;
			m_isAwayFromHome = true;
			m_canShake = false;

			direction = 0;
			switch (direction)
			{
			case enLeftUp:
				m_moveDirection = (g_camera3D->GetRight() * -1.0f) + g_camera3D->GetUp();
				m_moveDirection.Normalize();
				m_moveDirection = m_moveDirection * m_strength;
				
				break;

			case enLeftMiddle:
				break;
			default:
				break;
			}
		}
		else if (m_isAwayFromHome && !m_canShake)
		{
			m_moveDirection = m_positionToReturn - g_camera3D->GetPosition();
			Vector3 length = m_moveDirection;
			m_moveDirection.Normalize();
			m_moveDirection *= m_strength;
			if (a = length.Length() < 1.0f)
			{
				m_canShake = true;
			}
		}

		m_shakingDuration += GameTime().GetFrameDeltaTime();
		if (m_shakingDuration >= 0.5f)
		{
			m_shakingDuration = 0.0f;
			m_isAwayFromHome = false;
		}

		//段々激しくしていくならば
		if (m_shouldHarder)
		{
			//継続時間が半分以上あるならば
			if (m_duration >= m_duration / 2.0f)
			{
				//揺れの強さを毎フレーム強くする
				m_strength += m_strength / 100.0f;
			}
			//継続時間が残り半分以下ならば
			else
			{
				//揺れを収めていく
				m_strength -= m_strength / 100.0f;
			}
		}

		//継続時間をデルタタイムで減らしていく
		m_duration -= GameTime().GetFrameDeltaTime();
		//継続時間がなくなれば
		if (m_duration <= 0.0f)
		{
			//揺れおわり
			m_isFinishShake = true;
			//揺れに関するパラメータを初期化
			SetParam(0.0f, 0.0f, false);
		}

		return m_moveDirection;
	}
};