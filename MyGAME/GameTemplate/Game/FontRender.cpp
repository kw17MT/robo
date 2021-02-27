#include "stdafx.h"
#include "FontRender.h"


FontRender::FontRender()
{

}

void FontRender::WriteFont(const wchar_t* letter, RenderContext& rc)
{
	Font font;
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };
	font.Begin(rc);
	font.Draw(letter, { 400, 0 }, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	font.End(rc);
}