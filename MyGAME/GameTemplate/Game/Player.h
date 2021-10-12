#pragma once
class SkinModelRender;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_prevPosition = Vector3::Zero;
	Vector3 m_currentPosition = Vector3::Zero;

	Vector3 m_prevFront = g_camera3D->GetForward();
	Vector3 m_currentFront = g_camera3D->GetForward();

	Vector3 m_front = { 0.0f,0.0f,1.0f };

	bool m_isCapturing = false;

public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();

	Vector3 GetPosition()
	{
		return m_currentPosition;
	}

	const bool GetIsCapturing() const 
	{
		return m_isCapturing;
	}
private:
	SkinModelRender* m_skinModelRender;
	CameraMove m_cameraMove;
	PlayerMove m_roboMove;
	PlayerRotation m_roboRotation;
};

