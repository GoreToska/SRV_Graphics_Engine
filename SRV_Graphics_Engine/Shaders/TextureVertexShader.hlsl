cbuffer perObjectBuffer : register(b0)
{
    matrix viewProjection;
    matrix world;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTextCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPosition : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.outPosition = mul(float4(input.inPosition, 1.0f), viewProjection);
    output.outTextCoord = input.inTextCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0), world));
    output.outWorldPosition = mul(float4(input.inPosition, 1.0f), world);
    
    return output;
}

