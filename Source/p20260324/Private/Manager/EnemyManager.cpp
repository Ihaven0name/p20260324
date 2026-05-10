// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/EnemyManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

void UEnemyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SpawnedEnemies.Empty();
}

void UEnemyManager::Deinitialize()
{
	ClearAllEnemies();
	Super::Deinitialize();
}

AActor* UEnemyManager::SpawnEnemy(UObject* WorldContextObject, TSubclassOf<AActor> EnemyClass, FVector Location, FRotator Rotation)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 生成敌人失败：WorldContextObject为空"));
		return nullptr;
	}
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 生成敌人失败：EnemyClass为空"));
		return nullptr;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 生成敌人失败：World为空"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(EnemyClass, Location, Rotation, SpawnParams);
	if (SpawnedActor)
	{
		SpawnedEnemies.Add(SpawnedActor);
	}

	return SpawnedActor;
}

void UEnemyManager::AddEnemy(AActor* Enemy)
{
	if (Enemy)
	{
		SpawnedEnemies.Add(Enemy);
	}
}

void UEnemyManager::RemoveEnemy(AActor* Enemy)
{
	if (Enemy)
	{
		SpawnedEnemies.Remove(Enemy);
	}
}

void UEnemyManager::ClearAllEnemies()
{
	SpawnedEnemies.Empty();
}

TArray<AActor*> UEnemyManager::GetSpawnedEnemies() const
{
	return SpawnedEnemies;
}

int32 UEnemyManager::GetEnemyCount() const
{
	return SpawnedEnemies.Num();
}

TArray<AActor*> UEnemyManager::GetVisibleEnemies(AActor* PlayerActor, float MaxDistance, float VisibleRange)
{
	TArray<AActor*> Result;
	
	if (!PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 获取视野内敌人失败：PlayerActor为空"));
		return Result;
	}
	
	UWorld* World = PlayerActor->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 获取视野内敌人失败：World为空"));
		return Result;
	}
	
	const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] 获取视野内敌人失败：Camera为空"));
		return Result;
	}

	const FVector CamLoc = Camera->GetCameraLocation();
	const FVector CamForward = Camera->GetActorForwardVector();

	for (AActor* Enemy : SpawnedEnemies)
	{
		if (!Enemy || !IsValid(Enemy))
		{
			continue;
		}
		FVector Dir = Enemy->GetActorLocation() - CamLoc;
		const float Dist = Dir.Size();
		if (Dist > MaxDistance)
		{
			continue;
		}
		Dir.Normalize();
		const float Dot = FVector::DotProduct(CamForward, Dir);
		// 将角度转换为cos阈值：cos(VisibleRange / 2)
		const float Threshold = FMath::Cos(FMath::DegreesToRadians(VisibleRange * 0.5f));
		if (Dot >= Threshold)
		{
			Result.Add(Enemy);
		}
	}

	return Result;
}

AActor* UEnemyManager::GetSideEnemy(AActor* PlayerActor, AActor* CurrentTarget, bool bIsRight, float VisibleRange)
{
	if (!PlayerActor || !CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManager] GetSideEnemy failed: PlayerActor或CurrentTarget为空"));
		return nullptr;
	}

	// 先获取视野内的敌人
	TArray<AActor*> VisibleEnemies = GetVisibleEnemies(PlayerActor, 1500.0f, VisibleRange);
	if (VisibleEnemies.Num() == 0)
	{
		return nullptr;
	}

	UWorld* World = PlayerActor->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(World, 0);
	if (!Camera)
	{
		return nullptr;
	}

	const FVector CamLoc = Camera->GetCameraLocation();
	const FVector CamRight = Camera->GetActorRightVector();

	// 当前目标的方向
	const FVector TargetDir = (CurrentTarget->GetActorLocation() - CamLoc).GetSafeNormal();

	// 在视野内的敌人中找同侧最近的
	AActor* NearestEnemy = nullptr;
	float MaxDot = -FLT_MAX; // 找点积最大的（夹角最小的）

	for (AActor* Enemy : VisibleEnemies)
	{
		if (!Enemy || !IsValid(Enemy) || Enemy == CurrentTarget)
		{
			continue;
		}

		const FVector EnemyDir = (Enemy->GetActorLocation() - CamLoc).GetSafeNormal();
		const float RightDot = FVector::DotProduct(CamRight, EnemyDir);

		// 判断是否在同一侧
		const bool bEnemyIsRight = RightDot > 0;
		if (bEnemyIsRight != bIsRight) continue;

		// 找与目标方向夹角最小的（点积最大）
		const float Dot = FVector::DotProduct(TargetDir, EnemyDir);
		if (Dot > MaxDot)
		{
			MaxDot = Dot;
			NearestEnemy = Enemy;
		}
	}

	return NearestEnemy;
}