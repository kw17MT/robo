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
	PopUp2D* m_popUp = nullptr;

	//プレイヤーのパラメータ
	//座標
	Vector3 m_position = Vector3::Zero;
	//ポップアップの座標
	Vector3 m_popUpPosition = Vector3::Zero;
	//回転
	Quaternion m_rotation = Quaternion::Identity;
	//スケール
	Vector3 m_scale = Vector3::One;
	Vector3 m_shadowScale = Vector3::One;
	//角度
	float angle = 0.0f;
	//移動速度
	Vector3 moveSpeed = Vector3::Zero;
	//プレイヤーの移動速度を保存するベクトル
	Vector3 saveMoveSpeed = Vector3::Zero;

	//１で左、２で右
	int playerNo = 0;

	////バフの効果が有効かどうか。
	//bool Buff = false;
	////バフの有効時間。
	//int BuffTime = 120;
	//今、具材をターゲットしているか。1個以上ターゲティングしないように。
	bool TargetingState = false;

	EnItem m_enItem = enNonItem;

	int Distance = 0;

	//エフェクト関連
	//P1
	int moveCounter01 = 0;
	Effect m_effect01;
	//P2
	int moveCounter02 = 0;
	Effect m_effect02;

	//停止フラグ１
	bool m_moveStop01 = false;
	//停止フラグ２
	bool m_moveStop02 = false;

public:
	Player();
	~Player();
	//モデル、当たり判定、具材No.を格納する配列を９で初期化
	//↑の処理をコンストラクタからStart関数に変更しました。
	bool Start();
	void SetPlayerNo(int num) { playerNo = num; }

	//移動処理。
	void Update();

	/*void SetBuffAffect(bool buff) { Buff = buff; }
	bool stateBuff() { return Buff; }*/

	//具材格納用配列を９で初期化
	void SetGuzaiNo9();

	//void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	//自身の持つ座標情報をキャラコンに渡して処理してもらうために変更しました。
	void SetPosition(Vector3 pos) { m_position.x = pos.x, m_position.y = pos.y, m_position.z = pos.z; }
	void SetRotation(Quaternion rot) { m_rotation = rot; }
	//キャラコンの座標 → プレイヤー自身の座標を取得するように変更
	Vector3 GetPosition() { return m_position; }

	//移動不可を設定１
	void StopMove01(bool tf);
	//移動不可を設定２
	void StopMove02(bool tf);
	
	void SetMoveSpeed(Vector3 speed) {
		moveSpeed.x = speed.x, moveSpeed.y = speed.y, moveSpeed.z = speed.z;
	}

	//移動範囲の制限を行う
	void RestrictPos();

	//現在ターゲティングしているかどうか
	void SetTarget(bool target) { TargetingState = target; }
	//現在のターゲティング状態を返す。
	bool GetTargetState() { return TargetingState; }

	void GrabFromKitchen();

	void ClearLatestGuzaiNo(int num) { GuzaiNo[num] = 9; }

	/*const EnItem GetEnItem() const
	{
		return m_enItem;
	}

	void SetEnItem(EnItem enItem)
	{
		if (m_enItem == enNonItem)
		{
			GuzaiGene* gg;
			if (playerNo == 1)
			{
				gg = FindGO<GuzaiGene>("gene01");
			}
			else if (playerNo == 2)
			{
				gg = FindGO<GuzaiGene>("gene02");
			}
			else {
				return;
			}
			if (m_enItem == enDebuffItem)
			{
				gg->DeBuffnum = 0;
			}
			else if (m_enItem == enBuffItem)
			{
				gg->Buffnum = 0;
			}
		}
		m_enItem = enItem;
	}*/

	void UseItem();

	//プレイヤーの正規化された移動速度を入手
	Vector3 GetNormalMoveSpeed() { return saveMoveSpeed; }

	//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。-1でキッチンからはとれる状態
	int have = 0;
	
	//積み上げている具材の種類を格納していく。
	int GuzaiNo[10];

	enum enAnimationCrips
	{
		enAnimation_Idle,
		enAnimation_Run,
		enAnimation_Cut,			//トマトを切るときつかうかも
		enAnimation_Cook,			//バーガーを作るとき使うかも
		enAnimation_HaveIdle,
		enAnimation_HaveRun,
		enAnimation_Num
	};

	AnimationClip animationClips[enAnimation_Num];

	Kitchen* m_kitchen = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	SkinModelRender* m_shadow = nullptr;
};


