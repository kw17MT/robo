#pragma once
class SkinModelRender;

class SkyCube : public IGameObject
{
public:
	/**
	 * @brief コンストラクタ
	*/
	SkyCube() {}

	/**
	 * @brief デストラクタ
	*/
	~SkyCube();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;			//空インスタンス
};		