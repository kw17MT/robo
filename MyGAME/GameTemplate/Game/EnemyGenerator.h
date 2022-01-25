#pragma once
class Enemy;
#include "EnemySpawnPoint.h"

class EnemyGenerator : public IGameObject
{
private:
	Vector3 m_firstEnemyPos = Vector3::Zero;			//一番最初に出す敵の位置
	bool m_shouldGenerateEnemy = false;
public:
	/**
	 * @brief コンストラクタ
	*/
	EnemyGenerator() {};

	/**
	 * @brief デストラクタ
	*/
	~EnemyGenerator();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 生成した敵の数を取得する
	 * @return 敵の数
	*/
	int GetEnemyNum() { return m_enemys.size() - 1; }

	/**
	 * @brief 敵の位置を取得
	 * @param no 配列の番号
	 * @return 敵の位置
	*/
	Vector3 GetEnemyPos(int no);

	/**
	 * @brief 指定配列番号の敵インスタンスは生きているか
	 * @param no 指定する要素番号
	 * @return TRUE＝生きている　FALSE＝死んでいる
	*/
	bool GetIsEnemyAlive(int no);

	/**
	 * @brief 敵インスタンスを保存する配列をさらにする
	*/
	void CleanUpArray();

	/**
	 * @brief 敵を生成する
	*/
	void GenerateEnemy();
private:
	std::vector<Enemy*> m_enemys = { nullptr };				//敵インスタンス
	EnemySpawnPoint m_spawnPoint;							//敵のスポーンポイントを決定する
};

