--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--


---@type WBP_MainAttackUI_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

local function RegisterMainAttackActions(self)
    if not self.MainWidgetUIWidgetController then
        print(string.format("[UIDiag] MainAttackUI action registration skipped: WidgetController is nil widget=%s active=%s", self:GetName(), tostring(self.bMainAttackActionsActive)))
        return
    end

    self:UnbindActions()
    print(string.format("[UIDiag] MainAttackUI registering actions widget=%s controller=%s", self:GetName(), self.MainWidgetUIWidgetController:GetName()))

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
    self:RegisterAction(self.TagToFunction, self.InputActionInfo)
end

function M:Initialize(Initializer)
    ---@type BP_MainAttackUIWidgetController_C
    self.MainWidgetUIWidgetController=nil
    self.bMainAttackActionsActive = false
end

function M:AfterNativeOnActivated()
    self.bMainAttackActionsActive = true
    print(string.format("[UIDiag] MainAttackUI activated widget=%s controller=%s", self:GetName(), self.MainWidgetUIWidgetController and self.MainWidgetUIWidgetController:GetName() or "<nil>"))
    RegisterMainAttackActions(self)
end

function M:BeforeNativeOnDeactivated()
    self.bMainAttackActionsActive = false
end

function M:AfterSetWidgetController()
    self.MainWidgetUIWidgetController = self:GetWidgetController()
    print(string.format("[UIDiag] MainAttackUI controller set widget=%s controller=%s active=%s", self:GetName(), self.MainWidgetUIWidgetController and self.MainWidgetUIWidgetController:GetName() or "<nil>", tostring(self.bMainAttackActionsActive)))
    if self.WBP_PlayerHealth and self.MainWidgetUIWidgetController then
        self.WBP_PlayerHealth:SetWidgetController(self.MainWidgetUIWidgetController)
    end
    if self.bMainAttackActionsActive then
        RegisterMainAttackActions(self)
    end
end


function M:SwitchControllerCharacter(Index)
    ---@type UTeamManager
    local TeamManager = UE.UProjectBlueprintFunctionLibrary.GetTeamManager(self.MainWidgetUIWidgetController.PlayerController)
    TeamManager:SetControlledCharacterIndex(Index)
end

return M
