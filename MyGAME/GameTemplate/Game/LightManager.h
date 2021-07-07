#pragma once

struct AllLight
{
	//ディレクションライト
	Vector3 directionalDirection = { 0.0f,1.0f,0.0f };				//ディレクションライトの方向
	float pad0;
	Vector3 directionalColor = { 10.0f, 10.0f, 10.0f };				//ディレクションライトの色
	float pad1;
	Vector3 eyePos = Vector3::Zero;									//目の位置（カメラの位置）
	float specPow = 0.5f;											//反射の度合い
	//環境光
	Vector3 ambientLight = { 0.6f, 0.6f, 0.6f };					//環境光
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
		s_allLight.eyePos = g_camera3D->GetPosition();
	}
};

