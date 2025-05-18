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
    float3 position : POSITION;
    float2 textCoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 textCoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 globalPosition : POSITION1;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.position = mul(float4(input.position, 1.0f), mul(mul(world, view), projection));
    output.textCoord = input.textCoord;
    output.normal = normalize(mul(float4(input.normal, 0), world));
    output.globalPosition = mul(float4(input.position, 1.0f), world);

    return output;
}