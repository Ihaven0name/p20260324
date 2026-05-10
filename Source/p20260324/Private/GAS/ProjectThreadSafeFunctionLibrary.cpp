// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/ProjectThreadSafeFunctionLibrary.h"

float UProjectThreadSafeFunctionLibrary::GetAngleFromVectors(FVector ReferenceDir, FVector TargetDir)
{
	ReferenceDir.Z = 0;
	TargetDir.Z = 0;
	ReferenceDir.Normalize();
	TargetDir.Normalize();

	const float Dot = FVector::DotProduct(ReferenceDir, TargetDir);
	const FVector Cross = FVector::CrossProduct(ReferenceDir, TargetDir);
	// 计算角度（0-180）
	float Angle = FMath::Acos(FMath::Clamp(Dot, -1.0f, 1.0f));
	// 因为UE的坐标系是X向前，Y向右，Z向上，和正常的数学坐标系的X相反，所以当Cross.Z>0的时候，说明Target在参考的右边
	if (Cross.Z > 0)
	{
		Angle = -Angle;
	}
	return FMath::RadiansToDegrees(Angle);
}


