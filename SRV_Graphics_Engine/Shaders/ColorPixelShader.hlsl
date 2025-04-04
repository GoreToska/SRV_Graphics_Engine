cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightDirection;
    float dynamicLightAttenuation_const;
    float dynamicLightAttenuation_linear;
    float dynamicLightAttenuation_exponent;
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
    
    float3 vectorToLight = dynamicLightDirection - input.inWorldPosition;
    
    float3 diffuseLightIntensity = saturate(dot(dynamicLightDirection, input.normal));
    //float distanceToLight = distance(dynamicLightPosition, input.inWorldPosition);
    //float3 attenuation = 1 / (dynamicLightAttenuation_const + dynamicLightAttenuation_linear * distanceToLight
    //                         + dynamicLightAttenuation_exponent * pow(distanceToLight, 2));
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrenght * dynamicLightColor;
    //diffuseLight *= attenuation;
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * appliedLight;
   
    return float4(finalColor, 1.0f);
}