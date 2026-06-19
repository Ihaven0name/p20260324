--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Bar_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController = nil
    ---@type UProjectAttributeSet
    self.AttributeSet = nil
    ---@type AProjectPlayerCharacter
    self.PlayerCharacter = nil
end

function M:AfterSetWidgetController()
    self.InventoryWidgetController = self:GetWidgetController()
end



---@param AttributeSet UProjectAttributeSet
---@param PlayerCharacter AProjectPlayerCharacter
function M:SetBinding(AttributeSet,PlayerCharacter)
    self.AttributeSet = AttributeSet
    self.PlayerCharacter = PlayerCharacter
    if self.AttributeTag == Tags.Attribute_Primary_Health then
        self.InventoryWidgetController.OnSpecCharacterHealthChangeSignature:Remove(self,self.ModifyPercentByHealth)
        self.InventoryWidgetController.OnSpecCharacterMaxHealthChangeSignature:Remove(self,self.ModifyPercentByMaxHealth)
        self.InventoryWidgetController.OnSpecCharacterHealthChangeSignature:Add(self,self.ModifyPercentByHealth)
        self.InventoryWidgetController.OnSpecCharacterMaxHealthChangeSignature:Add(self,self.ModifyPercentByMaxHealth)
    elseif self.AttributeTag == Tags.Attribute_Primary_Mana then
        self.InventoryWidgetController.OnSpecCharacterManaChangeSignature:Remove(self,self.ModifyPercentByMana)
        self.InventoryWidgetController.OnSpecCharacterMaxManaChangeSignature:Remove(self,self.ModifyPercentByMaxMana)
        self.InventoryWidgetController.OnSpecCharacterManaChangeSignature:Add(self,self.ModifyPercentByMana)
        self.InventoryWidgetController.OnSpecCharacterMaxManaChangeSignature:Add(self,self.ModifyPercentByMaxMana)
    end
end

---@param PlayerCharacter AProjectPlayerCharacter
---@param NewHealth number
function M:ModifyPercentByHealth(PlayerCharacter, NewHealth)
    if self.PlayerCharacter ~= PlayerCharacter then
        return
    end
    self:SetPercent(NewHealth,self.AttributeSet:GetMaxHealthValueForLua())
end
---@param PlayerCharacter AProjectPlayerCharacter
---@param NewMaxHealth number
function M:ModifyPercentByMaxHealth(PlayerCharacter, NewMaxHealth)
    if self.PlayerCharacter ~= PlayerCharacter then
        return
    end
    self:SetPercent(self.AttributeSet:GetHealthValueForLua(),NewMaxHealth)
end
---@param PlayerCharacter AProjectPlayerCharacter
---@param NewMana number
function M:ModifyPercentByMana(PlayerCharacter, NewMana)
    if self.PlayerCharacter ~= PlayerCharacter then
        return
    end
    self:SetPercent(NewMana,self.AttributeSet:GetMaxManaValueForLua())
end
---@param PlayerCharacter AProjectPlayerCharacter
---@param NewMaxMana number
function M:ModifyPercentByMaxMana(PlayerCharacter, NewMaxMana)
    if self.PlayerCharacter ~= PlayerCharacter then
        return
    end
    self:SetPercent(self.AttributeSet:GetManaValueForLua(),NewMaxMana)
end


---@param LeftValue number
---@param RightValue number
function M:SetPercent(LeftValue,RightValue)
    if math.abs(RightValue) > 0.001 then
        self.ProgressBar_Attribute:SetPercent(LeftValue,RightValue)
    end
end
return M
