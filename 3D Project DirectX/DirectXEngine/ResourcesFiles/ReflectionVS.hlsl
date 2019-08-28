struct VS_IN
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float3 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;
};

cbuffer CB_PER_FRAME : register(b0)
{
	// MVP
	float4x4 view; //16
	float4x4 projection;
	float3 camPos; //12
	float padding; //4 now 16
}

cbuffer CB_PER_FRAME : register(b1)
{
	float4x4 world; //16byte
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	float4 pos = float4(input.Pos, 1.0f);
	float4x4 mvp = mul(projection, mul(view, world));
	output.Pos = mul(mvp, pos);
	output.Normal = mul((float3x3)world, normalize(input.Normal));
	output.tex = input.tex;
	output.worldPos = (float3)normalize(mul(world, input.Pos));
	output.camPos = mul((float3x3)world, camPos);

	return output;
}