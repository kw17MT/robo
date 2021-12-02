#pragma once
class SpriteRender;
class SkinModelRender;

class Title : public IGameObject
{
private:
	enum EnSpriteTypes
	{
		enTitle,
		enPlayButton,
		enHowToButton,
		enExitButton,
		enSpriteNum
	};
	Quaternion m_prevRot = Quaternion::Identity;
	Vector3 m_screenPos[4] = { Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
	float angleX = 0.0f;
	float angleY = 0.0f;
public:
	Title() {}
	~Title();
	bool Start();
	void Update();
public:
	SpriteRender* m_sprite[enSpriteNum] = { nullptr };
	SkinModelRender* m_robo = nullptr;
};

