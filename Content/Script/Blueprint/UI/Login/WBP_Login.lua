-- WBP_Login：登录后的大厅、创建/加入房间、邀请和房间成员状态。
-- 房间人数固定为 4；不要在此界面暴露人数上限设置。

---@type WBP_Login_C
local M = UnLua.Class()

local MAX_ROOM_PLAYERS = 4
local DEFAULT_MAP_NAME = "Test1"
local ROOM_PLAYER_WIDGET_CLASS = "/Game/Blueprint/UI/Login/WBP_PlayerReady.WBP_PlayerReady_C"
local INVITE_PROMPT_WIDGET_CLASS = "/Game/Blueprint/UI/Login/WBP_InvitePlayer.WBP_InvitePlayer_C"
local JOIN_REQUEST_WIDGET_CLASS = "/Game/Blueprint/UI/Login/WBP_AcceptInvition.WBP_AcceptInvition_C"
local INVITE_POLL_INTERVAL = 3.0
local JOIN_REQUEST_POLL_INTERVAL = 3.0
local PAGE_INDEX_CREATE = 0
local PAGE_INDEX_JOIN = 1
local PAGE_INDEX_ROOM = 2
local ROOM_ID_PREFIX = "房间号："

local function GetTextValue(Widget)
    if not Widget then
        return ""
    end

    local bGetSucceeded, Text = pcall(function()
        return Widget:GetText()
    end)
    if not bGetSucceeded or Text == nil then
        return ""
    end

    return tostring(Text)
end

local function SetTextValue(Widget, Value)
    if Widget then
        Widget:SetText(tostring(Value or ""))
    end
end

local function SetStatus(Message)
    print("[WBP_Login] " .. tostring(Message or ""))
end

local function GetAccountManager(self)
    if self.AccountManager then
        return self.AccountManager
    end

    local SubsystemLibrary = UE.USubsystemBlueprintLibrary
    if not SubsystemLibrary then
        return nil
    end

    self.AccountManager = SubsystemLibrary.GetGameInstanceSubsystem(self, UE.UAccountManager)
    if not self.AccountManager then
        print("[WBP_Login] 无法获取 UAccountManager GameInstanceSubsystem")
    end
    return self.AccountManager
end

local function BindButton(Owner, Button, Key, Function)
    if Button and Button.SetOnClickedCallback then
        Button:SetOnClickedCallback({Owner, Function})
        print(string.format("[WBP_Login] 已绑定项目按钮：%s", Key))
    elseif Button and Button.OnClicked then
        Button.OnClicked:Remove(Owner, Function)
        Button.OnClicked:Add(Owner, Function)
        print(string.format("[WBP_Login] 已绑定按钮：%s", Key))
    else
        print(string.format("[WBP_Login] 找不到按钮控件：%s，请检查蓝图控件名", Key))
    end
end

local function GetRoomId(self)
    return self.ActiveRoomId or ""
end

local function GetJoinTargetUsername(self)
    local Username = GetTextValue(self.UsernameInput_Join)
    Username = string.gsub(Username, "^%s*玩家名：%s*", "")
    Username = string.gsub(Username, "^%s*房间号：%s*", "")
    Username = string.gsub(Username, "^%s*(.-)%s*$", "%1")
    return Username
end

local function SetRoomIdInputValue(self, RoomId)
    SetTextValue(self.EditableTextBox_RoomID, ROOM_ID_PREFIX .. tostring(RoomId or ""))
end

local function SetActivePage(self, Index)
    if not self.WS_Login then
        print(string.format("[WBP_Login] 无法切换到页面 %d：WS_Login 为空", Index))
        return false
    end
    self.WS_Login:SetActiveWidgetIndex(Index)
    print(string.format("[WBP_Login] WidgetSwitcher 已切换到页面 %d", Index))
    return true
end

