
cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
}

cbuffer ModelConstants : register(b2)
{
	float4x4 world;
}

struct VSIn
{
	uint vertexId : SV_VertexID;
};

float4 VS(VSIn input) : SV_Position
{
	if (input.vertexId == 0)
		return mul(projection, mul(camera, mul(world, float4(-0.1f, -0.1f, 0.0f, 1.0f))));
	else if (input.vertexId == 1)
		return mul(projection, mul(camera, mul(world, float4(-0.1f, +0.1f, 0.0f, 1.0f))));
	else if (input.vertexId == 2)
		return mul(projection, mul(camera, mul(world, float4(+0.1f, -0.1f, 0.0f, 1.0f))));
	else if (input.vertexId == 3)
		return mul(projection, mul(camera, mul(world, float4(+0.1f, +0.1f, 0.0f, 1.0f))));

	return float4(0.f, 0.0f, 0.0f, 0.0f);
}

float4 PS() : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}