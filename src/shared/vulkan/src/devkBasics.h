/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEVKBASICS_H_
#define _DEVKBASICS_H_

#include <vulkan/vulkan.h>

class deSharedVulkan;

#ifdef WITH_DEBUG
#define VK_CHECKCOMMANDS 1
#endif

#ifdef VK_CHECKCOMMANDS
	void devkDebugCheckCommand(VkResult result, deSharedVulkan &vulkan, const char *file, int line);
	#define VK_CHECK(vulkan,cmd) devkDebugCheckCommand(cmd, vulkan, __FILE__, __LINE__)
	#define VK_IF_CHECK(cmd) cmd
	
#else
	#define VK_CHECK(renderThread,cmd) cmd
	#define VK_IF_CHECK(cmd)
#endif

#endif
