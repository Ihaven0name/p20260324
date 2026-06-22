--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_SettingUI_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil
    
end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()
    self.WBP_SettingButton_ContinueGame:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingButton_Setting:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingButton_QuitGame:SetWidgetController(self.SettingWidgetController)
    self.WBP_SettingButton_ContinueGame:SetOnClickedCallback({self,self.OnButtonClicked_ContinueGame})
    self.WBP_SettingButton_Setting:SetOnClickedCallback({self,self.OnButtonClicked_Setting})
    self.WBP_SettingButton_QuitGame:SetOnClickedCallback({self,self.OnButtonClicked_QuitGame})
end

function M:AfterNativeOnActivated()
    self.TagToFunction =
    {
        [Tags.UI_Action_Setting_RemoveSettingUI] =
        function()
            self.SettingWidgetController:CloseUIInputActionFunction(Tags.UI_Widget_Setting)
        end
    }
    self:RegisterAction(self.TagToFunction,self.InputActionInfo)
    UE.UGameplayStatics.SetGamePaused(self,true)
end

function M:OnButtonClicked_ContinueGame()
    UE.UGameplayStatics.SetGamePaused(self,false)
    self.SettingWidgetController:CloseUIInputActionFunction(Tags.UI_Widget_Setting)
end

function M:BP_OnHandleBackAction()
    UE.UGameplayStatics.SetGamePaused(self,false)
end

function M:OnButtonClicked_Setting()
    self.SettingWidgetController:OpenUIInputActionFunction(Tags.UI_Widget_Setting_SettingPanel)
end

function M:OnButtonClicked_QuitGame()
    --可能是返回上一个世界，目前是直接退出游戏
    UE.UKismetSystemLibrary.QuitGame(
            self,
            self.SettingWidgetController.PlayerController,
            UE.EQuitPreference.Quit,
            false
    )
end


return M
