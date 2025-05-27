struct VSInput
{
	uint vertexID : SV_VertexID;
};

struct SortListStruct
{
	uint index;
	float distanceSq;
};

StructuredBuffer<SortListStruct> sortListBuffer : register(t0);

struct VSOutput
{
	uint particleIndex : INDEX;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.particleIndex = sortListBuffer[input.vertexID].index;
	return output;
}
