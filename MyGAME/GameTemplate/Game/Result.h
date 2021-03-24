#pragma once
class SpriteRender;
class FixedUI;

class Result : public IGameObject
{
private:
	//各種パラメータ
	Vector2 m_position = Vector2::Zero;
	Vector2 m_pivot = Vector2::Zero;
	Vector2 m_scale;
	Quaternion m_rotation = Quaternion::Identity;

	//中央、左、右の画像表示位置用変数を用意
	//スプライトレンダーのSetPositionの引数がVector3なのでVector3型とする
	Vector3 center = Vector3::Zero;
	Vector3 left = Vector3::Zero;
	Vector3 right = Vector3::Zero;
	
	//画像の縦横解像度を定数で決めておく
	const int wideth = 256;
	const int height = 256;

	//タイムアップフラグ
	bool isTimeUp = false;
	//どのスプライトにするかを決めるための整数値
	int spriteNum;
	//どの位置に表示するかを決めるための整数値
	int positionNum;
	
	

public:

	//UI
	FixedUI* m_ui = nullptr;
	//スプライトレンダー
	SpriteRender* m_spriteRender = nullptr;

	//コンストラクタなど
	~Result();
	bool Start();
	void Update();

	//ゲッター各種
	Vector2 GetPos() { return m_position; }
	Vector2 GetPivot() { return m_pivot; }
	Vector2 GetScale() { return m_scale; }
	Quaternion GetRotation() { return m_rotation; }
	bool GetIsTimeUp() { return isTimeUp; }

	//セッター
	void SetPosition(const Vector2& pos) { m_position = pos; }
	void SetPivot(const Vector2& piv) { m_pivot = piv; }
	void SetScale(const Vector2& scl) { m_scale = scl; }
	void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	void SetIsTimeUp() { isTimeUp = true; }

	//どのスプライトにするか呼び出し元で決めてもらうための関数
	void SetSprite(const int& No) { spriteNum = No; }
	//どの位置に表示するか呼び出し元で決めてもらうための関数
	void SetSpritePos(const int& posNo) { positionNum = posNo; }
	
	//ddsの振り分け関数
	void DecideDDS();
	//スプライトの位置を決める関数
	void DecidePos();

	

};

