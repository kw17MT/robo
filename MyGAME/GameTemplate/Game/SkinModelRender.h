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

	bool m_isSun = false;												//太陽かどうか

	struct copyToVRAMDatas
	{
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
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, bool isCastShadow);
	//モデルのファイルパスのみを変更するときに使用する。

	/**
	 * @brief 影が映るもののモデル初期化関数
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos 初期位置
	*/
	void InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos);

	/**
	 * @brief モデルのファイルパスを変えたいときに使用
	 * @param newModelFilePath 新しいファイルパス
	*/
	void ChangeModel(const char* newModelFilePath);

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

	void SetIsSun() { m_isSun = true; }

	/**
	 * @brief モデルを描く
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc) 
	{
		//普通描画
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::normal) {
			m_model.Draw(rc);
			return;
		}
		//影作る
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::shadow) {
			m_shadow.Draw(rc, RenderingEngine::GetInstance()->GetLightCamera());
			return;
		}
		//太陽だけ描画＝レンズゴースト作成
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::lensGhost) {
			if (m_isSun)
			{
				m_model.Draw(rc);
			}
			return;
		}
	}

private:
	//アニメーション
	Animation m_animation;
	AnimationClip* m_animationClip = nullptr;
};

