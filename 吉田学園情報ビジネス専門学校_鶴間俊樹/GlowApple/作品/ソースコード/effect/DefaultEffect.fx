//=============================================================================
//
// �f�t�H���g�̃G�t�F�N�g�t�@�C�� [DefaultEffect.fx]
// Author : �ߊԏr��
//
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
//�}�g���b�N�X
float4x4 g_mWorld;	//���[���h�}�g���b�N�X
float4x4 g_mView;	//�r���[�}�g���b�N�X
float4x4 g_mProj;	//�v���W�F�N�V�����}�g���b�N�X

float4x4 g_mLightView;	//���C�g�ڐ��̃r���[�}�g���b�N�X
float4x4 g_mLightProj;	//���C�g�ڐ��̃v���W�F�N�V�����}�g���b�N�X

//�ʒu���W
float4 g_Light;		//���C�g
float4 g_posEye;	//���_�̈ʒu

//�}�e���A��
float4 g_matDiffuse;	//�}�e���A���̃f�B�t���[�Y�F
float4 g_matEmissive;	//�}�e���A���̃G�~�b�V�u�F
float4 g_matSpecular;	//�}�e���A���̃X�y�L�����[�F�@
float g_matPower;		//�}�e���A���̔��˂̋��x

float4 g_colGlow;	//�֊s�̔����F

//�t�H�O
bool g_bEnableFog;	//�t�H�O���L�����ǂ���	//if���͗ǂ��Ȃ��炵���̂Ō�ɕ��@�𒲂ׂ�
float3 g_fogColor;	//�t�H�O�̐F
float2 g_fogRange;	//�t�H�O�̌v�Z�p�̃x�N�g��

//�e�N�X�`��
texture g_Texture;	//�e�N�X�`��
texture g_texShadowMap;	//�V���h�E�}�b�v�e�N�X�`��

