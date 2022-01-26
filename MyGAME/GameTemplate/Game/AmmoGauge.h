#pragma once
class SpriteRender;
class FontRender;

//弾の種類
enum EnAmmoTypes
{
	enAmmoBullet,					//マシンガンの弾
	enAmmoMissile,					//ミサイル
	enAmmoRaser						//レールガン
};

class AmmoGauge : public IGameObject
{
private:
	Vector3 m_scale = { 2.0f,1.0f,1.0f };		//ゲージの拡大率
	Vector3 m_screenPos = Vector3::Zero;		//スクリーン上での位置
	Vector2 m_pivot = { 1.0f,0.0f };			//画像の中心
	const float m_fontScale = 0.5f;					//文字の拡大率

	int m_remaining_ammo = 0;					//残弾数
	int m_max_ammo = 0;							//種類に応じた最大弾数

	float m_reloadTimer = 0.0f;					//リロードタイマー
	bool m_isReloaded = true;					//リロードされているか（残弾はあるか）
	bool m_finishReloading = false;				//そのフレームにリロードが終わったか

	float m_gaugeScaleX = 1.0f;

	EnAmmoTypes m_ammoType = enAmmoBullet;		//どの種類の弾を管理するか
public:
	/**
	 * @brief コンストラクタ
	*/
	AmmoGauge() {}

	/**
	 * @brief デストラクタ
	*/
	~AmmoGauge();

	/**
	 * @brief インスタンスが生成された時に呼ばれる関数
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief リロードを行う関数
	 * @param reloadTime リロードにかかる時間
	*/
	void Reloading(const float reloadTime);

	/**
	 * @brief リロードされているか
	 * @return TRUE　されている　FALSE　されていない
	*/
	bool GetIsReloaded() { return m_isReloaded; }

	/**
	 * @brief そのフレームでリロードが終わったか確認する
	 * @return そのフレームでリロードが終わったか
	*/
	bool GetIsFinishedReloaded() { return m_finishReloading; }

	/**
	 * @brief 弾の種類を設定する
	 * @param ammoType  弾の種類
	*/
	void SetAmmoType(EnAmmoTypes ammoType) { m_ammoType = ammoType; }

	/**
	 * @brief 種類に応じた残弾数を設定する
	 * @param remainingAmmo 残弾数
	*/
	void SetRemainingAmmo(int remainingAmmo) { m_remaining_ammo = remainingAmmo; }

	/**
	 * @brief 種類に応じた最大弾数を設定する
	 * @param maxAmmo 最大弾数
	*/
	void SetMaxAmmo(int maxAmmo) { m_max_ammo = maxAmmo; }

	/**
	 * @brief 位置を更新する
	 * @param pos 位置
	*/
	void SetPosition(Vector3 pos) { m_screenPos = pos; }
private:
	SpriteRender* m_spriteRender = nullptr;				//ゲージ画像インスタンス
	SpriteRender* m_gaugeFrame = nullptr;
	FontRender* m_fontRender = nullptr;					//残弾数インスタンス
};

