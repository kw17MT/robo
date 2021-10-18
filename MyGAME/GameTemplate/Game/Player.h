#pragma once
class SkinModelRender;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = Vector3::Zero;			//�v���C���[�̌��݂̈ʒu

	Vector3 m_prevHomePosition = Vector3::Zero;			//1�t���[���O�̃z�[���|�W�V�����̈ʒu
	Vector3 m_currentHomePosition = Vector3::Zero;				//�J�������ǂ�������ʒu���W

	bool m_isCapturing = false;

public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();

	Vector3 GetPosition()
	{
		//return m_currentPosition;
		return m_currentHomePosition;
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

