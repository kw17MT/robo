#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"
#include "MachinGun.h"
#include "CaptureStateManager.h"

bool Player::Start()
{
	//�X�L�����쐬
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//�X�L���̏���������
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	//�R���W�����̍쐬
	//m_skinModelRender->InitCharaCon(10.0f, 100.0f, m_currentPosition);
	//�ŏ��̃z�[���|�W�V������������
	m_currentHomePosition = m_currentPosition;
	//1�t���[���O�̃z�[���|�W�V������������
	m_prevHomePosition = m_currentHomePosition;
	//���f���������ʒu�ɐݒ�
	m_skinModelRender->SetPosition(m_currentHomePosition);

	//�J�����̈ʒu�A�^�[�Q�b�g��������
	Vector3 cameraPos = m_currentHomePosition;
	cameraPos.y += 50.0f;
	g_camera3D->SetTarget(cameraPos);
	cameraPos.z += 230.0f;
	g_camera3D->SetPosition(cameraPos);

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

	//�v�Z���I������z�[���|�W�V���������Ƃɂ��炵���v���C���[�̈ʒu���쐬����B
	m_currentPosition = m_roboMove.CalcPlayerPos(m_currentHomePosition);
	//���f����V�����ʒu�ɐݒ肷��B
	m_skinModelRender->SetPosition(m_currentPosition);
	//1�t���[���O�̍��W�Ƃ��ċL�^
	m_prevHomePosition = m_currentHomePosition;


	if (g_pad[0]->IsPress(enButtonRB2))
	{
		m_machingun.push_back(NewGO<MachinGun>(0));
		m_machingun.back()->SetPosition(m_currentPosition);
		m_machingun.back()->SetTargetPosition(CaptureStateManager::GetInstance().GetCapturedEnemyPos());
	}
}