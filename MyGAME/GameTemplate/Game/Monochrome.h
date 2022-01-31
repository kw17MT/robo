#pragma once

class Monochrome
{
private:
	float m_monochromeRate = 0.0f;			//���m�N���x
public:
	/**
	 * @brief ���m�N����������^�[�Q�b�g�ƃ��m�N�����ł����X�v���C�g�̏�����
	*/
	void Init();

	/**
	 * @brief ���m�N���������������C����ʂ��R�s�[���A���m�N���������摜��`�悷��
	 * @param rc �����_�[�R���e�L�X�g
	 * @param mainSprite ���C���̃Q�[�����
	 * @param mainTarget ���C���̃Q�[����ʂ��g�������_�[�^�[�Q�b�g
	*/
	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	RenderTarget m_monochromedTarget;		//���m�N��������������ʂ��R�s�[���Ă��郌���_�[�^�[�Q�b�g
	Sprite m_monochromedSprite;				//���m�N���������摜
};

