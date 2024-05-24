// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewFrustumCover.h"
#include "Templates/Sorting.h"
#include <climits>

namespace FrustumCover {
	constexpr float GFlt_Eps = 1e-9f;

	bool FloatEqual(const float& a, const float& b) {
		return abs(a - b) < GFlt_Eps;
	}


	TArray<FVector2D> ConvexHull(TArray<FVector2D> Points) {
		
		TArray<FVector2D> Result; // Also used as the stack during the algo
		if (Points.Num() == 0) {
			return MoveTemp(Result);
		}
		FVector2D P0 = Points[0]; 
		int32 Min_index = 0;
		// The point with the smallest Y value must be in the Convex Hull
		for (auto i = 1; i < Points.Num(); i ++) {
			if (Points[i].Y < P0.Y) {
				P0 = Points[i];
				Min_index = i;
			}
		}
		Result.Push(P0);
		Swap(Points[Points.Num() - 1], Points[Min_index]);
		Points.Pop();
		
		Sort(Points.GetData(), Points.Num() - 1,
			[&P0](const FVector2D& a, const FVector2D& b) -> bool {
				auto cross =  FVector2D::CrossProduct(a - P0, b - P0);
				if (!FloatEqual(cross, 0.0f)) {
					return cross > 0.0f;
				}
				return FVector2D::DistSquared(a, P0) < FVector2D::DistSquared(b, P0);
			});

		// The points that have the smallest vector angle from P0 is in the Convex Hull
		Result.Push(Points[0]);

		// Graham scan:
		FVector2D Top = Result.Top(), SubTop = Result[Result.Num() - 1];
		for (auto i = 1; i < Points.Num(); i++) {
			while (Result.Num() >= 2 && FVector2D::CrossProduct(Top - SubTop, Points[i] - SubTop) > GFlt_Eps) {
				// The point at top is not in the Convex Hull.
				Result.Pop();
				Top = Result.Top(), SubTop = Result[Result.Num() - 1];
			}
			Result.Push(Points[i]);
		}

		return MoveTemp(Result);
	}

	float VectorLengthInDirection(const FVector2D& a, const FVector2D& direc) {
		return FVector2D::DotProduct(a, direc) / direc.Size();
	}
	
	FScreenInfo FindBestCircumscribedRectangleForAConvexHull
		(FVector2D Mid_point, TArray<FVector2D>& Hull_points, bool limit_aspect=true, float aspect = 1920.0f / 1080.0f) {
		FScreenInfo ScreenInfo;
		// TODO:
		// Iteration set up
		static int iter_cnt = 0;
		FVector2D screen_wide_edge_direction(1.0f, 0.0f);
		float rotate_step = 0.0001;
		float rotation_angle_in_total = 0.0;
		ScreenInfo.Width = 10000.0;
		ScreenInfo.Height = 10000.0;
		ScreenInfo.ScreenWideEdgeDir = FVector2D(1.0f, 0.0f);
		iter_cnt = 0;
		while (rotation_angle_in_total < PI){
			iter_cnt += 1;
			// Iter preparation
			float tmp_height = 0.0;
			float tmp_width = 0.0;
			// calculate the width of the edge that covers the hull
			
			for(auto i : Hull_points){
				// Print iand the point
				// print(i, hull_points[i])
				auto tmp = VectorLengthInDirection(i - Mid_point, screen_wide_edge_direction);
				// print(i, hull_points[i], tmp)
				if(tmp > tmp_width){
					tmp_width = tmp;
				}
			}
			// calculate the height of the edge that covers the hull
			
			for (auto i : Hull_points) {
				auto tmp = VectorLengthInDirection(i - Mid_point, FVector2D(screen_wide_edge_direction.Y, -screen_wide_edge_direction.X));
					// print(i, hull_points[i], tmp)
				if (tmp > tmp_height) {
					tmp_height = tmp;
				}
			}
			// print(tmp_width, tmp_height)
			// Adapt the tmp_width and tmp_height to the aspect ratio
			if(limit_aspect){
				if(tmp_width / tmp_height > aspect){
					tmp_height = tmp_width / aspect;
				}
				else{
						tmp_width = tmp_height * aspect;
				}
			}
			else {
				// Width should be greater than Height
				if (tmp_width < tmp_height) {
					Swap(tmp_width, tmp_height);
					screen_wide_edge_direction = FVector2D(screen_wide_edge_direction.Y, -screen_wide_edge_direction.X);
				}
			}
			// Record the best result : the smallest area that covers the hull
			if(tmp_width* tmp_height < ScreenInfo.ScreenArea()){
				ScreenInfo.Set(tmp_width, tmp_height, screen_wide_edge_direction);
			}
			screen_wide_edge_direction = screen_wide_edge_direction.GetRotated(rotate_step);
			screen_wide_edge_direction.Normalize();
			rotation_angle_in_total += rotate_step;
		}
		return ScreenInfo;
	}
	
