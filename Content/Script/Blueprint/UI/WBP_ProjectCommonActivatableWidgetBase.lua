--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_ProjectCommonActivatableWidgetBase_C
local M = UnLua.Class()

---@param TagToFunction table
---@param ActionInfo UInputActionInfo
function M:RegisterAction(TagToFunction,ActionInfo)
    for Key,Value in pairs(TagToFunction) do
        local InputAction = ActionInfo:FindActionForTag(Key)
        if InputAction then
            self:BindAction(Key,InputAction,{self,Value})
        end
        print("Registering")
    end
    print("Register Completed")
end

return M
