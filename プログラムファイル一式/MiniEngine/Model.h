#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"

class IShaderResource;

//モデルの上方向
enum EnModelUpAxis {
	enModelUpAxisY,		//モデルの上方向がY軸。
	enModelUpAxisZ,		//モデルの上方向がZ軸。
};
/// <summary>
/// モデルの初期化データ
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;							//tkmファイルパス。
	const char* m_vsEntryPointFunc = "VSMain";						//頂点シェーダーのエントリーポイント。
	const char* m_vsSkinEntryPointFunc = "VSMain";					//スキンありマテリアル用の頂点シェーダーのエントリーポイント。
	const char* m_psEntryPointFunc = "PSMain";						//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;								//.fxファイルのファイルパス。
	void* m_expandConstantBuffer = nullptr;							//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize = 0;								//ユーザー拡張の定数バッファのサイズ。
	std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV> m_expandShaderResoruceView = { nullptr };			//ユーザー拡張のシェーダーリソース。
	Skeleton* m_skeleton = nullptr;									//スケルトン。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					//モデルの上方向。
	D3D12_CULL_MODE cullingMode = D3D12_CULL_MODE_BACK;
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
};

/// <summary>
/// モデルクラス。
/// </summary>
class Model {

public:

	/// <summary>
	/// tkmファイルから初期化。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	void Init( const ModelInitData& initData );
	/// <summary>
	/// ワールド行列の更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void Draw(RenderContext& renderContext);
	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	/// <returns></returns>
	void Draw(RenderContext& renderContext, Camera camera);
 
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}

	const Matrix& GetPrevWorldMatrix() const
	{
		return m_prevWorld;
	}

	void DrawInstancing(RenderContext& rc, int numInstance);

	/// <summary>
	/// メッシュに対して問い合わせを行う。
	/// </summary>
	/// <param name="queryFunc">問い合わせ関数</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc) 
	{
		m_meshParts.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
	}
	/// <summary>
	/// アルベドマップを変更。
	/// </summary>
	/// <remarks>
	/// この関数を呼び出すとディスクリプタヒープの再構築が行われるため、処理負荷がかかります。
	/// 毎フレーム呼び出す必要がない場合は呼び出さないようにしてください。
	/// </remarks>
	/// <param name="materialName">変更しいたマテリアルの名前</param>
	/// <param name="albedoMap">アルベドマップ</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
	/// <summary>
	/// TKMファイルを取得。
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *m_tkmFile;
	}

	//TkmFile& operator=(TkmFile& tkmFile) {
	//	//TkmFile file;
	//	m_tkmFile = tkmFile;
	//	return m_tkmFile;
	//}

	Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mWorld;
		Matrix mBias;
		if (m_modelUpAxis == enModelUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);

		mWorld = mBias * mScale * mRot * mTrans;
		return mWorld;
	}
private:
	Matrix m_world;														//ワールド行列。
	Matrix m_prevWorld;													//1フレーム前のワールド行列
	TkmFile* m_tkmFile;													//tkmファイル。
	Skeleton m_skeleton;												//スケルトン。
	MeshParts m_meshParts;											//メッシュパーツ。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;		//モデルの上方向。

	bool m_isInited = false;
};
