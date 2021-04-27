#pragma once
class Level;
class Dish;
class Guzai;

class DishGene : public IGameObject
{
private:
	int DishNum = 0;
public:

	DishGene() {};
	~DishGene();

	bool Start();

	void PopDishes();

	Guzai* m_Dish[37];
};

