#include "stdafx.h"
#include "Dish.h"
#include "SkinModelRender.h"
#include "Level.h"
#include "Guzai.h"
#include "PlayerGene.h"
#include "PathMove.h"

namespace
{
	const float MOVESPEED = 130.0f;
}

bool Dish::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/egg.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	SetScale({ 1.0f,0.2f,1.0f });

	playerGene = FindGO<PlayerGene>("playerGene");
	
	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_position, MOVESPEED, enNormalLane);

	return true;
}

void Dish::Move()
{
	//ŽM‚ðƒEƒFƒCƒ|ƒCƒ“ƒg‚ÅˆÚ“®‚³‚¹‚éB
	m_position = m_pathMove.get()->Move();
	m_skinModelRender->SetPosition(m_position);
	//ÅŒã‚Ìƒ|ƒCƒ“ƒg‚Ü‚Å“ž’B‚µ‚½‚çB
	if (m_pathMove.get()->GetIsFinalPoint())
	{
	}

	//‹ïÞ‚ðŽM‚Ìã‚ÉˆÚ“®‚³‚¹‚é
	if (isHavingGuzai == true) {
		m_guzaiPos = m_position;
		m_guzaiPos.y += 10.0f;
		m_guzai->SetPosition(m_guzaiPos);
	}
}

void Dish::Update()
{
	//Å‰‚¾‚¯‚·‚×‚Ä‚ÌŽM‚Ìã‚Éo‚·B
	if (isCompletedFirstPop == false) {
		if (playerGene->GetPlayerGeneState() == false) {
				m_guzai = NewGO<Guzai>(0);
				isHavingGuzai = true;

				isCompletedFirstPop = true;
		}
	}

	

		//‚±‚Ì‚Ü‚Ü‚¾‚ÆA‚Æ‚Á‚½uŠÔ‚É‚Ü‚½o‚µ‚Ä‚µ‚Ü‚¤
		//‹ïÞ‚ªŽ‚½‚ê‚Ä‚¢‚½‚çAŽM‚É‹ïÞ‚ª‚È‚¢‚±‚Æ‚ð’m‚ç‚¹‚éB
		//‹ïÞ‚ª‚Æ‚ç‚ê‚Ä‚¢‚éŠÔ‚¸‚Á‚ÆƒCƒ“ƒNƒŠƒƒ“ƒg‚µ‚Ä‚µ‚Ü‚¤‚½‚ß•Ï”‚ª‘å‚«‚­‚È‚è‚·‚¬‚Ä‚·‚®‚É•â[‚µ‚Ä‚µ‚Ü‚¤

		//‹ïÞƒfƒXƒgƒ‰ƒNƒ^‚ÅnoHavingDish‚Ì•Ï”‚ð1“xƒCƒ“ƒNƒŠƒƒ“ƒg‚µ‚Ä‚¢‚éB
		if (m_guzai->state == 1) {
			isHavingGuzai = false;
			//playerGene->AddNoHavingDishCounter();
		}

		//‹ó‚ÌŽM‚ª‹K’è”‚æ‚è‘½‚¢‚Ì‚ÅA‹ó‚ÌŽM‚¾‚¯‚É•â[‚ðŠJŽn‚·‚éB
		if (playerGene->GetNoHavingDishCounter() >= maxNum2Refill) {
			if (isHavingGuzai == false) {
				m_guzai = NewGO<Guzai>(0);
				isHavingGuzai = true;
				
				//•â[‚µ‚½ŽM‚Ì–‡”‚ð‚P‘«‚·
				playerGene->AddRefilledNum();
			}
			//•â[‚µ‚½ŽM‚Ì”‚ª‹ó‚¾‚Á‚½ŽM‚Ì”‚Æ“¯‚¶‚É‚È‚Á‚½‚çA‚O‚Å‰Šú‰»
			if (playerGene->GetRefilledNum() >= maxNum2Refill) {
				playerGene->ResetNohavingDishCounter();
				playerGene->ResetRefilledNum();
			}
		}

		Move();

	m_skinModelRender->SetScale(m_scale);
}