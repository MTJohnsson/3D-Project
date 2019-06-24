TextureCube texCube : register(t0);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD;
};


struct DeferredPixelOut
{
	float4 textBuffer		: SV_Target0;
	float4 normalBuffer		: SV_Target1;
	float4 positionBuffer	: SV_Target2;
};

DeferredPixelOut main(VS_OUT input)
{
	DeferredPixelOut DefPOut;

	DefPOut.textBuffer = texCube.Sample(sampAni, input.tex);
	/*float height = input.tex.y;
	if (height < 0.0f) {
		height = 0;
	}
	DefPOut.textBuffer = lerp(float4(0.0f, 2.0f, 2.0f, 1.0f), float4(0.0f, 10.0f, 10.0f, 1.0f), height);
	*/
	DefPOut.normalBuffer = float4(0.0f, 0.0f, 0.0f, 1.0f);
	DefPOut.positionBuffer = float4(input.pos);

	return DefPOut;
}