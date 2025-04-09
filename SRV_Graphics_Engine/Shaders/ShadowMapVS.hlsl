cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float4 position : POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXTURE0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.position = mul(float4(input.position.xyz, 1.0), mul(mul(world, view), projection));
    output.depthPosition = output.position; 
    return output;
}