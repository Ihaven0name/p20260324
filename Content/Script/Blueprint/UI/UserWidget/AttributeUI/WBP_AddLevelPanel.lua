--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_AddLevelPanel_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_AttributeUIWidgetController_C
    self.AttributeWidgetController=nil
end

function M:AfterNativeOnActivated()
    self.WBP_Interact_XP_Small:InitializeState()
    self.WBP_Interact_XP_Medium:InitializeState()
    self.WBP_Interact_XP_Large:InitializeState()
end


function M:AfterSetWidgetController()
    self.AttributeWidgetController=self:GetWidgetController()
    self.WBP_Interact_XP_Small:SetWidgetController(self.AttributeWidgetController)
    self.WBP_Interact_XP_Medium:SetWidgetController(self.AttributeWidgetController)
    self.WBP_Interact_XP_Large:SetWidgetController(self.AttributeWidgetController)
    self.AttributeWidgetController.OnSelectedNumberChangedSignature:Remove(self,self.OnClicked_ThreeXPMaterial)
    self.AttributeWidgetController.OnSelectedNumberChangedSignature:Add(self,self.OnClicked_ThreeXPMaterial)
    self.WBP_InteractButton_Close:SetOnClickedCallback({self,function()
        self:OnClicked_Button_Close()
    end})
    self.WBP_InteractButton_Use:SetOnClickedCallback({self,function()
        self:OnClicked_Button_UseMaterial()
    end})
end

function M:BeforeNativeOnDeactivated()
    ---@type FProjectAttributeInfo
    local LevelAttributeInfo=UE.FProjectAttributeInfo()
    LevelAttributeInfo.AttributeValue = self.AttributeWidgetController.WidgetControllerParam.AttributeSet:GetLevelValueForLua()
    LevelAttributeInfo.AttributeTag=Tags.Attribute_Primary_Level

    ---@type FProjectAttributeInfo
    local XPAttributeInfo=UE.FProjectAttributeInfo()
    XPAttributeInfo.AttributeValue = self.AttributeWidgetController.WidgetControllerParam.AttributeSet:GetXPValueForLua()
    XPAttributeInfo.AttributeTag=Tags.Attribute_Primary_XP
    self.AttributeWidgetController.OnAttributeChangeSignature:Broadcast(LevelAttributeInfo)
    self.AttributeWidgetController.OnAttributeChangeSignature:Broadcast(XPAttributeInfo)
end

function M:OnClicked_ThreeXPMaterial()
    self.AttributeWidgetController.OnSelectXPElementsSignature:Broadcast(
            self.WBP_Interact_XP_Small.SelectNumber,
            self.WBP_Interact_XP_Medium.SelectNumber,
            self.WBP_Interact_XP_Large.SelectNumber
    )
end

function M:OnClicked_Button_UseMaterial()
    self.AttributeWidgetController.OnAcceptXPElementsSignature:Broadcast(
            self.WBP_Interact_XP_Small.SelectNumber,
            self.WBP_Interact_XP_Medium.SelectNumber,
            self.WBP_Interact_XP_Large.SelectNumber
    )
    self.WBP_Interact_XP_Small.SelectNumber=0
    self.WBP_Interact_XP_Medium.SelectNumber=0
    self.WBP_Interact_XP_Large.SelectNumber=0
    self.AttributeWidgetController:RequestChangeSwitcherIndex(0)
end

function M:OnClicked_Button_Close()
    self.AttributeWidgetController:RequestChangeSwitcherIndex(0)
end


return M
