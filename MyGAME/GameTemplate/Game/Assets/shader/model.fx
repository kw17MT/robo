/*!
 * @brief	シンプルなモデルシェーダー。
 */


//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
cbuffer LightCb : register (b1) {
	float3 ligDir;
	float3 ligColor;

	float3 eyePos;

	//ポイントライト、スポットライトが欲しい時以下を使う。
	/*float3 ptPos;
	float3 ptColor;
	float ptRange;

	float3 spDir;
	float spAngle;*/
};
	

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;	
	float3 normal	: NORMAL;	//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;
	float3 normal		: NORMAL;//スクリーン空間でのピクセルの座標。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float4 worldPos		: TEXCOORD1;
};

// グローバル変数。
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specMap : register(t2);



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
	SPSIn psIn;
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
	psIn.uv = vsIn.uv;

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

//float CalcDiffuseFromFresnel(float3 normal, float3 - DirectionLightDirection, float3 toEye)
//{
//
//}

//ピクセルシェーダーのエントリー関数。
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//ディレクションライト用/////////////////////////////////////////////
	//拡散反射光
	float t = dot(psIn.normal, ligDir);
	t *= -1.0f;
	if (t < 0.0f) { t = 0.0f; }
	float3 dirDiff = ligColor * t;

	//鏡面反射光
	float3 ref = reflect(ligDir, psIn.normal);
	float3 toEye = eyePos - psIn.worldPos;
	toEye = normalize(toEye);
	t = dot(ref, toEye);
	if (t < 0.0f) { t = 0.0f; }
	t = pow(t, 2.0f);
	float3 dirSpec = ligColor * t;
	//////////////////////////////////////////////////////////////////////

	//ポイントライト用///////////////////////////////////////////////////
	//影響度
	//float3 Pt2Surface = psIn.worldPos - ptPos;
	//float distance = length(Pt2Surface);
	//float affect = 1.0f - 1.0f / ptRange * distance;
	//if (affect < 0.0f) { affect = 0.0f; }
	//affect = pow(affect, 1.0f);

	//Pt2Surface = normalize(Pt2Surface);

	////拡散反射光
	//t = dot(Pt2Surface, psIn.normal);
	//t *= -1.0f;
	//if (t < 0.0f) { t = 0.0f; }
	//
	//float3 ptDiff = ptColor * t;
	//ptDiff *= affect;

	////鏡面反射光
	//float3 ptRef = reflect(Pt2Surface, psIn.normal);
	//t = dot(toEye, ptRef);
	//if (t < 0.0f) { t = 0.0f; }
	//t = pow(t, 5.0f);

	//float3 ptSpec = ptColor * t;
	//ptSpec *= affect;
	/////////////////////////////////////////////////////////////////////////

	//スポットライト用///////////////////////////////////////////////////////
	/*float angle = dot(Pt2Surface, spDir);
	angle = acos(angle);

	float AngleAffect = 1.0f - 1.0f / spAngle * angle;
	if (AngleAffect < 0.0f) { AngleAffect = 0.0f; }
	AngleAffect = pow(AngleAffect, 0.5f);

	float3 spDiff = ptDiff * AngleAffect;
	float3 spSpec = ptSpec * AngleAffect;*/
	//////////////////////////////////////////////////////////////////////////

	//環境光
	float3 environment = { 0.3f, 0.3f, 0.3f };

	//最終光
	float4 finalcolor = g_albedo.Sample(g_sampler, psIn.uv);
	finalcolor.xyz *= (dirDiff + dirSpec + environment);

	//PBR実装しようとしている。↓
	float3 specColor = g_specMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
	float metaric = g_specMap.Sample(g_sampler, psIn.uv).a;

	//ディレクションライト＋環境光、　ポイントライトなし
	return finalcolor;
	
	//ポイントライトあり
	/*float3 finaldiff = dirDiff + ptDiff;
	float3 finalspec = dirSpec + ptSpec;*/

	//ポイントライト
	/*float3 finaldiff = dirDiff + spDiff;
	float3 finalspec = dirSpec + spSpec;*/

	/*finalcolor.xyz *= (finaldiff + finalspec + environment);
	return finalcolor;*/

	/*float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	return albedoColor;*/
}

float4 FrontCulling(SPSIn psIn) : SV_Target0
{
	float4 BLUE = { 0.0f, 1.0f, 1.0f, 1.0f };
	return BLUE;
}