#pragma once
class SkinModelRender;
#include "RoboLean.h"
#include "CameraMove.h"
#include "RoboMove.h"
#include "RoboRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_prevPosition = Vector3::Zero;
	Vector3 m_currentPosition = Vector3::Zero;

	Vector3 m_prevFront = g_camera3D->GetForward();
	Vector3 m_currentFront = g_camera3D->GetForward();

	Vector3 m_front = { 0.0f,0.0f,1.0f };

public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender;
	CameraMove m_cameraMove;
	RoboMove m_roboMove;
	RoboRotation m_roboRotation;
};

