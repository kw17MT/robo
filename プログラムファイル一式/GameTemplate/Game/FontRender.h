#pragma once

//#include "Font.h"

class FontRender : public IGameObject
{
private:
	void Render(RenderContext& rc);

	Font m_font;
	std::wstring m_text;					//!<テキスト。
	const wchar_t* m_textUnsafe = nullptr;	//!<アンセーフ版のテキスト。
	Vector2 m_position = Vector2::Zero;	//!<座標。x = 0.0f, y = 0.0fで画面の中心。
	Vector4 m_color = Vector4::White;		//!<カラー。
	float m_rotation = 0.0f;				//!<回転。
	float m_scale = 1.0f;					//!<拡大率。
	Vector2 m_pivot;						//!<ピボット。

public:
	/**
	 * @brief 位置を設定する
	 * @param pos 新しい位置
	*/
	void SetPosition(const Vector2& pos);

	/**
	 * @brief 新しい回転を設定する
	 * @param rotation 新しい回転
	*/
	void SetRotation(float rotation);

	/**
	 * @brief 新しい拡大率の設定
	 * @param scale 新しい拡大率
	*/
	void SetScale(float scale);

	/**
	 * @brief テキストの設定
	 * @param text テキスト
	*/
	void SetText(const wchar_t* text);

	/**
	 * @brief アルファ抜きの色の設定
	 * @param color 色
	*/
	void SetColor(const Vector3& color);

	/**
	 * @brief アルファ込みの色の設定
	 * @param color 色
	*/
	void SetColor(const Vector4& color);

	/**
	 * @brief 色の加算
	 * @param colorPoint 加算する色
	*/
	void AddColorPoint(Vector4 colorPoint);

	/**
	 * @brief テキストの色を返す
	 * @return テキストの色
	*/
	Vector4 GetColor() { return m_color; }

	/**
	 * @brief カラーの色合いの程度を返す
	 * @return 全部の色の平均値
	*/
	float GetColorVolume();

	/**
	 * @brief フォントのサイズを変更する
	 * @param size 加算する拡大率
	*/
	void  AddFontScale(float size);

	/**
	 * @brief 画像の中心を決める
	 * @param pivot 中心にしたいところ
	*/
	void SetPivot(const Vector2& pivot);
};

