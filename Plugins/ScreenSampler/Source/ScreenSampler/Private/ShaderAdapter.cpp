#include "ShaderAdapter.h"
#include "ScreenSamplerShader.h"
#include <Engine/TextureRenderTarget2D.h>
#include <RHICommandList.h>

class FVertex_Pos_UV_Declaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FVertex_Pos_UV);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FVertex_Pos_UV, Position), VET_Float4, 0, Stride));
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FVertex_Pos_UV, UV), VET_Float2, 1, Stride));
		VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
	}

	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI->Release();
	}
};

void SampleScreen_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutScreen1,
	FTextureRenderTargetResource* InScreen,
	TArray<FVertex_Pos_UV> CornerInfo1
	)
{
	check(IsInRenderingThread());

	FRHITexture2D* InTexture = InScreen->GetRenderTargetTexture();
	FRHITexture2D* OutTexture = OutScreen1->GetRenderTargetTexture();
	// Transit InTexture to Shader Resource
	RHICmdList.Transition(FRHITransitionInfo(InTexture, ERHIAccess::SRVMask, ERHIAccess::CopySrc));
}

void UShaderAdapter::SampleScreen(UTextureRenderTarget2D* OutScreen1, UTextureRenderTarget2D* InScreen, TArray<FVertex_Pos_UV> CornerInfo1)
{
	if (!OutScreen1 || !InScreen || CornerInfo1.Num() != 4)
	{
		return;
	}

	check(IsInGameThread());
	
	FTextureRenderTargetResource* InScreenTexture =		InScreen->GameThread_GetRenderTargetResource();
	FTextureRenderTargetResource* OutScreenResource = OutScreen1->GameThread_GetRenderTargetResource();

	ENQUEUE_RENDER_COMMAND(CaptureCommond)(
		[InScreenTexture, OutScreenResource, CornerInfo1](FRHICommandListImmediate& RHICmdList)
		{
			SampleScreen_RenderThread(RHICmdList, OutScreenResource, InScreenTexture, CornerInfo1);
		}
	);
}