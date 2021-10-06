#pragma once
class SkinModelRender;
class RoboLean
{
private:
	int m_timer = 0;
	float m_leanDegree = 0.0f;
public:
	RoboLean() {};
	~RoboLean() {};
	void Lean(SkinModelRender* skinModel);
private:
};

