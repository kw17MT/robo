#pragma once
class SkinModelRender;
class Enemy;

class Missile : public IGameObject
{
private:
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Vector3 m_moveDirection = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_prevMoveSpeed = Vector3::Zero;
	float count = 0;						//����
	bool m_firstCalc = false;				//�ŏ��̑��x�v�Z���ł�����
	int m_number = 0;
public:
	Missile() {}
	~Missile();
	bool Start();
	void Update();

	/**
	 * @brief ���˒n�_���璅�e�\��n�_�̃x�N�g�������߂�
	 * @return ���˒n�_���璅�e�\��n�_�̒P�ʃx�N�g��
	*/
	Vector3 CalcToTargetVec();

	void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos) /*override;*/
	{
		m_position = pos;
		m_targetPos = targetPos;
	}

	void SetNumber(int number) { m_number = number; }

	void SetEnemy(Enemy* enemy) { m_enemy = enemy; }
private:
	SkinModelRender* m_skinModelRender = nullptr;
	Enemy* m_enemy = nullptr;
};

