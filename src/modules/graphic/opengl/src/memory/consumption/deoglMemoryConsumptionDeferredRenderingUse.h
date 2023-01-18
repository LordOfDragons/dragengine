/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLMEMORYCONSUMPTIONDEFERREDRENDERINGUSE_H_
#define _DEOGLMEMORYCONSUMPTIONDEFERREDRENDERINGUSE_H_

#include "deoglMemoryConsumptionGPUUse.h"

#include <dragengine/common/math/decMath.h>


class deoglMemoryConsumptionDeferredRendering;


/**
 * DeferredRendering memory consumption usage.
 */
class deoglMemoryConsumptionDeferredRenderingUse{
public:
	deoglMemoryConsumptionGPUUse texture;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption usage. */
	deoglMemoryConsumptionDeferredRenderingUse( deoglMemoryConsumptionDeferredRendering &tracker );
	
	/** Clean up vbo memory consumption. */
	~deoglMemoryConsumptionDeferredRenderingUse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total consumption. */
	unsigned long long Total() const;
	
	/** Total consumption. */
	unsigned int TotalMB() const;
	
	/** Clear all consumptions. */
	void Clear();
	/*@}*/
};

#endif
