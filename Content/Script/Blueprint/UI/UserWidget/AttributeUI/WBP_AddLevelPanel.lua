--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AddLevelPanel_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end

function M:AfterNativeOnActivated()
    
    self.WBP_XP_Interact_Small:ActivateWidget()
    self.WBP_XP_Interact_Medium:ActivateWidget()
    self.WBP_XP_Interact_Large:ActivateWidget()
end


function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_XP_Interact_Small:SetWidgetController(self.AttributeWidgetController)
    self.WBP_XP_Interact_Medium:SetWidgetController(self.AttributeWidgetController)
    self.WBP_XP_Interact_Large:SetWidgetController(self.AttributeWidgetController)
end

function M:BeforeNativeOnDeactivated()
    ---@type FProjectAttributeInfo
    local LevelAttributeInfo=UE.FProjectAttributeInfo()
    LevelAttributeInfo.AttributeValue = self.AttributeWidgetController.AttributeSet:GetLevelValueForLua()
    LevelAttributeInfo.AttributeTag=UE.FGameplayTag.RequestGameplayTag("Attribute.Primary.Level",true)

    ---@type FProjectAttributeInfo
    local XPAttributeInfo=UE.FProjectAttributeInfo()
    XPAttributeInfo.AttributeValue = self.AttributeWidgetController.AttributeSet:GetXPValueForLua()
    XPAttributeInfo.AttributeTag=UE.FGameplayTag.RequestGameplayTag("Attribute.Primary.XP",true)
    self.AttributeWidgetController.OnAttributeChangeSignature:Broadcast(LevelAttributeInfo)
    self.AttributeWidgetController.OnAttributeChangeSignature:Broadcast(XPAttributeInfo)

    self.WBP_XP_Interact_Small:DeactivateWidget()
    self.WBP_XP_Interact_Medium:DeactivateWidget()
    self.WBP_XP_Interact_Large:DeactivateWidget()
end

return M
