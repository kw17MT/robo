#include "stdafx.h"
#include "Bloom.h"

void Bloom::Init(RenderTarget& defferedTarget)
{
	m_luminanceTarget.Create(
		defferedTarget.GetWidth(),   // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		defferedTarget.GetHeight(),  // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		1,
		1,
		defferedTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//�u���[���摜�̏������p
	SpriteInitData bloomSpriteData;	
	bloomSpriteData.m_fxFilePath = "Assets/shader/blur/bloom.fx";
	bloomSpriteData.m_vsEntryPointFunc = "VSMain";
	bloomSpriteData.m_psEntryPoinFunc = "PSLuminance";
	bloomSpriteData.m_width = defferedTarget.GetWidth();
	bloomSpriteData.m_height = defferedTarget.GetHeight();
	//�P�x�𒊏o��������ʂ��擾
	bloomSpriteData.m_textures[0] = &defferedTarget.GetRenderTargetTexture();
	bloomSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_bloomSprite.Init(bloomSpriteData);

	//�K�E�V�A���u���[�̏�����
	m_gaussianBlur[0].Init(&m_luminanceTarget.GetRenderTargetTexture());
	m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
	m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
	m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

	SpriteInitData finalSpriteData;

	//�ŏI�\���p�̉摜�̏�����
	finalSpriteData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
	finalSpriteData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
	finalSpriteData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
	finalSpriteData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
	finalSpriteData.m_width = defferedTarget.GetWidth();
	finalSpriteData.m_height = defferedTarget.GetHeight();

	finalSpriteData.m_fxFilePath = "Assets/shader/blur/bloom.fx";
	finalSpriteData.m_vsEntryPointFunc = "VSMain";
	finalSpriteData.m_psEntryPoinFunc = "PSCalcBloom";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_finalSprite.Init(finalSpriteData);
}

void Bloom::Render(RenderContext& rc, RenderTarget& defferedTarget)
{
	//�P�x���o�p�ɉ摜��`�悷��B
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceTarget);
	rc.SetRenderTargetAndViewport(m_luminanceTarget);
	rc.ClearRenderTargetView(m_luminanceTarget);
	m_bloomSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceTarget);

	//�P�x�̍���������4��A���Ńu���[��������
	m_gaussianBlur[0].ExecuteOnGPU(rc, 15);
	m_gaussianBlur[1].ExecuteOnGPU(rc, 15);
	m_gaussianBlur[2].ExecuteOnGPU(rc, 15);
	m_gaussianBlur[3].ExecuteOnGPU(rc, 15);

	//���Ƃ̉摜�͏������ɁA�u���[����K�p����B
	rc.WaitUntilToPossibleSetRenderTarget(defferedTarget);
	rc.SetRenderTargetAndViewport(defferedTarget);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(defferedTarget);
}