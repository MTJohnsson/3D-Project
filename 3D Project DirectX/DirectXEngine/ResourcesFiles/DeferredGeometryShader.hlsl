struct GS_IN
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float3 Color : COLOR;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float3 Color : COLOR;
	float3 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;

	// normal map
	float3 edge1 : EDGE1;
	float3 edge2 : EDGE2;
	float2 deltaUV1 : dtUV1;
	float2 deltaUV2 : dtUV2;
};

cbuffer CB_PER_FRAME : register(b0)
{
	// MVP
	float4x4 world; //16byte
	float4x4 view; //16
	float4x4 projection;
	float3 camPos; //12
	float padding; //4 now 16
}

[maxvertexcount(3)]
void main(triangle GS_IN input[3], inout TriangleStream<GSOutput> OutputStream)
{
	GSOutput output;

	float3 u = (float3)(input[1].Pos - input[0].Pos);
	float3 v = (float3)(input[2].Pos - input[0].Pos);
	float3 normal = normalize(cross(u, v));

	// normal map
	float2 deltaUV1 = input[1].tex - input[0].tex;
	float2 deltaUV2 = input[2].tex - input[0].tex;

	matrix mvp = mul(projection, mul(view, world));


	for (uint i = 0; i < 3; i++)
	{
		output.pos = mul(mvp, input[i].Pos);
		output.Color = input[i].Color;
		output.tex = input[i].tex;
		output.worldPos = (float3)normalize(mul(world, input[i].Pos));
		output.normal = normalize(mul((float3x3) world, normal));
		output.camPos = normalize(camPos);

		// normal map
		output.edge1 = normalize(u);
		output.edge2 = normalize(v);
		output.deltaUV1 = deltaUV1;
		output.deltaUV2 = deltaUV2;
		OutputStream.Append(output);
	}

	OutputStream.RestartStrip();
}