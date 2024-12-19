//------------------------------------------------
// Global variables
//------------------------------------------------

//Matrices
float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
float4x4 gWorldMatrix : WorldMatrix;

//Maps
Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlosinessMap : GlosinessMap;

//Lights / Materials
const float3 gLightDirection : LightDirection = (0.577f, -0.577f, 0.577f);
const float  gLightIntensity = 7.0f;
const float  gShininess = 25.0f;

//Camera
float3 gCameraPosition : CameraPosition;

//Math consts
const float gPI = 3.14159265358979323846f;

//Sampler state
SamplerState gSamplerState : register(s0);


//------------------------------------------------
// Input/Output Struct
//------------------------------------------------
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color	: COLOR;
    float2 UV		: TEXCOORD;
    float3 Normal	: NORMAL;
    float3 Tangent	: TANGENT;
};

struct VS_OUTPUT
{
    float3 Position      : POSITION;
    float4 WorldPosition : WORLD;
    float3 Color         : COLOR;
    float2 UV            : TEXCOORD;
    float3 Normal        : NORMAL;
    float3 Tangent       : TANGENT;
};

//------------------------------------------------
// Vertex Shader
//------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    
    output.Position      = mul(float4(input.Position, 1.0f), gWorldViewProjectionMatrix);
    output.WorldPosition = mul(float4(input.Position, 1.0f), gWorldMatrix);
	output.Color         = input.Color;
    output.UV            = input.UV;
    output.Normal        = mul(normalize(input.Normal), (float3x3) gWorldMatrix);
    output.Tangent       = mul(normalize(input.Tangent), (float3x3) gWorldMatrix);
    
	return output;
}

//------------------------------------------------
// Pixel Shader Functions
//------------------------------------------------
float3 Phong(float3 ks, const float exp, const float3 l, const float3 v, const float3 n)
{
    float3 reflect = reflect(l, n);
   // l - (2 * max(dot(n, l), 0.f) * n);
    const float cosAlpha = max(dot(reflect, v), 0.f);
    return ks * pow(cosAlpha, exp);
}

float3 Lambert(const float3 cd, const float kd = 1)
{
    const float3 rho = kd * cd;
    return rho / gPI;
}

float3 Shade(VS_OUTPUT input)
{
    //Normal map
    float3   binormal = normalize(cross(input.Normal, input.Tangent));
    float3x3 tangentSpaceAxis = (normalize(input.Tangent), binormal, input.Normal);
    float3   normalMapSample = gNormalMap.Sample(gSamplerState, input.UV);
    input.Normal = normalize(mul(2.f * normalMapSample - 1.f, tangentSpaceAxis));
    
    //Observed area
    float cosOfAngle = dot(input.Normal, -gLightDirection);
    if (cosOfAngle < 0.f)
        return (0.f, 0.f, 0.f);
    
    float3 observedArea = saturate(cosOfAngle);

    //Diffuse map
    float3 diffuse = Lambert(gDiffuseMap.Sample(gSamplerState, input.UV).rgb);
    
    //Glosiness map
    float3 gloss = gGlosinessMap.Sample(gSamplerState, input.UV);
    float exp = gloss.r * gShininess;
    
    //Specular map
    float3 viewDirection = normalize(input.WorldPosition.rgb - gCameraPosition);
    float3 specular = Phong(gSpecularMap.Sample(gSamplerState, input.UV).rgb, exp, -gLightDirection, viewDirection, input.Normal);
    

    //Return combined
    return float3(specular + diffuse * observedArea * gLightIntensity);

}

//------------------------------------------------
// Pixel Shader
//------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return float4(Shade(input), 1.f);
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
