--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_InvitePlayer_C
local M = UnLua.Class()

function M:Construct()
    self.CurrentInviteId = ""
    self.LoginWidget = nil
    if self.WBP_InviteButton_Accept then
        self.WBP_InviteButton_Accept:SetOnClickedCallback({self, self.OnClickedAccept})
        print("[WBP_InvitePlayer] 已绑定同意按钮")
    else
        print("[WBP_InvitePlayer] 找不到 WBP_InviteButton_Accept")
    end
    if self.WBP_InviteButton_Refuse then
        self.WBP_InviteButton_Refuse:SetOnClickedCallback({self, self.OnClickedReject})
        print("[WBP_InvitePlayer] 已绑定拒绝按钮")
    else
        print("[WBP_InvitePlayer] 找不到 WBP_InviteButton_Refuse")
    end
end

function M:SetInviteInfo(InviteId, InviterUsername, MapName)
    self.CurrentInviteId = tostring(InviteId or "")
    print(string.format("[WBP_InvitePlayer] 收到邀请卡片数据，InviteId=%s", self.CurrentInviteId))
    if self.TextBlock_Inviter then
        self.TextBlock_Inviter:SetText("玩家：" .. tostring(InviterUsername or "未知玩家"))
    end
    if self.TextBlock_Map then
        local DisplayMap = tostring(MapName or "")
        self.TextBlock_Map:SetText(DisplayMap ~= "" and ("正在邀请您进入" .. DisplayMap) or "正在邀请您加入房间")
    end
end

function M:SetLoginWidget(LoginWidget)
    self.LoginWidget = LoginWidget
end

function M:OnClickedAccept()
    print(string.format("[WBP_InvitePlayer] 点击同意，InviteId=%s", self.CurrentInviteId))
    if not self.LoginWidget then
        print("[WBP_InvitePlayer] LoginWidget 未设置")
        return
    end
    if self.CurrentInviteId == "" then
        print("[WBP_InvitePlayer] InviteId 为空，无法接受邀请")
        return
    end
    self.LoginWidget:OnInvitePromptAction(self.CurrentInviteId, true)
end

function M:OnClickedReject()
    print(string.format("[WBP_InvitePlayer] 点击拒绝，InviteId=%s", self.CurrentInviteId))
    if not self.LoginWidget then
        print("[WBP_InvitePlayer] LoginWidget 未设置")
        return
    end
    if self.CurrentInviteId == "" then
        print("[WBP_InvitePlayer] InviteId 为空，无法拒绝邀请")
        return
    end
    self.LoginWidget:OnInvitePromptAction(self.CurrentInviteId, false)
end

return M
