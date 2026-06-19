--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_MappableBehaviorOneSlot_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil
    self.bIsUpdateFirst=true
end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()
    self.InputKeySelector_First.OnKeySelected:Add(self,self.OnKeySelectedFunction_First)
end

function M:AfterNativeOnActivated()
    ---@type UInputManager
    local InputManager = UE.UProjectBlueprintFunctionLibrary.GetInputManager(self)
    local PrimaryChord = UE.FInputChord()
    if InputManager:GetCurrentPrimaryKeyByGroupTag(self.GroupTag, PrimaryChord) then
        self.InputKeySelector_First:SetSelectedKey(PrimaryChord)
    else
        self.InputKeySelector_First:SetSelectedKey(UE.FInputChord())
    end
    self.bIsUpdateFirst=false
end

---@param SelectedChord FInputChord
function M:OnKeySelectedFunction_First(SelectedChord)
    if self.bIsUpdateFirst then
        self.bIsUpdateFirst=false
        return
    end
    self.SettingWidgetController:RemapMappableOneSlotKeyByGroupTag(self.GroupTag,SelectedChord)
end
return M
