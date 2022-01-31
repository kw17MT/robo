#include "stdafx.h"
#include "DepthInView.h"

void DepthInView::Init(RenderTarget& mainTarget, RenderTarget& depthTarget)
{
	//�u���[���|���������C���^�[�Q�b�g�̊G�ŏ�����
	m_depthGaussian.Init(&mainTarget.GetRenderTargetTexture());

	//��ʊE�[�x��K�p�����ŏI�̉摜
	SpriteInitData finalSpriteData;
	finalSpriteData.m_textures[0] = &m_depthGaussian.GetBokeTexture();
	finalSpriteData.m_textures[1] = &depthTarget.GetRenderTargetTexture();
	finalSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;
	//�V�F�[�_�[���ŁA�[�x�l���烏�[���h���W���v�Z���������߁A�J��������݂��v���W�F�N�V�����r���[�t�s��𑗐M
	finalSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData().ViewProjInverseMatrix;
	finalSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData().ViewProjInverseMatrix);
	finalSpriteData.m_fxFilePath = "Assets/shader/blur/depthInView.fx";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Trans;
	m_finalSprite.Init(finalSpriteData);
}

void DepthInView::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	//�K�E�V�A���u���[�����s
	m_depthGaussian.ExecuteOnGPU(rc, 20);

	//�������������_�����O�^�[�Q�b�g�ɐݒ肵�ăh���[
	rc.WaitUntilToPossibleSetRenderTarget(TargetToApply);
	rc.SetRenderTargetAndViewport(TargetToApply);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(TargetToApply);
}