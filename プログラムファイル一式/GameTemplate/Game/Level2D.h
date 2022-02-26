
#pragma once

#include "MapChip2D.h"
#include "CaslFile.h"

struct CaslData;
/// <summary>
/// 2Dのレベルデータ。
/// </summary>
struct Level2DObjectData {
	Vector3 position = Vector3::Zero;					//座標。
	Quaternion rotation = Quaternion::Identity;			//回転。
	Vector3 scale = Vector3::One;						//拡大率。
	int width = 0;										//横幅。
	int height = 0;										//縦幅。
	const char* name;									//名前。
	const char* ddsFilePath;							//.ddsファイルのファイルパス。
	
	/**
	 * @brief 引数で渡したオブジェクト名のオブジェクトを調べる。
	 * @param objName 調べる名前
	 * @return 名前が同じ場合にtrueを返します。
	*/
	bool EqualObjectName(const char* objName) const
	{
		return strcmp(objName, name) == 0;
	}
	
	/**
	 * @brief 名前が前方一致するか調べる。
	 * @param n 調べる名前。
	 * @return 名前が同じ場合にtrueを返します。
	*/
	bool ForwardMatchName(const char* n) const
	{
		auto len = strlen(n);
		auto namelen = strlen(name);
		if (len > namelen) {
			//名前が長い。不一致。
			return false;
		}
		return strncmp(n, name, len) == 0;
	}
};
/// <summary>
/// レベル2D
/// </summary>
class Level2D : public IGameObject{
private:
	using MapChip2DPtr = std::unique_ptr<MapChip2D>;
public:
	/// <summary>
	/// レベルを初期化。
	/// </summary>
	/// <param name="filePath">caslファイルのファイルパス。</param>
	/// <param name="hookFunc">オブジェクトを作成する時の処理をフックするための関数オブジェクト。</param>
	void Init(const char* filePath, std::function<bool(Level2DObjectData & objData)> hookFunc);
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update() const;
	/// <summary>
	/// マップチップ2Dの描画処理。
	/// </summary>
	/// <param name="renderContext">レンダーコンテキスト。</param>
	void Draw(RenderContext& renderContext) const;
private:
	/// <summary>
	/// マップチップ2Dを追加する。
	/// </summary>
	/// <param name="caslData">Caslデータ</param>
	void AddMapChip2D(CaslData* caslData);
private:
	std::vector<MapChip2DPtr> m_mapChip2DPtrList;		//マップチップ2D。
	std::vector<Level2DObjectData> m_level2DObjectDataList;
	CaslFile m_casl;
};
