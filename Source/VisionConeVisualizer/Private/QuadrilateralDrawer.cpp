// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadrilateralDrawer.h"
#include "ProceduralMeshComponent.h"

// Sets default values
UQuadrilateralDrawer::UQuadrilateralDrawer(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UQuadrilateralDrawer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UQuadrilateralDrawer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UQuadrilateralDrawer::AddQuadrilateralInEditor()
{
	QuadrilateralToAdd.CalcNormal();
	Quadrilaterals.Push(QuadrilateralToAdd);
}

void UQuadrilateralDrawer::AddQuadrilateral(const FVector& A, const FVector& B, const FVector& C, const FVector& D,
                                            const FLinearColor& Color)
{
	Quadrilaterals.Emplace(A, B, C, D, Color);
}

void UQuadrilateralDrawer::DrawQuadrilaterals()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FLinearColor> Colors;
	TArray<FVector2D> UVs;

	for (auto& Quadrilateral : Quadrilaterals) {
		auto Vertices_ = Quadrilateral.GetVertices();
		auto Triangles_ = Quadrilateral.GetTriangles();
		auto Normals_ = Quadrilateral.GetNormals();
		auto Colors_ = Quadrilateral.GetColor();
		auto UVs_ = Quadrilateral.GetUVs();

		auto Offset = Vertices.Num();
		Vertices.Append(Vertices_);
		for (auto& Triangle : Triangles_) {
			Triangles.Add(Triangle + Offset);
		}
		Normals.Append(Normals_);
		Colors.Append(Colors_);
		UVs.Append(UVs_);
	}

	this->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, TArray<FProcMeshTangent>(), false);

	// Create a unlit material
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(
					LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/Materials/ColorMaterial.ColorMaterial'")),
					this);
	Material->SetVectorParameterValue("Emissive Color", FLinearColor(1.0, 1.0, 0.0, 1.0));
	this->SetMaterial(0, Material);
	//this->OverrideMaterials.Add();
}

