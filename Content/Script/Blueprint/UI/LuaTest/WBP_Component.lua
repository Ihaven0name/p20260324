--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Component_C
local M = UnLua.Class()

function M:Initialize(Initializer)
    self._data = nil
    self._callBack = nil
    self._binObj = nil
    self._binParam = nil
end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    self.Button_219.OnClicked:Add(self,self.OnBtnClick)
end

function M:OnListItemObjectSet(Obj)
    self._data = Obj
    self.TextBlock_43:SetText(Obj.Name)
    self._binObj = Obj.BinObj
    self._binParam = Obj.Frame or {}
    self._callBack = Obj.Func
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:OnBtnClick()
    if self._callBack then
        self._callBack(self._binObj, self._binParam)
    end
end

return M
