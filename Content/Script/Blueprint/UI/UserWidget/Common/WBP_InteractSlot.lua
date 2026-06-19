--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InteractSlot_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
    self.SelectNumber=0
    self.OwnedNumber=0
end


function M:InitializeState()
    self.TextBlock_SelectNumber:SetVisibility(UE.ESlateVisibility.Hidden)
    self.WBP_Button_Reduce:SetVisibility(UE.ESlateVisibility.Hidden)
    ---@type  UInventoryManager
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)
    self.OwnedNumber=InventoryManager:GetItemQuantity(self.ItemTag)
    self.SelectNumber=0
    self.TextBlock_SelectNumber:SetText(string.format("%d",self.SelectNumber))
    self.TextBlock_OwnedNumber:SetText(string.format("%d",self.OwnedNumber))
end


function M:Construct()
    
end

function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    ---@type UInventoryManager
    local InventoryManager = UE.UProjectBlueprintFunctionLibrary.GetInventoryManager(self)
    InventoryManager.OnInventorySingleChanged:Remove(self,self.ModifyOwnedNumber)
    InventoryManager.OnInventorySingleChanged:Add(self,self.ModifyOwnedNumber)
    self:SetOnClickedCallback({self,function()
        self:OnClicked_Button_Add()
    end })
    self.WBP_Button_Reduce:SetOnClickedCallback({self,function()
        self:OnClicked_Button_Reduce()
    end })
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
        self.AttributeWidgetController.OnSelectedNumberChangedSignature:Broadcast()
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
        self.AttributeWidgetController.OnSelectedNumberChangedSignature:Broadcast()
    end

end

---@param ItemTag FGameplayTag
function M:ModifyOwnedNumber(ItemTag,ItemQuantity)
    if self.ItemTag == ItemTag then
        self.OwnedNumber=ItemQuantity
        self.TextBlock_OwnedNumber:SetText(string.format("%d",self.OwnedNumber))
    end
end

return M
