#pragma once

//#include "Font.h"

class FontRender : public IGameObject
{
private:
	void Render(RenderContext& rc);

	Font m_font;
	std::wstring m_text;					//!<テキスト。
	const wchar_t* m_textUnsafe = nullptr;	//!<アンセーフ版のテキスト。
	Vector2 m_position = Vector2::Zero;	//!<座標。x = 0.0f, y = 0.0fで画面の中心。
	Vector4 m_color = Vector4::White;		//!<カラー。
	float m_rotation = 0.0f;				//!<回転。
	float m_scale = 1.0f;					//!<拡大率。
	Vector2 m_pivot;						//!<ピボット。

public:
	void SetPosition(const Vector2& pos);
	void SetRotation(float rotation);
	void SetScale(float scale);

	void SetText(const wchar_t* text);

	
	
	void SetColor(const Vector3& color);
	void SetColor(const Vector4& color);
	void AddColorPoint(Vector4 colorPoint);
	Vector4 GetColor() { return m_color; }
	float GetColorVolume();
	void  AddFontScale(float size);

	void SetPivot(const Vector2& pivot);
};

