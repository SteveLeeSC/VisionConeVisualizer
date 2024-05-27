// Fill out your copyright notice in the Description page of Project Settings.


#include "Frustum.h"
#include "ViewFrustumCover.h"
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
	QuadrilateralDrawer = CreateDefaultSubobject<UQuadrilateralDrawer>(TEXT("QuadrilateralDrawer"));

	// Make the scene component the root component
	RootComponent = SceneComponent;

	// Attach the QuadrilateralDrawer to the SceneComponent
	QuadrilateralDrawer->SetupAttachment(SceneComponent);
	
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



void AFrustum::DrawFarPlane()
{
	// Forward direc in Actor coord system
	FVector ForwardDir = FVector(1, 0, 0);
	FVector Right = FVector(0, 1, 0);
	FVector Up = FVector(0, 0, 1);

	ForwardDir *= EndDistance;

	float HalfWidth = FMath::Tan(FMath::DegreesToRadians(FOV / 2)) * EndDistance;
	float HalfHeight = HalfWidth / AspectRatio;
	// Calc four corners of the far plane
	FVector TopLeft = ForwardDir + HalfWidth * Right + HalfHeight * Up;
	FVector TopRight = ForwardDir - HalfWidth * Right + HalfHeight * Up;
	FVector BottomLeft = ForwardDir + HalfWidth * Right - HalfHeight * Up;
	FVector BottomRight = ForwardDir - HalfWidth * Right - HalfHeight * Up;

	// Draw the far plane
	QuadrilateralDrawer->ClearOldQuadrilaterals();
	QuadrilateralDrawer->AddQuadrilateral(TopLeft, TopRight, BottomRight, BottomLeft, this->DrawFrustum->FrustumColor);
	QuadrilateralDrawer->DrawQuadrilaterals();
}

void AFrustum::DrawIntersectionQuadWithFrustumFarPlane(AFrustum* OtherFrustum)
{
	auto PlaneNormal = OtherFrustum->GetActorForwardVector() * (OtherFrustum->EndDistance - 0.1);
	FrustumCover::FMPlane Plane(PlaneNormal, OtherFrustum->GetActorForwardVector());

	
	float HalfWidth = FMath::Tan(FMath::DegreesToRadians(FOV / 2))* 1.0f;
	float HalfHeight = HalfWidth / AspectRatio;
	// Calc four corners of the far plane of this frustum and transform them to the world coordinate system
	FVector TopLeft = this->GetActorForwardVector() + this->GetActorRightVector() * HalfWidth + this->GetActorUpVector() * HalfHeight;
	FVector TopRight = this->GetActorForwardVector() - this->GetActorRightVector() * HalfWidth + this->GetActorUpVector() * HalfHeight;
	FVector BottomLeft = this->GetActorForwardVector() + this->GetActorRightVector() * HalfWidth - this->GetActorUpVector() * HalfHeight;
	FVector BottomRight = this->GetActorForwardVector() - this->GetActorRightVector() * HalfWidth - this->GetActorUpVector() * HalfHeight;

	// Debug: transform corners back to the actor coordinate system
	auto tmp = this->GetTransform().InverseTransformPosition(TopLeft);
	auto tmp2 = this->GetTransform().InverseTransformPosition(TopRight);
	auto tmp3 = this->GetTransform().InverseTransformPosition(BottomLeft);
	auto tmp4 = this->GetTransform().InverseTransformPosition(BottomRight);

	// Then print them
	UE_LOG(LogTemp, Warning, TEXT("TopLeft: %f %f %f"), tmp.X, tmp.Y, tmp.Z);
	UE_LOG(LogTemp, Warning, TEXT("TopRight: %f %f %f"), tmp2.X, tmp2.Y, tmp2.Z);
	UE_LOG(LogTemp, Warning, TEXT("BottomLeft: %f %f %f"), tmp3.X, tmp3.Y, tmp3.Z);
	UE_LOG(LogTemp, Warning, TEXT("BottomRight: %f %f %f"), tmp4.X, tmp4.Y, tmp4.Z);

	// Make corners rays
	FrustumCover::FMRay TopLeftRay(TopLeft, TopLeft - this->GetActorLocation());
	FrustumCover::FMRay TopRightRay(TopRight, TopRight - this->GetActorLocation());
	FrustumCover::FMRay BottomLeftRay(BottomLeft, BottomLeft - this->GetActorLocation());
	FrustumCover::FMRay BottomRightRay(BottomRight, BottomRight - this->GetActorLocation());
	
	// calc the intersections of the plane with this frustum
	bool OnRay = false;
	auto Intersection = FrustumCover::CalcIntersection(Plane, TopLeftRay, OnRay);
	auto Intersection2 = FrustumCover::CalcIntersection(Plane, TopRightRay, OnRay);
	auto Intersection3 = FrustumCover::CalcIntersection(Plane, BottomLeftRay, OnRay);
	auto Intersection4 = FrustumCover::CalcIntersection(Plane, BottomRightRay, OnRay);

	// Transform the intersections to the actor coordinate system
	Intersection = this->GetTransform().InverseTransformPosition(Intersection);
	Intersection2 = this->GetTransform().InverseTransformPosition(Intersection2);
	Intersection3 = this->GetTransform().InverseTransformPosition(Intersection3);
	Intersection4 = this->GetTransform().InverseTransformPosition(Intersection4);
	
	// Draw the intersection quad
	QuadrilateralDrawer->ClearOldQuadrilaterals();
	QuadrilateralDrawer->AddQuadrilateral(Intersection, Intersection2, Intersection4, Intersection3, this->DrawFrustum->FrustumColor);
	QuadrilateralDrawer->DrawQuadrilaterals();
}

void AFrustum::ChangeQuadrilateralDrawerVisibility()
{
	if(this->QuadrilateralDrawer->IsVisible())
	{
		this->QuadrilateralDrawer->SetVisibility(false);
	}else
	{
		this->QuadrilateralDrawer->SetVisibility(true);
	}
	
}
