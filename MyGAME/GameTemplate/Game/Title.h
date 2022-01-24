#pragma once
class SpriteRender;
class SkinModelRender;
#include "CameraShake.h"
class SoundSource;
class Fade;

class Title : public IGameObject
{
private:
	enum EnSpriteTypes
	{
		enTitleRogo,
		enPlayButton,
		enHowToButton,
		enExitButton,
		enSpriteNum
	};
	Quaternion m_prevRot = Quaternion::Identity;
	Vector3 m_screenPos[4] = { Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
	float angleX = 0.0f;
	float angleY = 0.0f;
	EnSpriteTypes m_enSelectedSpriteType = enPlayButton;
public:
	Title() {}
	~Title();
	bool Start();
	void Update();
public:
	SpriteRender* m_sprite[enSpriteNum] = { nullptr };
	SkinModelRender* m_robo = nullptr;
	SkinModelRender* m_bunker = nullptr;
	CameraShake m_cameraShake;
	CSoundSource* m_bgm = nullptr;
	Fade* m_fade = nullptr;
};

