
#include "Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;

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

	/* mul : 곱하기가 가능한 모든 행렬(좌변의 열, 우변의 행 같다면)에 대해서 다 곱하기를 수행해준다. */
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

    float2 offset = float2(0.f, g_Timer); // x축으로 0.1의 속도로 이동
    float2 newTexcoord = In.vTexcoord + offset;

   
    Out.vColor = g_Texture.Sample(LinearSampler, newTexcoord);

	//Out.vColor.gb = Out.vColor.r;
    //if (Out.vColor.a >= 0.5f)
    //    discard;
    
    float4 sandColor = float4(1.f, 0.82f, 0.7f, 0.55f);
    
    // 알파 값을 유지하며 황토색으로 변환
    Out.vColor.rgb = sandColor * Out.vColor.rgb;
    Out.vColor.a += 0.1f;
    
    return Out;
}

PS_OUT PS_ROTATION(PS_IN In)
{
    PS_OUT Out;

    // 중심점을 기준으로 텍스처 좌표를 회전시키기 위해 텍스처 좌표의 중심을 (0.5, 0.5)로 이동
    float2 centeredTexcoord = In.vTexcoord - float2(0.5f, 0.5f);

    // 회전 각도 계산 (라디안 단위)
    float angle = g_Timer;

    // 회전 행렬 적용
    float2 rotatedTexcoord;
    rotatedTexcoord.x = centeredTexcoord.x * cos(angle) - centeredTexcoord.y * sin(angle);
    rotatedTexcoord.y = centeredTexcoord.x * sin(angle) + centeredTexcoord.y * cos(angle);

    // 중심점을 다시 원래대로 이동
    rotatedTexcoord += float2(0.5f, 0.5f);

    // 텍스처 샘플링
    Out.vColor = g_Texture.Sample(LinearSampler, rotatedTexcoord);

    // 색상 조정 (선택 사항)
    float4 sandColor = float4(1.f, 0.82f, 0.7f, 0.55f);
    Out.vColor.rgb = sandColor * Out.vColor.rgb;
    Out.vColor.a += 0.1f;

    return Out;
}

PS_OUT PS_SNIPER(PS_IN In)
{
    PS_OUT Out;

    float2 offset = float2(g_Timer, 0); 
    float2 newTexcoord = In.vTexcoord + offset;

    Out.vColor = g_Texture.Sample(LinearSampler, newTexcoord);

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
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Rotation
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_ROTATION();
    }

    pass Sniper
    {
        SetRasterizerState(RS_Cull_NONE);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_SNIPER();
    }

}
