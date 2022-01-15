#pragma once
class EnemyGenerator;
class Player;
class SkyCube;
class Ground;
class UI;
class Sun;
class Rader;
class LaunchPad;
class SCoundSource;
class EliminateTelop;
class ObjectiveEnemyNum;

class Game : public IGameObject
{
private:
	int m_killedEnemyNum = 0;
	bool m_isSoundClear = false;
	float m_colapsedTimeAfterClear = 0.0f;
	float m_bgmVolume = 0.5f;
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
	CSoundSource* m_bgm = nullptr;
	EliminateTelop* m_eliminateTelop = nullptr;
	ObjectiveEnemyNum* m_objectiveEnemyNum = nullptr;
};

