/* 
 * Drag[en]gine IGDE
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

#ifndef _TOOLKIT_H_
#define _TOOLKIT_H_

#include "../../deigde_configuration.h"

#ifdef IGDE_TOOLKIT_FOX
#	include "fox/foxtoolkit.h"
#	include "fox/foxincludenative.h"

#elif defined IGDE_TOOLKIT_BEOS
#	include "beos/beostoolkit.h"
#	include "fox/foxincludenative.h"

#elif defined IGDE_TOOLKIT_NULL
#	include "null/nullincludenative.h"

#endif

#endif
