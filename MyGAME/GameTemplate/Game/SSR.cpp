#include "stdafx.h"
#include "SSR.h"

void SSR::Init(
	RenderTarget& mainRenderTarget,
	RenderTarget& depthRenderTarget,
	RenderTarget& normalRenderTarget,
	RenderTarget& metallicSmoothRenderTarget,
	RenderTarget& albedoRenderTarget )
{
	SpriteInitData ssrSpriteData;

	ssrSpriteData.m_width = mainRenderTarget.GetWidth();
	ssrSpriteData.m_height = mainRenderTarget.GetHeight();

	ssrSpriteData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	ssrSpriteData.m_textures[1] = &depthRenderTarget.GetRenderTargetTexture();
	ssrSpriteData.m_textures[2] = &normalRenderTarget.GetRenderTargetTexture();
	ssrSpriteData.m_textures[3] = &metallicSmoothRenderTarget.GetRenderTargetTexture();

	ssrSpriteData.m_fxFilePath = "Assets/shader/ssr.fx";
	ssrSpriteData.m_vsEntryPointFunc = "VSMain";
	ssrSpriteData.m_psEntryPoinFunc = "PSMain";

	ssrSpriteData.m_expandConstantBuffer = &m_ssrData;
	ssrSpriteData.m_expandConstantBufferSize = sizeof(Ssr_Cb/*m_ssrData*/);

	ssrSpriteData.m_alphaBlendMode = AlphaBlendMode_None;
	ssrSpriteData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
	ssrSpriteData.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

	m_reflectionSprite.Init(ssrSpriteData);

	float clearColor[] = {
			0.0f, 0.0f, 0.0f, 0.0f
	};

	m_reflectionTarget.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	m_blur.Init(&m_reflectionTarget.GetRenderTargetTexture());
	{
		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData initData;
		initData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		initData.m_textures[1] = &m_blur.GetBokeTexture();
		initData.m_textures[2] = &albedoRenderTarget.GetRenderTargetTexture();

		// �𑜓x��mainRenderTarget�̕��ƍ���
		initData.m_width = mainRenderTarget.GetWidth();
		initData.m_height = mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
		initData.m_fxFilePath = "Assets/shader/ssr.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_psEntryPoinFunc = "PSFinal";

		initData.m_alphaBlendMode = AlphaBlendMode_None;
		initData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

		m_finalSprite.Init(initData);

		m_finalTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),
			DXGI_FORMAT_D32_FLOAT
		);
	}
}

void SSR::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	// �܂��͉f�荞�݃C���[�W���쐬����B
	// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(m_reflectionTarget);
	// �����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(m_reflectionTarget);
	rc.ClearRenderTargetView(m_reflectionTarget);

	m_ssrData.cameraPosInWorld = g_camera3D->GetPosition();
	m_ssrData.s_viewProjection = g_camera3D->GetViewProjectionMatrix();
	m_ssrData.s_viewProjectionInv.Inverse(m_ssrData.s_viewProjection);


	// �`��
	m_reflectionSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_reflectionTarget);

	// �f�荞�݉摜���K�E�V�A���u���[�łڂ����ăf�m�C�Y���s���B
	m_blur.ExecuteOnGPU(rc, 2.0f);

	// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(TargetToApply);
	// �����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(TargetToApply);
	// �f�荞�݉摜�ƃ��C���V�[�����������Ă����B
	m_finalSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(TargetToApply);
}