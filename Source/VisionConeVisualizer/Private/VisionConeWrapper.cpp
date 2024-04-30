// Fill out your copyright notice in the Description page of Project Settings.


#include "VisionConeWrapper.h"


// Sets default values
AVisionConeWrapper::AVisionConeWrapper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVisionConeWrapper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVisionConeWrapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVisionConeWrapper::RecolorFrustums()
{
	if(Frustum1) Frustum1->OverrideFrustumColor(FColor(255, 0, 0, 255));
	if(Frustum2) Frustum2->OverrideFrustumColor(FColor(0, 255, 0, 255));
	if(FrustumWrap) FrustumWrap->OverrideFrustumColor(FColor(0, 0, 255, 255));
}

void AVisionConeWrapper::WarpCone()
{
	
}


