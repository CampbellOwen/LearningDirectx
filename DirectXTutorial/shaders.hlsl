
Texture2D shaderTexture;
SamplerState SampleType;

struct VOut
{
	float4 position : SV_POSITION;
	float2 texCoord: TEXCOORD;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	float4x4 camera;
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
	output.position = cameraSpace;

	output.texCoord = input.texCoord;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.texCoord);
}