--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_SettingPanel_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil

end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()
    self.WBP_SettingSelection:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingInterface:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingMappableKey:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingSound:SetWidgetController(self.SettingWidgetController)
    
    self.SettingWidgetController.OnSettingSwitcherIndexChangedSignature:Remove(self,self.ChangeSwitcherIndex)
    self.SettingWidgetController.OnSettingSwitcherIndexChangedSignature:Add(self,self.ChangeSwitcherIndex)
end

function M:AfterNativeOnActivated()
    self.WBP_SettingSelection:ActivateWidget()
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(0)
end

---@param InIndex number
function M:ChangeSwitcherIndex(InIndex)
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(InIndex)
end




return M
