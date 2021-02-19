
struct VOut
{
	float4 position : SV_POSITION0;
	float3 normal : NORMAL;
};

cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
	float4 lightPos;
}

cbuffer ModelConstants : register(b2)
{
	float4x4 world;
}

struct VertexInputType
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 color : COLOR;
};

VOut VShader(VertexInputType input)
{
	VOut output;
	
	float4 worldSpace = mul(world, input.position);
	float4 cameraSpace = mul(camera, worldSpace);
	float4 clipSpace = mul(projection, cameraSpace);

	// set w to 0 to cancel out translation aspect of world transform
	float4 normal4 = { input.normal, 0 };
	float4 worldNormal = mul(world, normal4);

	float3 normal = normalize(worldNormal.xyz);


	output.position = clipSpace;
	output.normal = normal;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	return float4(input.normal, 1.0f);
}
