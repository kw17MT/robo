#include "stdafx.h"
#include "CLevel2D.h"
#include "SpriteRender.h"
#include "MenuTimer.h"
#include "MissCounter.h"

//objdata.ddsFilePathにすでに用意されていたため不要

//引数にはobjdata.name
//引数を　char→wchar_t　にする。
//const char* CLevel2D::MakePathFromName(const char* name) 
//{
//	
//	
//	size_t iReturnValue;
//	size_t MAX = 20;
//	wchar_t result[20];
//	wchar_t filepath[256];
//
//	//wcstombs_s( size_t変換された文字数、wchar_t*変換結果、const size_t変換結果のサイズ、const char*変換する文字、なぞ。
//	errno_t ret = mbstowcs_s(&iReturnValue, result, MAX, name, _TRUNCATE);
//
//	//型がwchar_tに直ったobjdata.nameを用いてddsのパスを作る。
//	swprintf_s(filepath, L"Assets/Image/%s.dds", result);
//
//	//作ったパスがwchar_tなので、char型に直す。
//	size_t origsize = wcslen(filepath) + 1;
//	//変換した文字列
//	size_t convertedChars = 0;
//	//マルチバイト格納用
//	char strCon[] = "";
//	//サイズ
//	size_t strConSize = (strlen(strCon) + 1) * 2;
//	//変換後のサイズ。
//	const size_t newsize = origsize * 2;
//	//連結後のサイズ
//	char* nstring = new char[newsize + strConSize];
//	//マルチバイトに変換する。入りきらないものは切りすて
//	wcstombs_s(&convertedChars, nstring, newsize, filepath, _TRUNCATE);
//	//文字列の連結
//	_mbscat_s((unsigned char*)nstring, newsize + strConSize, (unsigned char*)strCon);
//
//	//戻り値にconst char* が来ている。
//	return nstring;
//}

bool CLevel2D::Start()
{
	//レベルを読み込む。
	//一番左が配列の3番目の要素、右が1番目の要素
	
	m_level2D.Init("Assets/level2D/level2D_add.casl", [&](Level2DObjectData& objdata) { 
		if (objdata.EqualObjectName("burger_cheese")) {
			//SpriteInitData data;
			////DDSファイル(画像データ)のファイルパスを指定する。
			//data.m_ddsFilePath[0] = objdata.ddsFilePath;
			////Sprite表示用のシェーダーのファイルパスを指定する。
			//data.m_fxFilePath = "Assets/shader/sprite.fx";
			////スプライトの幅と高さを取得する。
			//data.m_width = objdata.width;
			//data.m_height = objdata.height;
			////Sprite初期化オブジェクトを使用して、Spriteを初期化する
			//m_sprite.Init(data);
			////座標を取得する。
			//m_position = objdata.position;
			////大きさを設定する。
			//m_scale = objdata.scale;

			/*sprite[0] = NewGO<SpriteRender>(2);
			sprite[0]->Init(objdata.ddsFilePath, objdata.width /2 , objdata.height /2);
			sprite[0]->SetScale(objdata.scale);*/
			//sprite[0]->SetPosition(objdata.position);
			m_spritePositions[0] = objdata.position;
			m_level2DObjectDatas[enCheeseBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_tomato")) {
			//中間に出る

			/*sprite[1] = NewGO<SpriteRender>(2);
			sprite[1]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[1]->SetScale(objdata.scale);*/
			//sprite[1]->SetPosition(objdata.position);
			m_spritePositions[1] = objdata.position;
			m_level2DObjectDatas[enTomatoBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_egg")) {
			//左側に出る｛1p）

			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enEggBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_basic")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enBasicBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_double")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enDoubleBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_vegetable")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enVegetableBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_bacon")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enBaconBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_blt")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enBLTBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_onion")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enOnionBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_high_cal")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enHighcalBurger] = objdata;

			return true;
		}
		if (objdata.EqualObjectName("burger_on_onion")) {
			/*sprite[2] = NewGO<SpriteRender>(2);
			sprite[2]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[2]->SetScale(objdata.scale);*/
			//sprite[2]->SetPosition(objdata.position);
			//m_spritePositions[2] = objdata.position;
			m_level2DObjectDatas[enOnOnionBurger] = objdata;

			return true;
		}
		else{
			//return falseにすると、
			//Level2DクラスのSpriteで画像が読み込まれます。
			return true;
		}
	});

	sprite[0] = nullptr;
	sprite[1] = nullptr;
	sprite[2] = nullptr;
	sprite[3] = nullptr;
	sprite[4] = nullptr;
	sprite[5] = nullptr;
	sprite[6] = nullptr;
	sprite[7] = nullptr;
	sprite[8] = nullptr;
	sprite[9] = nullptr;
	sprite[10] = nullptr;

	m_showHamBurgers[0] = enCheeseBurger;
	m_showHamBurgers[1] = enTomatoBurger;
	m_showHamBurgers[2] = enEggBurger;
	m_showHamBurgers[3] = enBasicBurger;
	m_showHamBurgers[4] = enDoubleBurger;
	m_showHamBurgers[5] = enVegetableBurger;
	m_showHamBurgers[6] = enBaconBurger;
	m_showHamBurgers[7] = enBLTBurger;
	m_showHamBurgers[8] = enOnionBurger;
	m_showHamBurgers[9] = enHighcalBurger;
	m_showHamBurgers[10] = enOnOnionBurger;

	ShowHamBurger(0, m_showHamBurgers[0]);
	ShowHamBurger(1, m_showHamBurgers[1]);
	ShowHamBurger(2, m_showHamBurgers[2]);

	//左側ゲージ
	m_menuTimer[0] = NewGO<MenuTimer>(0);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationDegY(180.0f);
	m_menuTimer[0]->SetRotation(rot);
	m_menuTimer[0]->SetPosition({ 480.0f,5.0f,770.0f });
	
	//右側ゲージ
	m_menuTimer[1] = NewGO<MenuTimer>(0);
	m_menuTimer[1]->SetPosition({ -480.0f,5.0f,750.0f });


	m_missCounter = NewGO<MissCounter>(0);

	return true;
}

