#pragma once

//#include "Font.h"

class FontRender : public IGameObject
{
private:
	void Render(RenderContext& rc);

	Font m_font;
	std::wstring m_text;					//!<�e�L�X�g�B
	const wchar_t* m_textUnsafe = nullptr;	//!<�A���Z�[�t�ł̃e�L�X�g�B
	Vector2 m_position = Vector2::Zero;	//!<���W�Bx = 0.0f, y = 0.0f�ŉ�ʂ̒��S�B
	Vector4 m_color = Vector4::White;		//!<�J���[�B
	float m_rotation = 0.0f;				//!<��]�B
	float m_scale = 1.0f;					//!<�g�嗦�B
	Vector2 m_pivot;						//!<�s�{�b�g�B

public:
	/**
	 * @brief �ʒu��ݒ肷��
	 * @param pos �V�����ʒu
	*/
	void SetPosition(const Vector2& pos);

	/**
	 * @brief �V������]��ݒ肷��
	 * @param rotation �V������]
	*/
	void SetRotation(float rotation);

	/**
	 * @brief �V�����g�嗦�̐ݒ�
	 * @param scale �V�����g�嗦
	*/
	void SetScale(float scale);

	/**
	 * @brief �e�L�X�g�̐ݒ�
	 * @param text �e�L�X�g
	*/
	void SetText(const wchar_t* text);

	/**
	 * @brief �A���t�@�����̐F�̐ݒ�
	 * @param color �F
	*/
	void SetColor(const Vector3& color);

	/**
	 * @brief �A���t�@���݂̐F�̐ݒ�
	 * @param color �F
	*/
	void SetColor(const Vector4& color);

	/**
	 * @brief �F�̉��Z
	 * @param colorPoint ���Z����F
	*/
	void AddColorPoint(Vector4 colorPoint);

	/**
	 * @brief �e�L�X�g�̐F��Ԃ�
	 * @return �e�L�X�g�̐F
	*/
	Vector4 GetColor() { return m_color; }

	/**
	 * @brief �J���[�̐F�����̒��x��Ԃ�
	 * @return �S���̐F�̕��ϒl
	*/
	float GetColorVolume();

	/**
	 * @brief �t�H���g�̃T�C�Y��ύX����
	 * @param size ���Z����g�嗦
	*/
	void  AddFontScale(float size);

	/**
	 * @brief �摜�̒��S�����߂�
	 * @param pivot ���S�ɂ������Ƃ���
	*/
	void SetPivot(const Vector2& pivot);
};

