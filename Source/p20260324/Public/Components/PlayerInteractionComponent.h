#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerInteractionTargetChanged, AActor*, PreviousTarget, AActor*, NewTarget);

/** 玩家本地的交互扫描器：负责发现候选对象、维持当前目标和滚轮切换。 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class P20260324_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();

	/** 交互检测的总扇形角度；30 度表示朝向两侧各 15 度。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="交互", meta=(ClampMin="0.0", ClampMax="180.0"))
	float InteractionConeAngleDegrees = 30.0f;

	/** 交互检测半径，单位为厘米。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="交互", meta=(ClampMin="0.0"))
	float InteractionDetectionRadius = 500.0f;

	/** 刷新候选对象列表的间隔，单位为秒。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="交互", meta=(ClampMin="0.01"))
	float InteractionScanInterval = 0.1f;

	/** 当前选择的交互对象；目标仍在半径内且可交互时会保持选择。 */
	UPROPERTY(BlueprintReadOnly, Category="交互")
	TObjectPtr<AActor> CurrentInteractableActor = nullptr;

	/** 当前扇形中的候选对象，按距离从近到远排列。 */
	UPROPERTY(BlueprintReadOnly, Category="交互")
	TArray<TObjectPtr<AActor>> DetectedInteractableActors;

	UPROPERTY(BlueprintAssignable, Category="交互")
	FOnPlayerInteractionTargetChanged OnTargetChanged;

	/** 根据本地控制状态启动或停止扫描；玩家控制器变化时由角色调用。 */
	void RefreshScanningState();

	/** 立即重新扫描候选对象。 */
	void ScanForInteractables();

	/** 按滚轮方向切换目标；正数为下一个，负数为上一个。 */
	void CycleTarget(int32 Direction);

	UFUNCTION(BlueprintPure, Category="交互")
	AActor* GetCurrentTarget() const;

	UFUNCTION(BlueprintPure, Category="交互")
	FText GetCurrentTargetDisplayName() const;

	/** 在服务端验证目标并通过目标的 InteractableComponent 执行交互。 */
	bool CanInteractWithActor(const AActor* Target) const;
	bool TryInteractWithActor(AActor* Target) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SetCurrentTarget(AActor* NewTarget);
	void StopScanning();

	FTimerHandle InteractableScanTimerHandle;
};
