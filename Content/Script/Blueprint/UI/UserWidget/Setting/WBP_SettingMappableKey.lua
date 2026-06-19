--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_SettingMappableKey_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil
end


function M:Construct()
    local OneSlotWidgetBaseClass = UE.UClass.Load("/Game/Blueprint/UI/UserWidget/SettingUI/WBP_MappableBehaviorOneSlot.WBP_MappableBehaviorOneSlot_C")
    local TwoSlotWidgetBaseClass = UE.UClass.Load("/Game/Blueprint/UI/UserWidget/SettingUI/WBP_MappableBehaviorTwoSlot.WBP_MappableBehaviorTwoSlot_C")
    
    local Children = self.ScrollBox_Root:GetAllChildren()
    for i = 1, Children:Num() do
        local SlotComponent = Children:Get(i)
        if SlotComponent:Cast(OneSlotWidgetBaseClass) then
            self.OneSlotArray:Add(SlotComponent)
        elseif SlotComponent:Cast(TwoSlotWidgetBaseClass) then
            self.TwoSlotArray:Add(SlotComponent)
        end
    end
end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()

    for i = 1, self.OneSlotArray:Num() do
        local Widget = self.OneSlotArray:Get(i)
        Widget:SetWidgetController(self.SettingWidgetController)
    end
    for i = 1, self.TwoSlotArray:Num() do
        local Widget = self.TwoSlotArray:Get(i)
        Widget:SetWidgetController(self.SettingWidgetController)
    end
end

function M:AfterNativeOnActivated()
    for i = 1, self.OneSlotArray:Num() do
        ---@type WBP_MappableBehaviorOneSlot_C
        local Widget = self.OneSlotArray:Get(i)
        Widget:ActivateWidget()
    end
    for i = 1, self.TwoSlotArray:Num() do
        ---@type WBP_MappableBehaviorTwoSlot_C
        local Widget = self.TwoSlotArray:Get(i)
        Widget:ActivateWidget()
    end
end


return M
