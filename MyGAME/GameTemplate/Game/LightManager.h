#pragma once

struct DirectionalLight
{

	//�f�B���N�V�������C�g
	Vector3 directionalDirection = { 0.0f,-1.0f,0.0f };				//�f�B���N�V�������C�g�̕���
	float pad0 = 0;
	Vector3 directionalColor = { 10.0f, 10.0f, 10.0f };				//�f�B���N�V�������C�g�̐F
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
	Matrix s_lightCameraMatrix = GameObjectManager::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
	Matrix ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
	//�f�B���N�V�������C�g
	DirectionalLight directionalLight;

	//����
	Vector3 ambientLight = { 0.4f, 0.4f, 0.4f };
	float pad2 = 0;

	//�X�|�b�g���C�g
	SpotLight spotLight[2];
};

struct LightForGauge
{
	Vector3 ambient = { 0.0f,1.0f, 0.0f };
};

class LightManager : public IGameObject
{
private:
	static LightManager* instance;
	AllLight s_allLight;
	LightForGauge s_gaugeLight[3];
public:
	LightManager();
	/**
	 * @brief �C���X�^���X�����
	*/
	static LightManager CreateInstance()
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
	static LightManager DeleteInstance()
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
	 * @brief �Q�[�W��p�̃��C�g���擾����
	 * @return 
	*/
	const LightForGauge& GetGaugeLight(int lightNumber) const
	{
		return s_gaugeLight[lightNumber];
	}

	/**
	 * @brief �����Ŏw�肵���Q�[�W�̐F�����F�ɂ���
	 * @param lightNumber �@�Q�[�W�̔ԍ��@�O���A�P�E�A�Q�����p
	*/
	void MakeGaugeLightYellow(int lightNumber);

	/**
	 * @brief �����Ŏw�肵���Q�[�W�̐F��ԐF�ɂ���
	 * @param lightNumber �@�Q�[�W�̔ԍ��@�O���A�P�E�A�Q�����p
	*/
	void MakeGaugeLightRed(int lightNumber);

	/**
	 * @brief �����Ŏw�肵���Q�[�W�̐F��ΐF�ɂ���
	 * @param lightNumber �@�Q�[�W�̔ԍ��@�O���A�P�E�A�Q�����p
	*/
	void MakeGaugeLightGreen(int lightNumber);

	/**
	 * @brief ���t���[�����C�g�̈ʒu���X�V����
	*/
	void UpdateEyePos()
	{
		s_allLight.directionalLight.eyePos = g_camera3D->GetPosition();

		s_allLight.ViewProjInverseMatrix = g_camera3D->GetViewProjectionMatrix();
		s_allLight.ViewProjInverseMatrix.Inverse();
	}

	/**
	 * @brief �����Ŏw�肵���ԍ��̃Q�[�W�p���C�g���擾����
	 * @param gaugeNumber �Q�[�W�̔ԍ��@�O���A�P�E�A�Q�����p
	 * @return �����Ŏw�肵���ԍ��̃Q�[�W�p���C�g
	*/
	 LightForGauge& GetGaugeLight(int gaugeNumber) 
	{
		return s_gaugeLight[gaugeNumber];
	}
};

