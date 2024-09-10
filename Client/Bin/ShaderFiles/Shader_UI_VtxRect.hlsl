
#include "Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
float g_Ratio;
float g_Num;
float g_Timer;

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
    VS_OUT Out;
    

    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;

    // g_Ratio를 사용하여 텍스처 좌표 스케일링
    float2 scaledTexCoord = In.vTexcoord * g_Ratio;
    
      Out.vColor = g_Texture.Sample(LinearSampler, scaledTexCoord);

    // 알파 값이 0.1 이하인 경우 픽셀 제거
    if (Out.vColor.a <= 0.1f)
        discard;
    
    if (In.vTexcoord.x >= g_Ratio)
        Out.vColor.rgb = float3(0.5f, 0.5f, 0.5f);
        
    return Out;
}

PS_OUT PS_DEFAULT(PS_IN In)
{
    PS_OUT Out;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    if (Out.vColor.a <= 0.1f)
        discard;
    


    return Out;
}

PS_OUT PS_BOSS(PS_IN In)
{
    PS_OUT Out;

    float2 scaledTexCoord = In.vTexcoord * g_Ratio;
    
    Out.vColor = g_Texture.Sample(LinearSampler, scaledTexCoord);
    Out.vColor.rgb = float3(1.f, 0.f, 0.f);
	
    if (In.vTexcoord.x >= g_Ratio)
        Out.vColor.rgb = float3(0.5f, 0.5f, 0.5f);

    return Out;
}

PS_OUT PS_Alpha(PS_IN In)
{
    PS_OUT Out;
    
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    if (Out.vColor.a <= 0.1f)
        discard;
    
    Out.vColor.a = 1 - g_Timer;
    

    return Out;
}

technique11 DefaultTechnique
{
	/* PASS의 기준 : 셰이더 기법의 캡슐화. */
    pass Default
    {

        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEFAULT();
    }

    pass CW
    {
        SetRasterizerState(RS_Cull_CW);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DEFAULT();
    }

    pass Bar
    {

        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default,0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Boss
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_BOSS();
    }


    pass Alpha
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_Alpha();
    }

}
