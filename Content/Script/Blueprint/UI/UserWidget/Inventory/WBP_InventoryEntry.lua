--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventoryEntry_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController = nil
    ---@type UProjectInventoryItem
    self.CurrentItem = nil
end

function M:AfterSetWidgetController()
    self.InventoryWidgetController=self:GetWidgetController()
end


---@param ListItemObject UObject
function M:OnListItemObjectSet(ListItemObject)
    ---@type UProjectInventoryItem
    local ProjectInventoryItem = ListItemObject:Cast(UE.UProjectInventoryItem)
    if ProjectInventoryItem then
        local ConfigManager = UE.UProjectBlueprintFunctionLibrary.GetConfigManager(self)
        local InventoryInfo = ConfigManager:GetInventoryInfo()
        if ProjectInventoryItem.ItemTag:MatchesTagExact(InventoryInfo.EmptyItemTag) then
            self:SetIsFocusable(false)
        else
            self:SetIsFocusable(true)
        end
        if self.CurrentItem and UE.UKismetSystemLibrary.IsValid(self.CurrentItem) then
            self.CurrentItem.OnItemDataChanged:Remove(self,self.UpdateQuantityText)
        end
        self.CurrentItem=ProjectInventoryItem
        self:SetWidgetController(self.CurrentItem:GetWidgetController())
        self.CurrentItem.OnItemDataChanged:Add(self, self.UpdateQuantityText)
        self.Image_Show:SetBrushFromSoftTexture(ProjectInventoryItem.InventoryItemInfo.IconTexture)
        self:UpdateQuantityText(self.CurrentItem.OwnedNumber)
    end
end

function M:UpdateQuantityText(NewQuantity)
    self.TextBlock_OwnedNumber:SetText(tostring(NewQuantity))
end

return M
