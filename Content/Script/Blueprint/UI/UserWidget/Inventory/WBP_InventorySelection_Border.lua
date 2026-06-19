--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventorySelection_Border_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController = nil
    self.SelectedIndex = -1
    ---@type TArray<FTeamCharacterInfo>
    self.TeamArray = UE.TArray(UE.FTeamCharacterInfo);
end

function M:AfterSetWidgetController()
    
    self.InventoryWidgetController = self:GetWidgetController()
    self.WBP_Button_Use:SetOnClickedCallback({self,self.OnClicked_Button_Use})
    self.WBP_Button_Close:SetOnClickedCallback({self,self.OnClicked_Button_Close})

    ---@type UInventoryInfo
    local InventoryInfo = UE.UProjectBlueprintFunctionLibrary.GetConfigManager(self):GetInventoryInfo()
    ---@type UTeamManager
    local TeamManager = UE.UProjectBlueprintFunctionLibrary.GetTeamManager(self.InventoryWidgetController.PlayerController)

    self.HorizontalBox_Characters:ClearChildren()
    self.TeamArray = TeamManager:GetTeamInfoArray()
    for i = 1, self.TeamArray:Num() do
        ---@type WBP_InventorySelection_Character_C
        local Widget = UE.UWidgetBlueprintLibrary.Create(self,InventoryInfo.InventorySelection_Character,self.InventoryWidgetController.PlayerController)
        ---@type UHorizontalBoxSlot
        local Slot = self.HorizontalBox_Characters:AddChildToHorizontalBox(Widget)
        ---@type FTeamCharacterInfo
        local TeamCharacterInfo = self.TeamArray:Get(i)
        if i ~= 1 then
            Slot:SetPadding(UE.FMargin(20, 0, 0, 0))
        end
        Slot:SetHorizontalAlignment(UE.EHorizontalAlignment.HAlign_Center)
        Slot:SetVerticalAlignment(UE.EVerticalAlignment.VAlign_Center)
        Widget:SetWidgetController(self.InventoryWidgetController)
        Widget.TeamIndex = i
        Widget:SetCharacterInfo(TeamCharacterInfo)
        Widget.WBP_Inventory_Character:SetOnClickedCallback({self,function()
            self.SelectedIndex = Widget.TeamIndex
        end})
    end
end



function M:BeforeNativeOnDeactivated()
    self.HorizontalBox_Characters:ClearChildren()
end


function M:OnClicked_Button_Use()
    if self.SelectedIndex ~= -1 then
        ---@type FTeamCharacterInfo
        local TeamCharacterInfo = self.TeamArray:Get(self.SelectedIndex)
        self.InventoryWidgetController.OnConsumableItemUsedSignature:Broadcast(TeamCharacterInfo);
        self:OnClicked_Button_Close()
    end
    
end

function M:OnClicked_Button_Close()
    ---@type UUIManager
    local UIManager = UE.UProjectBlueprintFunctionLibrary.GetUIManager(self.InventoryWidgetController.PlayerController)
    UIManager:PopWidget(Tags.UI_Widget_Inventory_Selection_Border)
end



return M
