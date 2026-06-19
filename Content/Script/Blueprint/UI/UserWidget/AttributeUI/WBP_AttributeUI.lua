--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--


---@type WBP_AttributeUI_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)


function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
    
end

function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_CharacterList:SetWidgetController(self.AttributeWidgetController)
    self.WBP_AttributeMenu:SetWidgetController(self.AttributeWidgetController)
    self.AttributeWidgetController:BroadcastInitialAttribute()
    
end

function M:AfterNativeOnActivated()
    self.WBP_AttributeMenu:ActivateWidget()
    self.Func=function()
        self.AttributeWidgetController:CloseUIInputActionFunction(Tags.UI_Widget_Attribute)
    end
    self.TagToFunction =
    {
        [Tags.UI_Action_Attribute_RemoveAttributeUI] = self.Func
        
    }
    self:RegisterAction(self.TagToFunction,self.InputActionInfo)
end


function M:BP_OnDeactivated()
    print("ssss")
end



return M
