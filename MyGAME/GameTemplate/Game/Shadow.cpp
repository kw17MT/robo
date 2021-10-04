#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//レンダーターゲットを初期化する。解像度は2048x2048
	m_shadow.Create(2048, 2048, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);

	m_cascadeShadowMap[0].Create(2048, 2048, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);
	m_cascadeShadowMap[1].Create(1024, 1024, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);
	m_cascadeShadowMap[2].Create(512, 512, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);
}

void Shadow::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadow);
	rc.SetRenderTargetAndViewport(m_shadow);
	rc.ClearRenderTargetView(m_shadow);
	//シャドウキャスターのみ描く。
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

	//シャドウの作成を行うモデルのドロー
	//ライトカメラはRendeingEngineで定義し、LightManagerで統括し、SkinModelRender内で使用している。
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadow);

	//全てのオブジェクトを描くモードに戻す
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void Shadow::InitCascade(RenderTarget& mainTarget, RenderTarget& normalTexture, RenderTarget& specAndDepthTexture)
{
	m_cascadeShadowData.m_fxFilePath = "Assets/shader/shadow/CascadeShadow.fx";
	m_cascadeShadowData.m_textures[0] = &mainTarget.GetRenderTargetTexture();
	m_cascadeShadowData.m_textures[1] = &normalTexture.GetRenderTargetTexture();
	m_cascadeShadowData.m_textures[2] = &specAndDepthTexture.GetRenderTargetTexture();
	m_cascadeShadowData.m_textures[3] = &m_cascadeShadowMap[0].GetRenderTargetTexture();
	m_cascadeShadowData.m_textures[4] = &m_cascadeShadowMap[1].GetRenderTargetTexture();
	m_cascadeShadowData.m_textures[5] = &m_cascadeShadowMap[2].GetRenderTargetTexture();
	m_cascadeShadowData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_cascadeShadowData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_cascadeShadowData.m_width = 1280;
	m_cascadeShadowData.m_height = 720;
	m_cascadeShadowData.m_expandConstantBuffer = (void*)&mat;
	m_cascadeShadowData.m_expandConstantBufferSize = sizeof(Matrix) * 4;

	m_cascadeSprite.Init(m_cascadeShadowData);
}

void Shadow::Draw(RenderContext& rc)
{
	m_cascadeSprite.Draw(rc);
}

void Shadow::RenderCascade(RenderContext& rc)
{
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	Vector3 cameraUp = Cross(cameraForward, cameraRight);

	mat.inv.Inverse();

	for (int i = 0; i < m_cascadeShadowNum; i++)
	{
		float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * m_nearArea;
		float nearX = nearY * g_camera3D->GetAspect();

		float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * m_shadowArea[i];
		float farX = farY * g_camera3D->GetAspect();
		
		Vector3 nearCenterPos = g_camera3D->GetPosition() + cameraForward * g_camera3D->GetNear();
		Vector3 farCenterPos = g_camera3D->GetPosition() + cameraForward * m_shadowArea[i];

		Vector3 vertexPos[8];
		vertexPos[0] = nearCenterPos + cameraUp * nearY - cameraRight * nearX; //カメラから見て近平面左上
		vertexPos[1] = nearCenterPos - cameraUp * nearY - cameraRight * nearX; //カメラから見て近平面左下
		vertexPos[2] = nearCenterPos + cameraUp * nearY + cameraRight * nearX; //カメラから見て近平面右上
		vertexPos[3] = nearCenterPos - cameraUp * nearY + cameraRight * nearX; //カメラから見て近平面右下

		vertexPos[4] = nearCenterPos + cameraUp * farY - cameraRight * farX; //カメラから見て遠平面左上
		vertexPos[5] = nearCenterPos - cameraUp * farY - cameraRight * farX; //カメラから見て遠平面左下
		vertexPos[6] = nearCenterPos + cameraUp * farY + cameraRight * farX; //カメラから見て遠平面右上
		vertexPos[7] = nearCenterPos - cameraUp * farY + cameraRight * farX; //カメラから見て遠平面右下


		Vector3 vMax = { -FLT_MAX,-FLT_MAX, -FLT_MAX };
		Vector3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };

		Matrix lvpMatrix = RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
		for (auto& v : vertexPos)
		{
			lvpMatrix.Apply(v);
			vMax.Max(v);
			vMin.Min(v);
		}


		float xScale = 2.0f / (vMax.x - vMin.x);
		float yScale = 2.0f / (vMax.y - vMin.y);
		float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
		float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;

		Matrix clopMatrix;
		clopMatrix.m[0][0] = xScale;
		clopMatrix.m[1][1] = yScale;
		clopMatrix.m[3][0] = xOffset;
		clopMatrix.m[3][1] = yOffset;

		mat.s_LVPCMatrix[i] = lvpMatrix * clopMatrix;

		rc.WaitUntilToPossibleSetRenderTarget(m_cascadeShadowMap[i]);
		rc.SetRenderTargetAndViewport(m_cascadeShadowMap[i]);
		rc.ClearRenderTargetView(m_cascadeShadowMap[i]);
		//シャドウキャスターのみ描く。
		RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

		//シャドウの作成を行うモデルのドロー
		//ライトカメラはRendeingEngineで定義し、LightManagerで統括し、SkinModelRender内で使用している。
		GameObjectManager::GetInstance()->CallRenderWrapper(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_cascadeShadowMap[i]);

		//全てのオブジェクトを描くモードに戻す
		RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

		//次に使う近平面までの距離を更新
		m_nearArea = m_shadowArea[i];
	}
}