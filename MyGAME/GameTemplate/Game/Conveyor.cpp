#include "stdafx.h"
#include "Conveyor.h"
#include "SkinModelRender.h"

bool Conveyor::Start()
{
	//コンベアのインスタンス作成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//シャドウレシーバ―として初期化
	m_skinModelRender->InitForRecieveShadow("Assets/modelData/Conveyor.tkm", nullptr, enModelUpAxisZ, m_position, nullptr);

	return true;
}

void Conveyor::Update()
{
	//位置の設定
	m_skinModelRender->SetPosition(m_position);
	//拡大率の設定
	m_skinModelRender->SetScale(m_scale);
}