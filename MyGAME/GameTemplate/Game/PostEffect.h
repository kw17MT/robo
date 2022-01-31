#pragma once
#include "Bloom.h"
#include "FXAA.h"
#include "DepthInView.h"
#include "SSR.h"
#include "MotionBlur.h"
#include "LensGhost.h"
#include "Monochrome.h"

class PostEffect
{
public:
	/**
	 * @brief ポストエフェクトに使用するレンダリングターゲットを各ポストエフェクトインスタンスに送る
	 * @param defferedTarget ディファードライティングしたメインゲーム画面
	 * @param albedoMap	アルベドマップ
	 * @param normalMap 法線マップ
	 * @param specDepthMap 鏡面反射と深度値マップ
	 * @param velocityMap ヴェロシティマップ
	*/
	void Init(RenderTarget& defferedTarget, 
		RenderTarget& albedoMap,
		RenderTarget& normalMap,
		RenderTarget& specDepthMap,
		RenderTarget& velocityMap
		 );

	/**
	 * @brief 各ポストエフェクトを描画するかどうか決める
	 * @param rc レンダーコンテキスト
	 * @param mainSprite メインゲーム画面の画像
	 * @param mainTarget メインゲーム画面のレンダーターゲット
	*/
	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	DepthInView m_depthInView;			//被写界深度インスタンス
	Bloom m_bloom;						//ブルームインスタンス
	FXAA m_fxaa;						//FXAAインスタンス
	MotionBlur m_motionBlur;			//モーションブラーインスタンス
	LensGhost m_lensGhost;				//レンズゴーストインスタンス
	Monochrome m_monochrome;			//モノクロ化インスタンス
};