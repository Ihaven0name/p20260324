--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_TextValue_C
local M = UnLua.Class()



function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end



function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.AttributeWidgetController.OnAttributeChangeSignature:Add(self,self.OnAttributeChangeSignatureFunction)

end

---@param AttributeInfo FProjectAttributeInfo
function M:OnAttributeChangeSignatureFunction(AttributeInfo)
    ---@type FGameplayTag
    if self.AttributeTag==AttributeInfo.AttributeTag then
        ---@type string
        local DamageString=string.format("%.0f",AttributeInfo.AttributeValue)
        self.TextBlock_Value:SetText(DamageString)
    end
end
return M
