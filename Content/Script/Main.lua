-- Content/Script/Main.lua

---@class UWidgetClassMap
---@field CommonWidgetClass string
WidgetClassMap = {
    CommonWidgetClass = "Blueprint.UI.WBP_ProjectCommonActivatableWidgetBase"
}


require("LuaPanda").start("127.0.0.1",8818)
package.cpath = package.cpath .. ';C:/Users/76334/AppData/Roaming/JetBrains/Rider2024.3/plugins/EmmyLua/debugger/emmy/windows/x64/?.dll'
local dbg = require('emmy_core')
dbg.tcpListen('localhost', 9966)