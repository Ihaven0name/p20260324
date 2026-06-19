--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_XPBar_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
    ---@type 
    self.CurrentLevel=0
end



function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.CurrentLevel=self.AttributeWidgetController.WidgetControllerParam.AttributeSet:GetLevelValueForLua()
    self.AttributeWidgetController.OnAttributeChangeSignature:Remove(self,self.OnAttributeChangeFunction)
    self.AttributeWidgetController.OnAttributeChangeSignature:Add(self,self.OnAttributeChangeFunction)
    self.AttributeWidgetController.OnTemporaryLevelAndXPChangeSignature:Remove(self,self.OnTemporaryLevelAndXPChangeFunction)
    self.AttributeWidgetController.OnTemporaryLevelAndXPChangeSignature:Add(self,self.OnTemporaryLevelAndXPChangeFunction)
    
end



function M:OnTemporaryLevelAndXPChangeFunction(TempLevel,TempXP)

    ---@type UConfigManager
    local ConfigManager=UE.UProjectBlueprintFunctionLibrary.GetConfigManager(self.AttributeWidgetController.PlayerController);
    local NeedXP=ConfigManager:GetNeedXPAtLevel(math.tointeger(TempLevel))
    if not (NeedXP == 0) then
        local Percent=TempXP/NeedXP
        self.ProgressBar_XP:SetPercent(Percent)
        self.TextBlock_XP:SetText(string.format("%d/%d",TempXP,NeedXP))
    else
        self.ProgressBar_XP:SetPercent(1.0)
        self.TextBlock_XP:SetText(string.format("0/0"))
    end
    
end

---@param AttributeInfo FProjectAttributeInfo
function M:OnAttributeChangeFunction(AttributeInfo)
    if self.AttributeTag==AttributeInfo.AttributeTag then
        ---@type UConfigManager
        local ConfigManager=UE.UProjectBlueprintFunctionLibrary.GetConfigManager(self.AttributeWidgetController.PlayerController);
        self.CurrentLevel=self.AttributeWidgetController.WidgetControllerParam.AttributeSet:GetLevelValueForLua()
        local NeedXP=ConfigManager:GetNeedXPAtLevel(math.tointeger(self.CurrentLevel))
        if not (NeedXP == 0) then
            local Percent=AttributeInfo.AttributeValue/NeedXP
            self.ProgressBar_XP:SetPercent(Percent)
            self.TextBlock_XP:SetText(string.format("%d/%d",AttributeInfo.AttributeValue,NeedXP))
        else
            self.ProgressBar_XP:SetPercent(1.0)
            self.TextBlock_XP:SetText(string.format("0/0"))
        end
    end
end


return M

