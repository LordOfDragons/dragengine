/* 
 * Drag[en]gine X System Input Module
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

#ifndef _DEXSI_XINCLUDE_H_
#define _DEXSI_XINCLUDE_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

// undefined some crap macros defined by XInput.h
#undef Absolute

#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>

#endif
