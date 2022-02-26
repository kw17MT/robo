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
	/// function<�߂�l�^�i�����j> �֐��̖��O
	/// </summary>
	void Init(const char* name, std::function<bool(ObjectData& objectData) > Hookfunc);	

	/// <summary>
	/// �}�b�v�`�b�v�̐���for�����񂵂ă}�b�v�`�b�v��`��
	/// </summary>
	void Draw();

	void Draw(RenderContext& rc);

	/// <summary>
	/// Tkl�̃}�g���b�N�X��Level�ɕϊ�����B
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