function M:Construct()
    print("[WBP_Login] Lua Construct 已执行")
    self.AccountManager = GetAccountManager(self)
    self.ActiveRoomId = self.ActiveRoomId or ""
    self.bLocalReady = self.bLocalReady or false
    self.Invites = {}
    self.PlayerRowWidgets = {}
    self.InvitePromptWidget = nil
    self.HandledInviteIds = {}
    self.PendingInviteActionId = ""
    self.InvitePollingTimerHandle = nil
    self.JoinRequestPollingTimerHandle = nil
    self.JoinRequestStatusTimerHandle = nil
    self.JoinRequestPromptWidget = nil
    self.PendingJoinRequestId = ""
    self.HandledJoinRequestIds = {}
    self.PendingJoinRequestActionId = ""
    self.bLocalRoomOwner = false

    local JoinTargetInput = self.UsernameInput_Join
    if JoinTargetInput then
        pcall(function()
            JoinTargetInput:SetHintText("输入房主玩家名称")
        end)
    end

    if self.AccountManager then
        -- 登录按钮仍由蓝图触发；登录结果成功后由此处切换到大厅页。
        self.AccountManager.OnLoginCompleted:Remove(self, self.OnLoginCompleted)
        self.AccountManager.OnLoginCompleted:Add(self, self.OnLoginCompleted)
        self.AccountManager.OnRoomRequestCompleted:Remove(self, self.OnRoomRequestCompleted)
        self.AccountManager.OnRoomRequestCompleted:Add(self, self.OnRoomRequestCompleted)
        self.AccountManager.OnRoomInvitesReceived:Remove(self, self.OnRoomInvitesReceived)
        self.AccountManager.OnRoomInvitesReceived:Add(self, self.OnRoomInvitesReceived)
        self.AccountManager.OnRoomJoinRequestsReceived:Remove(self, self.OnRoomJoinRequestsReceived)
        self.AccountManager.OnRoomJoinRequestsReceived:Add(self, self.OnRoomJoinRequestsReceived)
    else
        SetStatus("找不到 AccountManager 子系统。")
    end

    BindButton(self, self.Btn_CreateRoom, "CreateRoom", self.OnClickCreateRoom)
    BindButton(self, self.Btn_JoinRoom, "JoinRoom", self.OnClickJoinRoom)
    BindButton(self, self.Btn_Invite, "InvitePlayer", self.OnClickInvitePlayer)
    BindButton(self, self.Btn_ToggleReady, "ToggleReady", self.OnClickToggleReady)
    BindButton(self, self.Btn_StartRoom, "StartRoom", self.OnClickStartRoom)
end

function M:Destruct()
    if self.AccountManager then
        self.AccountManager.OnLoginCompleted:Remove(self, self.OnLoginCompleted)
        self.AccountManager.OnRoomRequestCompleted:Remove(self, self.OnRoomRequestCompleted)
        self.AccountManager.OnRoomInvitesReceived:Remove(self, self.OnRoomInvitesReceived)
        self.AccountManager.OnRoomJoinRequestsReceived:Remove(self, self.OnRoomJoinRequestsReceived)
        self.AccountManager:StopRoomPolling()
    end
    if self.InvitePollingTimerHandle then
        UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self, self.InvitePollingTimerHandle)
        self.InvitePollingTimerHandle = nil
    end
    if self.InvitePromptWidget then
        self.InvitePromptWidget:RemoveFromParent()
        self.InvitePromptWidget = nil
    end
    if self.JoinRequestPollingTimerHandle then
        UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self, self.JoinRequestPollingTimerHandle)
        self.JoinRequestPollingTimerHandle = nil
    end
    if self.JoinRequestStatusTimerHandle then
        UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self, self.JoinRequestStatusTimerHandle)
        self.JoinRequestStatusTimerHandle = nil
    end
    if self.JoinRequestPromptWidget then
        self.JoinRequestPromptWidget:RemoveFromParent()
        self.JoinRequestPromptWidget = nil
    end
end

function M:CreateJoinRequestPrompt()
    if self.JoinRequestPromptWidget then
        return self.JoinRequestPromptWidget
    end
    local RequestClass = UE.UClass.Load(JOIN_REQUEST_WIDGET_CLASS)
    if not RequestClass then
        print("[WBP_Login] 找不到 WBP_AcceptInvition 蓝图类")
        return nil
    end
    local Prompt = UE.UWidgetBlueprintLibrary.Create(self, RequestClass, self:GetOwningPlayer())
    if not Prompt then
        print("[WBP_Login] 创建 WBP_AcceptInvition 失败")
        return nil
    end

    Prompt:AddToViewport(21)
    -- AddToViewport 会触发弹窗的 Construct；必须在 Construct 之后设置父界面引用，
    -- 否则弹窗 Construct 中初始化 LoginWidget 会把它清空。
    Prompt:SetLoginWidget(self)
    Prompt:SetAlignmentInViewport(UE.FVector2D(1.0, 1.0))
    local ViewportSize = UE.UWidgetLayoutLibrary.GetViewportSize(self)
    if ViewportSize then
        Prompt:SetPositionInViewport(
            UE.FVector2D(ViewportSize.X - 32.0, ViewportSize.Y - 220.0), true)
    end
    Prompt:SetRenderOpacity(0.0)
    Prompt:SetVisibility(UE.ESlateVisibility.Collapsed)
    self.JoinRequestPromptWidget = Prompt
    return Prompt
