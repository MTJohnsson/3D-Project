Texture2D	textureTexture: register(t2);
Texture2D	textureNormal: register(t3);
Texture2D	texturePos: register(t4);

SamplerState textureSampler;

struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TextureCoord : TEXCOORD;
	float4 worldPos : WORLDPOS;
	float3 camPos : CAMPOS;
};

struct PixelOut
{
	float4 colour : SV_Target;
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
cbuffer DISPLAY : register(b1)
{
	float display;
	float3 padding;
}

float SpotLight(float3 pos, float3 worldPos)
{
	float intensity = 0;
	float3 dir = normalize(pos);
	float3 vectorToLight = normalize(worldPos - pos);
	float value = dot(dir, vectorToLight);

	//the vertex lies beyond the outer
	if (value <= 0.5)
	{ //no light
		intensity = 0;
	}
	else
	{
		if (value <= 0.2)
		{
			float lf = (cos(value) - cos(0.5 / 2)) /
				(cos(0.25 / 2) - cos(0.5 / 2));
			float falloff = 0.6;
			intensity = lf * falloff;


		}
		else
			intensity = 1;
	}

	return intensity;

}

PixelOut main(GS_OUT input)
{
	PixelOut psOut;
	float3 normal = textureNormal.Sample(textureSampler, input.TextureCoord).rgb;
	float3 diffuse = textureTexture.Sample(textureSampler, input.TextureCoord).rgb;
	float3 position = texturePos.Sample(textureSampler, input.TextureCoord).rgb;
	psOut.colour = (float4)(diffuse, 1.0f);
	float4 totalLights;
	/*for (int i = 0; i < 2; i++)
	{
		float3 ambientLight = light[i].AmbientColor * light[i].AmbientLight;
		float3 appliedLight = ambientLight;
		float intensity = SpotLight(light[i].position, input.worldPos);
		float3 diffuseLight = intensity * light[i].Strength*  light[i].LightColor;
		appliedLight += diffuseLight;
		totalLights += float4(saturate(appliedLight), 1.0);
	}*/

	float3 specular = float3(0.f, 0.f, 0.f);

	float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f); // ambiant light
	float4 final_color = float4((1.0f, 1.0f, 1.0f) * ambient.xyz, 1);

	for (int i = 0; i < 2; i++)
	{
		/*//float3 ambientLight = light[i].AmbientColor * light[i].AmbientLight;
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
		//specular += float3(0.1f, 0.1f, 0.1f) * t * 0.5f * (1.0 / length(normalize(light[i].position - input.worldPos)));*/

		// Without normal mapping
		float3 lightDir = normalize(light[i].position - position);
		float diffuseFactor = dot(lightDir, normal);

		if (diffuseFactor < 0.0f)
		{
			diffuseFactor = 0.0f;
		}

		float3 color = light[i].LightColor * diffuseFactor * light[i].Strength * (1.0f / length(lightDir));

		final_color += float4(color, 1.0f);
		final_color = min(final_color, float4(1.0f, 1.0f, 1.0f, 1.0f));


		// Specular light / Phong shading
		float3 viewDir = normalize(input.camPos - position);
		float3 ref = pow(max((2.25f * (normalize(light[i].position - position) * normal)) * normal - normalize(light[i].position - position), 0), 10.f);
		float t = dot(ref, normal);
		if (t < 0.0f)
		{
			t = 0.0f;
		}
		specular += float3(0.1f, 0.1f, 0.1f) * t * 0.5f * (1.0f / length(normalize(light[i].position - position)));
		
	}

	//psOut.colour = float4(diffuse, 1.0f);
	//psOut.colour = float4(normal, 1.0f);
	switch (display)
	{
	case 0: //All buffers
		psOut.colour = float4(diffuse * final_color.xyz + specular, 1.0f);
		break;
	case 1: //Normal buffer only
		psOut.colour = float4(normal, 1.0f);
		break;
	case 2: //Pos buffer only
		float depth = texturePos.Sample(textureSampler, input.TextureCoord).b;
		psOut.colour = float4(depth, depth, depth, 1.0f);
		break;
	case 3: //Texture buffer only
		psOut.colour = float4(diffuse + specular, 1.0f);
		break;
	case 4: //Unused. (Could use for normal mapping)
		psOut.colour = float4(normal, 1.0f);
		break;
	}


	return psOut;
};