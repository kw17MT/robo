#pragma once
#include "Bloom.h"
#include "FXAA.h"
#include "DepthInView.h"
#include "SSR.h"
#include "MotionBlur.h"
#include "LensGhost.h"
#include "Monochrome.h"

class PostEffect
{
public:
	/**
	 * @brief �|�X�g�G�t�F�N�g�Ɏg�p���郌���_�����O�^�[�Q�b�g���e�|�X�g�G�t�F�N�g�C���X�^���X�ɑ���
	 * @param defferedTarget �f�B�t�@�[�h���C�e�B���O�������C���Q�[�����
	 * @param albedoMap	�A���x�h�}�b�v
	 * @param normalMap �@���}�b�v
	 * @param specDepthMap ���ʔ��˂Ɛ[�x�l�}�b�v
	 * @param velocityMap ���F���V�e�B�}�b�v
	*/
	void Init(RenderTarget& defferedTarget, 
		RenderTarget& albedoMap,
		RenderTarget& normalMap,
		RenderTarget& specDepthMap,
		RenderTarget& velocityMap
		 );

	/**
	 * @brief �e�|�X�g�G�t�F�N�g��`�悷�邩�ǂ������߂�
	 * @param rc �����_�[�R���e�L�X�g
	 * @param mainSprite ���C���Q�[����ʂ̉摜
	 * @param mainTarget ���C���Q�[����ʂ̃����_�[�^�[�Q�b�g
	*/
	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	DepthInView m_depthInView;			//��ʊE�[�x�C���X�^���X
	Bloom m_bloom;						//�u���[���C���X�^���X
	FXAA m_fxaa;						//FXAA�C���X�^���X
	MotionBlur m_motionBlur;			//���[�V�����u���[�C���X�^���X
	LensGhost m_lensGhost;				//�����Y�S�[�X�g�C���X�^���X
	Monochrome m_monochrome;			//���m�N�����C���X�^���X
};