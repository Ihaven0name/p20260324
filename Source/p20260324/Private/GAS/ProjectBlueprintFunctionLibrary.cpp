#include "GAS/ProjectBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommonInputSubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/AudioManager.h"
#include "Manager/ConfigManager.h"
#include "Manager/InputManager.h"
#include "Manager/InventoryManager.h"
#include "Manager/LevelManager.h"
#include "Manager/ObjectPoolManager.h"
#include "Manager/SaveManager.h"
#include "Manager/UIManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/TeamManager.h"
#include "GAS/ProjectGameplayEffectType.h"
#include "GAS/ProjectGameplayTag.h"
#include "Input/CommonUIInputSettings.h"
#include "p20260324/LogChannel.h"

#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectPlayerController.h"
#include "UI/HUD/ProjectAttackHUD.h"
#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "UI/WidgetController/SettingUIWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "UserSettings/EnhancedInputUserSettings.h"

class UEnhancedInputUserSettings;
class UEnhancedInputLocalPlayerSubsystem;
struct FFindFloorResult;
class UCharacterMovementComponent;

float UProjectBlueprintFunctionLibrary::GetDistanceToGround(const AActor* Actor)
{
	if (!Actor) return -1.0f;
	if (const ACharacter* Character=Cast<ACharacter>(Actor))
	{
		if (const UCharacterMovementComponent* CMC = Character->GetCharacterMovement())
		{
			if (CMC->IsMovingOnGround())
			{
				return 0.0f;
			}
        
			const FFindFloorResult& CurrentFloor = CMC->CurrentFloor;
			if (CurrentFloor.IsWalkableFloor())
			{
				float Distance = Character->GetActorLocation().Z - CurrentFloor.HitResult.ImpactPoint.Z;
				UE_LOG(LogTemp, Warning, TEXT("Distance is %f"), Distance);
				return FMath::Max(0.0f, Distance);
			}
		}
	}
	
    
	// 射线检测
	UWorld* World = Actor->GetWorld();
	if (!World) return -1.0f;
    
	FVector Start = Actor->GetActorLocation();
	FVector End = Start - FVector(0, 0, 10000.0f);
    
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Actor);
    
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		float Distance = Start.Z - Hit.ImpactPoint.Z;
		return FMath::Max(0.0f, Distance);
	}
    
	return 10000.0f;
}

UAudioManager* UProjectBlueprintFunctionLibrary::GetAudioManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UAudioManager>() : nullptr;
}

UConfigManager* UProjectBlueprintFunctionLibrary::GetConfigManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UConfigManager>() : nullptr;
}

UInputManager* UProjectBlueprintFunctionLibrary::GetInputManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UInputManager>() : nullptr;
}

UInventoryManager* UProjectBlueprintFunctionLibrary::GetInventoryManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UInventoryManager>() : nullptr;
}

ULevelManager* UProjectBlueprintFunctionLibrary::GetLevelManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<ULevelManager>() : nullptr;
}

UObjectPoolManager* UProjectBlueprintFunctionLibrary::GetObjectPoolManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UObjectPoolManager>() : nullptr;
}

USaveManager* UProjectBlueprintFunctionLibrary::GetSaveManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<USaveManager>() : nullptr;
}

UUIManager* UProjectBlueprintFunctionLibrary::GetUIManager(const APlayerController* PlayerController)
{
	if (!PlayerController) return nullptr;
	const ULocalPlayer* LocalPlayer=PlayerController->GetLocalPlayer();
	if (!LocalPlayer) return nullptr;
	return LocalPlayer->GetSubsystem<UUIManager>();
}


UEnemyManager* UProjectBlueprintFunctionLibrary::GetEnemyManager(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!World) return nullptr;
	UGameInstance* GI = UGameplayStatics::GetGameInstance(World);
	return GI ? GI->GetSubsystem<UEnemyManager>() : nullptr;
}

UTeamManager* UProjectBlueprintFunctionLibrary::GetTeamManager(const APlayerController* PlayerController)
{
	if (!PlayerController) return nullptr;
	const ULocalPlayer* LocalPlayer=PlayerController->GetLocalPlayer();
	if (!LocalPlayer) return nullptr;
	return LocalPlayer->GetSubsystem<UTeamManager>();
}


