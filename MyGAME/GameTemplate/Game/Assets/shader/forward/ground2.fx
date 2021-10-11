/*!
 * @brief	シンプルなモデルシェーダー。
 */

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer CalcVelocityMapMatrix : register(b1)
{
    float4x4 prevViewProjMatrix;
    float4x4 currentViewProjMatrix;
    float width;
    float height;
} 

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;	
	float3 normal	: NORMAL;	//モデルの頂点座標。
	float3 tangent  : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;
	float3 normal		: NORMAL;//スクリーン空間でのピクセルの座標。
	float3 tangent      : TANGENT;
	float3 biNormal     : BINORMAL;
	float2 uv 			: TEXCOORD0;	//uv座標。
	float4 worldPos		: TEXCOORD1;
    float4 posInLVP : TEXCOORD2;
};

struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float4 normal : SV_Target1; // 法線
    float4 SpecAndDepth : SV_Target2;
    float4 velocity : SV_Target3;				//4枚目のレンダーターゲットに速度マップを書き込みする
};

// グローバル変数。
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specMap : register(t2);
Texture2D<float4> g_velocityMap : register(t4);

Texture2D<float4> shadow : register(t10);
Texture2D<float4> noise : register(t11);
Texture2D<float4> sand : register(t12);
Texture2D<float4> snow : register(t13);
Texture2D<float4> grass : register(t14);

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

// 関数定義。
//スキン行列を計算する。
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

//頂点シェーダーのコア関数。
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn = (SPSIn)0;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else{
		m = mWorld;
	}

	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	
	psIn.normal = mul(m, vsIn.normal);
	psIn.normal = normalize(psIn.normal);
	
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    
    //modify here 
    float2 newUV = vsIn.uv;
    
    //もともとのuvのMAX、MINで割る　↓まだできてない
    //newUV.x /= newUV.x;
    //newUV.y /= newUV.y;
    
    //newUV.x /= width;
    //newUV.y /= height;
    
    //newUV.xy -= 0.5f;
    //newUV.xy *= 2.0f;
    
    newUV.xy += 6.0f;
    newUV.x *= 0.153f / 2.0f;
    newUV.y *= 0.153f / 2.0f;
    
    psIn.uv = newUV;
	//psIn.uv = vsIn.uv;
	
   // psIn.posInLVP = mul(mLVP, psIn.worldPos);

	return psIn;
}

//スキンなしメッシュ用の頂点シェーダーのエントリー関数。
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}

//スキンありメッシュの頂点シェーダーのエントリー関数。
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut = (SPSOut)0;

    // アルベドカラーを出力
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);

    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    float3 normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + psIn.normal * localNormal.z;
    // 法線を出力
    // 出力は0～1に丸められてしまうのでマイナスの値が失われてしまう
    // そのため-1～1を0～1に変換する
    // (-1 ～ 1) ÷ 2.0       = (-0.5 ～ 0.5)
    // (-0.5 ～ 0.5) + 0.5  = ( 0.0 ～ 1.0)
    psOut.normal.xyz = (normal / 2.0f) + 0.5f;
	//鏡面反射の強さを取得
    psOut.normal.w = g_specMap.Sample(g_sampler, psIn.uv).r;
	
    psOut.SpecAndDepth.xyz = g_specMap.Sample(g_sampler, psIn.uv).rgb;
	//深度値を記録
    psOut.SpecAndDepth.w = psIn.pos.z;
		
	/* ベロシティマップ */
    float4 prevVelocity = mul(prevViewProjMatrix,float4(psIn.worldPos.xyz, 1.0f));
    float4 currentVelocity = mul(currentViewProjMatrix,float4(psIn.worldPos.xyz, 1.0f));
    
	// prevVelocityとcurrentVelocityを正規化座標系に変換する
    prevVelocity.xy /= prevVelocity.w;
    currentVelocity.xy /= currentVelocity.w;

    prevVelocity.xy *= 0.5f;
    prevVelocity.xy += 0.5f;
    prevVelocity.xy *= float2(1280, 720);
	
    currentVelocity.xy *= 0.5f;
    currentVelocity.xy += 0.5f;
    currentVelocity.xy *= float2(1280, 720);
	
	
    psOut.velocity.xy = (prevVelocity.xy - currentVelocity.xy);
	//ビューポート座標系に変換
    psOut.velocity.zw = 0.0f;
    
	
    float4 splatMap = noise.Sample(g_sampler, psIn.uv);
    
    float4 sandTexture = sand.Sample(g_sampler, psIn.uv);
    float4 snowTexture = snow.Sample(g_sampler, psIn.uv);
    float4 grassTexture = grass.Sample(g_sampler, psIn.uv);
    
    float t = splatMap.r + splatMap.g + splatMap.b;
    float4 textureWeight;
    textureWeight.x = splatMap.r / t; //splatMap.w;
    textureWeight.y = splatMap.g / t; //splatMap.w;
    textureWeight.z = splatMap.b / t; //splatMap.w;
    

    float4 finalTexture = psOut.albedo;
    finalTexture += sandTexture * textureWeight.x;
    finalTexture += snowTexture * textureWeight.z;
    finalTexture += grassTexture * textureWeight.y;
    finalTexture /= 3.0f;
    finalTexture.w = 1.0f;
    psOut.albedo = finalTexture;
    
    //psOut.albedo = noise.Sample(g_sampler, psIn.uv);
	
    return psOut;
}