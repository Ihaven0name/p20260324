#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnLuaInterface.h"
#include "UnLuaEx.h"
#include "LuaGameInstanceSubsystem.generated.h"


UCLASS()
class LUAFRAMEWORK_API ULuaGameInstanceSubsystem : public UGameInstanceSubsystem,public IUnLuaInterface
{
	GENERATED_BODY()
public:
	//TODO:记笔记，C++和Lua的联系
	virtual FString GetModuleName_Implementation()const override
	{
		return TEXT("Plugins.LuaFrameWork.Service.GameInstanceService");
	}
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	UFUNCTION(BlueprintImplementableEvent)
	void LuaInit();
	UFUNCTION(BlueprintImplementableEvent)
	void LuaDeinit();

	//把C++触发的时间通知给Lua，通过事件名传递
	template<typename ... T>
	void NotifyEventToLua(const char* EventName,T&&...Args)
	{
		FString EventNameStr(EventName);
		lua_State* L = UnLua::GetState();
		if (L)
		{
			const UnLua::FLuaRetValues LuaRetValues = UnLua::Call(L,"ReceiveNotifyFromC",TCHAR_TO_ANSI(*EventNameStr),Forward<T>(Args)...);
		}
	}
};
