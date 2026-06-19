#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/ProjectPlayerController.h"
#include "Player/ProjectPlayerCharacter.h"
#include "ProjectAnimInstance.generated.h"


UENUM(BlueprintType)
enum class EAnimDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};





/**
 * 仅包含变量的 AnimInstance 基类
 */
UCLASS()
class P20260324_API UProjectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// ========== 获取主 AnimInstance (用于 Layer 蓝图读取数据) ==========
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UProjectAnimInstance* GetMainAnimInstance(const UAnimInstance* LayerAnimInstance);

	// ========== LockOn Data ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|LockOn")
	bool bIsLocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|LockOn")
	float LockingRadius;
};
