/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 * Minimal declarations of the vitaGL entry points PvZ uses on PS Vita.
 * vitaGL's own GL headers are NOT included: they redefine the Khronos/GL types
 * that glad/gles2.h defines, which breaks compilation. GL calls resolve through
 * glad, which loads them with vglGetProcAddress.
 * Signatures are from memory: verify against the installed vitaGL.
 */

#ifndef __VITAGL_SHIM_H__
#define __VITAGL_SHIM_H__

#include <cstdint>
#include <psp2/gxm.h>

extern "C"
{
unsigned char vglInitExtended(uint32_t legacy_pool_size, uint32_t width, uint32_t height,
                              uint32_t ram_threshold, SceGxmMultisampleMode msaa);
void vglWaitVblankStart(unsigned char enable);
void vglSwapBuffers(unsigned char has_commondialog);
void* vglGetProcAddress(const char* symbol);
}

#endif //__VITAGL_SHIM_H__
