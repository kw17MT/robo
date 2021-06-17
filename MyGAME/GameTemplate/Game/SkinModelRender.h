#pragma once
class AnimationClip;

class SkinModelRender :public IGameObject
{
private:
	ModelInitData m_modelInitData;										//モデルのファイルパスやシェーダーを設定する
	Model m_model;														//モデル
	Skeleton m_skeleton;												//スケルトン
	CharacterController m_charaCon;										//モデルの衝突判定や移動関係

	Vector3 m_position = Vector3::Zero;									//モデルの位置座標
	Vector3 m_scale = Vector3::One;										//モデルの拡大率
	Quaternion m_rot = Quaternion::Identity;							//モデルの回転

public:
	SkinModelRender() {};
	//インスタンスを破棄
	~SkinModelRender();

	bool Start() { return true; }

	/**
	 * @brief スケルトン、アニメーション、位置座標、回転を設定する。
	*/
	void Update();

	/**
	 * @brief 位置座標を設定する。
	 * @param pos 位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }
	/**
	 * @brief 拡大率を設定する。
	 * @param scale 拡大率
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }

	/**
	 * @brief 回転率を設定する。
	 * @param rot 回転率
	*/
	void SetRotation(Quaternion rot) { m_rot = rot; }

	/**
	 * @brief モデルの座標を設定する。
	 * @return 新しい座標
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief モデルの拡大率を設定する。
	 * @return 新しい拡大率
	*/
	Vector3 GetScale() { return m_scale; }

	/**
	 * @brief モデルの初期化を行う
	 * @param modelFilePath モデルのファイルパス　Assets/～
	 * @param UpAxis どの軸を上にするか
	 * @param pos モデルを出現させる最初の位置
	*/
	void Init(const char* modelFilePath, EnModelUpAxis UpAxis, Vector3 pos);
	//モデルのファイルパスと初期位置を設定する。

	/**
	 * @brief モデルの初期化を行う。ライトを独自のものにしたいときはこっちをつかう
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos モデルを出現させる最初の位置
	 * @param pLig 設定した独自のライト
	*/
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* pLig = nullptr);
	//モデルのファイルパスのみを変更するときに使用する。

	/**
	 * @brief モデルのファイルパスを変えたいときに使用
	 * @param newModelFilePath 新しいファイルパス
	*/
	void ChangeModel(const char* newModelFilePath);

	/**
	 * @brief 新しいファイルパスを適応する。
	*/
	void SetNewModel() { m_model.Init(m_modelInitData); }

	/**
	 * @brief シェーダーのファイルパスと使うピクセルシェーダ―を変えたいときに使用する。
	 * @param filePath 新しいシェーダーのファイルパス
	 * @param entryPointPath 頂点シェーダー
	 * @param skinEntryPointPath ピクセルシェーダ―
	 * @param colorBuffer カラーのフォーマット　DXGI_FORMAT_R?G?B?A?_???等
	*/
	void InitShader(const char* filePath, const char* entryPointPath, const char* skinEntryPointPath, DXGI_FORMAT colorBuffer);

	/**
	 * @brief 主にフロントカリングに変えるときに使用する
	 * @param psEntryPoint ピクセルシェーダ―
	*/
	void SetFrontCulling(const char* psEntryPoint) { m_modelInitData.m_psEntryPointFunc = psEntryPoint; }

	/**
	 * @brief 影響を受けるライトの変更。
	 * @param light 適応したい新しいライト
	*/
	void InitLight(Light& light);

	/**
	 * @brief 影を落とす背景用の初期化関数
	 * @param texture 適応するテクスチャ
	 * @param lightcamera ライトの位置
	 * @param lightcamerasize カメラのサイズ
	*/
	void InitBackGroundLight(Texture texture, Matrix lightcamera, Matrix lightcamerasize);

	/**
	 * @brief アニメーションを設定する。
	 * @param animation アニメーション
	 * @param animationNum アニメーションの数
	*/
	void InitAnimation(AnimationClip* animation, int animationNum);

	/**
	 * @brief アニメーションを再生する
	 * @param animNo アニメーションの数
	 * @param interpolateTime 流れる時間
	*/
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);

	/**
	 * @brief モデルを描く
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc) { m_model.Draw(rc); }

private:
	Animation m_animation;
	AnimationClip* m_animationClip;
};

