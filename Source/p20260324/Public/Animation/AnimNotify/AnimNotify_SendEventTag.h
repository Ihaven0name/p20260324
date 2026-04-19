#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SendEventTag.generated.h"

/**
 * 自定义 AnimNotify：在动画到达此帧时发送一个 GameplayEvent Tag
 * 配合 GAS 的 AbilityTask_WaitGameplayEvent 使用，可在蓝图中编辑要发送的 Tag
 */
UCLASS()
class P20260324_API UAN_SendEventTag : public UAnimNotify
{
	GENERATED_BODY()

public:
	/** 要发送的 GameplayEvent Tag（蓝图可编辑） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEvent")
	FGameplayTag EventTag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