end

function M:SetJoinRequestPromptVisible(bVisible)
    local Prompt = self.JoinRequestPromptWidget
    if Prompt then
        Prompt:SetRenderOpacity(bVisible and 1.0 or 0.0)
        Prompt:SetVisibility(bVisible and UE.ESlateVisibility.Visible or UE.ESlateVisibility.Collapsed)
    end
end

function M:DestroyJoinRequestPrompt()
    if self.JoinRequestPromptWidget then
        self.JoinRequestPromptWidget:RemoveFromParent()
        self.JoinRequestPromptWidget = nil
    end
end

function M:StartJoinRequestPolling()
    local Manager = GetAccountManager(self)
    if not Manager or not self.bLocalRoomOwner or self.ActiveRoomId == "" then
        return
    end
    self:CreateJoinRequestPrompt()
    Manager:GetRoomJoinRequests(self.ActiveRoomId)
    if not self.JoinRequestPollingTimerHandle then
        self.JoinRequestPollingTimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate(
            {self, self.PollJoinRequests}, JOIN_REQUEST_POLL_INTERVAL, true)
    end
end

function M:PollJoinRequests()
    local Manager = GetAccountManager(self)
    if Manager and self.bLocalRoomOwner and self.ActiveRoomId ~= "" then
        Manager:GetRoomJoinRequests(self.ActiveRoomId)
    end
end

function M:PollJoinRequestStatus()
    local Manager = GetAccountManager(self)
    if Manager and self.PendingJoinRequestId ~= "" then
        Manager:GetJoinRequestStatus(self.PendingJoinRequestId)
    end
end

function M:StartJoinRequestStatusPolling(RequestId)
    self.PendingJoinRequestId = tostring(RequestId or "")
    if self.PendingJoinRequestId == "" then
        return
    end
    self:PollJoinRequestStatus()
    if not self.JoinRequestStatusTimerHandle then
        self.JoinRequestStatusTimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate(
            {self, self.PollJoinRequestStatus}, JOIN_REQUEST_POLL_INTERVAL, true)
    end
end

function M:StopJoinRequestStatusPolling()
    self.PendingJoinRequestId = ""
    if self.JoinRequestStatusTimerHandle then
        UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self, self.JoinRequestStatusTimerHandle)
        self.JoinRequestStatusTimerHandle = nil
    end
end

function M:EnterRoom(Room, bLocalOwner)
    if not Room or not Room.RoomId or tostring(Room.RoomId) == "" then
        print("[WBP_Login] 进入房间失败：服务端成功响应中没有 RoomId")
        SetStatus("进入房间失败：服务端没有返回房间号，请检查 API 响应。")
        return false
    end

    print(string.format("[WBP_Login] 准备进入房间：RoomId=%s Owner=%s",
        tostring(Room.RoomId), tostring(bLocalOwner == true)))
    self:DestroyInvitePrompt()
    self.ActiveRoomId = Room.RoomId
    SetRoomIdInputValue(self, self.ActiveRoomId)
    self.bLocalReady = false
    self.PendingReadyState = nil
    self.bLocalRoomOwner = bLocalOwner == true
    if self.ActiveRoomId ~= "" then
        GetAccountManager(self):BeginRoomPolling(self.ActiveRoomId)
    end
    local bPageSwitched = SetActivePage(self, PAGE_INDEX_ROOM)
    self:RefreshRoomMembers(Room)
    if self.bLocalRoomOwner then
        self:StartJoinRequestPolling()
    end
    SetStatus(bPageSwitched
        and string.format("已进入房间 %s", self.ActiveRoomId)
        or string.format("已加入房间 %s，但找不到房间页面控件；请检查 WidgetSwitcher 名称。", self.ActiveRoomId))
    return true
end

