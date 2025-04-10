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
    float4 outPosition : SV_POSITION;
    float2 outTextCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    //float3 outWorldPosition : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.outPosition = mul(float4(input.inPosition, 1.0f), mul(mul(world, view), projection));
    output.outTextCoord = input.inTextCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0), world));
    //output.outWorldPosition = mul(float4(input.inPosition, 1.0f), world);
    //output.outLightPosition = mul(float4(input.inLightPosition, 1.0f), mul(mul(lightWorld, lightView), lightProjection));
    return output;
}