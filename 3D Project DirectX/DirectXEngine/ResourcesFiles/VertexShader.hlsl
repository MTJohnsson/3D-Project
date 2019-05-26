cbuffer CB_PER_FRAME : register(b0){
    float4x4 worldMat;
    float4x4 world;
    float3 camPos;
}

struct VS_IN{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
    float2 TexCoordIn : TEXCOORD;
};

struct VS_OUT{
	float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoordOut : TEXCOORD;
    float3 worldPos : POSITION;
    float3 camPos : CAM;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
    

    output.Pos = mul(float4(input.Pos, 1.0f), worldMat);
    output.worldPos = mul(float4(input.Pos, 1.0f), world);
    output.Normal = mul(float4(input.Normal, 1.0f), worldMat);
    output.TexCoordOut = input.TexCoordIn;
    output.camPos = camPos;
	return output;
}