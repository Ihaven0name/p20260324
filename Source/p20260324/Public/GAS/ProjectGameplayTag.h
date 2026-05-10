#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProjectGameplayTag.generated.h"

struct FGameplayAttribute;

USTRUCT(BlueprintType)
struct FProjectGameplayTag
{
	GENERATED_BODY()
	
public:
	static FProjectGameplayTag& Get() { return GameplayTags; }
	static void InitGameplayTags();
	bool bIsInitialized = false;

	// ==========================================
	// 鼠标按键 (Mouse)
	// ==========================================
	FGameplayTag Input_Mouse_LMB;       // 左键
	FGameplayTag Input_Mouse_RMB;       // 右键
	FGameplayTag Input_Mouse_MMB;       // 中键
	FGameplayTag Input_Mouse_X1;        // 侧键1 (后退)
	FGameplayTag Input_Mouse_X2;        // 侧键2 (前进)

	// ==========================================
	// 键盘 - 功能键与修饰键 (Function & Modifiers)
	// ==========================================
	FGameplayTag Input_Keyboard_Escape;
	FGameplayTag Input_Keyboard_Tab;
	FGameplayTag Input_Keyboard_CapsLock;
	
	FGameplayTag Input_Keyboard_LeftShift;
	FGameplayTag Input_Keyboard_RightShift;
	
	FGameplayTag Input_Keyboard_LeftControl;
	FGameplayTag Input_Keyboard_RightControl;
	
	FGameplayTag Input_Keyboard_LeftAlt;
	FGameplayTag Input_Keyboard_RightAlt;
	
	FGameplayTag Input_Keyboard_Space;     // 空格

	// ==========================================
	// 键盘 - 通用修饰键 (Common Modifiers)
	// ==========================================
	FGameplayTag Input_Keyboard_Modifier_Shift;
	FGameplayTag Input_Keyboard_Modifier_Control;
	FGameplayTag Input_Keyboard_Modifier_Alt;

	// ==========================================
	// 键盘 - 数字键 (Number Row 0-9)
	// ==========================================
	FGameplayTag Input_Keyboard_1;
	FGameplayTag Input_Keyboard_2;
	FGameplayTag Input_Keyboard_3;
	FGameplayTag Input_Keyboard_4;
	FGameplayTag Input_Keyboard_5;
	FGameplayTag Input_Keyboard_6;
	FGameplayTag Input_Keyboard_7;
	FGameplayTag Input_Keyboard_8;
	FGameplayTag Input_Keyboard_9;
	FGameplayTag Input_Keyboard_0;

	// ==========================================
	// 键盘 - 字母键 (A-Z)
	// ==========================================
	// Row 1
	FGameplayTag Input_Keyboard_Q;
	FGameplayTag Input_Keyboard_W;
	FGameplayTag Input_Keyboard_E;
	FGameplayTag Input_Keyboard_R;
	FGameplayTag Input_Keyboard_T;
	FGameplayTag Input_Keyboard_Y;
	FGameplayTag Input_Keyboard_U;
	FGameplayTag Input_Keyboard_I;
	FGameplayTag Input_Keyboard_O;
	FGameplayTag Input_Keyboard_P;

	// Row 2
	FGameplayTag Input_Keyboard_A;
	FGameplayTag Input_Keyboard_S;
	FGameplayTag Input_Keyboard_D;
	FGameplayTag Input_Keyboard_F;
	FGameplayTag Input_Keyboard_G;
	FGameplayTag Input_Keyboard_H;
	FGameplayTag Input_Keyboard_J;
	FGameplayTag Input_Keyboard_K;
	FGameplayTag Input_Keyboard_L;

	// Row 3
	FGameplayTag Input_Keyboard_Z;
	FGameplayTag Input_Keyboard_X;
	FGameplayTag Input_Keyboard_C;
	FGameplayTag Input_Keyboard_V;
	FGameplayTag Input_Keyboard_B;
	FGameplayTag Input_Keyboard_N;
	FGameplayTag Input_Keyboard_M;

