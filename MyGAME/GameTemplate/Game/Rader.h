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
	 * @brief 指定の要素番号に敵の位置座標を保存する
	 * @param no 要素番号
	 * @param enemyPos 敵の位置座標
	*/
	void SaveEnemyPos(int no, Vector3 enemyPos);

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
	Vector3 m_enemyPos[10];										//敵の位置
	Vector3 m_prevEnemyPos[10];
	EnemyIconOnRader* m_enemyIcon[10] = { nullptr };				//レーダーの敵
	SpriteRender* m_playerIcon = nullptr;						//プレイヤー
	SpriteRender* m_rader = nullptr;
};

