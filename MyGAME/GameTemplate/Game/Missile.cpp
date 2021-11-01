#include "stdafx.h"
#include "Missile.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "CaptureStateManager.h"

namespace
{
	const int MISSILE_SPEED = 25.0f;
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
	m_skinModelRender->SetScale({ 10.0f,10.0f,10.0f });

	return true;
}

Vector3 Missile::CalcToTargetVec()
{
	//���K�����ꂽ�^�[�Q�b�g�n�_�ւ̃x�N�g�����v�Z����
	Vector3 toTargetVec = m_enemy->GetPosition() - m_position;
	//Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Missile::Update()
{
	m_targetPos = CaptureStateManager::GetInstance().GetRocketTargetEnemyPos(m_number);
	//�G�܂ł̋�������ړ������Ƒ��x���v�Z����B
	m_moveDirection = CalcToTargetVec();
	m_moveSpeed = m_moveDirection * MISSILE_SPEED;

	
	

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

	//���b�N�I�����Ă����G���|���ꂽ��
	if (m_enemy == nullptr)
	{
		//�|�����O��
		m_moveSpeed = m_prevMoveSpeed;
	}
	else
	{
		m_prevMoveSpeed = m_moveSpeed;
	}

	m_rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_moveDirection);
	m_skinModelRender->SetRotation(m_rot);



}