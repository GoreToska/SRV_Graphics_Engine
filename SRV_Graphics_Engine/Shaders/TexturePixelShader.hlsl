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
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPosition : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2D shadowMap : SHADOWMAP : register(t1);
SamplerState shadowSampler : SHADOWSAMPLER : register(s1);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 shadowCoord = input.inPosition / input.inPosition.w;
    shadowCoord = shadowCoord * 0.5 + 0.5; // from [-1, 1] to [0, 1]
    float shadowDepth = shadowMap.Sample(shadowSampler, shadowCoord.xy).r;

    float visibility = (shadowCoord.z < shadowDepth) ? 0.2 : 1.0; 
    
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    //float3 sampleColor = input.inNormal; //<- debug normals
    
    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    float3 appliedLight = ambientLight;
    
    //float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPosition);
    
    float3 diffuseLightIntensity = saturate(dot(dynamicLightDirection, input.inNormal));
    
    // float distanceToLight = distance(dynamicLightPosition, input.inWorldPosition);
    //float3 attenuation = 1 / (dynamicLightAttenuation_const + dynamicLightAttenuation_linear * distanceToLight
    //                        + dynamicLightAttenuation_exponent * pow(distanceToLight, 2));
    
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrenght * dynamicLightColor;
   // diffuseLight *= attenuation;
    appliedLight += diffuseLight;
           
    float3 finalColor = sampleColor * appliedLight;
    
    return float4(finalColor * visibility, 1.0);
}
