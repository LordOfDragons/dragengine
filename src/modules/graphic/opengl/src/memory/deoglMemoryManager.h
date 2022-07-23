/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLMEMORYMANAGER_H_
#define _DEOGLMEMORYMANAGER_H_

#include "consumption/deoglMemoryConsumption.h"

#include <dragengine/common/math/decMath.h>



/**
 * Keeps track of the current memory consumption.
 */
class deoglMemoryManager{
private:
	deoglMemoryConsumption pConsumption;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory manager. */
	deoglMemoryManager();
	
	/** Clean up memory manager. */
	~deoglMemoryManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Memory consumption. */
	inline deoglMemoryConsumption &GetConsumption(){ return pConsumption; }
	/*@}*/
};

#endif
