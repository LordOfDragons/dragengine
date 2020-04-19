/* 
 * Drag[en]gine GUI Launcher
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

#include "../glconfig.h"

#ifdef PI
#define FIX_FOX_BROKEN_PI
#undef PI
#endif

#include <fox-1.7/fx.h>
#include <fox-1.7/fx3d.h>
#include <fox-1.7/fxkeys.h>
#include <fox-1.7/FXPNGIcon.h>

#ifdef FIX_FOX_BROKEN_PI
#include <dragengine/common/math/decMath.h>
#endif

#define ICONLIST_HEADER_ID FXIconList::ID_HEADER

// anti-windows macros
#undef GetClassName
#undef GetObject
#undef LoadImage

#endif // _FOXTOOLKIT_H_