	// ==========================================
	// 键盘 - 系统键与符号 (System & Punctuation)
	// ==========================================
	FGameplayTag Input_Keyboard_Tilde;        // ~
	FGameplayTag Input_Keyboard_Minus;        // -
	FGameplayTag Input_Keyboard_Equals;       // =
	FGameplayTag Input_Keyboard_LeftBracket;  // [
	FGameplayTag Input_Keyboard_RightBracket; // ]
	FGameplayTag Input_Keyboard_Backslash;    // \键
	FGameplayTag Input_Keyboard_Semicolon;    // ;
	FGameplayTag Input_Keyboard_Quote;        // '
	FGameplayTag Input_Keyboard_Comma;        // ,
	FGameplayTag Input_Keyboard_Period;       // .
	FGameplayTag Input_Keyboard_Slash;        // /

	// ==========================================
	// Ability - 技能标签
	// ==========================================
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo1;   // 蹲下普通攻击连段1
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo2;   // 蹲下普通攻击连段2
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo3;   // 蹲下普通攻击连段3
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo4;   // 蹲下普通攻击连段4
	FGameplayTag Ability_Offensive_Crouch_Skill_Dash;             // 蹲下技能-冲刺
	FGameplayTag Ability_Offensive_Crouch_Attack_HeavyAttack_Combo1;	//重攻击
	FGameplayTag Ability_Offensive_Crouch_Roll;

	FGameplayTag Ability_Passive;                                // 被动技能（父标签，用于区分主动/被动）

	// ==========================================
	// Ability Judge - 预输入判定标签
	// 每个 InputAction 对应一个 Judge Tag，用于绑定输入并激活 Judge Ability
	// ==========================================
	FGameplayTag Ability_Judge_Crouch_Attack;                     // 攻击判定
	FGameplayTag Ability_Judge_Crouch_Skill;                // 冲刺技能判定
	FGameplayTag Ability_Judge_Crouch_Roll;                      // 翻滚判定

	// ==========================================
	// Ability Effect - 技能效果/状态效果标签
	// ==========================================
	FGameplayTag Ability_Effect_KnockBack;                       // 击退效果

	// ==========================================
	// Ability Condition - 技能条件标签（用于判断是否可以进行下一步）
	// ==========================================
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo1;   // 蹲下普通攻击连段1条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo2;   // 蹲下普通攻击连段2条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo3;   // 蹲下普通攻击连段3条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo4;   // 蹲下普通攻击连段4条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_HeavyAttack_Combo1;   //重攻击连段1条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Skill_Dash;           // 蹲下技能-冲刺条件
	FGameplayTag Ability_Condition_Offensive_Crouch_Roll;
	

	// ==========================================
	// Block - 输入屏蔽标签
	// ==========================================
	FGameplayTag Block_InputAction_Move;
	FGameplayTag Block_InputAction_Look;
	FGameplayTag Block_InputAction_SpringArm;
	FGameplayTag Block_InputAction_Ability;
	FGameplayTag Block_InputAction_Jump;

	// Block - 技能互斥屏蔽（每个Ability对应一个Block Tag）
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo1;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo2;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo3;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo4;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_Skill_Dash;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_HeavyAttack_Combo1;
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_Roll;

	// ==========================================
	//  - 动画标签
	// ==========================================
	FGameplayTag Inventory_Material_Experience_Large;
	FGameplayTag Inventory_Material_Experience_Medium;
	FGameplayTag Inventory_Material_Experience_Small;
	
	// ==========================================
	// Animation - 动画标签
	// ==========================================
	FGameplayTag Animation_ANS_InputWindow;        // 输入窗口（动画通知状态）
	FGameplayTag Animation_ANS_PreInputWindow;     // 预输入窗口（动画通知状态）

