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
class ClearTelop;
class Fade;
class AfterGameOverScene;

/**
 * @brief インゲームクラス
*/
class Game : public IGameObject
{
private:
	int m_killedEnemyNum = 0;					//除去した敵の総数
	bool m_isSoundClear = false;				//クリアサウンドをならしたかどうか
	float m_colapsedTimeAfterClear = 0.0f;		//クリアした後の経過時間
	float m_bgmVolume = 0.5f;					//BGMの音量
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
	Player* m_player = nullptr;							//プレイヤーインスタンス
	EnemyGenerator* m_enemyGenerator = nullptr;			//敵生成器インスタンス
	SkyCube* m_sky = nullptr;							//スカイキューブマップインスタンス
	Ground* m_ground = nullptr;							//地面インスタンス
	UI* m_ui = nullptr;									//UIインスタンス
	Sun* m_sun = nullptr;								//太陽インスタンス
	Rader* m_rader = nullptr;							//レーダーインスタンス
	CSoundSource* m_bgm = nullptr;						//BGMインスタンス
	EliminateTelop* m_eliminateTelop = nullptr;			//殲滅せよテロップ
	ObjectiveEnemyNum* m_objectiveEnemyNum = nullptr;	//目標撃破数表示インスタンス
	ClearTelop* m_clear = nullptr;						//クリアテロップインスタンス
	Fade* m_fade = nullptr;								//フェードインスタンス
	AfterGameOverScene* m_afterGOScene = nullptr;		//ゲームオーバー後の各シーンへの遷移誘導インスタンス
};	

