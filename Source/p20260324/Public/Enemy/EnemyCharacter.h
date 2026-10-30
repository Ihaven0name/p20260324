



#pragma once

#include "CoreMinimal.h"
#include "ProjectBaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

class UProjectAttributeSet;
class UProjectAnimInstance;
class UWidgetComponent;

//只用在EnemyHealth的修改上
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHealthPercentChanged, float, NewPercent);

UCLASS()
class P20260324_API AEnemyCharacter : public AProjectBaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void OnDeathStateChanged() override;
	
public:


	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthPercentChanged OnEnemyHealthPercentChanged;

	/** 怪物分类，用于从 CharacterInfo 中读取对应初始化数据 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EEnemyCategory EnemyCategory = EEnemyCategory::Empty;
	//在创建的时候进行Set by caller，实现方便的设置怪物等级，在需要使用的地方或者SpawnActorDeferred里面修改怪物的等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 EnemyLevel=1;

	/** Enemy 死亡后在服务器端保留的时间，之后自动销毁。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Death", meta = (ClampMin = "0.0"))
	float DeathLifeSpan = 5.0f;



	
	//动画需要的数据
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool bIsChasing=false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool bIsCombating=false;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool bIsIdling=true;

	UPROPERTY(ReplicatedUsing = OnRep_TurnAnimationState, BlueprintReadOnly, Category = "Animation|Turn")
	bool bIsTurnAnimationActive = false;

	UPROPERTY(ReplicatedUsing = OnRep_TurnAnimationState, BlueprintReadOnly, Category = "Animation|Turn")
	float TurnAnimationRotationSpeed = 0.0f;

	/** Server-side AI updates these values to drive the turn BlendSpace on every client. */
	void SetTurnAnimationState(bool bIsActive, float SignedRotationSpeed);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetAIState(bool bNewIsChasing, bool bNewIsCombating, bool bNewIsIdling);

	/** 仅设置敌人的战斗状态。 */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetIsCombating(bool bNewIsCombating);




	
	/** 敌人当前追踪的目标，由 AI 在服务器端设置。 */
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TObjectPtr<AActor> ChasingActor = nullptr;


	//EnemyInterface开始
	virtual void SetTargetActor_Implementation(AActor* NewTargetActor) override;
	virtual AActor* GetTargetActor_Implementation() const override;
	virtual void UpdateHealthBarByPlayerLocation_Implementation(FVector PlayerLocation, bool bIsVisible) override;
	//EnemyInterface结束
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// /** 血条 WidgetComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

	/** 开始缩小血条的距离，单位为厘米；更近时使用最大比例。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|血条距离", meta = (ClampMin = "0.0"))
	float HealthBarMinScaleDistance = 500.0f;

	/** 达到最小比例并隐藏血条的距离，单位为厘米。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|血条距离", meta = (ClampMin = "0.0"))
	float HealthBarMaxScaleDistance = 2000.0f;

	/** 远距离下的血条最小缩放比例。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|血条距离", meta = (ClampMin = "0.0"))
	float HealthBarMinScale = 0.2f;

	/** 近距离下的血条最大缩放比例。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|血条距离", meta = (ClampMin = "0.0"))
	float HealthBarMaxScale = 1.0f;

	void BindHealthWidgetCallbacks();

private:
	UFUNCTION()
	void OnRep_TurnAnimationState();
	void ApplyTurnAnimationStateToAnimInstance();
	void HandleTargetDeath(AActor* DeadActor);
	void UnbindTargetDeath();

	void RegisterWithEnemyManager();

	TWeakObjectPtr<AProjectBaseCharacter> TargetDeathListener;
	FDelegateHandle TargetDeathDelegateHandle;
	FVector2D HealthWidgetBaseDrawSize = FVector2D::ZeroVector;
};
