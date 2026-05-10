#include "GAS/ProjectGameplayEffectType.h"

bool FProjectGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;

	if (Ar.IsSaving())
	{
		// --- 基类字段 (0~6) ---
		if (bReplicateInstigator && Instigator.IsValid())
			RepBits |= 1 << 0;
		if (bReplicateEffectCauser && EffectCauser.IsValid())
			RepBits |= 1 << 1;
		if (AbilityCDO.IsValid())
			RepBits |= 1 << 2;
		if (bReplicateSourceObject && SourceObject.IsValid())
			RepBits |= 1 << 3;
		if (Actors.Num() > 0)
			RepBits |= 1 << 4;
		if (HitResult.IsValid())
			RepBits |= 1 << 5;
		if (bHasWorldOrigin)
			RepBits |= 1 << 6;

		// --- 自定义字段 (7~12) ---
		if (bIsCriticalHit)
			RepBits |= 1 << 7;
		if (bIsApplyDebuff)
			RepBits |= 1 << 8;
		if (DebuffDamage > 0.f)
			RepBits |= 1 << 9;
		if (DebuffDuration > 0.f)
			RepBits |= 1 << 10;
		if (DebuffFrequency > 0.f)
			RepBits |= 1 << 11;
		if (!KnockBackImpulse.IsZero())
			RepBits |= 1 << 12;
	}

	Ar.SerializeBits(&RepBits, 13);

	// --- 基类序列化 ---
	if (RepBits & (1 << 0)) Ar << Instigator;
	if (RepBits & (1 << 1)) Ar << EffectCauser;
	if (RepBits & (1 << 2)) Ar << AbilityCDO;
	if (RepBits & (1 << 3)) Ar << SourceObject;
	if (RepBits & (1 << 4)) SafeNetSerializeTArray_Default<31>(Ar, Actors);
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading() && !HitResult.IsValid())
			HitResult = TSharedPtr<FHitResult>(new FHitResult());
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	// --- 自定义字段 ---
	if (RepBits & (1 << 7)) Ar << bIsCriticalHit;
	if (RepBits & (1 << 8)) Ar << bIsApplyDebuff;
	if (RepBits & (1 << 9)) Ar << DebuffDamage;
	if (RepBits & (1 << 10)) Ar << DebuffDuration;
	if (RepBits & (1 << 11)) Ar << DebuffFrequency;
	if (RepBits & (1 << 12)) Ar << KnockBackImpulse;

	if (Ar.IsLoading())
		AddInstigator(Instigator.Get(), EffectCauser.Get());

	bOutSuccess = true;
	return true;
}
