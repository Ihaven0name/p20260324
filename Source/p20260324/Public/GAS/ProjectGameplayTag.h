#pragma once
#include "GameplayTagContainer.h"

struct FProjectGameplayTag
{
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

private:
	static FProjectGameplayTag GameplayTags;
};
