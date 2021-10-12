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
	 * @brief ƒJƒƒ‰‚Ì“®‚«‚É‰ˆ‚Á‚½‰ñ“]
	 * @return 
	*/
	Quaternion RotationWithCamera();

	void UpdateRotation(SkinModelRender* skinModel);
private:
	float m_rotationAngle = 0.0f;
};

