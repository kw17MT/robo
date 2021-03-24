#pragma once
class SpriteRender;
#include "Level2D.h"

/// <summary>
/// レベル2D処理のサンプルクラス。
/// </summary>
class CLevel2D : public IGameObject
{
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	//不要
	//Level2Dから渡されたname(パスの一部）を用いてSpriteRenderで使うconst char*型を作る。
	//const char* MakePathFromName(const char* name);
private:
	Level2D m_level2D;		//レベル2D。
	//Sprite m_sprite;			//スプライト。
	Vector3 m_position;		//座標。
	Vector3 m_scale;		//大きさ。

	SpriteRender* sprite[3];
};