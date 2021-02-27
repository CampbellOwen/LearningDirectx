TextureCube skyCubemap : register (t0);
SamplerState SampleType : register (s0);

cbuffer CameraConstants : register(b0)
{
	float4x4 camera;
	float4x4 projection;
	float4 cameraPos;
}


struct VertexInputType
{
	float4 position : POSITION;
};

struct VertexOutputType
{
	float4 position : SV_Position;
	float3 texCoord : TEXCOORD;
};


VertexOutputType VShader(VertexInputType input)
{
	VertexOutputType output;

	output.position = mul(projection, mul(camera, float4(input.position.xyz, 0.0f))); 
	output.texCoord = input.position;
	
	output.position.z = output.position.w;

	return output;
}

float4 PShader(VertexOutputType input) : SV_Target
{
	return skyCubemap.Sample(SampleType, input.texCoord);
}
