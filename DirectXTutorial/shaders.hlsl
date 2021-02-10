
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

	float4 normal4 = { input.normal, 0 };
	float4 worldNormal = mul(world, normal4);

	float3 normal = normalize(worldNormal.xyz);


	output.position = clipSpace;
	output.worldPos = worldSpace.xyz;
	output.normal = normal;
	output.texCoord = input.texCoord;

	return output;
}

static const float PI = 3.14159265f;

float4 PShader(VOut input) : SV_TARGET
{
	float4 textureSample = shaderTexture.Sample(SampleType, input.texCoord);
	
	float3 lightDir = lightPos.xyz - input.worldPos;
	float distance = length(lightDir);
	lightDir = lightDir / distance;

	float NdotL = dot(input.normal, lightDir);

	float diffuse_intensity = saturate(NdotL);
	float kd = diffuse_intensity / (distance / 10);
	float4 diffuse = textureSample * kd;

	float3 reflected = normalize((2 * NdotL * input.normal) - lightDir);
	float3 view = normalize(cameraPos.xyz - input.worldPos);

	float4 ka = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float4 ambient = textureSample * ka;
	float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float3 half_vector = normalize(lightDir + view);
	
	float specular_constant = 0.1f;

	specular = (textureSample * NdotL) *
		pow(saturate(dot(reflected, view)), specular_constant) *
		((specular_constant + 8.0f) / (8.0f * PI));

	return ambient + diffuse + specular;
}