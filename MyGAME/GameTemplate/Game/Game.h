#pragma once
class EnemyGenerator;
class Player;
class SkyCube;
class Ground;
class UI;
class Sun;
class Rader;
class LaunchPad;

class Game : public IGameObject
{
private:
	bool m_isCreateAllObject = false;
public:
	/**
	 * @brief コンストラクタ
	*/
	Game() {};

	/**
	 * @brief デストラクタ
	*/
	~Game();

	/**
	 * @brief オブジェクトが生成された一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれ関数
	*/
	void Update();
private:
	Player* m_player = nullptr;						//プレイヤーインスタンス
	EnemyGenerator* m_enemyGenerator = nullptr;		//敵生成器インスタンス
	SkyCube* m_sky = nullptr;						//スカイキューブマップインスタンス
	Ground* m_ground = nullptr;						//地面インスタンス
	UI* m_ui = nullptr;								//UIインスタンス
	Sun* m_sun = nullptr;							//太陽インスタンス
	Rader* m_rader = nullptr;						//レーダーインスタンス
	LaunchPad* m_launchPad = nullptr;
};

