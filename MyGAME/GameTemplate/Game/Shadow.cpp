#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//�����_�[�^�[�Q�b�g������������B�𑜓x��2048x2048
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
	//�V���h�E�L���X�^�[�̂ݕ`���B
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

	//�V���h�E�̍쐬���s�����f���̃h���[
	//���C�g�J������RendeingEngine�Œ�`���ALightManager�œ������ASkinModelRender���Ŏg�p���Ă���B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadow);

	//�S�ẴI�u�W�F�N�g��`�����[�h�ɖ߂�
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
		vertexPos[0] = nearCenterPos + cameraUp * nearY - cameraRight * nearX; //�J�������猩�ċߕ��ʍ���
		vertexPos[1] = nearCenterPos - cameraUp * nearY - cameraRight * nearX; //�J�������猩�ċߕ��ʍ���
		vertexPos[2] = nearCenterPos + cameraUp * nearY + cameraRight * nearX; //�J�������猩�ċߕ��ʉE��
		vertexPos[3] = nearCenterPos - cameraUp * nearY + cameraRight * nearX; //�J�������猩�ċߕ��ʉE��

		vertexPos[4] = nearCenterPos + cameraUp * farY - cameraRight * farX; //�J�������猩�ĉ����ʍ���
		vertexPos[5] = nearCenterPos - cameraUp * farY - cameraRight * farX; //�J�������猩�ĉ����ʍ���
		vertexPos[6] = nearCenterPos + cameraUp * farY + cameraRight * farX; //�J�������猩�ĉ����ʉE��
		vertexPos[7] = nearCenterPos - cameraUp * farY + cameraRight * farX; //�J�������猩�ĉ����ʉE��


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
		//�V���h�E�L���X�^�[�̂ݕ`���B
		RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

		//�V���h�E�̍쐬���s�����f���̃h���[
		//���C�g�J������RendeingEngine�Œ�`���ALightManager�œ������ASkinModelRender���Ŏg�p���Ă���B
		GameObjectManager::GetInstance()->CallRenderWrapper(rc);
		rc.WaitUntilFinishDrawingToRenderTarget(m_cascadeShadowMap[i]);

		//�S�ẴI�u�W�F�N�g��`�����[�h�ɖ߂�
		RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

		//���Ɏg���ߕ��ʂ܂ł̋������X�V
		m_nearArea = m_shadowArea[i];
	}
}