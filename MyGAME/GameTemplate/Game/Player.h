#pragma once
class SkinModelRender;
class MachinGun;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = { 0.0f,400.0f,5000.0f };			//プレイヤーの現在の位置

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
		return m_currentHomePosition;
	}

	const bool GetIsCapturing() const 
	{
		return m_isCapturing;
	}
private:
	SkinModelRender* m_skinModelRender = nullptr;
	CameraMove m_cameraMove;
	PlayerMove m_roboMove;
	PlayerRotation m_roboRotation;

	CharacterController m_charaCon;

	std::vector<MachinGun*> m_machingun = { nullptr };
};

