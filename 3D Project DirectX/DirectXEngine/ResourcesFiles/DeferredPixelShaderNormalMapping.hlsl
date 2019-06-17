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
};

DeferredPixelOut main(GSOutput input)
{
	DeferredPixelOut DefPOut;
	// Texture
	
	float3 texNormal = txNormal.Sample(sampAni, input.tex).xyz; // normal map
	texNormal = (2 * texNormal.rgb) - 1; // normal = (2 * color)-1


	// Normal mapping
	float3 edge1 = input.edge1;
	float3 edge2 = input.edge2;
	float2 deltaUv1 = input.deltaUV1;
	float2 deltaUv2 = input.deltaUV2;

	float r = (float)(1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x));

	float3 tangent = r * (edge1 * deltaUv2.y - edge2 * deltaUv1.y);
	float3 bitangent = r * (edge2 * deltaUv1.x - edge1 * deltaUv2.x);

	tangent = cross(bitangent, texNormal);
	bitangent = cross(tangent, texNormal);

	float3x3 TBN = float3x3(tangent, bitangent, input.normal);

	float3 finalNormal = mul(texNormal, TBN);

	DefPOut.textBuffer = txDiffuse.Sample(sampAni, input.tex);
	//DefPOut.textBuffer = float4(1.0f, 0.0f, 1.0f, 1.0f);
	DefPOut.normalBuffer = float4(finalNormal, 1.0f);
	DefPOut.positionBuffer = float4(input.worldPos, 1.0f);

	return DefPOut;
};