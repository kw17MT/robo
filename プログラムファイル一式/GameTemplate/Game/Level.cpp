#include "stdafx.h"
#include "Level.h"


void Level::Init(const char* name, std::function<bool(ObjectData& objectData)> hookFunc )
{
	m_tkl.Load(name);

	MatrixTklToLevel();

	//ルートボーンはいらないため、iを1から始めることで除外できる。
	for (int i = 1; i < m_bonelist.size(); i++) {
		auto bone = m_bonelist[i].get();
		if (bone->GetParentBoneNo() == 0) {

			//マップチップを作成する。
			//行列からポジションを作成する。
			ObjectData objData;
			bone->CalcWorldTRS(objData.Pos, objData.Rot, objData.Scale);
			
			//ZupとYupの変更。
			float fix = objData.Pos.y;
			objData.Pos.y = objData.Pos.z;
			objData.Pos.z = -fix;

			fix = objData.Rot.y;
			objData.Rot.y = objData.Rot.z;
			objData.Rot.z = -fix;

			std::swap(objData.Scale.y, objData.Scale.z);
			//Zup⇔Yup　終わり。

			objData.name = m_bonelist[i]->GetName();
			objData.number = m_bonelist[i]->GetNo();

			//パスの作成
			wchar_t filePath[256];
			//swprintf_s(filePath, L"Assets/modelData/%s.tkm", objData.name);
			swprintf_s(filePath, L"Assets/modelData/%s.tkm", objData.name);
			//マルチバイトに変換 wchar→ char
			//ベースの文字列。
			size_t origsize = wcslen(filePath) + 1;
			//変換した文字列
			size_t convertedChars = 0;
			//マルチバイト格納用
			char strCon[] = "";
			//サイズ
			size_t strConSize = (strlen(strCon) + 1) * 2;
			//変換後のサイズ。
			const size_t newsize = origsize * 2;
			//連結後のサイズ
			char* nstring = new char[newsize + strConSize];
			//マルチバイトに変換する。入りきらないものは切りすて
			wcstombs_s(&convertedChars, nstring, newsize, filePath, _TRUNCATE);
			//文字列の連結
			_mbscat_s((unsigned char*)nstring, newsize + strConSize, (unsigned char*)strCon);




			//Hookが登録済みならばマップチップは作成不要
			//false のままなら作成する。
			bool isHooked = false;
			//hookFuncになにも入っていない＝Sobjectには何もない。
			if (hookFunc != nullptr) {
				isHooked = hookFunc(objData);
			}

			//hookがfalseなままならば。
			if (isHooked == false) {
				std::unique_ptr<MapChip> mapChip;
				mapChip = std::make_unique<MapChip>();
				mapChip->Init(nstring, objData.Pos, objData.Scale, objData.Rot);
				m_mapChipList.push_back(std::move(mapChip));
			}



			//MapChipの構造体にunique_ptrを追加
			//ここでマップチップを作成する。
			/*std::unique_ptr<MapChip> mapChip;
			mapChip = std::make_unique<MapChip>();
			mapChip->Init(nstring, objData.Pos, objData.Scale, objData.Rot);

			m_mapChipList.push_back(std::move(mapChip));*/
		}
	}
}

void Level::Draw()
{
	for (const auto& mapchip : m_mapChipList) {
		mapchip->Draw();
	}
}

void Level::Draw(RenderContext& rc)
{
	for (const auto& mapchip : m_mapChipList) {
		mapchip->Draw(rc);
	}
}

void Level::MatrixTklToLevel()
{
	m_tkl.QuaryObject([&](TklFile::SObject& tklObj) {
		//コピー。
		//バインドポーズ。
		Matrix bindPoseMatrix;
		memcpy(bindPoseMatrix.m[0], &tklObj.bindPose[0], sizeof(tklObj.bindPose[0]));
		memcpy(bindPoseMatrix.m[1], &tklObj.bindPose[1], sizeof(tklObj.bindPose[1]));
		memcpy(bindPoseMatrix.m[2], &tklObj.bindPose[2], sizeof(tklObj.bindPose[2]));
		memcpy(bindPoseMatrix.m[3], &tklObj.bindPose[3], sizeof(tklObj.bindPose[3]));
		bindPoseMatrix.m[0][3] = 0.0f;
		bindPoseMatrix.m[1][3] = 0.0f;
		bindPoseMatrix.m[2][3] = 0.0f;
		bindPoseMatrix.m[3][3] = 1.0f;

		/// <summary>
		/// バインドポーズの逆行列
		/// 列3はVector3変換の際に不要なので初期化。
		/// </summary>
		Matrix invBindPoseMatirx;
		memcpy(invBindPoseMatirx.m[0], &tklObj.invBindPose[0], sizeof(tklObj.invBindPose[0]));
		memcpy(invBindPoseMatirx.m[1], &tklObj.invBindPose[1], sizeof(tklObj.invBindPose[1]));
		memcpy(invBindPoseMatirx.m[2], &tklObj.invBindPose[2], sizeof(tklObj.invBindPose[2]));
		memcpy(invBindPoseMatirx.m[3], &tklObj.invBindPose[3], sizeof(tklObj.invBindPose[3]));
		invBindPoseMatirx.m[0][3] = 0.0f;
		invBindPoseMatirx.m[1][3] = 0.0f;
		invBindPoseMatirx.m[2][3] = 0.0f;
		invBindPoseMatirx.m[3][3] = 1.0f;

		wchar_t boneName[256];

		/// <summary>
		/// tklのオブジェクト名の型をワイド文字に変換。
		/// char -> wchar_t
		/// 変換するchar型の文字列。
		/// 変換結果のwchar_t型文字列。
		/// サイズ
		/// </summary>
		mbstowcs(boneName, tklObj.name.get(), 256);
		BonePtr bone = std::make_unique<Bone>(
			boneName,
			bindPoseMatrix,
			invBindPoseMatirx,
			tklObj.parentNo,
			tklObj.no
			);
		//ボーンを積む。
		m_bonelist.push_back(std::move(bone));
		//コピー終わり。
		});


	//不要かも		コードの書き方として参考になり得るため今のところ残している。
	//親座標系の行列になっているので、親が存在しているボーンからは親座標系をかき消していく。
	//for (auto& bone : m_bonelist) {
	//	if (bone->GetParentBoneNo() != -1) {
	//		//親ボーンが存在するとき
	//		//親ボーンを取得しておく。
	//		auto& parentBone = m_bonelist.at(bone->GetParentBoneNo());
	//		//親とする配列に子供を入れて登録しておく。
	//		parentBone->AddChild(bone.get());
	//		//親のバインドポーズの取得。
	//		Matrix parentInv = parentBone->GetInvBindPoseMatrix();
	//		//子どものマトリックスに変換する。
	//		Matrix LocalMat = bone->GetBindPoseMatrix() * parentInv;
	//		bone->SetLocalMatrix(LocalMat);
	//	}
	//	else {
	//		//もともとのマトリックスを入れておく。
	//		bone->SetLocalMatrix(bone->GetBindPoseMatrix());
	//	}
	//	m_matrixlist = std::make_unique<Matrix[]>(m_bonelist.size());	
	//	m_isInited = true;
	//}
}

//bool Level::EqualObjectName(const wchar_t* objName)
//{
//	return wcscmp(objName, name) == 0;
//}