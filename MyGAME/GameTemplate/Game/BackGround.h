#pragma once
class BackGround : public IGameObject
{
private:
	Model model;
public:
	BackGround() {};
	~BackGround() {};

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;
};

