#include "stdafx.h"
#include "CLevel2D.h"
#include "SpriteRender.h"


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
	
	m_level2D.Init("Assets/level2D/level2D.casl", [&](Level2DObjectData& objdata) { 
		if (objdata.EqualObjectName("burger_cheese_new")) {

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

			sprite[0] = NewGO<SpriteRender>(2);
			sprite[0]->Init(objdata.ddsFilePath, objdata.width /2 , objdata.height /2);
			sprite[0]->SetScale(objdata.scale);
			sprite[0]->SetPosition(objdata.position);
			
			return true;
		}
		if (objdata.EqualObjectName("burger_tomato")) {
			sprite[0] = NewGO<SpriteRender>(2);
			sprite[0]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[0]->SetScale(objdata.scale);
			sprite[0]->SetPosition(objdata.position);

			return true;
		}
		if (objdata.EqualObjectName("burger_egg")) {
			sprite[0] = NewGO<SpriteRender>(2);
			sprite[0]->Init(objdata.ddsFilePath, objdata.width / 2, objdata.height / 2);
			sprite[0]->SetScale(objdata.scale);
			sprite[0]->SetPosition(objdata.position);

			return true;
		}
		else{
			//return falseにすると、
			//Level2DクラスのSpriteで画像が読み込まれます。
			return false;
		}
	});
	return true;
}

void CLevel2D::Update()
{
	//m_sprite.Update(m_position, Quaternion::Identity, m_scale);
	//レベル2DクラスのSpriteの更新処理。
	m_level2D.Update();
}

void CLevel2D::Render(RenderContext& rc)
{
	//m_sprite.Draw(rc);
	//レベル2DクラスのSpriteの描画処理。
	m_level2D.Draw(rc);
}
