struct VS_IN
{
	float3 Pos : POSITION;
};

struct VS_OUT
{
	float4 Pos :POSITION;
};

struct DeferredPixelOut
{
	float4 textBuffer		: SV_Target0;
	float4 normalBuffer		: SV_Target1;
	float4 positionBuffer	: SV_Target2;
	float4 normalMapBuffer	: SV_Target3;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.Pos = float4(input.Pos,1.0);
	return output;
}

