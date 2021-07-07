#pragma once
#include "LightManager.h"
class AnimationClip;

class SkinModelRender :public IGameObject
{
private:
	ModelInitData m_modelInitData;										//モデルのファイルパスやシェーダーを設定する
	ModelInitData m_shadowData;											//影生成のためのモデル情報
	Model m_model;														//モデル
	Model m_shadow;														//影モデル
	Skeleton m_skeleton;												//スケルトン

	Vector3 m_position = Vector3::Zero;									//モデルの位置座標
	Vector3 m_scale = Vector3::One;										//モデルの拡大率
	Quaternion m_rot = Quaternion::Identity;							//モデルの回転

	bool m_isCastShadow = false;										//具材のシャドウ用モデルのファイルパス変更に使用する。
	bool m_isApplyBlur = false;											//ブラーを適応するかどうか

	RenderTarget& m_shadowMap = GameObjectManager::GetInstance()->GetShadowMap();	//作成したシャドウの取得
	Camera m_lightCamera = GameObjectManager::GetInstance()->GetLightCamera();		//ライトカメラの取得

	struct wannaCopyToVRAM
	{
		Matrix s_lightCameraMatrix = GameObjectManager::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
		AllLight s_lig = LightManager::GetInstance().GetLightData();
	}s_dataCopyToVRAM;



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
	 * @brief モデルの初期化を行う。ライトを独自のものにしたいときはこっちをつかう
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos モデルを出現させる最初の位置
	 * @param pLig 設定した独自のライト
	*/
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);
	//モデルのファイルパスのみを変更するときに使用する。

	/**
	 * @brief 影を生成する人のモデル初期化関数
	 * @param filePath 　
	 * @param skeletonPath 
	 * @param UpAxis 
	 * @param pos 
	 * @param pLig 
	*/
	void InitForCastShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief 影が映るもののモデル初期化関数
	 * @param filePath 
	 * @param skeletonPath 
	 * @param UpAxis 
	 * @param pos 
	*/
	void InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief 床専用シャドウレシーバ―としての初期化
	*/
	void InitAsFloor(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief モデルのファイルパスを変えたいときに使用
	 * @param newModelFilePath 新しいファイルパス
	*/
	void ChangeModel(const char* newModelFilePath);

	/**
	 * @brief 新しいファイルパスを適応する。
	*/
	void SetNewModel() {
		m_model.Init(m_modelInitData); 
		if (m_isCastShadow) {
			m_shadow.Init(m_shadowData);
		}
	}

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
	 * @brief アニメーションを設定する。
	 * @param animation アニメーション
	 * @param animationNum アニメーションの数
	*/
	//void InitAnimation(AnimationClip* animation, int animationNum);

	/**
	 * @brief アニメーションを再生する
	 * @param animNo アニメーションの数
	 * @param interpolateTime 流れる時間
	*/
	//void PlayAnimation(int animNo, float interpolateTime = 0.0f);

	/**
	 * @brief ブラーを適応するか
	 * @param state TRUE＝適応する
	*/
	void SetApplyBlur(bool state) { m_isApplyBlur = state; }

	/**
	 * @brief モデルを描く
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc) 
	{
		//普通描画
		if (GameObjectManager::GetInstance()->GetRenderTypes() == 0) {				
			m_model.Draw(rc);
			return;
		}
		//影作る
		if (GameObjectManager::GetInstance()->GetRenderTypes() == 1) {
			m_shadow.Draw(rc, GameObjectManager::GetInstance()->GetLightCamera());
			return;
		}
		if (GameObjectManager::GetInstance()->GetRenderTypes() == 2) {
			if (m_isApplyBlur) {
				m_model.Draw(rc);
			}
		}
	}

private:
	//アニメーションを付ける際に使う
	//Animation m_animation;
	//AnimationClip* m_animationClip = nullptr;
};

