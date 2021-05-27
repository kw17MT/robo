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
	int DishNum = 0;
	bool DishGeneState = true;
	
	static const int dishNum = 36;

	std::map<int, DishData> m_dishData;

	bool m_setSoundFlag = false;		//皿に具材が置かれるときに鳴る音
	bool m_fallingSoundFlag = false;	//具材が降って来るときに鳴る音
public:

	DishGene() {};
	~DishGene();

	bool Start();
	bool GetDishGeneState() { return DishGeneState; }
	//引数に皿の番号を入れることでその番号のポジションを返す。
	Vector3 GetDishPositionBasedNumber(int number);

	//皿に具材が置かれた音が鳴ったか？
	bool GetSetSound() { return m_setSoundFlag; }
	//具材が降ってきた音が鳴ったか？
	bool GetFallingSound() { return m_fallingSoundFlag; }
	//皿に具材が置かれた音が鳴ったかを設定。
	void SetSetSound(bool tf) { m_setSoundFlag = tf; }
	//具材が降ってきた音が鳴ったかを設定。
	void SetFallingSound(bool tf) { m_fallingSoundFlag = tf; }

	Dish* m_Dish[dishNum] = { nullptr };
};

