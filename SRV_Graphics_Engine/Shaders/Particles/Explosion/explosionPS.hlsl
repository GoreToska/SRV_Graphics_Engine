Texture2D objTexture : register(t0);
SamplerState objSampler : register(s0);

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(PSInput input) : SV_TARGET
{
	return input.color * objTexture.Sample(objSampler, input.uv);
	//return input.color;
}