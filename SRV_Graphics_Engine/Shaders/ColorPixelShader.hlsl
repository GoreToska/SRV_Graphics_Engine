cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
}

struct PS_INPUT
{
    float4 position : SV_POSITION; 
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = input.color;
    
    float3 ambientLight = ambientLightColor * ambientLightStrenght;
    
    float3 finalColor = sampleColor * ambientLight;
    
    return float4(finalColor, 1.0f);
    }