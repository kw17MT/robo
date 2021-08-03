#pragma once
class SkinModelRender;
#include "SkinModelRender.h"

class Floor : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;				//床の位置
	Vector3 m_scale = Vector3::One;					//床の拡大率

	bool m_isRed = false;								//赤い床にするかどうか

public:
	~Floor();

	/**
	 * @brief モデルの初期化を行う
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 位置の更新を行う
	*/
	void Update();

	/**
	 * @brief 新しい位置を設定する。
	 * @param pos 新しい位置
	*/
	void SetPosition(const Vector3 pos) { m_position = pos; }

	/**
	 * @brief 赤い床にするか青い床にするか
	 * @param filePath モデルのファイルパス
	*/
	void ChangeModel(const char* filePath) { m_skinModelRender->ChangeModel(filePath); }

	/**
	 * @brief 赤い床かどうか設定する
	 * @param red TRUE＝赤　FALSE＝青
	*/
	void SetRed(const bool red) { m_isRed = red; }

	void SetScale(const Vector3 scale) { m_scale = scale; }

private:
	SkinModelRender* m_skinModelRender = nullptr;		//床用モデル
};