	FVector CalcIntersection(const FMPlane& Plane, const FMRay& Ray, bool& IfOnRay)
	{
		float Distance = FVector::DotProduct(Plane.GetNormal(), Plane.GetPoint() - Ray.GetStart()) / FVector::DotProduct(Plane.GetNormal(), Ray.GetDirec());
		FVector Intersection = Ray.GetStart() + Distance * Ray.GetDirec();
		IfOnRay = Distance > GFlt_Eps;
		return Intersection;
	}

	bool FCoordinateSys2D::BuildSysOnPlane(const FMPlane& Plane)
	{
		bool Result = true;
		this->CoordSysPlane = Plane;
		this->Normal = Plane.GetNormal();
		this->Normal.Normalize();
		FVector Not_Aligned_Vector(0.0);
		// Find a vector that is not aligned with the normal vector
		if (this->Normal[0] == 0.0f && this->Normal[1] == 0.0f && this->Normal[2] == 0.0f) {
			Result = false;
		}
		if (this->Normal[0] == 0.0f && this->Normal[1] == 0.0f) {
			// The normal vector is aligned with the z - axis
			Not_Aligned_Vector.X = 1.0f;  // Use x - axis as the not aligned vector
		}
		else{
			// Use z - axis as the not aligned vector if the normal vector is not aligned with z - axis
			Not_Aligned_Vector.Z = 1.0f;
		}

		// First axis in the plane
		this->X_Axis = FVector::CrossProduct(this->Normal, Not_Aligned_Vector);
		this->X_Axis.Normalize();

		// Second axis in the plane, orthogonal to both the normal and first_axis
		this->Y_Axis = FVector::CrossProduct(this->Normal, this->X_Axis);
		this->Y_Axis.Normalize();

		this->Centroid = Plane.GetPoint();
		return Result;
	}

	FVector2D FCoordinateSys2D::TransformPointTo2DCoordSys(const FVector Point)const
	{
		FVector2D Point2D;
		FVector tmp = Point - this->Centroid;
		Point2D.X = FVector::DotProduct(tmp, this->X_Axis);
		Point2D.Y = FVector::DotProduct(tmp, this->Y_Axis);
		return Point2D;
	} 

	FVector FCoordinateSys2D::TransformPointFrom2DCoordSys(const FVector2D Point)const
	{
		FVector Point3D = Point.X * this->X_Axis + Point.Y * this->Y_Axis;
		Point3D += this->Centroid;
		return Point3D;
	}

	FVector FCoordinateSys2D::TransformVectorFrom2DCoordSys(const FVector2D Vector)const
	{
		FVector Vector3D;
		Vector3D = Vector.X * this->X_Axis + Vector.Y * this->Y_Axis;
		return Vector3D;
	}

	void FViewFrustum::CalcCorners() {
		auto distance = 1.0f;
		auto ForwardVec = Pos - ZAxis * distance, 
			 LeftVec = YAxis * distance * tanf(VFov[2]), 
			 UpVec = XAxis * distance * tan(VFov[1]);
		this->Corners[0] = ForwardVec + LeftVec - UpVec;
		this->Corners[1] = ForwardVec + LeftVec + UpVec;
		this->Corners[2] = ForwardVec - LeftVec + UpVec;
		this->Corners[3] = ForwardVec - LeftVec - UpVec;
	}


	void FViewFrustum::CalcAxises()
	{
		ZAxis = (Pos - LookAt).GetSafeNormal();
		XAxis = (UpVector ^ ZAxis).GetSafeNormal();
		YAxis = ZAxis ^ XAxis;
	}


	TArray<FVector> CalcInterSections(const FViewFrustum& ViewFrustum, FMPlane& Plane, bool& IfAllOnRays)
	{
		TArray<FVector> Intersections;
		
		IfAllOnRays = true;
		for (auto i = 0; i < 4; i++) {
			bool bTmp;
			FMRay Ray = FMRay(ViewFrustum.GetPos(), ViewFrustum.GetCorner(i) - ViewFrustum.GetPos());
			FVector Intersection = CalcIntersection(Plane, Ray, bTmp);
			if (!bTmp) {
				IfAllOnRays = false;
			}
			Intersections.Push(MoveTemp(Intersection));
		}

		return Intersections;
	}


	//void FViewFrustum::FormFrustum(const FScreenInfo& ScreenInfo)
	//{
	//}
	
}