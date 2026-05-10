--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AttributeMenu_C
local M = UnLua.Class()

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
    self.WBP_Button_AddXP:SetWidgetController(self.AttributeWidgetController)
    self.WBP_AddLevelPanel:SetWidgetController(self.AttributeWidgetController)
    
    self:AddBindingFunction()
    
end


function M:AddBindingFunction()
    self.AttributeWidgetController.OnChangeWidgetControllerParamSignature:Add(self,self.Switcher_OnChangeParam)
    self.WBP_Button_AddXP.Button.OnClicked:Add(self,self.OnClicked_Button_AddXP)
    self.WBP_AddLevelPanel.WBP_Button_Use.Button.OnClicked:Add(self,self.OnClicked_Button_UseMaterial)
    self.WBP_AddLevelPanel.WBP_Button_Close.Button.OnClicked:Add(self,self.OnClicked_Button_Close)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Small.WBP_XP_Small.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Medium.WBP_XP_Medium.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Large.WBP_XP_Large.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Small.WBP_Button_Reduce.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Medium.WBP_Button_Reduce.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_AddLevelPanel.WBP_XP_Interact_Large.WBP_Button_Reduce.Button.OnClicked:Add(self,self.OnClicked_ThreeXPMaterial)
    
    
end

function M:OnClicked_Button_AddXP()
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(1)
end

function M:OnClicked_Button_UseMaterial()
    self.AttributeWidgetController.OnAcceptXPElementsSignature:Broadcast(
            self.WBP_AddLevelPanel.WBP_XP_Interact_Small.SelectNumber,
            self.WBP_AddLevelPanel.WBP_XP_Interact_Medium.SelectNumber,
            self.WBP_AddLevelPanel.WBP_XP_Interact_Large.SelectNumber
    )
    self.WBP_AddLevelPanel.WBP_XP_Interact_Small.SelectNumber=0
    self.WBP_AddLevelPanel.WBP_XP_Interact_Medium.SelectNumber=0
    self.WBP_AddLevelPanel.WBP_XP_Interact_Large.SelectNumber=0
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(0)
end

function M:OnClicked_Button_Close()
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(0)
end

function M:Switcher_OnChangeParam()
    self.CommonActivatableWidgetSwitcher:SetActiveWidgetIndex(0)
end


function M:OnClicked_ThreeXPMaterial()
    self.AttributeWidgetController.OnSelectXPElementsSignature:Broadcast(
            self.WBP_AddLevelPanel.WBP_XP_Interact_Small.SelectNumber,
            self.WBP_AddLevelPanel.WBP_XP_Interact_Medium.SelectNumber,
            self.WBP_AddLevelPanel.WBP_XP_Interact_Large.SelectNumber
    )
end
return M
