#pragma once

#include "CoreMinimal.h"

struct FAbilityTagConfig;

/** 能力标签 DataTable 的运行时查询与编辑器资产刷新工具。 */
class FAbilityTagManager
{
public:
	static const FAbilityTagConfig* FindAbilityTagConfigByRowName(FName RowName);

#if WITH_EDITOR
	static void RefreshSingleAbilityTagInCDO(FName ChangedRowName);
#endif
};
