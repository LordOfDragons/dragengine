/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEVKBASICS_H_
#define _DEVKBASICS_H_

#include <vulkan/vulkan.h>

class deSharedVulkan;

#ifdef WITH_DEBUG
#define VK_CHECKCOMMANDS 1
#endif

#ifdef VK_CHECKCOMMANDS
	void devkDebugCheckCommand( VkResult result, deSharedVulkan &vulkan, const char *file, int line );
	#define VK_CHECK(vulkan,cmd) devkDebugCheckCommand( cmd, vulkan, __FILE__, __LINE__ )
	#define VK_IF_CHECK(cmd) cmd
	
#else
	#define VK_CHECK(renderThread,cmd) cmd
	#define VK_IF_CHECK(cmd)
#endif

#endif
