#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };
	m_prevPosition = m_currentPosition;
	m_skinModelRender->SetPosition(m_currentPosition);
	Vector3 pos = m_currentPosition;
	m_front = g_camera3D->GetForward();

	pos.y += 1000.0f;
	g_camera3D->SetTarget(pos);
	pos.z += 4000.0f;
	g_camera3D->SetPosition(pos);
	return true;
}

void Player::Update()
{
	//���݂̃v���C���[�̃|�W�V�������X�V
	m_currentPosition = m_roboMove.Execute(m_currentPosition);
	m_currentFront = g_camera3D->GetForward();
	//�X�V�������W�����f���ɓK�p
	m_skinModelRender->SetPosition(m_currentPosition);
	//�v���C���[�����삷��J�����̓���
	m_cameraMove.UpdatePlayerCamera(m_prevPosition, m_currentPosition);

	m_roboRotation.UpdateRotation(m_skinModelRender, m_prevPosition, m_currentPosition, m_front);
	//m_skinModelRender->SetRotation(RoboRotation::RotationX(m_skinModelRender->GetRotation()));
	//�_�b�V��������Ȃ����
	/*if (!m_dash)
	{
		CameraMove::FollowPlayer(m_position);
	}*/

	//1�t���[���O�̍��W�Ƃ��ċL�^
	m_prevPosition = m_currentPosition;
	m_prevFront = m_currentFront;
}