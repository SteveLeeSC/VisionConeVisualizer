// Fill out your copyright notice in the Description page of Project Settings.


#include "Frustum.h"

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
	
	// Setup camera defaults
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->FieldOfView = 90.0f;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->AspectRatio = 1.777778f;
	CameraComponent->PostProcessBlendWeight = 1.0f;

	CameraComponent->SetCameraMesh(nullptr);
	CameraComponent->SetupAttachment(SceneComponent);
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
	if(CameraComponent)
	{
		CameraComponent->OverrideFrustumColor(NewColor);
		CameraComponent->RefreshVisualRepresentation();
	}
}

