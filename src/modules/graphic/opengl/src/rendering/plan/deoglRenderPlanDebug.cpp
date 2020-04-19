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

#include <stdio.h>
#include <string.h>

#include "deoglRenderPlanDebug.h"



// Class deoglRenderPlanDebug
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanDebug::deoglRenderPlanDebug(){
	Reset();
}

deoglRenderPlanDebug::~deoglRenderPlanDebug(){
}



// Management
///////////////

void deoglRenderPlanDebug::Reset(){
	pViewObjects = 0;
	pViewTriangles = 0;
	pViewLights = 0;
	pViewEnvMaps = 0;
	
	pLODTriangles = 0;
	
	pCullPSObjects = 0;
	pCullPSTriangles = 0;
	pCullPSStrips = 0;
	pCullPSLights = 0;
	
	pOccMapObjects = 0;
	pOccMapTriangles = 0;
	pOccMapRenderTime = 0.0f;
	pOccTestTime = 0.0f;
	pOccTestCount = 0;
	
	pRenderedPasses = 0;
	pRenderedObjects = 0;
	pRenderedTriangles = 0;
	pRenderedLights = 0;
	pRenderedEnvMaps = 0;
	
	pSolidObjects = 0;
	pSolidTriangles = 0;
	
	pTransparentPasses = 0;
	pTransparentObjects = 0;
	pTransparentTriangles = 0;
}