void UProjectBlueprintFunctionLibrary::CalculateCurrentLevel(
	UProjectAttributeSet* AttributeSet,
	int32 TempXP,
	int& OutLevel,
	int& OutXP
)
{
	if (!AttributeSet)
	{
		UE_LOG(LogProject,Error,TEXT("In Calculate Current Level AttributeSet Owner Is Not AProjectPlayerCharacter"))
		return;
	}
	AActor* AttributeSetOwner=AttributeSet->GetActorInfo()->AvatarActor.Get();
	if (AttributeSetOwner->Implements<UPlayerInterface>())
	{
		UConfigManager* ConfigManager= GetConfigManager(AttributeSetOwner);
		const FString ShortName = StaticEnum<EPlayerCategory>()->GetNameStringByValue(static_cast<uint32>(IPlayerInterface::Execute_GetPlayerCategory(AttributeSetOwner)));
		const FString StateString_Double = FString::Printf(TEXT("State.%s.XP.Double"),*ShortName);
		const FString StateString_Triple = FString::Printf(TEXT("State.%s.XP.Triple"),*ShortName);
		FGameplayTagContainer OwnedTags;
		AttributeSet->GetOwningAbilitySystemComponent()->GetOwnedGameplayTags(OwnedTags);
		if (OwnedTags.HasTagExact(FGameplayTag::RequestGameplayTag(*StateString_Triple)))
		{
			TempXP*=3;
		}
		else if (OwnedTags.HasTagExact(FGameplayTag::RequestGameplayTag(*StateString_Triple)))
		{
			TempXP*=2;
		}
		OutLevel=AttributeSet->GetLevel();
		OutXP=AttributeSet->GetXP()+TempXP;
		while (OutLevel<ConfigManager->PlayerLevelBound)
		{
			const int NeedXP=ConfigManager->GetNeedXPAtLevel(OutLevel);
			if (OutXP>=NeedXP)
			{
				OutXP-=NeedXP;
				OutLevel+=1;
			}
			else
			{
				break;
			}
		}
		if (OutLevel==ConfigManager->PlayerLevelBound)
		{
			OutXP=0;
		}
	}
	else
	{
		UE_LOG(LogProject,Error,TEXT("In Calculate Current Level AttributeSet Owner Is Not AProjectPlayerCharacter"))
	}
}

AActor* UProjectBlueprintFunctionLibrary::FindNearestActor(const AActor* OriginalActor, const TArray<AActor*>& TargetActors)
{
	if (!OriginalActor || TargetActors.Num() == 0) return nullptr;

	AActor* NearestActor = nullptr;
	float MinDistance = FLT_MAX;
	const FVector OriginLocation = OriginalActor->GetActorLocation();
	for (AActor* Actor : TargetActors)
	{
		if (!Actor || !IsValid(Actor))
		{
			continue;
		}
		const float Distance = FVector::Dist(OriginLocation, Actor->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestActor = Actor;
		}
	}
	return NearestActor;
}

bool UProjectBlueprintFunctionLibrary::CanActivateAbilityAndOutputSpec(UAbilitySystemComponent* ASC, const FGameplayTag& AbilityTag, FGameplayAbilitySpec& OutSpec)
{
	if (!ASC || !AbilityTag.IsValid()) return false;

	FScopedAbilityListLock ScopedAbilityListLock(*ASC);
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		UGameplayAbility* Temp = Spec.GetPrimaryInstance();
		if (Temp->AbilityTags.HasTagExact(AbilityTag))
		{
			if (Spec.GetPrimaryInstance()->CanActivateAbility(Spec.Handle, ASC->AbilityActorInfo.Get()))
			{
				OutSpec = Spec;
				return true;
			}
			return false;
		}
	}
	return false;
}


void UProjectBlueprintFunctionLibrary::SetKnockBackImpulse(FGameplayEffectContextHandle& ContextHandle, const FVector& InKnockBackImpulse)
{
	if (FProjectGameplayEffectContext* ProjectContext = static_cast<FProjectGameplayEffectContext*>(ContextHandle.Get()))
	{
		ProjectContext->SetKnockBackImpulse(InKnockBackImpulse);
	}
}

FVector UProjectBlueprintFunctionLibrary::GetKnockBackImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FProjectGameplayEffectContext* ProjectContext = static_cast<const FProjectGameplayEffectContext*>(ContextHandle.Get()))
	{
		return ProjectContext->GetKnockBackImpulse();
	}
	return FVector::ZeroVector;
}

