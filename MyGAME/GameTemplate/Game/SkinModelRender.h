#pragma once
#include "LightManager.h"
class AnimationClip;

class SkinModelRender :public IGameObject
{
private:
	ModelInitData m_modelInitData;										//���f���̃t�@�C���p�X��V�F�[�_�[��ݒ肷��
	ModelInitData m_shadowData;											//�e�����̂��߂̃��f�����
	Model m_model;														//���f��
	Model m_shadow;														//�e���f��
	Skeleton m_skeleton;												//�X�P���g��

	Vector3 m_position = Vector3::Zero;									//���f���̈ʒu���W
	Vector3 m_scale = Vector3::One;										//���f���̊g�嗦
	Quaternion m_rot = Quaternion::Identity;							//���f���̉�]

	bool m_isSun = false;												//���z���ǂ���

	struct copyToVRAMDatas
	{
		AllLight s_lig = LightManager::GetInstance().GetLightData();
	}s_dataCopyToVRAM;

public:
	SkinModelRender() {};
	//�C���X�^���X��j��
	~SkinModelRender();

	bool Start() { return true; }

	/**
	 * @brief �X�P���g���A�A�j���[�V�����A�ʒu���W�A��]��ݒ肷��B
	*/
	void Update();

	/**
	 * @brief �ʒu���W��ݒ肷��B
	 * @param pos �ʒu���W
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }
	/**
	 * @brief �g�嗦��ݒ肷��B
	 * @param scale �g�嗦
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }

	/**
	 * @brief ��]����ݒ肷��B
	 * @param rot ��]��
	*/
	void SetRotation(Quaternion rot) { m_rot = rot; }

	/**
	 * @brief ���f���̍��W��ݒ肷��B
	 * @return �V�������W
	*/
	Vector3 GetPosition() const { return m_position; }

	/**
	 * @brief ���f���̊g�嗦��ݒ肷��B
	 * @return �V�����g�嗦
	*/
	Vector3 GetScale() const { return m_scale; }

	/**
	 * @brief ���f���̏��������s���B���C�g��Ǝ��̂��̂ɂ������Ƃ��͂�����������
	 * @param filePath ���f���̃t�@�C���p�X
	 * @param skeletonPath �X�P���g���̃t�@�C���p�X
	 * @param UpAxis �ǂ̎�����ɂ��邩
	 * @param pos ���f�����o��������ŏ��̈ʒu
	*/
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, bool isCastShadow);
	//���f���̃t�@�C���p�X�݂̂�ύX����Ƃ��Ɏg�p����B

	/**
	 * @brief �e���f����̂̃��f���������֐�
	 * @param filePath ���f���̃t�@�C���p�X
	 * @param skeletonPath �X�P���g���̃t�@�C���p�X
	 * @param UpAxis �ǂ̎�����ɂ��邩
	 * @param pos �����ʒu
	*/
	void InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief ���f���̃t�@�C���p�X��ς������Ƃ��Ɏg�p
	 * @param newModelFilePath �V�����t�@�C���p�X
	*/
	void ChangeModel(const char* newModelFilePath);

	/**
	 * @brief �A�j���[�V������ݒ肷��B
	 * @param animation �A�j���[�V����
	 * @param animationNum �A�j���[�V�����̐�
	*/
	void InitAnimation(AnimationClip* animation, int animationNum);

	/**
	 * @brief �A�j���[�V�������Đ�����
	 * @param animNo �A�j���[�V�����̐�
	 * @param interpolateTime ����鎞��
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);

	void SetIsSun() { m_isSun = true; }

	/**
	 * @brief ���f����`��
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Render(RenderContext& rc) 
	{
		//���ʕ`��
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::normal) {
			m_model.Draw(rc);
			return;
		}
		//�e���
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::shadow) {
			m_shadow.Draw(rc, RenderingEngine::GetInstance()->GetLightCamera());
			return;
		}
		//���z�����`�恁�����Y�S�[�X�g�쐬
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::lensGhost) {
			if (m_isSun)
			{
				m_model.Draw(rc);
			}
			return;
		}
	}

private:
	//�A�j���[�V����
	Animation m_animation;
	AnimationClip* m_animationClip = nullptr;
};

