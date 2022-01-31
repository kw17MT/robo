#pragma once
#include "PostEffect.h"
#include "DefferedLighting.h"
#include "Shadow.h"

struct EnMatrixes {
	Matrix prevVPMatrix = g_camera3D->GetViewProjectionMatrix();				//1�t���[���O�̃r���[�v���W�F�N�V�����s��
	Matrix currentVPMatrix = g_camera3D->GetViewProjectionMatrix();				//���݂̃r���[�v���W�F�N�V�����s��
};

class RenderingEngine
{
private:

	//�V���O���g���C���X�^���X
	static RenderingEngine* instance;
public:
	/**
	 * @brief �C���X�^���X���쐬����B
	 * @return ���̃C���X�^���X�B
	*/
	static void CreateInstance()
	{
		instance = new RenderingEngine;
	}

	/**
	 * @brief �C���X�^���X���擾����B
	*/
	static RenderingEngine* GetInstance()
	{
		return instance;
	}

	/**
	 * @brief �C���X�^���X���폜����B
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief �����_�����O�ɕK�v�ȏ������s��
	*/
	void PrepareRendering();

	/**
	 * @brief �����_�����O�ɕK�v�Ȃ��ׂẴI�t�X�N���[���^�[�Q�b�g������������B
	*/
	void InitRenderTargets();

	/**
	 * @brief �����_�����O�ɕK�v�Ȃ��ׂẲ摜������������B
	*/
	void InitSprites();

	/**
	 * @brief ���C�g�J�����̏��������s��
	*/
	void InitLightCamera();

	/**
	 * @brief �V���h�E�쐬�ɕK�v�ȃ��C�g�J�������擾����B
	*/
	Camera& GetLightCamera()
	{
		return m_lightCamera;
	}

	/**
	 * @brief �����_�����O���s��
	*/
	void Render(RenderContext& rc);

	/**
	 * @brief ���݂�1�t���[���O�̃v���W�F�N�V�����r���[�s����擾
	 * @return ���݂�1�t���[���O�̃v���W�F�N�V�����r���[�s��
	*/
	EnMatrixes& GetPrevViewProjMatrix()
	{
		return m_mat;
	}

	/**
	 * @brief �����_�����O����ΏیQ
	*/
	enum EnRenderTypes
	{
		normal = 0,				//�ʏ�`��
		shadow,					//�V���h�E���f����`��
		lensGhost,				//�����Y�S�[�X�g�p���f���̕`��
		ui						//UI�`��
	};

	/**
	 * @brief ����`�悷�郂�[�h���ݒ肷��
	 * @param type �����_�����O����ΏیQ
	*/
	void SetRenderTypes(EnRenderTypes type)
	{
		m_renderTypes = type;
	}

	/**
	 * @brief ����`�悷�郂�[�h���擾����
	 * @return �`�悷�郂�[�h
	*/
	EnRenderTypes GetRenderTypes()
	{
		return m_renderTypes;
	}

	/**
	 * @brief �t�H���[�h�����_�����O���郂�f���i�n�ʁj�ɃV���h�E�}�b�v�𑗂�
	 * @return �V���h�E�}�b�v
	*/
	RenderTarget& GetShadowMap() {
		return m_shadow.GetShadowMap();
	}

	/**
	 * @brief �f�B�t�@�[�h���C�e�B���O����1���̉摜�����C���̃����_�[�^�[�Q�b�g�ɏ�������
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void DrawInMainRenderTarget(RenderContext& rc);

	/**
	 * @brief ���C���̃����_�[�^�[�Q�b�g��UI����������
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void DrawUI(RenderContext& rc);

	/**
	 * @brief ���m�N��������x������ݒ肷��
	 * @param rate ���m�N����
	*/
	void SetMonochromeRate(float rate) { m_monochromeRate = rate; }

	/**
	 * @brief ���m�N�������擾����
	 * @return ���m�N����
	*/
	const float& GetMonochromeRate() const { return m_monochromeRate; }

private:
	//�ŏI�\������I�t�X�N���[���摜
	RenderTarget m_mainRenderTarget;
	SpriteInitData m_mainSpriteData;
	Sprite m_mainSprite;

	//�f�B�t�@�[�h���C�e�B���O�ς݂̉摜�Ƀ|�X�g�G�t�F�N�g���|���邽�߂̂���
	RenderTarget m_captureDeffered;
	SpriteInitData m_effectedDefferedData;
	Sprite m_effectedDeffered;
	
	//�V���h�E�֘A
	Shadow m_shadow;
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };		//��ʏ����p�̐F
	Camera m_lightCamera;								//���C�g�J����

	RenderTarget m_albedoTarget;						//�A���x�h�}�b�v
	RenderTarget m_normalTarget;						//�@���}�b�v
	RenderTarget m_specAndDepthTarget;					//�X�y�L�������˂Ɛ[�x�l
	RenderTarget m_velocityTarget;						//���x�}�b�v

	DefferedLighting m_defferedLighting;				//�f�B�t�@�[�h�摜
	PostEffect m_postEffect;							//�|�X�g�G�t�F�N�g����
	
	EnMatrixes m_mat;									//VelocityMap�쐬�̂��߂̃}�g���b�N�X
	
	EnRenderTypes m_renderTypes = normal;				//�����_�����O���[�h

	float m_monochromeRate = 0.0f;						//���m�N����
};