FGameplayEffectContextHandle UProjectBlueprintFunctionLibrary::ApplyDamageGameplayEffectByDamageEffectParam(const FDamageEffectParam& DamageEffectParam)
{
	const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();

	UAbilitySystemComponent* SourceASC = DamageEffectParam.SourceAbilitySystemComponent;
	UAbilitySystemComponent* TargetASC = DamageEffectParam.TargetAbilitySystemComponent;
	const TSubclassOf<UGameplayEffect>& DamageEffectClass = DamageEffectParam.DamageEffectClass;
	const int32 Level = DamageEffectParam.AbilityLevel;

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();

	if (DamageEffectParam.KnockBackChance>FMath::RandRange(0.f,1.f))
		SetKnockBackImpulse(ContextHandle, DamageEffectParam.KnockBackImpulse);

	ContextHandle.AddSourceObject(SourceASC->GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, Level, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Caller_DamageMultiplier, DamageEffectParam.DamageMultiplier);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Caller_Debuff_Data_Damage, DamageEffectParam.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Caller_Debuff_Data_Chance, DamageEffectParam.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Caller_Debuff_Data_Duration, DamageEffectParam.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Caller_Debuff_Data_Frequency, DamageEffectParam.DebuffFrequency);

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

	return ContextHandle;
}


UMainAttackUIWidgetController* UProjectBlueprintFunctionLibrary::GetMainAttackUIWidgetController(const AProjectPlayerController* InPlayerController)
{
	return GetUIManager(InPlayerController)->GetMainAttackUIWidgetController(const_cast<AProjectPlayerController*>(InPlayerController));
}

USettingUIWidgetController* UProjectBlueprintFunctionLibrary::GetSettingUIWidgetController(const AProjectPlayerController* InPlayerController)
{
	return GetUIManager(InPlayerController)->GetSettingUIWidgetController(const_cast<AProjectPlayerController*>(InPlayerController));
}

UAttributeWidgetController* UProjectBlueprintFunctionLibrary::GetAttributeUIWidgetController(const AProjectPlayerController* InPlayerController)
{
	return GetUIManager(InPlayerController)->GetAttributeWidgetController(const_cast<AProjectPlayerController*>(InPlayerController));
}

UInventoryUIWidgetController* UProjectBlueprintFunctionLibrary::GetInventoryUIWidgetController(const AProjectPlayerController* InPlayerController)
{
	return GetUIManager(InPlayerController)->GetInventoryUIWidgetController(const_cast<AProjectPlayerController*>(InPlayerController));
}

UProjectBaseWidgetController* UProjectBlueprintFunctionLibrary::GetWidgetControllerByWidgetTag(const AProjectPlayerController* InPlayerController, const FGameplayTag& WidgetTag)
{
	return GetUIManager(InPlayerController)->GetWidgetControllerByWidgetTag(WidgetTag, const_cast<AProjectPlayerController*>(InPlayerController));
}

TSoftObjectPtr<UTexture2D> UProjectBlueprintFunctionLibrary::GetTeamCharacterTexture(const FTeamCharacterInfo& TeamCharacterInfo)
{
	return TeamCharacterInfo.TeamStaticInfo.CharacterTexture;
}

