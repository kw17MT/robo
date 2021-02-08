#pragma once
class Unity : public IGameObject
{
private:
	Model model;
	ModelInitData data;
	
public:
	Unity();
	~Unity();

	bool Start();
	void Update();

	void Render(RenderContext& rc);
};

