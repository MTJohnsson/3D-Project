#include "Lights.hlsl"

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
SamplerState sampAni;

struct GSOutput
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float3 Color : COLOR;
    float3 worldPos : WORLDPOS;
    float3 camPos : CAMPOS;

    float3 edge1 : EDGE1;
    float3 edge2 : EDGE2;
    float2 deltaUV1 : dtUV1;
    float2 deltaUV2 : dtUV2;
};

struct LightPixelShader
{
    float lightType;
    float3 AmbientColor;
    float AmbientLight;
    float3 LightColor;
    float Strength;
    float3 position;
};

cbuffer LIGHT : register(b0)
{
    LightPixelShader light[2];
};

float4 main(GSOutput input) : SV_Target
{
    // Texture
    float3 texDiffuse = txDiffuse.Sample(sampAni, input.tex).xyz;


    float4 finalColor = float4(1.f, 1.f, 1.f, 1.f);
   
    for (int i = 0; i < 2; i++)
    {
        float3 ambientLight = light[i].AmbientColor * light[i].AmbientLight;
        finalColor = float4(finalColor.xyz * ambientLight, 1);
        //float intensity = CalculateLight(light[i].lightType, light[i].position, input.worldPos, input.normal);
        //float intensity = SpotLight(light[i].position, input.worldPos);
        float intensity = PointLight(input.normal, light[i].position, input.worldPos);
        
        float3 diffuseLight = intensity * light[i].LightColor;
       
        finalColor += float4(diffuseLight, 1.0);
        finalColor = min(finalColor, float4(1.0, 1.0, 1.0, 1.0));
    }

    return float4(input.Color * finalColor.xyz, 1);
};