void UProjectBlueprintFunctionLibrary::RebindOnCommonUIAction(
    APlayerController* PC,
    FUIActionTag ActionTag,
    FName MappingName,
    FKey NewKey
)
{
    if (!PC) return;

    // --- 1. 修改 Enhanced Input (IMC 部分) ---
    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings())
            {
                FMapPlayerKeyArgs Args = {};
                Args.Slot = EPlayerMappableKeySlot::First;
            	Args.MappingName = MappingName;
                Args.NewKey = NewKey;

                FGameplayTagContainer FailureReason;
                UserSettings->MapPlayerKey(Args, FailureReason);
                UserSettings->ApplySettings();
                UserSettings->SaveSettings();
            }
        }
    }

    // --- 2. 修改 CommonUI Settings (利用 ActionOverrides 覆盖层) ---
    UCommonUIInputSettings* Settings = const_cast<UCommonUIInputSettings*>(&UCommonUIInputSettings::Get());
    UClass* SettingsClass = UCommonUIInputSettings::StaticClass();

    // 获取两个关键数组的属性
    FArrayProperty* OverridesProp = FindFieldChecked<FArrayProperty>(SettingsClass, "ActionOverrides");
    FArrayProperty* InputActionsProp = FindFieldChecked<FArrayProperty>(SettingsClass, "InputActions");

    FScriptArrayHelper OverridesHelper(OverridesProp, OverridesProp->ContainerPtrToValuePtr<void>(Settings));
    FScriptArrayHelper ActionsHelper(InputActionsProp, InputActionsProp->ContainerPtrToValuePtr<void>(Settings));

	UScriptStruct* ActionStruct = CastChecked<UScriptStruct>(OverridesProp->Inner->GetOwnerStruct());
	
    uint8* TargetOverridePtr = nullptr;

    // A. 先在 ActionOverrides 中寻找是否已有该 Tag 的覆盖
    for (int32 i = 0; i < OverridesHelper.Num(); ++i)
    {
        uint8* Ptr = OverridesHelper.GetRawPtr(i);
        FUIActionTag* Tag = FindFieldChecked<FStructProperty>(ActionStruct, "ActionTag")->ContainerPtrToValuePtr<FUIActionTag>(Ptr);
        if (Tag && *Tag == ActionTag)
        {
            TargetOverridePtr = Ptr;
            break;
        }
    }

    // B. 如果没有覆盖，从 InputActions 拷贝一份基础模板到 Overrides
    if (!TargetOverridePtr)
    {
        for (int32 i = 0; i < ActionsHelper.Num(); ++i)
        {
        	uint8* SourcePtr = ActionsHelper.GetRawPtr(i);
            FUIActionTag* Tag = FindFieldChecked<FStructProperty>(ActionStruct, "ActionTag")->ContainerPtrToValuePtr<FUIActionTag>(SourcePtr);
            if (Tag && *Tag == ActionTag)
            {
                int32 NewIdx = OverridesHelper.AddValue();
                TargetOverridePtr = OverridesHelper.GetRawPtr(NewIdx);
                // 深度拷贝整个结构体内容
            	ActionStruct->CopyScriptStruct(TargetOverridePtr, SourcePtr, 1);
                break;
            }
        }
    }

    // C. 对查找到或创建的 Override 条目进行按键更新
    if (TargetOverridePtr)
    {
        FArrayProperty* MappingsProp = FindFieldChecked<FArrayProperty>(ActionStruct, "KeyMappings");
        FScriptArrayHelper MappingsHelper(MappingsProp, MappingsProp->ContainerPtrToValuePtr<void>(TargetOverridePtr));
        UStruct* MappingStruct = CastFieldChecked<FStructProperty>(MappingsProp->Inner)->Struct; // FUIActionKeyMapping

        bool bFoundAndUpdated = false;
        bool bIsGamepad = NewKey.IsGamepadKey();

        for (int32 j = 0; j < MappingsHelper.Num(); ++j)
        {
            uint8* MappingPtr = MappingsHelper.GetRawPtr(j);
            FKey* KeyPtr = FindFieldChecked<FStructProperty>(MappingStruct, "Key")->ContainerPtrToValuePtr<FKey>(MappingPtr);
            if (KeyPtr && KeyPtr->IsGamepadKey() == bIsGamepad)
            {
                *KeyPtr = NewKey;
                bFoundAndUpdated = true;
                break;
            }
        }

        if (!bFoundAndUpdated)
        {
        	int32 NewIdx = MappingsHelper.AddValue();
        	uint8* NewMappingPtr = MappingsHelper.GetRawPtr(NewIdx);
    
        	// 重新获取一下新格子的 Key 属性指针
        	FStructProperty* NewKeyProp = FindFieldChecked<FStructProperty>(MappingStruct, "Key");
        	FKey* KeyPtr = NewKeyProp->ContainerPtrToValuePtr<FKey>(NewMappingPtr);
        	if (KeyPtr)
        	{
        		*KeyPtr = NewKey; // 填入新按键
        	}
        }
    }

    // --- 3. 广播刷新 ---
    if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(PC->GetLocalPlayer()))
    {
        // 重新设置输入类型以触发按钮的图标更新逻辑
        InputSubsystem->OnInputMethodChangedNative.Broadcast(InputSubsystem->GetCurrentInputType());
    }

	
    // --- 4. 持久化 ---
    Settings->SaveConfig();
}

