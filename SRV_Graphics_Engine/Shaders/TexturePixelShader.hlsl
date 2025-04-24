#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 dynamicLightColor;
    float dynamicLightStrenght;
    float3 dynamicLightDirection;
}

cbuffer cascadeBuffer : register(b1)
{
    float4x4 cascadeViewProjection[CASCADE_COUNT];
    float4 distances;
};

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

float shadowMapResolution = 1024;


float CalculateShadow(float4 worldSpacePosition, float4 lightViewPosition, float3 normal)
{
    float depthValue = abs(lightViewPosition.z);

    int layer = -1;
    for (int i = 0; i < CASCADE_COUNT; ++i)
    {
        if (depthValue < distances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = CASCADE_COUNT;
    }
    
    float4 posLightSpace = mul(float4(worldSpacePosition.xyz, 1.0), cascadeViewProjection[layer]);
    float3 projCoords = posLightSpace.xyz / posLightSpace.w;
    projCoords = (mul(float4(projCoords, 1.0f), gT)).xyz;

    
    return 0;
    
    
   /* float3 projCoords = lightViewPosition.xyz / lightViewPosition.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;
    projCoords.y = 1.0 - projCoords.y;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0 ||
        projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 1.0;
    }

    float currentDepth = projCoords.z;
    
    const int filterSize = 3; 
    float shadow = 0.0;
    float totalSamples = 0.0;

    for (int x = -filterSize; x <= filterSize; ++x)
    {
        for (int y = -filterSize; y <= filterSize; ++y)
        {
            float2 offset = float2(x, y) / 2048.0f;
            float2 sampleCoords = projCoords.xy + offset;

            if (sampleCoords.x >= 0.0 && sampleCoords.x <= 1.0 &&
                sampleCoords.y >= 0.0 && sampleCoords.y <= 1.0)
            {
                float shadowMapDepth = shadowMap.Sample(shadowSampler, sampleCoords).r;
                shadow += (currentDepth - 0.001) <= shadowMapDepth ? 1.0 : 0.0;
                totalSamples += 1.0;
            }
        }
    }

    return shadow / totalSamples;*/
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 normal = normalize(input.inNormal);
    float3 lightDir = normalize(dynamicLightDirection);    
    
    float3 ambient = ambientLightColor * ambientLightStrenght;
    float diffuseFactor = saturate(dot(normal, lightDir));
    float3 diffuse = dynamicLightColor * dynamicLightStrenght * diffuseFactor;
    
    float shadow = CalculateShadow(input.inPosition, input.lightViewPosition, normal);
    
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    float3 finalColor = sampleColor * (ambient + diffuse * shadow);
    
    return float4(saturate(finalColor), 1.0);
}
