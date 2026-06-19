--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventoryInteract_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController=nil
end

function M:AfterSetWidgetController()
    self.InventoryWidgetController=self:GetWidgetController()
    self.WBP_InventoryInteractButton:SetWidgetController(self.InventoryWidgetController)
end

function M:AfterNativeOnActivated()
    self.TextBlock_Name:SetVisibility(UE.ESlateVisibility.Hidden)
    self.TextBlock_Description:SetVisibility(UE.ESlateVisibility.Hidden)
    self.WBP_InventoryInteractButton:SetVisibility(UE.ESlateVisibility.Hidden)
end


---@param SelectedItem UProjectInventoryItem
function M:OnItemSelected(SelectedItem)
    self.TextBlock_Description:SetText(SelectedItem.InventoryItemInfo.DescriptionText)
    self.TextBlock_Name:SetText(SelectedItem.InventoryItemInfo.ItemName)
    self.TextBlock_Name:SetVisibility(UE.ESlateVisibility.Visible)
    self.TextBlock_Description:SetVisibility(UE.ESlateVisibility.Visible)
    if SelectedItem.InventoryItemInfo.ItemTypeTag:MatchesTagExact(Tags.Inventory_Consumable) then
        self.WBP_InventoryInteractButton:SetVisibility(UE.ESlateVisibility.Visible)
    else
        self.WBP_InventoryInteractButton:SetVisibility(UE.ESlateVisibility.Hidden)
    end
end

function M:OnItemCancelSelected()
    self.TextBlock_Name:SetVisibility(UE.ESlateVisibility.Hidden)
    self.TextBlock_Description:SetVisibility(UE.ESlateVisibility.Hidden)
    self.WBP_InventoryInteractButton:SetVisibility(UE.ESlateVisibility.Hidden)
end



return M
