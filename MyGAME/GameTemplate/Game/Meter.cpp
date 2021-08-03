#include "stdafx.h"
#include "Meter.h"
#include "SkinModelRender.h"

namespace 
{
	const int USING_LIGHT_NUMBER = 2;
}

Meter::~Meter()
{
	//モデルの消去
	DeleteGO(m_skinModelRender);
}

bool Meter::Start()
{

	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画モデルの初期化
	m_skinModelRender->InitAsGauge("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position, USING_LIGHT_NUMBER);
	//影用モデルの初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position);
	//モデルの向きを調節
	m_skinModelRender->SetRotation(m_rot);

	return true;
}

void Meter::Update()
{
	//拡大率の変更
	m_skinModelRender->SetScale(m_scale);
	//位置の変更
	m_skinModelRender->SetPosition(m_position);
	//回転の変更
	m_skinModelRender->SetRotation(m_rot);
}