#pragma once
class SpriteRender;

enum EnFadePhase
{
	enFadeIn,
	enFadeOut,
	enWait
};

class Fade : public IGameObject
{
private:
	EnFadePhase m_fadePhase = enFadeIn;
	float m_alpha = 0.0f;
public:
	Fade() {}
	~Fade();
	bool Start();
	void Update();

	EnFadePhase& GetFadePhase() { return m_fadePhase; }
	void SetFadeOut() { m_fadePhase = enFadeOut; }
private:
	SpriteRender* m_sprite = nullptr;
};

