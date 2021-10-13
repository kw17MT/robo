#pragma once
class SkinModelRender;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_prevPosition = Vector3::Zero;				//1�t���[���O�̃v���C���[�̈ʒu
	Vector3 m_currentPosition = Vector3::Zero;			//�v���C���[�̌��݂̈ʒu

	Vector3 m_homePosition = Vector3::Zero;				//�J�������ǂ�������ʒu���W

	bool m_isCapturing = false;

public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();

	Vector3 GetPosition()
	{
		//return m_currentPosition;
		return m_homePosition;
	}

	const bool GetIsCapturing() const 
	{
		return m_isCapturing;
	}
private:
	SkinModelRender* m_skinModelRender;
	CameraMove m_cameraMove;
	PlayerMove m_roboMove;
	PlayerRotation m_roboRotation;
};

