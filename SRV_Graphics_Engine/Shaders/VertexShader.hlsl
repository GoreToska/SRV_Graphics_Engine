cbuffer mycBuffer : register(b0)
{
    matrix mat;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float2 inTextCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTextCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPosition, 1.0f), mat);
    output.outTextCoord = input.inTextCoord;
    
    return output;
}

