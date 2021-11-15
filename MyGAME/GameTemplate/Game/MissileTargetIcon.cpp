#include "stdafx.h"
#include "MissileTargetIcon.h"
#include "SpriteRender.h"
#include "Enemy.h"

namespace
{
	const int ICON_SIZE = 96;									//�A�C�R���̑傫��
	const Vector3 EXPANDED_SCALE = { 2.0f,2.0f,1.0f };			//�g�債�����̑傫��
	const Vector3 SHRINK_RATE = { 0.1f, 0.1f, 0.0f };			//�k�����x
	const float DECREASE_ANGLE_RATE = 9.0f;						//�p�x�̌������x
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

MissileTargetIcon::~MissileTargetIcon()
{
	//�A�C�R���摜���폜
	DeleteGO(m_spriteRender);
}

bool MissileTargetIcon::Start()
{
	//�A�C�R������
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticle_missile.dds", ICON_SIZE, ICON_SIZE);

	return true;
}

void MissileTargetIcon::Update()
{
	//�G�̃X�N���[�����W���v�Z����
	CalcMethods::CalcScreenPos(m_screenPos, m_enemy->GetPosition());

	//�A�C�R�����o�������͊g�債����Ԃŏo���B
	if (m_isFirstExpand)
	{
		//�g��
		m_scale = EXPANDED_SCALE;
		//90�x��]��������Ԃɂ���
		m_angle = 90.0f;
		//�g����s��Ȃ��悤��
		m_isFirstExpand = false;
	}
	//�k�������Ă���
	m_scale -= SHRINK_RATE;
	//�p�x�����炵�Ă���
	m_angle -= DECREASE_ANGLE_RATE;

	if (m_scale.x <= 1.0f)
	{
		m_scale = Vector3::One;
	}
	if (m_angle <= 0.0f)
	{
		m_angle = 0.0f;
	}

	//��]���쐬
	qRot.SetRotationDegZ(m_angle);
	//�摜����]������
	m_spriteRender->SetRotation(qRot);
	//�摜�̈ʒu���X�V
	m_spriteRender->SetPosition(m_screenPos);
	//�摜�̊g�嗦���X�V
	m_spriteRender->SetScale(m_scale);
}