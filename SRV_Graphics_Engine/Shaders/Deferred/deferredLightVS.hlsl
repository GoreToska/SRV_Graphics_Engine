#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 3
#endif

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

    output.tex.xy = float2(id & 1, (id & 2) >> 1);
    output.pos = float4(output.tex.xy * float2(2, -2) + float2(-1, 1), 0, 1);
    
    return output;
}