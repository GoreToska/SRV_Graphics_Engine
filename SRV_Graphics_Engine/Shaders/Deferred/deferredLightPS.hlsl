const int DIRECTIONAL_LIGHT = 1;
const int POINT_LIGHT = 2;
const int SPOT_LIGHT = 3;

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

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 tex : TEXCOORD0;
};

Texture2D decalTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);
Texture2DArray shadowMap : SHADOWMAP : register(t1);
SamplerComparisonState shadowSampler : SHADOWSAMPLER : register(s1);

Texture2D<float4> depthTex : register(t2);
Texture2D<float4> normalTex : register(t3);
Texture2D<float4> diffuseTex : register(t4);
Texture2D<float4> specularTex : register(t5);

float4 main(PS_IN input) : SV_Target
{
    float3 normal = normalTex.Load(int3(input.pos.xy, 0)).xyz;
    float3 diffuse = diffuseTex.Load(int3(input.pos.xy, 0)).xyz;
    float4 specular = specularTex.Load(int3(input.pos.xy, 0));
    
    float3 lightDir = -normalize(dynamicLightDirection);
    float3 ambient = ambientLightColor * ambientLightStrenght;
    float diffuseFactor = saturate(dot(normal, lightDir));
    diffuse = dynamicLightColor * dynamicLightStrenght * diffuseFactor;
    
    //float shadow = CalculateShadow(input.globalPosition.xyz);
    //float decal = decalTexture.Sample(objSamplerState, input.tex.xy * 10) * 0.5;
    
    //float3 sampleColor = objTexture.Sample(objSamplerState, input.tex.xy);
    
    //float3 finalColor = sampleColor * (ambient + diffuse * shadow);
    float3 finalColor;
    
    //if (shadow != 1)
    //    finalColor = sampleColor * (ambient + diffuse * decal * shadow);
    //else
    finalColor = ambient + diffuse;

      
    return float4(saturate(finalColor), 1.0);
}