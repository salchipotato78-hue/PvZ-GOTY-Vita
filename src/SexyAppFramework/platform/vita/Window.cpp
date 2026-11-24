/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * PS Vita window layer (vitaGL). Mirrors platform/switch/Window.cpp.
 * UNTESTED: first draft, pending compilation against VitaSDK.
 */
#include "VitaGLShim.h"

#include "SexyAppBase.h"
#include "graphics/GLInterface.h"
#include "graphics/GLImage.h"
#include "widget/WidgetManager.h"

using namespace Sexy;

void SexyAppBase::MakeWindow()
{
	// Vita native resolution. The game renders 4:3 letterboxed inside it.
	if (mGLInterface == nullptr)
	{
		vglInitExtended(0x1000000, 960, 544, 0x1000000, SCE_GXM_MULTISAMPLE_NONE);
		vglWaitVblankStart(GL_TRUE); // vsync -> 60 Hz pacing

		mWindow = reinterpret_cast<void*>(1); // non-null marker, no window handle on Vita
		mSurface = nullptr;

		mGLInterface = std::make_unique<GLInterface>(this);
		if (!InitGLInterface())
		{
			mGLInterface = nullptr;
			return;
		}

		mGLInterface->UpdateViewport();
		mWidgetManager->Resize(mScreenBounds, mGLInterface->mPresentationRect);
	}

	bool isActive = mActive;
	mActive = true;
	mPhysMinimized = false;
	if (isActive != mActive)
		RehupFocus();

	ReInitImages();
	mWidgetManager->mImage = mGLInterface->GetScreenImage();
	mWidgetManager->MarkAllDirty();
}
