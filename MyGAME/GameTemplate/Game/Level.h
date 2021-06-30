#pragma once
using namespace std;
#include "TklFile.h"
#include"MapChip.h"
#include "TklFile.h"

struct ObjectData
{
	Vector3 Pos;
	Vector3 Scale;
	Quaternion Rot;
	const wchar_t* name;
	int number;
};

class Level : public IGameObject
{
public:
	/// <summary>
	/// function<戻り値型（引数）> 関数の名前
	/// </summary>
	void Init(const char* name, std::function<bool(ObjectData& objectData) > Hookfunc);	

	/// <summary>
	/// マップチップの数分for文を回してマップチップを描画
	/// </summary>
	void Draw();

	void Draw(RenderContext& rc);

	/// <summary>
	/// TklのマトリックスをLevelに変換する。
	/// </summary>
	void MatrixTklToLevel();

	//bool EqualObjectName(const wchar_t* objName);

private:
	bool m_isInited = false;
	using BonePtr = std::unique_ptr<Bone>;
	Skeleton m_skeleton;
	vector<std::unique_ptr<MapChip>> m_mapChipList;
	TklFile m_tkl;
	std::vector<BonePtr> m_bonelist;
	std::unique_ptr<Matrix[]> m_matrixlist;
};