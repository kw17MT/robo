#pragma once
#include "PostEffect.h"
#include "DefferedLighting.h"
#include "Shadow.h"

struct EnMatrixes {
	Matrix prevVPMatrix = g_camera3D->GetViewProjectionMatrix();
	Matrix currentVPMatrix = g_camera3D->GetViewProjectionMatrix();
};

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

	EnMatrixes& GetPrevViewProjMatrix()
	{
		return m_mat;
	}

	enum EnRenderTypes
	{
		normal = 0,
		shadow,
		lensGhost,
		ui
	};

	void SetRenderTypes(EnRenderTypes type)
	{
		m_renderTypes = type;
	}

	EnRenderTypes GetRenderTypes()
	{
		return m_renderTypes;
	}

	//モーションブラー作成に使用する
	void GetDefferedSprite(RenderContext& rc)
	{
		m_defferedLighting.Draw(rc);
	}

	RenderTarget& GetShadowMap() {
		return m_shadow.GetShadowMap();
	}

	void DrawInMainRenderTarget(RenderContext& rc);

	void DrawUI(RenderContext& rc);

	void SetMonochromeRate(float rate) { m_monochromeRate = rate; }

	float& GetMonochromeRate() { return m_monochromeRate; }

private:
	//最終表示するオフスクリーン画像
	RenderTarget m_mainRenderTarget;
	SpriteInitData m_mainSpriteData;
	Sprite m_mainSprite;

	//ディファードライティング済みの画像にポストエフェクトを掛けるためのもの
	RenderTarget m_captureDeffered;
	SpriteInitData m_effectedDefferedData;
	Sprite m_effectedDeffered;
	
	//シャドウ関連
	Shadow m_shadow;
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };		//画面消去用の色
	Camera m_lightCamera;								//ライトカメラ

	RenderTarget m_albedoTarget;						//アルベドマップ
	RenderTarget m_normalTarget;						//法線マップ
	RenderTarget m_specAndDepthTarget;					//スペキュラ反射と深度値
	RenderTarget m_velocityTarget;						//速度マップ

	DefferedLighting m_defferedLighting;				//ディファード画像
	PostEffect m_postEffect;							//ポストエフェクト統括
	
	EnMatrixes m_mat;									//VelocityMap作成のためのマトリックス
	
	EnRenderTypes m_renderTypes = normal;				//レンダリングモード

	RenderTarget m_forwardTarget;

	SpriteInitData m_forwardData;
	Sprite m_forward;
	RenderTarget m_sky;
	float m_monochromeRate = 0.0f;
};

