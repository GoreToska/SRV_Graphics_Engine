
cbuffer cascadeBuffer : register(b0)
{
    float4x4 cascadeViewProjection[4];
	float4 distances;
    float4 cameraPosition;
};

struct GS_IN
{
	float4 position : POSITION;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	uint arrayIndex : SV_RenderTargetArrayIndex;
};

[instance(4)]
[maxvertexcount(3)]
void main(triangle GS_IN p[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_OUT> stream)
{
	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		GS_OUT gs = (GS_OUT) 0;
        gs.position = mul(float4(p[i].position.xyz, 1.0f), cascadeViewProjection[id]);
        gs.arrayIndex = id;
		stream.Append(gs);
	}
}

