#include "GAS/ProjectGameplayTag.h"
#include "GameplayTagsManager.h"

FProjectGameplayTag FProjectGameplayTag::GameplayTags;

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

	GameplayTags.bIsInitialized = true;
}