struct VOut
{
	float4 position : SV_POSITION;
	float4 color: COLOR;
};

cbuffer VS_CONSANT_BUFFER : register(b0)
{
	float4x4 camera;
	float4x4 world;
}

VOut VShader(float4 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD, float4 color : COLOR)
{
	VOut output;
	
	float4 worldSpace = mul(world, position);
	float4 cameraSpace = mul(camera, worldSpace);
	output.position = cameraSpace;

	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : color) : SV_TARGET
{
	return color;
}