// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraCaptureActor.h"

// Sets default values
ACameraCaptureActor::ACameraCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Initialize the Scene Capture Component and set it as the root component
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;
	
	// Initialize the Render Target and set it as the render target for the Scene Capture Component
	CapturedScene = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
	SceneCaptureComponent->TextureTarget = CapturedScene;

	/*// Set render target resolution to 3960x2160
	CapturedScene->SizeX = 3960;
	CapturedScene->SizeY = 2160;
	CapturedScene->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;*/

	// Initialize the two render targets to be displayed in two viewports
	RenderTarget1 = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget1"));
	RenderTarget2 = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget2"));

	// Set render target resolution to 1920x1080
	RenderTarget1->SizeX = 1920, RenderTarget1->SizeY = 1080;
	RenderTarget1->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	RenderTarget2->SizeX = 1920, RenderTarget2->SizeY = 1080;
	RenderTarget2->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	
	// Initialize the two windows and all other objects: viewports, slate renderers, etc
	// Set up window 1 and its viewport and slate renderer
	// then display render target 1 in the viewport
	// First, create the window
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
		.HasCloseButton(true);
	FSlateApplication::Get().AddWindow(Window1);
	// Create the fviewport which will be used to display the render target
	
	
	// Attach viewport to window
	Window1->SetContent(Viewport1.ToSharedRef());
	// Display render target 1 in the viewport
	
	
	Window2 = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(FText::FromString("Windows 2"))
		.IsInitiallyMaximized(false)
		.ClientSize(FVector2D(800,600))
		.ScreenPosition(FVector2D(0, 0))
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMaximize(true)
		.SupportsMinimize(true)
		.HasCloseButton(true);

	// Initialize the two viewports
	FSlateApplication::Get().AddWindow(Window2.ToSharedRef());
	Viewport2 = SNew(SViewport)
		.RenderDirectlyToWindow(true)
		.EnableGammaCorrection(false)
		.IgnoreTextureAlpha(true)
		.ViewportSize(FVector2D(800, 600))
		.EnableBlending(true);

	Window2->SetContent(Viewport2.ToSharedRef());
}

// Called when the game starts or when spawned
void ACameraCaptureActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

