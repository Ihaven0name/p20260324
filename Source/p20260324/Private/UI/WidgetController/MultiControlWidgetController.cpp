#include "UI/WidgetController/MultiControlWidgetController.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/TeamManager.h"
#include "Player/ProjectPlayerController.h"

void UMultiControlWidgetController::InitWidgetController(AProjectPlayerController* InPlayerController)
{
	Super::InitWidgetController(InPlayerController);
	RefreshWidgetControllerParams();
}

void UMultiControlWidgetController::OnChangeWidgetControllerParamsFunction()
{
	RefreshWidgetControllerParams();
}

void UMultiControlWidgetController::BindCallback()
{
	OnChangeWidgetControllerParamsSignature.AddDynamic(this,&ThisClass::OnChangeWidgetControllerParamsFunction);
}

void UMultiControlWidgetController::RefreshWidgetControllerParams()
{
	WidgetControllerParams.Empty();
	if (UTeamManager* TeamManager = UProjectBlueprintFunctionLibrary::GetTeamManager(PlayerController))
	{
		TArray<FTeamCharacterInfo> TempTeamCharacterInfos;
		TeamManager->GetTeamInfoArray(TempTeamCharacterInfos);
		for (int32 i = 0; i < TempTeamCharacterInfos.Num(); i++)
		{
			FWidgetControllerParam TempWidgetControllerParam;
			TempWidgetControllerParam.AttributeSet = TempTeamCharacterInfos[i].AttributeSet;
			TempWidgetControllerParam.PlayerCharacter = TempTeamCharacterInfos[i].Character;
			TempWidgetControllerParam.AbilitySystemComponent = TempTeamCharacterInfos[i].AbilitySystemComponent;
			WidgetControllerParams.Add(TempWidgetControllerParam);
		}
	}
}