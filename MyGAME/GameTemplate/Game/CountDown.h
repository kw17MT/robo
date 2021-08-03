#pragma once
class SpriteRender;

class CountDown : public IGameObject
{
private:
	Vector4 m_color = { 1.0f,1.0f,1.0f,0.0f };					//画像のカラー
	Vector3 m_position = { 0.0f,100.0f,0.0f };					//画像の位置
	Vector3 m_scale = { 2.0f,2.0f,2.0f };						//画像の初期拡大率
	Vector2 m_pivot = { 0.5f,0.5f };							//画像の中心
	bool m_soundFlag = false;									//現在なんらかの音はなっているか（カウントダウン関係）
	bool m_isFadeIn = true;										//フェードアウトを開始するか
	int m_changeCount = 1;										//カウントダウンの
	float m_alpha = 0.0f;										//
	float m_timer = 4.0f;										//カウントダウン用。
public:
	bool Start();
	void Update();
private:
	SpriteRender* m_spriteRender = nullptr;
};

