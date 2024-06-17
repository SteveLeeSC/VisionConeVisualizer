// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraCaptureActor.h"



// Sets default values
ACameraCaptureActor::ACameraCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

ACameraCaptureActor::ACameraCaptureActor(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	// Initialize the Scene Capture Component and set it as the root component
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;
	SceneCaptureComponent->bCaptureEveryFrame = true;
	
	// Initialize the Render Target and set it as the render target for the Scene Capture Component
	CapturedScene = NewObject<UTextureRenderTarget2D>(SceneCaptureComponent, TEXT("CapturedScene"));
	CapturedScene->InitAutoFormat(800, 600);
	
	if(CapturedScene != nullptr)
	{
		SceneCaptureComponent->TextureTarget = CapturedScene;
	}
	CapturedScene->UpdateResourceImmediate();

	// Create a fsceneviewport and its client
	ViewportClient1 = MakeShareable(new FRenderTargetDrawerViewportClient());
	ViewportClient1->SetRenderTarget(CapturedScene);
	
	// Create the sviewport
	Viewport1 = SNew(SViewport)
		// .RenderDirectlyToWindow(true)
		.IsEnabled(true)
		.EnableGammaCorrection(false)
		.IgnoreTextureAlpha(true)
		.ViewportSize(FVector2D(800, 600))
		.ShowEffectWhenDisabled(false)
		.EnableBlending(true)
		.Visibility(EVisibility::Visible)
		;
	SceneViewport1 = MakeShared<FSceneViewport>(ViewportClient1.Get(), Viewport1);
	
	SceneViewport1->SetViewportSize(800, 600);
	
}

// Called when the game starts or when spawned
void ACameraCaptureActor::BeginPlay()
{
	Super::BeginPlay();
	// Initialize the two render targets to be displayed in two viewports
	// RenderTarget1 = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget1"));
	// RenderTarget2 = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget2"));
	//
	// // Set render target resolution to 1920x1080
	// RenderTarget1->SizeX = 1920, RenderTarget1->SizeY = 1080;
	// RenderTarget1->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	// RenderTarget2->SizeX = 1920, RenderTarget2->SizeY = 1080;
	// RenderTarget2->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	
	// Initialize the two windows and all other objects: viewports, slate renderers, etc
	// Set up window 1 and its viewport and slate renderer
	// then display render target 1 in the viewport
	// Create the sviewport which will be used to display the render target
	
	
	// SlateRenderer->RegisterCurrentScene();
	SceneViewport1->UpdateViewportRHI(false, 800, 600, EWindowMode::Windowed, CapturedScene->GetFormat());
	Viewport1->SetViewportInterface(SceneViewport1.ToSharedRef());
	// Create the window
	Window1 = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(FText::FromString("Windows 1"))
		.IsInitiallyMaximized(false)
		.ClientSize(FVector2D(800,600))
		.ScreenPosition(FVector2D(0, 0))
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMaximize(true)
		.SupportsMinimize(true)
		.SaneWindowPlacement(false)
		.HasCloseButton(true)
		[
			Viewport1.ToSharedRef()
		];
	Window1->SetViewportSizeDrivenByWindow(true);
	Window1->FlashWindow();
	FSlateApplication::Get().AddWindow(Window1.ToSharedRef(), true);
	Window1->SetWindowMode(EWindowMode::Windowed);
	CapturedScene->UpdateResourceImmediate(false);
	// SceneViewport1->Invalidate();
	
	
	// Window2 = SNew(SWindow)
	// 	.AutoCenter(EAutoCenter::None)
	// 	.Title(FText::FromString("Windows 2"))
	// 	.IsInitiallyMaximized(false)
	// 	.ClientSize(FVector2D(800,600))
	// 	.ScreenPosition(FVector2D(0, 0))
	// 	.CreateTitleBar(true)
	// 	.SizingRule(ESizingRule::UserSized)
	// 	.SupportsMaximize(true)
	// 	.SupportsMinimize(true)
	// 	.HasCloseButton(true);
	//
	// // Initialize the two viewports
	// FSlateApplication::Get().AddWindow(Window2.ToSharedRef());
	// Viewport2 = SNew(SViewport)
	// 	.RenderDirectlyToWindow(true)
	// 	.EnableGammaCorrection(false)
	// 	.IgnoreTextureAlpha(true)
	// 	.ViewportSize(FVector2D(800, 600))
	// 	.EnableBlending(true);
	//
	// Window2->SetContent(Viewport2.ToSharedRef());
	// SceneViewport->Invalidate();
}

// Called every frame
void ACameraCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Extract the texture from the captured render target to be used in shader
	CapturedScene->UpdateResourceImmediate();
	auto tmp = CapturedScene->TextureReference.TextureReferenceRHI->GetReferencedTexture()->GetTexture2D();
	
	SceneViewport1->Draw();
}

