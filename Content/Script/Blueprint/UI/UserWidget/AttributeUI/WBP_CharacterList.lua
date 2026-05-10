--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CharacterList_C
local M = UnLua.Class()

function M:Initialize()
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end

function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_CharacterHead_Player1:SetWidgetController(self.AttributeWidgetController)
    self.WBP_CharacterHead_Player2:SetWidgetController(self.AttributeWidgetController)
    self.WBP_CharacterHead_Player3:SetWidgetController(self.AttributeWidgetController)
end

return M
