#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };

	m_currentHomePosition = m_currentPosition;
	m_prevHomePosition = m_currentHomePosition;

	m_skinModelRender->SetPosition(m_currentHomePosition);
	Vector3 pos = m_currentHomePosition;

	pos.y += 50.0f;
	g_camera3D->SetTarget(pos);
	pos.z += 230.0f;
	g_camera3D->SetPosition(pos);
	return true;
}

void Player::Update()
{
	//�v���C���[�̃z�[���|�W�V�������X�V
	m_currentHomePosition = m_roboMove.Execute(m_currentHomePosition);
	
	//�z�[���|�W�V�����ɂ���ăJ�����𓮂���
	m_cameraMove.UpdatePlayerCamera(m_prevHomePosition, m_currentHomePosition);

	//�E�X�e�B�b�N�̃J�����̉�]�ɏ]�����v���C���[�̉�]
	m_roboRotation.UpdateRotation(m_skinModelRender);

	m_currentPosition = m_roboMove.CalcPlayerPos(m_currentHomePosition);
	m_skinModelRender->SetPosition(m_currentPosition);

	//�X�V�������W�����f���ɓK�p
	//m_skinModelRender->SetPosition(m_homePosition);
	//1�t���[���O�̍��W�Ƃ��ċL�^
	//m_prevPosition = m_currentPosition;
	m_prevHomePosition = m_currentHomePosition;
}