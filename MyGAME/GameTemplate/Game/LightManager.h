#pragma once
#include "RenderingEngine.h"

struct DirectionalLight
{

	//�f�B���N�V�������C�g
	Vector3 directionalDirection = { 0.0f,-1.0f,0.0f };				//�f�B���N�V�������C�g�̕���
	float pad0 = 0;
	Vector3 directionalColor = { 20.0f, 20.0f, 20.0f };				//�f�B���N�V�������C�g�̐F
	//Vector3 directionalColor = { 1.0f, 1.0f, 1.0f };
	float pad1 = 0;
	Vector3 eyePos = g_camera3D->GetPosition();						//�ڂ̈ʒu�i�J�����̈ʒu�j
	float specPow = 0.5f;											//���˂̓x����
};

struct SpotLight
{
	//�X�|�b�g���C�g�@�R���X�g���N�^�ŏ������A���K��
	Vector3 spotPosition = Vector3::Zero;
	float pad3 = 0;
	Vector3 spotColor = Vector3::One;								//���C�g�̐F
	float spotRange = 1500.0f;
	Vector3 spotDirection = Vector3::Zero;					//���C�g�̌���
	float spotAngle = Math::DegToRad(90.0f);
};

struct AllLight
{
	Matrix s_lightCameraMatrix = RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
	Matrix ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
	//�f�B���N�V�������C�g
	DirectionalLight directionalLight;
	//����
	Vector3 ambientLight = { 0.4f, 0.4f, 0.4f };
	float pad2 = 0;
	//�X�|�b�g���C�g
	SpotLight spotLight[2];
};

class LightManager : public IGameObject
{
private:
	static LightManager* instance;
	AllLight s_allLight;
public:
	LightManager();
	/**
	 * @brief �C���X�^���X�����
	*/
	static void CreateInstance()
	{
		instance = new LightManager;
	}

	/**
	 * @brief �C���X�^���X���擾����
	 * @return �V���O���g���C���X�^���X
	*/
	static LightManager& GetInstance()
	{
		static LightManager instance;
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
	 * @brief �S�Ẵ��C�g�̏����擾����
	 * @return �S�Ẵ��C�g�̏��
	*/
	const AllLight& GetLightData() const
	{
		return s_allLight;
	}

	/**
	 * @brief ���t���[�����C�g�̈ʒu���X�V����
	*/
	void UpdateEyePos()
	{
		s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();
		s_allLight.s_lightCameraMatrix = RenderingEngine::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix.Inverse();
	}
};

