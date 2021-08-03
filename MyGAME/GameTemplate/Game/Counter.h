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
	std::array<std::array<bool, 5>, 4> m_spriteCompareFlagTrue = { false };								//要素5個の配列を4つ作る　＝　5列4行
	std::array<std::array<bool, 5>, 4> m_spriteCompareFlagFalse = { false };
	bool m_bonusPoint = false;							//ボーナスポイントのフラグ

	int m_spriteTime = 0;								//正誤画像を出す時間
	std::array<std::array<int, 5>, 4> m_guzaiJudge = { 2 };
	int m_menuNum = 3;									//表示できるメニューの種類。
	int m_counterNo = 0;								//カウンター番号　０が左、１が右
	int m_stackNum = 0;									//何段のハンバーガーを作ったのかの変数、スコアに使用する
	int m_delay = 0;									//遅延時間

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
	 * @brief 地点１と地点２の距離をはかる
	 * @param pos1 地点１
	 * @param pos2 地点２
	 * @return ２地点間の距離
	*/
	float CalcDistance(Vector3 pos1, Vector3 pos2);

	/**
	 * @brief カウンターに近いところでAボタン押下後、ハンバーガーの判定とスコアの増加をするかの判定
	*/
	void JudgeAndDelete();

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
	const EnHamBurger* GetShowHamBurgers() const 
	{
		return m_showHamBurgers;
	}

	/**
	 * @brief ボーナス点をあげるかどうか設定する。
	 * @param あげる＝TRUE、あげない＝FALSE
	*/
	void SetBonusPoint(bool giveBonus) { m_bonusPoint = giveBonus; }

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
	
	EnHamBurger m_showHamBurgers[enHamBurgerNum];					//表示しているハンバーガーの配列。
	Kitchen* m_kitchen = nullptr;						//対応するキッチン
	CLevel2D* m_level2d = nullptr;						//レベル２D
	Player* m_player = nullptr;							//対応するプレイヤー
	SkinModelRender* m_skinModelRender = nullptr;		//カウンターのモデル
	SpriteRender* m_spriteJudge = nullptr;				//MISSやCORRECTの文字表示用
	std::array<std::array<SpriteRender*, 5>, 4> m_spriteCompare = { nullptr };
};