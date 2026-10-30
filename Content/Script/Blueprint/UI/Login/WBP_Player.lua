-- WBP_Player：房间成员列表中的单行信息。

---@type WBP_Player_C
local M = UnLua.Class()

local function FindWidget(self, Names)
    for _, Name in ipairs(Names) do
        local BoundWidget = self[Name]
        if BoundWidget then
            return BoundWidget
        end
        local GetWidgetFromName = self.GetWidgetFromName
        if GetWidgetFromName then
            local bOk, FoundWidget = pcall(GetWidgetFromName, self, Name)
            if bOk and FoundWidget then
                return FoundWidget
            end
        end
    end
    return nil
end

function M:Construct()
    self.PlayerNameText = FindWidget(self, { "Text_PlayerName", "TextBlock_PlayerName", "PlayerName" })
    self.ReadyStateText = FindWidget(self, { "Text_ReadyState", "TextBlock_ReadyState", "ReadyState" })
end

---由 WBP_Login 在收到房间状态后调用，刷新当前成员行。
---@param Username string
---@param bReady boolean
function M:SetPlayerInfo(Username, bReady)
    self.Username = Username or ""
    self.bReady = bReady == true

    if not self.PlayerNameText then
        self.PlayerNameText = FindWidget(self, { "Text_PlayerName", "TextBlock_PlayerName", "PlayerName" })
    end
    if not self.ReadyStateText then
        self.ReadyStateText = FindWidget(self, { "Text_ReadyState", "TextBlock_ReadyState", "ReadyState" })
    end

    if self.PlayerNameText then
        self.PlayerNameText:SetText(self.Username)
    end
    if self.ReadyStateText then
        self.ReadyStateText:SetText(self.bReady and "已准备" or "未准备")
    end
end

return M
