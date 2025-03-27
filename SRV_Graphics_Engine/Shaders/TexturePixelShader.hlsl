cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightPosition;
    float dynamicLightAttenuation_const;
    float dynamicLightAttenuation_linear;
    float dynamicLightAttenuation_exponent;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPosition : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    //float3 sampleColor = input.inNormal; //<- debug normals
    
    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    
    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPosition);
    
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);
    
    float distanceToLight = distance(dynamicLightPosition, input.inWorldPosition);
    float3 attenuation = 1 / (dynamicLightAttenuation_const + dynamicLightAttenuation_linear * distanceToLight
                             + dynamicLightAttenuation_exponent * pow(distanceToLight, 2));
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrenght * dynamicLightColor;
    diffuseLight *= attenuation;
    appliedLight += diffuseLight;
        
    appliedLight += diffuseLight;
    
    float3 finalColor = sampleColor * appliedLight;
    
    return float4(finalColor, 1.0f);
}
