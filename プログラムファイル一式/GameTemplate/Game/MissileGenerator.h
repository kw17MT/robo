#pragma once
class Missile;
class AmmoGauge;
class Enemy;

/**
 * @brief �~�T�C���𐶐�����N���X
*/
class MissileGenerator : public IGameObject
{
private:
	Vector3 m_launchPos = Vector3::Zero;	//���˂���錳�̈ʒu
		
	int m_lockOnTargetNum = 0;				//���܂Ń��b�N�I�����Ă����^�[�Q�b�g�̐�
	int m_remaining_missile = 20;			//�c��̃~�T�C����

	bool m_isPrepareLaunch = false;			//LB2�{�^��������������Ĕ��ˏ�������Ă��邩			
	bool m_canTargetMore = true;			//����Ƀ��b�N�I���ł��邩�ǂ���
	bool m_deleteMissileIcon = false;		//�~�T�C�����b�N�I���̃A�C�R�����������ǂ���
public:
	/**
	 * @brief �f�X�g���N�^
	*/
	MissileGenerator() {}

	/**
	 * @brief �R���X�g���N�^
	*/
	~MissileGenerator();

	/**
	 * @brief �C���X�^���X���������ꂽ���Ɉ�x�����s����֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief ���ˌ��̈ʒu��ݒ肷��֐�
	 * @param pos �v���C���[�̈ʒu
	*/
	void SetLaunchPos(Vector3 pos) { m_launchPos = pos; }

	/**
	 * @brief �~�T�C�������˂���ă��b�N�I���A�C�R�����������ǂ������擾����
	 * @return �A�C�R�����������ǂ���
	*/
	bool GetDeleteMissileIcon() { return m_deleteMissileIcon; }

	/**
	 * @brief �~�T�C���̒��e��ƂȂ�G�̃C���X�^���X��ۑ�����
	 * @param enemy �~�T�C���̒��e��ƂȂ�G
	*/
	void SaveTargetedEnemy(Enemy* enemy);

	/**
	 * @brief ����Ƀ~�T�C���Ń��b�N�I���ł��邩�ǂ������擾����
	 * @return TRUE�ł���@FALSE�ł��Ȃ�
	*/
	bool CanTargetMore() { return m_canTargetMore; }
private:
	std::vector<Missile*> m_missiles = { nullptr };				//��������������~�T�C���̃C���X�^���X
	std::vector<Enemy*> m_enemy = { nullptr };					//���b�N�I����ƂȂ�G�̃C���X�^���X�ۑ��p
	AmmoGauge* m_ammoGauge = nullptr;							//�c��e���̕\���Ƃ��̃Q�[�W���o���C���X�^���X
};