//=============================================================================
// �e�N�X�`���T���v���[
//=============================================================================
//�ʏ�̃e�N�X�`���T���v���[
sampler texSampler = sampler_state
{
	texture = g_Texture;
	AddressU = WRAP;
	AddressV = WRAP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//�V���h�E�}�b�v�e�N�X�`���p�̃e�N�X�`���T���v���[
sampler texSamplerShadow = sampler_state
{
	texture = g_texShadowMap;

	//�V���h�E�}�b�v��UV�͈̔͊O��1.0�ɂ��ĉe�������悤�ɂ���
	BorderColor = float4(1.0, 1.0, 1.0, 1.0);
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
//------------------------------
//���_�V�F�[�_�̏o�͒�`
//------------------------------
struct VS_OUTPUT
{
	float4 Pos : POSITION;	//�ʒu
	float4 Diffuse : COLOR0;//�F
	float3 Fog : COLOR1;	//�t�H�O�̉e���x
	float4 TexUV : TEXCOORD0;	//�e�N�X�`�����W

	float4 ZCalcTex : TEXCOORD1;   	// Z�l�Z�o�p�e�N�X�`��
	float4 ColShadow : COLOR2;		//�e���ł���ꍇ�̐F
};

//------------------------------
//�f�t�H���gVS
//------------------------------
VS_OUTPUT RenderSceneVSDefault(
	float4 vPos : POSITION,
	float4 vTexUV : TEXCOORD0)
{
	VS_OUTPUT Out;	//�o��

	//WVP�ϊ�
	Out.Pos = mul(vPos, g_mWorld);
	Out.Pos = mul(Out.Pos, g_mView);
	Out.Pos = mul(Out.Pos, g_mProj);

	//�F
	Out.Diffuse = g_matDiffuse;

	//�t�H�O���v�Z
	Out.Fog = 1.0 - saturate(g_fogRange.x + g_fogRange.y * Out.Pos.w);

	//�e�N�X�`�����W
	Out.TexUV = vTexUV;

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mat = mul(g_mWorld, g_mLightView);
	mat = mul(mat, g_mLightProj);
	Out.ZCalcTex = mul(vPos, mat);

	//�e�̐F
	Out.ColShadow = float4(0.0, 0.0, 0.0, 0.0);

	return Out;
}

//------------------------------
//VS���C�g
//------------------------------
VS_OUTPUT RenderSceneVSLight(
	float4 vPos : POSITION,
	float4 vNor : NORMAL,
	float4 vTexUV : TEXCOORD0
)
{
	VS_OUTPUT Out;

	//WVP�ϊ�
	Out.Pos = mul(vPos, g_mWorld);
	float3 vecView = normalize(g_posEye.xyz - Out.Pos.xyz);	//�����x�N�g��

	Out.Pos = mul(Out.Pos, g_mView);
	Out.Pos = mul(Out.Pos, g_mProj);

	//���_�̖@�������[���h�ϊ����Đ��K��
	float3 nor = normalize(mul((float3)vNor, (float3x3)g_mWorld));	//��]�̂�
	float3 light = normalize((float3)g_Light);	//���_���烉�C�g�ւ̃x�N�g��


	//�e�̐F
	Out.ColShadow = float4(0.0, 0.0, 0.0, 0.0);

	//�f�B�t���[�Y�̌v�Z
	float col = dot(nor, -light);	//�v�Z��̐F�̖��邳

	//�����������Ă���ꍇ�̂݃I�u�W�F�N�g�ɎՂ�ꂽ�Ƃ��̉e�̐F��ݒ�
	if (col >= 0.0) {
		Out.ColShadow = float4(1.0, 1.0, 1.0, 1.0);
	}

	 //�n�[�t�����o�[�g
	col += 1.0;
	col *= 0.5;

	//�X�y�L�����[�̌v�Z
	float3 vecHarf = normalize(-light + vecView);	//�n�[�t�x�N�g��

	//�F�Ɗ|�����킹��
	Out.Diffuse.xyz = g_matDiffuse.xyz * col + pow(g_matSpecular.xyz * saturate(dot(nor, vecHarf)), g_matPower);	//�f�B�t���[�Y + �X�y�L�����[
	Out.Diffuse.w = g_matDiffuse.w;

	//�֊s�����点��
	Out.Diffuse.xyz += pow(1.0 - saturate(dot(vecView, nor)), 2) * g_colGlow.xyz;

	//�t�H�O���v�Z
	Out.Fog = 1.0 - saturate(g_fogRange.x + g_fogRange.y * Out.Pos.w);

	//�e�N�X�`�����W
	Out.TexUV = vTexUV;

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	float4x4 mat = mul(g_mWorld, g_mLightView);
	mat = mul(mat, g_mLightProj);
	Out.ZCalcTex = mul(vPos, mat);


	return Out;
}

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
//------------------------------
//�s�N�Z���V�F�[�_�o�͒�`
//------------------------------
struct PS_OUTPUT
{
	float4 RGB : COLOR0;	//�J���[
};

//------------------------------
// 2d�e�N�X�`������
//------------------------------
PS_OUTPUT RenderScenePS2D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	return Out;
}

//------------------------------
// 2d�e�N�X�`������
//------------------------------
PS_OUTPUT RenderScenePSTex2D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	return Out;
}

//------------------------------
// 3d�f�t�H���g
//------------------------------
PS_OUTPUT RenderScenePSDefault(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	//�G�~�b�V�u�����Z
	Out.RGB.xyz += g_matEmissive.xyz;

	//�t�H�O�����Z
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * In.Fog;

	return Out;
}

//------------------------------
// 3D���C�g
//------------------------------
PS_OUTPUT RenderScenePS3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = In.ZCalcTex.z / In.ZCalcTex.w;

	// �e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + In.ZCalcTex.x / In.ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - In.ZCalcTex.y / In.ZCalcTex.w) * 0.5f;

	// �������W��Z�l�𒊏o
	float SM_Z = tex2D(texSamplerShadow, TransTexCoord).x;

	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (ZValue > SM_Z + 0.002f && SM_Z < 1.0) {	//�V���h�E�}�b�v��Z�l��1.0��������e���ł��Ȃ��悤�ɂ���
		Out.RGB.xyz += Out.RGB.xyz * -0.5f * In.ColShadow.xyz;
	}

	//�G�~�b�V�u�����Z
	Out.RGB.xyz += g_matEmissive.xyz;

	//�t�H�O�����Z
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * In.Fog;

	return Out;
}

