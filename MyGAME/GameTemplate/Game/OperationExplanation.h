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

//現在未使用
//操作説明の画像表示などに使用する。
class OperationExplanation : public IGameObject
{
private:
	//ステート
	enum EnState {
		enState_Idle,
		enState_Change,			//切り替え
		enState_End				//終わり
	};
	EnState m_currentState;		//ステート

	int m_currentPage = 0;		//今のページ
	int m_nextWidth = 1200;		//次の横の長さ
	int m_nextHeight = 700;		//次の縦の長さ

public:
	/**
	 * @brief 画像の初期化
	*/
	OperationExplanation();

	~OperationExplanation();

	//更新処理。
	void Update();
private:
	Fade* m_fade = nullptr;		//フェード
	SpriteData m_spriteData[6] = {nullptr};	//スプライトのデータ
	SpriteRender* m_spriteRender = nullptr;
};