#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"
#include "MachinGun.h"
#include "CaptureStateManager.h"

Player::~Player()
{
	for (int i = 0; i < m_machingun.size(); i++)
	{
		DeleteGO(m_machingun.back());
		m_machingun.pop_back();
	}
	m_machingun.clear();
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//�X�L�����쐬
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//�X�L���̏���������
	m_skinModelRender->Init("Assets/modelData/testBox/a12.tkm", "Assets/modelData/testBox/a12.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);

	m_animClip[enIdle].Load("Assets/modelData/testBox/a12.tka");
	m_animClip[enIdle].SetLoopFlag(true);
	m_animClip[enForwardLeaning].Load("Assets/modelData/testBox/a12_2_transform.tka");
	m_animClip[enForwardLeaning].SetLoopFlag(false);
	m_animClip[enFlying].Load("Assets/modelData/testBox/anim_fly_only.tka");
	m_animClip[enFlying].SetLoopFlag(true);
	m_animClip[enShooting].Load("Assets/modelData/testBox/a12_4_shootw.tka");
	m_animClip[enShooting].SetLoopFlag(true);
	m_animClip[enBackLeaning].Load("Assets/modelData/testBox/a12_5_backLean.tka");
	m_animClip[enBackLeaning].SetLoopFlag(false);
	m_animClip[enBacking].Load("Assets/modelData/testBox/anim_back_keep.tka");
	m_animClip[enBacking].SetLoopFlag(true);
	m_animClip[enUp].Load("Assets/modelData/testBox/anim_up.tka");
	m_animClip[enUp].SetLoopFlag(true);
	m_skinModelRender->InitAnimation(m_animClip, animNum);




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
	m_playerAnim.UpdateAnimState();
	m_skinModelRender->PlayAnimation(m_playerAnim.GetPlayerState(), 1.0f);
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
		m_skinModelRender->PlayAnimation(1, 1);
		m_machingun.push_back(NewGO<MachinGun>(0));
		m_machingun.back()->SetPosition(m_currentPosition);
		m_machingun.back()->SetTargetPosition(CaptureStateManager::GetInstance().GetCapturedEnemyPos());
	}
}