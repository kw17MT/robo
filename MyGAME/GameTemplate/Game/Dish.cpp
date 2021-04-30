#include "stdafx.h"
#include "Dish.h"
#include "SkinModelRender.h"
#include "Level.h"
#include "Guzai.h"
#include "PlayerGene.h"

bool Dish::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/egg.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	SetScale({ 1.0f,0.2f,1.0f });

	playerGene = FindGO<PlayerGene>("playerGene");
	//‰Šú‚Ì‹ïŞ‚ğoŒ»‚³‚¹‚é
	//m_guzai = NewGO<Guzai>(0);
	//m_guzaiPos = m_position;
	//m_guzaiPos.y += 10.0f;
	//m_guzai->SetPosition(m_guzaiPos);

	//isHavingGuzai = true;

	return true;
}

void Dish::Update()
{
	time++;
	if (time > 0 && time < 10) {
		m_position.x += 2.0f;
	}
	if (time > 10 && time < 20) {
		m_position.x -= 2.0f;
	}
	if (time == 20) {
		time = 0;
	}

	if (playerGene->GetPlayerGeneState() == false) {
		if (isHavingGuzai == false) {
			m_guzai = NewGO<Guzai>(0);
			isHavingGuzai = true;
		}
	}

	if (isHavingGuzai == true) {
		m_guzaiPos = m_position;
		m_guzaiPos.y += 10.0f;
		m_guzai->SetPosition(m_guzaiPos);
	}

	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetPosition(m_position);
}