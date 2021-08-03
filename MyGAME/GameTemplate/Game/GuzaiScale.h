#pragma once
class GuzaiScale
{
public:
	/**
	 * @brief 具材のターゲット状態により、拡大率を変更させる。
	 * @param isTargeted ターゲットされているかどうか
	 * @param typeNumber 具材の種類番号
	 * @param nowScale 現在の拡大率
	 * @return 
	*/
	static Vector3 ChangeScaleDependOnTargetedState(bool isTargeted, int typeNumber, Vector3 nowScale);
};

