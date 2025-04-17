cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightDirection;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
};


Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2D shadowMap : SHADOWMAP : register(t1);
SamplerState shadowSampler : SHADOWSAMPLER : register(s1);


float CalculateShadow(float3 lightDir, float4 lightViewPosition, float3 normal)
{
    float3 projCoords = lightViewPosition.xyz / lightViewPosition.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;
    projCoords.y = 1.0 - projCoords.y;
    
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
       projCoords.y < 0.0 || projCoords.y > 1.0 ||
       projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 1.0; 
    }
    
    float shadowMapDepth = shadowMap.Sample(shadowSampler, projCoords.xy).r;
    
    float currentDepth = projCoords.z;
    
    float bias = 0.0001;
    
    return (currentDepth - bias) <= shadowMapDepth ? 1.0 : 0.3;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 normal = normalize(input.inNormal);
    float3 lightDir = normalize(-dynamicLightDirection);
    
    float3 ambient = ambientLightColor * ambientLightStrenght;
    float diffuseFactor = saturate(dot(normal, lightDir));
    float3 diffuse = dynamicLightColor * dynamicLightStrenght * diffuseFactor;
    
    float shadow = CalculateShadow(lightDir, input.lightViewPosition, normal);
    
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    float3 finalColor = sampleColor * (ambient + diffuse * shadow);
    
    return float4(saturate(finalColor), 1.0);
}
