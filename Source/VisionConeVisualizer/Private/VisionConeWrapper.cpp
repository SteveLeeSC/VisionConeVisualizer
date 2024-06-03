// Fill out your copyright notice in the Description page of Project Settings.


#include "VisionConeWrapper.h"
#include<string>
using namespace FrustumCover;
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
	// UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

bool AVisionConeWrapper::ShouldTickIfViewportsOnly()const{
	return bEnableEditorTick;
}

void AVisionConeWrapper::RecolorFrustums()
{
	if(Frustum1) Frustum1->OverrideFrustumColor(FColor(255, 0, 0, 255));
	if(Frustum2) Frustum2->OverrideFrustumColor(FColor(0, 255, 0, 255));
	if(FrustumWrap) FrustumWrap->OverrideFrustumColor(FColor(0, 0, 255, 255));
}

void ReadFrustumPara(FString Path, FVector& LookAt, FVector& UpVector, float vFov[]) {
	FILE* file;
	std::string file_path(TCHAR_TO_UTF8(*(Path)));
	if (fopen_s(&file, file_path.c_str(), "rb") == NULL) {
		int len = 0;
		for (int i = 0; i < 3; i++)
		{
			fread(&LookAt[i], sizeof(LookAt[i]), 1, file);
			len += sizeof(LookAt[i]);
		}

		for (int i = 0; i < 3; i++)
		{
			fread(&UpVector[i], sizeof(UpVector[i]), 1, file);
			len += sizeof(UpVector[i]);
		}

		for (int i = 0; i < 4; i++)
		{
			fread(&vFov[i], sizeof(vFov[i]), 1, file);
			len += sizeof(vFov[i]);
		}

		fclose(file);
	}
}
struct Quaternion {
	Quaternion() :x(0), y(0), z(0), w(1) {};
	Quaternion(double x, double y, double z, double w) :x(x), y(y), z(z), w(w) {};

	void normalize() {
		double norm = sqrt(x * x + y * y + z * z + w * w);
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	}

	double norm() {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	double x;
	double y;
	double z;
	double w;

};
///////////////////////////////
// Quaternion to Euler
///////////////////////////////
enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };




void twoaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
	res[0] = atan2(r11, r12);
	res[1] = acos(r21);
	res[2] = atan2(r31, r32);
}

void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double res[]) {
	res[0] = atan2(r31, r32);
	res[1] = asin(r21);
	res[2] = atan2(r11, r12);
}

