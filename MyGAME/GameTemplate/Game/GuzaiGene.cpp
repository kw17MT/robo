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
	if (isCompletedInitialAction == false) {
		for (int i = 0;i < guzaiNum; i++)
		{
			m_guzai[i] = NewGO<Guzai>(0);
			m_guzai[i]->SetPosition(m_dishGene->GetDishPositionBasedNumber(i));
		}



		isCompletedInitialAction = true;
	}


	//全ての皿の上に具材をNewGO
	//のためには37個のさらのポジションが必要になる。
	//と同時に自分（具材）はいま、何番目の皿の上にいるか自覚させる


	//初動がおわったフラグを皿が感知したら動き始める。
	//皿が動くごとに具材も動く
	//これはGuzaicppのほうで処理したのでいい


	//空の皿を計測し、n個以上になったら、一気に補充
	//流れにそって落ちてくるように

	//流れ
	//Game/DishGene/Dish/GuzaiGene/Guzai/Dish/GuzaiGene/Guzai...


	//もし、皿にある変数が偽だったら、その皿の上に具材をnewGO






















	//timer++;

	////rand()(線形合同法)より良さげな別の方法
	//std::random_device rnd; //非決定的乱数生成器(pc内部の情報から乱数を生成) シード値を生成するのに使用
	//std::mt19937 mt(rnd()); //決定的乱数生成器 メルセンヌツイスタ32bit版 rndが出力する値をシード値とする
	//std::uniform_int_distribution<int> rand10(0, 9); //範囲指定乱数生成(現状0〜9)
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
}\