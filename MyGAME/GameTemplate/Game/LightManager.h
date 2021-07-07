#pragma once

//struct DirectionalLight
//{
//	//ディレクションライト
//	Vector3 directionalDirection = { 0.0f,1.0f,0.0f };				//ディレクションライトの方向
//	float pad0 = 0;
//	Vector3 directionalColor = { 10.0f, 10.0f, 10.0f };				//ディレクションライトの色
//	float pad1 = 0;
//	Vector3 eyePos = Vector3::Zero;									//目の位置（カメラの位置）
//	float specPow = 0.5f;											//反射の度合い
//};
//
//struct SpotLight
//{
//	//スポットライト　コンストラクタで初期化、正規化
//	Vector3 spotPosition = Vector3::Zero;
//	float pad0 = 0;
//	Vector3 spotColor = Vector3::One;								//ライトの色
//	float spotRange = 500.0f;
//	Vector3 spotDirection = Vector3::Zero;					//ライトの向き
//	float spotAngle = Math::DegToRad(30.0f);
//};

struct AllLight
{
	//DirectionalLight directionalLight;
	//ディレクションライト
	Vector3 directionalDirection = { 0.0f,1.0f,0.0f };				//ディレクションライトの方向
	float pad0 = 0;
	Vector3 directionalColor = { 10.0f, 10.0f, 10.0f };				//ディレクションライトの色
	float pad1 = 0;
	Vector3 eyePos = Vector3::Zero;									//目の位置（カメラの位置）
	float specPow = 0.5f;											//反射の度合い

	//環境光
	Vector3 ambientLight = { 0.6f, 0.6f, 0.6f };					//環境光
	float pad2 = 0;

	//スポットライト
	//SpotLight spotLight0;
	//SpotLight spotLight1;
	//スポットライト　コンストラクタで初期化、正規化
	Vector3 spotPosition = { 1000.0f, 300.0f, 0.0f };
	float pad3 = 0;
	Vector3 spotColor = { 1.0f,0.0f,0.0f };								//ライトの色
	float spotRange = 500.0f;
	Vector3 spotDirection = { 1.0f, -1.0f, 0.0 };					//ライトの向き
	float spotAngle = Math::DegToRad(30.0f);
};

class LightManager : public IGameObject
{
private:
	static LightManager* instance;
	AllLight s_allLight;
public:
	LightManager();
	/**
	 * @brief インスタンスを作る
	*/
	static LightManager CreateInstance()
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
	static LightManager DeleteInstance()
	{
		delete instance;
	}

	AllLight& GetLightData()
	{
		return s_allLight;
	}

	void UpdateEyePos()
	{
		//s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
		s_allLight.eyePos = g_camera3D->GetPosition();
	}
};

