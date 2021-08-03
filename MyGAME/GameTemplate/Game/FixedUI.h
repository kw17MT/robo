#pragma once
class FontRender;
class SpriteRender;
class SoundSource;
class PlayerGene;
class PathMove;

class FixedUI : public IGameObject
{
private:
	float m_remainingTime = 130.0f;							//表示する残り時間
	int m_timer = 0;									//フレームカウント用、60になったらLastTimeを１減らす。
	bool m_isTimeUp = false;							//タイムアップフラグ
public:
	~FixedUI();

	/**
	 * @brief 表示する画像の初期化
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief 残り時間や文字の表示
	*/
	void Update();

	/**
	 * @brief タイムアップ状態を取得
	 * @return TRUE＝タイムアップしている　FALSE＝してない
	*/
	bool GetIsTimeUp() const { return m_isTimeUp; }

	/**
	 * @brief 残り時間を変えす
	 * @return 残り時間
	*/
	int GetTime() const { return m_remainingTime; }

	/**
	 * @brief 残時間が少ないときの色の変化
	*/
	void RemainingTimeColor();

private:
	FontRender* m_textTime = nullptr;								//時間の文字部分
	FontRender* m_textScore[2] = { nullptr, nullptr };				//スコアの文字部分
	FontRender* m_textMiss[2] = { nullptr, nullptr };				//ミスの文字部分
	FontRender* m_time = nullptr;									//時間の数字部分
	CSoundSource* m_timeSound = nullptr;							//残り時間わずかの時に出す音用
};


