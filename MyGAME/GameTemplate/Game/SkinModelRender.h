#pragma once
#include "RenderingEngine.h"
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

	Texture m_texture[4];

	bool m_isSun = false;												//太陽かどうか
	bool m_isGround = false;											//地面かどうか
	bool m_isSky = false;

	struct Matrixes
	{
		EnMatrixes s_mat;					//現在と1フレーム前のプロジェクションビュー行列
		Matrix s_prevWorldMatrix;			//1フレーム前のワールド行列
	}s_matrix;	

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
	 * @brief モデルの座標を取得する。
	 * @return 新しい座標
	*/
	Vector3 GetPosition() const { return m_position; }

	/**
	 * @brief モデルの拡大率を取得する。
	 * @return 新しい拡大率
	*/
	Vector3 GetScale() const { return m_scale; }

	/**
	 * @brief モデルの回転を取得する。
	*/
	Quaternion GetRotation() const { return m_rot; }

	/**
	 * @brief モデルの初期化を行う。ライトを独自のものにしたいときはこっちをつかう
	 * @param filePath モデルのファイルパス
	 * @param skeletonPath スケルトンのファイルパス
	 * @param UpAxis どの軸を上にするか
	 * @param pos モデルを出現させる最初の位置
	*/
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, bool isCastShadow);
	
	void InitCharaCon(float radius, float height, Vector3& position);

	void InitGround(const char* modelFilePath, EnModelUpAxis UpAxis, Vector3 pos, bool isCastShadow);


	void InitSkyCube(const char* modelFilePath, EnModelUpAxis UpAxis);

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
	void SetIsGround() { m_isGround = true; }
	void SetIsSky() { m_isSky = true; }

	Model& GetModel() { return m_model; }

	/**
	 * @brief モデルを描く
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc) 
	{
		//描画前に現在と1フレーム前のプロジェクションビュー行列を更新
		s_matrix.s_mat = RenderingEngine::GetInstance()->GetPrevViewProjMatrix();
		//1フレーム前のワールド行列を更新
		s_matrix.s_prevWorldMatrix = m_model.GetPrevWorldMatrix();

		//普通描画
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::normal) {
			/*if (m_isSky) {
				return;
			}*/
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
		if (RenderingEngine::GetInstance()->GetRenderTypes() == RenderingEngine::EnRenderTypes::forward)
		{
			/*if (m_isSky) {
				m_model.Draw(rc);
			}*/
		}
	}

private:
	//アニメーション
	Animation m_animation;
	AnimationClip* m_animationClip = nullptr;
};

