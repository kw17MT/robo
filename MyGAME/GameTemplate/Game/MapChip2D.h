#pragma once

struct CaslData;

class MapChip2D
{
private:
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale = g_vec3One;
	Sprite m_sprite;
public:
	void Init(CaslData* calsData);
	void Update()
	{
		m_sprite.Update(m_position, m_rotation, m_scale);
	}
	void Draw(RenderContext& renderContext)
	{
		m_sprite.Draw(renderContext);
	}
};
