
#include "Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;

float           g_Timer;
bool            g_Hit;

struct VS_IN
{
	float3 vPosition : POSITION;
	float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexcoord = In.vTexcoord;

	return Out;
}


/* w나누기연산을수행한다. 투영변환. */
/* 뷰포트 변환. */
/* 래스타라이즈 -> 픽셀을 만들어준다. */
//float4 PS_MAIN(PS_IN In) : SV_TARGET0
//{
//
//}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;	

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    if (Out.vColor.a <= 0.1f)
        discard;

	return Out;
}

PS_OUT PS_Effect(PS_IN In)
{
    PS_OUT Out;
	
    float2 center = float2(0.5, 0.5);
    float distance = length(In.vTexcoord - center);

    // 거리 기반으로 알파값 계산 (중심에서 가까울수록 알파값이 큼)
    float alpha = saturate(1.0 - distance * 2.0);

    // 바깥쪽으로 갈수록 어둡게 하기 위해 컬러를 조정
    float brightness = saturate(1.0 - distance * 1.5);

    // 최종 색상 (예: 빨간색 바탕에 알파값을 적용)
    Out.vColor = float4(1.0, 0.0, 0.0, alpha);

    // 밝기를 조정하여 바깥쪽을 어둡게 만듦
    Out.vColor.rgb *= brightness;
	
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    if (Out.vColor.a <= 0.1f)
        discard;

    return Out;
}

PS_OUT PS_CircleEffect(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

    float2 Center = float2(0.5f, 0.5f);
    float2 offset = In.vTexcoord - Center;
    float distance = length(offset);

// 거리 기반으로 블렌딩 비율 계산 (중앙일수록 blendFactor가 1, 외곽일수록 0)
    float blendFactor = 1.0f - saturate(distance / 1.0f);

// 블렌딩 계산 (중앙이 어둡고 외곽이 밝음)
    Out.vColor = lerp(float4(1, 1, 1, 0), Out.vColor, blendFactor);

    if (Out.vColor.a <= 0.1f)
        discard;


    return Out;
}

PS_OUT PS_HIT(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
   
    if (g_Hit)
        Out.vColor.a -= g_Timer;
    else
        Out.vColor.a = 0;
    return Out;
}

PS_OUT PS_Shield(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    Out.vColor.rgb = float3(1, 0.647f, 0.f);

    return Out;
}

technique11		DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */
	pass Default
	{
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass None
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Effect
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Effect();
    }

    pass CircleEffect
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_CircleEffect();
    }

    pass Hit
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_HIT();
    }

    pass Shield
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Shield();
    }

}
