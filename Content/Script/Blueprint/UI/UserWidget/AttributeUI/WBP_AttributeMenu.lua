--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AttributeMenu_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end

function M:AfterNativeOnActivated()
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(0)
end



function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_TextValue_AttackPower:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_CriticalHitChance:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_CriticalHitDamage:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_DamageBonus:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_DamagePenetration:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_DamageResilience:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_Defense:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_Level:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_MaxHealth:SetWidgetController(self.AttributeWidgetController)
    self.WBP_TextValue_MaxMana:SetWidgetController(self.AttributeWidgetController)
    self.WBP_XPBar:SetWidgetController(self.AttributeWidgetController)
    self.WBP_Button_AddLevel:SetWidgetController(self.AttributeWidgetController)
    self.WBP_AddLevelPanel:SetWidgetController(self.AttributeWidgetController)
    
    self.AttributeWidgetController.OnChangeWidgetControllerParamSignature:Remove(self,self.Switcher_OnChangeParam)
    self.AttributeWidgetController.OnChangeWidgetControllerParamSignature:Add(self,self.Switcher_OnChangeParam)

    self.AttributeWidgetController.OnSwitcherIndexChangedSignature:Remove(self,self.Switcher_ChangeIndex)
    self.AttributeWidgetController.OnSwitcherIndexChangedSignature:Add(self,self.Switcher_ChangeIndex)

    self.WBP_Button_AddLevel:SetOnClickedCallback({self,function()
        self:Switcher_ChangeIndex(1)
    end})
end



function M:Switcher_OnChangeParam()
    self:Switcher_ChangeIndex(0)
end

function M:Switcher_ChangeIndex(Index)
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(Index)
end

return M