function M:OnJoinRequestPromptAction(RequestId, bAccept)
    local Manager = GetAccountManager(self)
    if not Manager then
        return
    end
    local Id = tostring(RequestId or "")
    if Id == "" or self.PendingJoinRequestActionId ~= "" then
        return
    end
    self.PendingJoinRequestActionId = Id
    self:SetJoinRequestPromptVisible(false)
    Manager:RespondToJoinRequest(Id, bAccept)
    SetStatus(bAccept and "正在接受加入申请…" or "正在拒绝加入申请…")
end

function M:OnLoginCompleted(bSuccess, StatusCode, Message)
    SetStatus(Message)
    if bSuccess then
        -- 登录成功后进入 WidgetSwitcher 的第二页（索引从 0 开始，因此为 1）。
        SetActivePage(self, 1)
        self:StartInvitePolling()
    end
end

function M:CreateInvitePrompt()
    if self.InvitePromptWidget then
        return self.InvitePromptWidget
    end
    local InviteClass = UE.UClass.Load(INVITE_PROMPT_WIDGET_CLASS)
    if not InviteClass then
        print("[WBP_Login] 找不到 WBP_InvitePlayer 蓝图类")
        return nil
    end
    local Prompt = UE.UWidgetBlueprintLibrary.Create(self, InviteClass, self:GetOwningPlayer())
    if not Prompt then
        print("[WBP_Login] 创建 WBP_InvitePlayer 失败")
        return nil
    end

    Prompt:AddToViewport(20)
    -- AddToViewport 会触发弹窗的 Construct；必须在 Construct 之后设置父界面引用，
    -- 否则弹窗 Construct 中初始化 LoginWidget 会把它清空。
    Prompt:SetLoginWidget(self)
    Prompt:SetAlignmentInViewport(UE.FVector2D(1.0, 1.0))
    local ViewportSize = UE.UWidgetLayoutLibrary.GetViewportSize(self)
    if ViewportSize then
        Prompt:SetPositionInViewport(
            UE.FVector2D(ViewportSize.X - 32.0, ViewportSize.Y - 32.0), true)
    end
    Prompt:SetRenderOpacity(0.0)
    Prompt:SetVisibility(UE.ESlateVisibility.Collapsed)
    self.InvitePromptWidget = Prompt
    return Prompt
end

function M:DestroyInvitePrompt()
    if self.InvitePromptWidget then
        self.InvitePromptWidget:RemoveFromParent()
        self.InvitePromptWidget = nil
    end
end

function M:StartInvitePolling()
    local Manager = GetAccountManager(self)
    if not Manager then
        return
    end
    self:CreateInvitePrompt()
    Manager:GetMyRoomInvites()
    if not self.InvitePollingTimerHandle then
        self.InvitePollingTimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate(
            {self, self.PollInvites}, INVITE_POLL_INTERVAL, true)
    end
end

function M:PollInvites()
    local Manager = GetAccountManager(self)
    if Manager then
        Manager:GetMyRoomInvites()
    end
end

function M:OnInvitePromptAction(InviteId, bAccept)
    local Manager = GetAccountManager(self)
    if not Manager then
        print("[WBP_Login] 接受/拒绝邀请失败：AccountManager 不存在")
        return
    end
    local Id = tostring(InviteId or "")
    if Id == "" or self.PendingInviteActionId ~= "" then
        return
    end
    self.PendingInviteActionId = Id
    self:SetInvitePromptVisible(false)
    print(string.format("[WBP_Login] 邀请操作：%s，InviteId=%s", bAccept and "接受" or "拒绝", Id))
    if bAccept then
        Manager:AcceptRoomInvite(Id)
        SetStatus("正在接受邀请…")
    else
        Manager:RejectRoomInvite(Id)
        SetStatus("正在拒绝邀请…")
    end
end

function M:SetInvitePromptVisible(bVisible)
    local Prompt = self.InvitePromptWidget
    if not Prompt then
        return
    end
    Prompt:SetRenderOpacity(bVisible and 1.0 or 0.0)
    Prompt:SetVisibility(bVisible and UE.ESlateVisibility.Visible or UE.ESlateVisibility.Collapsed)
end

function M:OnClickPageCreate()
    SetActivePage(self, PAGE_INDEX_CREATE)
end

function M:OnClickPageJoin()
    SetActivePage(self, PAGE_INDEX_JOIN)
end

function M:OnClickPageInvite()
    SetActivePage(self, PAGE_INDEX_ROOM)
end

