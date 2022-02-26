#include "stdafx.h"
#include "Level.h"


void Level::Init(const char* name, std::function<bool(ObjectData& objectData)> hookFunc )
{
	m_tkl.Load(name);

	MatrixTklToLevel();

	//���[�g�{�[���͂���Ȃ����߁Ai��1����n�߂邱�Ƃŏ��O�ł���B
	for (int i = 1; i < m_bonelist.size(); i++) {
		auto bone = m_bonelist[i].get();
		if (bone->GetParentBoneNo() == 0) {

			//�}�b�v�`�b�v���쐬����B
			//�s�񂩂�|�W�V�������쐬����B
			ObjectData objData;
			bone->CalcWorldTRS(objData.Pos, objData.Rot, objData.Scale);
			
			//Zup��Yup�̕ύX�B
			float fix = objData.Pos.y;
			objData.Pos.y = objData.Pos.z;
			objData.Pos.z = -fix;

			fix = objData.Rot.y;
			objData.Rot.y = objData.Rot.z;
			objData.Rot.z = -fix;

			std::swap(objData.Scale.y, objData.Scale.z);
			//Zup��Yup�@�I���B

			objData.name = m_bonelist[i]->GetName();
			objData.number = m_bonelist[i]->GetNo();

			//�p�X�̍쐬
			wchar_t filePath[256];
			//swprintf_s(filePath, L"Assets/modelData/%s.tkm", objData.name);
			swprintf_s(filePath, L"Assets/modelData/%s.tkm", objData.name);
			//�}���`�o�C�g�ɕϊ� wchar�� char
			//�x�[�X�̕�����B
			size_t origsize = wcslen(filePath) + 1;
			//�ϊ�����������
			size_t convertedChars = 0;
			//�}���`�o�C�g�i�[�p
			char strCon[] = "";
			//�T�C�Y
			size_t strConSize = (strlen(strCon) + 1) * 2;
			//�ϊ���̃T�C�Y�B
			const size_t newsize = origsize * 2;
			//�A����̃T�C�Y
			char* nstring = new char[newsize + strConSize];
			//�}���`�o�C�g�ɕϊ�����B���肫��Ȃ����̂͐؂肷��
			wcstombs_s(&convertedChars, nstring, newsize, filePath, _TRUNCATE);
			//������̘A��
			_mbscat_s((unsigned char*)nstring, newsize + strConSize, (unsigned char*)strCon);




			//Hook���o�^�ς݂Ȃ�΃}�b�v�`�b�v�͍쐬�s�v
			//false �̂܂܂Ȃ�쐬����B
			bool isHooked = false;
			//hookFunc�ɂȂɂ������Ă��Ȃ���Sobject�ɂ͉����Ȃ��B
			if (hookFunc != nullptr) {
				isHooked = hookFunc(objData);
			}

			//hook��false�Ȃ܂܂Ȃ�΁B
			if (isHooked == false) {
				std::unique_ptr<MapChip> mapChip;
				mapChip = std::make_unique<MapChip>();
				mapChip->Init(nstring, objData.Pos, objData.Scale, objData.Rot);
				m_mapChipList.push_back(std::move(mapChip));
			}



			//MapChip�̍\���̂�unique_ptr��ǉ�
			//�����Ń}�b�v�`�b�v���쐬����B
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
		//�R�s�[�B
		//�o�C���h�|�[�Y�B
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
		/// �o�C���h�|�[�Y�̋t�s��
		/// ��3��Vector3�ϊ��̍ۂɕs�v�Ȃ̂ŏ������B
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
		/// tkl�̃I�u�W�F�N�g���̌^�����C�h�����ɕϊ��B
		/// char -> wchar_t
		/// �ϊ�����char�^�̕�����B
		/// �ϊ����ʂ�wchar_t�^������B
		/// �T�C�Y
		/// </summary>
		mbstowcs(boneName, tklObj.name.get(), 256);
		BonePtr bone = std::make_unique<Bone>(
			boneName,
			bindPoseMatrix,
			invBindPoseMatirx,
			tklObj.parentNo,
			tklObj.no
			);
		//�{�[����ςށB
		m_bonelist.push_back(std::move(bone));
		//�R�s�[�I���B
		});


	//�s�v����		�R�[�h�̏������Ƃ��ĎQ�l�ɂȂ蓾�邽�ߍ��̂Ƃ���c���Ă���B
	//�e���W�n�̍s��ɂȂ��Ă���̂ŁA�e�����݂��Ă���{�[������͐e���W�n�����������Ă����B
	//for (auto& bone : m_bonelist) {
	//	if (bone->GetParentBoneNo() != -1) {
	//		//�e�{�[�������݂���Ƃ�
	//		//�e�{�[�����擾���Ă����B
	//		auto& parentBone = m_bonelist.at(bone->GetParentBoneNo());
	//		//�e�Ƃ���z��Ɏq�������ēo�^���Ă����B
	//		parentBone->AddChild(bone.get());
	//		//�e�̃o�C���h�|�[�Y�̎擾�B
	//		Matrix parentInv = parentBone->GetInvBindPoseMatrix();
	//		//�q�ǂ��̃}�g���b�N�X�ɕϊ�����B
	//		Matrix LocalMat = bone->GetBindPoseMatrix() * parentInv;
	//		bone->SetLocalMatrix(LocalMat);
	//	}
	//	else {
	//		//���Ƃ��Ƃ̃}�g���b�N�X�����Ă����B
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