#include "stdafx.h"
#include "SkyCube.h"
#include "SkinModelRender.h"

namespace
{
	const Vector3 SKY_SCALE = { 100000.0f,100000.0f,100000.0f };			//��̊g�嗦
}

SkyCube::~SkyCube()
{
	//���f���̏���
	DeleteGO(m_skinModelRender);
}

bool SkyCube::Start()
{
	//�󐶐�
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitSkyCube("Assets/modelData/preset/sky.tkm", enModelUpAxisZ);
	m_skinModelRender->SetPosition(Vector3::Zero);
	//�g��
	m_skinModelRender->SetScale(SKY_SCALE);
	//���̃��f���͋�ł��邱�Ƃ�ݒ�
	m_skinModelRender->SetIsSky();

	return true;
}

void SkyCube::Update()
{

}