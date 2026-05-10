--
-- DESCRIPTION
-- 通用按钮槽组件：支持 Normal/Hovered/Pressed/Disabled 四态切换 + 音效 + 背景图
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_ButtonSlot_C
local M = UnLua.Class()



function M:AfterSetWidgetController()
    self:BindButtonEvents()
end
--- 绑定按钮事件：Hover 播放音效，Press 播放按压音效
function M:BindButtonEvents()
    if not self.Button then return end

    -- Hovered 事件 → 播放 Hover 音效
    self.Button.OnHovered:Add(function()
        if self.Button_Hovered_Sound then
            UGameplayStatics.PlaySound2D(self, self.Button_Hovered_Sound, 1.0)
        end
    end)

    -- Pressed 事件 → 播放 Press 音效
    self.Button.OnPressed:Add(function()
        if self.Button_Pressed_Sound then
            UGameplayStatics.PlaySound2D(self, self.Button_Pressed_Sound, 1.0)
        end
    end)
end


return M
