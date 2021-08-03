#pragma once
class AjustGuzaiPosition
{
public:
	/**
	 * @brief 具材の位置をプレイヤーの前にする
	 * @param normalizedSpeed 正規化されたプレイヤーの移動速度
	 * @param playerPosition 現在のプレイヤーの位置
	 * @return プレイヤーの前方の座標
	*/
	static Vector3 CalcAjustedPosition(Vector3 normalizedSpeed, Vector3 playerPosition);
};

