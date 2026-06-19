--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_MappableBehaviorTwoSlot_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    ---@type BP_SettingUIWidgetController_C
    self.SettingWidgetController = nil
    self.bIsUpdateFirst=true
    self.bIsUpdateSecond=true
end

function M:AfterSetWidgetController()
    self.SettingWidgetController = self:GetWidgetController()
    
    self.InputKeySelector_First.OnKeySelected:Add(self,self.OnKeySelectedFunction_First)
    self.InputKeySelector_Second.OnKeySelected:Add(self,self.OnKeySelectedFunction_Second)
    self.SettingWidgetController.OnSettingMappableKeyClearedSignature:Add(self,self.OnMappableKeyCleared)
end

function M:AfterNativeOnActivated()
    ---@type UInputManager
    local InputManager = UE.UProjectBlueprintFunctionLibrary.GetInputManager(self)
    if not InputManager or not self.GroupTag then
        return
    end

    local PrimaryChord = UE.FInputChord()
    if InputManager:GetCurrentPrimaryKeyByGroupTag(self.GroupTag, PrimaryChord) then
        self.InputKeySelector_First:SetSelectedKey(PrimaryChord)
    else
        self.InputKeySelector_First:SetSelectedKey(UE.FInputChord())
    end

    local SecondaryChord = UE.FInputChord()
    if InputManager:GetCurrentSecondaryKeyByGroupTag(self.GroupTag, SecondaryChord) then
        self.InputKeySelector_Second:SetSelectedKey(SecondaryChord)
    else
        self.InputKeySelector_Second:SetSelectedKey(UE.FInputChord())
    end
    self.bIsUpdateFirst=false
    self.bIsUpdateSecond=false
end

---@param SelectedChord FInputChord
function M:OnKeySelectedFunction_First(SelectedChord)
    if self.bIsUpdateFirst then
        return
    end
    self.SettingWidgetController:RemapMappableKeyByGroupTagWithSlot(self.GroupTag,UE.EPlayerMappableKeySlot.First,SelectedChord)
end

---@param SelectedChord FInputChord
function M:OnKeySelectedFunction_Second(SelectedChord)
    if self.bIsUpdateSecond then
        return
    end
    self.SettingWidgetController:RemapMappableKeyByGroupTagWithSlot(self.GroupTag,UE.EPlayerMappableKeySlot.Second,SelectedChord)
end

function M:OnMappableKeyCleared(InGroupTag, InSlot)
    if not self.GroupTag or self.GroupTag ~= InGroupTag then
        return
    end

    if InSlot == UE.EPlayerMappableKeySlot.First then
        self.bIsUpdateFirst=true
        self.InputKeySelector_First:SetSelectedKey(UE.FInputChord())
        self.bIsUpdateFirst=false
        return
    end

    if InSlot == UE.EPlayerMappableKeySlot.Second then
        self.bIsUpdateSecond=true
        self.InputKeySelector_Second:SetSelectedKey(UE.FInputChord())
        self.bIsUpdateSecond=false
    end
end
return M
