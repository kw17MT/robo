#include "stdafx.h"
#include "Counter.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Kitchen.h"
#include "Score.h"

Counter::Counter()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/box2.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	modeldata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	m_charaCon.Init(0.0f, 0.0f, g_vec3One);

	//ハンバーガーのデータを作る。
	HamBurger cheese;
	cheese.push_back(3);
	cheese.push_back(0);
	cheese.push_back(2);
	m_hamBurgers[enCheeseBurger] = cheese;

	HamBurger tomato;
	tomato.push_back(3);
	tomato.push_back(4);
	tomato.push_back(4);
	tomato.push_back(2);
	m_hamBurgers[enTomatoBurger] = tomato;

	HamBurger egg;
	egg.push_back(2);
	egg.push_back(4);
	egg.push_back(1);
	egg.push_back(3);
	m_hamBurgers[enEggBurger] = egg;
}

//////////////////////判別するところ////////////////////////////////////////////////////////////////////////////////
bool Counter::Judge()
{
	if (CounterNo == 1) {
		Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
		ModelRender* pl01 = FindGO<ModelRender>("player01");

		//最終結果を記録するもの。
		bool correct01 = true;
		//判別していく過程で正しかったらインクリメントされていく。正解数みたいなもの。
		int correctCount01 = 0;
		//満点となる数。sizeofの中身を変えることでいろんな種類のバーガーに対応できると思う。←変えてもいい。
		int correctGuzaiNum01 = sizeof(burger01) / sizeof(int);

		//作ったバーガーの層によって回すFOR文の回数が変わる。
		//StackNumはバーガーができる瞬間にこちら側に保存される。（in Kitchen.cpp)
		//判別過程で一度でも間違えたらFALSE
		for (int i = 0; i < enHamBurgerNum; i++)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = m_hamBurgers[i];
			//長さ違ったら。
			if (StackNum != hamBurger.size())
				//以下の処理しなーい。
				continue;
			
			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				//具材が違ってたら。
				if (pl01->GuzaiNo[j] != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				return true;
			}
		}
		//同じじゃなかったらtrue以外ありえない。
		return false;
		/*for (int i = 0; i < StackNum; i++) {
			
			if (burger01[i] == pl01->GuzaiNo[i]) {
				correctCount01++;
				continue;
			}
			else{
				correct01 = false;
				break;
			}
		}
		//ここまでの文では途中まで完璧にできていたらTRUEになる。
		//したの文で、最終的に層の数が違っていたらFALSEにする。
		if (correctCount01 != correctGuzaiNum01) {
			correct01 = false;
		}

		return correct01;
		*/
	}

	if (CounterNo == 2) {
		Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
		ModelRender* pl02 = FindGO<ModelRender>("player02");

		bool correct02 = true;
		int correctCount02 = 0;
		int correctGuzaiNum02 = sizeof(burger01) / sizeof(int);
		for (int i = 0; i < enHamBurgerNum; i++)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = m_hamBurgers[i];
			//長さ違ったら。
			if (StackNum != hamBurger.size())
				//以下の処理しなーい。
				continue;

			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				//具材が違ってたら。
				if (pl02->GuzaiNo[j] != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				return true;
			}
		}
		//同じじゃなった！
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////

//バーガーを最終的に消してスコアを発生させる。
//カウンターに近いところでBボタンを押すといったん載せて消す。
void Counter::Delete()
{
	if (CounterNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		Vector3 plPos = pl01->GetPosition();

		Vector3 CounterPos01 = m_charaCon.GetPosition();

		//カウンターからプレイヤーの距離
		float pl2Counter = (plPos.x - CounterPos01.x) * (plPos.x - CounterPos01.x) + (plPos.y - CounterPos01.y) * (plPos.y - CounterPos01.y) + (plPos.z - CounterPos01.z) * (plPos.z - CounterPos01.z);
		pl2Counter = sqrt(pl2Counter);

		//プレイヤーがバーガーをもっていたら
		if (pl01->have == 2) {
			Burger* bu01 = FindGO<Burger>("burger01");

			//キッチンに置く準備
			//できたハンバーガーの組成をJudge関数で調べ、あっていたらカウンターに置ける。
			if (g_pad[0]->IsPress(enButtonB) && pl2Counter < 100.0f) {
				if (Judge() == true) {
					bu01->putOnKitchen = 1;
				}
			}

			//置いたら30フレーム後に消去
			if (bu01->putOnKitchen == 1) {
				Delay++;
				CounterPos01.y += 100.0f;
				bu01->SetPosition(CounterPos01);

				if (Delay == 30) {
					//ここで積み上げてた具材の数をScoreに渡してあげる。
					Score* sco = FindGO<Score>("score");
					sco->SetBasePoint01(StackNum);

					//次の具材No.を保存するため、９で初期化。
					pl01->SetGuzaiNo9();
					
					DeleteGO(bu01);
					Delay = 0;
					pl01->have = 0;
					StackNum = 0;
				}
			}
		}
	}

	if (CounterNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		Vector3 plPos = pl02->GetPosition();

		Vector3 CounterPos02 = m_charaCon.GetPosition();

		float pl2Counter = (plPos.x - CounterPos02.x) * (plPos.x - CounterPos02.x) + (plPos.y - CounterPos02.y) * (plPos.y - CounterPos02.y) + (plPos.z - CounterPos02.z) * (plPos.z - CounterPos02.z);
		pl2Counter = sqrt(pl2Counter);

		//プレイヤーがバーガーをもっていたら
		if (pl02->have == 2) {
			Burger* bu02 = FindGO<Burger>("burger02");

			//キッチンに置く準備
			//できたハンバーガーの組成をJudge関数でしらべ、あっていたらカウンターに置ける。
			if (g_pad[1]->IsPress(enButtonB) && pl2Counter < 100.0f) {
				if (Judge() == true) {
					bu02->putOnKitchen = 1;
				}
			}

			//置いたら30フレーム後に消去
			if (bu02->putOnKitchen == 1) {
				Delay++;
				CounterPos02.y += 100.0f;
				bu02->SetPosition(CounterPos02);

				if (Delay == 30) {
					//ここで積み上げてた具材の数をScoreに渡してあげる。
					Score* sco = FindGO<Score>("score");
					sco->SetBasePoint02(StackNum);

					//次の具材No.を保存するため、９で初期化。
					pl02->SetGuzaiNo9();

					DeleteGO(bu02);
					Delay = 0;
					pl02->have = 0;
					StackNum = 0;
				}

			}
		}
	}
}

void Counter::Update()
{
	Delete();
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}