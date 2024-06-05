// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "Engine/TextureRenderTarget2D.h"
#include "CanvasItem.h"

/**
 * 
 */
class VISIONCONEVISUALIZER_API FRenderTargetDrawerViewportClient : public FViewportClient
{
private:
	UTextureRenderTarget2D* RenderTarget = nullptr;
public:
	virtual void Draw(FViewport* Viewport, FCanvas* Canvas) override;
	void SetRenderTarget(UTextureRenderTarget2D* InRenderTarget);
};