void UProjectBlueprintFunctionLibrary::ApplyGameplayEffectSpecToSelfByClass(
	UProjectAbilitySystemComponent* AbilitySystemComponent,
	const TSubclassOf<UGameplayEffect> GameplayEffectClass
)
{
	FGameplayEffectContextHandle EffectContextHandle=AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	const FGameplayEffectSpecHandle EffectSpecHandle=AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

// ==========================================
// Initialize Player Startup Abilities (Offensive / Passive)
// ==========================================
void UProjectBlueprintFunctionLibrary::InitializePlayerStartupAbilities(UAbilitySystemComponent* ASC, EPlayerCategory Category)
{
	if (!ASC) return;
	if (Category == EPlayerCategory::Empty)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializePlayerStartupAbilities: Category is Empty for %s"), *ASC->GetAvatarActor()->GetName());
		return;
	}

	UWorld* World = ASC->GetWorld();
	if (!World) return;

	UConfigManager* CM = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!CM) return;

	UCharacterInfo* CharInfo = CM->GetCharacterInfo();
	if (!CharInfo) return;

	FSpecialPlayerInfo Info;
	if (CharInfo->FindSpecialPlayerInfoByCategory(Category,Info))
	{
		// 1. 通用 + Category 特有：主动技能（Offensive）
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharInfo->GetCommonPlayerStartupAbilities_Offensive())
		{
			if (AbilityClass)
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		}
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : Info.SpecialStartUpAbilities_Offensive)
		{
			if (AbilityClass)
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		}

		// 2. 通用 + Category 特有：被动技能（Passive）— GiveAbilityAndActivateOnce
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharInfo->GetCommonPlayerStartupAbilities_Passive())
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : Info.SpecialStartUpAbilities_Passive)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
	}
}

// ==========================================
// Initialize Player Attribute By Effect
// ==========================================
void UProjectBlueprintFunctionLibrary::InitializePlayerAttributeByEffect(UAbilitySystemComponent* ASC, EPlayerCategory Category)
{
	if (!ASC) return;
	if (Category == EPlayerCategory::Empty)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializePlayerAttributeByEffect: Category is Empty for %s"), *ASC->GetAvatarActor()->GetName());
		return;
	}

	UWorld* World = ASC->GetWorld();
	if (!World) return;

	UConfigManager* CM = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!CM) return;

	UCharacterInfo* CharInfo = CM->GetCharacterInfo();
	if (!CharInfo) return;

	FSpecialPlayerInfo Info;
	if (CharInfo->FindSpecialPlayerInfoByCategory(Category,Info))
	{
		const AActor* AvatarActor = ASC->GetAvatarActor();

		// 1. 初始化 Level 属性
		if (Info.LevelAttributeEffectClass)
		{
			FGameplayEffectContextHandle LevelHandle = ASC->MakeEffectContext();
			LevelHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle LevelSpecHandle = ASC->MakeOutgoingSpec(Info.LevelAttributeEffectClass, 1, LevelHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*LevelSpecHandle.Data.Get());
		}

		// 2. 应用 MMC Attribute GE（基于已设好的 Level，曲线计算 MaxHealth/MaxMana 等属性）
		if (Info.MMCAttributeEffectClass)
		{
			FGameplayEffectContextHandle MMCHandle = ASC->MakeEffectContext();
			MMCHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle MMCSpecHandle = ASC->MakeOutgoingSpec(Info.MMCAttributeEffectClass, 1, MMCHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*MMCSpecHandle.Data.Get());
		}

		// 3. 应用 Other Override Attribute GE（设置 Health/Mana 等，依赖 MaxHealth/MaxMana 已正确初始化）
		if (Info.OtherAttributeEffectClass)
		{
			FGameplayEffectContextHandle OtherAttributeHandle = ASC->MakeEffectContext();
			OtherAttributeHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle OtherAttributeSpecHandle = ASC->MakeOutgoingSpec(Info.OtherAttributeEffectClass, 1, OtherAttributeHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*OtherAttributeSpecHandle.Data.Get());
		}
	}
}

