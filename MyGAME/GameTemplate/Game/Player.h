#pragma once
class SkinModelRender;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = Vector3::Zero;			//プレイヤーの現在の位置

	Vector3 m_prevHomePosition = Vector3::Zero;			//1フレーム前のホームポジションの位置
	Vector3 m_currentHomePosition = Vector3::Zero;				//カメラが追いかける位置座標

	bool m_isCapturing = false;

public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();

	Vector3 GetPosition()
	{
		//return m_currentPosition;
		return m_currentHomePosition;
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

