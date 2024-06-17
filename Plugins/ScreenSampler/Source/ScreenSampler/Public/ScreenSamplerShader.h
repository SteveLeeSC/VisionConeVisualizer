// Copyleft 2024 not Any Compony, Inc. All wrongs reserved.

# pragma once

#include "GlobalShader.h"
#include "Shader.h"

class FScreenSamplerShader : public FGlobalShader
{
	DECLARE_INLINE_TYPE_LAYOUT(FScreenSamplerShader, NonVirtual);
public:
	FScreenSamplerShader(){}
	FScreenSamplerShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		// MainColorVal.Bind(Initializer.ParameterMap, TEXT("MainColor"));
		ScreenTextureVal.Bind(Initializer.ParameterMap, TEXT("ScreenTexture"));
		ScreenSamplerVal.Bind(Initializer.ParameterMap, TEXT("ScreenSampler"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
	}

	void SetParameters(FRHICommandList& RHICmdList, FRHITexture2D* InScreenTexture)
	{
		FRHIPixelShader* PixelShaderRHI = RHICmdList.GetBoundPixelShader();
		SetTextureParameter(
			RHICmdList,
			PixelShaderRHI,
			ScreenTextureVal,
			ScreenSamplerVal,
			TStaticSamplerState<SF_Trilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI(),
			InScreenTexture
			);
	}
private:
	// LAYOUT_FIELD(FShaderParameter, MainColorVal);
	LAYOUT_FIELD(FShaderResourceParameter, ScreenSamplerVal);
	LAYOUT_FIELD(FShaderResourceParameter, ScreenTextureVal);
};

class FScreenSamplerVS : public FScreenSamplerShader
{
	DECLARE_GLOBAL_SHADER(FScreenSamplerVS);
public:
	FScreenSamplerVS(){}
	FScreenSamplerVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FScreenSamplerShader(Initializer)
	{}
};



class FScreenSamplerPS : public FScreenSamplerShader
{
	DECLARE_GLOBAL_SHADER(FScreenSamplerPS);
public:
	FScreenSamplerPS(){}
	FScreenSamplerPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FScreenSamplerShader(Initializer)
	{}
};

IMPLEMENT_SHADER_TYPE(, FScreenSamplerVS, TEXT("/ScreenSampler/ScreenSampler.usf"), TEXT("MainVS"), SF_Vertex)
IMPLEMENT_SHADER_TYPE(, FScreenSamplerPS, TEXT("/ScreenSampler/ScreenSampler.usf"), TEXT("MainPS"), SF_Pixel)