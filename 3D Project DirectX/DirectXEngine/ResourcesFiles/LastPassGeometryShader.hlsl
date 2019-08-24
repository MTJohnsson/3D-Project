struct GS_IN
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TextureCoord : TEXCOORD;
	float4 worldPos : WORLDPOS;
};

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TextureCoord : TEXCOORD;
	float4 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;
};

cbuffer DISPLAY : register(b0)
{
	float display;
	float3 camPos;
}

[maxvertexcount(3)]
void main(triangle GS_IN input[3], inout TriangleStream<GS_OUT> OutputStream)
{
	GS_OUT output;

	//matrix mvp = mul(projection, mul(view, world));


	for (uint i = 0; i < 3; i++)
	{
		output.Pos = input[i].Pos;
		output.TextureCoord = input[i].TextureCoord;
		output.worldPos = input[i].worldPos;
		output.Normal = input[i].Normal;
		output.camPos = normalize(camPos);

		OutputStream.Append(output);
	}

	OutputStream.RestartStrip();
}