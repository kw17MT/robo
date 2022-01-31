#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& mainRenderTarget, RenderTarget& normalTarget, RenderTarget& specAndDepthTarget, RenderTarget& velocityTarget)
{
	//�x���V�e�B�}�b�v�ɃK�E�V�A���u���[��K�p
	m_gaussian[0].Init(&velocityTarget.GetRenderTargetTexture());
	m_gaussian[1].Init(&m_gaussian[0].GetBokeTexture());
	m_gaussian[2].Init(&m_gaussian[1].GetBokeTexture());
	m_gaussian[3].Init(&m_gaussian[2].GetBokeTexture());

	//���[�V�����u���[�Ŏg�������_�[�^�[�Q�b�g�̏�����
	m_motionBlurTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	SpriteInitData motionBlurData;
	motionBlurData.m_width = 1280;
	motionBlurData.m_height = 720;
	motionBlurData.m_textures[0] = &m_motionBlurTarget.GetRenderTargetTexture();
	motionBlurData.m_textures[1] = &velocityTarget.GetRenderTargetTexture();
	motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	motionBlurData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	motionBlurData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	motionBlurData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	motionBlurData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurSprite.Init(motionBlurData);

	//�ŏI�\���p�̉摜�̏�����
	SpriteInitData finalSpriteData;
	finalSpriteData.m_textures[0] = &m_gaussian[0].GetBokeTexture();
	finalSpriteData.m_textures[1] = &m_gaussian[1].GetBokeTexture();
	finalSpriteData.m_textures[2] = &m_gaussian[2].GetBokeTexture();
	finalSpriteData.m_textures[3] = &m_gaussian[3].GetBokeTexture();
	finalSpriteData.m_width = mainRenderTarget.GetWidth();
	finalSpriteData.m_height = mainRenderTarget.GetHeight();
	finalSpriteData.m_fxFilePath = "Assets/shader/blur/bloom.fx";
	finalSpriteData.m_vsEntryPointFunc = "VSMain";
	finalSpriteData.m_psEntryPoinFunc = "PSCalcBloom";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_finalGaussian.Init(finalSpriteData);
}

void MotionBlur::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& targetToApply)
{
	//���[�V�����u���[�K�p��̃����_�[�^�[�Q�b�g���쐬�B
	rc.WaitUntilToPossibleSetRenderTarget(m_motionBlurTarget);
	rc.SetRenderTargetAndViewport(m_motionBlurTarget);
	rc.ClearRenderTargetView(m_motionBlurTarget);
	// ���C�������_�����O�^�[�Q�b�g�̃e�N�X�`�����g����
	// 2D��S��ʂɏ��������ɕύX����B
	mainSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_motionBlurTarget);

	//���C�������_�[�^�[�Q�b�g�̉�ʂɃK�E�V�A���u���[���|����
	m_gaussian[0].ExecuteOnGPU(rc, 15);
	m_gaussian[1].ExecuteOnGPU(rc, 15);
	m_gaussian[2].ExecuteOnGPU(rc, 15);
	m_gaussian[3].ExecuteOnGPU(rc, 15);

	//���[�V�����u���[�摜�����C�������_�[�^�[�Q�b�g�ɉ��Z
	rc.WaitUntilToPossibleSetRenderTarget(targetToApply);
	rc.SetRenderTargetAndViewport(targetToApply);
	m_motionBlurSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(targetToApply);
}