// ==========================================
// Initialize Enemy Startup Abilities (Offensive / Passive)
// ==========================================
void UProjectBlueprintFunctionLibrary::InitializeEnemyStartupAbilities(UAbilitySystemComponent* ASC, EEnemyCategory Category)
{
	if (!ASC) return;
	if (Category == EEnemyCategory::Empty)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeEnemyStartupAbilities: Category is Empty for %s"), *ASC->GetAvatarActor()->GetName());
		return;
	}

	UWorld* World = ASC->GetWorld();
	if (!World) return;

	UConfigManager* CM = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!CM) return;

	UCharacterInfo* CharInfo = CM->GetCharacterInfo();
	if (!CharInfo) return;

	FSpecialEnemyInfo Info;
	if (CharInfo->FindSpecialEnemyInfoByCategory(Category,Info))
	{
		// 1. 通用 + Category 特有：主动技能（Offensive）
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharInfo->GetCommonEnemyStartupAbilities_Offensive())
		{
			if (AbilityClass)
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		}
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : Info.SpecialStartUpAbilities_Offensive)
		{
			if (AbilityClass)
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
		}

		// 2. 通用 + Category 特有：被动技能（Passive）— GiveAbilityAndActivateOnce
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharInfo->GetCommonEnemyStartupAbilities_Passive())
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : Info.SpecialStartUpAbilities_Passive)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				ASC->GiveAbilityAndActivateOnce(Spec);
			}
		}
	}
}

// ==========================================
// Initialize Enemy Attribute By Effect
// 目前对于怪物的属性初始化来说，确实可以使用一个Instant的Override GE来确定属性，但是如果后面有新的需求，比如说一系列属性随着某些变化随之变化，可能需要对于类似的某些属性设置为一个Infinite的GE
// ==========================================
void UProjectBlueprintFunctionLibrary::InitializeEnemyAttributeByEffect(UAbilitySystemComponent* ASC, EEnemyCategory Category, int32 EnemyLevel)
{
	if (!ASC) return;
	if (Category == EEnemyCategory::Empty)
	{
		UE_LOG(LogTemp, Error, TEXT("InitializeEnemyAttributeByEffect: Category is Empty for %s"), *ASC->GetAvatarActor()->GetName());
		return;
	}

	UWorld* World = ASC->GetWorld();
	if (!World) return;

	UConfigManager* CM = World->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!CM) return;

	UCharacterInfo* CharInfo = CM->GetCharacterInfo();
	if (!CharInfo) return;

	FSpecialEnemyInfo Info;
	if (CharInfo->FindSpecialEnemyInfoByCategory(Category,Info))
	{
		const AActor* AvatarActor = ASC->GetAvatarActor();
		const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();

		// 1. 初始化 Level 属性（通过 SetByCaller 传入怪物实际等级）
		if (Info.LevelAttributeEffectClass)
		{
			FGameplayEffectContextHandle LevelHandle = ASC->MakeEffectContext();
			LevelHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle LevelSpecHandle = ASC->MakeOutgoingSpec(Info.LevelAttributeEffectClass, 1, LevelHandle);

			// 通过 SetByCaller 传入怪物实际等级（GE 内 Level Modifier 选 SetByCaller 类型，Tag 用 Caller.Attribute.Primary.Level）
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(LevelSpecHandle, GameplayTags.Caller_Attribute_Primary_Level, static_cast<float>(EnemyLevel));

			ASC->ApplyGameplayEffectSpecToSelf(*LevelSpecHandle.Data.Get());
		}

		// 2. 应用 Level Based Attribute GE（基于已设好的 Level，曲线计算 MaxHealth/MaxMana 等属性）
		if (Info.LevelBasedAttributeEffectClass)
		{
			FGameplayEffectContextHandle LevelBasedHandle = ASC->MakeEffectContext();
			LevelBasedHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle LevelBasedSpecHandle = ASC->MakeOutgoingSpec(Info.LevelBasedAttributeEffectClass, 1, LevelBasedHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*LevelBasedSpecHandle.Data.Get());
		}

		// 3. 应用 Other Override Attribute GE（设置 Health/Mana 等，依赖 MaxHealth/MaxMana 已正确初始化）
		if (Info.OtherAttributeEffectClass)
		{
			FGameplayEffectContextHandle OtherAttributeHandle = ASC->MakeEffectContext();
			OtherAttributeHandle.AddSourceObject(AvatarActor);
			FGameplayEffectSpecHandle OtherAttributeSpecHandle = ASC->MakeOutgoingSpec(Info.OtherAttributeEffectClass, 1, OtherAttributeHandle);
			ASC->ApplyGameplayEffectSpecToSelf(*OtherAttributeSpecHandle.Data.Get());
		}
	}
}
