#pragma once
class SpriteRender;
class Level2D;
class Result;
#include "Level2D.h"

class MissCounter : public IGameObject
{
private:
	int m_pl1MissCount = 0;								//プレイヤー１のミス数				
	int m_pl2MissCount = 0;								//プレイヤー２のミス数

	bool m_pl1Win = false;								//プレイヤー1は勝ったか
	bool m_pl2Win = false;								//プレイヤー2は勝ったか
	bool m_shouldChangeFilePath = false;				//どれかの画像を変えるべきか
	bool m_isGameSet = false;							//ゲームが終了したか
public:
	~MissCounter();

	/**
	 * @brief レベルを用いた画像の配置
	 * @return true
	*/
	bool Start();

	/**
	 * @brief プレイヤーのミス数に応じて画像を変更する。WIN,LOSEの結果発表も
	*/
	void Update();

	/**
	 * @brief プレイヤー１のミス数を加算する
	*/
	void AddPl1MissCount() { m_pl1MissCount++; }
	
	/**
	 * @brief プレイヤー２のミス数を加算する
	*/
	void AddPl2MissCount() { m_pl2MissCount++; }

	/**
	 * @brief どれかの画像を変更するべきか
	 * @param state TRUE＝するべき
	*/
	void ChangeMarkState(bool state) { m_shouldChangeFilePath = state; }

private:
	Level2D m_level2D;		
	Result* m_result[2] = {nullptr};
	SpriteRender* m_spriteRender[6] = { nullptr };
};

