/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Vita implementations of the SDL3 functions the shared code still calls.
 * SDL3 itself is not available on Vita. These definitions include the real
 * SDL3 headers, so the compiler checks every signature against them.
 * Cursor, clipboard and message box are stubs for the first build.
 */

#include <SDL3/SDL.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C"
{

Uint64 SDL_GetTicks(void)
{
	return sceKernelGetProcessTimeWide() / 1000;
}

Uint64 SDL_GetPerformanceCounter(void)
{
	return sceKernelGetProcessTimeWide();
}

Uint64 SDL_GetPerformanceFrequency(void)
{
	return 1000000; // sceKernelGetProcessTimeWide counts microseconds
}

void SDL_DelayNS(Uint64 ns)
{
	sceKernelDelayThread(static_cast<SceUInt>(ns / 1000));
}

void SDL_Quit(void)
{
}

void SDL_free(void* mem)
{
	free(mem);
}

size_t SDL_strlcpy(char* dst, const char* src, size_t maxlen)
{
	const size_t srcLen = strlen(src);
	if (maxlen != 0)
	{
		const size_t copy = (srcLen >= maxlen) ? maxlen - 1 : srcLen;
		memcpy(dst, src, copy);
		dst[copy] = '\0';
	}
	return srcLen;
}

void SDL_LogMessage(int, SDL_LogPriority, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
}

bool SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags, const char* title, const char* message, SDL_Window*)
{
	printf("[%s] %s\n", title ? title : "", message ? message : "");
	return true;
}

const char* SDL_GetBasePath(void)
{
	return "app0:/";
}

char* SDL_GetPrefPath(const char*, const char*)
{
	char* path = static_cast<char*>(malloc(sizeof("ux0:data/PvZ/")));
	if (path)
		strcpy(path, "ux0:data/PvZ/");
	return path;
}

char* SDL_GetClipboardText(void)
{
	char* text = static_cast<char*>(malloc(1));
	if (text)
		text[0] = '\0';
	return text;
}

bool SDL_SetClipboardText(const char*)
{
	return false;
}

// Cursors do not exist on Vita; every call succeeds and returns a placeholder.
bool SDL_ShowCursor(void)
{
	return true;
}

bool SDL_HideCursor(void)
{
	return true;
}

bool SDL_SetCursor(SDL_Cursor*)
{
	return true;
}

SDL_Cursor* SDL_CreateSystemCursor(SDL_SystemCursor)
{
	return nullptr;
}

SDL_Cursor* SDL_CreateColorCursor(SDL_Surface*, int, int)
{
	return nullptr;
}

SDL_Cursor* SDL_GetDefaultCursor(void)
{
	return nullptr;
}

void SDL_DestroyCursor(SDL_Cursor*)
{
}

SDL_Surface* SDL_CreateSurfaceFrom(int, int, SDL_PixelFormat, void*, int)
{
	return nullptr;
}

void SDL_DestroySurface(SDL_Surface*)
{
}

// Passthrough: the game only converts text that is already UTF-8 on Vita.
char* SDL_iconv_string(const char*, const char*, const char* inbuf, size_t inbytesleft)
{
	char* out = static_cast<char*>(malloc(inbytesleft + 1));
	if (!out)
		return nullptr;
	memcpy(out, inbuf, inbytesleft);
	out[inbytesleft] = '\0';
	return out;
}

}
