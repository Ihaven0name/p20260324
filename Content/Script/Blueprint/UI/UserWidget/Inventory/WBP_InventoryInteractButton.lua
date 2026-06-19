--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventoryInteractButton_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController = nil
end

function M:AfterSetWidgetController()
    self.InventoryWidgetController = self:GetWidgetController()
    self:SetOnClickedCallback({self,function()
        self:OnConsumeButtonBaseClickedFunction() 
    end})
end
function M:OnConsumeButtonBaseClickedFunction()
    ---@type UUIManager
    local UIManager = UE.UProjectBlueprintFunctionLibrary.GetUIManager(self.InventoryWidgetController.PlayerController)
    UIManager:PushWidget(Tags.UI_Widget_Inventory_Selection_Border)
end

return M
