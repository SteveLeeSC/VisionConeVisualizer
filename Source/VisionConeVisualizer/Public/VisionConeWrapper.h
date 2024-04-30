// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Frustum.h"
#include "Camera/CameraComponent.h"
#include "VisionConeWrapper.generated.h"

UCLASS()
class VISIONCONEVISUALIZER_API AVisionConeWrapper : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVisionConeWrapper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* Frustum1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* Frustum2;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* FrustumWrap;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to recolor frustums
	UFUNCTION(CallInEditor, BlueprintCallable)
	void RecolorFrustums();

	// Function to find a wrap cone which is the smallest cover for Cone A and B.
	// IMPLEMENT THIS FUNCTION.
	UFUNCTION(CallInEditor, BlueprintCallable)
	void WarpCone();
};
