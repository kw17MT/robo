#pragma once
#include "GuzaiGene.h"
#include "effect/Effect.h"


class Guzai;
class FontRender;
class PopUp2D;

class SkinModelRender;
class Kitchen;
class Effect;

enum EnItem
{
	enNonItem = -1,
	enBuffItem = 0,
	enDebuffItem = 1,
	enItemNum = 2,
};

class Player : public IGameObject
{
private:
	//プレイヤーのパラメータ
	Vector3 m_position = Vector3::Zero;								//座標
	Vector3 moveSpeed = Vector3::Zero;								//移動速度
	Vector3 saveMoveSpeed = Vector3::Zero;							//プレイヤーの移動速度を保存するベクトル
	Vector3 m_scale = Vector3::One;									//スケール
	Quaternion m_rotation = Quaternion::Identity;					//回転
	float angle = 0.0f;												//角度
	int playerNo = 0;												//１で左、２で右

	bool TargetingState = false;									//今、具材をターゲットしているか。1個以上ターゲティングしないように。
	int Distance = 0;

	//エフェクト関連
	int moveCounter01 = 0;											//P1
	int moveCounter02 = 0;											//P2
	
	bool m_moveStop01 = false;										//停止フラグ１
	bool m_moveStop02 = false;										//停止フラグ２

	const int m_maxNumCanSaveGuzaiType = 10;								//プレイヤーが保存できる具材種類の総数

public:
	Player() {};
	~Player();
	bool Start();
	

	//移動処理。
	void Update();

	//具材格納用配列を９で初期化
	void SetGuzaiNo9();

	//セッター
	void SetPlayerNo(int num) { playerNo = num; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetRotation(Quaternion rot) { m_rotation = rot; }
	void SetMoveSpeed(Vector3 speed) { moveSpeed = speed; }
	//現在ターゲティングしているかどうか
	void SetTarget(bool target) { TargetingState = target; }

	//ゲッタ―
	Vector3 GetPosition() { return m_position; }
	//現在のターゲティング状態を返す。
	bool GetTargetState() { return TargetingState; }
	//プレイヤーの正規化された移動速度を入手
	Vector3 GetNormalMoveSpeed() { return saveMoveSpeed; }
	int GetMaxNumCanSaveGuzaiType() { return m_maxNumCanSaveGuzaiType; }

	//移動不可を設定１
	void StopMove01(bool tf);
	//移動不可を設定２
	void StopMove02(bool tf);

	//移動範囲の制限を行う
	void RestrictPos();

	//一番最後にキッチンに積んだ具材の記録を消す
	void ClearLatestGuzaiNo(int num) { GuzaiNo[num] = 9; }

	//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。-1でキッチンからはとれる状態
	int have = 0;
	
	//積み上げている具材の種類を格納していく。
	int GuzaiNo[10];

	Kitchen* m_kitchen = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	Effect* m_effect01 = nullptr;
	Effect* m_effect02 = nullptr;
};


