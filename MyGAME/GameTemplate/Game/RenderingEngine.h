#pragma once
#include "PostEffect.h"
#include "DefferedLighting.h"
#include "Shadow.h"
#include "MotionBlur.h"

class RenderingEngine
{
private:
	static RenderingEngine* instance;
public:
	/**
	 * @brief インスタンスを作成する。
	 * @return このインスタンス。
	*/
	static void CreateInstance()
	{
		instance = new RenderingEngine;
	}

	/**
	 * @brief インスタンスを取得する。
	*/
	static RenderingEngine* GetInstance()
	{
		return instance;
	}

	/**
	 * @brief インスタンスを削除する。
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief レンダリングに必要な準備を行う
	*/
	void PrepareRendering();

	/**
	 * @brief レンダリングに必要なすべてのオフスクリーンターゲットを初期化する。
	*/
	void InitRenderTargets();

	/**
	 * @brief レンダリングに必要なすべての画像を初期化する。
	*/
	void InitSprites();

	/**
	 * @brief ライトカメラの初期化を行う
	*/
	void InitLightCamera();

	/**
	 * @brief メインレンダーターゲットに画像の合成を行う。
	*/
	void DrawInMainRenderTarget(RenderContext& rc);

	/**
	 * @brief シャドウ作成に必要なライトカメラを取得する。
	*/
	Camera& GetLightCamera()
	{
		return m_lightCamera;
	}

	/**
	 * @brief レンダリングを行う
	*/
	void Render(RenderContext& rc);

	Matrix& GetPrevViewProjMatrix()
	{
		return m_prevViewProjMatrix;
	}

private:
	RenderTarget m_mainRenderTarget;
	SpriteInitData m_mainSpriteData;
	Sprite m_mainSprite;

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	RenderTarget m_shadowTarget;
	//ライト座標から見た影を作るためのもの
	Camera m_lightCamera;

	RenderTarget m_albedoTarget;
	RenderTarget m_normalTarget;
	RenderTarget m_specAndDepthTarget;
	RenderTarget m_speedTarget;


	Shadow m_shadow;
	PostEffect m_postEffect;
	DefferedLighting m_defferedLighting;
	MotionBlur m_motionBlur;

	Matrix m_prevViewProjMatrix = g_camera3D->GetViewProjectionMatrix();
};

