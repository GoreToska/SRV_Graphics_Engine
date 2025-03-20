cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightPosition;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 inWorldPosition : WORLD_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = input.color;
    //float3 sampleColor = input.normal; // <- debug normals

    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPosition);
    
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.normal), 0);
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrenght * dynamicLightColor;
    
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * appliedLight;
    //float3 finalColor = appliedLight;
   
    return float4(finalColor, 1.0f);
}