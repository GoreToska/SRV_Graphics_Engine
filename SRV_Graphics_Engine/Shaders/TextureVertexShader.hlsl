cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inLightPosition : LIGHT_POSITION;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION; // clip space pos
    float2 outTextCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLDPOS; // world pos
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.outPosition = mul(float4(input.inPosition, 1.0f), mul(mul(world, view), projection));
    output.outTextCoord = input.inTextCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0), world));
    output.outWorldPos = mul(float4(input.inPosition, 1.0f), world).xyz;

    return output;
}