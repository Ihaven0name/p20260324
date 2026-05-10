--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AttributeUI_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end


function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_CharacterList:SetWidgetController(self.AttributeWidgetController)
    self.WBP_AttributeMenu:SetWidgetController(self.AttributeWidgetController)
    self:BindUIInputActionByTag(self.AttributeWidgetController)
    self.AttributeWidgetController:BroadcastInitialAttribute()
end

function M:AfterNativeOnActivated()
    if self.AttributeWidgetController then
        self:BindUIInputActionByTag(self.AttributeWidgetController)
    end
    self.WBP_AttributeMenu:ActivateWidget()
end

return M
