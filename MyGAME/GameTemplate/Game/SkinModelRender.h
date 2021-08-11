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

	struct copyToVRAMDatas
	{
		//光の位置を示すカメラの取得
		//Matrix s_lightCameraMatrix = GameObjectManager::GetInstance()->GetLightCamera().GetViewProjectionMatrix();
		//ディレクション、スポット、アンビエントライトすべてのライトを取得
		AllLight s_lig = LightManager::GetInstance().GetLightData();
	}s_dataCopyToVRAM;

	enum EnRenderTaypes
	{
		enRenderNormal,					//通常描画タイプ
		enRenderShade,					//影描画タイプ
		enRenderLuminance,				//輝度描画タイプ
		enRenderDepthInView				//被写界深度描画タイプ
	};

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
	Vector3 GetPosition() const { return m_position; }

	/**
	 * @brief モデルの拡大率を設定する。
	 * @return 新しい拡大率
	*/
	Vector3 GetScale() const { return m_scale; }

	/**
	 * @brief モデルの初期化を行う。ライトを独自のものにしたいときはこっちをつかう
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos モデルを出現させる最初の位置
	*/
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);
	//モデルのファイルパスのみを変更するときに使用する。

	/**
	 * @brief 影を生成する人のモデル初期化関数
	 * @param filePath 使用するモデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos 初期位置
	*/
	void InitForCastShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief 影が映るもののモデル初期化関数
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos 初期位置
	*/
	void InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief 床専用シャドウレシーバ―としての初期化
	 * @param modelFilePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos 初期位置
	*/
	void InitAsFloor(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);
	
	/**
	 * @brief ゲージ専用初期化
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos 初期位置
	 * @param gaugeNumber どっちにあるゲージか０が左、１が右、２は調理用ゲージ
	*/
	void InitAsGauge(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, int gaugeNumber);

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
		if (GameObjectManager::GetInstance()->GetRenderTypes() == enRenderNormal
			|| GameObjectManager::GetInstance()->GetRenderTypes() == enRenderDepthInView) {
			m_model.Draw(rc);
			return;
		}
		//影作る
		if (GameObjectManager::GetInstance()->GetRenderTypes() == enRenderShade) {
			m_shadow.Draw(rc, GameObjectManager::GetInstance()->GetLightCamera());
			return;
		}
		if (GameObjectManager::GetInstance()->GetRenderTypes() == enRenderLuminance) {
			if (m_isApplyBlur) {
				m_model.Draw(rc);
				return;
			}
		}
	}

private:
	//アニメーションを付ける際に使う
	//Animation m_animation;
	//AnimationClip* m_animationClip = nullptr;
};

