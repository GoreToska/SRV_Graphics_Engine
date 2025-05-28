#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#define DIRECTIONAL_LIGHT 1
#define SPOT_LIGHT 2
#define POINT_LIGHT 3
#endif

cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix inverseView;
    matrix inverseProjection;
};

cbuffer lightBuffer : register(b1)
{
    float3 ambientLightColor;
    float ambientLightStrenght;
    float3 lightColor;
    float lightStrenght;
    float3 lightDirection;
    float3 lightPosition;
    int lightType;
    float lightAngle;
}

struct VS_IN
{
    float4 pos : SV_POSITION;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 viewPos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 globalPos : POSITION1;
};

PS_IN main(VS_IN input, uint id : SV_VertexID)
{
    PS_IN output = (PS_IN) 0;
    
    // todo: add aabb 

    if (lightType == DIRECTIONAL_LIGHT)
    {
        output.tex.xy = float2(id & 1, (id & 2) >> 1);
        output.pos = float4(output.tex.xy * float2(2, -2) + float2(-1, 1), 0, 1);
    }
    else if (lightType == SPOT_LIGHT || lightType == POINT_LIGHT)
    {
        output.pos = mul(float4(input.pos.xyz, 1), view);
        output.pos = mul(output.pos, projection);
        //output.tex.xy = float2(id & 1, (id & 2) >> 1);
        //output.pos = float4(output.tex.xy * float2(2, -2) + float2(-1, 1), 0, 1);
    }
    
    return output;
}