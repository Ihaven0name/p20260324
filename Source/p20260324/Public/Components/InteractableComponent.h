#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

/** 可交互对象的组件数据与入口；具体交互逻辑仍由对象实现 UInteractableInterface。 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class P20260324_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 交互提示中显示的名称。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="交互")
	FText InteractionDisplayName;

	/** 关闭后对象仍实现接口，但暂时不会被玩家选中或交互。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="交互")
	bool bCanInteract = true;

	UFUNCTION(BlueprintCallable, Category="交互")
	bool CanInteractWith(AActor* InteractingActor) const;

	/** 玩家交互组件通过此函数与目标组件通信，再转发到目标接口。 */
	UFUNCTION(BlueprintCallable, Category="交互")
	bool TryInteract(AActor* InteractingActor);
};
