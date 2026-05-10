#include "Info/CharacterInfo.h"

bool UCharacterInfo::FindSpecialPlayerInfoByCategory(EPlayerCategory PlayerCategory, FSpecialPlayerInfo& OutPlayerInfo) const
{
	if (PlayerCategory == EPlayerCategory::Empty) return false;
	if (const FSpecialPlayerInfo* Found = PlayerCategoryToPlayerInfo.Find(PlayerCategory))
	{
		OutPlayerInfo = *Found;
		return true;
	}
	return false;
}

bool UCharacterInfo::FindSpecialEnemyInfoByCategory(EEnemyCategory EnemyCategory, FSpecialEnemyInfo& OutEnemyInfo) const
{
	if (EnemyCategory == EEnemyCategory::Empty) return false;
	if (const FSpecialEnemyInfo* Found = EnemyCategoryToEnemyInfo.Find(EnemyCategory))
	{
		OutEnemyInfo = *Found;
		return true;
	}
	return false;
}