#include "GAS/ProjectGameplayTag.h"
#include "GameplayTagsManager.h"
#include "GAS/ProjectAttributeSet.h"

FProjectGameplayTag FProjectGameplayTag::GameplayTags;
TMap<FGameplayTag, FGameplayAttribute> FProjectGameplayTag::TagToAttributeMap;
static FGameplayTag AddTag(UGameplayTagsManager& Manager, const char* Name, const char* Desc)
{
	return Manager.AddNativeGameplayTag(FName(Name), FString(Desc));
}
void FProjectGameplayTag::InitGameplayTags()
{
	if (GameplayTags.bIsInitialized) return;

	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// ==========================================
	// 鼠标
	// ==========================================
	GameplayTags.Input_Mouse_LMB = AddTag(Manager, "Input.Mouse.LMB", "Mouse Left Button");
	GameplayTags.Input_Mouse_RMB = AddTag(Manager, "Input.Mouse.RMB", "Mouse Right Button");
	GameplayTags.Input_Mouse_MMB = AddTag(Manager, "Input.Mouse.MMB", "Mouse Middle Button");
	GameplayTags.Input_Mouse_X1  = AddTag(Manager, "Input.Mouse.X1",  "Mouse Button X1");
	GameplayTags.Input_Mouse_X2  = AddTag(Manager, "Input.Mouse.X2",  "Mouse Button X2");

	// ==========================================
	// 功能键 & 修饰键
	// ==========================================
	GameplayTags.Input_Keyboard_Escape = AddTag(Manager, "Input.Keyboard.Escape", "Escape Key");
	GameplayTags.Input_Keyboard_Tab = AddTag(Manager, "Input.Keyboard.Tab", "Tab Key");
	GameplayTags.Input_Keyboard_CapsLock = AddTag(Manager, "Input.Keyboard.CapsLock", "CapsLock Key");

	GameplayTags.Input_Keyboard_LeftShift  = AddTag(Manager, "Input.Keyboard.LeftShift", "Left Shift");
	GameplayTags.Input_Keyboard_RightShift = AddTag(Manager, "Input.Keyboard.RightShift", "Right Shift");

	GameplayTags.Input_Keyboard_LeftControl  = AddTag(Manager, "Input.Keyboard.LeftControl", "Left Ctrl");
	GameplayTags.Input_Keyboard_RightControl = AddTag(Manager, "Input.Keyboard.RightControl", "Right Ctrl");

	GameplayTags.Input_Keyboard_LeftAlt  = AddTag(Manager, "Input.Keyboard.LeftAlt", "Left Alt");
	GameplayTags.Input_Keyboard_RightAlt = AddTag(Manager, "Input.Keyboard.RightAlt", "Right Alt");

	GameplayTags.Input_Keyboard_Space = AddTag(Manager, "Input.Keyboard.Space", "Space Key");

	// ==========================================
	// 通用修饰键
	// ==========================================
	GameplayTags.Input_Keyboard_Modifier_Shift   = AddTag(Manager, "Input.Keyboard.Modifier.Shift", "Any Shift");
	GameplayTags.Input_Keyboard_Modifier_Control = AddTag(Manager, "Input.Keyboard.Modifier.Control", "Any Ctrl");
	GameplayTags.Input_Keyboard_Modifier_Alt     = AddTag(Manager, "Input.Keyboard.Modifier.Alt", "Any Alt");

	// ==========================================
	// 数字键
	// ==========================================
	GameplayTags.Input_Keyboard_1 = AddTag(Manager, "Input.Keyboard.1", "Key 1");
	GameplayTags.Input_Keyboard_2 = AddTag(Manager, "Input.Keyboard.2", "Key 2");
	GameplayTags.Input_Keyboard_3 = AddTag(Manager, "Input.Keyboard.3", "Key 3");
	GameplayTags.Input_Keyboard_4 = AddTag(Manager, "Input.Keyboard.4", "Key 4");
	GameplayTags.Input_Keyboard_5 = AddTag(Manager, "Input.Keyboard.5", "Key 5");
	GameplayTags.Input_Keyboard_6 = AddTag(Manager, "Input.Keyboard.6", "Key 6");
	GameplayTags.Input_Keyboard_7 = AddTag(Manager, "Input.Keyboard.7", "Key 7");
	GameplayTags.Input_Keyboard_8 = AddTag(Manager, "Input.Keyboard.8", "Key 8");
	GameplayTags.Input_Keyboard_9 = AddTag(Manager, "Input.Keyboard.9", "Key 9");
	GameplayTags.Input_Keyboard_0 = AddTag(Manager, "Input.Keyboard.0", "Key 0");

	// ==========================================
	// 字母 Q-P
	// ==========================================
	GameplayTags.Input_Keyboard_Q = AddTag(Manager, "Input.Keyboard.Q", "Key Q");
	GameplayTags.Input_Keyboard_W = AddTag(Manager, "Input.Keyboard.W", "Key W");
	GameplayTags.Input_Keyboard_E = AddTag(Manager, "Input.Keyboard.E", "Key E");
	GameplayTags.Input_Keyboard_R = AddTag(Manager, "Input.Keyboard.R", "Key R");
	GameplayTags.Input_Keyboard_T = AddTag(Manager, "Input.Keyboard.T", "Key T");
	GameplayTags.Input_Keyboard_Y = AddTag(Manager, "Input.Keyboard.Y", "Key Y");
	GameplayTags.Input_Keyboard_U = AddTag(Manager, "Input.Keyboard.U", "Key U");
	GameplayTags.Input_Keyboard_I = AddTag(Manager, "Input.Keyboard.I", "Key I");
	GameplayTags.Input_Keyboard_O = AddTag(Manager, "Input.Keyboard.O", "Key O");
	GameplayTags.Input_Keyboard_P = AddTag(Manager, "Input.Keyboard.P", "Key P");

	// ==========================================
	// 字母 A-L
	// ==========================================
	GameplayTags.Input_Keyboard_A = AddTag(Manager, "Input.Keyboard.A", "Key A");
	GameplayTags.Input_Keyboard_S = AddTag(Manager, "Input.Keyboard.S", "Key S");
	GameplayTags.Input_Keyboard_D = AddTag(Manager, "Input.Keyboard.D", "Key D");
	GameplayTags.Input_Keyboard_F = AddTag(Manager, "Input.Keyboard.F", "Key F");
	GameplayTags.Input_Keyboard_G = AddTag(Manager, "Input.Keyboard.G", "Key G");
	GameplayTags.Input_Keyboard_H = AddTag(Manager, "Input.Keyboard.H", "Key H");
	GameplayTags.Input_Keyboard_J = AddTag(Manager, "Input.Keyboard.J", "Key J");
	GameplayTags.Input_Keyboard_K = AddTag(Manager, "Input.Keyboard.K", "Key K");
	GameplayTags.Input_Keyboard_L = AddTag(Manager, "Input.Keyboard.L", "Key L");

	// ==========================================
	// 字母 Z-M
	// ==========================================
	GameplayTags.Input_Keyboard_Z = AddTag(Manager, "Input.Keyboard.Z", "Key Z");
	GameplayTags.Input_Keyboard_X = AddTag(Manager, "Input.Keyboard.X", "Key X");
	GameplayTags.Input_Keyboard_C = AddTag(Manager, "Input.Keyboard.C", "Key C");
	GameplayTags.Input_Keyboard_V = AddTag(Manager, "Input.Keyboard.V", "Key V");
	GameplayTags.Input_Keyboard_B = AddTag(Manager, "Input.Keyboard.B", "Key B");
	GameplayTags.Input_Keyboard_N = AddTag(Manager, "Input.Keyboard.N", "Key N");
	GameplayTags.Input_Keyboard_M = AddTag(Manager, "Input.Keyboard.M", "Key M");

	// ==========================================
	// 符号键
	// ==========================================
	GameplayTags.Input_Keyboard_Tilde        = AddTag(Manager, "Input.Keyboard.Tilde", "~");
	GameplayTags.Input_Keyboard_Minus        = AddTag(Manager, "Input.Keyboard.Minus", "-");
	GameplayTags.Input_Keyboard_Equals       = AddTag(Manager, "Input.Keyboard.Equals", "=");
	GameplayTags.Input_Keyboard_LeftBracket  = AddTag(Manager, "Input.Keyboard.LeftBracket", "[");
	GameplayTags.Input_Keyboard_RightBracket = AddTag(Manager, "Input.Keyboard.RightBracket", "]");
	GameplayTags.Input_Keyboard_Backslash    = AddTag(Manager, "Input.Keyboard.Backslash", "\\");
	GameplayTags.Input_Keyboard_Semicolon    = AddTag(Manager, "Input.Keyboard.Semicolon", ";");
	GameplayTags.Input_Keyboard_Quote        = AddTag(Manager, "Input.Keyboard.Quote", "'");
	GameplayTags.Input_Keyboard_Comma        = AddTag(Manager, "Input.Keyboard.Comma", ",");
	GameplayTags.Input_Keyboard_Period       = AddTag(Manager, "Input.Keyboard.Period", ".");
	GameplayTags.Input_Keyboard_Slash        = AddTag(Manager, "Input.Keyboard.Slash", "/");

	// ==========================================
	// Ability - 技能标签
	// ==========================================
	GameplayTags.Ability_Offensive_Crouch_Attack_NormalAttack_Combo1 = AddTag(Manager, "Ability.Offensive.Crouch.Attack.NormalAttack.Combo1", "蹲下普通攻击连段1");
	GameplayTags.Ability_Offensive_Crouch_Attack_NormalAttack_Combo2 = AddTag(Manager, "Ability.Offensive.Crouch.Attack.NormalAttack.Combo2", "蹲下普通攻击连段2");
	GameplayTags.Ability_Offensive_Crouch_Attack_NormalAttack_Combo3 = AddTag(Manager, "Ability.Offensive.Crouch.Attack.NormalAttack.Combo3", "蹲下普通攻击连段3");
	GameplayTags.Ability_Offensive_Crouch_Attack_NormalAttack_Combo4 = AddTag(Manager, "Ability.Offensive.Crouch.Attack.NormalAttack.Combo4", "蹲下普通攻击连段4");
	GameplayTags.Ability_Offensive_Crouch_Attack_HeavyAttack_Combo1 = AddTag(Manager, "Ability.Offensive.Crouch.Attack.HeavyAttack.Combo1", "重攻击连段1");
	GameplayTags.Ability_Offensive_Crouch_Skill_Dash = AddTag(Manager, "Ability.Offensive.Crouch.Skill.Dash", "蹲下技能-冲刺");
	GameplayTags.Ability_Offensive_Crouch_Roll=AddTag(Manager,"Ability.Offensive.Crouch.Roll","进行翻滚，可以实现闪避");

	// Passive 父标签（用于区分主动/被动技能）
	GameplayTags.Ability_Passive = AddTag(Manager, "Ability.Passive", "被动技能");

	// ==========================================
	// Ability Judge - 预输入判定标签
	// ==========================================
	GameplayTags.Ability_Judge_Crouch_Attack = AddTag(Manager, "Ability.Judge.Crouch.Attack", "普通攻击判定（Combo1~4）");
	GameplayTags.Ability_Judge_Crouch_Skill = AddTag(Manager, "Ability.Judge.Crouch.Skill.Dash", "冲刺技能判定");
	GameplayTags.Ability_Judge_Crouch_Roll = AddTag(Manager, "Ability.Judge.Crouch.Roll", "翻滚判定");

	// ==========================================
	// Ability Effect - 技能效果/状态效果标签
	// ==========================================
	GameplayTags.Ability_Effect_KnockBack = AddTag(Manager, "Ability.Effect.KnockBack", "击退效果");

	// ==========================================
	// Ability Condition - 技能条件标签
	// ==========================================
	GameplayTags.Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo1 = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Attack.NormalAttack.Combo1", "蹲下普通攻击连段1条件");
	GameplayTags.Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo2 = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Attack.NormalAttack.Combo2", "蹲下普通攻击连段2条件");
	GameplayTags.Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo3 = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Attack.NormalAttack.Combo3", "蹲下普通攻击连段3条件");
	GameplayTags.Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo4 = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Attack.NormalAttack.Combo4", "蹲下普通攻击连段4条件");
	GameplayTags.Ability_Condition_Offensive_Crouch_Attack_HeavyAttack_Combo1 = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Attack.HeavyAttack.Combo1", "蹲下技能-冲刺");
	GameplayTags.Ability_Condition_Offensive_Crouch_Skill_Dash = AddTag(Manager, "Ability.Condition.Offensive.Crouch.Skill.Dash", "蹲下技能-冲刺条件");

	// ==========================================
	// Block - 输入屏蔽标签
	// ==========================================
	GameplayTags.Block_InputAction_Move = AddTag(Manager, "Block.InputAction.Move", "屏蔽移动输入");
	GameplayTags.Block_InputAction_Look = AddTag(Manager, "Block.InputAction.Look", "屏蔽视角输入");
	GameplayTags.Block_InputAction_SpringArm = AddTag(Manager, "Block.InputAction.SpringArm", "屏蔽弹簧臂输入");
	GameplayTags.Block_InputAction_Ability = AddTag(Manager, "Block.InputAction.Ability", "屏蔽技能输入");
	GameplayTags.Block_InputAction_Jump = AddTag(Manager, "Block.InputAction.Jump","屏蔽跳跃输入");

	// Block - 技能互斥屏蔽
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo1 = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.NormalAttack.Combo1", "屏蔽蹲下普攻连段1");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo2 = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.NormalAttack.Combo2", "屏蔽蹲下普攻连段2");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo3 = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.NormalAttack.Combo3", "屏蔽蹲下普攻连段3");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo4 = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.NormalAttack.Combo4", "屏蔽蹲下普攻连段4");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_Skill_Dash = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.Skill.Dash", "屏蔽蹲下冲刺");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_HeavyAttack_Combo1 = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.HeavyAttack.Combo1", "屏蔽重攻击连段1");
	GameplayTags.Block_InputAction_Ability_Offensive_Crouch_Roll = AddTag(Manager, "Block.InputAction.Ability.Offensive.Crouch.Roll", "屏蔽翻滚");

	// ==========================================
	//  - 动画标签
	// ==========================================
	GameplayTags.Inventory_Material_Experience_Large = AddTag(Manager, "Inventory.Material.Experience.Large", "Large Experience Material");
	GameplayTags.Inventory_Material_Experience_Medium = AddTag(Manager, "Inventory.Material.Experience.Medium", "Medium Experience Material");
	GameplayTags.Inventory_Material_Experience_Small = AddTag(Manager, "Inventory.Material.Experience.Small", "Small Experience Material");
	
	// ==========================================
	// Animation - 动画标签
	// ==========================================
	GameplayTags.Animation_ANS_InputWindow = AddTag(Manager, "Animation.ANS.InputWindow", "输入窗口");
	GameplayTags.Animation_ANS_PreInputWindow = AddTag(Manager, "Animation.ANS.PreInputWindow", "预输入窗口");

	// ==========================================
	// Event - 动画发送事件标签
	// ==========================================
	GameplayTags.Event_Crouch_Dash_AddLaunch = AddTag(Manager, "Event.Crouch.Dash.AddLaunch", "Dash的时候添加向前的推力");

	// ==========================================
	// UI - 界面标签
	// ==========================================
	// --- Layers ---
	GameplayTags.UI_Layer_Gameplay = AddTag(Manager, "UI.Layer.Gameplay", "Gameplay Layer (HUD)");
	GameplayTags.UI_Layer_Prompt = AddTag(Manager, "UI.Layer.Prompt", "Prompt Layer");

	// --- Widgets ---
	GameplayTags.UI_Widget_MainAttack = AddTag(Manager, "UI.Widget.MainAttack", "Main Attack Widget");
	GameplayTags.UI_Widget_Attribute = AddTag(Manager, "UI.Widget.Attribute", "Attribute Widget");
	GameplayTags.UI_Widget_Setting = AddTag(Manager, "UI.Widget.Setting", "Setting Widget");


	// ==========================================
	// UI Action - 每个Common Activatable Widget需要对应的Action
	// ==========================================
	GameplayTags.UI_Action_MainAttack_AddAttributeUI = AddTag(Manager, "UI.Action.MainAttack.AddAttributeUI", "Add Attribute UI");
	GameplayTags.UI_Action_MainAttack_AddSettingUI = AddTag(Manager, "UI.Action.MainAttack.AddSettingUI", "Add Setting UI");
	GameplayTags.UI_Action_Attribute_RemoveAttributeUI = AddTag(Manager, "UI.Action.Attribute.RemoveAttributeUI", "Close Attribute UI");
	GameplayTags.UI_Action_Confirm = AddTag(Manager, "UI.Action.Confirm", "CommonUI Confirm Action");
	GameplayTags.UI_Action_Cancel = AddTag(Manager, "UI.Action.Cancel", "CommonUI Cancel/Back Action");
	GameplayTags.UI_Action_PreviousTab = AddTag(Manager, "UI.Action.PreviousTab", "CommonUI Previous Tab Action");
	GameplayTags.UI_Action_NextTab = AddTag(Manager, "UI.Action.NextTab", "CommonUI Next Tab Action");

	// ==========================================
	// State - 玩家状态标签（比如某个角色升级享有翻倍效果）
	// ==========================================

	GameplayTags.State_Crouch_XP_Double = AddTag(Manager, "State.Crouch.XP.Double", "Double XP multiplier state");
	GameplayTags.State_Crouch_XP_Triple = AddTag(Manager, "State.Crouch.XP.Triple", "Triple XP multiplier state");
	GameplayTags.State_Wraith_XP_Double = AddTag(Manager, "State.Wraith.XP.Double", "Wraith character double XP multiplier");
	GameplayTags.State_Wraith_XP_Triple = AddTag(Manager, "State.Wraith.XP.Triple", "Wraith character triple XP multiplier");
	// ==========================================
	// Attributes - 属性标签 (Primary)
	// ==========================================
	GameplayTags.Attribute_Primary_Health = AddTag(Manager, "Attribute.Primary.Health", "生命值");
	GameplayTags.Attribute_Primary_Mana = AddTag(Manager, "Attribute.Primary.Mana", "魔法值");
	GameplayTags.Attribute_Primary_MaxHealth = AddTag(Manager, "Attribute.Primary.MaxHealth", "最大生命值");
	GameplayTags.Attribute_Primary_MaxMana = AddTag(Manager, "Attribute.Primary.MaxMana", "最大魔法值");
	GameplayTags.Attribute_Primary_Level = AddTag(Manager, "Attribute.Primary.Level", "等级");
	GameplayTags.Attribute_Primary_XP = AddTag(Manager, "Attribute.Primary.XP", "经验值");

	// ==========================================
	// Attributes - 属性标签 (Secondary)
	// ==========================================
	GameplayTags.Attribute_Secondary_AttackPower = AddTag(Manager, "Attribute.Secondary.AttackPower", "攻击力");
	GameplayTags.Attribute_Secondary_Defense = AddTag(Manager, "Attribute.Secondary.Defense", "防御力");
	GameplayTags.Attribute_Secondary_DamageBonus = AddTag(Manager, "Attribute.Secondary.DamageBonus", "伤害加成");
	GameplayTags.Attribute_Secondary_DamageResilience = AddTag(Manager, "Attribute.Secondary.DamageResilience", "伤害抗性");
	GameplayTags.Attribute_Secondary_CriticalHitChance = AddTag(Manager, "Attribute.Secondary.CriticalHitChance", "暴击率");
	GameplayTags.Attribute_Secondary_CriticalHitDamage = AddTag(Manager, "Attribute.Secondary.CriticalHitDamage", "暴击伤害");
	GameplayTags.Attribute_Secondary_DamagePenetration = AddTag(Manager, "Attribute.Secondary.DamagePenetration", "伤害穿透");

	GameplayTags.Attribute_Meta_IncomingDamage = AddTag(Manager, "Attribute.Meta.IncomingDamage", "Incoming damage meta attribute");
	GameplayTags.Attribute_Meta_IncomingXP = AddTag(Manager, "Attribute.Meta.IncomingXP", "Incoming XP meta attribute");
	// ==========================================
	// SetByCaller 数据标签（统一 Caller. 前缀）
	// ==========================================
	GameplayTags.Caller_Attribute_Primary_Level = AddTag(Manager, "Caller.Attribute.Primary.Level", "等级（SetByCaller）");
	GameplayTags.Caller_DamageMultiplier = AddTag(Manager, "Caller.DamageMultiplier", "伤害倍率");
	GameplayTags.Caller_Debuff_Stun = AddTag(Manager, "Caller.Debuff.Stun", "眩晕");
	GameplayTags.Caller_Debuff_Data_Chance = AddTag(Manager, "Caller.Debuff.Data.Chance", "Debuff触发概率");
	GameplayTags.Caller_Debuff_Data_Damage = AddTag(Manager, "Caller.Debuff.Data.Damage", "Debuff伤害");
	GameplayTags.Caller_Debuff_Data_Frequency = AddTag(Manager, "Caller.Debuff.Data.Frequency", "Debuff频率");
	GameplayTags.Caller_Debuff_Data_Duration = AddTag(Manager, "Caller.Debuff.Data.Duration", "Debuff持续时间");

	// 初始化 Tag ↔ Attribute 映射
	InitTagToAttributeMap();

	GameplayTags.bIsInitialized = true;
}

