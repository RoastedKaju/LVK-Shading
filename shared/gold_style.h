#pragma once

#include <imgui/imgui.h>

void SetupImGuiStyle()
{
	// Gold style by CookiePLMonster from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(1.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 2.0f);
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(10.0f, 2.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 12.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(0.91764706f, 0.91764706f, 0.91764706f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4392157f, 0.4392157f, 0.4392157f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10980392f, 0.10980392f, 0.10980392f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10980392f, 0.10980392f, 0.10980392f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20784314f, 0.20784314f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.46666667f, 0.46666667f, 0.46666667f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80784315f, 0.827451f, 0.80784315f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92941177f, 0.64705884f, 0.13725491f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.20784314f, 0.20784314f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.20784314f, 0.20784314f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.50980395f, 0.35686275f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.9098039f, 0.6392157f, 0.12941177f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.7764706f, 0.54901963f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667f, 0.09803922f, 0.14901961f, 0.97f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13725491f, 0.25882354f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}