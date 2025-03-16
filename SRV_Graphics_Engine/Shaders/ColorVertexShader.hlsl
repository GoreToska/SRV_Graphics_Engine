cbuffer mycBuffer : register(b0)
{
    matrix mat;
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
};

PS_INPUT main(VS_INPUT input)
{
    //PS_INPUT output;
    //output.position = mul(float4(input.position, 1.0f), mat);
    //output.color = input.color;
    //return output;
	
    PS_INPUT output;

    // Изгиб по синусу и косинусу
    float amplitude = 1.5f; // Амплитуда изгиба
    float frequency = 0.5f; // Частота изгиба
    float yOffset = amplitude * sin(input.position.x * frequency) * cos(input.position.z * frequency);

    // Применяем изгиб к координате y
    float3 modifiedPosition = input.position;
    modifiedPosition.y += yOffset;

    // Преобразуем координаты с использованием матрицы
    output.position = mul(float4(modifiedPosition, 1.0f), mat);

    // Передаём цвет
    output.color = input.color;

    return output;
}