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
	 * @brief �J�����̓����ɉ�������]
	 * @return 
	*/
	Quaternion RotationWithCamera();

	void UpdateRotation(SkinModelRender* skinModel, Vector3 prevMoveVec, Vector3 currentMoveVec, Vector3& front);
private:
	float m_rotationAngle = 0.0f;
};

