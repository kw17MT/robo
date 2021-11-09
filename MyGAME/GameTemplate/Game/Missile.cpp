#include "stdafx.h"
#include "Missile.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "CaptureStateManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace
{
	const float MISSILE_SPEED = 150.0f;
	const float DEPLOY_SPEED = 40.0f;
}

Missile::~Missile()
{
	//�o�����Ă��郍�P�b�g�̑������f�N�������g
	CaptureStateManager::GetInstance().MinusRockeTargetNum();

	DeleteGO(m_skinModelRender);
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/missile/missile.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 20.0f,20.0f,20.0f });

	m_deployDirection = CalcDeployDirection();
	m_prevMoveDirection = m_deployDirection;
	return true;
}

Vector3 Missile::CalcDeployDirection()
{
	//��ʂ̑O�������擾
	Vector3 front = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	Vector3 up = front.CalcCross(right);

	srand((unsigned int)time(NULL));
	float degreeX = rand() % 90 + 1;
	float degreeY = rand() % 90 + 1;
	degreeX -= 45.0f; degreeY -= 45.0f;
	/*if ((int)degree % 2)
	{
		degree *= -1.0f;
	}*/
	Quaternion qRot;
	qRot.SetRotationDeg(right, degreeY);
	qRot.Apply(front);
	qRot.SetRotationDeg(up, degreeX);
	qRot.Apply(front);

	return front;
}

Vector3 Missile::CalcToTargetVec()
{
	//���K�����ꂽ�^�[�Q�b�g�n�_�ւ̃x�N�g�����v�Z����
	Vector3 toTargetVec = m_enemy->GetPosition() - m_position;
	//Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Missile::RestrictRotation()
{
	//���b�N�I�����Ă����G���|���ꂽ��
	if (m_enemy->IsDead())
	{
		//�|�����O�̓G�ւ̕������g��
		m_moveDirection = m_prevMoveDirection;
	}
	else
	{
		//�i�s���Ă������������݂̃~�T�C���̑O�����ƃ^�[�Q�b�g�ւ̃x�N�g�����ς����߂�B
		if (float a = m_moveDirection.Dot(m_prevMoveDirection) > 0.8f)
		{
		}
		else
		{
			//�^�[�Q�b�g�ւ̃x�N�g���ƃ~�T�C���̑O�����̏�������v�Z
			Vector3 up = m_moveDirection.CalcCross(m_prevMoveDirection);
			Quaternion rot;
			rot.SetRotationDeg(up, 80.0f);
			rot.Apply(m_moveDirection);
		}
		m_prevMoveDirection = m_moveDirection;
	}
}

void Missile::Update()
{
	switch (m_moveStage)
	{
	//���P�b�g���˒���̃����_�������ւ̓W�J
	case enDeploying:
		//�ŏ��Ɍv�Z�����W�J�������g�p���ă~�T�C����W�J
		m_moveSpeed = m_deployDirection * DEPLOY_SPEED;
		//1�b���炢��������
		if (count >= 1.0f)
		{
			//���i���[�h�ɐ؂�ւ�
			m_moveStage = enChaseTarget;//enStraightTarget;
		}
		//1�t���[���O�̈ړ������Ƃ��ĕۑ�
		m_prevMoveDirection = m_deployDirection;
		break;
	//�W�J��͏����̊ԓG�ɒ��i����悤��
	case enStraightTarget:
		//�G�ւ̒��i�������v�Z
		m_moveDirection = CalcToTargetVec();
		//1�t���[���O�̈ړ������Ƃ��ĕۑ�
		m_prevMoveDirection = m_moveDirection;
		//���i����ړ����x���v�Z
		m_moveSpeed = m_moveDirection * MISSILE_SPEED;
		//�ǐՃ��[�h�ɐ؂�ւ�
		m_moveStage = enChaseTarget;
		break;
	//�G���������s����܂ŒǐՂ���
	case enChaseTarget:
		//�G�܂ł̋�������ړ������Ƒ��x���v�Z����B
		m_moveDirection = CalcToTargetVec();
		//��]�̗}��
		m_prevMoveDirection = m_moveDirection;
		//RestrictRotation();
		//�}���ς݂̈ړ�������p���đ��x���v�Z
		m_moveSpeed = m_moveDirection * MISSILE_SPEED;
		break;
	}

	//���x���l�������ʒu���W��ݒ肷��B
	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);

	//�X�V�����ʒu���W�Ƃ��ׂĂ̓G�Ƃ̋��������߂�
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//�G�ƒe�̋������v�Z����B
		Vector3 diff = enemy->GetPosition() - m_position;
		if (diff.Length() < 200.0f) {
			//���P�b�g����_���[�W���󂯂����Ƃ�m�点��
			enemy->TakenDamage(enMissile);
			//���S�B
			DeleteGO(this);
			//�I���B
			return false;
		}
		//�N�G���͌p���B
		return true;
		});

	//�e�̎���
	count += GameTime().GetFrameDeltaTime();
	if (count >= 30.0f)
	{
		DeleteGO(this);
	}

	//���݂̈ړ�������p���ă~�T�C�����f���̉�]���s��
	m_rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_moveDirection);
	m_skinModelRender->SetRotation(m_rot);
}