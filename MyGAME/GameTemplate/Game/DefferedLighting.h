#pragma once
class DefferedLighting
{
public:
	/**
	 * @brief �f�B�t�@�[�h���C�e�B���O�K�p�̉摜�������֐�
	 * @param albedoMap �A���x�h�}�b�v
	 * @param normalMap �@���}�b�v
	 * @param specAndDepthMap ���ʔ��ˁA�[�x�l�}�b�v
	 * @param shadowMap �V���h�E�}�b�v
	 * @param speedMap �X�s�[�h�}�b�v
	*/
	void InitSprite(RenderTarget& albedoMap, 
		RenderTarget& normalMap, 
		RenderTarget& specAndDepthMap, 
		RenderTarget& shadowMap,
		RenderTarget& velocityMap);

	/**
	 * @brief �f�B�t�@�[�h�����_�����O���J�n����
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Render(RenderContext& rc);

	/**
	 * @brief �쐬�����f�B�t�@�[�h���C�e�B���O�ς݉摜��`�悷��B
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Draw(RenderContext& rc);
private:									
	RenderTarget* m_defferedTargets[4];		//MRT�p	
	SpriteInitData m_defferedSpriteData;	//�f�B�t�@�[�h�����_�����O�摜�̃p�����[�^
	Sprite m_defferedSprite;				//�ŏI�̉摜

	Texture m_groundTextures[4];
};

