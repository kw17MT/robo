#include "stdafx.h"
#include "EnemyMachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 0.2f,0.2f,0.2f };
}

EnemyMachinGun::~EnemyMachinGun()
{
	//���f���̍폜
	DeleteGO(m_skinModelRender);
}

bool EnemyMachinGun::Start()
{
	//�}�V���K���̃��f������
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	return true;
}

void EnemyMachinGun::Update()
{
	if (m_canShoot)
	{
		//�e�𐶐��i���ԂŎ����ŏ�����j
		Bullet* bullet = NewGO<Bullet>(0);
		//�^�[�Q�b�g�̈ʒu�Ǝ����̈ʒu��^����
		bullet->SetTargetAndCurrentPos(m_targetPos, m_position);
		//�e�𔭎˂����̂͒N�����L�^����
		bullet->SetOwner(enEnemy);
		Quaternion rot;
		//���f���������Ō����Ă����������^�[�Q�b�g�ւ̕����։�]������
		rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_targetPos - m_position);
		bullet->SetRotation(rot);
		//�A���Ō��ĂȂ��悤�ɂ���
		m_canShoot = false;
	}
	//���f���̈ʒu���X�V����
	m_skinModelRender->SetPosition(m_position);
}