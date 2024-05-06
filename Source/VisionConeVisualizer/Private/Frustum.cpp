// Fill out your copyright notice in the Description page of Project Settings.


#include "Frustum.h"
#include "Components/DrawFrustumComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AFrustum::AFrustum()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AFrustum::AFrustum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Make the scene component the root component
	RootComponent = SceneComponent;
	
	//// Setup camera defaults
	//CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//CameraComponent->FieldOfView = 90.0f;
	//CameraComponent->bConstrainAspectRatio = true;
	//CameraComponent->AspectRatio = 1.777778f;
	//CameraComponent->PostProcessBlendWeight = 1.0f;

	//CameraComponent->SetCameraMesh(nullptr);
	//CameraComponent->SetupAttachment(SceneComponent);

	// Setup DrawFrustumComp
	DrawFrustum = CreateDefaultSubobject<UDrawFrustumComponent>(TEXT("DrawFrustumComponent"));
	DrawFrustum->SetupAttachment(this->SceneComponent);
	DrawFrustum->SetIsVisualizationComponent(true);
	DrawFrustum->CreationMethod = this->SceneComponent->CreationMethod;
	DrawFrustum->SetVisibility(true);
	DrawFrustum->RegisterComponentWithWorld(GetWorld());

	EndDistance = 1000.0f;
}

// Called when the game starts or when spawned
void AFrustum::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFrustum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFrustum::OverrideFrustumColor(FColor NewColor)
{
	if(DrawFrustum)
	{
		//CameraComponent->OverrideFrustumColor(NewColor);
		DrawFrustum->FrustumColor = NewColor;
		//CameraComponent->RefreshVisualRepresentation();
		DrawFrustum->MarkRenderStateDirty();
	}
}

void AFrustum::DrawThisFrustum() {
	DrawFrustum->FrustumAngle = this->FOV;
	DrawFrustum->FrustumStartDist = 10.f;
	DrawFrustum->FrustumEndDist = DrawFrustum->FrustumStartDist + EndDistance;
	DrawFrustum->FrustumAspectRatio = this->AspectRatio;
	DrawFrustum->MarkRenderStateDirty();
}

void AFrustum::ChangeFrustumVisibility() {
	if (DrawFrustum->GetVisibleFlag()) {
		DrawFrustum->SetVisibility(false);
	}
	else {
		DrawFrustum->SetVisibility(true);
	}
	this->DrawThisFrustum();
}