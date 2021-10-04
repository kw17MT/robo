#pragma once
class Shadow
{
public:
	/**
	 * @brief �V���h�E�̃����_�����O�^�[�Q�b�g��������
	*/
	void InitShadowTarget();

	/**
	 * @brief �I�t�X�N���[���ɉe���쐬����B
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Render(RenderContext& rc);

	/**
	 * @brief �쐬�����V���h�E�}�b�v��Ԃ�
	 * @return �V���h�E�}�b�v
	*/
	RenderTarget& GetShadowMap()
	{
		return m_shadow;
	}
	
	//�J�X�P�[�h�V���h�E���쐬����
	void RenderCascade(RenderContext& rc);

	struct Matrixes
	{
		Matrix s_LVPCMatrix[3];
		Matrix inv = g_camera3D->GetViewProjectionMatrix();
	};

	void InitCascade(RenderTarget& mainTarget, RenderTarget& normalTexture, RenderTarget& specAndDepthTexture);

	void Draw(RenderContext& rc);

private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//����������F
	RenderTarget m_shadow;										//�e���������ރ^�[�Q�b�g

	/////////////////////////////////////////////////////////
	static const int m_cascadeShadowNum = 3;
	int m_nearArea = g_camera3D->GetNear();
	const float m_shadowArea[m_cascadeShadowNum] = { 1000, 5000, g_camera3D->GetFar() };
	RenderTarget m_cascadeShadowMap[m_cascadeShadowNum];

	SpriteInitData m_cascadeShadowData;
	Sprite m_cascadeSprite;

	Matrixes mat;
};

