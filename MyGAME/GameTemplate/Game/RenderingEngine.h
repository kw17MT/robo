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
	 * @brief ���C�������_�[�^�[�Q�b�g�ɉ摜�̍������s���B
	*/
	void DrawInMainRenderTarget(RenderContext& rc);

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
		ui
	};

	void SetRenderTypes(EnRenderTypes type)
	{
		m_renderTypes = type;
	}

	EnRenderTypes GetRenderTypes()
	{
		return m_renderTypes;
	}

	//���[�V�����u���[�쐬���Ɏg�p����
	void GetDefferedSprite(RenderContext& rc)
	{
		m_defferedLighting.Draw(rc);
	}

private:
	RenderTarget m_mainRenderTarget;
	SpriteInitData m_mainSpriteData;
	Sprite m_mainSprite;

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	//���C�g���W���猩���e����邽�߂̂���
	Camera m_lightCamera;

	RenderTarget m_albedoTarget;
	RenderTarget m_normalTarget;
	RenderTarget m_specAndDepthTarget; 
	RenderTarget m_velocityTarget;

	Shadow m_shadow;
	PostEffect m_postEffect;
	DefferedLighting m_defferedLighting;
	LensGhost m_lensGhost;
	
	EnMatrixes m_mat;
	
	EnRenderTypes m_renderTypes = normal;
};

