#pragma once

#include "MapChip2D.h"

struct CaslData;

struct Level2DObjectData {
	//座標。０で初期化
	Vector3 position = Vector3::Zero;
	//回転。０で初期化
	Quaternion rotation = Quaternion::Identity;
	//拡大率。１で初期化
	Vector3 scale = Vector3::One;
	int width = 0;
	int height = 0;
	const char* name;
	const char* ddsFilePath;

	//レベルの内のオブジェクトの名前を調べて等しい（０）ならばTURE
	bool EqualObjectName(const char* objName) const
	{
		return strcmp(objName, name) == 0;
	}

	bool ForwardMatchName(const char* n) const
	{
		auto len = strlen(n);
		auto namelen = strlen(name);
		if (len > namelen) {
			return false;
		}
		return strncmp(n, name, len) == 0;
	}
};

class Level2D {
private:
	using MapChip2DPtr = std::unique_ptr<MapChip2D>;
public:
	void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc);

	void Update() const;
	/// マップチップ2Dの描画処理。
	void Draw(RenderContext& renderContext) const;
private:
	/// マップチップ2Dを追加する。
	void AddMapChip2D(CaslData* caslData);
private:
	std::vector<MapChip2DPtr> m_mapChip2DPtrList;		//マップチップ2D。
};