cbuffer EnvCB_ : register(b1)
{
	row_major matrix EnvCB_Transform; // 座標変換行列
	row_major matrix EnvCB_ViewProjection; // ビュープロジェクション
	float4 EnvCB_DirectionalLightDirection; // 平行光源/光線 の向き
}

cbuffer RenderCB : register(b0)
{
	float4 RenderCB_Color;
}

struct SV_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

SV_OUT VS(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD)
{
	SV_OUT outData;

	// ワールドビュープロジェクション
	matrix wvp = mul(EnvCB_Transform, EnvCB_ViewProjection);

	// ワールド座標
	outData.pos = mul(pos, wvp);

	// ワールド法線(平行移動成分を消した行列を掛ける)
	float3 worldNormal = normalize(mul(normal.xyz, (float3x3) EnvCB_Transform));
	
	// ライトの方向
	float3 light = normalize(-EnvCB_DirectionalLightDirection);

	float nDotL = saturate(dot(worldNormal, light));
	float3 ambient = RenderCB_Diffuse.rgb * 0.1f;
	outData.color.rgb = (RenderCB_Diffuse.rgb * nDotL) + ambient;
	outData.color.a = RenderCB_Diffuse.a;
	
	return outData;
}

float4 PS(SV_OUT inData) : SV_TARGET
{
	return float4(1, 0, 0, 1);
	float4 color = inData.color;
	return color;
}
