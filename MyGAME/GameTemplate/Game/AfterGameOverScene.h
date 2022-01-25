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

	bool m_shouldStartNextScene = false;
	float m_alpha = 0.0f;

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

	/**
	 * @brief 上下ボタンで画面上ボタンを選択する関数
	*/
	void SelectButton();
private:
	SpriteRender* m_sprite[enSpriteNum] = { nullptr };				//画像インスタンス用配列
	Fade* m_fade = nullptr;											//フェードインスタンス

	SpriteRender* m_blackSprite = nullptr;
};

