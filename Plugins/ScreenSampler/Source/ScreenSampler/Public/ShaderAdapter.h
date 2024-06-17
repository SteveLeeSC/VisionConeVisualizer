#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShaderAdapter.generated.h"

struct FVertex_Pos_UV
{
	FVector4 Position;
	FVector2D UV;
};

UCLASS(MinimalAPI)
class UShaderAdapter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static void SampleScreen(UTextureRenderTarget2D* OutScreen1, UTextureRenderTarget2D* InScreen, TArray<FVertex_Pos_UV> CornerInfo1);
};