#pragma once
class PlayerMove
{
private:	
	Vector3 m_currentSpeed = Vector3::Zero;							//現在の通常移動スピード
	Vector3 m_dashSpeed = Vector3::Zero;							//ダッシュした時のスピード
	Vector3 m_nextPos = Vector3::Zero;								//各種スピードを考慮した時のプレイヤーの最終位置
	Vector3 m_forwardDirection = g_camera3D->GetForward();			//画面の前方
	Vector3 m_sideDirection = g_camera3D->GetRight();				//画面の右方向
	Vector3 m_upDirection = Vector3::Zero;							//画面の上方向

	bool m_isDash = false;
	float m_dashSpeedRate = 1.0f;

public:
	/**
	 * @brief R1ボタン入力によるダッシュスピードの計算関数
	 * @return ダッシュスピード
	*/
	const void Dash();

	/**
	 * @brief 通常の移動速度の計算関数
	 * @param currentPos プレイヤーの現在の位置
	 * @return 通常の移動速度
	*/
	const void Move(const Vector3 currentPos);

	/**
	 * @brief 各種速度を考慮後のプレイヤーの最終位置の計算関数
	 * @param currentPos プレイヤーの現在位置
	 * @return プレイヤーの最終位置
	*/
	Vector3 Execute(Vector3 currentPos);

	Vector3 CalcPlayerPos(Vector3 homePos);
};