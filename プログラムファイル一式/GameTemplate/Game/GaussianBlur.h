#pragma once
#include "Sprite.h"

class GaussianBlur
{
private:
	enum { NUM_WEIGHTS = 8 };				//�d�݂̐��B
	float m_weights[NUM_WEIGHTS];			//�d�݃e�[�u���B
	Texture* m_originalTexture = nullptr;	//�I���W�i���e�N�X�`���B
	RenderTarget m_xBlurRenderTarget;		//���{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	RenderTarget m_yBlurRenderTarget;		//�c�{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	Sprite m_xBlurSprite;					//���{�P�摜��`�悷�邽�߂̃X�v���C�g�B
	Sprite m_yBlurSprite;					//�c�{�P�摜��`�悷�邽�߂̃X�v���C�g�B

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���������B
	/// </summary>
	void InitRenderTargets();
	/// <summary>
	/// �X�v���C�g���������B
	/// </summary>
	void InitSprites();
	/// <summary>
	/// �d�݃e�[�u�����X�V����B
	/// </summary>
	void UpdateWeightsTable(float blurPower);

public:
	
	//�K�E�V�A���u���[��������e�N�X�`���B
	void Init(Texture* originalTexture);
	/// <summary>
	/// �K�E�V�A���u���[��GPU��Ŏ��s�B
	/// </summary>
	/// <remarks>
	/// �{�֐��̌Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���K�v������܂��B
	/// </remarks>
	/// <param name="rc">�����_�����O�^�[�Q�b�g</param>
	/// <param name="blurPower">�u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B</param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);
	/// <summary>
	/// �{�P�e�N�X�`�����擾�B
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}
};