void FProjectGameplayTag::InitTagToAttributeMap()
{
	// ---- Primary Attributes ----
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_Health,     UProjectAttributeSet::GetHealthAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_Mana,       UProjectAttributeSet::GetManaAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_MaxHealth,   UProjectAttributeSet::GetMaxHealthAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_MaxMana,     UProjectAttributeSet::GetMaxManaAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_Level,      UProjectAttributeSet::GetLevelAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Primary_XP,         UProjectAttributeSet::GetXPAttribute());

	// ---- Secondary Attributes ----
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_AttackPower,       UProjectAttributeSet::GetAttackPowerAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_Defense,           UProjectAttributeSet::GetDefenseAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_DamageBonus,       UProjectAttributeSet::GetDamageBonusAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_DamageResilience,  UProjectAttributeSet::GetDamageResilienceAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_CriticalHitChance, UProjectAttributeSet::GetCriticalHitChanceAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_CriticalHitDamage, UProjectAttributeSet::GetCriticalHitDamageAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Secondary_DamagePenetration, UProjectAttributeSet::GetDamagePenetrationAttribute());

	// ---- Meta Attributes ----
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Meta_IncomingDamage, UProjectAttributeSet::GetIncomingDamageAttribute());
	TagToAttributeMap.Add(FProjectGameplayTag::Get().Attribute_Meta_IncomingXP, UProjectAttributeSet::GetIncomingXPAttribute());

	// 如果需要 IncomingDamage / IncomingXP 也可在此追加
}

FGameplayAttribute FProjectGameplayTag::GetAttributeByTag(const FGameplayTag InTag)
{
	if (const FGameplayAttribute* FoundAttr = TagToAttributeMap.Find(InTag))
	{
		return *FoundAttr;
	}
	// 返回无效的空 Attribute
	return FGameplayAttribute();
}
