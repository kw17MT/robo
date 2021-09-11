#include "stdafx.h"
#include "SkinModelRender.h"

SkinModelRender::~SkinModelRender()
{
	DeleteGO(this);
}

//���f����ʏ�`�悷�邽�߂̏�����
void SkinModelRender::Init(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	//���f���̃t�@�C���p�X�ݒ�
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//���f�����g�p����V�F�[�_�[�i����PBR�̂݁j
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//���_�V�F�[�_�[�ݒ�
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSMain";
	//�g���F�͈̔͐ݒ�
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�ǂ̎�����ɂ��邩
	m_modelInitData.m_modelUpAxis = UpAxis;
	
	m_modelInitData.m_expandConstantBuffer = &s_dataCopyToVRAM.s_lig;
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_dataCopyToVRAM.s_lig);

	//���f���̃X�P���g��������Ȃ�
	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);
}

//�Q�[�W�̃��f���̏�����
void SkinModelRender::InitAsGauge(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, int gaugeNumber)
{
	//���f���̃t�@�C���p�X�ݒ�
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//���f�����g�p����V�F�[�_�[�i����PBR�̂݁j
	m_modelInitData.m_fxFilePath = "Assets/shader/forGauge.fx";
	//���_�V�F�[�_�[�ݒ�
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSMain";
	//�g���F�͈̔͐ݒ�
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	//�ǂ̎�����ɂ��邩
	m_modelInitData.m_modelUpAxis = UpAxis;
	//�Q�[�W��p�̌����擾
	m_modelInitData.m_expandConstantBuffer = &LightManager::GetInstance().GetGaugeLight(gaugeNumber);
	m_modelInitData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetGaugeLight(gaugeNumber));

	//���f���̃X�P���g��������Ȃ�
	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);
}

//�e���o�����߂̏�����
void SkinModelRender::InitForCastShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_shadowData.m_tkmFilePath = modelFilePath;

	/****************************************************/
	/*	�f�v�X�V���h�E��K�p							*/
	/*	�V���h�E�}�b�v�̃J���[�o�b�t�@�[���ύX���邱�Ɓ@*/
	/****************************************************/
	m_shadowData.m_fxFilePath = "Assets/shader/shadow/drawDepthShadowMap.fx";
	m_shadowData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;


	/****************************************************/
	/*	���e�V���h�E��K�p								*/
	/*	�V���h�E�}�b�v�̃J���[�o�b�t�@�[���ύX���邱�Ɓ@*/
	/****************************************************/
	//m_shadowData.m_fxFilePath = "Assets/shader/shadow/drawProjectionShadowMap.fx";
	//m_shadowData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_shadowData.m_vsEntryPointFunc = "VSMain";
	m_shadowData.m_vsSkinEntryPointFunc = "VSMain";


	m_shadowData.m_modelUpAxis = UpAxis;	

	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_shadowData.m_skeleton = &m_skeleton;
	}

	m_shadow.Init(m_shadowData);

	m_isCastShadow = true;
}

void SkinModelRender::InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	/****************************************************/
	/*	���e�V���h�E��K�p								*/
	/****************************************************/
	//m_modelInitData.m_fxFilePath = "Assets/shader/shadow/projectionShadowReciever.fx";

	/****************************************************/
	/*	�f�v�X�V���h�E��K�p							*/
	/****************************************************/
	//m_modelInitData.m_fxFilePath = "Assets/shader/shadow/depthShadowReciever.fx";
	m_modelInitData.m_fxFilePath = "Assets/shader/deffered/defferedmodel.fx";

	//�J���[�o�b�t�@�[�̃t�H�[�}�b�g�͋���
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_modelInitData.m_modelUpAxis = UpAxis;

	//�V���h�E�}�b�v�̃e�N�X�`���A���C�g�J�����̃r���[�v���s��̎擾
	m_modelInitData.m_expandShaderResoruceView = &GameObjectManager::GetInstance()->GetShadowMap().GetRenderTargetTexture();

	m_modelInitData.m_expandConstantBuffer = (void*)&s_dataCopyToVRAM;
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_dataCopyToVRAM);
	

	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}


	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitAsFloor(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	//m_modelInitData.m_fxFilePath = "Assets/shader/shadow/shadowRecieverForFloor.fx";
	m_modelInitData.m_fxFilePath = "Assets/shader/deffered/defferedmodel.fx";

	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_modelInitData.m_modelUpAxis = UpAxis;

	m_modelInitData.m_expandShaderResoruceView = &GameObjectManager::GetInstance()->GetShadowMap().GetRenderTargetTexture();

	m_modelInitData.m_expandConstantBuffer = (void*)&s_dataCopyToVRAM;
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_dataCopyToVRAM);


	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);
}

void SkinModelRender::ChangeModel(const char* newModelFilePath)
{
	m_modelInitData.m_tkmFilePath = newModelFilePath;
	if (m_isCastShadow) {
		m_shadowData.m_tkmFilePath = newModelFilePath;
	}
}


void SkinModelRender::InitAnimation(AnimationClip* animationClip, int animationNum)
{
	m_animationClip = animationClip;

	m_animation.Init(m_skeleton, m_animationClip, animationNum);
}

void SkinModelRender::PlayAnimation(int animNo, float interpolateTime)
{
	m_animation.Play(animNo, interpolateTime);
}

void SkinModelRender::Update()
{
	//�X�P���g�����X�V�B
	m_skeleton.Update(m_model.GetWorldMatrix());

	m_animation.Progress(GameTime().GetFrameDeltaTime());
	
	m_model.UpdateWorldMatrix(m_position, m_rot, m_scale);
	m_shadow.UpdateWorldMatrix(m_position, m_rot, m_scale);
}