	// ==========================================
	// Event - 动画发送事件标签（用于 AnimNotify → GAS WaitGameplayEvent）
	// ==========================================
	FGameplayTag Event_Crouch_Dash_AddLaunch;        // 蹲下冲刺-蓄力循环结束事件

	// ==========================================
	// UI - 界面标签
	// ==========================================
	FGameplayTag UI_Layer_Gameplay;
	FGameplayTag UI_Layer_Prompt;
	
	FGameplayTag UI_Widget_MainAttack;
	FGameplayTag UI_Widget_Attribute;
	FGameplayTag UI_Widget_Setting;

	
	// ==========================================
	// UI Action - 每个Common Activatable Widget需要对应的Action
	// ==========================================
	FGameplayTag UI_Action_MainAttack_AddAttributeUI;
	FGameplayTag UI_Action_MainAttack_AddSettingUI;
	FGameplayTag UI_Action_Attribute_RemoveAttributeUI;
	FGameplayTag UI_Action_Confirm;
	FGameplayTag UI_Action_Cancel;
	FGameplayTag UI_Action_PreviousTab;
	FGameplayTag UI_Action_NextTab;
	
	// ==========================================
	// State - 玩家状态标签（比如某个角色升级享有翻倍效果）
	// ==========================================

	FGameplayTag State_Crouch_XP_Double;
	FGameplayTag State_Crouch_XP_Triple;
	FGameplayTag State_Wraith_XP_Double;
	FGameplayTag State_Wraith_XP_Triple;
	
	// ==========================================
	// Attributes - 属性标签
	// ==========================================
	
	// -------- Primary Attributes --------
	FGameplayTag Attribute_Primary_Health;
	FGameplayTag Attribute_Primary_Mana;
	FGameplayTag Attribute_Primary_MaxHealth;
	FGameplayTag Attribute_Primary_MaxMana;
	FGameplayTag Attribute_Primary_Level;
	FGameplayTag Attribute_Primary_XP;

	
	// -------- Secondary Attributes --------
	FGameplayTag Attribute_Secondary_AttackPower;
	FGameplayTag Attribute_Secondary_Defense;
	FGameplayTag Attribute_Secondary_DamageBonus;
	FGameplayTag Attribute_Secondary_DamageResilience;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_DamagePenetration;

	// -------- Meta Attributes -------------
	FGameplayTag Attribute_Meta_IncomingDamage;
	FGameplayTag Attribute_Meta_IncomingXP;

	// ==========================================
	// SetByCaller 数据标签（统一 Caller. 前缀）
	// ==========================================
	FGameplayTag Caller_Attribute_Primary_Level;  // 等级（SetByCaller 传入 GE）
	FGameplayTag Caller_DamageMultiplier;          // 伤害倍率
	FGameplayTag Caller_Debuff_Stun;               // 眩晕 Debuff
	FGameplayTag Caller_Debuff_Data_Chance;        // Debuff 触发概率
	FGameplayTag Caller_Debuff_Data_Damage;        // Debuff 伤害
	FGameplayTag Caller_Debuff_Data_Frequency;     // Debuff 频率/间隔
	FGameplayTag Caller_Debuff_Data_Duration;      // Debuff 持续时间

	// ==========================================
	// TagToAttribute Map - AttributeTag ↔ FGameplayAttribute 一一映射
	// 通过 Attribute Tag 快速查找对应的 FGameplayAttribute
	// ==========================================
	static TMap<FGameplayTag, FGameplayAttribute> TagToAttributeMap;

	/** 根据 AttributeTag 获取 FGameplayAttribute，未找到返回无效的 FGameplayAttribute */
	static FGameplayAttribute GetAttributeByTag(const FGameplayTag InTag);

	/** 初始化 TagToAttribute 映射（在 InitGameplayTags 末尾调用） */
	static void InitTagToAttributeMap();

private:
	static FProjectGameplayTag GameplayTags;
};
