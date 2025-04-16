cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightDirection;
    
    // todo: �������� ���� � ������ �����
    matrix lightWorld;
    matrix lightView;
    matrix lightProjection;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLDPOS;
    //float4 lightViewPosition : TEXCOORD1;
    //float3 lightPosition : TEXCOORD2;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2D shadowMap : SHADOWMAP : register(t1);
SamplerState shadowSampler : SHADOWSAMPLER : register(s1);

float4 main(PS_INPUT input) : SV_TARGET
{    
    float4 lightViewPosition = mul(float4(input.inWorldPos, 1.0), lightWorld);
    lightViewPosition = mul(lightView, lightViewPosition);
    lightViewPosition = mul(lightProjection, lightViewPosition);

    float3 projCoords = lightViewPosition.xyz / lightViewPosition.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;
    projCoords.y = 1.0 - projCoords.y;
    
    float closestDepth = shadowMap.Sample(shadowSampler, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = (currentDepth > closestDepth + 0.001f) ? 0.3f : 1.0f;
    
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    float3 diffuseLight = saturate(dot(dynamicLightDirection, input.inNormal)) * dynamicLightColor * dynamicLightStrenght;
    
    diffuseLight *= shadow;

    //float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    //float3 sampleColor = input.inNormal; //<- debug normals
    
    //float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    //float3 appliedLight = ambientLight;
    
    //float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPosition);
    
    //float3 diffuseLightIntensity = saturate(dot(dynamicLightDirection, input.inNormal));
    
    // float distanceToLight = distance(dynamicLightPosition, input.inWorldPosition);
    //float3 attenuation = 1 / (dynamicLightAttenuation_const + dynamicLightAttenuation_linear * distanceToLight
    //                        + dynamicLightAttenuation_exponent * pow(distanceToLight, 2));
    
    //float3 diffuseLight = diffuseLightIntensity * dynamicLightStrenght * dynamicLightColor;
   // diffuseLight *= attenuation;
    //appliedLight += diffuseLight;
    float3 finalColor = sampleColor * (ambientLight + diffuseLight);
     
//    float3 finalColor = sampleColor * appliedLight;
    
    return float4(finalColor /* * visibility*/, 1.0);
}
