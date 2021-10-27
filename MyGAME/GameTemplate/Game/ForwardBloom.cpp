#include "stdafx.h"
#include "ForwardBloom.h"

void ForwardBloom::Init(RenderTarget& specialBloomTarget)
{
	m_luminanceTarget.Create(
		specialBloomTarget.GetWidth(),   // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		specialBloomTarget.GetHeight(),  // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		1,
		1,
		specialBloomTarget.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//�u���[���摜�̏������p
	SpriteInitData bloomSpriteData;
	bloomSpriteData.m_fxFilePath = "Assets/shader/blur/forwardBloom.fx";
	bloomSpriteData.m_vsEntryPointFunc = "VSMain";
	bloomSpriteData.m_psEntryPoinFunc = "PSLuminance";
	bloomSpriteData.m_width = specialBloomTarget.GetWidth();
	bloomSpriteData.m_height = specialBloomTarget.GetHeight();
	//�P�x�𒊏o��������ʂ��擾
	bloomSpriteData.m_textures[0] = &specialBloomTarget.GetRenderTargetTexture();
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
	finalSpriteData.m_width = specialBloomTarget.GetWidth();
	finalSpriteData.m_height = specialBloomTarget.GetHeight();

	finalSpriteData.m_fxFilePath = "Assets/shader/blur/forwardBloom.fx";
	finalSpriteData.m_vsEntryPointFunc = "VSMain";
	finalSpriteData.m_psEntryPoinFunc = "PSLuminance";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_finalSprite.Init(finalSpriteData);
}

void ForwardBloom::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	//�P�x���o�p�ɉ摜��`�悷��B
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceTarget);
	rc.SetRenderTargetAndViewport(m_luminanceTarget);
	//rc.ClearRenderTargetView(m_luminanceTarget);
	m_bloomSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceTarget);

	//�P�x�̍���������4��A���Ńu���[��������
	m_gaussianBlur[0].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[1].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[2].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[3].ExecuteOnGPU(rc, 5);

	//���Ƃ̉摜�͏������ɁA�u���[����K�p����B
	rc.WaitUntilToPossibleSetRenderTarget(TargetToApply);
	rc.SetRenderTargetAndViewport(TargetToApply);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(TargetToApply);
}