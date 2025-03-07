cbuffer mycBuffer : register(b0)
{
    matrix mat;
};

struct VS_INPUT
{
    float3 inPosition : POSITION;
    float3 inColor : COLOR;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float3 outColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPosition, 1.0f), mat);
    output.outColor = input.inColor;
    
    return output;
}

