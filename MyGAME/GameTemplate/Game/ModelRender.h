#pragma once
class ModelRender : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;

public:
	ModelRender() {}

	void GiveData(const char* ModelPath, const char* ShaderPath);
	void GiveEx(const char& expandBuffer, const char* expandSize);
};