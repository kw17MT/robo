#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"
#include "MachinGun.h"
#include "CaptureStateManager.h"
#include "MissileGenerator.h"
#include "Reticle.h"

Player::~Player()
{
	DeleteGO(m_machingun);
	DeleteGO(m_skinModelRender);
}

bool Player::Start()
{
	//�X�L�����쐬
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//�X�L���̏���������
	//m_skinModelRender->Init("Assets/modelData/testBox/test.tkm", "Assets/modelData/testBox/test.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", "Assets/modelData/robo/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);

	m_animClip[enIdle].Load("Assets/animData/robo/anim_idle.tka");
	m_animClip[enIdle].SetLoopFlag(true);
	m_animClip[enForwardLeaning].Load("Assets/animData/robo/anim_transform.tka");
	m_animClip[enForwardLeaning].SetLoopFlag(false);
	m_animClip[enFlying].Load("Assets/animData/robo/anim_fly_only.tka");
	m_animClip[enFlying].SetLoopFlag(true);
	m_animClip[enShooting].Load("Assets/animData/robo/anim_shoot.tka");
	m_animClip[enShooting].SetLoopFlag(true);
	m_animClip[enBackLeaning].Load("Assets/animData/robo/anim_backLean.tka");
	m_animClip[enBackLeaning].SetLoopFlag(false);
	m_animClip[enBacking].Load("Assets/animData/robo/anim_back_keep.tka");
	m_animClip[enBacking].SetLoopFlag(true);
	m_animClip[enUp].Load("Assets/animData/robo/anim_up.tka");
	m_animClip[enUp].SetLoopFlag(true);
	m_skinModelRender->InitAnimation(m_animClip, animNum);


	m_machingun = NewGO<MachinGun>(0);
	m_missileGene = NewGO<MissileGenerator>(0);

	m_reticle = NewGO<Reticle>(0);

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
	//�v���C���[�̃A�j���[�V�������s��
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

	//�}�V���K���Ƀ^�[�Q�b�g�ʒu�ƃv���C���[�̌��݂̈ʒu��^����
	m_machingun->SetTargetAndCurrentPos(CaptureStateManager::GetInstance().GetCapturedEnemyPos(), m_currentPosition);
	m_missileGene->SetLaunchPos(m_currentPosition);
}