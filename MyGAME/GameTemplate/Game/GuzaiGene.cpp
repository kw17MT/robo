#include "stdafx.h"
#include "GuzaiGene.h"
#include "Guzai.h"
#include "Buff.h"
#include "DeBuff.h"
#include "SkinModelRender.h"
#include "DishGene.h"

#include <ctime>        // time
#include <cstdlib>      // srand,rand
#include <random>		//乱数生成用

GuzaiGene::~GuzaiGene()
{
	//DeleteGO(m_skinModelRender);

	//具材を削除
	DeleteGO(m_skinModelRender);
	for (int i = 0; i < guzaiNum; i++) {
		if (m_guzai[i] != nullptr) {
			DeleteGO(m_guzai[i]);
		}
	}
}

bool GuzaiGene::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/egg.tkm", nullptr, enModelUpAxisY, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_dishGene = FindGO<DishGene>("dishGene");

	return true;
}

void GuzaiGene::Update()
{
	//まだ皿を生成中であればUpdate関数をスルー
	if (m_dishGene->GetDishGeneState() == true) {
		return;
	}

	//最初に36個出す
	//まだ最初の具材を並べるのおわってない！
	//if (isCompletedSetGuzai == false) {
	//	for (int i = 0;i < guzaiNum; i++)
	//	{
	//		m_guzai[i] = NewGO<Guzai>(0);
	//		//WayPoint１から順に具材を設置
	//		m_guzai[i]->SetPosition(m_dishGene->GetDishPositionBasedNumber(i));
	//		//自分（具材）はいま、何番目の皿の上にいるか自覚させる
	//		m_guzai[i]->SetDishNumber(i + 1);
	//	}

	//	//全ての皿の上に具材を置き終えた！
	//	isCompletedSetGuzai = true;
	//}

	////空の皿を計測し、n個以上になったら、一気に補充
	//if (emptyDishCounter >= LostNumber) {
	//	for (int i = 0; i < emptyDishCounter; i++) {
	//		m_guzai[emptyDishNumber[i]] = NewGO<Guzai>(0);
	//		m_setPos = m_dishGene->GetDishPositionBasedNumber(emptyDishNumber[i]);
	//		m_setPos.y += 1000.0f;
	//		//m_guzai[emptyDishNumber[i]]->SetPosition(m_dishGene->GetDishPositionBasedNumber(emptyDishNumber[i]));
	//		m_guzai[emptyDishNumber[i]]->SetPosition(m_setPos);
	//		m_guzai[emptyDishNumber[i]]->SetDishNumber(emptyDishNumber[i]);
	//	}

	//	for (int i = 0;emptyDishCounter < i; i++)
	//	{
	//		emptyDishNumber[i] = 0;
	//	}
	//	emptyDishCounter = 0;
	//}
	
	m_skinModelRender->SetScale(m_scale);
}