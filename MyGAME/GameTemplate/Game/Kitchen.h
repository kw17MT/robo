#pragma once
class Guzai;
class Burger;
class SkinModelRender;
class Player;
class CSoundSource;
class Meter;
class Counter;

class Kitchen : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;						//キッチンの座標							
	Vector3 m_scale = { 0.3f,0.3f,0.3f };					//キッチンの拡大率
	Vector3 m_slidePos = Vector3::Zero;						//具材がハンバーガーになるときに一か所に集まるときのスピード
	
	int m_kitchenNo = 0;									//どっち側のキッチンか。０が左、１が右
	int m_stack = 0;											//いま何個具材を積んでいるか		
	int m_delay = 60;											//具材を消す時遅延を起こす用の変数
	const int m_maxStack = 5;									//キッチンにおける最大数

	float m_distance = 0.0f;

	bool m_soundFlag = false;								//音が出ているか？
	bool m_canGrab = true;									//現在つかめるか（フレームのずれを起こす用）
	bool m_isPlayerCookingOnKitchen = false;							//具材を消すことが決まっているかどうか

public:

	~Kitchen();

	/**
	 * @brief キッチンモデルの初期化や、必要オブジェクトのデータ取得。
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ハンバーガーを生み出すかの判定や、キッチン上から具材を取るかの判定を毎フレーム行う
	*/
	void Update();

	/**
	 * @brief キッチン番号を設定する。
	 * @param num キッチンの番号　０＝左、１＝右
	*/
	void SetKitchenNo(int num) { m_kitchenNo = num; }

	/**
	 * @brief キッチンの座標を設定する。
	 * @param pos 新しいキッチンの座標
	*/
	void SetKitchenPos(Vector3 pos) { m_position = pos; }

	/**
	 * @brief キッチンの座標を取得
	 * @return 現在のキッチンの座標
	*/
	Vector3 GetKitchenPos() const { return m_position; }

	/**
	 * @brief キッチン上の具材を全部消去、ハンバーガーを生み出す時に使用する
	*/
	void Delete();

	/**
	 * @brief キッチン上にある具材の数をインクリメント
	*/
	void PlusStack() { m_stack++; }

	/**
	 * @brief スタックした層数をデクリメント
	*/
	void MinusStack() { m_stack--; }

	/**
	 * @brief キッチン上には具材が存在しないことを設定する
	*/
	void SetStack0() { m_stack = 0; }

	/**
	 * @brief 現在何段具材が積まれているかを伝える関数
	 * @return 現在、積まれている具材の数
	*/
	int GetStackNum() const { return m_stack; }

	/**
	 * @brief 具材をハンバーガーに加工する。
	*/
	void BornBurger();

	/**
	 * @brief 具材を消す時に、プレイヤーに格納されている種類No.を９で初期化する。
	 * この関数は具材を消す前に使うこと。
	*/
	void ClearNo();

	/**
	 * @brief キッチンの拡大率を変更する。
	 * @param scale 変更したい拡大率
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }

	/**
	 * @brief そのフレーム中にキッチンから具材が取られたか
	 * @param state TRUE＝取られた。
	*/
	void ChangeGrabState(bool state) { m_canGrab = state; }

	/**
	 * @brief キッチン上にある具材の登録
	 * ここの具材は複製する必要がなく、パラメーターをいじりたいのでconst不要
	 * Delete関数で使用する。
	*/
	void RegistStackedGuzai(Guzai* m_guzai) { m_stackedGuzai[m_stack - 1] = m_guzai; }

	/**
	 * @brief プレイヤーはキッチンでハンバーガーを作っている途中か
	 * @return TRUE＝作成中。キッチン上の具材をとれなくする。
	*/
	bool GetIsPlayerCookingOnKitchen() const { return m_isPlayerCookingOnKitchen; }

	/**
	 * @brief キッチンの状にある一番後に置かれた具材をプレイヤーに持たせる。
	*/
	void TakePlayerLastestGuzai();

private:
	Player* m_player = nullptr;
	Burger* bur = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	CSoundSource* m_soundSource = nullptr;
	Meter* m_meter = nullptr;
	Counter* m_counter = nullptr;
	std::array<Guzai*, 5> m_stackedGuzai = { nullptr };					//キッチンの上にある具材
};

