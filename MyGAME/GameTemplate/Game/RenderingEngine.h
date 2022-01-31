#pragma once
#include "PostEffect.h"
#include "DefferedLighting.h"
#include "Shadow.h"

struct EnMatrixes {
	Matrix prevVPMatrix = g_camera3D->GetViewProjectionMatrix();				//1フレーム前のビュープロジェクション行列
	Matrix currentVPMatrix = g_camera3D->GetViewProjectionMatrix();				//現在のビュープロジェクション行列
};

class RenderingEngine
{
private:

	//シングルトンインスタンス
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

	/**
	 * @brief 現在と1フレーム前のプロジェクションビュー行列を取得
	 * @return 現在と1フレーム前のプロジェクションビュー行列
	*/
	EnMatrixes& GetPrevViewProjMatrix()
	{
		return m_mat;
	}

	/**
	 * @brief レンダリングする対象群
	*/
	enum EnRenderTypes
	{
		normal = 0,				//通常描画
		shadow,					//シャドウモデルを描画
		lensGhost,				//レンズゴースト用モデルの描画
		ui						//UI描画
	};

	/**
	 * @brief 何を描画するモードか設定する
	 * @param type レンダリングする対象群
	*/
	void SetRenderTypes(EnRenderTypes type)
	{
		m_renderTypes = type;
	}

	/**
	 * @brief 何を描画するモードか取得する
	 * @return 描画するモード
	*/
	EnRenderTypes GetRenderTypes()
	{
		return m_renderTypes;
	}

	/**
	 * @brief フォワードレンダリングするモデル（地面）にシャドウマップを送る
	 * @return シャドウマップ
	*/
	RenderTarget& GetShadowMap() {
		return m_shadow.GetShadowMap();
	}

	/**
	 * @brief ディファードライティングした1枚の画像をメインのレンダーターゲットに書きこむ
	 * @param rc レンダーコンテキスト
	*/
	void DrawInMainRenderTarget(RenderContext& rc);

	/**
	 * @brief メインのレンダーターゲットにUIを書き込む
	 * @param rc レンダーコンテキスト
	*/
	void DrawUI(RenderContext& rc);

	/**
	 * @brief モノクロ化する度合いを設定する
	 * @param rate モノクロ率
	*/
	void SetMonochromeRate(float rate) { m_monochromeRate = rate; }

	/**
	 * @brief モノクロ率を取得する
	 * @return モノクロ率
	*/
	const float& GetMonochromeRate() const { return m_monochromeRate; }

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

	float m_monochromeRate = 0.0f;						//モノクロ率
};

