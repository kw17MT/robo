#pragma once
#include "effect/Effect.h"
class Counter;
class CSoundSourse;
class Effect;
class Player;
class SkinModelRender;
class TrashCan;

/**
 * @brief ハンバーガーのオブジェクト情報があるクラス
 * スコアの源
 * キッチンで生まれてカウンターで消える。
*/
class Burger : public IGameObject
{
private:
	const Vector3 m_burgerScale = { 10.0f, 10.0f, 10.0f };	//ハンバーガーの拡大率
	Vector3 m_position = Vector3::Zero;						//ハンバーガーの位置
	Vector3 m_beHadPos = Vector3::Zero;						//ハンバーガーが持たれた時の位置設定用
	int m_burgerNo = 0;										//どちらがつくったバーガーか。０が１P、１が２P
	int m_decrementTime = 20;								//ゴミ箱にセットした時に消えるまでの遅延時間
	int m_deleteTimer = 0;									//ボタン長押しでバーガーを消すようにするため。
	bool m_isSetOnTrashCan = false;							//ゴミ箱にセットされたか
	bool m_putOnCounter = false;							//キッチンにいるかどうか、キッチンの上に位置を合わせるための変数

public:
	/**
	 * @brief コストラクタ
	*/
	Burger() {}
	
	/**
	 * @briefデストラクタ
	*/
	~Burger();

	/**
	 * @brief ハンバーガーの移動処理、掴まれたかの判定処理。
	*/
	void Update();

	/**
	 * @brief モデルの初期化、必要なインスタンスの情報を探して取ってくる。
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief ハンバーガーを持つ処理
	*/
	void GrabBurger();

	/**
	 * @brief バーガーを消した際、カウンターに積んでいた具材の種類を初期化。
	 *プレイヤーに保存していた、ハンバーガーを構成する具材をで初期化。	
	*/
	void ClearNo();

	/**
	 * @brief ハンバーガーの座標のセッター
	 * @param pos 位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }
	
	/**
	 * @brief バーガーはどちら側のか
	 * @param num ０、左　１、右
	*/
	void SetBurgerNo(int num) { m_burgerNo = num; }

	/**
	 * @brief ゴミ箱にセットしてハンバーガーを消す。
	*/
	void SetOnTrashCan();

	/**
	 * @brief カウンターに置いたかどうかのセッター
	 * @param state カウンタ―に置いたらTRUE。
	*/
	void SetPutOnCounterState(bool isPut) { m_putOnCounter = isPut; }
	
	/**
	 * @brief プレイヤー状態
	*/
	enum EnPlayerState {
		enFullKitchen = -1,		//キッチンが一杯の状態でレーンの上からはとれないが、具材置き場かキッチン上からはとれるようにするのに必要
		enNothing,				//何も持っていない
		enHaveGuzai,			//具材を持っている状態
		enHaveBurger,			//ハンバーガーを持っている状態
		enStateNum,				//なりえる状態の数
	};
	
private:
	Counter* m_counter = nullptr;							//対応するカウンター
	SkinModelRender* m_skinModelRender = nullptr;			//モデルのレンダー
	Player* m_player = nullptr;								//対応するプレイヤー
	TrashCan* m_trashCan = nullptr;							//対応するゴミ箱
};