--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventoryUI_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController=nil
    ---@type FGameplayTag
    self.CurrentItemType=Tags.Inventory_Material
    ---@type TArray<UProjectInventoryItem>
    self.CurrentItemArray=UE.TArray(UE.UProjectInventoryItem)
    ---@type TArray<UProjectInventoryItem>
    self.EmptyItemArray=UE.TArray(UE.UProjectInventoryItem)
    self.LeastEntryNumber=48
    self.RowEntryNumber=8
    ---@type UProjectInventoryItem
    self.CurrentSelectedItem = nil
end

function M:AfterNativeOnActivated()
    self.TagToFunction =
    {
        [Tags.UI_Action_Inventory_RemoveInventoryUI] =
        function()
            self.InventoryWidgetController:CloseUIInputActionFunction(Tags.UI_Widget_Inventory)
        end
    }
    self:RegisterAction(self.TagToFunction,self.InputActionInfo)
    self.WBP_InventoryInteract:ActivateWidget()
    self.WBP_InventorySelection:ActivateWidget()
    
end


function M:AfterSetWidgetController()
    self.InventoryWidgetController=self:GetWidgetController()
    self.WBP_InventorySelection:SetWidgetController(self.InventoryWidgetController)
    self.WBP_InventoryInteract:SetWidgetController(self.InventoryWidgetController)
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)
    InventoryManager.OnInventorySingleChanged:Remove(self,self.RefreshSingleItem)
    InventoryManager.OnInventorySingleChanged:Add(self,self.RefreshSingleItem)
    InventoryManager.OnInventoryMultiChanged:Remove(self,self.RefreshMultiItem)
    InventoryManager.OnInventoryMultiChanged:Add(self,self.RefreshMultiItem)
    self.InventoryWidgetController.OnItemTypeChangedSignature:Remove(self,self.OnItemTypeChangedFunction)
    self.InventoryWidgetController.OnItemTypeChangedSignature:Add(self,self.OnItemTypeChangedFunction)
    self.CommonTileView.BP_OnItemSelectionChanged:Remove(self,self.OnItemSelectionChangedFunction)
    self.CommonTileView.BP_OnItemSelectionChanged:Add(self,self.OnItemSelectionChangedFunction)
    self.InventoryWidgetController.OnConsumableItemUsedSignature:Remove(self,self.OnConsumableItemUsedFunction)
    self.InventoryWidgetController.OnConsumableItemUsedSignature:Add(self,self.OnConsumableItemUsedFunction)
    self:RefreshMultiItem()
end


---@param ItemTag FGameplayTag
function M:RefreshSingleItem(ItemTag, ItemQuantity)
    if not ItemTag:MatchesTag(self.CurrentItemType) then return end

    local CurrentItemNumber = self.CurrentItemArray:Num()
    local bFound = false
    for i = 1, CurrentItemNumber do
        ---@type UProjectInventoryItem
        local TempItem = self.CurrentItemArray:Get(i)
        if TempItem and TempItem.ItemTag == ItemTag then
            TempItem.OwnedNumber = ItemQuantity
            TempItem.OnItemDataChanged:Broadcast(TempItem.OwnedNumber)
            bFound = true
            break
        end
    end
    if not bFound then
        self:RefreshMultiItem()
    end
end

function M:RefreshMultiItem()
    self.CurrentItemArray:Clear()
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)
    InventoryManager:GetFilterItems(self.CurrentItemType,self.CurrentItemArray)
    local CurrentItemNumber = self.CurrentItemArray:Num()
    local NeedEmptyItemNumber = self.LeastEntryNumber-CurrentItemNumber
    local EmptyItemNumber = self.EmptyItemArray:Num()
    local NeedEntryWidget = (CurrentItemNumber+self.RowEntryNumber-1)//self.RowEntryNumber
    self.LeastEntryNumber=math.max(self.LeastEntryNumber,self.RowEntryNumber*NeedEntryWidget)
    for Index = 1,self.CurrentItemArray:Num() do
        local TempItem = self.CurrentItemArray:Get(Index)
        TempItem:SetWidgetController(self.InventoryWidgetController)
    end
    
    if NeedEmptyItemNumber > EmptyItemNumber then
        local ConfigManager = UE.UProjectBlueprintFunctionLibrary.GetConfigManager(self)
        local InventoryInfo = ConfigManager:GetInventoryInfo()
        for _ = 1,NeedEmptyItemNumber - EmptyItemNumber do
            local TempItem = NewObject(UE.UProjectInventoryItem, self, nil)
            TempItem.ItemTag = InventoryInfo.EmptyItemTag
            TempItem.InventoryItemInfo = InventoryInfo.EmptyInventoryItemInfo
            TempItem:SetWidgetController(self.InventoryWidgetController)
            self.EmptyItemArray:Add(TempItem)
        end
    else
        self.EmptyItemArray:Resize(NeedEmptyItemNumber)
    end
    self.CurrentItemArray:Append(self.EmptyItemArray)
    self.CommonTileView:BP_SetListItems(self.CurrentItemArray)
end

---@param ItemTypeTag FGameplayTag 
function M:OnItemTypeChangedFunction(ItemTypeTag)
    self.CurrentItemType = ItemTypeTag
    self:RefreshMultiItem()
end

---@param ItemObject UObject
---@param bIsSelected boolean
function M:OnItemSelectionChangedFunction(ItemObject, bIsSelected)
    if bIsSelected then
        ---@type UProjectInventoryItem
        local TempItem = ItemObject:Cast(UE.UProjectInventoryItem)
        if TempItem then
            self.CurrentSelectedItem=TempItem
            self.WBP_InventoryInteract:OnItemSelected(TempItem)
        end
    else
        self.CurrentSelectedItem = nil
        self.WBP_InventoryInteract:OnItemCancelSelected()
    end
    
end

---@param TeamCharacterInfo FTeamCharacterInfo
function M:OnConsumableItemUsedFunction(TeamCharacterInfo)
    self.InventoryWidgetController:ApplyConsumableToTeamCharacter(self.CurrentSelectedItem,TeamCharacterInfo)
    if self.CurrentSelectedItem then
        self.InventoryWidgetController.OnConsumableItemNumberChangedSignature:Broadcast(self.CurrentSelectedItem.ItemTag, -1)
    end
end

return M
