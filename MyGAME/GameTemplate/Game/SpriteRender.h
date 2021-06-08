#pragma once
class SpriteRender : public IGameObject
{
private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;

	Vector3 m_pos;
	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };
	Quaternion m_Rot = Quaternion::Identity;
	Vector2 m_pivot = { 0.5f,0.5f };

public:
	SpriteRender() {};

	void Update();

	/**
	 * @brief 画像の初期化を行う
	 * @param name ファイルパスの名前
	 * @param width 横幅
	 * @param height 縦幅
	*/
	void Init(const char* name, int width, int height);

	void SetPosition(Vector3 pos) { m_pos = pos; }
	void SetScale(Vector3 scale) {m_scale = scale;}
	void SetRotation(Quaternion& rot) { m_Rot = rot; }
	void SetPivot(Vector2 pivot) { m_pivot = pivot; }
	//カラー変更(Vector4版)
	void SetColor(const Vector4& color) { m_sprite.ColorUpdate(color); }
	//カラー変更(float版)
	void SetColor(const float& r,const float& g,const float& b,const float& a) { m_sprite.ColorUpdate(r,g,b,a); }
	void Render(RenderContext& rc) { m_sprite.Draw(rc); }
};


