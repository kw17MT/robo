#pragma once
class SkinModelRender;

class LaunchPad : public IGameObject
{
private:
	enum EnPadParts
	{
		enTunnel,
		enWireAndTube,
		enWay,
		enPartsNum
	};

	Vector3 m_position = { 0.0f,0.0f,5000.0f };
	Vector3 m_scale = { 0.25f,0.25f,0.25f };

public:
	LaunchPad() {}
	~LaunchPad();
	bool Start();
	void Update();
private:
	SkinModelRender* m_model[3] = { nullptr };
};

