#pragma once
#include "SpriteRender.h"
#include "FontRender.h"

enum EnPlayerDamageTypes
{
	enEnemyBullet,		//弾丸によるダメージ
	enEnemyMissile,		//ミサイルによるダメージ
	enEnemyRaser		//レールガンによるダメージ
};

class PlayerHP : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -300.0f, 0.0f };			//固定位置

	float m_playerHp = 100.0f;								//プレイヤーの初期HP
	bool m_isPlayerAlive = true;							//プレイヤーは生きているか
public:
	/**
	 * @brief コンストラクタ
	*/
	PlayerHP() {}

	/**
	 * @brief デストラクタ
	*/
	~PlayerHP();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief プレイヤーが生きているかの確認関数
	 * @return TRUE生きている　FALSE倒されている
	*/
	bool GetIsPlayerAlive() { return m_isPlayerAlive; }

	/**
	 * @brief プレイヤーにダメージを適用する
	 * @param enemyDamageType ダメージの種類
	*/
	void ApplyDamage(EnPlayerDamageTypes enemyDamageType);

	/**
	 * @brief プレイヤーのHPを一気に減らす
	*/
	void SetHPZero() { m_playerHp = 0.0f; }
private:
	SpriteRender* m_spriteRender = nullptr;				//HP表示用画像
	FontRender* m_fontRender = nullptr;					//HP表示用文字
};

