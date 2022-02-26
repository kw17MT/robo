#pragma once

struct BarParam
{
	float barDeleteRate = 0.0f;							//主にゲージの画像に使用。画像をどのくらい消すかのレート（右端から消す）
	float scrollAmount = 0.0f;
};

struct WarningSignParam
{
	float alpha = 1.0f;									//透過率
};

class SpriteRender : public IGameObject
{
private:
	Sprite m_sprite;									//画像
	SpriteInitData m_spriteData;						//画像のデータ
	Vector3 m_pos = Vector3::Zero;						//画像の表示位置
	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };				//画像の初期拡大率
	Quaternion m_rot = Quaternion::Identity;			//画像の回転
	Vector2 m_pivot = { 0.5f,0.5f };					//画像の中心

	Texture m_texture;
	BarParam s_param;
	WarningSignParam s_signParam;
	SpriteInitData spdata;

	enum {
		enRenderNormal
	};

public:
	SpriteRender() {};

	/**
	 * @brief 画像の拡大率や位置、回転のの更新
	*/
	void Update();

	/**
	 * @brief 画像の初期化を行う
	 * @param name ファイルパスの名前
	 * @param width 横幅
	 * @param height 縦幅
	*/
	void Init(const char* name, int width, int height, const char* PixelShader = "PSMain");

	void InitGauge(const char* name, int width, int height);

	void SetSpriteSizeRate(float rate) { s_param.barDeleteRate = rate; }

	void SetSpriteAlpha(float addColorRate) { s_signParam.alpha = addColorRate; }

	/**
	 * @brief 新しい位置の設定
	 * @param pos 新しい位置
	*/
	void SetPosition(Vector3 pos) { m_pos = pos; }

	/**
	 * @brief 新しい拡大率の設定
	 * @param scale 新しい拡大率
	*/
	void SetScale(Vector3 scale) {m_scale = scale;}

	/**
	 * @brief 新しい回転の設定
	 * @param rot 新しい回転
	*/
	void SetRotation(Quaternion& rot) { m_rot = rot; }

	/**
	 * @brief 新しい中心の設定
	 * @param pivot 新しい中心
	*/
	void SetPivot(Vector2 pivot) { m_pivot = pivot; }

	/**
	 * @brief カラー変更(Vector4版)
	 * @param color 色
	*/
	void SetColor(const Vector4 color) { m_sprite.ColorUpdate(color); }

	/**
	 * @brief 画像のアルファを変更する
	 * @param alpha アルファ値
	*/
	void SetAlpha(const float alpha) {	m_sprite.SetAlpha(alpha);}
	/**
	 * @brief カラー変更(float版)
	 * @param r 赤色成分
	 * @param g 緑色成分
	 * @param b 青色成分
	 * @param a 透明度成分
	*/
	void SetColor(const float& r,const float& g,const float& b,const float& a) { m_sprite.ColorUpdate(r,g,b,a); }

	/**
	 * @brief 画像を描く
	 * @param rc レンダーターゲット
	*/
	void Render(RenderContext& rc) 
	{ 
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::GetInstance()->ui) {
			m_sprite.Draw(rc);
		}
		else {
			return;
		}
	}
};


