#include "GAS/ProjectBlueprintFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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
#include "GAS/ProjectGameplayEffectType.h"
#include "GAS/ProjectGameplayTag.h"
#include "p20260324/LogChannel.h"

#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectPlayerController.h"
#include "UI/HUD/ProjectAttackHUD.h"
#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "UI/WidgetController/SettingUIWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/ProjectWidgetController.h"

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
	AActor* AttributeSetOwner=AttributeSet->GetActorInfo()->OwnerActor.Get();
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
		if (Spec.GetPrimaryInstance()->AbilityTags.HasTagExact(AbilityTag))
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

bool UProjectBlueprintFunctionLibrary::MakeWidgetControllerParam(
	UObject* WorldContextObject,
	FWidgetControllerParam& OutWidgetControllerParam
)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			AProjectPlayerController* ProjectPC = Cast<AProjectPlayerController>(PC);
			AProjectAttackHUD* AttackHUD = Cast<AProjectAttackHUD>(ProjectPC->GetHUD());
			AProjectPlayerCharacter* ProjectCharacter = Cast<AProjectPlayerCharacter>(ProjectPC->GetCharacter());
			UProjectAttributeSet* ProjectAttributeSet = Cast<UProjectAttributeSet>(ProjectCharacter->GetAttributeSet());
			if (!ProjectPC || !AttackHUD || !ProjectCharacter) return false;

			UProjectAbilitySystemComponent* ASC = Cast<UProjectAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ProjectCharacter));

			OutWidgetControllerParam.PlayerCharacter = ProjectCharacter;
			OutWidgetControllerParam.PlayerController = ProjectPC;
			OutWidgetControllerParam.AbilitySystemComponent = ASC;
			OutWidgetControllerParam.AttributeSet=ProjectAttributeSet;
			return true;
		}
	}
	return false;
}

UMainAttackUIWidgetController* UProjectBlueprintFunctionLibrary::GetMainAttackUIWidgetController(UObject* WorldContextObject)
{
	FWidgetControllerParam Params;
	if (MakeWidgetControllerParam(WorldContextObject, Params))
	{
		GetUIManager(Params.PlayerController)->GetMainAttackUIWidgetController(Params);
	}
	return nullptr;
}

USettingUIWidgetController* UProjectBlueprintFunctionLibrary::GetSettingUIWidgetController(UObject* WorldContextObject)
{
	FWidgetControllerParam Params;
	if (MakeWidgetControllerParam(WorldContextObject, Params))
	{
		return GetUIManager(Params.PlayerController)->GetSettingUIWidgetController(Params);
	}
	return nullptr;
}

UAttributeWidgetController* UProjectBlueprintFunctionLibrary::GetAttributeWidgetController(UObject* WorldContextObject)
{
	FWidgetControllerParam Params;
	if (MakeWidgetControllerParam(WorldContextObject, Params))
	{
		return GetUIManager(Params.PlayerController)->GetAttributeWidgetController(Params);
	}
	return nullptr;
}

UProjectWidgetController* UProjectBlueprintFunctionLibrary::GetWidgetControllerByWidgetTag(
	UObject* WorldContextObject,
	const FGameplayTag& WidgetTag
)
{
	FWidgetControllerParam Params;
	if (MakeWidgetControllerParam(WorldContextObject, Params))
	{
		return GetUIManager(Params.PlayerController)->GetWidgetControllerByWidgetTag(Params,WidgetTag);
	}
	return nullptr;
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
