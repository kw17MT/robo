#pragma once
class CameraMove
{
private:
	bool m_isSetDeadCamera = false;						//プレイヤーが倒された時のカメラの位置を計算するかどうか
	Vector3 m_prevCameraPos = Vector3::Zero;			//1フレーム前のカメラの位置
public:
	/**
	 * @brief カメラのターゲットを更新する
	*/
	void UpdateCameraTarget(Vector3 currentHomePos);

	/**
	 * @brief プレイヤーの移動に伴うカメラターゲットの平行移動
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	void Translation(Vector3 prevPlayerPos, Vector3 currentHomePos);

	/**
	 * @brief カメラの更新処理のすべてを行う
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	void UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentHomePos);

	/**
	 * @brief プレイヤーが倒された時のカメラの位置計算関数
	 * @param prevMoveDirection 1フレーム前のプレイヤーの移動方向
	*/
	void SetDeadCamera(Vector3 prevMoveDirection);

	/**
	 * @brief プレイヤーが倒された時のカメラの位置を計算するかどうか設定する
	 * @param state TRUE　計算する　FALSE　計算しない
	*/
	void SetIsDeadCamera(bool state) { m_isSetDeadCamera = state; }
};

