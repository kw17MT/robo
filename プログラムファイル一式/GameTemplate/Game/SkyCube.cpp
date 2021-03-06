#include "stdafx.h"
#include "SkyCube.h"
#include "SkinModelRender.h"

namespace
{
	const Vector3 SKY_SCALE = { 100000.0f,100000.0f,100000.0f };			//空の拡大率
}

SkyCube::~SkyCube()
{
	//モデルの消去
	DeleteGO(m_skinModelRender);
}

bool SkyCube::Start()
{
	//空生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitSkyCube("Assets/modelData/preset/sky.tkm", enModelUpAxisZ);
	m_skinModelRender->SetPosition(Vector3::Zero);
	//拡大
	m_skinModelRender->SetScale(SKY_SCALE);
	//このモデルは空であることを設定
	m_skinModelRender->SetIsSky();

	return true;
}

void SkyCube::Update()
{

}