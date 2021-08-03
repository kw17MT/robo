#include "stdafx.h"
#include "Floor.h"
#include "SkinModelRender.h"

Floor::~Floor()
{
	DeleteGO(m_skinModelRender);
}

bool Floor::Start()
{
	//モデルの初期化
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//赤色の床を出すならば
	if (m_isRed) {
		m_skinModelRender->InitAsFloor("Assets/modelData/floor/floor_red.tkm", nullptr, enModelUpAxisZ, m_position);
	}
	//青色の床を出すならば
	else {
		m_skinModelRender->InitAsFloor("Assets/modelData/floor/floor_blue.tkm", nullptr, enModelUpAxisZ, m_position);
	}

	return true;
}
 
void Floor::Update()
{
	//位置を更新
	m_skinModelRender->SetPosition(m_position);
	//拡大率を更新
	m_skinModelRender->SetScale(m_scale);
}