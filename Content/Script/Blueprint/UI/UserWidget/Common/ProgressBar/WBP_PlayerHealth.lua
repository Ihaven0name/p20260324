---@type WBP_PlayerHealth_C
local M = UnLua.Class()

local INTERP_SPEED = 5.0
local RED_DELAY = 1.0

function M:Initialize(Initializer)
    self.MainAttackUIWidgetController = nil
    self.CurrentHealth = 0.0
    self.MaxHealth = 0.0
    self.TargetPercent = 1.0
    self.PendingTargetPercent = 1.0
    self.bGrayIsActive = false
    self.bRedInterp = false
    self.bRedDelayActive = false
    self.bHasInitializedHealthPercent = false
    self.TimerHandle = nil
end

function M:Construct()
    self.TargetPercent = 1.0
    self.PendingTargetPercent = 1.0
    self.bGrayIsActive = false
    self.bRedInterp = false
    self.bRedDelayActive = false
    self.bHasInitializedHealthPercent = false
    self.TimerHandle = nil
end

function M:AfterSetWidgetController()
    self.MainAttackUIWidgetController = self:GetWidgetController()
    if not self.MainAttackUIWidgetController then
        return
    end

    self.MainAttackUIWidgetController.OnHealthChangeSignature:Remove(self, self.OnHealthChanged)
    self.MainAttackUIWidgetController.OnHealthChangeSignature:Add(self, self.OnHealthChanged)
    self.MainAttackUIWidgetController.OnMaxHealthChangeSignature:Remove(self, self.OnMaxHealthChanged)
    self.MainAttackUIWidgetController.OnMaxHealthChangeSignature:Add(self, self.OnMaxHealthChanged)

    local ControllerParam = self.MainAttackUIWidgetController.WidgetControllerParam
    if ControllerParam and ControllerParam.AttributeSet then
        self.CurrentHealth = ControllerParam.AttributeSet:GetHealthValueForLua()
        self.MaxHealth = ControllerParam.AttributeSet:GetMaxHealthValueForLua()
        self:UpdateHealthPercent()
    end

    -- 广播当前属性，避免 UI 创建时只等后续属性变化事件。
    self.MainAttackUIWidgetController:BroadcastInitialAttribute()
end

function M:Destruct()
    if self.MainAttackUIWidgetController then
        self.MainAttackUIWidgetController.OnHealthChangeSignature:Remove(self, self.OnHealthChanged)
        self.MainAttackUIWidgetController.OnMaxHealthChangeSignature:Remove(self, self.OnMaxHealthChanged)
    end
    self:ClearTimer()
end

function M:OnHealthChanged(NewHealth)
    self.CurrentHealth = NewHealth
    self:UpdateHealthPercent()
end

function M:OnMaxHealthChanged(NewMaxHealth)
    self.MaxHealth = NewMaxHealth
    self:UpdateHealthPercent()
end

function M:UpdateHealthPercent()
    if not self.ProgressBar_Green or not self.ProgressBar_Red or not self.ProgressBar_Gray then
        return
    end

    if self.MaxHealth <= 0.0 then
        return
    end

    local NewPercent = math.max(0.0, math.min(self.CurrentHealth / self.MaxHealth, 1.0))

    if not self.bHasInitializedHealthPercent then
        if self.CurrentHealth <= 0.0 then
            return
        end

        self:ClearTimer()
        self.ProgressBar_Green:SetPercent(NewPercent)
        self.ProgressBar_Red:SetPercent(NewPercent)
        self.ProgressBar_Gray:SetPercent(NewPercent)
        self.ProgressBar_Gray:SetRenderOpacity(1.0)
        self.TargetPercent = NewPercent
        self.PendingTargetPercent = NewPercent
        self.bRedInterp = false
        self.bRedDelayActive = false
        self.bHasInitializedHealthPercent = true
        return
    end

    if NewPercent >= self.ProgressBar_Green.Percent then
        -- 回复生命时，红色延迟条立即跟上，绿色由恢复动画追赶。
        self:ClearTimer()
        self.bRedDelayActive = false
        self.bRedInterp = false
        self.ProgressBar_Gray:SetPercent(NewPercent)
        self.ProgressBar_Red:SetPercent(NewPercent)
        self.TargetPercent = NewPercent

        if not self.bGrayIsActive then
            self:PlayAnimation(self.RecoveryHealth, 0.0, 1, UE.EUMGSequencePlayMode.Forward, 1.0)
        end
    else
        -- 受伤时绿色立即下降，红色延迟后缓动追赶。
        self.ProgressBar_Green:SetPercent(NewPercent)
        self.ProgressBar_Gray:SetRenderOpacity(0.0)

        if self.bRedInterp then
            self.TargetPercent = NewPercent
        else
            self.PendingTargetPercent = NewPercent
            self.bRedDelayActive = true
            self:ClearTimer()
            self:SetTimer()
        end
    end
end

function M:Tick(MyGeometry, InDeltaTime)
    if not self.bRedInterp then
        return
    end

    local CurrentRed = self.ProgressBar_Red.Percent
    local NewRed = UE.UKismetMathLibrary.FInterpTo(CurrentRed, self.TargetPercent, InDeltaTime, INTERP_SPEED)
    self.ProgressBar_Red:SetPercent(NewRed)

    if math.abs(NewRed - self.TargetPercent) < 0.001 then
        self.ProgressBar_Red:SetPercent(self.TargetPercent)
        self.bRedInterp = false
        self.ProgressBar_Gray:SetRenderOpacity(1.0)
    end
end

function M:SetTimer()
    self.TimerHandle = UE.UKismetSystemLibrary.K2_SetTimerDelegate(
        {self, self.OnRedDelayEnd}, RED_DELAY, false)
end

function M:ClearTimer()
    if self.TimerHandle ~= nil then
        UE.UKismetSystemLibrary.K2_ClearAndInvalidateTimerHandle(self, self.TimerHandle)
        self.TimerHandle = nil
    end
end

function M:OnRedDelayEnd()
    self.bRedDelayActive = false
    self.TargetPercent = self.PendingTargetPercent
    self.bRedInterp = true
    self:ClearTimer()
end

function M:RecoveryHealth_AnimStart(ProgressBar_Gray)
    self.bGrayIsActive = true
end

function M:RecoveryHealth_AnimEnd(ProgressBar_Gray)
    self.bGrayIsActive = false
    self.ProgressBar_Green:SetPercent(ProgressBar_Gray.Percent)
end

return M
