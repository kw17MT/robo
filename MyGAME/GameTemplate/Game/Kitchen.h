#pragma once
class Guzai;
class Burger;
class SkinModelRender;
class Player;
class CSoundSource;

class Kitchen : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_kitchenPos;
	Vector3 m_scale = Vector3::One;

	const int MaxStack = 5;									//キッチンにおける最大数
	int KitchenNo = 0;										//どっち側のキッチンか
	int stack = 0;											//いま何個具材を積んでいるか		
	bool canGrab = true;									//現在つかめるか（フレームのずれを起こす用）
	Guzai* StackedGuzai[5];									//キッチンの上にある具材

	bool m_soundFlag01 = false;								//1Pの音が出ているか？
	bool m_soundFlag02 = false;								//2Pの音が出ているか？

public:
	//キッチンのモデルなどの設定。
	bool Start();			

	void Update();

	//キッチン番号を設定する。
	void SetKitchenNo(int num) { KitchenNo = num;}

	//キッチンの座標を設定する。
	void SetKitchenPos(Vector3 pos) { m_kitchenPos = pos; m_position = pos; }

	//キッチンの座標を取得
	Vector3 GetKitchenPos() { return m_kitchenPos; }

	//キッチン上の具材を全消去
	void Delete();

	//スタックした層数をインクリメント
	void PlusStack() { stack++; }

	//スタックした層数をデクリメント
	void MinusStack() { stack--; }

	//数えていたStack変数を０に初期化。
	void SetStack0() { stack = 0; }

	//プレイヤー側に具材の種類を格納する際に使用する。
	int GetStackNum() { return stack; }

	//具材をハンバーガーに変換する。
	//消す時遅延を起こしてやらないとエラーを吐くためそれ対策
	void BornBurger();
	int Delay = 60;

	//具材を消す時に、プレイヤーに格納されている種類No.を９で初期化する。
	//この関数は具材を消す前に使うこと。
	void ClearNo();
	//キッチンのモデルを変更する
	void ChangeModel(const char* modelPath);
	//拡大率を変更する
	void SetScale(Vector3 scale) { m_scale = scale; }

	void ChangeGrabState(bool state) { canGrab = state; }

	//スタックしている具材を保存していく。
	void RegistStackedGuzai(Guzai* m_guzai) { StackedGuzai[stack - 1] = m_guzai; }

	Player* m_player[2];
	Burger* bur;
	SkinModelRender* m_skinModelRender;
	CSoundSource* m_soundSource;
};

