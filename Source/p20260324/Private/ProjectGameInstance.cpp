#include "ProjectGameInstance.h"

#include "GameFramework/GameUserSettings.h"
#include "GenericPlatform/GenericApplication.h"
#include "GAS/ProjectGameplayTag.h"
#include "Manager/AccountManager.h"

UAccountManager* UProjectGameInstance::GetAccountManager() const
{
	return GetSubsystem<UAccountManager>();
}

FString UProjectGameInstance::GetSaveSlotName(const EProjectSaveSlotName InSaveSlotName)
{
	switch (InSaveSlotName)
	{
	case EProjectSaveSlotName::MappableKey:
		return TEXT("MappableKeySlot");
	default:
		return TEXT("UnknownSaveSlot");
	}
}

void UProjectGameInstance::Init()
{
	Super::Init();

	// Keep editor and dedicated-server windows untouched; this default is for the packaged game.
	if (FApp::CanEverRender() && !GIsEditor)
	{
		FDisplayMetrics DisplayMetrics;
		FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);

		if (DisplayMetrics.PrimaryDisplayWidth > 0 && DisplayMetrics.PrimaryDisplayHeight > 0)
		{
			const int32 WindowWidth = DisplayMetrics.PrimaryDisplayWidth / 2;
			const int32 WindowHeight = DisplayMetrics.PrimaryDisplayHeight / 2;

			if (UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings())
			{
				UserSettings->SetFullscreenMode(EWindowMode::Windowed);
				UserSettings->SetScreenResolution(FIntPoint(WindowWidth, WindowHeight));
				UserSettings->SetWindowPosition(
					(DisplayMetrics.PrimaryDisplayWidth - WindowWidth) / 2,
					(DisplayMetrics.PrimaryDisplayHeight - WindowHeight) / 2);
				UserSettings->ApplySettings(false);
			}
		}
	}

	const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();

	XPTagToXPValue.Empty();
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Large, 2000);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Medium, 500);
	XPTagToXPValue.Add(GameplayTags.Inventory_Material_Experience_Small, 200);
}
