//
//Texture2D txDiffuse : register(t0);
//Texture2D txNormal : register(t1);
//SamplerState sampAni;
//
//struct GSOutput
//{
//    float4 pos : SV_POSITION;
//    float3 normal : NORMAL;
//    float2 tex : TEXCOORD;
//    float3 worldPos : WORLDPOS;
//    float3 camPos : CAMPOS;
//
//    float3 edge1 : EDGE1;
//    float3 edge2 : EDGE2;
//    float2 deltaUV1 : dtUV1;
//    float2 deltaUV2 : dtUV2;
//};
//
//struct LightPixelShader
//{
//    float lightType;
//    float3 AmbientColor;
//    float AmbientLight;
//    float3 LightColor;
//    float Strength;
//    float3 position;
//};
//
//cbuffer LIGHT : register(b0)
//{
//    LightPixelShader light[2];
//};
//
//float4 main(GSOutput input) : SV_Target
//{
//    // Texture
//    float3 texDiffuse = txDiffuse.Sample(sampAni, input.tex).xyz;
//
//
//    //float4 finalColor = float4(1.f, 1.f, 1.f, 1.f);
//    float3 specular = float3(0.f, 0.f, 0.f);
//
//    float4 ambient = float4(0.2, 0.2, 0.2, 1); // ambiant light
//    float4 final_color = float4((1, 1, 1) * ambient.xyz, 1);
//
//
//    for (int i = 0; i < 2; i++)
//    {
//        //float3 ambientLight = light[i].AmbientColor * light[i].AmbientLight;
//        //finalColor = float4(finalColor.xyz * ambientLight, 1);
//        ////float intensity = CalculateLight(light[i].lightType, light[i].position, input.worldPos, input.normal);
//        ////float intensity = SpotLight(light[i].position, input.worldPos);
//        //float intensity = PointLight(input.normal, light[i].position, input.worldPos);
//        
//        //float3 diffuseLight = light[i].LightColor * intensity;
//       
//        //finalColor += float4(diffuseLight, 1.0);
//        //finalColor = min(finalColor, float4(1.0, 1.0, 1.0, 1.0));
//
//
//        // Diffuse
//        float3 lightDir = normalize(light[i].position - input.worldPos.xyz);
//        float diffuseFactor = dot(lightDir, input.normal);
//
//        if (diffuseFactor < 0.0f)
//        {
//            diffuseFactor = 0.0f;
//        }
//
//        float3 color = light[i].LightColor * diffuseFactor * light[i].Strength * (1.0 / length(lightDir));
//
//        final_color += float4(color, 1.0);
//        final_color = min(final_color, float4(1.0, 1.0, 1.0, 1.0));
//
//
//        // Specular light / Phong shading
//        float3 viewDir = normalize(input.camPos - input.worldPos.xyz);
//        float3 ref = pow(max((2.1f * (normalize(light[i].position - input.worldPos) * input.normal)) * input.normal - normalize(light[i].position - input.worldPos), 0), 10);
//        float t = dot(ref, input.normal);
//        if (t < 0.0f)
//        {
//            t = 0.0f;
//        }
//        specular += float3(0.1f, 0.1f, 0.1f) * t * 0.5f * (1.0 / length(normalize(light[i].position - input.worldPos)));
//		return float4(color, 1);
//	}
//	return float4(0.1f, 0.1f, 0.1f, 1);
//};

Texture2D txDiffuse : register(t0);
SamplerState sampAni;
struct GSOutput
{
	float4 Pos: SV_POSITION;
	float2 tex : TEXCOORD;
	float3 Normal :NORMAL;
};


struct DeferredPixelOut
{
	float4 textBuffer		: SV_Target0;
	float4 normalBuffer		: SV_Target1;
	float4 positionBuffer	: SV_Target2;
	float4 normalMapBuffer	: SV_Target3;
};

DeferredPixelOut main(GSOutput input)
{
	DeferredPixelOut DefPOut;
	
	DefPOut.textBuffer = txDiffuse.Sample(sampAni, input.tex);
	DefPOut.normalBuffer = float4(input.Normal, 1.0f);
	DefPOut.positionBuffer = float4(input.Pos.xyz, 1.0f);
	DefPOut.normalMapBuffer	= float4(1.0,1.0,1.0,1.0);
	return DefPOut;
}
