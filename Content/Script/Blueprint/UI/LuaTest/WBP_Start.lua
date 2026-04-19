--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--


local LongInData = {
    [1] = {
        Name = "新游戏",
        Index = 1,
    },
    [2] = {
        Name = "继续游戏",
        Index = 2,
    },
    [3] = {
        Name = "设置",
        Index = 3,
    },
    [4] = {
        Name = "退出游戏",
        Index = 4,
    },
}
---@type WBP_Start_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    -- 创建数据
    for i = 1, #LongInData do
        local ObjectClass = UE.UClass.Load("/Game/Blueprint/UI/LuaTest/BP_ItemData.BP_ItemData_C")
        local NewObj = NewObject(ObjectClass)
        NewObj.Name = LongInData[i].Name
        NewObj.Func = self.HandleItemClick
        NewObj.BinObj = self
        NewObj.Frame = LongInData[i].Index
        self.ListView_86:AddItem(NewObj)
    end
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:HandleItemClick(idx)
    if idx <= #LongInData then
        print(LongInData[idx].Name)
    end
end

return M
