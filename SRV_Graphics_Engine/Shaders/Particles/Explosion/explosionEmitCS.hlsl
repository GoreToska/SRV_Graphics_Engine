struct Particle // todo: change for my particles
{
	float4 initialColor;
	float4 endColor;
	float3 position;
	float _1;
	float3 prevPosition;
	float _2;
	float3 velocity;
	float _3;
	float3 acceleration;
	float _4;
	float maxLifetime;
	float lifetime;
	float initialSize;
	float endSize;
	float initialWeight;
	float endWeight;
};

struct SortListStruct
{
	uint index;
	float distanceSq;
};

float distanceSquared(float3 a, float3 b)
{
	float3 d = a - b;
	return dot(d, d);
}

RWStructuredBuffer<Particle> particlePool : register(u0);
RWStructuredBuffer<SortListStruct> sortListBuffer : register(u1);
ConsumeStructuredBuffer<uint> deadListIn : register(u2);

cbuffer ParticleParams : register(b0)
{
	float deltaTime;
	uint maxNumParticles;
	uint numEmitInThisFrame;
	uint numAliveParticles;
	float3 emitPosition;
	int _trash1;
	float3 cameraPosition;
	int _trash2;
};

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint index = DTid.y * 32 + DTid.x;
	uint numDeadParticles = maxNumParticles - numAliveParticles;
	if (index >= numDeadParticles || index >= numEmitInThisFrame || index >= maxNumParticles)
	{
		return;
	}

	uint particleIndex = deadListIn.Consume();

	Particle p;
	p.position = emitPosition;
	p.prevPosition = emitPosition;
	p.velocity = float3(0.0f, 5.0f, 0.0f) * 0.001f;
	p.acceleration = 0;
	//float3(0, -0.981f, 0) * 0.01;
	p.maxLifetime = 1000.0f;
	p.lifetime = 0.0f;

	p.initialColor = float4(1.0, 1.0, 1.0, 1);
	p.endColor = float4(0.0, 1.0, 0, 1);

	p.initialSize = 0.1;
	p.endSize = 1.3;

	p.initialWeight = 1.0;
	p.endWeight = 1.0;

	p._1 = 0;
	p._2 = 0;
	p._3 = 0;
	p._4 = 0; // padding

	particlePool[particleIndex] = p;
	SortListStruct sls;
	sls.index = particleIndex;
	sls.distanceSq = distanceSquared(p.position, cameraPosition);
	uint sortListIndex = sortListBuffer.IncrementCounter();
	sortListBuffer[sortListIndex] = sls;
}

