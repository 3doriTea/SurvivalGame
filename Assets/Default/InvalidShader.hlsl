struct SV_OUT
{
	float4 pos : SV_POSITION;
};

SV_OUT VS(float4 pos : POSITION)
{
	SV_OUT outData;

	outData.pos = pos;
	
	return outData;
}

float4 PS(SV_OUT inData) : SV_TARGET
{
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
}
