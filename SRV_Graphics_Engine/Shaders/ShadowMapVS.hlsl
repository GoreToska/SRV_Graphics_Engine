cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float4 inPosition : POSITION0;
};

struct VS_OUTPUT
{
    float4 outPosition : POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.outPosition = mul(input.inPosition, world);
    
    return output;
}