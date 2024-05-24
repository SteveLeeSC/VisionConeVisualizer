// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Frustum.h"
#include "ViewFrustumCover.h"
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

	UPROPERTY(EditAnywhere, Category = Tick)
		bool bEnableEditorTick = false;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* Frustum1;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* Frustum2;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFrustum* FrustumWrap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Frustum1Path;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Frustum2Path;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString FrustumWrapPath;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

	// Function to recolor frustums
	UFUNCTION(CallInEditor, BlueprintCallable)
		void RecolorFrustums();

	// Function to find a wrap cone which is the smallest cover for Cone A and B.
	UFUNCTION(CallInEditor, BlueprintCallable)
		void ReadAndSetFrustums();

	UFUNCTION(CallInEditor, BlueprintCallable)
		void WarpFrustums();
};
