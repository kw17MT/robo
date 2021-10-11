#pragma once
#include "PostEffect.h"
#include "DefferedLighting.h"
#include "Shadow.h"

class RenderingEngine
{
private:
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

	struct EnMatrixes {
		Matrix prevVPMatrix = g_camera3D->GetViewProjectionMatrix();
		Matrix currentVPMatrix = g_camera3D->GetViewProjectionMatrix();
	};

	EnMatrixes& GetPrevViewProjMatrix()
	{
		return m_mat;
	}

	enum EnRenderTypes
	{
		normal = 0,
		shadow,
		lensGhost,
		ui,
		forward
	};

	void SetRenderTypes(EnRenderTypes type)
	{
		m_renderTypes = type;
	}

	EnRenderTypes GetRenderTypes()
	{
		return m_renderTypes;
	}

	//���[�V�����u���[�쐬�Ɏg�p����
	void GetDefferedSprite(RenderContext& rc)
	{
		m_defferedLighting.Draw(rc);
		//m_effectedDeffered.Draw(rc);
	}

	RenderTarget& GetShadowMap() {
		return m_shadow.GetShadowMap();
	}

	void DrawForwardRendering(RenderContext& rc, RenderTarget& target);
	void DrawForwardRendering(RenderContext& rc);
	/**
	* @brief �f�B�t�@�[�h���C�e�B���O�摜�̕ۑ����s���B
	*/
	void DrawInDefferedRenderTarget(RenderContext& rc);

	void DrawInMainRenderTarget(RenderContext& rc);

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

	RenderTarget m_forwardTarget;

	SpriteInitData m_forwardData;
	Sprite m_forward;
	RenderTarget m_sky;
};

