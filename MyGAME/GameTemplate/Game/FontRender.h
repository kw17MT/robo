#pragma once

class FontRender : public IGameObject
{
public:
	FontRender();
	void WriteFont(const wchar_t* letter, RenderContext& rc);
};

