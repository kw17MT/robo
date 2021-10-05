#pragma once
class CameraMove
{
public:
	/**
	 * @brief カメラのターゲットを更新する
	*/
	static void UpdateCameraTarget(Vector3 currentPlayerPos);

	/**
	 * @brief カメラの位置を更新する。
	 * @param playerPos 現在のプレイヤーの位置
	 * @return 
	*/
	static void UpdateCameraPos(Vector3 currentPlayerPos);

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