void CLevel2D::Update()
{
	//m_sprite.Update(m_position, Quaternion::Identity, m_scale);
	//レベル2DクラスのSpriteの更新処理。

	//プレイヤー1の時間切れ
	if (m_menuTimer[0]->GetTimeUpState()) {
		//左のメニューの再抽選
		Roulette(2);
		//1Pのミス数を1足す
		m_missCounter->AddPl1MissCount();
		//バツ印の画像を出す
		m_missCounter->ChangeMarkState(true);
		//バツを付けたのでFALSEにもどしてやる
		m_menuTimer[0]->SetTimeUpState(false);
	}
	//プレイヤー2の時間切れ
	if (m_menuTimer[1]->GetTimeUpState()) {
		Roulette(0);
		m_missCounter->AddPl2MissCount();
		m_missCounter->ChangeMarkState(true);
		m_menuTimer[1]->SetTimeUpState(false);
	}
	

	m_level2D.Update();
}

void CLevel2D::Render(RenderContext& rc)
{
	//m_sprite.Draw(rc);
	//レベル2DクラスのSpriteの描画処理。
	m_level2D.Draw(rc);
}

bool CLevel2D::GetIsMatchHamBurger(int* numbers, int size, int counterNo)
{
	if (counterNo == 1) {
		//メニューの左と中央を対象に比較開始
		for (int i = 2; i >= counterNo; i--)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			//長さ違ったら。
			if (size != hamBurger.size()) 
				//以下の処理しなーい。
			continue;
			
			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				int num = numbers[j];
				//具材が違ってたら。
				if (num != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				//次に表示するハンバーガー決めるお！
				Roulette(i);
				m_menuTimer[counterNo - 1]->ResetTimerParam();
				return true;
			}
			
		}
	}

	if (counterNo == 2) {
		//メニューの右と中央を対象に比較開始
		for (int i = 0; i < counterNo; i++)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			//長さ違ったら。
			if (size != hamBurger.size())
				//以下の処理しなーい。
				continue;

			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				int num = numbers[j];
				//具材が違ってたら。
				if (num != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				//次に表示するハンバーガー決めるお！
				Roulette(i);
				m_menuTimer[counterNo - 1]->ResetTimerParam();

				return true;
			}
		}
	}

	//同じじゃなかったら、false以外ありえない。
	return false;
}

void CLevel2D::Roulette(int number)
{
	//TODO ここの乱数要修正？
	int rn = rand() % enHamBurgerNum;

	m_showHamBurgers[number] = EnHamBurger(rn);
	//ハンバーガーの画像を表示しまーす。
	ShowHamBurger(number, m_showHamBurgers[number]);
}

void CLevel2D::ShowHamBurger(int number, EnHamBurger enHamBurger)
{
	if (sprite[number] != nullptr)
	{
		DeleteGO(sprite[number]);
	}

	Level2DObjectData& objData = m_level2DObjectDatas[enHamBurger];
	sprite[number] = NewGO<SpriteRender>(2);
	sprite[number]->Init(objData.ddsFilePath, objData.width, objData.height);
	sprite[number]->SetScale({ 0.5f,0.5f,0.5f }/*objData.scale*/);
	//sprite[number]->SetScale(objData.scale);

	sprite[number]->SetPosition(m_spritePositions[number]);
}