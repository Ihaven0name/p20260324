-- Content/Script/Main.lua

---@class UWidgetClassMap
---@field CommonWidgetClass string
WidgetClassMap = {
    CommonWidgetClass = "Blueprint.UI.WBP_ProjectCommonActivatableWidgetBase"
}


require("LuaPanda").start("127.0.0.1",8818)
package.cpath = package.cpath .. ';D:/JetBrain_Files/Rider2024.3/config/plugins/EmmyLua/debugger/emmy/windows/x64/?.dll'
local dbg = require('emmy_core')
dbg.tcpListen('localhost', 9966)