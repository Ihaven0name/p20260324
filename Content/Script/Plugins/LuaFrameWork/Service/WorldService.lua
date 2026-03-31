local WorldService = UnLua.Class()

function WorldService:LuaInit()
    print("WorldService:LuaInit")
end

function WorldService:LuaPostInit()
    print("WorldService:LuaPostInit")
end

function WorldService:LuaOnWorldBeginPlay()
    print("WorldService:LuaOnWorldBeginPlay")
end

function WorldService:LuaDeinit()
    print("WorldService:LuaDeinit")
end

return WorldService