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
    float3 texNormal = txNormal.Sample(sampAni, input.tex).xyz; // normal map
    texNormal = (2 * texNormal.rgb) - 1; // normal = (2 * color)-1


    // Normal mapping
    float3 edge1 = input.edge1;
    float3 edge2 = input.edge2;
    float2 deltaUv1 = input.deltaUV1;
    float2 deltaUv2 = input.deltaUV2;

    float r = (float) (1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x));
   
    float3 tangent = r * (edge1 * deltaUv2.y - edge2 * deltaUv1.y);
    float3 bitangent = r * (edge2 * deltaUv1.x - edge1 * deltaUv2.x);

    tangent = cross(bitangent, texNormal);
    bitangent = cross(tangent, texNormal);

    float3x3 TBN = float3x3(tangent, bitangent, input.normal);

    float3 finalNormal = mul(texNormal, TBN);



    //float4 finalColor = float4(1.f, 1.f, 1.f, 1.f);
    float3 specular = float3(0.f, 0.f, 0.f);

    float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f); // ambiant light
    float4 final_color = float4((1.0f, 1.0f, 1.0f) * ambient.xyz, 1);

    for (int i = 0; i < 2; i++)
    {
        //float3 ambientLight = light[i].AmbientColor * light[i].AmbientLight;
        //finalColor = float4(finalColor.xyz * ambientLight, 1);
        ////float intensity = CalculateLight(light[i].lightType, light[i].position, input.worldPos, input.normal);
        ////float intensity = SpotLight(light[i].position, input.worldPos);
        //float intensity = PointLight(finalNormal, light[i].position, input.worldPos);
        
        //float3 diffuseLight = intensity * light[i].LightColor;
       
        //finalColor += float4(diffuseLight, 1.0);
        //finalColor = min(finalColor, float4(1.0, 1.0, 1.0, 1.0));


        // Diffuse
        //float3 lightDir = normalize(light[i].position - input.worldPos.xyz);
        //float diffuseFactor = dot(lightDir, finalNormal);

        //if (diffuseFactor < 0.0f)
        //{
        //    diffuseFactor = 0.0f;
        //}

        //float3 color = light[i].LightColor * diffuseFactor * light[i].Strength * (1.0 / length(lightDir));

        //final_color += float4(color, 1.0);
        //final_color = min(final_color, float4(1.0, 1.0, 1.0, 1.0));


        //// Specular light / Phong shading
        //float3 viewDir = normalize(input.camPos - input.worldPos.xyz);
        //float3 ref = pow(max((0.05 * (normalize(light[i].position - input.worldPos) * finalNormal)) * finalNormal - normalize(light[i].position - input.worldPos), 0), 10);
        //float t = dot(ref, finalNormal);
        //if (t < 0.0f)
        //{
        //    t = 0.0f;
        //}
        //specular += float3(0.1f, 0.1f, 0.1f) * t * 0.5f * (1.0 / length(normalize(light[i].position - input.worldPos)));
    
        
        // Without normal mapping
        float3 lightDir = normalize(light[i].position - input.worldPos.xyz);
        float diffuseFactor = dot(lightDir, input.normal);

        if (diffuseFactor < 0.0f)
        {
            diffuseFactor = 0.0f;
        }

        float3 color = light[i].LightColor * diffuseFactor * light[i].Strength * (1.0f / length(lightDir));

        final_color += float4(color, 1.0f);
        final_color = min(final_color, float4(1.0f, 1.0f, 1.0f, 1.0f));


        // Specular light / Phong shading
        float3 viewDir = normalize(input.camPos - input.worldPos.xyz);
        float3 ref = pow(max((2.25f * (normalize(light[i].position - input.worldPos) * input.normal)) * input.normal - normalize(light[i].position - input.worldPos), 0), 10.f);
        float t = dot(ref, input.normal);
        if (t < 0.0f)
        {
            t = 0.0f;
        }
        specular += float3(0.1f, 0.1f, 0.1f) * t * 0.5f * (1.0f / length(normalize(light[i].position - input.worldPos)));
    }

    return float4(texDiffuse * final_color.xyz + specular, 1.0f);
};