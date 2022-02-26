#pragma once
class Missile;
class AmmoGauge;
class Enemy;

/**
 * @brief ミサイルを生成するクラス
*/
class MissileGenerator : public IGameObject
{
private:
	Vector3 m_launchPos = Vector3::Zero;	//発射される元の位置
		
	int m_lockOnTargetNum = 0;				//今までロックオンしてきたターゲットの数
	int m_remaining_missile = 20;			//残りのミサイル数

	bool m_isPrepareLaunch = false;			//LB2ボタンが長押しされて発射準備されているか			
	bool m_canTargetMore = true;			//さらにロックオンできるかどうか
	bool m_deleteMissileIcon = false;		//ミサイルロックオンのアイコンを消すかどうか
public:
	/**
	 * @brief デストラクタ
	*/
	MissileGenerator() {}

	/**
	 * @brief コンストラクタ
	*/
	~MissileGenerator();

	/**
	 * @brief インスタンスが生成された時に一度だけ行われる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief 発射元の位置を設定する関数
	 * @param pos プレイヤーの位置
	*/
	void SetLaunchPos(Vector3 pos) { m_launchPos = pos; }

	/**
	 * @brief ミサイルが発射されてロックオンアイコンを消すかどうかを取得する
	 * @return アイコンを消すかどうか
	*/
	bool GetDeleteMissileIcon() { return m_deleteMissileIcon; }

	/**
	 * @brief ミサイルの着弾先となる敵のインスタンスを保存する
	 * @param enemy ミサイルの着弾先となる敵
	*/
	void SaveTargetedEnemy(Enemy* enemy);

	/**
	 * @brief さらにミサイルでロックオンできるかどうかを取得する
	 * @return TRUEできる　FALSEできない
	*/
	bool CanTargetMore() { return m_canTargetMore; }
private:
	std::vector<Missile*> m_missiles = { nullptr };				//自分が生成するミサイルのインスタンス
	std::vector<Enemy*> m_enemy = { nullptr };					//ロックオン先となる敵のインスタンス保存用
	AmmoGauge* m_ammoGauge = nullptr;							//残り弾数の表示とそのゲージを出すインスタンス
};