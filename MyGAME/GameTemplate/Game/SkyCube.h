#pragma once
class SkinModelRender;

class SkyCube : public IGameObject
{
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	SkyCube() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~SkyCube();

	/**
	 * @brief �C���X�^���X�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;			//��C���X�^���X
};		