-- 三个页面可以共用一个确认按钮：创建页创建房间，加入页提交房间 ID，
-- 已进入房间后则切换准备状态。邀请仍由独立的邀请按钮触发。
function M:OnClickConfirm()
    print("[WBP_Login] 确认按钮点击回调已触发")
    local Switcher = self.WS_Login
    if not Switcher then
        SetStatus("找不到 WS_Login。")
        return
    end

    local PageIndex = Switcher:GetActiveWidgetIndex()
    if PageIndex == PAGE_INDEX_CREATE then
        self:OnClickCreateRoom()
    elseif PageIndex == PAGE_INDEX_JOIN then
        self:OnClickJoinRoom()
    elseif self.ActiveRoomId and self.ActiveRoomId ~= "" then
        self:OnClickToggleReady()
    else
        SetStatus("当前页面没有可执行的确认操作。")
    end
end

function M:OnClickCreateRoom()
    local Manager = GetAccountManager(self)
    if not Manager then
        return
    end

    local MapName = GetTextValue(self.MapNameInput_Create)
    if MapName == "" then
        MapName = DEFAULT_MAP_NAME
    end

    -- 房间人数固定为 4，不提供人数上限设置控件。
    Manager:CreateRoom(MapName, MAX_ROOM_PLAYERS)
    SetStatus("正在创建房间…")
end

function M:OnClickJoinRoom()
    print("[WBP_Login] 点击请求加入房间")
    local Manager = GetAccountManager(self)
    if not Manager then
        print("[WBP_Login] 请求加入房间失败：AccountManager 不存在")
        SetStatus("请求失败：找不到 AccountManager。")
        return
    end
    local PlayerUsername = GetJoinTargetUsername(self)
    print(string.format("[WBP_Login] 请求加入玩家房间：Username='%s'", tostring(PlayerUsername)))
    if PlayerUsername == "" then
        SetStatus("请输入房主的玩家名称。")
        return
    end

    self:StopJoinRequestStatusPolling()
    Manager:JoinRoom(PlayerUsername)
    SetStatus(string.format("正在向玩家 %s 的房间发送加入申请…", PlayerUsername))
end

function M:OnClickInvitePlayer()
    local Manager = GetAccountManager(self)
    if not Manager then
        return
    end
    local RoomId = GetRoomId(self)
    local Username = GetTextValue(self.PlayerNameInput_Invite)
    if RoomId == "" then
        SetStatus("请先创建或加入房间。")
        return
    end
    if Username == "" then
        SetStatus("请输入要邀请的玩家名称。")
        return
    end

    Manager:InvitePlayer(RoomId, Username)
    SetStatus("正在发送邀请…")
end

function M:OnClickToggleReady()
    local Manager = GetAccountManager(self)
    if not Manager then
        return
    end
    local RoomId = GetRoomId(self)
    if RoomId == "" then
        SetStatus("请先创建或加入房间。")
        return
    end

    self.PendingReadyState = not self.bLocalReady
    Manager:SetRoomReady(RoomId, self.PendingReadyState)
    SetStatus(self.PendingReadyState and "正在设置为准备…" or "正在取消准备…")
end

function M:OnClickStartRoom()
    print("[WBP_Login] 点击开始游戏")
    local Manager = GetAccountManager(self)
    if not Manager then
        print("[WBP_Login] 开始游戏失败：AccountManager 不存在")
        SetStatus("开始失败：找不到 AccountManager。")
        return
    end
    local RoomId = GetRoomId(self)
    print(string.format("[WBP_Login] 开始房间：RoomId='%s'", tostring(RoomId)))
    if RoomId == "" then
        SetStatus("请先创建或加入房间。")
        return
    end

    Manager:StartRoom(RoomId)
    SetStatus("正在请求开始对局…")
end

function M:OnRoomInvitesReceived(bSuccess, StatusCode, Message, Invites)
    self.Invites = Invites
    if not bSuccess then
        SetStatus(Message)
        return
    end

    local InviteCount = self.Invites and self.Invites:Num() or 0
    local Invite = nil
    for Index = 1, InviteCount do
        local Candidate = self.Invites:Get(Index)
        local CandidateId = tostring(Candidate.InviteId or "")
        if CandidateId ~= ""
            and not self.HandledInviteIds[CandidateId]
            and CandidateId ~= self.PendingInviteActionId then
            Invite = Candidate
            break
        end
    end
    if Invite then
        self.SelectedInviteId = tostring(Invite.InviteId)
        local Prompt = self:CreateInvitePrompt()
        if Prompt then
            Prompt:SetInviteInfo(Invite.InviteId, Invite.InviterUsername, Invite.MapName)
            self:SetInvitePromptVisible(true)
        end
    else
        self.SelectedInviteId = ""
        self:SetInvitePromptVisible(false)
    end
