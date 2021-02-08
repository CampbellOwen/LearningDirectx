
Texture2D shaderTexture;
SamplerState SampleType;

struct VOut
{
	float4 position : SV_POSITION0;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord: TEXCOORD;
};

cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
	float4 lightPos;
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
	output.worldPos = worldSpace.xyz;
	output.normal = normal;
	output.texCoord = input.texCoord;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	float4 textureSample = shaderTexture.Sample(SampleType, input.texCoord);
	
	float3 lightDir = input.worldPos - lightPos.xyz;
	float distance = length(lightDir);
	lightDir = lightDir / distance;

	float distance_2 = distance * distance;


	float NdotL = dot(input.normal, lightDir);
	float diffuse_intensity = saturate(NdotL);

	float kd = diffuse_intensity;
	float4 ka = float4(0.1f, 0.1f, 0.1f, 1.0f);


	float4 diffuse = textureSample * kd;
	float4 ambient = textureSample * ka;

	return diffuse;
}