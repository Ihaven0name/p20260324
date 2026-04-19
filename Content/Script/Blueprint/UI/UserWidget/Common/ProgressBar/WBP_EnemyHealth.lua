--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_EnemyHealth_C
local M = UnLua.Class()

--- ========== 常量 ==========

local INTERP_SPEED = 5   -- 插值速度
local RED_DELAY = 1.0      -- 延迟等待时间（秒）


function M:Initialize(Initializer)
end

function M:PreConstruct(IsDesignTime)
end

function M:Construct()
    self.targetPercent = 1.0
    self.pendingTargetPercent = 1.0
    self.bGrayIsActive = false
    self.bRedInterp = false
    self.bRedDelayActive = false
    self.enemyCharacter = nil
end

--- 每帧更新：Red 血条缓动插值（FInterpTo）
function M:Tick(MyGeometry, InDeltaTime)
    if not self.bRedInterp then
        return
    end

    local currentRed = self.ProgressBar_Red.Percent
    local newRed = UE.UKismetMathLibrary.FInterpTo(currentRed, self.targetPercent, InDeltaTime, INTERP_SPEED)
    self.ProgressBar_Red:SetPercent(newRed)

    if math.abs(newRed - self.targetPercent) < 0.001 then
        self.ProgressBar_Red:SetPercent(self.targetPercent)
        self.bRedInterp = false
        self.ProgressBar_Gray:SetRenderOpacity(1.0)
    end
end

--- WidgetController 设置完成后存储 Controller 并绑定事件
function M:AfterSetWidgetController()
    local wc = self:GetWidgetController()
    if wc then
        self.enemyCharacter = wc:Cast(UE.AEnemyCharacter)
    end

    -- 绑定委托（函数引用格式）
    self.enemyCharacter.OnEnemyHealthPercentChanged:Add(self, self.OnEnemyHealthPercentChangedFunction)
    -- 初始化血量显示
    self:OnEnemyHealthPercentChangedFunction(1.0)
end

--- 血量百分比变化回调
---@param NewPercent number 新的血量百分比 (0~1)
function M:OnEnemyHealthPercentChangedFunction(NewPercent)

    if NewPercent >= self.ProgressBar_Green.Percent then
        -- ========== 恢复生命 ==========
        self:CancelRedDelay()
        self.bRedInterp = false

        self.ProgressBar_Gray:SetPercent(NewPercent)
        self.ProgressBar_Red:SetPercent(NewPercent)
        self.targetPercent = NewPercent
        if bRedInterp then
            self.bRedInterp = false
            self.ProgressBar_Gray:SetRenderOpacity(1.0)
        end

        if not self.bGrayIsActive then
            self:PlayAnimation(self.RecoveryHealth, 0.0, 1, UE.EUMGSequencePlayMode.Forward, 1.0)
        end
    else
        -- ========== 降低生命 ==========
        self.ProgressBar_Green:SetPercent(NewPercent)
        self.ProgressBar_Gray:SetRenderOpacity(0.0)

        if self.bRedInterp then
            -- 已在插值中 → 直接改目标
            self.targetPercent = NewPercent
        elseif self.bRedDelayActive then
            -- 延迟等待中 → 重置计时，更新待执行值
            self.pendingTargetPercent = NewPercent
            self:ClearLuaTimer("RedDelay")
            self:SetLuaTimer("RedDelay", RED_DELAY, false)
        else
            -- 无活跃延迟也无插值 → 开始新的延迟
            self.pendingTargetPercent = NewPercent
            self.bRedDelayActive = true
            self:SetLuaTimer("RedDelay", RED_DELAY, false)
        end
    end
end

--- 取消 Red 延迟 + 关闭插值状态
function M:CancelRedDelay()
    self:ClearLuaTimer("RedDelay")
    self.bRedDelayActive = false
    self.bRedInterp = false
end

--- 定时器回调：延迟结束 → 开始让 Red 缓动追赶 Green
function M:OnLuaTimerEvent(TimerName)
    if TimerName == "RedDelay" then
        self.bRedDelayActive = false
        self.targetPercent = self.pendingTargetPercent
        self.bRedInterp = true
        self:ClearLuaTimer("RedDelay")
    end
end

--- 恢复动画开始事件
function M:RecoveryHealth_AnimStart(ProgressBar_Gray)
    self.bGrayIsActive = true
end

--- 恢复动画结束事件：Green 跟上 Gray
function M:RecoveryHealth_AnimEnd(ProgressBar_Gray)
    self.bGrayIsActive = false
    self.ProgressBar_Green:SetPercent(ProgressBar_Gray.Percent)
end

return M
