#include "stdafx.h"
#include "SpriteRender.h"

void SpriteRender::Init(const char* name, int width, int height)
{
	SpriteInitData spdata;
	//�摜�̃t�@�C���p�X����ݒ�
	spdata.m_ddsFilePath[0] = name;
	//�V�F�[�_�[��ݒ�
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	//�J���[�t�H�[�}�b�g��ݒ�
	spdata.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//������ݒ�
	spdata.m_width = width;
	//������ݒ�
	spdata.m_height = height;
	//�A���t�@�u�����f�B���O���[�h��L����
	spdata.m_alphaBlendMode = AlphaBlendMode_Trans; 

	m_sprite.Init(spdata);
}

void SpriteRender::InitGauge(const char* name, int width, int height)
{
	//SpriteInitData spdata;
	//�摜�̃t�@�C���p�X����ݒ�
	spdata.m_ddsFilePath[0] = name;
	//�V�F�[�_�[��ݒ�
	spdata.m_fxFilePath = "Assets/shader/gaugeSprite.fx";
	//�J���[�t�H�[�}�b�g��ݒ�
	spdata.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//������ݒ�
	spdata.m_width = width;
	//������ݒ�
	spdata.m_height = height;

	spdata.m_expandConstantBuffer = (void*)&s_param;
	spdata.m_expandConstantBufferSize = sizeof(s_param);

	m_texture.InitFromDDSFile(L"Assets/Image/bluetec.dds");
	spdata.m_expandShaderResoruceView = &m_texture;

	//�A���t�@�u�����f�B���O���[�h��L����
	spdata.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_sprite.Init(spdata);
}

void SpriteRender::Update()
{
	//�ʒu�A�g�嗦�A���S����ݒ�
	m_sprite.Update(m_pos, m_rot, m_scale, m_pivot);

	s_param.scrollAmount += GameTime().GetFrameDeltaTime() / 10.0f;
	if (s_param.scrollAmount >= 1.0f)
	{
		s_param.scrollAmount = 0.0f;
	}
}