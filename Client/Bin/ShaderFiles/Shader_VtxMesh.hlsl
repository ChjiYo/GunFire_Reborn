
float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float4			g_vLightDir = float4(1.f, -1.f, 1.f, 0.f);
float4			g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
float4			g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
float4			g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);

texture2D		g_DiffuseTexture;
float4			g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
float4			g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);

vector			g_vCamPosition;

sampler DefaultSampler = sampler_state 
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float3 vTangent : TANGENT;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vTexcoord = In.vTexcoord;
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;	

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
	if (vMtrlDiffuse.a < 0.1f)
		discard;

	/* 0.0f ~ 1.f */
	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	/* 0.3f ~ 1.f */
	vector		vShade = saturate(fShade + g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	vector		vLook = In.vWorldPos - g_vCamPosition;

	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f);
	vector		vSpecular = g_vLightSpecular * g_vMtrlSpecular * fSpecular;

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * vShade + vSpecular;

	return Out;
}

technique11		DefaultTechnique
{	
	pass Default
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}














