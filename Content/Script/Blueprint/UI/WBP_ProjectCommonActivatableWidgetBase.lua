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
    if not ActionInfo then
        print(string.format("[UIDiag] RegisterAction failed: InputActionInfo missing widget=%s", self:GetName()))
        return
    end

    for Key,Value in pairs(TagToFunction) do
        local InputAction = ActionInfo:FindActionForTag(Key)
        if InputAction then
            print(string.format("[UIDiag] RegisterAction widget=%s tag=%s input=%s", self:GetName(), tostring(Key), InputAction:GetName()))
            self:BindAction(Key,InputAction,{self,Value})
        else
            print(string.format("[UIDiag] RegisterAction missing input widget=%s tag=%s", self:GetName(), tostring(Key)))
        end
    end
    print(string.format("[UIDiag] RegisterAction completed widget=%s", self:GetName()))
end

return M
