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

#include <dragengine/common/math/decMath.h>

#include "deoglMemoryConsumptionDeferredRendering.h"
#include "deoglMemoryConsumptionDeferredRenderingUse.h"


// Class deoglMemoryConsumptionDeferredRenderingUse
/////////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglMemoryConsumptionDeferredRenderingUse::deoglMemoryConsumptionDeferredRenderingUse(
	deoglMemoryConsumptionDeferredRendering &tracker ) :
texture( tracker.texture ){
}

deoglMemoryConsumptionDeferredRenderingUse::~deoglMemoryConsumptionDeferredRenderingUse(){
}



// Management
///////////////

unsigned long long deoglMemoryConsumptionDeferredRenderingUse::Total() const{
	return texture.GetConsumption();
}

unsigned int deoglMemoryConsumptionDeferredRenderingUse::TotalMB() const{
	return ( unsigned int )( Total() / 1024000ull );
}

void deoglMemoryConsumptionDeferredRenderingUse::Clear(){
	texture = 0;
}
