#pragma once
class SpriteRender;
class FontRender;

//�e�̎��
enum EnAmmoTypes
{
	enAmmoBullet,					//�}�V���K���̒e
	enAmmoMissile,					//�~�T�C��
	enAmmoRaser						//���[���K��
};

class AmmoGauge : public IGameObject
{
private:
	Vector3 m_scale = { 2.0f,1.0f,1.0f };		//�Q�[�W�̊g�嗦
	Vector3 m_screenPos = Vector3::Zero;		//�X�N���[����ł̈ʒu
	Vector2 m_pivot = { 1.0f,0.0f };			//�摜�̒��S
	const float m_fontScale = 0.5f;					//�����̊g�嗦

	int m_remaining_ammo = 0;					//�c�e��
	int m_max_ammo = 0;							//��ނɉ������ő�e��

	float m_reloadTimer = 0.0f;					//�����[�h�^�C�}�[
	bool m_isReloaded = true;					//�����[�h����Ă��邩�i�c�e�͂��邩�j
	bool m_finishReloading = false;				//���̃t���[���Ƀ����[�h���I�������

	float m_gaugeScaleX = 1.0f;

	EnAmmoTypes m_ammoType = enAmmoBullet;		//�ǂ̎�ނ̒e���Ǘ����邩
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	AmmoGauge() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~AmmoGauge();

	/**
	 * @brief �C���X�^���X���������ꂽ���ɌĂ΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief �����[�h���s���֐�
	 * @param reloadTime �����[�h�ɂ����鎞��
	*/
	void Reloading(const float reloadTime);

	/**
	 * @brief �����[�h����Ă��邩
	 * @return TRUE�@����Ă���@FALSE�@����Ă��Ȃ�
	*/
	bool GetIsReloaded() { return m_isReloaded; }

	/**
	 * @brief ���̃t���[���Ń����[�h���I��������m�F����
	 * @return ���̃t���[���Ń����[�h���I�������
	*/
	bool GetIsFinishedReloaded() { return m_finishReloading; }

	/**
	 * @brief �e�̎�ނ�ݒ肷��
	 * @param ammoType  �e�̎��
	*/
	void SetAmmoType(EnAmmoTypes ammoType) { m_ammoType = ammoType; }

	/**
	 * @brief ��ނɉ������c�e����ݒ肷��
	 * @param remainingAmmo �c�e��
	*/
	void SetRemainingAmmo(int remainingAmmo) { m_remaining_ammo = remainingAmmo; }

	/**
	 * @brief ��ނɉ������ő�e����ݒ肷��
	 * @param maxAmmo �ő�e��
	*/
	void SetMaxAmmo(int maxAmmo) { m_max_ammo = maxAmmo; }

	/**
	 * @brief �ʒu���X�V����
	 * @param pos �ʒu
	*/
	void SetPosition(Vector3 pos) { m_screenPos = pos; }
private:
	SpriteRender* m_spriteRender = nullptr;				//�Q�[�W�摜�C���X�^���X
	SpriteRender* m_gaugeFrame = nullptr;
	FontRender* m_fontRender = nullptr;					//�c�e���C���X�^���X
};

