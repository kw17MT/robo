#pragma once
class CameraMove
{
public:
	/**
	 * @brief カメラのターゲットを更新する
	 * @param currentCameraPos 現在のカメラの位置
	*/
	static void UpdateCameraTarget(Vector3 currentCameraPos);

	/**
	 * @brief カメラの位置を更新する。
	 * @param playerPos 現在のプレイヤーの位置
	 * @return 
	*/
	static Vector3 UpdateCameraPos(Vector3 currentPlayerPos);

	/**
	 * @brief プレイヤーの移動に伴うカメラターゲットの平行移動
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	static void Translation(Vector3 prevPlayerPos, Vector3 currentPlayerPos);

	/**
	 * @brief カメラの更新処理のすべてを行う
	 * @param prevPlayerPos 1フレーム前のプレイヤーの位置
	 * @param currentPlayerPos 現在のプレイヤーの位置
	*/
	static void UpdatePlayerCamera(Vector3 prevPlayerPos, Vector3 currentPlayerPos);
};

