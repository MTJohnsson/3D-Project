struct GS_IN
{
	float4 Pos : POSITION;
};

struct GSOutput
{
	float4 Pos: SV_POSITION;
	float2 tex : TEXCOORD;
	float3 Normal:NORMAL;
	float3 camPos : CAMPOS;
	float3 worldPos : WORLDPOS;
};
cbuffer camVector : register(b1)
{
	float3 upp;
}
cbuffer CB_PER_FRAME : register(b0)
{
	// MVP
	float4x4 view; //16
	float4x4 projection;
	float3 camPos; //12
	float padding; //4 now 16
}

cbuffer CB_PER_OBJECT : register(b2)
{
	float4x4 world; //16byte
}

[maxvertexcount(4)]
void main(point GS_IN input[1], inout TriangleStream<GSOutput> OutputStream)
{
	GSOutput output;
	//	float3 up = (0, 1, 0);
	float3 viewDir = camPos.xyz - input[0].Pos;
	viewDir = normalize(viewDir);
	// right = up x F 
	float3 right = cross(upp, viewDir);
	float size = 5;
	//blev fel här tidigare
	//ordningen i cross producten är viktigt då den bestämmer riktning.
	float3 up = normalize(cross(viewDir, right));
	right = normalize(right);
	float3 vert[4];
	vert[0] = input[0].Pos + (-right * size); // Bottom left
	vert[1] = input[0].Pos + (right * size); // Bottom right
	vert[2] = input[0].Pos + ((-right + up) * size); // Top left
	vert[3] = input[0].Pos + ((right + up) * size); // Top right

	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);
	matrix mvp = mul(projection, mul(view, world));
	for (int i = 0; i < 4; i++)
	{
		output.Pos = mul(projection, mul(view, float4(vert[i], 1.0f)));
		output.tex = texCoord[i];
		output.Normal = viewDir; // Normal;
		output.camPos = camPos.xyz;
		output.worldPos = vert[i];
		OutputStream.Append(output);
	}
	OutputStream.RestartStrip();
}