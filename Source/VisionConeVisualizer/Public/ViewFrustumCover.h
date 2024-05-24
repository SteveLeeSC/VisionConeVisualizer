// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "ViewFrustumCover.generated.h"

namespace FrustumCover {
	struct FMRay;

	USTRUCT()
	struct FMPlane {
		private:
			FVector Point;
			FVector Normal;
		public:
			FMPlane() {};
			FMPlane(const FVector& Point, const FVector& Normal):Point(Point) {
				this->Normal = Normal.GetSafeNormal();
			}
			FVector GetNormal() const {
				return Normal;
			}
			FVector GetPoint() const {
				return Point;
			}
		
			
	};

	FVector CalcIntersection(const FMPlane& Plane, const FMRay& Ray, bool& IfOnRay);
	
	USTRUCT()
	struct FCoordinateSys2D {
		private:
			FMPlane CoordSysPlane;
			FVector Centroid;
			FVector X_Axis;
			FVector Y_Axis;
			FVector Normal;
		public:
			FCoordinateSys2D() {};
			bool BuildSysOnPlane(const FMPlane&);
			FVector2D TransformPointTo2DCoordSys(FVector)const;
			FVector TransformPointFrom2DCoordSys(FVector2D)const;
			FVector TransformVectorFrom2DCoordSys(FVector2D)const;
	};


	USTRUCT()
	struct FMRay {
		private:
			FVector Start;
			FVector Direc;
		public:
			FMRay() {};
			FMRay(const FVector Start, FVector Direc) :Start(Start) {
				this->Direc = Direc.GetSafeNormal();
			}

			FVector GetStart() const {
				return Start;
			}

			FVector GetDirec() const {
				return Direc;
			}
	};

	USTRUCT()
	struct FViewFrustum {
		private:
			FVector Pos;
			FVector LookAt;
			FVector UpVector;
			float VFov[4];

			FVector XAxis;
			FVector YAxis;
			FVector ZAxis;

			FVector Corners[4];
		public:
			FViewFrustum(): VFov{0.0}
			{
			}

			FViewFrustum(const FVector Pos, const FVector LookAt, const FVector UpVector, float VFov[4]):
				Pos(Pos), LookAt(LookAt),UpVector(UpVector) 
			{
				for (int i = 0; i < 4; i++) {
					this->VFov[i] = VFov[i];
				}

				this->CalcAxises();
				this->CalcCorners();
			}

			FVector GetPos() const {
				return Pos;
			}

			FVector GetLookAt() const {
				return LookAt;
			}

			FVector GetUpVector() const {
				return UpVector;
			}

			float GetVFov(int i) const {
				return VFov[i];
			}

			FVector GetAxis(int i) const {
				switch (i) {
					case 0:
						return XAxis;
					case 1:
						return YAxis;
					case 2:
						return ZAxis;
					default:
						return FVector();
				}
			}

			FVector GetCorner(int i) const {
				return Corners[i];
			}
		
			void CalcCorners();
			void CalcAxises();
			
			//void FormFrustum(const FScreenInfo& ScreenInfo);
	};
	TArray<FVector> CalcInterSections(const FViewFrustum& ViewFrustum, const FMPlane& Plane, bool& IfAllOnRays);
	USTRUCT()
	struct FScreenInfo {
		float Width;
		float Height;
		FVector2D ScreenWideEdgeDir;
		
		float ScreenArea() const {
			return Width * Height;
		}
		
		void Set(const float& OutWidth, const float& OutHeight, FVector2D OutScreenWideEdgeDir) {
			this->Width = OutWidth;
			this->Height = OutHeight;
			this->ScreenWideEdgeDir = OutScreenWideEdgeDir;
		}
	};
	// Coordinate System things: Build2DCoordSys, Transformation of points and vector between 2D and 3D ...
	FCoordinateSys2D Build2DCoordinateSysOnPlane(const FMPlane);

	FVector IntersectionBetweenRayAndPlane(const FMPlane, const FRay);
	TArray<FVector2D>* ConvexHull(TArray<FVector2D>&);
	FScreenInfo FindBestCircumscribedRectangleOfConvexHull
				(FVector2D Mid_Point, TArray<FVector2D>& Hull_Points, float Aspect, bool bLimit_Aspect);

	float VectorLengthInDirection(const FVector2D& A, const FVector2D& Direc);
}