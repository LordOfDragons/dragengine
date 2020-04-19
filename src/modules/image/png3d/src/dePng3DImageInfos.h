/* 
 * Drag[en]gine 3D PNG Image Module
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

#ifndef _DEPNG3DIMAGEINFO_H_
#define _DEPNG3DIMAGEINFO_H_

#include "png.h"

#include <dragengine/systems/modules/image/deBaseImageInfo.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>



/**
 * @brief Image information class.
 */
class dePng3DImageInfo : public deBaseImageInfo{
public:
	decString filename;
	
	int width;
	int height;
	int depth;
	int componentCount;
	int bitCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new info object. */
	dePng3DImageInfo( const char *nfilename );
	/** Cleans up the info object. */
	virtual ~dePng3DImageInfo();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the width in pixels. */
	virtual int GetWidth();
	/** Retrieves the height in pixels. */
	virtual int GetHeight();
	/** Retrieves the depth in pixels. */
	virtual int GetDepth();
	/** Retrieves the number of components. */
	virtual int GetComponentCount();
	/** Retrieves the bit count of each component. */
	virtual int GetBitCount();
	/*@}*/
};

// end of include only once
#endif
