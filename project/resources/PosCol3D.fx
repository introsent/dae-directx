//------------------------------------------------
// Global variables
//------------------------------------------------
float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;

//------------------------------------------------
// Input/Output Struct
//------------------------------------------------
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};
SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};


struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR;
    float2 UV : TEXCOORD;
};
//------------------------------------------------
// Vertex Shader
//------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.0f), gWorldViewProjectionMatrix);
	output.Color = input.Color;
    output.UV = input.UV;
	return output;
}

//------------------------------------------------
// Pixel Shader
//------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return float4(input.Color, 1.0f);
}

float4 PSPoint(VS_OUTPUT input) : SV_TARGET
{
    return float4(gDiffuseMap.Sample(samPoint, input.UV));
}
float4 PSLinear(VS_OUTPUT input) : SV_TARGET
{
    return float4(gDiffuseMap.Sample(samLinear, input.UV));
}
float4 PSAnisotropic(VS_OUTPUT input) : SV_TARGET
{
    return float4(gDiffuseMap.Sample(samAnisotropic, input.UV));
}

//------------------------------------------------
// Technique
//------------------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

technique11 PointTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSPoint()));
    }
}
technique11 LinearTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSLinear()));
    }
}
technique11 AnisotropicTechnique
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSAnisotropic()));
    }
}
