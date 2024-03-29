struct VS_IN
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 Color : COLOR;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 Color : COLOR;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.Pos = float4(input.Pos, 1.0f);
    output.Normal = input.Normal;
    output.tex = input.tex;
    output.Color = input.Color;

    return output;
}