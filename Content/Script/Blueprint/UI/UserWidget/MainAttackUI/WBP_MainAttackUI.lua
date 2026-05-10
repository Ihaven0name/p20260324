--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_MainAttackUI_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_MainAttackUIWidgetController_C
    self.MainWidgetUIWidgetController=nil
end

function M:AfterSetWidgetController()
    self.MainWidgetUIWidgetController=self:GetWidgetController()
    self:BindUIInputActionByTag(self.MainWidgetUIWidgetController)
end

function M:AfterNativeOnActivated()
    if self.MainWidgetUIWidgetController then
        self:BindUIInputActionByTag(self.MainWidgetUIWidgetController)
    end
end

return M
