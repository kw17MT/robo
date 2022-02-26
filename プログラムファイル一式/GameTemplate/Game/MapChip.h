#pragma once
#include "LightManager.h"

class MapChip
{
public:
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <param name="path01">オブジェクトのパス</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="rot">回転</param>
	void Init(const char* path01, Vector3 pos, Vector3 scale, Quaternion rot);

	//使用するシェーダーを設定。
	void Draw();

	void Draw(RenderContext& rc);

private:
	Model m_model;
	ModelInitData m_modeldata;
	AllLight m_lig = LightManager::GetInstance().GetLightData();
};