end

function M:OnRoomJoinRequestsReceived(bSuccess, StatusCode, Message, Requests)
    if not bSuccess then
        print(string.format("[WBP_Login] 查询加入申请失败：HTTP %s，%s", tostring(StatusCode), tostring(Message)))
        return
    end

    local RequestCount = Requests and Requests:Num() or 0
    local Request = nil
    for Index = 1, RequestCount do
        local Candidate = Requests:Get(Index)
        local CandidateId = tostring(Candidate.RequestId or "")
        if CandidateId ~= ""
            and not self.HandledJoinRequestIds[CandidateId]
            and CandidateId ~= self.PendingJoinRequestActionId then
            Request = Candidate
            break
        end
    end
    if not Request then
        self:SetJoinRequestPromptVisible(false)
        return
    end

    local Prompt = self:CreateJoinRequestPrompt()
    if Prompt then
        Prompt:SetJoinRequestInfo(Request.RequestId, Request.RequesterUsername, Request.MapName)
        self:SetJoinRequestPromptVisible(true)
    end
end

function M:OnRoomRequestCompleted(Operation, bSuccess, StatusCode, Message, Room, Players)
    local EOperation = UE.EAccountRoomOperation
    if Operation == EOperation.AcceptInvite or Operation == EOperation.RejectInvite then
        print(string.format("[WBP_Login] 邀请请求响应：Operation=%s Success=%s Status=%s Message=%s",
            tostring(Operation), tostring(bSuccess), tostring(StatusCode), tostring(Message)))
    end

    if Operation == EOperation.PresenceHeartbeat or Operation == EOperation.GetJoinRequests then
        return
    end

    if not bSuccess then
        if Operation == EOperation.SetReady then
            self.PendingReadyState = nil
        end
        if Operation == EOperation.JoinRequestStatus then
            self:StopJoinRequestStatusPolling()
        elseif Operation == EOperation.AcceptInvite or Operation == EOperation.RejectInvite then
            self.PendingInviteActionId = ""
            self:PollInvites()
        elseif Operation == EOperation.AcceptJoinRequest or Operation == EOperation.RejectJoinRequest then
            self.PendingJoinRequestActionId = ""
            self:PollJoinRequests()
        end
        if Operation == EOperation.JoinRoom or Operation == EOperation.JoinRequestStatus then
            print(string.format("[WBP_Login] 加入房间流程失败：Operation=%s HTTP=%s Message=%s",
                tostring(Operation), tostring(StatusCode), tostring(Message)))
        elseif Operation == EOperation.StartRoom then
            print(string.format("[WBP_Login] 开始游戏请求失败：HTTP=%s Message=%s",
                tostring(StatusCode), tostring(Message)))
        end
        SetStatus(Message)
        return
    end

    if Operation == EOperation.JoinRoom then
        print(string.format("[WBP_Login] 加入申请已创建：RequestId=%s Status=%s",
            tostring(Room.JoinRequestId), tostring(Room.JoinRequestStatus)))
        if Room.JoinRequestId == "" then
            print("[WBP_Login] 服务端返回成功，但没有 request_id，无法开始状态轮询")
            SetStatus("服务端没有返回加入申请 ID。")
            return
        end
        self:StartJoinRequestStatusPolling(Room.JoinRequestId)
        SetStatus("加入申请已发送，等待房主处理…")
        return
    end

    if Operation == EOperation.JoinRequestStatus then
        print(string.format("[WBP_Login] 加入申请状态：%s，RoomId=%s",
            tostring(Room.JoinRequestStatus), tostring(Room.RoomId)))
        if Room.JoinRequestStatus == "accepted" then
            self:StopJoinRequestStatusPolling()
            self:EnterRoom(Room, false)
        elseif Room.JoinRequestStatus == "rejected" or Room.JoinRequestStatus == "expired" then
            self:StopJoinRequestStatusPolling()
            SetStatus(
                Room.JoinRequestStatus == "rejected" and "房主拒绝了加入申请。" or "加入申请已失效。")
        end
        return
    end

    if Operation == EOperation.AcceptJoinRequest or Operation == EOperation.RejectJoinRequest then
        local CompletedRequestId = self.PendingJoinRequestActionId
        if CompletedRequestId ~= "" then
            self.HandledJoinRequestIds[CompletedRequestId] = true
        end
        self.PendingJoinRequestActionId = ""
        self:DestroyJoinRequestPrompt()
        if Operation == EOperation.AcceptJoinRequest then
            self:RefreshRoomMembers(Room)
            SetStatus("已同意玩家加入房间。")
        else
            SetStatus("已拒绝玩家的加入申请。")
        end
        self:PollJoinRequests()
        return
    end

    if Operation == EOperation.CreateRoom
        or Operation == EOperation.AcceptInvite then
        print(string.format("[WBP_Login] 房间进入回调：Operation=%s RoomId=%s Map=%s",
            tostring(Operation), tostring(Room and Room.RoomId or ""), tostring(Room and Room.MapName or "")))
        if Operation == EOperation.AcceptInvite and self.PendingInviteActionId ~= "" then
            self.HandledInviteIds[self.PendingInviteActionId] = true
            self.PendingInviteActionId = ""
        end
        self:EnterRoom(Room, Operation == EOperation.CreateRoom)
        return
    end

    if Operation == EOperation.RoomStatus or Operation == EOperation.SetReady then
        if Room.RoomId ~= "" then
            self.ActiveRoomId = Room.RoomId
        end
        self:RefreshRoomMembers(Room)
        if Operation == EOperation.SetReady then
            self.bLocalReady = self.PendingReadyState == true
            self.PendingReadyState = nil
        end
        if Room.Status ~= "" then
            SetStatus(string.format("房间状态：%s", Room.Status))
        end
        return
    end

    if Operation == EOperation.InvitePlayer then
        SetStatus(Message ~= "" and Message or "邀请已发送。")
        return
    end

    if Operation == EOperation.RejectInvite then
        if self.PendingInviteActionId ~= "" then
            self.HandledInviteIds[self.PendingInviteActionId] = true
        end
        self.PendingInviteActionId = ""
        self:DestroyInvitePrompt()
        SetStatus(Message ~= "" and Message or "已拒绝邀请。")
        self:PollInvites()
        return
    end

    if Operation == EOperation.StartRoom then
        print(string.format("[WBP_Login] 开始游戏请求成功：Status=%s Server=%s:%s",
            tostring(Room.Status), tostring(Room.ServerAddress), tostring(Room.ServerPort)))
        SetStatus(Message ~= "" and Message or "正在启动对局…")
        return
    end

