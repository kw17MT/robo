#include "stdafx.h"
#include "FontRender.h"


//シャドウ
/*void SetShadowParam(bool isDrawShadow, float shadowOffset, const CVector4 & shadowColor)
{
	m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
}*/

//テキストを設定。
void FontRender::SetText(const wchar_t* text)
{
	if (wcscmp(m_text.c_str(), text) != 0) {
		//文字列が変わった。
		m_text = text;
	}
}

//void FontRender::SetFont(DirectX::SpriteFont* font)
//{
//	m_font.SetFont(font);
//}
	
void FontRender::SetPosition(const Vector2& pos)
{
	m_position = pos;
}
void FontRender::SetColor(const Vector3 & color)
{
	m_color = color;
}
void FontRender::SetColor(const Vector4 & color)
{
	color.CopyTo(m_color);
}
void FontRender::SetRotation(float rotation)
{
	m_rotation = rotation;
}
void FontRender::SetScale(float scale)	{
	m_scale = scale;
}
void FontRender::SetPivot(const Vector2& pivot)
{
	m_pivot = pivot;
}

void FontRender::Render(RenderContext& rc)
{
	m_font.Begin(rc);
	const wchar_t* text = nullptr;

	if (m_text.c_str() != nullptr) {
		text = m_text.c_str();
	}

	m_font.Draw(
		text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);
	m_font.End(rc);
}
