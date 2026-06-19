--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventorySwitchButton_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController=nil
end

function M:Construct()
    
end

function M:AfterSetWidgetController()
    self.InventoryWidgetController=self:GetWidgetController()
    self:SetOnClickedCallback({self,function()
        self:ChangeItemTypeTag()
    end })
end

function M:ChangeItemTypeTag()
    self.InventoryWidgetController.OnItemTypeChangedSignature:Broadcast(self.ItemTypeTag)
end


return M
