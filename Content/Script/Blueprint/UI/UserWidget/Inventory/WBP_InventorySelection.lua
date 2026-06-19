--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventorySelection_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController=nil
    
end


function M:AfterSetWidgetController()
    self.InventoryWidgetController=self:GetWidgetController()
    self.WBP_InventorySwitchButton_Material:SetWidgetController(self.InventoryWidgetController)
    self.WBP_InventorySwitchButton_Consumable:SetWidgetController(self.InventoryWidgetController)
    self.WBP_InventorySwitchButton_Special:SetWidgetController(self.InventoryWidgetController)
end

return M