/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRUTILS_H_
#define _DEOXRUTILS_H_

#include "deoxrBasics.h"

#include <dragengine/common/math/decMath.h>



/**
 * Oxr utils.
 * 
 * \note Coordinate Systems.
 * 
 * OpenXR uses X=right, Y=up and Z=backwards and is a right handed system.
 */
class deoxrUtils{
private:
	deoxrUtils();
	~deoxrUtils();
	
public:
	/** \name Utils */
	/*@{*/
	/** Convert XrVector3f to decVector. */
	static decVector Convert( const XrVector3f &vector );
	
	/** Convert XrQuaternionf to decQuaternion. */
	static decQuaternion Convert( const XrQuaternionf &quaternion );
	
	/** Convert XrPosef to decMatrix. */
	static decMatrix Convert( const XrPosef &pose );
};

#endif

