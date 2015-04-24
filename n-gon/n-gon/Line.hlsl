struct VOut
{
	float4 position : SV_POSITION;
	float texcoord : TEXCOORD;
	float4 color : COLOR;
};

Texture2DArray Texture;
SamplerState ss;

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix viewProjection;
};

VOut VShader(float3 position : VPOSITION, float3 normal : VNORMAL, float2 texcoord : VTEXCOORD, float3 p1 : IPOINTA, float3 p2 : IPOINTB, float3 n1 : INORMALA, float3 n2 : INORMALB, float4 color : ICOLOR, matrix model : IMODEL)
{
	VOut output;
	position = p1 * (texcoord.x - 1) * -1;
	position += p2 * texcoord.x;
	position -= n1 * (texcoord.y - 1) * (texcoord.x - 1);
	position -= n1 * texcoord.y * (texcoord.x - 1);
	position += n2 * (texcoord.y - 1) * texcoord.x;
	position += n2 * texcoord.y * texcoord.x;

	output.position = float4(position, 1.0);
	output.position = mul(model, output.position);
	output.position = mul(viewProjection, output.position);
	output.texcoord = texcoord.y;
	output.color = color;
	return output;
}

float4 PShader(float4 position : SV_POSITION, float light : TEXCOORD, float4 color : COLOR) : SV_TARGET
{
	if (fmod(position.y, 4.0) < 1)
	{
		return color * 0;
	}
	if (fmod(position.y, 4.0) > 2 && fmod(position.y, 4.0) < 3)
	{
		return color;
	}
	return  color *(1 - abs((light - 0.5f) * 2));
}
