// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "QuadrilateralDrawer.generated.h"

USTRUCT(BlueprintType)
struct FQuadrilateral
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FVector A;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FVector B;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FVector C;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FVector D;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FLinearColor Color;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	TArray<FVector> Normals;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	TArray<FVector2D> UVs;

	/* Default Constructor
	 * Construct a rectangular quadrilateral with vertices at (0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)
	 * Color is White.
	 */
	FQuadrilateral()
		:A(FVector(0, 0, 0)), B(FVector(1, 0, 0)),
		C(FVector(1, 1, 0)), D(FVector(0, 1, 0)),
		Color(FLinearColor::White),
		UVs({ FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1) })
	{
		CalcNormal();
	}

	FQuadrilateral(const FVector& A, const FVector& B, const FVector& C, const FVector& D, const FLinearColor& Color)
		:A(A), B(B), C(C), D(D), Color(Color),
		UVs({ FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1) })
	{
		CalcNormal();
	}
	
	void CalcNormal()
	{
		Normals.SetNum(4);
		Normals[0] = FVector::CrossProduct(B - A, C - A).GetSafeNormal();
		Normals[1] = FVector::CrossProduct(C - B, D - B).GetSafeNormal();
		Normals[2] = FVector::CrossProduct(D - C, A - C).GetSafeNormal();
		Normals[3] = FVector::CrossProduct(A - D, B - D).GetSafeNormal();
	}

	TArray<FVector> GetVertices() const
	{
		return { A, B, C, D };
	}

	static TArray<int32> GetTriangles()
	{
		return { 0, 1, 2, 0, 2, 3 };
	}

	TArray<FVector> GetNormals() const
	{
		return Normals;
	}

	TArray<FLinearColor> GetColor() const
	{
		return { Color, Color, Color, Color };
	}

	TArray<FVector2D> GetUVs() const
	{
		return UVs;
	}
};

UCLASS()
class VISIONCONEVISUALIZER_API UQuadrilateralDrawer : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UQuadrilateralDrawer(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(EditAnywhere, Category = "Quadrilateral")
	//UProceduralMeshComponent* MeshComponent;
	
	TArray<FQuadrilateral> Quadrilaterals;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quadrilateral")
	FQuadrilateral QuadrilateralToAdd;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Quadrilateral")
	void AddQuadrilateralInEditor();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Quadrilateral")
	void ClearOldQuadrilaterals();
	
	UFUNCTION(BlueprintCallable, Category = "Quadrilateral")
	void AddQuadrilateral(const FVector& A, const FVector& B, const FVector& C, const FVector& D, const FLinearColor& Color);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Quadrilateral")
	void DrawQuadrilaterals();
};
