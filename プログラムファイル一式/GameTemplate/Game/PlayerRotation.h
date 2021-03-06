#pragma once
#include "SkinModelRender.h"

class PlayerRotation
{
private:
public:

	Quaternion RotationX(Quaternion playerRot);

	Quaternion RotationY(Quaternion playerRot);

	Quaternion ResetRotation();
	/**
	 * @brief カメラの動きに沿った回転
	 * @return 
	*/
	Quaternion RotationWithCamera();

	void UpdateRotation(SkinModelRender* skinModel);
private:
	float m_rotationAngle = 0.0f;
};

