



#pragma once

#include "CoreMinimal.h"
#include "ProjectBaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UProjectAttributeSet;
class UWidgetComponent;

//只用在EnemyHealth的修改上
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHealthPercentChanged, float, NewPercent);

UCLASS()
class P20260324_API AEnemyCharacter : public AProjectBaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthPercentChanged OnEnemyHealthPercentChanged;

	/** 怪物分类，用于从 CharacterInfo 中读取对应初始化数据 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	EEnemyCategory EnemyCategory = EEnemyCategory::Empty;
	//在创建的时候进行Set by caller，实现方便的设置怪物等级，在需要使用的地方或者SpawnActorDeferred里面修改怪物的等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 EnemyLevel=1;

	// /** 血条 WidgetComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UWidgetComponent> HealthWidgetComponent;

	void BindHealthWidgetCallbacks();
};
