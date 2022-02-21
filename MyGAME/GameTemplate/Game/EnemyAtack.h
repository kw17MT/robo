#pragma once

/**
 * @brief 敵の攻撃を行うかどうか決定するクラス。
*/
class EnemyAtack
{
private:
	float counter = 0.0f;		//敵のマシンガン発射間隔
public:
	/**
	 * @brief マシンガンを発射可能かどうかを返す
	 * @return TRUE＝発射可能 FALSE＝不可
	*/
	bool Execute();
private:
};

