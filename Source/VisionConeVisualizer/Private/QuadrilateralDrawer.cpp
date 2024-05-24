// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadrilateralDrawer.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AQuadrilateralDrawer::AQuadrilateralDrawer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("QuadrilateralDrawerMesh"), false);
//	RootComponent = MeshComponent;
	SetRootComponent(MeshComponent);
	
}

// Called when the game starts or when spawned
void AQuadrilateralDrawer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuadrilateralDrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuadrilateralDrawer::AddQuadrilateralInEditor()
{
	QuadrilateralToAdd.CalcNormal();
	Quadrilaterals.Push(QuadrilateralToAdd);
}

void AQuadrilateralDrawer::AddQuadrilateral(const FVector& A, const FVector& B, const FVector& C, const FVector& D,
                                            const FLinearColor& Color)
{
	Quadrilaterals.Emplace(A, B, C, D, Color);
}

void AQuadrilateralDrawer::DrawQuadrilaterals()
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

	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, TArray<FProcMeshTangent>(), false);

	// Create a unlit material
	UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(
					LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/Materials/ColorMaterial.ColorMaterial'")),
					this);
	Material->SetVectorParameterValue("Emissive Color", FLinearColor(1.0, 1.0, 0.0, 1.0));
	MeshComponent->SetMaterial(0, Material);
	//MeshComponent->OverrideMaterials.Add();
}

