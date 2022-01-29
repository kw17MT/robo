#include "stdafx.h"
#include "SkinModelRender.h"

SkinModelRender::~SkinModelRender()
{
	DeleteGO(this);
}

//���f����ʏ�`�悷�邽�߂̏�����
void SkinModelRender::Init(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, bool isCastShadow)
{
	//���f���̃t�@�C���p�X�ݒ�
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//���f�����g�p����V�F�[�_�[
	m_modelInitData.m_fxFilePath = "Assets/shader/deffered/defferedmodel.fx";
	//���_�V�F�[�_�[�ݒ�
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
	//�g���F�͈̔͐ݒ�
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�ǂ̎�����ɂ��邩
	m_modelInitData.m_modelUpAxis = UpAxis;
	
	m_modelInitData.m_expandConstantBuffer = (void*)&s_matrix;//RenderingEngine::GetInstance()->GetPrevViewProjMatrix();
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_matrix/*RenderingEngine::GetInstance()->GetPrevViewProjMatrix()*/);

	//���f���̃X�P���g��������Ȃ�
	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	//�V���h�E�L���X�^�[�Ȃ��
	if (isCastShadow)
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
		m_shadowData.m_vsSkinEntryPointFunc = "VSSkinMain";


		m_shadowData.m_modelUpAxis = UpAxis;

		if (skeletonPath != nullptr) {
			m_shadowData.m_skeleton = &m_skeleton;
		}

		m_shadow.Init(m_shadowData);
	}

	m_model.Init(m_modelInitData);
}

void SkinModelRender::InitCharaCon(float radius, float height, Vector3& position)
{
	//m_charaCon.Init(radius, height, position);
}

void SkinModelRender::InitGround(const char* modelFilePath, EnModelUpAxis UpAxis, Vector3 pos, bool isPhyisicsStatic)
{
	//���f���̃t�@�C���p�X�ݒ�
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//���f�����g�p����V�F�[�_�[�i����PBR�̂݁j
	m_modelInitData.m_fxFilePath = "Assets/shader/forward/ground.fx";
	//���_�V�F�[�_�[�ݒ�
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSMain";
	//�g���F�͈̔͐ݒ�
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�ǂ̎�����ɂ��邩
	m_modelInitData.m_modelUpAxis = UpAxis;

	m_texture[0].InitFromDDSFile(L"Assets/Image/ground/noise.dds");
	m_texture[1].InitFromDDSFile(L"Assets/Image/ground/Sand_Albedo.dds");
	m_texture[2].InitFromDDSFile(L"Assets/Image/ground/snow.dds");
	m_texture[3].InitFromDDSFile(L"Assets/Image/ground/grass.dds");

	m_modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetShadowMap().GetRenderTargetTexture();
	m_modelInitData.m_expandShaderResoruceView[1] = &m_texture[0];
	m_modelInitData.m_expandShaderResoruceView[2] = &m_texture[1];
	m_modelInitData.m_expandShaderResoruceView[3] = &m_texture[2];
	m_modelInitData.m_expandShaderResoruceView[4] = &m_texture[3];

	m_modelInitData.m_expandConstantBuffer = (void*)&s_matrix;/*RenderingEngine::GetInstance()->GetPrevViewProjMatrix();*/
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_matrix/*RenderingEngine::GetInstance()->GetPrevViewProjMatrix()*/);

	m_model.Init(m_modelInitData);

	//�����Ń��f���̑S���𒲂ׂ�R�[�h���쐬����B
	Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	Vector3 vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };

	const auto& tkmFile = m_model.GetTkmFile();
	tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		for (const auto& vertex : mesh.vertexBuffer) {
			vMax.Max(vertex.pos);
			vMin.Min(vertex.pos);
		}
		});

	//s_matrix.width = (vMax.x - vMin.x) / m_texture[0].GetWidth();
	//s_matrix.height = (vMax.y - vMin.y) / m_texture[0].GetHeight();

	if (isPhyisicsStatic)
	{
		//m_physicsStatic.CreateFromModel(m_model, m_model.GetWorldMatrix());
	}
}

void SkinModelRender::InitSkyCube(const char* modelFilePath, EnModelUpAxis UpAxis)
{
	//���f���̃t�@�C���p�X�ݒ�
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//���f�����g�p����V�F�[�_�[�i����PBR�̂݁j
	m_modelInitData.m_fxFilePath = "Assets/shader/SkyCube.fx";
	//���_�V�F�[�_�[�ݒ�
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_psEntryPointFunc = "PSMain";
	//�g���F�͈̔͐ݒ�
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�ǂ̎�����ɂ��邩
	m_modelInitData.m_modelUpAxis = UpAxis;

	m_texture[0].InitFromDDSFile(L"Assets/modelData/preset/skyCubeMapSunrise_Toon.dds");

	m_modelInitData.m_expandConstantBuffer = (void*)&s_matrix;
	m_modelInitData.m_expandConstantBufferSize = sizeof(s_matrix);

	m_modelInitData.m_expandShaderResoruceView[0] = &m_texture[0];

	m_model.Init(m_modelInitData);
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

	m_animation.Progress(GameTime().GetFrameDeltaTime());

	m_model.UpdateWorldMatrix(m_position, m_rot, m_scale);

	m_shadow.UpdateWorldMatrix(m_position, m_rot, m_scale);
	//�X�P���g�����X�V�B
	m_skeleton.Update(m_model.GetWorldMatrix());
}