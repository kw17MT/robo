#pragma once
class SkinModelRender;
class TrashCan : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale
public:
	SkinModelRender* m_skinModelRender;
};

