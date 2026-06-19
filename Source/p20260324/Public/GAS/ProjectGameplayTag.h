#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ProjectGameplayTag.generated.h"

struct FGameplayAttribute;



USTRUCT(BlueprintType)
struct FProjectGameplayTag
{
	GENERATED_BODY()

	
	static FProjectGameplayTag& Get() { return GameplayTags; }
	static void InitGameplayTags();
	bool bIsInitialized = false;


	// ==========================================
	// 鼠标按键 (Mouse)
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mouse")
	FGameplayTag Input_Mouse_LMB;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mouse")
	FGameplayTag Input_Mouse_RMB;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mouse")
	FGameplayTag Input_Mouse_MMB;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mouse")
	FGameplayTag Input_Mouse_X1;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Mouse")
	FGameplayTag Input_Mouse_X2;

	// ==========================================
	// 键盘 - 功能键与修饰键
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Escape;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Tab;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_CapsLock;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_LeftShift;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_RightShift;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_LeftControl;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_RightControl;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_LeftAlt;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_RightAlt;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Space;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Modifier_Shift;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Modifier_Control;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Modifier_Alt;

	// ==========================================
	// 键盘 - 数字键
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_1;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_2;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_3;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_4;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_5;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_6;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_7;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_8;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_9;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_0;

	// ==========================================
	// 键盘 - 字母键
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Q;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_W;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_E;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_R;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_T;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Y;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_U;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_I;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_O;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_P;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_A;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_S;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_D;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_F;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_G;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_H;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_J;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_K;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_L;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_Z;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_X;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_C;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_V;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_B;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_N;
	UPROPERTY(EditDefaultsOnly, Category = "Input|Keyboard")
	FGameplayTag Input_Keyboard_M;

	// ==========================================
	// 键盘 - 系统键与符号
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Tilde;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Minus;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Equals;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_LeftBracket;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_RightBracket;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Backslash;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Semicolon;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Quote;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Comma;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Period;
	UPROPERTY(EditDefaultsOnly, Category = "Input|System")
	FGameplayTag Input_Keyboard_Slash;

	// ==========================================
	// Ability - 技能
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo2;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo3;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Attack_NormalAttack_Combo4;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Skill_Dash;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Attack_HeavyAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Offensive_Crouch_Roll;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag Ability_Passive;

	// ==========================================
	// Ability Judge
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Judge")
	FGameplayTag Ability_Judge_Crouch_Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Judge")
	FGameplayTag Ability_Judge_Crouch_Skill;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Judge")
	FGameplayTag Ability_Judge_Crouch_Roll;

	// ==========================================
	// Ability Effect
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Effect")
	FGameplayTag Ability_Effect_KnockBack;

	// ==========================================
	// Ability Condition
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo2;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo3;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_NormalAttack_Combo4;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Attack_HeavyAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Skill_Dash;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Condition")
	FGameplayTag Ability_Condition_Offensive_Crouch_Roll;

	// ==========================================
	// Block
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Look;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo2;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo3;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_NormalAttack_Combo4;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_Skill_Dash;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_HeavyAttack_Combo1;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FGameplayTag Block_InputAction_Ability_Offensive_Crouch_Roll;

	// ==========================================
	// 角色
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FGameplayTag Character_Crouch;
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FGameplayTag Character_Wraith;
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FGameplayTag Character_Phase;

	// ==========================================
	// 库存标签
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Material;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Consumable;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Special;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Empty;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Material_Experience_Large;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Material_Experience_Medium;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Material_Experience_Small;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Consumable_Crystal_Health;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Consumable_Crystal_Mana;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Consumable_Potion_Health;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FGameplayTag Inventory_Consumable_Potion_Mana;

	// ==========================================
	// Animation
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag Animation_ANS_InputWindow;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FGameplayTag Animation_ANS_PreInputWindow;

	// ==========================================
	// Event
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Event")
	FGameplayTag Event_Crouch_Dash_AddLaunch;

	// ==========================================
	// UI
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Layer_Gameplay;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Layer_Prompt;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_MainAttack;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Attribute;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Setting;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Setting_SettingPanel;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Inventory;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Inventory_Selection_Border;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_Widget_Inventory_Selection_Character;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_WidgetController_MainAttack;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_WidgetController_Attribute;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_WidgetController_Setting;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FGameplayTag UI_WidgetController_Inventory;

	// ==========================================
	// UI Action
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_AddAttributeUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_AddSettingUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_AddInventoryUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_Switch1;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_Switch2;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_MainAttack_Switch3;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_Attribute_RemoveAttributeUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_Setting_RemoveSettingUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_Inventory_RemoveInventoryUI;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_Confirm;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_Cancel;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_PreviousTab;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_NextTab;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Open;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Close;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Reduce;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Add;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Switch;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Use;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_ContinueGame;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Mappable;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_DefaultSetting;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_SaveSetting;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Setting;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Switch1;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Switch2;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_Switch3;
	UPROPERTY(EditDefaultsOnly, Category = "UI|Action")
	FGameplayTag UI_Action_InteractButton_QuitGame;

	// ==========================================
	// State
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FGameplayTag State_Crouch_XP_Double;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FGameplayTag State_Crouch_XP_Triple;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FGameplayTag State_Wraith_XP_Double;
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FGameplayTag State_Wraith_XP_Triple;

	// ==========================================
	// Setting
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	FGameplayTag Setting_Sound;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	FGameplayTag Setting_MappableKey;
	UPROPERTY(EditDefaultsOnly, Category = "Setting")
	FGameplayTag Setting_Interface;

	// ==========================================
	// Mappable
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Attribute_CloseAttributeUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Attribute_OpenAttributeUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Inventory_CloseInventoryUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Inventory_OpenInventoryUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Setting_CloseSettingUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Setting_OpenSettingUI;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Switch1;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Switch2;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Switch3;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Roll;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Skill;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_MoveForward;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_MoveBackward;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_MoveLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Mappable")
	FGameplayTag Mappable_MoveRight;

	// ==========================================
	// MappableGroup
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_Default;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_Primary;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_Skill_Crouch;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_Attribute;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_Inventory;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_MainAttack;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_Setting;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_CommonUI;
	UPROPERTY(EditDefaultsOnly, Category = "MappableGroup")
	FGameplayTag MappableGroup_UI_CommonButton;

	// ==========================================
	// Attributes
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_Mana;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_MaxHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_MaxMana;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_Level;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Primary")
	FGameplayTag Attribute_Primary_XP;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_AttackPower;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_Defense;
	UPROPERTY(EditDefaultsOnly, Category = "UPROPERTY")
	FGameplayTag Attribute_Secondary_DamageBonus;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_DamageResilience;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Secondary")
	FGameplayTag Attribute_Secondary_DamagePenetration;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Meta")
	FGameplayTag Attribute_Meta_IncomingDamage;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Meta")
	FGameplayTag Attribute_Meta_IncomingXP;

	// ==========================================
	// SetByCaller
	// ==========================================
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Attribute_Primary_Level;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_DamageMultiplier;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Debuff_Stun;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Debuff_Data_Chance;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Debuff_Data_Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Debuff_Data_Frequency;
	UPROPERTY(EditDefaultsOnly, Category = "Caller")
	FGameplayTag Caller_Debuff_Data_Duration;

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
