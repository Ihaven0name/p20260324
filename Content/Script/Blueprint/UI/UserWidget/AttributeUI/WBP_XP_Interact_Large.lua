--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_XP_Interact_Large_C
local M = UnLua.Class()


function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
    self.SelectNumber=0
    self.OwnedNumber=0

end

function M:AfterNativeOnActivated()

    self.TextBlock_SelectNumber:SetVisibility(UE.ESlateVisibility.Hidden)
    self.WBP_Button_Reduce:SetVisibility(UE.ESlateVisibility.Hidden)
    self.SelectNumber=0
    ---@type  UInventoryManager
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)
    self.OwnedNumber=InventoryManager:GetItemQuantity(self.WBP_XP_Large.ItemTag)
    self.TextBlock_OwnedNumber:SetText(string.format("%d",self.OwnedNumber))
end

function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_Button_Reduce.Button.OnClicked:Add(self,self.OnClicked_Button_Reduce)
    ---@type UInventoryManager
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)

    InventoryManager.OnInventoryChanged:Add(self,self.ModifyOwnedNumber)
    self.WBP_XP_Large.Button.OnClicked:Add(self,self.OnClicked_Button_Add)
end

function M:OnClicked_Button_Reduce()
    if self.SelectNumber > 0 then
        self.SelectNumber=self.SelectNumber-1
        if self.SelectNumber == 0 then
            self.TextBlock_SelectNumber:SetVisibility(UE.ESlateVisibility.Hidden)
            self.WBP_Button_Reduce:SetVisibility(UE.ESlateVisibility.Hidden)
        end
        ---@type string
        local Text= string.format("%d",self.SelectNumber)
        self.TextBlock_SelectNumber:SetText(Text)
    end
end

function M:OnClicked_Button_Add()
    if self.SelectNumber+1<=self.OwnedNumber then
        if self.SelectNumber == 0 then
            self.TextBlock_SelectNumber:SetVisibility(UE.ESlateVisibility.SelfHitTestInvisible)
            self.WBP_Button_Reduce:SetVisibility(UE.ESlateVisibility.SelfHitTestInvisible)
        end
        self.SelectNumber=self.SelectNumber+1
        ---@type string
        local Text= string.format("%d",self.SelectNumber)
        self.TextBlock_SelectNumber:SetText(Text)
    end
end


---@param ItemTag FGameplayTag
function M:ModifyOwnedNumber(ItemTag,ItemQuantity)
    if self.WBP_XP_Large.ItemTag == ItemTag then
        self.OwnedNumber=ItemQuantity
        self.TextBlock_OwnedNumber:SetText(string.format("%d",self.OwnedNumber))
    end
end
return M
