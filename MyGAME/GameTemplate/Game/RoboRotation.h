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
	 * @brief ƒJƒƒ‰‚Ì“®‚«‚É‰ˆ‚Á‚½‰ñ“]
	 * @return 
	*/
	static Quaternion RotationWithCamera(Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front);

	static void UpdateRotation(SkinModelRender* skinModel, Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front);
private:
};

