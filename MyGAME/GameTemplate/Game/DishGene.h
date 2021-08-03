#pragma once
class Level;
class Dish;
class Guzai;

struct DishData {
	Vector3 s_dishPosition;
	int s_number;
};

class DishGene : public IGameObject
{
private:
	int m_popedDishNum = 0;						//皿を何個出しているか。		
	bool m_dishGeneState = true;				//皿生成器は稼働しているかFALSE＝全部の皿を出し終えた
	static const int m_dishMaxNum = 36;			//出す皿の最大数
	bool m_fallingSoundFlag = false;			//具材が降って来るときに鳴る音がでているか

	std::map<int, DishData> m_dishData;
public:
	DishGene() {};
	~DishGene();

	/**
	 * @brief 	作成したレベルを用いて、皿をポップさせる位置を決定後、出す。
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 現在皿の生成器は動いているか
	 * @return FALSE＝出し終えて止まっている、TRUE＝動いている
	*/
	bool GetDishGeneState() const { return m_dishGeneState; }
	
	/**
	 * @brief 引数に皿の番号を入れることでその番号のポジションを返す。
	 * @param number 皿の番号
	 * @return 皿の番号に対応した位置座標
	*/
	Vector3 GetDishPositionBasedNumber(int number);

	/**
	 * @brief 具材が降ってきた音が鳴ったか？
	 * @return 
	*/
	bool GetFallingSound() const { return m_fallingSoundFlag; }

	/**
	 * @brief 具材が降ってきた音が鳴ったかを設定
	 * @param playingSound 音が鳴ったかどうか
	*/
	void SetFallingSound(bool playingSound) { m_fallingSoundFlag = playingSound; }

private:
	std::array<Dish*, m_dishMaxNum> m_Dish = { nullptr };
};

