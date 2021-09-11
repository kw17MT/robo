#pragma once

struct DirectionalLight
{

	//ディレクションライト
	Vector3 directionalDirection = { 0.0f,-1.0f,0.0f };				//ディレクションライトの方向
	float pad0 = 0;
	Vector3 directionalColor = { 10.0f, 10.0f, 10.0f };				//ディレクションライトの色
	//Vector3 directionalColor = { 1.0f, 1.0f, 1.0f };
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
	Matrix s_lightCameraMatrix = GameObjectManager::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
	Matrix ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
	//ディレクションライト
	DirectionalLight directionalLight;

	//環境光
	Vector3 ambientLight = { 0.4f, 0.4f, 0.4f };
	float pad2 = 0;

	//スポットライト
	SpotLight spotLight[2];
};

struct LightForGauge
{
	Vector3 ambient = { 0.0f,1.0f, 0.0f };
};

class LightManager : public IGameObject
{
private:
	static LightManager* instance;
	AllLight s_allLight;
	LightForGauge s_gaugeLight[3];
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

	/**
	 * @brief 全てのライトの情報を取得する
	 * @return 全てのライトの情報
	*/
	const AllLight& GetLightData() const
	{
		return s_allLight;
	}

	/**
	 * @brief ゲージ専用のライトを取得する
	 * @return 
	*/
	const LightForGauge& GetGaugeLight(int lightNumber) const
	{
		return s_gaugeLight[lightNumber];
	}

	/**
	 * @brief 引数で指定したゲージの色を黄色にする
	 * @param lightNumber 　ゲージの番号　０左、１右、２料理用
	*/
	void MakeGaugeLightYellow(int lightNumber);

	/**
	 * @brief 引数で指定したゲージの色を赤色にする
	 * @param lightNumber 　ゲージの番号　０左、１右、２料理用
	*/
	void MakeGaugeLightRed(int lightNumber);

	/**
	 * @brief 引数で指定したゲージの色を緑色にする
	 * @param lightNumber 　ゲージの番号　０左、１右、２料理用
	*/
	void MakeGaugeLightGreen(int lightNumber);

	/**
	 * @brief 毎フレームライトの位置を更新する
	*/
	void UpdateEyePos()
	{
		s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();

		s_allLight.ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix.Inverse();
	}

	/**
	 * @brief 引数で指定した番号のゲージ用ライトを取得する
	 * @param gaugeNumber ゲージの番号　０左、１右、２料理用
	 * @return 引数で指定した番号のゲージ用ライト
	*/
	 LightForGauge& GetGaugeLight(int gaugeNumber) 
	{
		return s_gaugeLight[gaugeNumber];
	}
};

