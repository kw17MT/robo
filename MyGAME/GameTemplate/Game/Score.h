#pragma once
class FixedUI;
class FontRender;
class CSoundSource;

class Score : public IGameObject
{
private:
	Vector2 m_score00Pos = { -400.0f,-250.0f };					//1P用の位置
	Vector2 m_score01Pos = { 450.0f,-250.0f };					//2P用の位置

	int m_score00 = 0;											//最終的に表示するスコア　＝　具材の数 * 100
	int m_score01 = 0;											//最終的に表示するスコア　＝　具材の数 * 100
	int m_basePoint00 = 0;										//カウンター側で何層のハンバーガーを提供したか。
	int m_basePoint01 = 0;										//提供した具材を数えておく。
	
	bool m_isTimeUp = false;									//タイムアップフラグ
	bool m_isChangeColor00 = false;								//文字の色を変えるべきかそうでないか
	bool m_isChangeColor01 = false;								//文字の色を変えるべきかそうでないか
			
	std::wstring m_nowScore00 = L"0";							//プレイヤー1の現在のスコア
	std::wstring m_prevScore00 = L"0";							//プレイヤー1の前のスコア
	std::wstring m_nowScore01 = L"0";							//プレイヤー2の現在のスコア
	std::wstring m_prevScore01 = L"0";							//プレイヤー2の前のスコア

public:
	//勝敗の状態
	//プレイヤー1

	/**
	 * @brief プレイヤー1の勝敗の状態
	*/
	enum EnResultP1 {
		enResultP1_draw,	//0
		enResultP1_win,		//1
		enResultP1_lose,	//2
		enResultP1Num		//ダミー(3)
	}ResultP1;

	/**
	 * @brief プレイヤー2の勝敗の状態
	*/
	enum EnResultP2 {
		enResultP2_draw,	//0
		enResultP2_win,		//1
		enResultP2_lose,	//2
		enResultP2Num		//ダミー(3)
	}ResultP2;

	~Score();

	/**
	 * @brief スコアのテキストデータの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 何層のハンバーガーを提出できたか
	 * @param num ハンバーガーの層
	*/
	void SetBasePoint01(int num) { m_basePoint00 += num; }

	/**
	 * @brief 何層のハンバーガーを提出できたか
	 * @param num ハンバーガーの層
	*/
	void SetBasePoint02(int num) { m_basePoint01 += num; }

	/**
	 * @brief タイムアップ時の結果を記録
	*/
	void SetResult();

	/**
	 * @brief プレイヤー1の点数を返す
	 * @return プレイヤー1の点数
	*/
	int GetPlayer00Score() const { return m_score00; }

	/**
	 * @brief プレイヤー2の点数を返す
	 * @return プレイヤー2の点数
	*/
	int GetPlayer01Score() const { return m_score01; }

	/**
	 * @brief タイムアップフラグをtrueにする
	*/
	void SetIsTimeUp() { m_isTimeUp = true; }

	/**
	 * @brief スコアの計算を行ったあと、加算されていたら数字の色を強調表示のために変える
	*/
	void AddScoreNChangeColor();

	/**
	 * @brief タイムアップになったら得点次第でなんの画像を出すか決める
	*/
	void Update();
private:
	
	FixedUI* m_ui = nullptr;								//ゲームがタイムアップしているかどうか知るため
	FontRender* m_score[2] = { nullptr, nullptr };			//スコアの表示用
	CSoundSource* m_moneySound = nullptr;					//キャッシャーの音用
};

