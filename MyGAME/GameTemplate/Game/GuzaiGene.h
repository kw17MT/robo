#pragma once
class Guzai;
class DishGene;
class SkinModelRender;

#include <vector>

class GuzaiGene : public IGameObject
{
private:
	static const int m_guzaiNum = 36 + 20;

	int m_emptyDishNumber[6];						//空の皿の番号を保存しておく。
	int m_emptyDishCounter = 0;						//いくつからの皿があるか。
	const int m_lostNumber = 5;						//補充を開始する空の皿の数
	bool m_guzaiFlag[99] = { false };				//具材が出ているかどうかのフラグ。

	Vector3 m_setPos = Vector3::Zero;
public:
	GuzaiGene() {};
	~GuzaiGene();
	bool Start();
	void Update();

	/**
	 * @brief 具材が格納されている配列に引数で特定して、それが存在しているかどうかを返す
	 * @param number 配列の要素番号
	 * @return 存在している＝TRUE。
	*/
	bool GetGuzaiFlag(int number) { return m_guzaiFlag[number]; }

	/**
	 * @brief その引数の要素には具材のデータが格納されているか設定する。
	 * @param number 要素の番号
	 * @param tf 存在する＝TRUE。
	*/
	void SetGuzaiFlag(int number, bool tf) { m_guzaiFlag[number] = tf; }

	Guzai* m_guzai[99] = {nullptr};
};
