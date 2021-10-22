#pragma once
class SkinModelRender;
class MachinGun;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"

namespace
{
	const int animNum = 7;
}

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = { 0.0f,400.0f,5000.0f };			//�v���C���[�̌��݂̈ʒu

	Vector3 m_prevHomePosition = Vector3::Zero;			//1�t���[���O�̃z�[���|�W�V�����̈ʒu
	Vector3 m_currentHomePosition = Vector3::Zero;				//�J�������ǂ�������ʒu���W

	bool m_isCapturing = false;


public:
	Player() {};
	~Player();
	bool Start();
	void Update();

	Vector3 GetPosition()
	{
		return m_currentHomePosition;
	}

	const bool GetIsCapturing() const 
	{
		return m_isCapturing;
	}
private:
	SkinModelRender* m_skinModelRender = nullptr;
	CameraMove m_cameraMove;
	PlayerMove m_roboMove;
	PlayerRotation m_roboRotation;

	PlayerAnimation m_playerAnim;
	AnimationClip m_animClip[animNum];
	
	std::vector<MachinGun*> m_machingun = { nullptr };
};

