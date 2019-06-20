struct VS_IN
{
    float3 Pos : SV_POSITION;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 tex : TEXCOORD;
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

VS_OUT main(VS_IN input) {
	VS_OUT output;
	output.pos = mul(float4(input.Pos, 1.0f), world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection).xyww;
	output.tex = input.Pos.xyz;
	return output;
}