#pragma once

struct BarParam
{
	float barDeleteRate = 0.0f;							//��ɃQ�[�W�̉摜�Ɏg�p�B�摜���ǂ̂��炢�������̃��[�g�i�E�[��������j
	float scrollAmount = 0.0f;
};

struct WarningSignParam
{
	float alpha = 1.0f;									//���ߗ�
};

class SpriteRender : public IGameObject
{
private:
	Sprite m_sprite;									//�摜
	SpriteInitData m_spriteData;						//�摜�̃f�[�^
	Vector3 m_pos = Vector3::Zero;						//�摜�̕\���ʒu
	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };				//�摜�̏����g�嗦
	Quaternion m_rot = Quaternion::Identity;			//�摜�̉�]
	Vector2 m_pivot = { 0.5f,0.5f };					//�摜�̒��S

	Texture m_texture;
	BarParam s_param;
	WarningSignParam s_signParam;
	SpriteInitData spdata;

	enum {
		enRenderNormal
	};

public:
	SpriteRender() {};

	/**
	 * @brief �摜�̊g�嗦��ʒu�A��]�̂̍X�V
	*/
	void Update();

	/**
	 * @brief �摜�̏��������s��
	 * @param name �t�@�C���p�X�̖��O
	 * @param width ����
	 * @param height �c��
	*/
	void Init(const char* name, int width, int height, const char* PixelShader = "PSMain");

	void InitGauge(const char* name, int width, int height);

	void SetSpriteSizeRate(float rate) { s_param.barDeleteRate = rate; }

	void SetSpriteAlpha(float addColorRate) { s_signParam.alpha = addColorRate; }

	/**
	 * @brief �V�����ʒu�̐ݒ�
	 * @param pos �V�����ʒu
	*/
	void SetPosition(Vector3 pos) { m_pos = pos; }

	/**
	 * @brief �V�����g�嗦�̐ݒ�
	 * @param scale �V�����g�嗦
	*/
	void SetScale(Vector3 scale) {m_scale = scale;}

	/**
	 * @brief �V������]�̐ݒ�
	 * @param rot �V������]
	*/
	void SetRotation(Quaternion& rot) { m_rot = rot; }

	/**
	 * @brief �V�������S�̐ݒ�
	 * @param pivot �V�������S
	*/
	void SetPivot(Vector2 pivot) { m_pivot = pivot; }

	/**
	 * @brief �J���[�ύX(Vector4��)
	 * @param color �F
	*/
	void SetColor(const Vector4 color) { m_sprite.ColorUpdate(color); }

	/**
	 * @brief �摜�̃A���t�@��ύX����
	 * @param alpha �A���t�@�l
	*/
	void SetAlpha(const float alpha) {	m_sprite.SetAlpha(alpha);}
	/**
	 * @brief �J���[�ύX(float��)
	 * @param r �ԐF����
	 * @param g �ΐF����
	 * @param b �F����
	 * @param a �����x����
	*/
	void SetColor(const float& r,const float& g,const float& b,const float& a) { m_sprite.ColorUpdate(r,g,b,a); }

	/**
	 * @brief �摜��`��
	 * @param rc �����_�[�^�[�Q�b�g
	*/
	void Render(RenderContext& rc) 
	{ 
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::GetInstance()->ui) {
			m_sprite.Draw(rc);
		}
		else {
			return;
		}
	}
};


