#pragma once
class SpriteRender;

/**
 * @brief フェードの段階
*/
enum EnFadePhase
{
	enFadeIn,
	enFadeOut,
	enWait
};

/**
 * @brief 各シーンの遷移間で組み込むフェードクラス
*/
class Fade : public IGameObject
{
private:
	EnFadePhase m_fadePhase = enFadeIn;
	float m_alpha = 0.0f;
public:
	/**
	 * @brief コンストラクタ
	*/
	Fade() {}

	/**
	 * @brief　デストラクタ
	*/
	~Fade();

	/**
	 * @brief インスタンス生成時一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 現在のフェードの段階を取得する
	 * @return 現在のフェードの段階
	*/
	EnFadePhase& GetFadePhase() { return m_fadePhase; }

	/**
	 * @brief フェードアウトするように設定する
	*/
	void SetFadeOut() { m_fadePhase = enFadeOut; }
private:
	SpriteRender* m_sprite = nullptr;
};

