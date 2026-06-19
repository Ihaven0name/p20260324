--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CharacterHead_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end


function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.Button_Switch.OnClicked:Remove(self,self.ButtonSwitchClicked)
    self.Button_Switch.OnClicked:Add(self,self.ButtonSwitchClicked)
end

function M:ButtonSwitchClicked()
    self.AttributeWidgetController.OnAttributeChangeSignature:Broadcast()
    
end
return M
