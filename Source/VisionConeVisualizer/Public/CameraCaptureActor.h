// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Rendering/SlateRenderer.h"
#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SUserWidget.h"
#include "Slate/SceneViewport.h"
#include "RendertargetDrawerViewportClient.h"
#include "CameraCaptureActor.generated.h"

UCLASS()
class VISIONCONEVISUALIZER_API ACameraCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraCaptureActor();

	// Constructor with object initializer
	ACameraCaptureActor(const FObjectInitializer& ObjectInitializer);

	// Scene Capture Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;

	// Render Target to store the captured scene
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* CapturedScene;

	// Two render targets to be displayed in two viewports
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* RenderTarget1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	UTextureRenderTarget2D* RenderTarget2;

	// Create two viewports
	TSharedPtr<SViewport> Viewport1;
	TSharedPtr<SViewport> Viewport2;
	
	// Create two windows
	TSharedPtr<SWindow> Window1;
	TSharedPtr<SWindow> Window2;

	// Create two viewport clients
	TSharedPtr<FRenderTargetDrawerViewportClient> ViewportClient1;
	TSharedPtr<FRenderTargetDrawerViewportClient> ViewportClient2;

	// SceneViewports
	TSharedPtr<FSceneViewport> SceneViewport1;
#if WITH_EDITOR
	/*// Called when the actor is being removed from a level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called when editor shuts down
	virtual void Shutdown() override;*/
#endif	// WITH_EDITOR
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
