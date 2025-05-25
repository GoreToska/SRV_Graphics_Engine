#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3
#endif

cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 lightColor;
    float lightStrenght;
    float3 lightDirection;
    float3 lightPosition;
    int lightType;
    float lightAngle;
}

cbuffer cascadeBuffer : register(b1)
{
    float4x4 cascadeViewProjection[CASCADE_COUNT];
    float4 distances;
    float4 cameraPosition;
};

cbuffer perObjectBuffer : register(b2)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
}

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 globalPos : POSITION1;
};

Texture2D decalTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2DArray shadowMap : SHADOWMAP : register(t1);
SamplerComparisonState shadowSampler : SHADOWSAMPLER : register(s1);

Texture2D<float4> depthTex : register(t2);
Texture2D<float4> normalTex : register(t3);
Texture2D<float4> diffuseTex : register(t4);
Texture2D<float4> specularTex : register(t5);
Texture2D<float4> nonLinearDepthTex : register(t6);

float CalculateShadow(float depth, float3 worldPosition)
{
    
    float3 color_mult = float3(0, 0, 0);
    int layer = 3;

    for (int i = 0; i < 4; i++)
    {
        if (depth < distances[i])
        {
            layer = i;
            break;
        }
    }
    
    if (layer == 0)
    {
        color_mult = float3(1, 0, 0);

    }
    else if (layer == 1)
    {
        color_mult = float3(0, 1, 0);
    }
    else if (layer == 2)
    {
        color_mult = float3(0, 0, 1);
    }
    else
    {
        color_mult = float3(0, 0, 1);
    }
    
    float4 lightSpacePos = mul(float4(worldPosition, 1.0f), cascadeViewProjection[layer]);
    
    lightSpacePos.xyz /= lightSpacePos.w;

    float2 shadowUV = float2(lightSpacePos.x, -lightSpacePos.y) * 0.5f + 0.5f;
    if (shadowUV.x < 0 || shadowUV.x > 1 || shadowUV.y < 0 || shadowUV.y > 1)
        return 1.0f;
    
    float bias = 0.0005f;
    depth = lightSpacePos.z;
    float shadow = shadowMap.SampleCmpLevelZero(shadowSampler, float3(shadowUV, layer), depth - bias);

    //if(shadow == 1)
   //     return (1,1,1);
    
   // return float3(color_mult);
    
    const int filterSize = 3;
    float summ_shadow = 0.0;
    float totalSamples = 0.0;
    float shadowMapResolution = 4096;

    for (int x = -filterSize; x <= filterSize; ++x)
    {
        for (int y = -filterSize; y <= filterSize; ++y)
        {
            float2 offset = float2(x, y) / shadowMapResolution;
            float2 sampleCoords = shadowUV.xy + offset;

            if (sampleCoords.x >= 0.0 && sampleCoords.x <= 1.0 &&
                sampleCoords.y >= 0.0 && sampleCoords.y <= 1.0)
            {
                summ_shadow += shadowMap.SampleCmpLevelZero(shadowSampler, float3(sampleCoords, layer), depth - bias);
                totalSamples += 1.0;
            }
            else
            {
                summ_shadow += 1;
                totalSamples += 1.0;
            }
        }
    }
    
    return /*color_mult*/summ_shadow / totalSamples < 0.3f ? 0.3 : summ_shadow / totalSamples;
}

float4 main(PS_IN input) : SV_Target
{
    // TODO: get this values from const buffer
    // ---------------------------------
    const float client_width = 1024.0f;
    const float client_height = 768.0f;
    // ---------------------------------
    
    float depth = depthTex.Load(int3(input.pos.xy, 0));
    float3 diffuse = diffuseTex.Load(int3(input.pos.xy, 0)).xyz;
    float4 specular = specularTex.Load(int3(input.pos.xy, 0));
    float3 normal = normalTex.Load(int3(input.pos.xy, 0)).xyz;
    float nonlinearDepth = nonLinearDepthTex.Load(int3(input.pos.xy, 0));
    
    float3 lightDir = 0;
    float shadow = 1.0;
    float ndcX = input.pos.x / client_width * 2.0f - 1.0f;
    float ndcY = -(input.pos.y / client_height * 2.0f - 1.0f);
    float4 viewSpaceVertPos = mul(float4(ndcX, ndcY, nonlinearDepth, 1.0f), inverseProjection);
    viewSpaceVertPos /= viewSpaceVertPos.w;
    float4 globalSpaceVertPos = mul(viewSpaceVertPos, inverseView);
    globalSpaceVertPos /= globalSpaceVertPos.w;
    float3 globalVertPos = globalSpaceVertPos.xyz;
    float shine = 1.0f;
    float shineDistance = 100.0f;
    
    [branch]
    if (lightType == DIRECTIONAL_LIGHT)
    {
        lightDir = -normalize(lightDirection);
        shadow = CalculateShadow(depth, globalVertPos);
    }
    else if (lightType == POINT_LIGHT || lightType == SPOT_LIGHT)
    {
        lightDir = normalize(lightPosition.xyz - globalVertPos);
        shine = saturate(1 - length(lightPosition.xyz - globalVertPos) / shineDistance);
    }
    
    if (lightType == SPOT_LIGHT && dot(lightDirection, -lightDir) <= cos(lightAngle))
    {
        return float4(0.0, 0.0, 0.0, 0.0);
    }
    
    float3 ambient = ambientLightColor * ambientLightStrenght;
    float3 diffuseFactor = saturate(dot(normal, lightDir));
    diffuseFactor *= lightColor * lightStrenght * diffuseFactor;
    
    float decal = decalTexture.Sample(objSamplerState, globalVertPos.xz / 10);
    float4 finalColor = 0;
    
    float3 cameraViewAngle = normalize(cameraPosition.xyz - globalVertPos);
    normal = normalize(normal);
    float3 shineRadius = normalize(2 * dot(lightDir, normal) * normal - lightDir);
    float3 spec = 0;
    
    if (lightType == POINT_LIGHT || lightType == SPOT_LIGHT)
        spec = shine * lightColor * saturate(specular.xyz * lightStrenght * pow(dot(cameraViewAngle, shineRadius), 200));
    
    float3 dif = shine * lightColor * diffuse * lightStrenght * saturate(dot(lightDir, normal));
    
    if (shadow != 1)
        finalColor = float4(spec + dif, 1.0) * shadow * decal;
    else
        finalColor = float4(spec + dif, 1.0) * shadow;
    
    return finalColor;
}