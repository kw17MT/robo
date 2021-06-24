#pragma once
class ShadowParam
{
private:
	static ShadowParam* instance;
	Camera lightCamera;
	RenderTarget shadowMap;
	float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };
public:
	static void CreateInstance()
	{
		instance = new ShadowParam;
	}
	static void DeleteInstance()
	{
		delete instance;
	}
	static ShadowParam& GetInstance()
	{
		static ShadowParam instance;
		return instance;
	}

	void SetLightCameraParam(Vector3 pos)
	{
		lightCamera.SetPosition(pos);
		lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
		lightCamera.SetViewAngle(Math::DegToRad(20.0f));
		lightCamera.SetUp({ 1, 0, 0 });
		lightCamera.Update();
	}

	void CreateShadowMap()
	{
		shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	Camera GetLightCamera()
	{
		return lightCamera;
	}

	RenderTarget GetShadowRenderTarget()
	{
		return shadowMap;
	}
};

