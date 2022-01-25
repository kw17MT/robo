#include "stdafx.h"
#include "AfterGameOverScene.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

namespace
{
	const Vector4 BLACK = { 0.0f,0.0f,0.0f,0.0f };
	const float SE_VOLUME = 0.5f;
	const float BACKGROUND_DARKNESS = 0.7f;
}

AfterGameOverScene::~AfterGameOverScene()
{
	//選択肢の画像を削除する
	for (int i = 0; i < enSpriteNum; i++)
	{
		DeleteGO(m_sprite[i]);
	}
	DeleteGO(m_blackSprite);
}

bool AfterGameOverScene::Start()
{
	//選択肢の画像分生成
	for (int i = 0; i < enSpriteNum; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(10);
	}
	//Replay画像で初期化
	m_sprite[enRePlayButton]->Init("Assets/image/GameOver/PlayAgain.dds", 400, 130, "PSWarningSign");
	//ToTitle画像で初期化
	m_sprite[enToTitleButton]->Init("Assets/image/GameOver/ReturnToTitle.dds", 400, 130, "PSWarningSign");
	//画像の位置を初期化
	m_sprite[enRePlayButton]->SetPosition({ 0.0f,100.0f,0.0f });
	m_sprite[enToTitleButton]->SetPosition({ 0.0f,-100.0f,0.0f });
	


	return true;
}

void AfterGameOverScene::SelectButton()
{
	//次のシーンに遷移しないときだけ、ボタン操作できるようにする
	if (m_canSelectButton)
	{
		//上ボタンが押されていて
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//現在選んでいる画像の種類に応じて反応する
			switch (m_selectedSpriteType)
			{
			case enRePlayButton:
				//選択しているボタンをタイトルへ戻るボタンにする
				m_selectedSpriteType = enToTitleButton;
				break;
			case enToTitleButton:
				//選択しているボタンをリプレイボタンにする
				m_selectedSpriteType = enRePlayButton;
				break;
			}
			//セレクトする音を出す
			CSoundSource* selectSE = NewGO<CSoundSource>(0);
			selectSE->Init(L"Assets/sound/select.wav", false);
			selectSE->SetVolume(SE_VOLUME);
			selectSE->Play(false);
		}
		//下ボタンが押されていて
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			switch (m_selectedSpriteType)
			{
			case enRePlayButton:
				//選択しているボタンをタイトルへ戻るボタンにする
				m_selectedSpriteType = enToTitleButton;
				break;
			case enToTitleButton:
				//選択しているボタンをリプレイボタンにする
				m_selectedSpriteType = enRePlayButton;
				break;
			}
			//セレクトする音を出す
			CSoundSource* selectSE = NewGO<CSoundSource>(0);
			selectSE->Init(L"Assets/sound/select.wav", false);
			selectSE->SetVolume(SE_VOLUME);
			selectSE->Play(false);
		}
	}
}

void AfterGameOverScene::Update()
{
	if (m_blackSprite == nullptr)
	{
		m_blackSprite = NewGO<SpriteRender>(0);
		m_blackSprite->Init("Assets/image/fade/black.dds", 1280, 720);
		m_blackSprite->SetColor(BLACK);
	}
	else
	{
		m_alpha += GameTime().GetFrameDeltaTime();
		if (m_alpha >= BACKGROUND_DARKNESS)
		{
			m_alpha = BACKGROUND_DARKNESS;
		}
		Vector4 color = BLACK;
		color.w = m_alpha;
		m_blackSprite->SetColor(color);
	}

	//上下ボタンで画面上ボタンを選択する
	SelectButton();

	//現在選択しているボタン画像を黄色くする
	for (int i = 0; i < enSpriteNum; i++)
	{
		//選択している番号と一致していたら
		if (i == m_selectedSpriteType)
		{
			//黄色にする
			m_sprite[i]->SetSpriteAlpha(1.0f);
		}
		else
		{
			//通常の色にする
			m_sprite[i]->SetSpriteAlpha(0.0f);
		}
	}

	//Aボタンを押したら
	if (g_pad[0]->IsTrigger(enButtonA)
		&& m_fade == nullptr)
	{
		//フェードインする
		m_fade = NewGO<Fade>(0);
		//ボタンを動かせないようにする
		m_canSelectButton = false;
	}

	//フェードインが終わって待機状態ならば
	if (m_fade != nullptr && m_fade->GetFadePhase() == enWait)
	{
		//さっきまでしていたゲームを検索して
		Game* pastGame = FindGO<Game>("game");
		//破棄する
		DeleteGO(pastGame);
		//フェードアウトするように伝える
		m_fade->SetFadeOut();
		//次のシーンに遷移する
		m_shouldStartNextScene = true;
		//この画像群を消す
		DeleteGO(this);
	}
	if (m_shouldStartNextScene)
	{
		//押されたボタンがリプレイボタンならば
		if (m_selectedSpriteType == enRePlayButton)
		{
			//新しいゲームを作成
			Game* newGame = NewGO<Game>(0, "game");
		}
		//タイトルへ戻るボタンならば
		else if (m_selectedSpriteType == enToTitleButton)
		{
			//タイトルを作成
			Title* newTitle = NewGO<Title>(0, "title");
		}
	}
}