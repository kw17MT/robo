#include "stdafx.h"
#include "Dish.h"
#include "SkinModelRender.h"
#include "Level.h"
#include "Guzai.h"
#include "PlayerGene.h"
#include "PathMove.h"
#include "DishSpeedManeger.h"

namespace
{
	const float MOVESPEED = 130.0f;
}

bool Dish::Start()
{
	m_lig.ambientLight.Set(0.98f, 0.98f, 0.98f);
	m_lig.directionalLight.direction.x = 1.0f;
	m_lig.directionalLight.direction.y = 1.0f;
	m_lig.directionalLight.direction.z = 1.0f;
	m_lig.directionalLight.color.Set(100.0f, 100.0f, 100.0f);
	m_lig.directionalLight.direction.Normalize();

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/object/dish.tkm", nullptr, enModelUpAxisZ, m_position, &m_lig);
	m_scale = { 1.0f,1.0f,1.0f };
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);



	playerGene = FindGO<PlayerGene>("playerGene");
	m_speedManeger = FindGO<DishSpeedManeger>("speedManeger");

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
		//m_guzaiPos.y += 10.0f;
		m_guzaiPos.y += m_guzaiYPos;
		m_guzai->SetPosition(m_guzaiPos);
		if (m_guzaiYPos > 800.0f) {
			m_guzaiYPos -= 4.0f;
		}
		else if (m_guzaiYPos > 10.0f) {
			m_guzaiYPos -= 20.0f;
		}
	}
	else {
		m_guzaiYPos = 1000.0f;
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
	
	//Ž©•ª‚Ìã‚Ì‹ïÞ‚ªŽ‚½‚ê‚Ä‚¢‚é‚È‚ç‚Î
	if (m_guzai->state == 1) {
		isHavingGuzai = false;
	}

	//‹ó‚ÌŽM‚ª‹K’è”‚æ‚è‘½‚¢‚Ì‚ÅA‹ó‚ÌŽM‚¾‚¯‚É•â[‚ðŠJŽn‚·‚éB
	if (playerGene->GetNoHavingDishCounter() >= maxNum2Refill) {
		m_guzaiTimer++;
		if (m_guzaiTimer > 60) {
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
			m_guzaiTimer = 0;
		}
	}

	if (m_speedManeger->GetSpeedUpState() == true) {
		float moveSpeed = MOVESPEED * 2.0f;
		m_pathMove.get()->ChangeMoveSpeed(moveSpeed);
	}
	if (m_speedManeger->GetSpeedUpState() == false) {
		m_pathMove.get()->ChangeMoveSpeed(MOVESPEED);
	}


	Move();

	m_skinModelRender->SetScale(m_scale);
}