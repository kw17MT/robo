#include "stdafx.h"
#include "SkyCube.h"
#include "SkinModelRender.h"

namespace
{
	const Vector3 SKY_SCALE = { 100000.0f,100000.0f,100000.0f };			//‹ó‚ÌŠg‘å—¦
}

SkyCube::~SkyCube()
{
	//ƒ‚ƒfƒ‹‚ÌÁ‹
	DeleteGO(m_skinModelRender);
}

bool SkyCube::Start()
{
	//‹ó¶¬
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitSkyCube("Assets/modelData/preset/sky.tkm", enModelUpAxisZ);
	m_skinModelRender->SetPosition(Vector3::Zero);
	//Šg‘å
	m_skinModelRender->SetScale(SKY_SCALE);
	//‚±‚Ìƒ‚ƒfƒ‹‚Í‹ó‚Å‚ ‚é‚±‚Æ‚ğİ’è
	m_skinModelRender->SetIsSky();

	return true;
}

void SkyCube::Update()
{

}