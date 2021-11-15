#include "stdafx.h"
#include "Rader.h"
#include "SpriteRender.h"

namespace
{
	const float ICON_SIZE = 4;
	const Vector2 PIVOT = { -125.0f, 50.0f };
	const Vector3 DIRECTION_Z = { 0.0f,0.0f,1.0f };
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);
extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

Rader::~Rader()
{
	//���[�_�[��̓G�̃A�C�R�������ׂč폜
	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_spriteRender[i]);
	}
	//���[�_�[��̃v���C���[�̃A�C�R�����폜
	DeleteGO(m_playerIcon);
}

bool Rader::Start()
{
	//���[�_�[��̓G�̃A�C�R�����ő吔�쐬
	for (int i = 0; i < 10; i++)
	{
		m_spriteRender[i] = NewGO<SpriteRender>(0);
		m_spriteRender[i]->Init("Assets/Image/HPBar/HPBar.dds", ICON_SIZE, ICON_SIZE);
		//�摜����ʂ̍������炢�ɗ���悤�ɐݒ�
		m_spriteRender[i]->SetPivot(PIVOT);
		m_spriteRender[i]->SetScale(m_scale);
	}

	//���[�_�[��̃v���C���[�̃A�C�R���𐶐�
	m_playerIcon = NewGO<SpriteRender>(0);
	m_playerIcon->Init("Assets/Image/HPBar/HPBar.dds", ICON_SIZE, ICON_SIZE);
	//�摜����ʂ̍������炢�ɗ���悤�ɐݒ�
	m_playerIcon->SetPivot(PIVOT);
	m_playerIcon->SetScale(Vector3::One);
	//���W�͈ȍ~�Œ�
	m_playerIcon->SetPosition(Vector3::Zero);

	return true;
}

void Rader::SaveEnemyPos(int no, Vector3 enemyPos)
{
	m_enemyPos[no] = enemyPos;
}

void Rader::Update()
{
	Quaternion qRot;
	//���݂̃v���C���[�̌������烏�[���h��Z�����ւ̉�]���쐬
	qRot.SetRotation(g_camera3D->GetForward(), DIRECTION_Z);

	for (int i = 0; i < m_enemyNum - 1; i++)
	{
		//�v���C���[����G�Ɍ������x�N�g��
		Vector3 playerToEnemyVec = m_enemyPos[i] - m_playerPos;
		//���K��
		playerToEnemyVec.Normalize();
		//y���W�͕s�v
		playerToEnemyVec.y = 0.0f;
		//��]��K�p
		qRot.Apply(playerToEnemyVec);
		//���]�𒲐�
		playerToEnemyVec.x *= -1.0f;
		//���[���h����Ղ����G����ʂɕ\�����������߁Ay��z����ꂩ��
		playerToEnemyVec.y = playerToEnemyVec.z;
		//z�͕s�v
		playerToEnemyVec.z = 0.0f;
		
		//�v���C���[�ƓG�̋����𑪒�
		float distance = CalcMethods::CalcDistance(m_enemyPos[i], m_playerPos);
		//���[�_�[�ɂ��傤�ǂ�����ɒ���
		distance /= 100.0f;
		//�l�𐧌�
		if (distance >= 100.0f)distance = 100.0f;
		//�ʒu�֌W�Ƌ�������A�C�R����u�������ꏊ���v�Z
		m_screenPos = playerToEnemyVec * distance;

		//�摜�̈ʒu��ݒ�
		m_spriteRender[i]->SetPosition(m_screenPos);
		//�g�嗦��1�ɂ��ĉ�ʏ�Ō����悤�ɂ���
		m_scale = Vector3::One;
		//�摜�̊g�嗦���X�V
		m_spriteRender[i]->SetScale(m_scale);
	}
}