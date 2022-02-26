#include "stdafx.h"
#include "PostEffect.h"
#include "GameDirector.h"

void PostEffect::Init(RenderTarget& mainRenderTarget,
	RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specDepthMap,
	RenderTarget& velocityMap)
{
	//��ʊE�[�x�̏�����
	m_depthInView.Init(mainRenderTarget, specDepthMap);
	//�u���[���̏�����
	m_bloom.Init(mainRenderTarget);
	//FXAA�̏�����
	m_fxaa.Init(mainRenderTarget);
	//���[�V�����u���[�̏�����
	m_motionBlur.InitSprite(mainRenderTarget, normalMap, velocityMap);
	//�����Y�S�[�X�g�̏�����
	m_lensGhost.Init();
	//���m�N�����̏�����
	m_monochrome.Init();
}

void PostEffect::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget)
{
	//�Q�[���V�[�����^�C�g���Ȃ��
	if (GameDirector::GetInstance().GetGameScene() == enTitle)
	{
		//��ʊE�[�x�K�p
		m_depthInView.Render(rc, mainTarget);
	}
	//����ȊO�̃V�[����
	else
	{
		//���[�V�����u���[�K�p
		m_motionBlur.Render(rc, mainSprite, mainTarget);
	}
	//�u���[���K�p
	m_bloom.Render(rc, mainTarget);
	//FXAA�K�p
	m_fxaa.Render(rc, mainTarget);
	//�����Y�S�[�X�g�K�p
	m_lensGhost.Render(rc, mainTarget);
	//���m�N�����̐��l�������
	if (RenderingEngine::GetInstance()->GetMonochromeRate() != 0.0f)
	{
		//���m�N�����K�p
		m_monochrome.Render(rc, mainSprite, mainTarget);
	}
}