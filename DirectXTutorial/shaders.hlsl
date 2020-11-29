
Texture2D shaderTexture;
SamplerState SampleType;

struct VOut
{
	float4 position : SV_POSITION0;
	float3 normal : NORMAL;
	float2 texCoord: TEXCOORD;
	float3 lightPos : POSITION1;
};

cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
}
cbuffer ModelConstants : register(b1)
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

	float4 normal4 = { input.normal, 1 };
	float4 worldNormal = mul(world, normal4);

	float3 normal = normalize(worldNormal.xyz);


	output.position = clipSpace;
	output.normal = normal;
	output.texCoord = input.texCoord;
	float4 lightPositionWorld = worldSpace - float4(0.0f, 0.0f, -15.0f, 1.0f);
	output.lightPos = normalize(lightPositionWorld.xyz);

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	
	float kd = clamp(dot(input.lightPos, input.normal), 0.0, 1.0);

	return shaderTexture.Sample(SampleType, input.texCoord) * kd;
	//float4 normalColor = { input.normal, 1 };
	//return normalColor;
}