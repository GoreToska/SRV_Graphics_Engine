cbuffer perObjectBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float3 inLightPosition : LIGHT_POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 outWorldPosition : WORLD_POSITION;
};

Texture2D shadowMap : SHADOWMAP : register(t1);
SamplerState shadowSampler : SHADOWSAMPLER : register(s1);

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.position = mul(float4(input.position, 1.0f), mul(mul(world, view), projection));
    output.color = input.color;
    
    float delta = 0.1;
    float3 first;
    first.x = delta;
    first.y = 0;
    first.z = 0;
    float3 second;
    second.x = 0;
    second.y = 0;
    second.z = delta;
    float3 normal = normalize(cross(second, first));
    output.normal = normalize(mul(float4(normal, 0), mul(mul(world, view), projection)));

    return output;
	

    
    //PS_INPUT output;

    //float amplitude = 1.5f;
    //float frequency = 0.5f;
    //float yOffset = amplitude * sin(input.position.x * frequency) * cos(input.position.z * frequency);
    //float delta = 0.0001f;
    //float yOffset1 = amplitude * sin((input.position.x + delta) * frequency) * cos((input.position.z) * frequency);
    //float yOffset2 = amplitude * sin((input.position.x) * frequency) * cos((input.position.z + delta) * frequency);
    
    //float3 modifiedPosition = input.position;
    //modifiedPosition.y += yOffset;
    
    //float3 firstVector;
    //firstVector.x = delta;
    //firstVector.y = yOffset1- yOffset;
    //firstVector.z = 0;
    
    //float3 secondVector;
    //secondVector.x = 0;
    //secondVector.y = yOffset2 - yOffset;
    //secondVector.z = delta;
   

    //output.position = mul(float4(modifiedPosition, 1.0f), mul(mul(world, view), projection));
    //output.color = input.color;
    
    //float3 normal = cross(secondVector, firstVector);
    //normal = normalize(normal);
    //output.normal = normalize(mul(float4(normal, 0), world));

    //output.outWorldPosition = mul(float4(input.position, 1.0f), world);
    
    //return output;
}