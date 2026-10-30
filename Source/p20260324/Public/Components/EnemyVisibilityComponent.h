#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyVisibilityComponent.generated.h"

class AActor;

/** 本地玩家控制器上的可见敌人扫描与血条显隐更新器。 */
UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class P20260324_API UEnemyVisibilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyVisibilityComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI|敌人血条", meta=(ClampMin="0.01"))
	float UpdateInterval = 0.1f;

	/** 立即扫描视口并更新可见敌人血条，返回当前可见的敌人。 */
	UFUNCTION(BlueprintCallable, Category="UI|敌人血条")
	void UpdateVisibleEnemies(TArray<AActor*>& OutVisibleEnemies);

	/** PlayerController 接管或切换 Pawn 后调用，启动本地定时扫描。 */
	void RefreshTrackingState();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void UpdateVisibleEnemiesTimer();
	void StopTracking(bool bHidePrevious);
	void HidePreviousEnemies(const FVector& PlayerLocation);

	FTimerHandle UpdateTimerHandle;
	TArray<TWeakObjectPtr<AActor>> PreviousVisibleEnemies;
};
