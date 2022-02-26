#pragma once
class SpriteRender;
class EnemyIconOnRader;

class Rader : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;		//スクリーン上の位置
	Vector3 m_playerPos = Vector3::Zero;		//プレイヤーの位置
	Vector3 m_scale = Vector3::Zero;			//拡大率
	Vector2 m_pivot = Vector2::Zero;			//中央

	int m_enemyNum = 0;							//敵の数
public:
	/**
	 * @brief コンストラクタ
	*/
	Rader() {}

	/**
	 * @brief デストラクタ
	*/
	~Rader();

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
	 * @brief 指定配列番号に敵の位置と生存状態を保存する
	 * @param no 指定要素番号
	 * @param enemyPos 敵の位置
	*/
	void SaveEnemyPosAndIsAlive(int no, Vector4 enemyPos);

	/**
	 * @brief 生成されている敵の数を保存する
	 * @param num 敵の数
	*/
	void SetEnemyNum(int num) { m_enemyNum = num; }

	/**
	 * @brief プレイヤーの座標を保存する
	 * @param pos プレイヤーの座標
	*/
	void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
private:
	Vector4 m_enemyPosAndIsAlive[10];							//敵の位置と生存状況
	Vector3 m_prevEnemyPos[10];									//1フレーム前の敵の位置
	EnemyIconOnRader* m_enemyIcon[10] = { nullptr };			//レーダーの敵
	SpriteRender* m_playerIcon = nullptr;						//プレイヤー
	SpriteRender* m_rader = nullptr;
};

