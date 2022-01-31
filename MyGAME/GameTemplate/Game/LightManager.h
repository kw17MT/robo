#pragma once
#include "RenderingEngine.h"

const int g_spotLightNum = 8;

struct DirectionalLight
{

	//ディレクションライト
	Vector3 directionalDirection = { 0.0f,-1.0f,1.0f };				//ディレクションライトの方向
	float pad0 = 0;
	Vector3 directionalColor = { 20.0f, 20.0f, 20.0f };				//ディレクションライトの色
	float pad1 = 0;
	Vector3 eyePos = g_camera3D->GetPosition();						//目の位置（カメラの位置）
	float specPow = 0.5f;											//反射の度合い
};

struct SpotLight
{
	//スポットライト　コンストラクタで初期化、正規化
	Vector3 spotPosition = Vector3::Zero;
	float pad3 = 0;
	Vector3 spotColor = Vector3::One;								//ライトの色
	float spotRange = 1500.0f;
	Vector3 spotDirection = Vector3::Zero;					//ライトの向き
	float spotAngle = Math::DegToRad(90.0f);
};

struct AllLight
{
	Matrix s_lightCameraMatrix = RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
	Matrix ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
	//ディレクションライト
	DirectionalLight directionalLight;
	//環境光
	Vector3 ambientLight = { 0.4f, 0.4f, 0.4f };
	float pad = 0;
	//スポットライト
	SpotLight spotLight[g_spotLightNum];
};

class LightManager : public IGameObject
{
private:
	static LightManager* instance;			//シングルトンインスタンス
	AllLight s_allLight;					//全てのライトの情報
public:
	/**
	 * @brief コンストラクタ
	*/
	LightManager();

	/**
	 * @brief インスタンスを作る
	*/
	static void CreateInstance()
	{
		instance = new LightManager;
	}

	/**
	 * @brief インスタンスを取得する
	 * @return シングルトンインスタンス
	*/
	static LightManager& GetInstance()
	{
		static LightManager instance;
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
	 * @brief 全てのライトの情報を取得する
	 * @return 全てのライトの情報
	*/
	const AllLight& GetLightData() const
	{
		return s_allLight;
	}

	/**
	 * @brief 毎フレームライトの位置を更新する
	*/
	void UpdateEyePos()
	{
		s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
		s_allLight.s_lightCameraMatrix = RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix.Inverse();
	}

	/**
	 * @brief 全てのスポットライトを消す
	*/
	void TurnOffSpotLight()
	{
		for (int i = 0; i < g_spotLightNum; i++)
		{
			s_allLight.spotLight[i].spotColor = g_vec3Zero;
		}
	}

	/**
	 * @brief 全てのスポットライトを付ける
	*/
	void TurnOnSpotLight();

	/**
	 * @brief マシンガンにスポットライトを与える
	 * @param pos マシンガン
	 * @param gunToPlayerDir マシンガンからプレイヤーへの方向
	*/
	void GiveLightForMachinGun(Vector3 pos, Vector3 gunToPlayerDir)
	{
		s_allLight.spotLight[0].spotPosition = pos;
		s_allLight.spotLight[0].spotDirection = gunToPlayerDir;
		s_allLight.spotLight[0].spotDirection.Normalize();
		s_allLight.spotLight[0].spotColor = { 0.75f,0.35f,0.25f };
		s_allLight.spotLight[0].spotRange = 80.0f;
	}

	void GiveLightForMissile(Vector3 pos, Vector3 missileToPlayerDir)
	{
		s_allLight.spotLight[0].spotPosition = pos;
		s_allLight.spotLight[0].spotDirection = missileToPlayerDir;
		s_allLight.spotLight[0].spotDirection.Normalize();
		s_allLight.spotLight[0].spotColor = { 0.75f,0.35f,0.25f };
		s_allLight.spotLight[0].spotRange = 1500.0f;
	}
};