end

function M:RefreshRoomMembers(Room)
    if not Room then
        return
    end

    local Members = Room.Members
    local PlayerList = self.VerticalBox_PlayerReady
    local Count = Members and Members:Num() or 0
    local Lines = {}
    local CurrentUsername = GetTextValue(self.UsernameInput_Login)

    if PlayerList then
        PlayerList:ClearChildren()
    else
        print("[WBP_Login] 找不到 VerticalBox_PlayerReady，请确认控件已勾选 Is Variable 且名称一致")
    end
    self.PlayerRowWidgets = {}

    for Index = 1, Count do
        local Member = Members:Get(Index)
        local ReadyText = Member.bReady and "已准备" or "未准备"
        Lines[#Lines + 1] = string.format("%s  %s", Member.Username, ReadyText)
        if Member.Username == CurrentUsername then
            self.bLocalReady = Member.bReady
        end

        if PlayerList then
            local PlayerClass = UE.UClass.Load(ROOM_PLAYER_WIDGET_CLASS)
            if PlayerClass then
                local Row = UE.UWidgetBlueprintLibrary.Create(self, PlayerClass, self:GetOwningPlayer())
                if Row then
                    Row:SetPlayerInfo(Member.Username, Member.bReady)
                    PlayerList:AddChild(Row)
                    self.PlayerRowWidgets[#self.PlayerRowWidgets + 1] = Row
                else
                    print("[WBP_Login] 创建 WBP_PlayerReady 失败")
                end
            else
                print("[WBP_Login] 找不到 /Game/Blueprint/UI/Login/WBP_PlayerReady")
            end
        end
    end

    print("[WBP_Login] 房间成员：" .. table.concat(Lines, "\n"))
end

return M
