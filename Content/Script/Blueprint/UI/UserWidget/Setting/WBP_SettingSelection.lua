--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_SettingSelection_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil

end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()
    self.WBP_SettingSwitchButton_Sound:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingSwitchButton_MappableKey:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingSwitchButton_Interface:SetWidgetController(self.SettingWidgetController)

    self.WBP_SettingSwitchButton_Sound:SetOnClickedCallback({self,function()
        self.SettingWidgetController.OnSettingSwitcherIndexChangedSignature:Broadcast(self.WBP_SettingSwitchButton_Sound.Index)
    end})
    self.WBP_SettingSwitchButton_MappableKey:SetOnClickedCallback({self,function()
        self.SettingWidgetController.OnSettingSwitcherIndexChangedSignature:Broadcast(self.WBP_SettingSwitchButton_MappableKey.Index)
    end})
    self.WBP_SettingSwitchButton_Interface:SetOnClickedCallback({self,function()
        self.SettingWidgetController.OnSettingSwitcherIndexChangedSignature:Broadcast(self.WBP_SettingSwitchButton_Interface.Index)
    end})
end

function M:AfterNativeOnActivated()
    
end


return M
