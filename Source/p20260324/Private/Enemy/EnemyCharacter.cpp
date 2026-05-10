#include "Enemy/EnemyCharacter.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/ProjectUserWidget.h"

//TODO:小地图系统,准备做一个大世界

AEnemyCharacter::AEnemyCharacter()
{
	// ASC - Enemy 使用 Minimal 复制模式
	AbilitySystemComponent = CreateDefaultSubobject<UProjectAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UProjectAttributeSet>("AttributeSet");	
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());

	// 血条 WidgetComponent
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetCapsuleComponent());
	
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enemy 只在服务器（权威端）初始化能力和属性
	if (HasAuthority())
	{
		UProjectBlueprintFunctionLibrary::InitializeEnemyStartupAbilities(AbilitySystemComponent.Get(), EnemyCategory);
		UProjectBlueprintFunctionLibrary::InitializeEnemyAttributeByEffect(AbilitySystemComponent.Get(), EnemyCategory, EnemyLevel);
	}

	BindHealthWidgetCallbacks();

	// 给血条 Widget 设置 Controller（让 Lua 侧能拿到 EnemyCharacter 引用）
	if (HealthWidgetComponent)
	{
		UProjectUserWidget* HealthWidget = Cast<UProjectUserWidget>(HealthWidgetComponent->GetWidget());
		if (HealthWidget)
		{
			HealthWidget->SetWidgetController(this);
		}
	}
}

void AEnemyCharacter::BindHealthWidgetCallbacks()
{
	if (!AbilitySystemComponent) return;

	UProjectAttributeSet* ProjectAS = Cast<UProjectAttributeSet>(AttributeSet);
	if (!ProjectAS) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectAS->GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		float MaxHealth = AttributeSet->GetMaxHealth();
		float Percent = MaxHealth > 0.f ? Data.NewValue / MaxHealth : 0.f;
		OnEnemyHealthPercentChanged.Broadcast(Percent);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectAS->GetMaxHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		float Health = AttributeSet->GetHealth();
		float Percent = Data.NewValue > 0.f ? Health / Data.NewValue : 0.f;
		OnEnemyHealthPercentChanged.Broadcast(Percent);
	});
}

