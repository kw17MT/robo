#pragma once
#include "effect/Effect.h"
class Guzai;
class FontRender;
class PopUp2D;
class SkinModelRender;
class Kitchen;
class Effect;

class Player : public IGameObject
{
private:
	//プレイヤーのパラメータ
	Vector3 m_position = Vector3::Zero;								//プレイヤーの座標
	Vector3 m_moveSpeed = Vector3::Zero;							//プレイヤーの移動速度
	Vector3 m_saveMoveSpeed = Vector3::Zero;						//プレイヤーの移動速度を保存するベクトル
	Vector3 m_scale = { 0.3f,0.4f,0.3f };							//プレイヤーのスケール
	Quaternion m_rotation = Quaternion::Identity;					//プレイヤーの回転

	float m_angle = 0.0f;											//プレイヤーの角度

	const int m_maxNumCanSaveGuzaiType = 10;						//プレイヤーが保存できる具材種類の総数
	int m_playerNo = 0;												//０で左、１で右
	int m_have = enNothing;											//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。-1でキッチンからはとれる状態
	int m_guzaiNo[10] = {9,9,9,9,9,9,9,9,9,9};			//積み上げている具材の種類を格納していく。
	int m_moveCounter = 0;											//プレイヤーが歩いたときに生じるエフェクトのタイマー
	
	bool m_targetingState = false;									//今、具材をターゲットしているか。1個以上ターゲティングしないように。
	bool m_moveStop = false;										//停止フラグ１

public:
	Player() {};
	~Player();

	/**
	 * @brief モデル、歩く時のエフェクトの初期化
	 * @return true
	*/
	bool Start();
	
	/**
	 * @brief 主に移動の処理
	*/
	void Update();

	/**
	 * @brief プレイヤー内の具材格納用配列を９で初期化
	*/
	void SetGuzaiEmpty();

	/**
	 * @brief プレイヤーの番号を設定
	 * @param num ０＝左　１＝右
	*/
	void SetPlayerNo(int num) { m_playerNo = num; }

	/**
	 * @brief 新しい位置の設定
	 * @param pos 新しい位置
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief 新しい回転の設定
	 * @param rot 新しい回転
	*/
	void SetRotation(Quaternion rot) { m_rotation = rot; }

	/**
	 * @brief 新しい移動速度の設定
	 * @param speed 新しい移動速度
	*/
	void SetMoveSpeed(Vector3 speed) { m_moveSpeed = speed; }

	/**
	 * @brief 自分は何かをターゲットしているか
	 * @param target TRUE＝している
	*/
	void SetTarget(bool target) { m_targetingState = target; }

	/**
	 * @brief プレイヤーの状態を設定する
	 * @param state 状態の番号。列挙型enPlayerStateを参照
	*/
	void SetPlayerState(int state) { m_have = state; };

	void SetPlayerStackedGuzais(int num, int guzaiType) { m_guzaiNo[num] = guzaiType; }

	/**
	 * @brief 現在の位置を返す
	 * @return 現在の位置
	*/
	Vector3 GetPosition() { return m_position; }
	
	/**
	 * @brief 現在のターゲティング状態を返す。
	 * @return ターゲティング状態　TRUE＝している
	*/
	bool GetTargetState() { return m_targetingState; }

	/**
	 * @brief プレイヤーの正規化された移動速度を入手
	 * @return プレイヤーの正規化された移動速度
	*/
	Vector3 GetNormalMoveSpeed() { return m_saveMoveSpeed; }

	/**
	 * @brief プレイヤーが保存できる具材の種類の総数
	 * @return 今のところ10個
	*/
	int GetMaxNumCanSaveGuzaiType() { return m_maxNumCanSaveGuzaiType; }

	/**
	 * @brief プレイヤーの状態を返す。
	 * @return enPlayerStateを参照
	*/
	int GetPlayerState() { return m_have; }

	/**
	 * @brief その段数には何の具材を積んでいるのかを返す
	 * @param num 積んでいる具材の段数の場所（0~4)
	 * @return true
	*/
	int GetPlayerStackedGuzais(int num) { return m_guzaiNo[num]; }

	/**
	 * @brief 積み上げている具材すべての情報を渡す
	 * @return  積み上げている具材すべての情報
	*/
	int* GetPlayerStackedGuzais() { return m_guzaiNo; }

	/**
	 * @brief 移動不可を設定
	 * @param tf TRUE＝移動できない　FALSE＝移動できる
	*/
	void StopMove(bool tf) { m_moveStop = tf; }

	/**
	 * @brief 座標による移動範囲の制限を行う
	*/
	void RestrictPos();
	
	/**
	 * @brief 一番最後にキッチンに積んだ具材の記録を消す
	 * @param num 消したい箇所の要素番号
	*/
	void ClearSpecificGuzaiNo(int num) { m_guzaiNo[num] = 9; }

	/**
	 * @brief プレイヤーの移動に伴う回転設定
	*/
	void PlayerRotation();

	/**
	 * @brief 調理中など、プレイヤーを移動を制限させるかどうかの判定を行う
	*/
	void RestrictMove();

	/**
	 * @brief 移動している時に生じさせるエフェクトを出す
	*/
	void PopWalkingEffect();

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

private:
	Kitchen* m_kitchen = nullptr;						//キッチンの情報取得用
	SkinModelRender* m_skinModelRender = nullptr;		//プレイヤーのモデル
	Effect* m_effect = nullptr;							//エフェクト
};


