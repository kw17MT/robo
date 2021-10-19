#pragma once
class SpriteRender;

class EnemyStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale[2] = { Vector3::One, Vector3::Zero };
	Vector3 m_enemyPos = Vector3::Zero;
	bool m_isFirstExpand = false;
	bool m_isCaptured = false;			//�������ߑ����ꂽ��

	enum EnemyState
	{
		enemyTooFar,					//�o�c�̉摜�����o��
		enemyNear,						//�l�p�̉摜���o��
		enemyTargeted					//�^�[�Q�b�g����Ă��邩�炷�ׂĂ̊g�嗦���[����
	}m_enemyState;

public:
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetEnemyPos(Vector3& pos) { m_enemyPos = pos; }

	/**
	 * @brief �G�̃C���X�^���X�ŋ����������ɂ����
	*/
	void JudgeState(int distance);

	void DisplayIcons();

	void IconBehaviour();

	void CalcPosition();
private:
	SpriteRender* m_spriteRender[2] = { nullptr, nullptr };
};

