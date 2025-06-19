#pragma pack_matrix(row_major)

struct VS_IN
{
    float4 pos : POSITION0;
    float4 tex : TEXCOORD0;
    float4 normal : NORMAL0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 tex : TEXCOORD;
    float4 normal : NORMAL;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gInvTrWorld;
    float isSpinningFloor; // 0 = no spin, 1 = spin
    float3 padding; // Align to 16-byte boundary (64 bytes total)
};

cbuffer cbPerScene : register(b1)
{
    float4 lightDir;
    float4 lightColorAmbStr;
    float4 viewDirSpecStr;
    float gTime; // Add time for animation
    float3 padding2; // Align to 16-byte boundary (64 bytes total)
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    
#ifdef VERTEX_PASS_THROUGH
    output.pos = float4(input.pos.xyz, 1.0f);
#else
    output.pos = mul(float4(input.pos.xyz, 1.0f), gWorldViewProj);
#endif
    output.tex = input.tex;
    output.normal = mul(float4(input.normal.xyz, 0.0f), gInvTrWorld);
    
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
#ifdef TREAT_TEX_AS_COL
    return input.tex;
#endif

    float4 objColor;
    if (isSpinningFloor > 0.5f) // Enable spinning for this object
    {
        // Extract tile and local coordinates
        float2 texCoord = input.tex.xy;
        float2 tileCoord = floor(texCoord);
        float2 localCoord = frac(texCoord);
        float2 centeredCoord = localCoord - 0.5f;

        // Clockwise rotation
        float spinSpeed = 1.0f; // Radians per second
        float angle = gTime * spinSpeed;
        float cosA = cos(angle);
        float sinA = sin(angle);
        float2x2 rotationMatrix = float2x2(cosA, sinA, -sinA, cosA);

        // Rotate and recombine
        float2 rotatedCoord = mul(centeredCoord, rotationMatrix);
        float2 finalCoord = tileCoord + (rotatedCoord + 0.5f);

        objColor = DiffuseMap.SampleLevel(Sampler, finalCoord, 0);
    }
    else
    {
        objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
    }

    float4 ambient = lightColorAmbStr.w * float4(lightColorAmbStr.xyz, 1.0f);
    float4 result = ambient * objColor;
    
    return float4(result.xyz, 1.0f);
}