#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/AbilityBase_Attack.h"
#include "GameplayTagContainer.h"
#include "Ability_Shoot.generated.h"

UCLASS()
class P20260324_API UAbility_Shoot : public UAbilityBase_Attack
{
	GENERATED_BODY()

public:
	UAbility_Shoot();

	/** 射线使用的碰撞通道。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|射线")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1;

	/** 射线最大距离，单位为厘米。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|射线", meta = (ClampMin = "1.0"))
	float TraceRange = 10000.f;

	/** 水平方向随机散布的最大偏转角，实际偏转范围为负值到正值。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|散布", meta = (ClampMin = "0.0", ClampMax = "45.0", Units = "deg"))
	float HorizontalSpreadAngle = 2.0f;

	/** 垂直方向随机散布的最大偏转角，实际偏转范围为负值到正值。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|散布", meta = (ClampMin = "0.0", ClampMax = "45.0", Units = "deg"))
	float VerticalSpreadAngle = 1.5f;

	/** 射线发射点使用的角色网格插槽名称。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|射线")
	FName SocketName;

	/** 动画通知发送此 Gameplay Event Tag 后执行射线检测。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|事件")
	FGameplayTag ShootEventTag;

	/** 是否绘制射击射线调试线。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|调试")
	bool bDrawDebugTrace = true;

	/** 射线调试线显示时长，单位为秒。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "射击|调试", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DebugTraceDuration = 1.5f;

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnShootEventReceived(FGameplayEventData Payload);

	void TraceShotAndNotifyHits();
};
