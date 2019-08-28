TextureCube	cubeTexture: register(t0);
SamplerState textureSampler;

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float3 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;
};

struct DeferredPixelOut
{
	float4 textBuffer		: SV_Target0;
	float4 normalBuffer		: SV_Target1;
	float4 positionBuffer	: SV_Target2;
};

DeferredPixelOut main(VS_OUT input)
{
	DeferredPixelOut psOut;
	
	float3 incident = (input.camPos - input.worldPos);
	float3 reflectionDirection = reflect(incident, input.Normal);
	float4 reflectionColour = cubeTexture.Sample(textureSampler, reflectionDirection);

	//psOut.textBuffer = float4(1.0f, 0.0f, 0.0f, 1.0f);
	psOut.textBuffer = reflectionColour;
	psOut.normalBuffer = float4(input.Normal, 1.0f);
	psOut.positionBuffer = float4(input.worldPos, 1.0f);

	return psOut;
};