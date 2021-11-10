#pragma once
class SpriteRender;
class MissileTargetIcon;
class Enemy;
class CrossIcon;
class CapturedSquareIcon;

class Reticle;
class MissileGenerator;

class EnemyStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_scale[2] = { Vector3::One, Vector3::Zero };
	Vector3 m_enemyPos = Vector3::Zero;
	bool m_isFirstExpand = false;
	bool m_isCaptured = false;			//�������ߑ����ꂽ��
	bool m_nextTarget = false;			//�v���C���[�����݃^�[�Q�b�g���Ă���G���|���ꂽ�玟�Ɏ����I�Ƀ^�[�Q�b�g�����G�ł��邩�ǂ���

	enum EnemyState
	{
		enemyTooFar,					//�o�c�̉摜�����o��
		enemyNear,						//�l�p�̉摜���o��
		enemyTargeted,					//�^�[�Q�b�g����Ă��邩�炷�ׂĂ̊g�嗦���[����
	}m_enemyState;

public:
	EnemyStateIcon() {}
	~EnemyStateIcon();
	bool Start();
	void Update();
	void SetEnemyPos(Vector3& pos) { m_enemyPos = pos; }
	bool IsTargeted() {
		if (m_enemyState == enemyTargeted) { return true; }
		else { return false; }
	}

	/**
	 * @brief �G�̃C���X�^���X�ŋ����������ɂ����
	*/
	void JudgeState(int distance);

	void DisplayIcons();

	void IconBehaviour();

	const void SetEnemyObject(Enemy* enemy) { m_enemy = enemy; }

	void SetReticleInstance(Reticle* reticle) { m_reticle = reticle; }


private:
	std::vector<MissileTargetIcon*> m_missileTargetIcon;

	CrossIcon* m_crossIcon = nullptr;
	CapturedSquareIcon* m_squareIcon = nullptr;

	Reticle* m_reticle = nullptr;					//���e�B�N���̃C���X�^���X�ۑ��p
	MissileGenerator* m_missileGenerator = nullptr;	//�~�T�C��������̃C���X�^���X�ۑ��p

	Enemy* m_enemy;
};

