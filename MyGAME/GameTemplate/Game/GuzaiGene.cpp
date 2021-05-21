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

	//まだ最初の具材を並べるのおわってない！
	if (isCompletedSetGuzai == false) {
		for (int i = 0;i < guzaiNum; i++)
		{
			m_guzai[i] = NewGO<Guzai>(0);
			//WayPoint１から順に具材を設置
			m_guzai[i]->SetPosition(m_dishGene->GetDishPositionBasedNumber(i));
			//自分（具材）はいま、何番目の皿の上にいるか自覚させる
			m_guzai[i]->SetDishNumber(i + 1);
		}

		//全ての皿の上に具材を置き終えた！
		isCompletedSetGuzai = true;
	}

	//空の皿を計測し、n個以上になったら、一気に補充
	if (emptyDishCounter >= LostNumber) {
		for (int i = 0; i < emptyDishCounter; i++) {
			m_guzai[emptyDishNumber[i]] = NewGO<Guzai>(0);
			m_setPos = m_dishGene->GetDishPositionBasedNumber(emptyDishNumber[i]);
			m_setPos.y += 1000.0f;
			//m_guzai[emptyDishNumber[i]]->SetPosition(m_dishGene->GetDishPositionBasedNumber(emptyDishNumber[i]));
			m_guzai[emptyDishNumber[i]]->SetPosition(m_setPos);
			m_guzai[emptyDishNumber[i]]->SetDishNumber(emptyDishNumber[i]);
		}

		for (int i = 0;emptyDishCounter < i; i++)
		{
			emptyDishNumber[i] = 0;
		}
		emptyDishCounter = 0;
	}
	
	////rand()(線形合同法)より良さげな別の方法
	//std::random_device rnd; //非決定的乱数生成器(pc内部の情報から乱数を生成) シード値を生成するのに使用
	//std::mt19937 mt(rnd()); //決定的乱数生成器 メルセンヌツイスタ32bit版 rndが出力する値をシード値とする
	//std::uniform_int_distribution<int> rand10(0, 9); //範囲指定乱数生成(現状0～9)
	//int randNum = rand10(mt);
	////2つある生成器で具材の出現パターンがほぼ同じになる問題あり

	//if (timer >= 50 && randNum != 1) {
	//	m_guzai = NewGO<Guzai>(0,"guzai");
	//	//m_guzai->SetGuzaiNo(GeneNo);
	//	m_guzai->SetPosition(m_charaCon.GetPosition());
	//	timer = 0;
	//}

	////５０フレーム経った後、ランダムな変数が１の時（10%）
	//if (timer == 50 && randNum == 1) {
	//	//バフアイテムが画面に出ていないとき。
	//	if (Buffnum == 0) {
	//		m_buff = NewGO<Buff>(0, "buff");
	//		m_buff->SetBuffNo(GeneNo);
	//		m_buff->SetPosition(m_charaCon.GetPosition());
	//		timer = 0;
	//		Buffnum = 1;
	//	}
	//	else if (DeBuffnum == 0)
	//	{
	//		DeBuff* deBuff = NewGO<DeBuff>(0, "debuff");
	//		deBuff->SetBuffNo(GeneNo);
	//		deBuff->SetPosition(m_charaCon.GetPosition());
	//		timer = 0;
	//		DeBuffnum = 1;
	//	}
	//	//バフアイテムが画面に出ているとき、バフアイテムを出さずに具材をだす。
	//	else {
	//		m_guzai = NewGO<Guzai>(0, "guzai");
	//		m_guzai->SetGuzaiNo(GeneNo);
	//		m_guzai->SetPosition(m_charaCon.GetPosition());
	//		timer = 0;
	//	}
	//}
	
	m_skinModelRender->SetScale(m_scale);
}