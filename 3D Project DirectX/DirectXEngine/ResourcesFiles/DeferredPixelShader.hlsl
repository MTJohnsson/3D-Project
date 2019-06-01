#include "Lights.hlsl"

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
SamplerState sampAni;

struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float3 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;

	float3 edge1 : EDGE1;
	float3 edge2 : EDGE2;
	float2 deltaUV1 : dtUV1;
	float2 deltaUV2 : dtUV2;
};


struct DeferredPixelOut
{
	float4 textBuffer		: SV_Target0;
	float4 normalBuffer		: SV_Target1;
	float4 positionBuffer	: SV_Target2;
	float4 normalMapBuffer	: SV_Target3;
};

DeferredPixelOut main(GSOutput input)
{
	DeferredPixelOut DefPOut;
	DefPOut.textBuffer = txDiffuse.Sample(sampAni, input.tex);
	//DefPOut.textBuffer = float4(1.0f, 0.0f, 1.0f, 1.0f);
	DefPOut.normalBuffer = float4(input.normal, 1.0f);
	DefPOut.positionBuffer = float4(input.worldPos, 1.0f);
	DefPOut.normalMapBuffer = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return DefPOut;
}