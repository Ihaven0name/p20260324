#include "Info/TeamInfo.h"

bool UTeamInfo::FindTeamStaticInfoByTag(const FGameplayTag& CharacterTag, FTeamStaticInfo& OutTeamStaticInfo) const
{
	if (!CharacterTag.IsValid()) return false;
	if (const FTeamStaticInfo* Found = CharacterTagToTeamStaticInfo.Find(CharacterTag))
	{
		OutTeamStaticInfo = *Found;
		return true;
	}
	return false;
}
