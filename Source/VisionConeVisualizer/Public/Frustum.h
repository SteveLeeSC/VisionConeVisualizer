// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Frustum.generated.h"

UCLASS()
class VISIONCONEVISUALIZER_API AFrustum : public AActor
{
	GENERATED_BODY()
public:

	/** The camera component for this camera */
	//UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UCameraComponent* CameraComponent;

	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AAA")
	float AspectRatio;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AAA")
	float FOV;
public:
	// Sets default values for this actor's properties
	AFrustum();
	
	AFrustum(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OverrideFrustumColor(FColor NewColor);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "AAA")
		void DrawThisFrustum();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "AAA")
		void ChangeFrustumVisibility();

#if WITH_EDITORONLY_DATA
	// The frustum component used to show visually where the camera field of view is
	class UDrawFrustumComponent* DrawFrustum;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AAA")
		float EndDistance;
#endif	// WITH_EDITORONLY_DATA
};
