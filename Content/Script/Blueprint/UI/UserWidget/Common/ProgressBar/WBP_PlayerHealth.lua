--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_PlayerHealth_C
local M = UnLua.Class()


function M:Initialize(Initializer)
    ---@type BP_MainAttackUIWidgetController_C
    self.MainAttackUIWidgetController=nil
end

function M:AfterSetWidgetController()
    self.MainAttackUIWidgetController=self:GetWidgetController()
end
--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
