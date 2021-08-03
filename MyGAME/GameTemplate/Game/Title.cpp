#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "Game.h"

namespace
{
	const int SPRITE_WIDTH = 1280;
	const int SPRITE_HEIGHT = 720;

	const int PLAYER_ONE_PAD = 0;
}

Title::~Title()
{
	DeleteGO(m_spriteRender);
}

bool Title::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	//タイトル画像のファイルパス、縦幅、横幅を設定
	m_spriteRender->Init("Assets/Image/Title_512.dds", SPRITE_WIDTH, SPRITE_HEIGHT);

	return true;
}

void Title::Update()
{
	//フェードアウト実行ちゅうでなく
	if (m_fade == nullptr) {
		//タイトル中にAボタンが押されたら
		if (g_pad[PLAYER_ONE_PAD]->IsTrigger(enButtonA))
		{
			//フェード
			m_fade = NewGO<Fade>(0, "fade");
		}
		return;
	}
	//真っ暗になったら遷移
	if (m_fade->GetState() == enState_Wait) {
		//真っ暗になりきった段階でタイトルのスプライトを削除
		DeleteGO(this);
		//フェードインに移行
		m_fade->SetState(enState_In);
		//ゲームを生成
		NewGO<Game>(0, "game");
	}

	//フェード用画像の不等明度が0以下になったときにフェード画像とタイトル自身を削除
	if (m_fade->GetState() == enState_In && m_fade->GetAlpha() <= 0.0f) {
		DeleteGO(m_fade);
		DeleteGO(this);
	}

}