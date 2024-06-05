// Fill out your copyright notice in the Description page of Project Settings.


#include "RendertargetDrawerViewportClient.h"




void FRenderTargetDrawerViewportClient::Draw(FViewport* Viewport, FCanvas* Canvas)
{
	if(RenderTarget != nullptr)
	{
		FCanvasTileItem TileItem(FVector2D(0, 0), RenderTarget->Resource, FVector2D(RenderTarget->SizeX, RenderTarget->SizeY), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Opaque;
		Canvas->DrawItem(TileItem);
	}
}

void FRenderTargetDrawerViewportClient::SetRenderTarget(UTextureRenderTarget2D* InRenderTarget)
{
	this->RenderTarget = InRenderTarget;
	
}
