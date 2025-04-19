cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

cbuffer lightMatrixBuffer : register(b1)
{
    matrix lightWorld;
    matrix lightView;
    matrix lightProjection;
}

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTextCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION; // clip space pos
    float2 outTextCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.outPosition = mul(float4(input.inPosition, 1.0f), mul(mul(world, view), projection));
    output.outTextCoord = input.inTextCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0), world));
    output.lightViewPosition = mul(mul(mul(float4(input.inPosition, 1.0f), world), lightView), lightProjection);

    return output;
}