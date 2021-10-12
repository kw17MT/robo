#pragma once
class CameraMove
{
public:
	/**
	 * @brief カメラのターゲットを更新する
	*/
	void UpdateCameraTarget(Vector3 currentPlayerPos);


	/**
	 * @brief プレイヤーの移動に伴うカメラターゲットの平行移動
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	void Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos);

	/**
	 * @brief カメラの更新処理のすべてを行う
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	void UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos);

private:
	Vector3 m_correctTarget = g_camera3D->GetTarget();
	Vector3 m_correctPos = g_camera3D->GetPosition();
	Vector3 m_prevPos = Vector3::Zero;
	Quaternion m_prevRot = Quaternion::Identity;
};

