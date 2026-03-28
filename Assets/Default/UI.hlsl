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

SV_OUT VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	SV_OUT outData;

	outData.pos = mul(pos, mul(EnvCB_Transform, EnvCB_ViewProjection)),
	outData.color = RenderCB_Color;
	
	return outData;
}

float4 PS(SV_OUT inData) : SV_TARGET
{
	return float4(1, 0, 0, 1);
	float4 color = inData.color;
	return color;
}
