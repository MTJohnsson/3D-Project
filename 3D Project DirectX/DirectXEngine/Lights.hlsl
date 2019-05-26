
struct LIGHTTYPE
{
    int POINTLIGHT;
    int SPOTLIGHT;
};
float SpotLight(float3 pos, float3 worldPos);
float PointLight(float3 normal, float3 lightPos, float3 worldPos);

float CalculateLight(int lightType,float3 lightPos,float3 worldPos,float3 normal)
{
    LIGHTTYPE t;
    t.POINTLIGHT = 0;
    t.SPOTLIGHT = 1;
 
    if (lightType == t.SPOTLIGHT)
    {
      return  SpotLight(lightPos, worldPos);
    }
    if (lightType == t.POINTLIGHT)
    {
        return PointLight(normal, lightPos, worldPos);
    }
    return 0;

}
float PointLight(float3 normal, float3 lightPos, float3 worldPos)
{
    float3 dir = (float3(0, -1, 0));
 // Invert the light direction for calculations.
    float3 lightDir = -dir;
    float3 posToLightDir = normalize(lightPos - worldPos); // same as inverted lightDir in your case
  //  posToLightDir = normalize(lightPos);

    float lightIntensity = saturate(dot(normal, lightDir));

// here normalize(lightPos) would be same as -lightDir


    //float lightIntensity = saturate(dot(normal, -lightDir));
    lightIntensity = max(0.0f, lightIntensity);
    return lightIntensity;


    //// Diffuse shading
    //float3 lightDir = normalize(lightPos - worldPos);
    //float diffuseFactor = dot(lightDir, normal);

    //if (diffuseFactor < 0.0f)
    //{
    //    diffuseFactor = 0.0f;
    //}

    //float3 diffuse = diffuseFactor * 1.0f * (1.0 / length(lightDir));
    //return diffuse;
}



float SpotLight(float3 pos, float3 worldPos)
{
    float intensity = 0;
    float3 dir = normalize(float3(0, -1, 0));
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