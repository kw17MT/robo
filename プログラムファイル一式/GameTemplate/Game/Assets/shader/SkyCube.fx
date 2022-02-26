///////////////////////////////////////
// 3Dモデル用のGBufferへの描画シェーダー。
///////////////////////////////////////

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer ViewProjectionMatrix
{
    float4x4 prevViewProjMatrix;
    float4x4 currentViewProjMatrix;
    float4x4 prevWorldMatrix;
};

struct SVSIn
{
    float4 pos : POSITION; //頂点座標。
    float3 normal : NORMAL; //法線。
    float2 uv : TEXCOORD0; //UV座標。
    float3 tangent : TANGENT; //接ベクトル。
    float3 biNormal : BINORMAL; //従ベクトル。
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
	float4 pos : SV_POSITION;       //座標。
	float3 normal : NORMAL;         //法線。
    float3 tangent : TANGENT; //接ベクトル。
    float3 biNormal : BINORMAL; //従ベクトル。
	float2 uv : TEXCOORD0;          //UV座標。
	float3 worldPos : TEXCOORD1;    // ワールド座標
    float3 prevWorldPos : TEXCOORD2;
};

struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float4 normal : SV_Target1; //法線
    float4 spec : SV_Target2;   //鏡面反射
    float4 velocity : SV_Target3; //4枚目のレンダーターゲットに速度マップを書き込みする
};

Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ
Texture2D<float4> g_velocity : register(t3);    //スペキュラマップ

TextureCube<float4> g_skyCubeMap : register(t10);

sampler g_sampler : register(s0);

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normal.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn = (SPSIn)0;
	float4x4 m;

    m = mWorld;


	psIn.pos = mul(m, vsIn.pos); // モデルの頂点をワールド座標系に変換
	// 頂点シェーダーからワールド座標を出力
	psIn.worldPos = psIn.pos;
    psIn.prevWorldPos = mul(prevWorldMatrix, vsIn.pos);

	psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
	psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
	psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
SPSIn VSMainSkin(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
SPSOut PSMain(SPSIn psIn) : SV_Target0
{
    SPSOut psOut = (SPSOut) 0;
	
	float4 albedoColor;
	float3 normal = normalize(psIn.normal);
    psOut.normal.xyz = normal;
	//albedoColor = g_skyCubeMap.Sample(g_sampler, psIn.normal);
	psOut.albedo = g_skyCubeMap.Sample(g_sampler, normal) * 2.0f;
	
    
	/* ベロシティマップ */
    float4 prevVelocity = mul(prevViewProjMatrix, float4(psIn.prevWorldPos, 1.0f));
    float4 currentVelocity = mul(currentViewProjMatrix, float4(psIn.worldPos.xyz, 1.0f));
    
	//prevVelocityとcurrentVelocityを正規化座標系に変換する
    prevVelocity.xyz /= prevVelocity.w;
    currentVelocity.xyz /= currentVelocity.w;
    
    prevVelocity.x *= 16.0f / 9.0f;
    currentVelocity.x *= 16.0f / 9.0f;
    prevVelocity.y *= -1.0f;
    currentVelocity.y *= -1.0f;

    psOut.velocity.xyz = (prevVelocity.xyz - currentVelocity.xyz);
    
	//ビューポート座標系に変換
    psOut.velocity.w = 1.0f;
	 
	
    return psOut;
}