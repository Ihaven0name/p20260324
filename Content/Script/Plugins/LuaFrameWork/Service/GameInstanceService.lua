local GameInstanceService = UnLua.Class()
local UIManagerService = require("Service.UIManager")

function GameInstanceService:LuaInit()
    print("GameIns LuaInit")
    _G.GameInstanceService = self
    _G.UIManagerService=UIManagerService
end

function GameInstanceService:LuaDeinit()
    print("GameIns LuaDeinit")
end

function _G.ReceiveNotifyFromC(eventName, ...)
    print("ReceiveNotifyFromC")
    -- EventSys:Dispatch(eventName, ...)
end

return GameInstanceService