#pragma once
#include "HamBurgerFactory.h"
#include "CLevel2D.h"
class SkinModelRender;
class PlayerGene;
class SpriteRender;
class Burger;
class CSoundSource;
class Player;
class Kitchen;

class Counter : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;					//カウンターの位置座標
	Vector3 m_burgerPos = Vector3::Zero;				//バーガーの位置設定用の座標
	Vector3 m_spritePos = { 500.0f,-200.0f,0.0f };		//ハンバーガーの正誤表示をだす時の位置座標

	bool m_burgerCorrectFlag = false;					//提出されたバーガーが合っているフラグ。
	bool m_burgerMistakeFlag = false;					//提出されたバーガーが間違っているフラグ。
	bool m_spriteFlag = false;							//正誤画像が出ているか
	bool m_spriteCompareFlagTrue[4][5] = { false };		//メニューと一致しているときの画像表示フラグ
	bool m_spriteCompareFlagFalse[4][5] = { false };	//メニューと不一致のときの画像表示フラグ
	bool m_bonusPoint = false;							//ボーナスポイントのフラグ

	int m_spriteTime = 0;								//正誤画像を出す時間
	int m_guzaiJudge[4][5] = { 2 };						//積まれた具材が合っているか？0,違っている。1,合っている。2,積まれていない。
	int m_menuNum = 3;									//表示できるメニューの種類。
	int m_counterNo = 0;									//カウンター番号　０が左、１が右
	int m_stackNum = 0;									//何段のハンバーガーを作ったのかの変数、スコアに使用する
	int m_delay = 0;										//遅延時間

public:
	Counter() {};
	~Counter();

	/**
	 * @brief モデルの初期化、ハンバーガーの情報を取得、必要なオブジェクトの捜索と取得
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief ハンバーガーをどうするかの選定
	*/
	void Update();

	/**
	 * @brief カウンターに近いところでAボタン押下後、ハンバーガーの判定とスコアの増加をするかの判定
	*/
	void Delete();

	/**
	 * @brief カウンターの座標設定
	 * @param pos 新しい座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief カウンターのナンバリングを行う
	 * @param num カウンターのナンバー　０が左、１が右
	*/
	void SetCounterNo(int num) { m_counterNo = num; }

	/**
	 * @brief 何段のハンバーガーをを作ったのかを記録を行う
	 * @param num ハンバーガーの層数
	*/
	void SetStackNum(int num) { m_stackNum = num; }

	/**
	 * @brief 記録していたハンバーガーの層数を０で初期化する。
	*/
	void SetStack0() { m_stackNum = 0; }

	/**
	 * @brief 正しいハンバーガーか調べる。
	 * @return 正しい＝TRUE、間違い＝FALSE
	*/
	bool Judge();

	/**
	 * @brief 現在キッチンで積んでいる具材とメニューの具材をリアルタイムで判定し、違っていたらバツマーク、あっていたらチェックマークをメニュー上に出す。
	*/
	void HamBurgerCompare();

	/**
	 * @brief 表示している3つのハンバーガーを取得。
	 * @return 
	*/
	EnHamBurger* GetShowHamBurgers()
	{
		return m_showHamBurgers;
	}

	/**
	 * @brief ボーナス点をあげるかどうか設定する。
	 * @param tf あげる＝TRUE、あげない＝FALSE
	*/
	void SetBonusPoint(bool tf) { m_bonusPoint = tf; }

	/**
	 * @brief プレイヤーの状態
	*/
	enum enPlayerState {
		enFullKitchen = -1,		//キッチンが一杯の状態でレーンの上からはとれないが、具材置き場かキッチン上からはとれるようにするのに必要
		enNothing,				//何も持っていない
		enHaveGuzai,			//具材を持っている状態
		enHaveBurger,			//ハンバーガーを持っている状態
		enStateNum,				//なりえる状態の数
	};
	
	EnHamBurger m_showHamBurgers[11];					//表示しているハンバーガーの配列。
	Kitchen* m_kitchen = nullptr;
	CLevel2D* m_level2d = nullptr;
	Player* m_player = nullptr;
	PlayerGene* m_playerGene = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	SpriteRender* m_spriteJudge = nullptr;
	SpriteRender* m_spriteCompare[4][5] = {nullptr};	//メニューと一致しているかの画像を表示
};