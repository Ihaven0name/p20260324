--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InventorySelection_Character_C
local M = UnLua.Class(WidgetClassMap.CommonWidgetClass)

function M:Initialize(Initializer)
    ---@type BP_InventoryUIWidgetController_C
    self.InventoryWidgetController=nil
end



function M:AfterSetWidgetController()
    self.InventoryWidgetController = self:GetWidgetController()
    self.WBP_HealthBar:SetWidgetController(self.InventoryWidgetController)
    self.WBP_ManaBar:SetWidgetController(self.InventoryWidgetController)
end

---@param TeamCharacterInfo FTeamCharacterInfo 
function M:SetCharacterInfo(TeamCharacterInfo)
    local CharacterTexture = UE.UProjectBlueprintFunctionLibrary.GetTeamCharacterTexture(TeamCharacterInfo)
    self.WBP_Inventory_Character.Image_Show:SetBrushFromSoftTexture(CharacterTexture,false)
    self.WBP_HealthBar:SetBinding(TeamCharacterInfo.AttributeSet,TeamCharacterInfo.Character)
    self.WBP_ManaBar:SetBinding(TeamCharacterInfo.AttributeSet,TeamCharacterInfo.Character)
    self.InventoryWidgetController:BroadcastSpecCharacterInitialValue(TeamCharacterInfo.Character)
end

return M