//------------------------------
// 3d�e�N�X�`��
//------------------------------
PS_OUTPUT RenderScenePSTex3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	//�G�~�b�V�u�����Z
	Out.RGB.xyz += g_matEmissive.xyz;

	//�t�H�O�����Z
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * In.Fog;

	return Out;
}

//------------------------------
// 3d���C�g�e�N�X�`��
//------------------------------
PS_OUTPUT RenderScenePSLightTex3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = In.ZCalcTex.z / In.ZCalcTex.w;

	// �e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + In.ZCalcTex.x / In.ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - In.ZCalcTex.y / In.ZCalcTex.w) * 0.5f;

	// �������W��Z�l�𒊏o
	float SM_Z = tex2D(texSamplerShadow, TransTexCoord).x;

	// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
	if (ZValue > SM_Z + 0.002f && SM_Z < 1.0) {	//�V���h�E�}�b�v��1.0��������e���ł��Ȃ��悤�ɂ���
		Out.RGB.xyz += Out.RGB.xyz * -0.5f * In.ColShadow.xyz;
	}

	//�G�~�b�V�u�����Z
	Out.RGB.xyz += g_matEmissive.xyz;

	//�t�H�O�����Z
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * In.Fog;

	return Out;
}

//=============================================================================
// �[�x�o�b�t�@�`��p
//=============================================================================
struct VS_OUTPUT_SHADOW
{
	float4 Pos : POSITION;   // �ˉe�ϊ����W
	float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

//------------------------------
// ���_�V�F�[�_
//------------------------------
VS_OUTPUT_SHADOW ZBufferCalc_VS(float4 vPos : POSITION, float4 color : COLOR0)
{
	VS_OUTPUT_SHADOW Out;

	//WVP�ϊ�
	Out.Pos = mul(vPos, g_mWorld);
	Out.Pos = mul(Out.Pos, g_mLightView);
	Out.Pos = mul(Out.Pos, g_mLightProj);

	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	Out.ShadowMapTex = Out.Pos;

	return Out;
}

//------------------------------
// �s�N�Z���V�F�[�_
//------------------------------
float4 ZBufferPlot_PS(float4 ShadowMapTex : TEXCOORD0) : COLOR
{
	// Z�l�Z�o
	return ShadowMapTex.z / ShadowMapTex.w;
}

//=============================================================================
// �e�N�j�b�N�ƃp�X
//=============================================================================
technique RenderScene
{
	//�[�x�o�b�t�@�`��p
	pass P0
	{
		VertexShader = compile vs_3_0 ZBufferCalc_VS();
		PixelShader = compile ps_3_0 ZBufferPlot_PS();
	}

	//-----------------------------------
	//2D�|���S��
	//-----------------------------------
	//�ϊ��ς݂Ȃ̂Œ��_�V�F�[�_�͕K�v�Ȃ�����
	//2d
	pass P2D
	{
		PixelShader = compile ps_3_0 RenderScenePS2D();
	}
	//2d�e�N�X�`������
	pass P2D_TEX
	{
		PixelShader = compile ps_3_0 RenderScenePSTex2D();
	}

	//-----------------------------------
	//3D�|���S��
	//-----------------------------------
	//3d
	pass P3D
	{
		VertexShader = compile vs_3_0 RenderSceneVSDefault();
		PixelShader = compile ps_3_0 RenderScenePSDefault();
	}
	//3D���C�g
	pass P3D_LIGHT
	{
		VertexShader = compile vs_3_0 RenderSceneVSLight();
		PixelShader = compile ps_3_0 RenderScenePS3D();
	}
	//3d�e�N�X�`��
	pass P3D_TEX
	{
		VertexShader = compile vs_3_0 RenderSceneVSDefault();
		PixelShader = compile ps_3_0 RenderScenePSTex3D();
	}
	//3d���C�g�e�N�X�`��
	pass P3D_LIGHT_TEX
	{
		VertexShader = compile vs_3_0 RenderSceneVSLight();
		PixelShader = compile ps_3_0 RenderScenePSLightTex3D();
	}
}