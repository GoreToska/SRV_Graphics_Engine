cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTextCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTextCoord);
    
    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    float3 finalColor = sampleColor * ambientLight;
    
    return float4(finalColor, 1.0f);
}
