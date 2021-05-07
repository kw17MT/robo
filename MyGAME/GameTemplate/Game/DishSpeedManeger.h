#pragma once
class DishSpeedManeger : public IGameObject
{
private:
	int countFrame = 0;
	int elapseTime = 0;						//経過時間、ゲームスタートからどのくらい時間がたっているか
	int speedUpTimeStart = 20;					//スピードアップさせたい時間、ランダムにしたい
	int speedUpTimeEnd = 25;						//スピードアップ時間の長さ、ランダムな数字にしたい

	bool isSpeedUp = false;
public:
	bool GetSpeedUpState() { return isSpeedUp; }

	void Update();

	static DishSpeedManeger& GetInstance()
	{
		static DishSpeedManeger instance;
		return instance;
	}
};

