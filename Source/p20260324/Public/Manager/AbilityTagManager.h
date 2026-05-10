#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AbilityTagManager.generated.h"

struct FAbilityTagConfig;

UCLASS()
class P20260324_API UAbilityTagManager : public UEditorSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void RefreshAbilityTagInCDO();
	static FAbilityTagConfig* FindAbilityTagConfigByRowName(FName RowName);
	static TObjectPtr<UDataTable> AbilityTagConfigDataTable;
	
};
