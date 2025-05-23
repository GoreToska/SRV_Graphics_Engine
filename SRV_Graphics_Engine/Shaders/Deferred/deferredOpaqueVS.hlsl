struct VS_IN
{
    float4 pos : POSITION0;
    float4 norm : NORMAL;
    float4 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 tex : TEXCOORD0;
    float3 norm : NORMAL;
};

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;

    output.pos = mul(float4(input.pos.xyz, 1.0f), mul(mul(world, view), projection));
    output.viewPos = mul(mul(float4(input.pos.xyz, 1), world), view);
    output.tex.xy = input.tex.xy;
    output.norm = normalize(mul(float4(input.norm.xyz, 0), world));
    
    return output;
}