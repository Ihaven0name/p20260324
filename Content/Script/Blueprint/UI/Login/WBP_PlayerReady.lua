-- WBP_PlayerReady：房间玩家准备列表中的单行信息。

---@type WBP_PlayerReady_C
local M = UnLua.Class()

function M:Construct()
    self.PlayerNameText = self.Text_PlayerName
    self.ReadyStateText = self.Text_ReadyState
end

function M:SetPlayerInfo(Username, bReady)
    self.Username = tostring(Username or "")
    self.bReady = bReady == true

    if not self.PlayerNameText then
        self.PlayerNameText = self.Text_PlayerName
    end
    if not self.ReadyStateText then
        self.ReadyStateText = self.Text_ReadyState
    end

    if self.PlayerNameText then
        self.PlayerNameText:SetText(self.Username)
    else
        print("[WBP_PlayerReady] 找不到 Text_PlayerName")
    end

    if self.ReadyStateText then
        self.ReadyStateText:SetText(self.bReady and "已准备" or "未准备")
    else
        print("[WBP_PlayerReady] 找不到 Text_ReadyState")
    end
end

return M
