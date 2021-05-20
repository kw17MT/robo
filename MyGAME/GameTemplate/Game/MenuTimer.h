#pragma once
class SkinModelRender;

class MenuTimer : public IGameObject
{
private:
	Vector3 m_position;
	Vector3 m_scale = { 2.8f,1.0f,1.0f };
	Quaternion m_rot = Quaternion::Identity;

	Light m_lig;

	//int timer = 1000;
	int timer = 100;
	bool isTimeUp = false;
public:
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetRotation(Quaternion rot) { m_rot = rot; }
	bool GetTimeUpState() { return isTimeUp; }
	void SetTimeUpState(bool state) { isTimeUp = state; }
	void ResetTimerParam();

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
};

