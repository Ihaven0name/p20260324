-- 房主收到玩家的加入申请时显示的审批卡片。

---@type WBP_AcceptInvition_C
local M = UnLua.Class()

local function BindButton(Button, Owner, Callback, Label)
    if not Button then
        print("[WBP_AcceptInvition] 找不到" .. Label .. "按钮")
        return
    end
    if Button.SetOnClickedCallback then
        Button:SetOnClickedCallback({Owner, Callback})
        print("[WBP_AcceptInvition] 已绑定" .. Label .. "按钮")
    elseif Button.OnClicked then
        Button.OnClicked:Remove(Owner, Callback)
        Button.OnClicked:Add(Owner, Callback)
        print("[WBP_AcceptInvition] 已绑定" .. Label .. "按钮")
    else
        print("[WBP_AcceptInvition] " .. Label .. "控件不是可点击按钮")
    end
end

function M:Construct()
    self.CurrentRequestId = ""
    self.LoginWidget = nil

    self.AcceptButton = self.WBP_InviteButton_Accept
    self.RejectButton = self.WBP_InviteButton_Refuse
    BindButton(self.AcceptButton, self, self.OnClickedAccept, "同意")
    BindButton(self.RejectButton, self, self.OnClickedReject, "拒绝")
end

function M:SetLoginWidget(LoginWidget)
    self.LoginWidget = LoginWidget
end

function M:SetJoinRequestInfo(RequestId, RequesterUsername, MapName)
    self.CurrentRequestId = tostring(RequestId or "")
    local InviterText = self.TextBlock_Inviter
    local MapText = self.TextBlock_Map

    if InviterText then
        InviterText:SetText("玩家：" .. tostring(RequesterUsername or "未知玩家"))
    else
        print("[WBP_AcceptInvition] 找不到 TextBlock_Inviter")
    end

    if MapText then
        local DisplayMap = tostring(MapName or "")
        MapText:SetText(DisplayMap ~= "" and ("申请加入您的房间：" .. DisplayMap) or "申请加入您的房间")
    else
        print("[WBP_AcceptInvition] 找不到 TextBlock_Map")
    end
end

function M:OnClickedAccept()
    print("[WBP_AcceptInvition] 点击同意，RequestId=" .. self.CurrentRequestId)
    if self.LoginWidget and self.CurrentRequestId ~= "" then
        self.LoginWidget:OnJoinRequestPromptAction(self.CurrentRequestId, true)
    end
end

function M:OnClickedReject()
    print("[WBP_AcceptInvition] 点击拒绝，RequestId=" .. self.CurrentRequestId)
    if self.LoginWidget and self.CurrentRequestId ~= "" then
        self.LoginWidget:OnJoinRequestPromptAction(self.CurrentRequestId, false)
    end
end

return M
