#pragma once
class SpriteRender : public IGameObject
{
private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;

	Vector3 m_Pos;
	Vector3 m_Scale = { 1.0f, 1.0f, 1.0f };
	Quaternion m_Rot = Quaternion::Identity;
	Vector2 m_Pivot = { 0.5f,0.5f };

public:
	SpriteRender() {};
	/// <summary>
	/// 初期化する関数。
	/// </summary>
	/// <param name="name">画像のパス</param>
	/// <param name="a">表示する横幅</param>
	/// <param name="b">表示する縦幅</param>
	void Init(const char* name, int a, int b);

	void SetPosition(Vector3 pos){ m_sprite.Update(pos, m_Rot, m_Scale, m_Pivot); }
	void SetScale(Vector3 scale) { m_sprite.Update(m_Pos, m_Rot, scale, m_Pivot); }
	//カラー変更(Vector4版)
	void SetColor(const Vector4& color) { m_sprite.ColorUpdate(color); }
	//カラー変更(float版)
	void SetColor(const float& r,const float& g,const float& b,const float& a) { m_sprite.ColorUpdate(r,g,b,a); }
	void Render(RenderContext& rc) { m_sprite.Draw(rc); }
};


