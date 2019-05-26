Texture2D txDiffuse : register(t0);
SamplerState sampAni;

struct VS_OUT
{
	float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoordIn : TEXCOORD;
    float3 worldPos : POSITION;
    float3 camPos : CAM;
};


float4 main(VS_OUT input) : SV_Target
{
    //float3 s = txDiffuse.Sample(sampAni, input.TexCoordIn).xyz;
    //return float4(s, 1.0f);
    
    float3 s = txDiffuse.Sample(sampAni, input.TexCoordIn).xyz;
	//punkten jag träffar, ljuset position, quad normal, samma beräkningar som assigment 1, använd worldpos
    float4 finalColor = float4(0.2, 0.2, 0.2, 1.0f) * float4(s, 1.0f);
    float4 finalColor1 = float4(0.2, 0.2, 0.2, 1.0f);
	//float3 lightColor = (1.0f, 1.0f,1.0f);
    float3 lightPosition = (0.0f, 0.0f, -2.0f);
    float3 lightDirection = (lightPosition - input.worldPos.xyz);
    float diffuse = (dot(normalize(lightDirection), (input.Normal)));
    diffuse = max(0.0f, diffuse);
	//diffuse = 0.5;
    float3 Color = s * diffuse  * (1.0 / (length(lightDirection)));

    finalColor += float4(Color.xyz, 0.0f); // finalColor;
    finalColor = min(finalColor, float4(1.0f, 1.0f, 1.0f, 1.0f));
    return float4(s, 1.0f);
    //return float4(diffuse, diffuse, diffuse,1.0f);

    //// Specular light        // Phong shading
    //float3 viewDir = normalize(input.camPos - input.worldPos.xyz);

 
    ////float h = normalize(lightDirection + viewDir);
    ////float NdotH = dot(input.Normal, h);
    ////if (NdotH < 0.0f)
    ////{
    ////    NdotH = 0.0f;
    ////}
    ////float intensity = pow(saturate(NdotH), 100);
    ////float3 specular = specular_color.xyz * NdotH /** intensity * (1.0 / length(lightDir))*/;
    ////float3 reflection = normalize(2 * 1.0f * normalize(input.Normal) - lightDirection);
    ////float3 specular = pow(saturate(dot(reflection, viewDir)), 10) /* * NdotH*/;


   // return finalColor + float4(Specular.xyz, 0.0f);
};
