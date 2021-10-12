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

	pos.y += 2000.0f;
	g_camera3D->SetTarget(pos);
	pos.z += 4000.0f;
	g_camera3D->SetPosition(pos);
	return true;
}

void Player::Update()
{
	//���݂̃v���C���[�̃|�W�V�������X�V
	m_currentPosition = m_roboMove.Execute(m_currentPosition);
	
	//�v���C���[�����삷��J�����̓���
	m_cameraMove.UpdatePlayerCamera(m_prevPosition, m_currentPosition);

	m_roboRotation.UpdateRotation(m_skinModelRender);

	//�X�V�������W�����f���ɓK�p
	m_skinModelRender->SetPosition(m_currentPosition);
	//1�t���[���O�̍��W�Ƃ��ċL�^
	m_prevPosition = m_currentPosition;
}