void quaternion2Euler(const Quaternion& q, double res[], RotSeq rotSeq)
{
	switch (rotSeq) {
	case zyx:
		threeaxisrot(2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			-2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case zyz:
		twoaxisrot(2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			res);
		break;

	case zxy:
		threeaxisrot(-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			res);
		break;

	case zxz:
		twoaxisrot(2 * (q.x * q.z + q.w * q.y),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			res);
		break;

	case yxz:
		threeaxisrot(2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			-2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case yxy:
		twoaxisrot(2 * (q.x * q.y - q.w * q.z),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			res);
		break;

	case yzx:
		threeaxisrot(-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			res);
		break;

	case yzy:
		twoaxisrot(2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			res);
		break;

	case xyz:
		threeaxisrot(-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case xyx:
		twoaxisrot(2 * (q.x * q.y + q.w * q.z),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			res);
		break;

	case xzy:
		threeaxisrot(2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			-2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			res);
		break;

	case xzx:
		twoaxisrot(2 * (q.x * q.z - q.w * q.y),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			res);
		break;
	default:
		//std::cout << "Unknown rotation sequence" << std::endl;
		break;
	}
}

///////////////////////////////
// Helper functions
///////////////////////////////
Quaternion operator*(Quaternion& q1, Quaternion& q2) {
	Quaternion q;
	q.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	q.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	q.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	q.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	return q;
}

FVector QuaternionToVector3(FQuat _q)
{
	FVector v(
		atan((2 * (_q.W * _q.X + _q.Y * _q.Z)) / (1 - 2 * (pow(_q.X, 2) + pow(_q.Y, 2)))),
		asin(2 * (_q.W * _q.Y - _q.Z * _q.X)),
		atan((2 * (_q.W * _q.Z + _q.X * _q.Y)) / (1 - 2 * (pow(_q.Y, 2) + pow(_q.Z, 2))))
	);
	return FVector(v.X * 180 / PI, v.Y * 180 / PI, v.Z * 180 / PI);
}

struct FusionPoint
{
	float u;
	float v;
	float multi;
	float add;
};
typedef struct color
{
	float r;
	float g;
	float b;
}color;

FMatrix makeLookat(const FVector& EyePosition, const FVector& LookAtPosition, const FVector& UpVector)
{
	FMatrix mat;

	const FVector ZAxis = (LookAtPosition - EyePosition).GetSafeNormal();
	const FVector XAxis = (UpVector ^ ZAxis).GetSafeNormal();
	const FVector YAxis = ZAxis ^ XAxis;
	mat.M[0][0] = -XAxis[0];
	mat.M[0][1] = YAxis[0];
	mat.M[0][2] = -ZAxis[0];
	mat.M[0][3] = 0.0;

	mat.M[1][0] = -XAxis[1];
	mat.M[1][1] = YAxis[1];
	mat.M[1][2] = -ZAxis[1];
	mat.M[1][3] = 0.0;

	mat.M[2][0] = -XAxis[2];
	mat.M[2][1] = YAxis[2];
	mat.M[2][2] = -ZAxis[2];
	mat.M[2][3] = 0.0;


	mat.M[3][0] = 0;
	mat.M[3][1] = 0;
	mat.M[3][2] = 0;
	mat.M[3][3] = 1;

	return mat;
}

FQuat getRotate(FMatrix mat)
{
	FQuat q;

	float s;
	float tq[4];
	int    i, j;

	// Use tq to store the largest trace
	tq[0] = 1 + mat.M[0][0] + mat.M[1][1] + mat.M[2][2];
	tq[1] = 1 + mat.M[0][0] - mat.M[1][1] - mat.M[2][2];
	tq[2] = 1 - mat.M[0][0] + mat.M[1][1] - mat.M[2][2];
	tq[3] = 1 - mat.M[0][0] - mat.M[1][1] + mat.M[2][2];

	// Find the maximum (could also use stacked if's later)
	j = 0;
	for (i = 1; i < 4; i++) j = (tq[i] > tq[j]) ? i : j;

	// check the diagonal
	if (j == 0)
	{
		/* perform instant calculation */
		q.W = tq[0];
		q.X = mat.M[1][2] - mat.M[2][1];
		q.Y = mat.M[2][0] - mat.M[0][2];
		q.Z = mat.M[0][1] - mat.M[1][0];
	}
	else if (j == 1)
	{
		q.W = mat.M[1][2] - mat.M[2][1];
		q.X = tq[1];
		q.Y = mat.M[0][1] + mat.M[1][0];
		q.Z = mat.M[2][0] + mat.M[0][2];
	}
	else if (j == 2)
	{
		q.W = mat.M[2][0] - mat.M[0][2];
		q.X = mat.M[0][1] + mat.M[1][0];
		q.Y = tq[2];
		q.Z = mat.M[1][2] + mat.M[2][1];
	}
	else /* if (j==3) */
	{
		q.W = mat.M[0][1] - mat.M[1][0];
		q.X = mat.M[2][0] + mat.M[0][2];
		q.Y = mat.M[1][2] + mat.M[2][1];
		q.Z = tq[3];
	}

	s = sqrt(0.25 / tq[j]);
	q.W *= s;
	q.X *= s;
	q.Y *= s;
	q.Z *= s;

	return q;
}

float Rad2Deg(float rad) {
	return rad * 180 / PI;
}

FRotator CalcRotator(FVector& LookAt, FVector& Up) {
	//FVector XAxis, YAxis, ZAxis;
	//ZAxis = (LookAt).GetSafeNormal();
	//XAxis = (UpVector ^ ZAxis).GetSafeNormal();
	//YAxis = ZAxis ^ XAxis;

	//float yaw = atan2(XAxis[1], XAxis[0]);
	//float pitch = asinf(-XAxis[2]);
	//float roll = atan2(YAxis[2], ZAxis[2]);

	//yaw = Rad2Deg(yaw);
	//pitch = Rad2Deg(pitch);
	//roll = Rad2Deg(roll);

	FMatrix CameraMat = makeLookat(FVector(0, 0, 0), FVector(LookAt[0], LookAt[1], -LookAt[2]), FVector(Up[0], Up[1], -Up[2]));

	FQuat quat = getRotate(CameraMat);
	Quaternion qquat(quat.X, quat.Y, quat.Z, quat.W);
	double HPR[3];
	quaternion2Euler(qquat, HPR, zxy);

	return FRotator(-HPR[1] * 180 / PI, HPR[0] * 180 / PI, HPR[2] * 180 / PI);
}

void CalcFovAndAspect(float vFov[], float& aspect, float& m_Fov) {
	float left = -1 * tanf(vFov[0]);
	float right = 1 * tanf(vFov[1]);
	float top = 1 * tanf(vFov[2]);
	float bottom = -1 * tanf(vFov[3]);
	aspect = right / top;
	m_Fov = (atan(right) - atan(left)) * 180 / PI;

	return;
}

struct FrustumPara {
	FVector LookAt;
	FVector UpVector;
	float vFov[4];
};

void SetFrustum(AFrustum* frustum, FString Path) {
	FrustumPara FrustumPara;
	ReadFrustumPara(Path, FrustumPara.LookAt, FrustumPara.UpVector, FrustumPara.vFov);

	FRotator FrustumRot;
	FrustumRot = CalcRotator(FrustumPara.LookAt, FrustumPara.UpVector);

	float aspect, fov;
	CalcFovAndAspect(FrustumPara.vFov, aspect, fov);

	//frustum->CameraComponent->SetAspectRatio(aspect);
	//frustum->CameraComponent->SetFieldOfView(fov);
	//FRotator ZeroRot(0,0,0);
	//frustum->CameraComponent->SetRelativeRotation(ZeroRot);
	//frustum->CameraComponent->AddRelativeRotation(FrustumRot);

	frustum->AspectRatio = aspect;
	frustum->FOV = fov;
	FRotator ZeroRot(0, 0, 0);
	frustum->SceneComponent->SetRelativeRotation(ZeroRot);
	frustum->SceneComponent->AddRelativeRotation(FrustumRot);

	frustum->DrawThisFrustum();
	return;
}

void AVisionConeWrapper::ReadAndSetFrustums()
{
	SetFrustum(this->Frustum1, this->Frustum1Path);
	SetFrustum(this->Frustum2, this->Frustum2Path);
	SetFrustum(this->FrustumWrap, this->FrustumWrapPath);
}


void AVisionConeWrapper::WarpFrustums() {
	FCoordinateSys2D CoordSys;
	FMPlane Plane;
	CoordSys.BuildSysOnPlane(Plane);
}

void AVisionConeWrapper::DrawPlanes()
{
	this->FrustumWrap->DrawFarPlane();
	this->Frustum1->DrawIntersectionQuadWithFrustumFarPlane(this->FrustumWrap);
	this->Frustum2->DrawIntersectionQuadWithFrustumFarPlane(this->FrustumWrap);
}

