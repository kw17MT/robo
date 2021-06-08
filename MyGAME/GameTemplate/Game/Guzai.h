#pragma once
#include "effect/Effect.h"
class CSoundSource;
class Effect;
class GuzaiGene;
class GuzaiOkiba;
class Kitchen;
class Meter;
class Player;
class PlayerGene;
class SkinModelRender;
class TrashCan;

class Guzai : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;					//具材の座標
	Vector3 m_scale = Vector3::One;						//具材の拡大率
	const Vector3 m_TargetedScale = { 1.3f,1.3f,1.3f }; //ターゲットされた時の拡大率

	Quaternion m_rotation = Quaternion::Identity;		//具材の回転
	
	int m_typeNo = 9;									//０．チーズ　１．エッグ　２．レタス　３．パテ　４．トマト　５．オニオン　６．ベーコン
	int m_decrementTime = m_holdTime;					//HOLDTIMEが待ち時間。DECREMENTTIMEが徐々に減らしていく役割がある。
	int m_whichPlayerGet = 0;							//その具材はどちらのプレイヤーにつかまれたか
	int m_whichPlayerTargetMe = 0;						//どちらのプレイヤーにターゲットされたか
	int m_setKitchenNum = 9;							//自身がセットされた具材置き場の番号
	int m_hold01 = 0;									//1Pの調理中の時間。
	int m_hold02 = 0;									//2Pの調理中の時間。
	int m_myNo = 100;									//自分の番号
	const int m_cookingTime = 60;						//調理にかかる時間。
	const int m_holdTime = 10;							//一度ターゲッティングしたものをある程度見つめるための時間。
	const int m_targetRangeNear = 100;					//ターゲットする最短距離。
	const int m_targetRangeFar = 110;					//ターゲッティングを切り始める距離。（実際はディレイがあるため消えるのはもうちょっと先になる）

	float m_guzai2Pl00 = 0.0f;							//具材からプレイヤーへの距離
	float m_guzai2Pl01 = 0.0f;
	float m_kit2Pl00 = 0.0f;							//キッチンからプレイヤーへの距離
	float m_kit2Pl01 = 0.0f;
	float m_angle = 0.0f;								//回転の際の角度

	bool m_targeted = false;							//ターゲティングされた具材を設定するよう。
	bool m_guzaiOkibaSet = false;						//自身が具材置き場にセットされているか？
	bool m_isCooked = false;							//自身が調理されているか？
	bool m_soundFlag01 = false;							//1Pの音を鳴らしたかのフラグ
	bool m_soundFlag02 = false;							//2Pの音を鳴らしたかのフラグ
	bool m_isSetOnTrashCan = false;						//ゴミ箱に置いたのかどうか
	bool m_canPutOnGuzaiOkiba = true;					//具材置き場における状態であるか
	bool m_isHad = false;								//１ならば持たれている。
	bool m_returnedFromKitchen = false;					//一度キッチンに置かれてから、また取られたとき
	bool m_isPutOnKitchen = false;						//TRUEならばもうキッチンに置かれている。

	const char* m_nowModelPath;							//その具材は何であるかのメモ用、ターゲティングのオブジェクトを作成時に使用。

public:	
	Guzai() {};
	~Guzai();
	bool Start();
	void Update();
	/**
	 * @brief ポジションのゲッタ―
	 * @return 具材の位置座標
	*/
	Vector3 GetPosition() { return m_position; }
	/**
	 * @brief 自分の具材ナンバーのゲッタ―
	 * @return 具材種類の番号
	*/
	int GetTypeNo() { return m_typeNo; }
	/**
	 * @brief 自分に設定された番号を入手
	 * @return マイナンバーの取得
	*/
	int GetGuziNo() { return m_myNo; }		
	/**
	 * @brief 自分が持たれたかどうかを返す。
	 * @return 持たれているかどうか。
	*/
	bool GetisHadState() { return m_isHad; }							
	/**
	 * @brief 位置座標を設定する。
	 * @param pos 位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }
	/**
	 * @brief 拡大率を設定する。
	 * @param scale 拡大率
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }
	/**
	 * @brief どちらのプレイヤーに持たれているかを設定する。
	 * @param num プレイヤー番号
	*/
	void SetWhichPlayerGet(int num) { m_whichPlayerGet = num; }
	/**
	 * @brief 自分の番号を設定する。
	 * @param number マイナンバー
	*/
	void SetGuzaiNo(int number) { m_myNo = number; }					
	/**
	 * @brief キッチンに置かれているかどうかを設定する。
	 * @param state キッチンに置かれている＝TRUE。
	*/
	void SetPutState(bool state) { m_isPutOnKitchen = state; }			//キッチンに置かれているかの変数の変更
	/**
	 * @brief プレイヤーに持たれているかどうかを設定する。
	 * @param state プレイヤーに持たれている＝TRUE。
	*/
	void SetisHadState(bool state) { m_isHad = state; }
	/**
	 * @brief キッチンから帰ってきた具材かどうかを設定する。
	 * @param state キッチンから帰ってきた＝TRUE。
	*/
	void SetReturnedState(bool state) { m_returnedFromKitchen = state; }
	/**
	 * @brief キッチンに置いたときに違うモデルに差し替える
	 * @param guzaiType 具材の種類の番号
	*/
	void ChangeModel(int& guzaiType);
	/**
	 * @brief 具材を持ったり置いたりする処理
	*/
	void GrabNPut();
	/**
	 * @brief 一定範囲内で一番近い具材をターゲットする
	*/
	void Targeting();
	/**
	 * @brief 具材置き場に置くときの処理
	*/
	void SetGuzaiOkiba();
	/**
	 * @brief 具材置き場から取り出す処理。
	*/
	void AwayFromGuzaiOkiba();
	/**
	 * @brief 具材置き場で調理する処理。
	*/
	void Cooking();
	/**
	 * @brief ゴミ箱の上に具材をセット後、消す。
	*/
	void SetOnTrashCan();
	/**
	 * @brief 具材の回転処理。
	*/
	void Rotation();

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

	/**
	 * @brief 具材の種類リスト
	*/
	enum enGuzaiType {
		enCheese,			//０．チーズ
		enEgg,				//１．エッグ
		enLettuce,			//２．レタス　
		enPatty,			//３．パテ　
		enTomato,			//４．トマト
		enOnion,			//５．オニオン 
		enBacon,			//６．ベーコン　
		enGuzaiTypeNum		//具材の種類数
	};

	Player* m_player00 = nullptr;
	Player* m_player01 = nullptr;
	Kitchen* m_kitchen00 = nullptr;
	Kitchen* m_kitchen01 = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	PlayerGene* m_playerGene = nullptr;
	GuzaiOkiba* m_guzaiOkiba = nullptr;
	TrashCan* m_trashCan[2] = { nullptr, nullptr };
	CSoundSource* m_cookingSe = nullptr;
	Meter* m_meter = nullptr;
};

