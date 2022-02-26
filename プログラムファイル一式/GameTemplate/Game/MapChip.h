#pragma once
#include "LightManager.h"

class MapChip
{
public:
	/// <summary>
	/// �������֐��B
	/// </summary>
	/// <param name="path01">�I�u�W�F�N�g�̃p�X</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="rot">��]</param>
	void Init(const char* path01, Vector3 pos, Vector3 scale, Quaternion rot);

	//�g�p����V�F�[�_�[��ݒ�B
	void Draw();

	void Draw(RenderContext& rc);

private:
	Model m_model;
	ModelInitData m_modeldata;
	AllLight m_lig = LightManager::GetInstance().GetLightData();
};

