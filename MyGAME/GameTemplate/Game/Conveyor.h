#pragma once
class SkinModelRender;

class Conveyor : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;									//コンベアの位置座標
	const Vector3 m_scale = { 0.8f,1.0f,1.0f };							//コンベアの拡大率調整
public:

	/**
	 * @brief コンベアの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 位置の固定を行う
	*/
	void Update();

	/**
	 * @brief コンベアの位置を設定する
	 * @param pos 新しい位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

private:
	SkinModelRender* m_skinModelRender = nullptr;		//コンベアのモデル
};	

