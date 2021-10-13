#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };

	m_homePosition = m_currentPosition;
	m_prevPosition = m_homePosition;

	m_skinModelRender->SetPosition(m_homePosition);
	Vector3 pos = m_homePosition;

	pos.y += 50.0f;
	g_camera3D->SetTarget(pos);
	pos.z += 230.0f;
	g_camera3D->SetPosition(pos);
	return true;
}

void Player::Update()
{
	//�v���C���[�̃z�[���|�W�V�������X�V
	m_homePosition = m_roboMove.Execute(m_homePosition);
	
	//�z�[���|�W�V�����ɂ���ăJ�����𓮂���
	m_cameraMove.UpdatePlayerCamera(m_prevPosition, m_homePosition);

	//�E�X�e�B�b�N�̃J�����̉�]�ɏ]�����v���C���[�̉�]
	m_roboRotation.UpdateRotation(m_skinModelRender);

	m_currentPosition = m_roboMove.CalcPlayerPos(m_homePosition);
	m_skinModelRender->SetPosition(m_currentPosition);

	//�X�V�������W�����f���ɓK�p
	//m_skinModelRender->SetPosition(m_homePosition);
	//1�t���[���O�̍��W�Ƃ��ċL�^
	//m_prevPosition = m_currentPosition;
	m_prevPosition = m_homePosition;
}