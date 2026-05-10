// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * UMMC_MaxHealth
 * 
 * 两层查表：
 *   1. ConfigManager.GetCurveTableByRowName(PlayerCategory) → 取出该 Category 的 UCurveTable*
 *   2. CurveTable->FindCurve(AttributeRowName) → 取出该属性对应的 FRealCurve*
 *   3. RealCurve.Eval(Level) → 返回最终值
 * 
 * 蓝图可创建子类，修改 PlayerCategory / AttributeRowName
 */
UCLASS(Blueprintable)
class P20260324_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();

	/** 第一层：DT_Based_Curve 的行名（即 EPlayerCategory 名，如 "Crouch"），蓝图可改 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName PlayerCategory;

	/** 第二层：CurveTable 内的曲线行名（即属性名，如 "MaxHealth"），蓝图可改 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMC")
	FName AttributeRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	/** Level 属性捕获定义（构造时注册一次，计算时复用） */
	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;
};
