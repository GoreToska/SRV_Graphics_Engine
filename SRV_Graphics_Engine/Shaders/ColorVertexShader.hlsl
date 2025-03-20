cbuffer mycBuffer : register(b0)
{
    matrix viewProjection;
    matrix world;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

PS_INPUT main(VS_INPUT input)
{
    //PS_INPUT output;
    //output.position = mul(float4(input.position, 1.0f), mat);
    //output.color = input.color;
    //return output;
	
    //output.outPosition = mul(float4(input.inPosition, 1.0f), viewProjection);
    //output.outTextCoord = input.inTextCoord;
    //output.outNormal = normalize(mul(float4(input.inNormal, 0), world));
    //output.outWorldPosition = mul(float4(input.inPosition, 1.0f), world);
    
    PS_INPUT output;

    float amplitude = 1.5f;
    float frequency = 0.5f;
    float yOffset = amplitude * sin(input.position.x * frequency) * cos(input.position.z * frequency);
    float delta = 0.01f;
    float yOffset1 = amplitude * sin((input.position.x + delta) * frequency) * cos((input.position.z) * frequency);
    float yOffset2 = amplitude * sin((input.position.x) * frequency) * cos((input.position.z + delta) * frequency);
    
    float3 modifiedPosition = input.position;
    modifiedPosition.y += yOffset;
    
    float3 firstVector;
    firstVector.x = delta;
    firstVector.y = yOffset1- yOffset;
    firstVector.z = 0;
    
    float3 secondVector;
    secondVector.x = 0;
    secondVector.y = yOffset2 - yOffset;
    secondVector.z = delta;
   

    output.position = mul(float4(modifiedPosition, 1.0f), viewProjection);
    output.color = input.color;
    
    float3 normal = cross(secondVector, firstVector);
    normal = normalize(normal);
    output.normal = normalize(mul(float4(normal, 0), world));

    return output;
}