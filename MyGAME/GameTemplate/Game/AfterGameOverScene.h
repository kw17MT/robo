#pragma once
class SpriteRender;
class Fade;

/**
 * @brief ゲームオーバー演出後、リトライかタイトルに戻るかの案内をするシーン
*/
class AfterGameOverScene : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	enum EnSelectedSpriteTypes
	{
		enRePlayButton,
		enToTitleButton,
		enSpriteNum
	};

	EnSelectedSpriteTypes m_selectedSpriteType = enRePlayButton;
public:
	/**
	 * @brief コンストラクタ
	*/
	AfterGameOverScene() {}

	/**
	 * @brief デストラクタ
	*/
	~AfterGameOverScene();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();
private:
	SpriteRender* m_sprite[enSpriteNum] = { nullptr };				//画像インスタンス用配列
	Fade* m_fade = nullptr;											//フェードインスタンス
};

