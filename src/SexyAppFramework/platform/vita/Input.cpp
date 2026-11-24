/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * PS Vita input layer (sceCtrl + sceTouch). Mirrors platform/switch/Input.cpp.
 * UNTESTED: first draft, pending compilation against VitaSDK.
 */
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <unordered_map>

#include "SexyAppBase.h"
#include "graphics/GLInterface.h"
#include "graphics/GLImage.h"
#include "widget/WidgetManager.h"

using namespace Sexy;

// Vita button bit -> framework key
static const std::unordered_map<unsigned int, KeyCode> sKeyMaps = {
	{SCE_CTRL_START,    KEYCODE_ESCAPE},
	{SCE_CTRL_SELECT,   KEYCODE_SPACE},
	{SCE_CTRL_CROSS,    KEYCODE_RETURN},
	{SCE_CTRL_L1,       KEYCODE_LBUTTON},
	{SCE_CTRL_R1,       KEYCODE_RBUTTON},
};

static unsigned int sPrevButtons = 0;
static int sPrevTouchCount = 0;

void SexyAppBase::InitInput()
{
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	if (!mMouseIn)
		mMouseIn = true;
}

bool SexyAppBase::StartTextInput([[maybe_unused]] std::string& theInput)
{
	// TODO: sceImeDialog for text entry
	return false;
}

void SexyAppBase::StopTextInput() {}
void SexyAppBase::SetTextInputRect([[maybe_unused]] const Rect& theRect) {}

bool SexyAppBase::ProcessDeferredMessages([[maybe_unused]] bool singleMessage)
{
	// Buttons: edge detection on the held-state bitmask
	SceCtrlData pad;
	sceCtrlPeekBufferPositive(0, &pad, 1);
	unsigned int down = pad.buttons & ~sPrevButtons;
	unsigned int up   = ~pad.buttons & sPrevButtons;
	sPrevButtons = pad.buttons;

	if (down || up)
		mLastUserInputTick = mLastTimerTime;

	for (auto& k : sKeyMaps)
	{
		if (down & k.first) mWidgetManager->KeyDown(k.second);
		if (up & k.first)   mWidgetManager->KeyUp(k.second);
	}

	// Touch: front panel is 1920x1088; the game surface is 960x544
	SceTouchData touch;
	sceTouchPeek(SCE_TOUCH_PORT_FRONT, &touch, 1);
	static int x = 0, y = 0;
	if (touch.reportNum > 0)
	{
		mLastUserInputTick = mLastTimerTime;
		x = touch.report[0].x / 2;
		y = touch.report[0].y / 2;
		mWidgetManager->RemapMouse(x, y);
		mWidgetManager->MouseMove(x, y);
	}
	if (touch.reportNum && !sPrevTouchCount)
		mWidgetManager->MouseDown(x, y, 1);
	else if (!touch.reportNum && sPrevTouchCount)
		mWidgetManager->MouseUp(x, y, 1);
	sPrevTouchCount = touch.reportNum;

	return false;
}
