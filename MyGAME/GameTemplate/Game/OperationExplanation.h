#pragma once
#include"SpriteRender.h"

class Fade;

/// <summary>
/// スプライトのデータ
/// </summary>
struct SpriteData
{
	const char* fliePath = nullptr;		//ファイルパス
};

class OperationExplanation : public IGameObject
{
private:
	SpriteRender* m_spriteRender;
	//ステート
	enum EnState {
		enState_Idle,
		enState_Change,			//切り替え
		enState_End				//終わり
	};
	EnState m_currentState;		//ステート

	SpriteData m_spriteData[6];	//スプライトのデータ
	int m_currentPage = 0;		//今のページ

	//試し用
	int a = 1200;
	int b = 700;

	Fade* m_fade = nullptr;		//フェード
public:
	OperationExplanation();
	~OperationExplanation();
	//更新処理。
	void Update();
};