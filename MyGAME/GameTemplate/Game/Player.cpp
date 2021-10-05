#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "RoboMove.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0, "player");
	m_skinModelRender->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };
	m_skinModelRender->SetPosition(m_currentPosition);
	

	g_camera3D->SetTarget(m_currentPosition);
	return true;
}

void Player::Update()
{
	//���݂̃v���C���[�̃|�W�V�������X�V
	m_currentPosition = RoboMove::Move(m_currentPosition);
	//�X�V�������W�����f���ɓK�p
	m_skinModelRender->SetPosition(m_currentPosition);
	//�v���C���[�����삷��J�����̓���
	CameraMove::UpdatePlayerCamera(m_prevPosition, m_currentPosition);;
	
	//�_�b�V��������Ȃ����
	/*if (!m_dash)
	{
		CameraMove::FollowPlayer(m_position);
	}*/

	//1�t���[���O�̍��W�Ƃ��ċL�^
	m_prevPosition = m_currentPosition;
}