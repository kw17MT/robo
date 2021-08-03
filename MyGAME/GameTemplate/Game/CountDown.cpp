#include "stdafx.h"
#include "CountDown.h"
#include "GameDirector.h"
#include "SoundSource.h"
#include "SpriteRender.h"

namespace
{
	const Vector3 DEFAULT_SCALE = { 2.0f,2.0f,1.0f };
	const Vector4 COUNTDOWN_START_COLOR = { 1.0f,1.0f,1.0f,0.0f };
	const int AJUST_TIMER_SPEED = 2;
	const int COUNTDOWN_SPRITE_WIDTH = 512;
	const int COUNTDOWN_SPRITE_HEIGHT = 512;
	const int COUNTDOWN_PHASE_ONE = 1;
	const int COUNTDOWN_PHASE_TWO = 2;
	const int COUNTDOWN_PHASE_THREE = 3;
	const int COUNTDOWN_PHASE_END = 4;
	const float COUNTDOWN_PHASE1_START = 4.0f;
	const float COUNTDOWN_PHASE1_END = 3.0f;
	const float COUNTDOWN_PHASE2_START = 3.0f;
	const float COUNTDOWN_PHASE2_END = 2.0f;
	const float COUNTDOWN_PHASE3_START = 2.0f;
	const float COUNTDOWN_PHASE3_END = 1.0f;
	const float COUNTDOWN_PHASE_FINAL_START = 1.0f;
	const float COUNTDOWN_PHASE_FINAL_END = 0.0f;
	const float COUNTDOWN_SHRINK_SPEED = 2.0f / 60.0f;
	const float COUNTDOWN_START_SCALE = 2.0f;
	const float SE_BUZZER_VOLUME = 0.4f;
	const float SE_VOLUME = 1.0f;
	const float COLOR_ALPHA_RUN_OUT = 0.0f;
}

bool CountDown::Start()
{
	m_spriteRender = NewGO<SpriteRender>(10, "count");
	//色を設定
	m_spriteRender->SetColor(m_color);
	//中心を設定
	m_spriteRender->SetPivot(m_pivot);
	//位置を設定
	m_spriteRender->SetPosition(m_position);

	//カウントダウンを開始するということを設定する。
	GetGameDirector().SetGameScene(enGameCountDown);

	return true;
}

void CountDown::Update()
{
	//1フレームにかかる時間を取得（秒）
	float gameTime = GameTime().GetFrameDeltaTime();

	m_timer -= gameTime / AJUST_TIMER_SPEED;

	//スプライトの不透明度とカウントダウンのすべてのフェーズが終わったらゲームスタート
	if (m_timer <= COUNTDOWN_PHASE_FINAL_END && m_alpha <= COLOR_ALPHA_RUN_OUT) {
		//ゲームが開始したことを設定する。
		GetGameDirector().SetGameScene(enGamePlay);
		//削除
		DeleteGO(this);
	}

	//スプライト変更処理
	if (m_spriteRender != nullptr) {
		//３を出す段階。
		if (m_timer <= COUNTDOWN_PHASE1_START && m_timer > COUNTDOWN_PHASE1_END && m_changeCount == COUNTDOWN_PHASE_ONE) {
			//ファイルパスを３で初期化
			m_spriteRender->Init("Assets/Image/3.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			//透明にする
			m_alpha = 0.0f;
			//最初の拡大率に戻す
			m_scale = DEFAULT_SCALE;
			//画像を変えた回数をインクリメント
			m_changeCount++;
			//音を鳴らす準備
			m_soundFlag = true;
		}
		//２を出す段階。
		else if (m_timer <= COUNTDOWN_PHASE2_START && m_timer > COUNTDOWN_PHASE2_END && m_changeCount == COUNTDOWN_PHASE_TWO) {
			m_spriteRender->Init("Assets/Image/2.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			m_alpha = 0.0f;
			m_scale = DEFAULT_SCALE;
			m_isFadeIn = true;
			m_changeCount++;
			m_soundFlag = true;
		}
		//１を出す段階。
		else if (m_timer <= COUNTDOWN_PHASE3_START && m_timer > COUNTDOWN_PHASE3_END && m_changeCount == COUNTDOWN_PHASE_THREE) {
			m_spriteRender->Init("Assets/Image/1.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			m_alpha = 0.0f;
			m_scale = DEFAULT_SCALE;
			m_isFadeIn = true;
			m_changeCount++;
			m_soundFlag = true;
		}
		//STARTを出す段階
		else if (m_timer <= COUNTDOWN_PHASE_FINAL_START && m_timer > COUNTDOWN_PHASE_FINAL_END && m_changeCount == COUNTDOWN_PHASE_END) {
			m_spriteRender->Init("Assets/Image/start.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			m_alpha = 0.0f;
			m_scale = DEFAULT_SCALE;
			m_isFadeIn = true;
			m_changeCount++;
			m_soundFlag = true;		
		}
	}

	//フェードイン
	if (m_isFadeIn == true) {
		m_alpha += gameTime;
	}
	//フェードアウト
	else {
		m_alpha -= gameTime;
	}

	//フェードイン完了
	if (m_alpha > 1.0f) {
		m_alpha = 1.0f;
		m_isFadeIn = false;
	}
	else if (m_alpha < 0.0f) {
		m_alpha = 0.0f;
	}

	//スケール縮小処理
	m_scale.x -= COUNTDOWN_SHRINK_SPEED;
	m_scale.y -= COUNTDOWN_SHRINK_SPEED;
	//通常の大きさ未満にならないようにする。
	if (m_scale.x < 1.0f || m_scale.y < 1.0f) {
		m_scale.x = 1.0f;
		m_scale.y = 1.0f;
	}

	//開始時のカウントダウンに応じて音を鳴らす。
	if (m_timer < COUNTDOWN_PHASE_FINAL_START && m_soundFlag == true && m_isFadeIn == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/basketball_buzzer1.wav", false);
		se->SetVolume(SE_BUZZER_VOLUME);
		se->Play(false);
		m_soundFlag = false;
	}
	else if (m_timer < COUNTDOWN_PHASE1_START && m_timer > COUNTDOWN_PHASE_FINAL_START && m_soundFlag == true && m_isFadeIn == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Time.wav", false);
		se->SetVolume(SE_VOLUME);
		se->Play(false);
		m_soundFlag = false;
	}

	m_color.w = m_alpha;
	m_spriteRender->SetColor(m_color);
	m_spriteRender->SetScale(m_scale);
}