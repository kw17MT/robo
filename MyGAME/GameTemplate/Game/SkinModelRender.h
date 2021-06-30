#pragma once
class AnimationClip;

class SkinModelRender :public IGameObject
{
private:
	ModelInitData m_modelInitData;										//モデルのファイルパスやシェーダーを設定する
	ModelInitData m_shadowData;
	Model m_model;														//モデル
	Model m_shadow;
	Skeleton m_skeleton;												//スケルトン
	CharacterController m_charaCon;										//モデルの衝突判定や移動関係

	Vector3 m_position = Vector3::Zero;									//モデルの位置座標
	Vector3 m_scale = Vector3::One;										//モデルの拡大率
	Quaternion m_rot = Quaternion::Identity;							//モデルの回転

	RenderTarget m_shadowMap = GameObjectManager::GetInstance()->GetShadowMap();
	Camera m_lightCamera = GameObjectManager::GetInstance()->GetLightCamera();

	bool m_isCastShadow = false;
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
	 * @brief 影を生成する人のモデル初期化関数
	 * @param filePath 　
	 * @param skeletonPath 
	 * @param UpAxis 
	 * @param pos 
	 * @param pLig 
	*/
	void InitForCastShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* pLig);

	/**
	 * @brief 影が映るもののモデル初期化関数
	 * @param filePath 
	 * @param skeletonPath 
	 * @param UpAxis 
	 * @param pos 
	*/
	void InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* lig);

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
	 * @brief 影響を受けるライトの変更。
	 * @param light 適応したい新しいライト
	*/
	void InitLight(Light& light);

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
	 * @brief モデルを描く
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc) 
	{
		/*
		if (m_isCastShadow) {
			GameObjectManager::GetInstance()->GetLightCamera()->Update();
			m_model.Draw(rc, *GameObjectManager::GetInstance()->GetLightCamera());
		}
		else {
			m_model.Draw(rc);
		}
		*/

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
	}

	/**
	 * @brief 影を生成するときに使用する
	 * @param rc 今のレンダーターゲット
	 * @param lightCamera 光の位置
	*/
	/*void Render(RenderContext& rc, Camera lightCamera) { m_model.Draw(rc, lightCamera); }*/
private:
	//Animation m_animation;
	//AnimationClip* m_animationClip = nullptr;
};

