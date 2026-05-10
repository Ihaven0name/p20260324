local UIManager = UnLua.Class()

function UIManager:LuaInit()
    print("UIManager:LuaInit")
    self._WidgetMap = {}
end

function UIManager:LuaPostInit()
    print("UIManager:LuaPostInit")
end

function UIManager:LuaOnWorldBeginPlay(InWorld)
    print("UIManager:LuaOnWorldBeginPlay")
    local worldName = UE.UKismetSystemLibrary.GetObjectName(InWorld)
    if worldName == "Demo" then
        local path = '/Game/UI/UIBP/Login/WBP_LoginView.WBP_LoginView_C'
        self:OpenWidgetByPath(path, "Login")
    end
end

function UIManager:OpenWidgetByPath(Path, Name)
    -- local path = '/Game/UI/UIBP/Login/WBP_LoginView.WBP_LoginView_C'
    if self._WidgetMap[Name] then
        return self._WidgetMap[Name]
    end
    local widget = WorldService:GetWidgetByPath(Path)
    widget:AddToViewport()
    self._WidgetMap[Name] = widget
end

function UIManager:RemoveWidgetByName(Name)
    if self._WidgetMap[Name] then
        self._WidgetMap[Name]:RemoveFromViewport()
        self._WidgetMap[Name] = nil
    end
end

function UIManager:RemoveAllWidget()
    for k, v in pairs(self._WidgetMap) do
        self:RemoveWidgetByName(k)
    end
end

function UIManager:LuaDeinit()
    self:RemoveAllWidget()
    print("UIManager:LuaDeinit")
end

return UIManager
