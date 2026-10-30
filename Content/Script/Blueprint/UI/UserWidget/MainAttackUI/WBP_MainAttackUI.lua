--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--


---@type WBP_MainAttackUI_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_MainAttackUIWidgetController_C
    self.MainWidgetUIWidgetController=nil
end

function M:AfterNativeOnActivated()
    if self.MainWidgetUIWidgetController then
        self.TagToFunction =
        {
            [Tags.UI_Action_MainAttack_AddAttributeUI] =
            function()
                self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Attribute)
            end,
            [Tags.UI_Action_MainAttack_AddInventoryUI] =
            function()
                self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Inventory)
            end,
            [Tags.UI_Action_MainAttack_AddSettingUI] =
            function()
                self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Setting)
            end,
            [Tags.UI_Action_MainAttack_Switch1] =
            function()
                self:SwitchControllerCharacter(0)
            end,
            [Tags.UI_Action_MainAttack_Switch2] =
            function()
                self:SwitchControllerCharacter(1)
            end,
            [Tags.UI_Action_MainAttack_Switch3] =
            function()
                self:SwitchControllerCharacter(2)
            end,
        }
        self:RegisterAction(self.TagToFunction,self.InputActionInfo)
    end
end


function M:AfterSetWidgetController()
    self.MainWidgetUIWidgetController = self:GetWidgetController()
    if self.WBP_PlayerHealth and self.MainWidgetUIWidgetController then
        self.WBP_PlayerHealth:SetWidgetController(self.MainWidgetUIWidgetController)
    end

    self.TagToFunction =
    {
        [Tags.UI_Action_MainAttack_AddAttributeUI] =
        function()
            self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Attribute)
        end,
        [Tags.UI_Action_MainAttack_AddInventoryUI] =
        function()
            self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Inventory)
        end,
        [Tags.UI_Action_MainAttack_AddSettingUI] =
        function()
            self.MainWidgetUIWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Setting)
        end,
        [Tags.UI_Action_MainAttack_Switch1] =
        function()
            self:SwitchControllerCharacter(0)
        end,
        [Tags.UI_Action_MainAttack_Switch2] =
        function()
            self:SwitchControllerCharacter(1)
        end,
        [Tags.UI_Action_MainAttack_Switch3] =
        function()
            self:SwitchControllerCharacter(2)
        end,
    }
    self:RegisterAction(self.TagToFunction,self.InputActionInfo)
end


function M:SwitchControllerCharacter(Index)
    ---@type UTeamManager
    local TeamManager = UE.UProjectBlueprintFunctionLibrary.GetTeamManager(self.MainWidgetUIWidgetController.PlayerController)
    TeamManager:SetControlledCharacterIndex(Index)
end

return M
