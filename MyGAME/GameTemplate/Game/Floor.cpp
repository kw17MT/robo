#include "stdafx.h"
#include "Floor.h"
#include "SkinModelRender.h"

namespace
{
	const Vector3 AJUST_LIGHT_RIGHTER = { 4.5f,4.5f,4.5f };
}

Floor::~Floor()
{
	DeleteGO(m_skinModelRender);
}

bool Floor::Start()
{
	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	m_skinModelRender = NewGO<SkinModelRender>(0);
	if (isRed) {
		m_skinModelRender->InitAsFloor("Assets/modelData/floor/floor_red.tkm", nullptr, enModelUpAxisZ, m_position);
	}
	else {
		m_skinModelRender->InitAsFloor("Assets/modelData/floor/floor_blue.tkm", nullptr, enModelUpAxisZ, m_position);
	}

	return true;
}
 
void Floor::Update()
{
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);
}