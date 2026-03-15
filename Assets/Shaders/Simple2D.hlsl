
struct VS_OUT
{
	float4 pos : SV_POSITION;  // 位置
	float4 color : COLOR0;     // 色
};

VS_OUT VS(float4 pos : POSITION, float4 color : COLOR0)
{
	VS_OUT data;

	data.pos = pos;
	data.color = color;

	return data;
}

float4 PS(VS_OUT data) : SV_TARGET
{
	return data.color;
}
