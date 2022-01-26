#include "stdafx.h"
#include "Rader.h"
#include "SpriteRender.h"
#include "EnemyIconOnRader.h"

namespace
{
	const float ICON_SIZE = 12;													//���[�_�[��̃A�C�R���̂�������
	const Vector3 DIRECTION_Z = { 0.0f,0.0f,1.0f };								//Z����
	const Vector3 SPRITE_DIRECTION_UP = { 0.0f,-1.0f,0.0f };					//�摜�̏����
	const Vector3 BASE_SPRITE_POSITION = { 520.0f, -240.0f, 0.0f };				//�摜�̒��S��u����ʒu
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);
extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

Rader::~Rader()
{
	//���[�_�[��̓G�̃A�C�R�������ׂč폜
	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_enemyIcon[i]);
	}
	//���[�_�[��̃v���C���[�̃A�C�R�����폜
	DeleteGO(m_playerIcon);
	DeleteGO(m_rader);
}

bool Rader::Start()
{
	//���[�_�[�쐬
	m_rader = NewGO<SpriteRender>(0);
	m_rader->Init("Assets/Image/rader/rader.dds", 200, 200);
	m_rader->SetPosition(BASE_SPRITE_POSITION);

	//���[�_�[��̓G�̃A�C�R�����ő吔�쐬
	for (int i = 0; i < 10; i++)
	{
		m_enemyIcon[i] = NewGO<EnemyIconOnRader>(0);
	}

	//���[�_�[��̃v���C���[�̃A�C�R���𐶐�
	m_playerIcon = NewGO<SpriteRender>(0);
	m_playerIcon->Init("Assets/Image/rader/playerIcon.dds", ICON_SIZE, ICON_SIZE);
	//�摜����ʂ̍������炢�ɗ���悤�ɐݒ�
	m_playerIcon->SetScale(Vector3::One);
	m_playerIcon->SetPosition(BASE_SPRITE_POSITION);

	return true;
}

void Rader::SaveEnemyPosAndIsAlive(int no, Vector4 enemyPos)
{
	//no�Ԗڂ̔z��ɓG�̍��W����
	m_enemyPosAndIsAlive[no] = enemyPos;
}

void Rader::Update()
{


	for (int i = 0; i < m_enemyNum; i++)
	{
		if (m_enemyPosAndIsAlive[i].w == false)
		{
			m_enemyIcon[i]->SetScale({ 0.0f,0.0f,0.0f });
			continue;
		}

		//�G�̈ʒu��XYZ���W�n�ɓ��꒼��
		Vector3 enemyPos;
		enemyPos.x = m_enemyPosAndIsAlive[i].x; enemyPos.y = m_enemyPosAndIsAlive[i].y; enemyPos.z = m_enemyPosAndIsAlive[i].z;

		Quaternion qRot;
		//���݂̃v���C���[�̌������烏�[���h��Z�����ւ̉�]���쐬
		Vector3 ForwardWithoutY = g_camera3D->GetForward();
		ForwardWithoutY.y = 0.0f;
		qRot.SetRotation(ForwardWithoutY, DIRECTION_Z);

		//�v���C���[����G�Ɍ������x�N�g��
		Vector3 playerToEnemyVec = enemyPos - m_playerPos;
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
		float distance = CalcMethods::CalcDistance(enemyPos, m_playerPos);
		//���[�_�[�ɂ��傤�ǂ�����ɒ���
		distance /= 100.0f;
		//�l�𐧌�
		if (distance >= 70.0f)distance = 70.0f;
		//�ʒu�֌W�Ƌ�������A�C�R����u�������ꏊ���v�Z
		m_screenPos = playerToEnemyVec * distance;

		//1�t���[���O�̈ʒu�ƌ��݂̈ʒu����ړ����x�����߂�
		Vector3 speed = enemyPos - m_prevEnemyPos[i];
		////�摜���W�ɕϊ����������߁A�l����ꂩ��
		std::swap(speed.y, speed.z);
		speed.Normalize();
		//z���W�͕s�v
		speed.z = 0.0f;
		//x�ɂ��ĉ摜���W�ɒ���
		speed.x *= -1.0f;
		//�ړ������ւ̉�]
		Quaternion moveDirectionRot;
		moveDirectionRot.SetRotation(SPRITE_DIRECTION_UP, speed);

		//�J�����̏�������擾
		Vector3 cameraFront = g_camera3D->GetForward();
		std::swap(cameraFront.y, cameraFront.z);
		cameraFront.Normalize();
		cameraFront.z = 0.0f;
		cameraFront.x *= -1.0f;
		//�J�����̉�]���v�Z
		qRot.SetRotation(cameraFront, SPRITE_DIRECTION_UP);
		//�J�����̉�]�ɉ摜���̂̉�]���|�����킹�A�ŏI�I�ȉ�]���v�Z
		moveDirectionRot.Multiply(qRot);
		m_enemyIcon[i]->SetRotation(moveDirectionRot);

		//1�t���[���O�̍��W�Ƃ��Đݒ�
		m_prevEnemyPos[i] = enemyPos;

		//�摜�̈ʒu��ݒ�
		m_screenPos += BASE_SPRITE_POSITION;
		m_enemyIcon[i]->SetPosition(m_screenPos);
		//�g�嗦��1�ɂ��ĉ�ʏ�Ō����悤�ɂ���
		m_scale = Vector3::One;
		//�摜�̊g�嗦���X�V
		m_enemyIcon[i]->SetScale(m_scale);
	}
}