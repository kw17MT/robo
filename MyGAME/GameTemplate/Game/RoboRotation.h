#pragma once
#include "SkinModelRender.h"

class RoboRotation
{
private:
public:

	static Quaternion RotationX(Quaternion playerRot);

	static Quaternion RotationY(Quaternion playerRot);

	static Quaternion ResetRotation(Quaternion playerRot);

	/**
	 * @brief カメラの動きに沿った回転
	 * @return 
	*/
	static Quaternion RotationWithCamera(Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front);

	static void UpdateRotation(SkinModelRender* skinModel, Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front);
private:
	const static int a = 0.0f;
};

