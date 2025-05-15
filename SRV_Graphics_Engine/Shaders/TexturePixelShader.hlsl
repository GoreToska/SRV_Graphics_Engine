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
    float4 cameraPosition;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 textCoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 globalPosition : POSITION1;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2DArray shadowMap : SHADOWMAP : register(t1);
SamplerComparisonState shadowSampler : SHADOWSAMPLER : register(s1);

float shadowMapResolution = 1024;


float CalculateShadow(float3 worldPosition)
{
    float depthFromCamera = length(cameraPosition.xyz - worldPosition);
    int layer = 3;

    for (int i = 0; i < 4; i++)
    {
        if (depthFromCamera < distances[i])
        {
            layer = i;
            break;
        }
    }
    
    float4 lightSpacePos = mul(float4(worldPosition, 1.0f), cascadeViewProjection[layer]);
    
    lightSpacePos.xyz /= lightSpacePos.w;

    float2 shadowUV = float2(lightSpacePos.x, -lightSpacePos.y) * 0.5f + 0.5f;
    if (shadowUV.x < 0 || shadowUV.x > 1 || shadowUV.y < 0 || shadowUV.y > 1)
        return 1.0f;
    
    float depth = lightSpacePos.z;
    float bias = 0.00001f;

    float shadow = shadowMap.SampleCmpLevelZero(shadowSampler, float3(shadowUV, layer), depth - bias);

    return shadow;
    
   
    
    
    /*float3 projCoords = lightViewPosition.xyz / lightViewPosition.w;
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
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(dynamicLightDirection);    
    
    float3 ambient = ambientLightColor * ambientLightStrenght;
    float diffuseFactor = saturate(dot(normal, lightDir));
    float3 diffuse = dynamicLightColor * dynamicLightStrenght * diffuseFactor;
    
    float shadow = CalculateShadow(input.globalPosition.xyz);
    
    float3 sampleColor = objTexture.Sample(objSamplerState, input.textCoord);
    
    
    float depthFromCamera = length(cameraPosition.xyz - input.globalPosition);
    int layer;
    float3 colorMult;
    
    for (int i = 0; i < 4; i++)
    {
        if (depthFromCamera < distances[i])
        {
            layer = i;
            break;
        }
    }
    
    if(layer == 0)
    {
        colorMult = float3(1,0,0);
    }
    if(layer == 1)
    {
        colorMult = float3(0,1,0);   
    }
    if(layer == 2)
    {
        colorMult = float3(0,0,1);   
    }
    if(layer == 3)
    {
        colorMult = float3(0,1,1);   
    }
    
    //shadow *= colorMult;
    
    float3 finalColor = sampleColor * (ambient + diffuse * shadow);
    
    return float4(saturate(finalColor /** colorMult*/), 1.0);
}
