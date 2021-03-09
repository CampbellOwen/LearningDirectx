
struct VOut
{
	float4 position : SV_POSITION0;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
};

cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
}

struct Light
{
	int type;
	float3 padding;
	float4 position;
};

cbuffer LightConstants : register(b1)
{
	int numLights;
	float3 padding;
	Light lights[8];
}

cbuffer ModelConstants : register(b2)
{
	float4x4 world;
	float3 albedo;
	float roughness;
	bool useLambert;
	bool useDisney;
	bool useEarlHammon;
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
	output.worldPos = worldSpace.xyz;
	output.normal = normal;

	return output;
}

static const float PI = 3.14159265f;

float LambertianDiffuseBRDF()
{
	return 1.0f / PI;
}

float FresnelSchlick(float u, float3 f0, float f90)
{
	return f0 + (f90 - f0) * pow(1.0 - u, 5.0);
}

float DisneyDiffuseBRDF(float NoV, float NoL, float LoH, float roughness)
{
	float f90 = 0.5 + 2.0 * roughness * LoH * LoH;
	float f0 = 1.0;
	float lightScatter = FresnelSchlick(NoL, f0, f90);
	float viewScatter = FresnelSchlick(NoV, f0, f90);

	return lightScatter * viewScatter * (1.0 / PI);
}

float EarlHammonDiffuseBRDF(float NoV)
{
	return (1.05 / PI) * (1 - pow(1 - NoV, 5));
}

float4 PShader(VOut input) : SV_TARGET
{
	
	float4 lightPos = lights[0].position;
	float3 lightDir = normalize(lightPos.xyz - input.worldPos);
   float3 viewDir = normalize(cameraPos - input.worldPos);

	float3 halfVector = normalize(lightDir + viewDir);

	float NoL = saturate(dot(input.normal, lightDir));
	float NoV = dot(input.normal, viewDir);
	float Loh = dot(lightDir, halfVector);

	float f0 = 0.04;

	float fresnel = FresnelSchlick(NoV, f0, 1.0f);

	float Fd = 0;
	if (useLambert)
		Fd = LambertianDiffuseBRDF();
	else if (useDisney)
		Fd = DisneyDiffuseBRDF(NoV, NoL, Loh, roughness * roughness);
	else if (useEarlHammon)
		Fd = EarlHammonDiffuseBRDF(NoV);

	return float4(albedo, 1.0f) * ((1 - fresnel) * Fd) * NoL;
}
