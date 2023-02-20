/* 
 * Drag[en]gine Basic Crash Recovery Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _FOXTOOLKIT_H_
#define _FOXTOOLKIT_H_

#include <dragengine/dragengine_configuration.h>

#ifdef PI
#define FIX_FOX_BROKEN_PI
#undef PI
#endif

#ifdef OS_W32_VS
	// disable warning about "class X needs to have dll-interface to be used by clients of class Y"
	#pragma warning( disable: 4251 )
#endif

#include <fox-1.7/fx.h>
#include <fox-1.7/fx3d.h>

#ifdef FIX_FOX_BROKEN_PI
#include <dragengine/common/math/decMath.h>
#endif

#endif
